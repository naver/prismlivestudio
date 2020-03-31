#include "browser-panel-client.hpp"
#include "obs-frontend-api.h"
#include <util/dstr.h>

#include <QUrl>
#include <QDesktopServices>
#include <QApplication>
#include <QSettings>

#ifdef _WIN32
#include <windows.h>
#endif

/* CefClient */
CefRefPtr<CefLoadHandler> QCefBrowserClient::GetLoadHandler()
{
	return this;
}

CefRefPtr<CefDisplayHandler> QCefBrowserClient::GetDisplayHandler()
{
	return this;
}

CefRefPtr<CefRequestHandler> QCefBrowserClient::GetRequestHandler()
{
	return this;
}

CefRefPtr<CefLifeSpanHandler> QCefBrowserClient::GetLifeSpanHandler()
{
	return this;
}

CefRefPtr<CefKeyboardHandler> QCefBrowserClient::GetKeyboardHandler()
{
	return this;
}

/* CefDisplayHandler */
void QCefBrowserClient::OnTitleChange(CefRefPtr<CefBrowser> browser,
				      const CefString &title)
{
	if (widget && widget->cefBrowser->IsSame(browser)) {
		std::string str_title = title;
		QString qt_title = QString::fromUtf8(str_title.c_str());
		QMetaObject::invokeMethod(widget, "titleChanged",
					  Q_ARG(QString, qt_title));
	} else { /* handle popup title */
#ifdef _WIN32
		std::wstring str_title = title;
		HWND hwnd = browser->GetHost()->GetWindowHandle();
		SetWindowTextW(hwnd, str_title.c_str());
#endif
	}
}

/* CefRequestHandler */
bool QCefBrowserClient::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
				       CefRefPtr<CefFrame>,
				       CefRefPtr<CefRequest> request, bool,
				       bool)
{
	std::string str_url = request->GetURL();

	std::lock_guard<std::mutex> lock(popup_whitelist_mutex);
	for (size_t i = forced_popups.size(); i > 0; i--) {
		PopupWhitelistInfo &info = forced_popups[i - 1];

		if (!info.obj) {
			forced_popups.erase(forced_popups.begin() + (i - 1));
			continue;
		}

		if (astrcmpi(info.url.c_str(), str_url.c_str()) == 0) {
			/* Open tab popup URLs in user's actual browser */
			QUrl url = QUrl(str_url.c_str(), QUrl::TolerantMode);
			QDesktopServices::openUrl(url);
			browser->GoBack();
			return true;
		}
	}

	if (widget) {
		// OBS Modification:
		// Zhang dewen / 20200211 / Related Issue ID=347
		// Reason: modify request headers before browse
		// Solution: modify request headers
		if (!headers.empty()) {
			for (auto &header : headers) {
				request->SetHeaderByName(header.first,
							 header.second, true);
			}
		}

		QString qt_url = QString::fromUtf8(str_url.c_str());
		QMetaObject::invokeMethod(widget, "urlChanged",
					  Q_ARG(QString, qt_url));
	}
	return false;
}

bool QCefBrowserClient::OnOpenURLFromTab(
	CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, const CefString &target_url,
	CefRequestHandler::WindowOpenDisposition, bool)
{
	std::string str_url = target_url;

	/* Open tab popup URLs in user's actual browser */
	QUrl url = QUrl(str_url.c_str(), QUrl::TolerantMode);
	QDesktopServices::openUrl(url);
	return true;
}

/* CefLifeSpanHandler */
bool QCefBrowserClient::OnBeforePopup(
	CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, const CefString &target_url,
	const CefString &, CefLifeSpanHandler::WindowOpenDisposition, bool,
	const CefPopupFeatures &, CefWindowInfo &windowInfo,
	CefRefPtr<CefClient> &, CefBrowserSettings &,
#if CHROME_VERSION_BUILD >= 3770
	CefRefPtr<CefDictionaryValue> &,
#endif
	bool *)
{
	if (allowAllPopups) {
#ifdef _WIN32
		HWND hwnd = (HWND)widget->effectiveWinId();
		windowInfo.parent_window = hwnd;
#endif
		return false;
	}

	std::string str_url = target_url;

	std::lock_guard<std::mutex> lock(popup_whitelist_mutex);
	for (size_t i = popup_whitelist.size(); i > 0; i--) {
		PopupWhitelistInfo &info = popup_whitelist[i - 1];

		if (!info.obj) {
			popup_whitelist.erase(popup_whitelist.begin() +
					      (i - 1));
			continue;
		}

		if (astrcmpi(info.url.c_str(), str_url.c_str()) == 0) {
#ifdef _WIN32
			HWND hwnd = (HWND)widget->effectiveWinId();
			windowInfo.parent_window = hwnd;
#endif
			return false;
		}
	}

	/* Open popup URLs in user's actual browser */
	QUrl url = QUrl(str_url.c_str(), QUrl::TolerantMode);
	QDesktopServices::openUrl(url);
	return true;
}

void QCefBrowserClient::OnLoadEnd(CefRefPtr<CefBrowser>,
				  CefRefPtr<CefFrame> frame, int)
{
	if (frame->IsMain() && !script.empty())
		frame->ExecuteJavaScript(script, CefString(), 0);
}

bool QCefBrowserClient::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
				      const CefKeyEvent &event, CefEventHandle,
				      bool *)
{
#ifdef _WIN32
	if (event.type != KEYEVENT_RAWKEYDOWN)
		return false;

	//RENJINBO ignore refresh key, because chat local html can't reload.
	/*if (event.windows_key_code == 'R' &&
	    (event.modifiers & EVENTFLAG_CONTROL_DOWN) != 0) {
		browser->ReloadIgnoreCache();
		return true;
	}*/
#endif
	return false;
}

bool QCefBrowserClient::OnProcessMessageReceived(
	CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
	CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetArgumentList()->GetSize() == 0) {
		return false;
	}

	std::string result("{}");
	const std::string &name = message->GetName();
	const std::string &param = message->GetArgumentList()->GetString(0);

	if ("sendToPrism" == name) {
		if (nullptr != prism_frontend_web_invoked) {
			result = prism_frontend_web_invoked(param.c_str());
		}
	} else {
		return false;
	}

	if (message->GetArgumentList()->GetSize() == 2) {
		CefRefPtr<CefProcessMessage> msg =
			CefProcessMessage::Create("executeCallback");

		CefRefPtr<CefListValue> args = msg->GetArgumentList();
		args->SetInt(0, message->GetArgumentList()->GetInt(1));
		args->SetString(1, result);

		SendBrowserProcessMessage(browser, PID_RENDERER, msg);
	}

	return true;
}

/*CefContextMenuHandler*/
CefRefPtr<CefContextMenuHandler> QCefBrowserClient::GetContextMenuHandler()
{
	return this;
}

enum UserDefineEnum {
	MENU_ID_USER_SHOWDEVTOOLS = MENU_ID_USER_FIRST + 300,
};

void QCefBrowserClient::OnBeforeContextMenu(
	CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	model->Clear();

	QSettings setting("PrismLive", QApplication::applicationName());
	const QString KEY_DEV_SERVER = QStringLiteral("DevServer");
	bool devServer = setting.value(KEY_DEV_SERVER, false).toBool();
	if (!devServer) {
		return;
	}

	model->AddItem(MENU_ID_BACK, "Back");
	model->SetEnabled(MENU_ID_BACK, browser->CanGoBack());

	model->AddItem(MENU_ID_FORWARD, "Forward");
	model->SetEnabled(MENU_ID_FORWARD, browser->CanGoForward());

	model->AddItem(MENU_ID_RELOAD_NOCACHE, "Reload");
	model->SetEnabled(MENU_ID_RELOAD_NOCACHE, true);

	if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) !=
	    0) {
		model->AddItem(MENU_ID_USER_SHOWDEVTOOLS, "Show DevTools");
	}
}

bool QCefBrowserClient::OnContextMenuCommand(
	CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params, int command_id,
	EventFlags event_flags)
{
	switch (command_id) {
	case MENU_ID_USER_SHOWDEVTOOLS: {
		ShowDevelopTools(browser);
		return true;
	}
	default:
		break;
	}
	return false;
}

void QCefBrowserClient::ShowDevelopTools(CefRefPtr<CefBrowser> browser)
{
	CefWindowInfo windowInfo;
	CefBrowserSettings settings;

#if defined(OS_WIN)
	windowInfo.SetAsPopup(NULL, "DevTools");
#endif
	browser->GetHost()->ShowDevTools(windowInfo, this, settings,
					 CefPoint());
}
