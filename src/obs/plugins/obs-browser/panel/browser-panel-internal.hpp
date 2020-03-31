#pragma once

#include <QTimer>
#include <QPointer>
#include "browser-panel.hpp"
#include "cef-headers.hpp"

#include <vector>
#include <mutex>

struct PopupWhitelistInfo {
	std::string url;
	QPointer<QObject> obj;

	inline PopupWhitelistInfo(const std::string &url_, QObject *obj_)
		: url(url_), obj(obj_)
	{
	}
};

extern std::mutex popup_whitelist_mutex;
extern std::vector<PopupWhitelistInfo> popup_whitelist;
extern std::vector<PopupWhitelistInfo> forced_popups;

/* ------------------------------------------------------------------------- */

#if CHROME_VERSION_BUILD < 3770
class QCefRequestContextHandler : public CefRequestContextHandler {
	CefRefPtr<CefCookieManager> cm;

public:
	inline QCefRequestContextHandler(CefRefPtr<CefCookieManager> cm_)
		: cm(cm_)
	{
	}

	virtual CefRefPtr<CefCookieManager> GetCookieManager() override;

	IMPLEMENT_REFCOUNTING(QCefRequestContextHandler);
};
#endif

/* ------------------------------------------------------------------------- */

class QCefWidgetInternal : public QCefWidget {
	Q_OBJECT

public:
	// OBS Modification:
	// Zhang dewen / 20200211 / Related Issue ID=347
	// Reason: store request headers
	// Solution: modify request headers
	QCefWidgetInternal(QWidget *parent, const std::string &url,
			   const std::string &script,
			   CefRefPtr<CefRequestContext> rqc,
			   const std::map<std::string, std::string> &headers);
	~QCefWidgetInternal();

	CefRefPtr<CefBrowser> cefBrowser;
	std::string url;
	std::string script;
	CefRefPtr<CefRequestContext> rqc;
	QTimer timer;
	bool allowAllPopups_ = false;

	// OBS Modification:
	// Zhang dewen / 20200211 / Related Issue ID=347
	// Reason: store request headers
	// Solution: modify request headers
	std::map<std::string, std::string> headers;

	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;
	virtual QPaintEngine *paintEngine() const override;

	virtual void setURL(const std::string &url) override;
	virtual void allowAllPopups(bool allow) override;
	virtual void closeBrowser() override;
	virtual void closeEvent(QCloseEvent *event) override;

	void Resize(bool bImmediately);

	void ExecuteOnBrowser(std::function<void(CefRefPtr<CefBrowser>)> func,
			      bool async);
public slots:
	void Init();
};
