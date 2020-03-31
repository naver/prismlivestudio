#pragma once
#include <Windows.h>
#include "cef-headers.hpp"
#include "osr_ime_handler_win.h"

#define VIEW_BK_RENDER_COLOR 0X151515
#define VIEW_BK_RESIZE_COLOR 0X000000

/*
In class InteractionView, many code is integrated from CEF's sample and we never modify its code style.
You can get the sample from https://github.com/chromiumembedded/cef/tree/master/tests/cefclient/browser
*/

class BrowserInteractionMain;
class InteractionView {
public:
	static void RegisterClassName();
	static void UnregisterClassName();

	InteractionView(BrowserInteractionMain &interaction_main);
	virtual ~InteractionView();

	// should be invoked in CEF's main thread
	void Create(HWND hp);
	void ResetInteraction();

	void OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser, const CefRange &selection_range,
		const CefRenderHandler::RectList &character_bounds);

protected:
	static LRESULT CALLBACK InteractionViewProc(HWND hWnd, UINT message,
						    WPARAM wParam,
						    LPARAM lParam);

	void OnDestroy();
	void OnPaint();

	void OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnFocus(bool setFocus);
	void OnCaptureLost();
	void OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam);

	void OnIMESetContext(UINT message, WPARAM wParam, LPARAM lParam);
	void OnIMEStartComposition();
	void OnIMEComposition(UINT message, WPARAM wParam, LPARAM lParam);
	void OnIMECancelCompositionEvent();

	int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam);
	int GetCefMouseModifiers(WPARAM wparam);
	POINT TransMousePointToCef(const POINT &client_point);

public:
	BrowserInteractionMain &main_ui_;

	int source_cx_ = 0;
	int source_cy_ = 0;

	HWND hwnd_ = NULL;
	CefRefPtr<CefBrowser> browser_;
	scoped_ptr<client::OsrImeHandlerWin> ime_handler_;

	// Mouse state tracking.
	POINT last_mouse_pos_ = {0, 0};
	POINT current_mouse_pos_ = {0, 0};
	bool mouse_rotation_ = false;
	bool mouse_tracking_ = false;
	int last_click_x_ = 0;
	int last_click_y_ = 0;
	CefBrowserHost::MouseButtonType last_click_button_ = MBT_LEFT;
	int last_click_count_ = 0;
	double last_click_time_ = 0;
	bool last_mouse_down_on_view_ = false;
};
