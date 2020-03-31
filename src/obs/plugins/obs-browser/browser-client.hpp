/******************************************************************************
 Copyright (C) 2014 by John R. Bradley <jrb@turrettech.com>
 Copyright (C) 2018 by Hugh Bailey ("Jim") <jim@obsproject.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#pragma once

#include <graphics/graphics.h>
#include "cef-headers.hpp"
#include "browser-config.h"

//PRISM/Wangshaohui/20200811/#3784/for cef interaction
#include "interaction/interaction_main.h"

#define USE_TEXTURE_COPY 0

struct BrowserSource;
struct TextMotionSource;

//PRISM/Wangshaohui/20200917/#3714/for check client
void SetValidClient(void *pointer, bool valid);

class BrowserClient : public CefClient,
		      public CefDisplayHandler,
		      public CefLifeSpanHandler,
		      public CefContextMenuHandler,
		      public CefRenderHandler,
#if CHROME_VERSION_BUILD >= 3683
		      public CefAudioHandler,
#endif
		      public CefLoadHandler,
		      public CefRequestHandler {

#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
#if USE_TEXTURE_COPY
	gs_texture_t *texture = nullptr;
#endif
	void *last_handle = INVALID_HANDLE_VALUE;
#endif
	bool sharing_available = false;
	bool reroute_audio = true;

public:
	BrowserSource *bs;
	CefRect popupRect;
	CefRect originalPopupRect;

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	INTERACTION_WEAK_PTR interaction_weak;

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	inline BrowserClient(BrowserSource *bs_, bool sharing_avail,
			     bool reroute_audio_, INTERACTION_WEAK_PTR itr)
		: bs(bs_),
		  sharing_available(sharing_avail),
		  reroute_audio(reroute_audio_),
		  interaction_weak(itr)
	{
		//PRISM/Wangshaohui/20200917/#3714/for check client
		SetValidClient(this, true);
	}

	virtual ~BrowserClient();

	/* CefClient */
	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override;
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
	virtual CefRefPtr<CefContextMenuHandler>
	GetContextMenuHandler() override;
#if CHROME_VERSION_BUILD >= 3683
	virtual CefRefPtr<CefAudioHandler> GetAudioHandler() override;
#endif

	virtual bool
	OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
#if CHROME_VERSION_BUILD >= 3770
				 CefRefPtr<CefFrame> frame,
#endif
				 CefProcessId source_process,
				 CefRefPtr<CefProcessMessage> message) override;

	/* CefDisplayHandler */
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
#if CHROME_VERSION_BUILD >= 3282
				      cef_log_severity_t level,
#endif
				      const CefString &message,
				      const CefString &source,
				      int line) override;

	/* CefRequestHandler */
	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
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

	/* CefContextMenuHandler */
	virtual void
	OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
			    CefRefPtr<CefFrame> frame,
			    CefRefPtr<CefContextMenuParams> params,
			    CefRefPtr<CefMenuModel> model) override;

	/* CefRenderHandler */
#if CHROME_VERSION_BUILD >= 3578
	virtual void GetViewRect(
#else
	virtual bool GetViewRect(
#endif
		CefRefPtr<CefBrowser> browser, CefRect &rect) override;

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX,
				    int viewY, int &screenX,
				    int &screenY) OVERRIDE;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser,
			     PaintElementType type, const RectList &dirtyRects,
			     const void *buffer, int width,
			     int height) override;
#if EXPERIMENTAL_SHARED_TEXTURE_SUPPORT_ENABLED
	virtual void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
					PaintElementType type,
					const RectList &dirtyRects,
					void *shared_handle) override;
#endif

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
				    CefCursorHandle cursor,
				    CefRenderHandler::CursorType type,
				    const CefCursorInfo &custom_cursor_info);

	//PRISM/Wangshaohui/20200811/#3784/for cef interaction
	virtual void OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser, const CefRange &selection_range,
		const CefRenderHandler::RectList &character_bounds);

#if CHROME_VERSION_BUILD >= 3683
	virtual void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser,
					 int audio_stream_id,
					 const float **data, int frames,
					 int64_t pts) override;

	virtual void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser,
					  int audio_stream_id);

	virtual void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
					  int audio_stream_id, int channels,
					  ChannelLayout channel_layout,
					  int sample_rate,
					  int frames_per_buffer) override;

#endif
	/* CefLoadHandler */
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			       CefRefPtr<CefFrame> frame,
			       int httpStatusCode) override;

	//PRISM/WangShaohui/20200310/#1332/adding logs for exceptions
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
				 CefRefPtr<CefFrame> frame,
				 CefLoadHandler::ErrorCode errorCode,
				 const CefString &errorText,
				 const CefString &failedUrl) override;

	IMPLEMENT_REFCOUNTING(BrowserClient);
};
