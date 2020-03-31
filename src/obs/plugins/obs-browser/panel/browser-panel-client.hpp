#pragma once

#include "cef-headers.hpp"
#include "browser-panel-internal.hpp"

#include <string>
#include <map>

class QCefBrowserClient : public CefClient,
			  public CefDisplayHandler,
			  public CefRequestHandler,
			  public CefLifeSpanHandler,
			  public CefLoadHandler,
			  public CefKeyboardHandler,
			  public CefContextMenuHandler {

public:
	// OBS Modification:
	// Zhang dewen / 20200211 / Related Issue ID=347
	// Reason: store request headers
	// Solution: modify request headers
	inline QCefBrowserClient(
		QCefWidgetInternal *widget_, const std::string &script_,
		bool allowAllPopups_,
		const std::map<std::string, std::string> &headers_)
		: widget(widget_),
		  script(script_),
		  allowAllPopups(allowAllPopups_),
		  headers(headers_)
	{
	}

	/* CefClient */
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override;
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override;

	/* CefDisplayHandler */
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
				   const CefString &title) override;

	/* CefRequestHandler */
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
				    CefRefPtr<CefFrame> frame,
				    CefRefPtr<CefRequest> request,
				    bool user_gesture,
				    bool is_redirect) override;

	virtual bool OnOpenURLFromTab(
		CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
		const CefString &target_url,
		CefRequestHandler::WindowOpenDisposition target_disposition,
		bool user_gesture) override;

	/* CefLifeSpanHandler */
	virtual bool OnBeforePopup(
		CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
		const CefString &target_url, const CefString &target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture, const CefPopupFeatures &popupFeatures,
		CefWindowInfo &windowInfo, CefRefPtr<CefClient> &client,
		CefBrowserSettings &settings,
#if CHROME_VERSION_BUILD >= 3770
		CefRefPtr<CefDictionaryValue> &extra_info,
#endif
		bool *no_javascript_access) override;

	/* CefLoadHandler */
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			       CefRefPtr<CefFrame> frame,
			       int httpStatusCode) override;

	/* CefKeyboardHandler */
	virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
				   const CefKeyEvent &event,
				   CefEventHandle os_event,
				   bool *is_keyboard_shortcut) override;

	QCefWidgetInternal *widget = nullptr;
	std::string script;
	bool allowAllPopups;

	// OBS Modification:
	// Zhang dewen / 20200211 / Related Issue ID=347
	// Reason: store request headers
	// Solution: modify request headers
	std::map<std::string, std::string> headers;

	// OBS Modification:
	// wu.longyue@navercorp.com / 20200228 / new feature
	// Reason: The host and javascript need communicate eachother
	// Solution: override this function to receive message
	virtual bool
	OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
				 CefRefPtr<CefFrame> frame,
				 CefProcessId source_process,
				 CefRefPtr<CefProcessMessage> message) override;

	IMPLEMENT_REFCOUNTING(QCefBrowserClient);

	// OBS Modification:
	// ren.jinbo@navercorp.com / 20200609 / add feature
	// Reason: the right menu need add develper tool in debug
	// Solution: override this function to receive message
	virtual CefRefPtr<CefContextMenuHandler>
	GetContextMenuHandler() override;
	void ShowDevelopTools(CefRefPtr<CefBrowser> browser);
	virtual void
	OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
			    CefRefPtr<CefFrame> frame,
			    CefRefPtr<CefContextMenuParams> params,
			    CefRefPtr<CefMenuModel> model) override;

	virtual bool
	OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
			     CefRefPtr<CefFrame> frame,
			     CefRefPtr<CefContextMenuParams> params,
			     int command_id, EventFlags event_flags) override;
};
