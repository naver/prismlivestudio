#include "browser-panel-internal.hpp"
#include "browser-panel-client.hpp"
#include "cef-headers.hpp"
#include "browser-app.hpp"

#include <QWindow>
#include <QApplication>

#ifdef USE_QT_LOOP
#include <QEventLoop>
#include <QThread>
#endif

#include <obs-module.h>
#include <util/threading.h>
#include <util/base.h>
#include <thread>
#include <algorithm>
#include <qeventloop.h>
#include <qevent.h>

extern bool QueueCEFTask(std::function<void()> task);
extern "C" void obs_browser_initialize(void);
extern os_event_t *cef_started_event;

std::mutex popup_whitelist_mutex;
std::vector<PopupWhitelistInfo> popup_whitelist;
std::vector<PopupWhitelistInfo> forced_popups;

std::list<QCefWidgetInternal *> g_lstCefWidget;

/* ------------------------------------------------------------------------- */

#if CHROME_VERSION_BUILD < 3770
CefRefPtr<CefCookieManager> QCefRequestContextHandler::GetCookieManager()
{
	return cm;
}
#endif

class CookieCheck : public CefCookieVisitor {
public:
	QCefCookieManager::cookie_exists_cb callback;
	std::string target;
	bool cookie_found = false;

	inline CookieCheck(QCefCookieManager::cookie_exists_cb callback_,
			   const std::string target_)
		: callback(callback_), target(target_)
	{
	}

	virtual ~CookieCheck() { callback(cookie_found); }

	virtual bool Visit(const CefCookie &cookie, int, int, bool &) override
	{
		CefString cef_name = cookie.name.str;
		std::string name = cef_name;

		if (name == target) {
			cookie_found = true;
			return false;
		}
		return true;
	}

	IMPLEMENT_REFCOUNTING(CookieCheck);
};

class CookieRead : public CefCookieVisitor {
public:
	QCefCookieManager::read_cookie_cb cookie_cb;
	void *context;
	bool has_cookie;

	inline CookieRead(QCefCookieManager::read_cookie_cb cookie_cb_,
			  void *context_)
		: cookie_cb(cookie_cb_), context(context_), has_cookie(false)
	{
	}

	virtual ~CookieRead()
	{
		if (!has_cookie) {
			cookie_cb(nullptr, nullptr, nullptr, nullptr, true,
				  context);
		}
	}

	virtual bool Visit(const CefCookie &cookie, int index, int total,
			   bool &deleteCookie) override
	{
		has_cookie = true;
		cookie_cb(CefString(cookie.name.str).ToString().c_str(),
			  CefString(cookie.value.str).ToString().c_str(),
			  CefString(cookie.domain.str).ToString().c_str(),
			  CefString(cookie.path.str).ToString().c_str(),
			  (index + 1) == total, context);
		return true;
	}

	IMPLEMENT_REFCOUNTING(CookieRead);
};

struct QCefCookieManagerInternal : QCefCookieManager {
	CefRefPtr<CefCookieManager> cm;
#if CHROME_VERSION_BUILD < 3770
	CefRefPtr<CefRequestContextHandler> rch;
#endif
	CefRefPtr<CefRequestContext> rc;

	QCefCookieManagerInternal(const std::string &storage_path,
				  bool persist_session_cookies)
	{
		if (os_event_try(cef_started_event) != 0)
			throw "Browser thread not initialized";

		BPtr<char> rpath = obs_module_config_path(storage_path.c_str());
		BPtr<char> path = os_get_abs_path_ptr(rpath.Get());

#if CHROME_VERSION_BUILD < 3770
		cm = CefCookieManager::CreateManager(
			path.Get(), persist_session_cookies, nullptr);
		if (!cm)
			throw "Failed to create cookie manager";
#endif

#if CHROME_VERSION_BUILD < 3770
		rch = new QCefRequestContextHandler(cm);

		rc = CefRequestContext::CreateContext(
			CefRequestContext::GetGlobalContext(), rch);
#else
		CefRequestContextSettings settings;
		CefString(&settings.cache_path) = path.Get();
		rc = CefRequestContext::CreateContext(
			settings, CefRefPtr<CefRequestContextHandler>());
		if (rc)
			cm = rc->GetCookieManager(nullptr);

		UNUSED_PARAMETER(persist_session_cookies);
#endif
	}

	virtual bool DeleteCookies(const std::string &url,
				   const std::string &name) override
	{
		return !!cm ? cm->DeleteCookies(url, name, nullptr) : false;
	}

	virtual bool SetStoragePath(const std::string &storage_path,
				    bool persist_session_cookies) override
	{
		BPtr<char> rpath = obs_module_config_path(storage_path.c_str());
		BPtr<char> path = os_get_abs_path_ptr(rpath.Get());

#if CHROME_VERSION_BUILD < 3770
		return cm->SetStoragePath(path.Get(), persist_session_cookies,
					  nullptr);
#else
		CefRequestContextSettings settings;
		CefString(&settings.cache_path) = storage_path;
		rc = CefRequestContext::CreateContext(
			settings, CefRefPtr<CefRequestContextHandler>());
		if (rc)
			cm = rc->GetCookieManager(nullptr);

		UNUSED_PARAMETER(persist_session_cookies);
		return true;
#endif
	}

	virtual bool FlushStore() override
	{
		return !!cm ? cm->FlushStore(nullptr) : false;
	}

	virtual void CheckForCookie(const std::string &site,
				    const std::string &cookie,
				    cookie_exists_cb callback) override
	{
		if (!cm)
			return;

		CefRefPtr<CookieCheck> c = new CookieCheck(callback, cookie);
		cm->VisitUrlCookies(site, true, c);
	}

	virtual void ReadCookies(const std::string &site,
				 read_cookie_cb cookie_cb,
				 void *context) override
	{
		if (!cm)
			return;

		CefRefPtr<CookieRead> c = new CookieRead(cookie_cb, context);
		cm->VisitUrlCookies(site, true, c);
	}
	// OBS Modification:
	// Cheng Bing / 20200513 /
	// Reason: naverTv chat need cookie information to login;cef have not save the cookies;
	// Solution: app init Manual set naverTv cookies into cef
	virtual bool SetCookie(const std::string &url, const std::string &name,
			       const std::string &value,
			       const std::string &domain,
			       const std::string &path, bool isOnlyHttp)
	{
		if (!cm)
			return false;

		CefCookie cookie;
		cef_string_utf8_to_utf16(name.c_str(), name.size(),
					 &cookie.name);
		cef_string_utf8_to_utf16(value.c_str(), value.size(),
					 &cookie.value);
		cef_string_utf8_to_utf16(domain.c_str(), domain.size(),
					 &cookie.domain);
		cef_string_utf8_to_utf16(path.c_str(), path.size(),
					 &cookie.path);

		if (isOnlyHttp) {
			cookie.httponly = true;
		} else {
			cookie.secure = true;
		}

		return cm->SetCookie(url, cookie, nullptr);
	}
	// OBS Modification:
	// Cheng Bing / 20200518/
	// Reason: get cef all cookies infomation

	virtual void visitAllCookies(read_cookie_cb cookie_cb, void *context)
	{
		if (!cm)
			return;
		CefRefPtr<CookieRead> c = new CookieRead(cookie_cb, context);
		cm->VisitAllCookies(c);
	}
};

/* ------------------------------------------------------------------------- */

// OBS Modification:
// Zhang dewen / 20200211 / Related Issue ID=347
// Reason: store request headers
// Solution: modify request headers
QCefWidgetInternal::QCefWidgetInternal(
	QWidget *parent, const std::string &url_, const std::string &script_,
	CefRefPtr<CefRequestContext> rqc_,
	const std::map<std::string, std::string> &headers_)
	: QCefWidget(parent),
	  url(url_),
	  script(script_),
	  rqc(rqc_),
	  headers(headers_)
{
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_StaticContents);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	setAttribute(Qt::WA_NativeWindow);

	setFocusPolicy(Qt::ClickFocus);

	g_lstCefWidget.push_back(this);

	obs_browser_initialize();
	Init();
}

QCefWidgetInternal::~QCefWidgetInternal()
{
	closeBrowser();

	g_lstCefWidget.remove(this);
}

void QCefWidgetInternal::closeBrowser()
{
	auto destroyBrowser = [](CefRefPtr<CefBrowser> cefBrowser) {
		CefRefPtr<CefClient> client =
			cefBrowser->GetHost()->GetClient();
		QCefBrowserClient *bc =
			reinterpret_cast<QCefBrowserClient *>(client.get());

		cefBrowser->GetHost()->WasHidden(true);
		cefBrowser->GetHost()->CloseBrowser(true);

		bc->widget = nullptr;
	};

	CefRefPtr<CefBrowser> browser = cefBrowser;
	if (!!browser) {
#ifdef _WIN32
		/* So you're probably wondering what's going on here.  If you
		 * call CefBrowserHost::CloseBrowser, and it fails to unload
		 * the web page *before* WM_NCDESTROY is called on the browser
		 * HWND, it will call an internal CEF function
		 * CefBrowserPlatformDelegateNativeWin::CloseHostWindow, which
		 * will attempt to close the browser's main window itself.
		 * Problem is, this closes the root window containing the
		 * browser's HWND rather than the browser's specific HWND for
		 * whatever mysterious reason.  If the browser is in a dock
		 * widget, then the window it closes is, unfortunately, the
		 * main program's window, causing the entire program to shut
		 * down.
		 *
		 * So, instead, before closing the browser, we need to decouple
		 * the browser from the widget.  To do this, we hide it, then
		 * remove its parent. */
		HWND hwnd = (HWND)cefBrowser->GetHost()->GetWindowHandle();
		if (hwnd) {
			ShowWindow(hwnd, SW_HIDE);
			SetParent(hwnd, nullptr);
		}
#endif

		ExecuteOnBrowser(destroyBrowser, false);
		cefBrowser = nullptr;
	} else {
		QEventLoop loop;
		auto waitForInit = [this, &loop]() {
			/* WuLongyue/2020-05-19/#2669
			 * Nothing to do,
			 * Only post a task to cef runloop, then wait this task to finish
			 * It's used for waiting previous QCefWidgetInternal::Init() to finish
			 * Because Init() runs on cef thread.
			 * Reason: If likes `QCefWidget *cefWidget = cef->create_widget` on first line,
			 * then `delete cefWidget` on second line immedately,
			 * The `Init` method on cef thread may not be finished, So need to wait it.
			*/
			QMetaObject::invokeMethod(&loop, &QEventLoop::quit);
		};

		if (QueueCEFTask(waitForInit)) {
			loop.exec(QEventLoop::ExcludeUserInputEvents);
		}

		CefRefPtr<CefBrowser> browser = cefBrowser;
		if (!!browser) {
			closeBrowser();
		}
	}
}

void QCefWidgetInternal::Init()
{
	QSize size = this->size() * devicePixelRatio();
	WId id = winId();
	QEventLoop loop;

	bool success = QueueCEFTask([this, size, id, &loop]() {
		CefWindowInfo windowInfo;

		/* Make sure Init isn't called more than once. */
		if (cefBrowser) {
			QMetaObject::invokeMethod(&loop, &QEventLoop::quit);
			return;
		}

#ifdef _WIN32
		RECT rc = {0, 0, size.width(), size.height()};
		windowInfo.SetAsChild((HWND)id, rc);
#elif __APPLE__
		windowInfo.SetAsChild((CefWindowHandle)id, 0, 0, size.width(),
				      size.height());
#endif

		// OBS Modification:
		// Zhang dewen / 20200211 / Related Issue ID=347
		// Reason: penetrate request headers
		// Solution: modify request headers
		CefRefPtr<QCefBrowserClient> browserClient =
			new QCefBrowserClient(this, script, allowAllPopups_,
					      headers);

		CefBrowserSettings cefBrowserSettings;
		cefBrowserSettings.web_security = STATE_DISABLED;
		cefBrowser = CefBrowserHost::CreateBrowserSync(
			windowInfo, browserClient, url, cefBrowserSettings,
#if CHROME_VERSION_BUILD >= 3770
			CefRefPtr<CefDictionaryValue>(),
#endif
			rqc);
#ifdef _WIN32
		Resize(true);
#endif
		QMetaObject::invokeMethod(&loop, &QEventLoop::quit);
	});

	if (success) {
		loop.exec();
	}
	//timer.stop();
}

void QCefWidgetInternal::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	Resize(false);
}

void QCefWidgetInternal::Resize(bool bImmediately)
{
#ifdef _WIN32
	QSize size = this->size() * devicePixelRatio();

	auto func = [this, size]() {
		if (!cefBrowser)
			return;

		HWND hwnd = cefBrowser->GetHost()->GetWindowHandle();
		SetWindowPos(hwnd, nullptr, 0, 0, size.width(), size.height(),
			     SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		SendMessage(hwnd, WM_SIZE, 0,
			    MAKELPARAM(size.width(), size.height()));
	};

	if (bImmediately) {
		func();
	} else {
		QueueCEFTask(func);
	}

#endif
}

void QCefWidgetInternal::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);

	if (!cefBrowser) {
		//obs_browser_initialize();
		//connect(&timer, SIGNAL(timeout()), this, SLOT(Init()));
		//timer.start(500);
		//Init();
	}
}
void QCefWidgetInternal::closeEvent(QCloseEvent *event)
{
	//renjinbo, #5053 because chat widget when start to load cef, this widget will cauch alt+f4 to close this cef widget instead of chat dialog.
	event->ignore();
	this->window()->hide();
}

QPaintEngine *QCefWidgetInternal::paintEngine() const
{
	return nullptr;
}

void QCefWidgetInternal::setURL(const std::string &url_)
{
	url = url_;
	if (cefBrowser) {
		cefBrowser->GetMainFrame()->LoadURL(url);
	}
}

void QCefWidgetInternal::allowAllPopups(bool allow)
{
	allowAllPopups_ = allow;
}

void QCefWidgetInternal::ExecuteOnBrowser(
	std::function<void(CefRefPtr<CefBrowser>)> func, bool async)
{
	if (!async) {
#ifdef USE_QT_LOOP
		if (QThread::currentThread() == qApp->thread()) {
			if (!!cefBrowser)
				func(cefBrowser);
			return;
		}
#endif
		QEventLoop loop;
		bool success = QueueCEFTask([&]() {
			if (!!cefBrowser)
				func(cefBrowser);
			QMetaObject::invokeMethod(&loop, &QEventLoop::quit);
		});
		if (success) {
			loop.exec(QEventLoop::ExcludeUserInputEvents);
		}
	} else {
		CefRefPtr<CefBrowser> browser = cefBrowser;
		if (!!browser) {
#ifdef USE_QT_LOOP
			QueueBrowserTask(cefBrowser, func);
#else
			QueueCEFTask([=]() { func(browser); });
#endif
		}
	}
}

extern void DispatchJSEvent(std::string eventName, std::string jsonString);
//PRISM/Zhangdewen/20200921/#/fix jsonString must be keep problem
extern void DispatchJSEvent(obs_source_t *source, std::string eventName,
			    std::string jsonString);

void DispatchPrismEvent(const char *eventName, const char *jsonString)
{
	std::string event = eventName;
	std::string json = jsonString ? jsonString : "{}";

	auto func = [event, json](CefRefPtr<CefBrowser> browser) {
		CefRefPtr<CefProcessMessage> msg =
			CefProcessMessage::Create("DispatchJSEvent");
		CefRefPtr<CefListValue> args = msg->GetArgumentList();

		args->SetString(0, event);
		args->SetString(1, json);
		SendBrowserProcessMessage(browser, PID_RENDERER, msg);
	};

	std::for_each(begin(g_lstCefWidget), end(g_lstCefWidget),
		      [&](auto item) { item->ExecuteOnBrowser(func, true); });

	//PRISM/Zhangdewen/20200921/#/fix event not notify web source
	DispatchJSEvent(event, json);
}

//PRISM/Zhangdewen/20200901/#/for chat source
void DispatchPrismEvent(obs_source_t *source, const char *eventName,
			const char *jsonString)
{
	DispatchJSEvent(source, eventName, jsonString);
}

/* ------------------------------------------------------------------------- */

struct QCefInternal : QCef {
	virtual bool init_browser(void) override;
	virtual bool initialized(void) override;
	virtual bool wait_for_browser_init(void) override;

	// OBS Modification:
	// Zhang dewen / 20200211 / Related Issue ID=347
	// Reason: add request headers parameter
	// Solution: modify request headers
	virtual QCefWidget *create_widget(
		QWidget *parent, const std::string &url,
		const std::string &script, QCefCookieManager *cookie_manager,
		const std::map<std::string, std::string> &headers) override;

	virtual QCefCookieManager *
	create_cookie_manager(const std::string &storage_path,
			      bool persist_session_cookies) override;

	virtual BPtr<char>
	get_cookie_path(const std::string &storage_path) override;

	virtual void add_popup_whitelist_url(const std::string &url,
					     QObject *obj) override;
	virtual void add_force_popup_url(const std::string &url,
					 QObject *obj) override;
};

bool QCefInternal::init_browser(void)
{
	if (os_event_try(cef_started_event) == 0)
		return true;

	obs_browser_initialize();
	return false;
}

bool QCefInternal::initialized(void)
{
	return os_event_try(cef_started_event) == 0;
}

bool QCefInternal::wait_for_browser_init(void)
{
	return os_event_wait(cef_started_event) == 0;
}

// OBS Modification:
// Zhang dewen / 20200211 / Related Issue ID=347
// Reason: add request headers parameter
// Solution: modify request headers
QCefWidget *
QCefInternal::create_widget(QWidget *parent, const std::string &url,
			    const std::string &script, QCefCookieManager *cm,
			    const std::map<std::string, std::string> &headers)
{
	QCefCookieManagerInternal *cmi =
		reinterpret_cast<QCefCookieManagerInternal *>(cm);

	return new QCefWidgetInternal(parent, url, script,
				      cmi ? cmi->rc : nullptr, headers);
}

QCefCookieManager *
QCefInternal::create_cookie_manager(const std::string &storage_path,
				    bool persist_session_cookies)
{
	try {
		return new QCefCookieManagerInternal(storage_path,
						     persist_session_cookies);
	} catch (const char *error) {
		blog(LOG_ERROR, "Failed to create cookie manager: %s", error);
		return nullptr;
	}
}

BPtr<char> QCefInternal::get_cookie_path(const std::string &storage_path)
{
	BPtr<char> rpath = obs_module_config_path(storage_path.c_str());
	return os_get_abs_path_ptr(rpath.Get());
}

void QCefInternal::add_popup_whitelist_url(const std::string &url, QObject *obj)
{
	std::lock_guard<std::mutex> lock(popup_whitelist_mutex);
	popup_whitelist.emplace_back(url, obj);
}

void QCefInternal::add_force_popup_url(const std::string &url, QObject *obj)
{
	std::lock_guard<std::mutex> lock(popup_whitelist_mutex);
	forced_popups.emplace_back(url, obj);
}

extern "C" EXPORT QCef *obs_browser_create_qcef(void)
{
	return new QCefInternal();
}

#define BROWSER_PANEL_VERSION 2

extern "C" EXPORT int obs_browser_qcef_version_export(void)
{
	return BROWSER_PANEL_VERSION;
}
