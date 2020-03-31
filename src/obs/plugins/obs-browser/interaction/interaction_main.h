#pragma once
#include "interaction_view.h"
#include "interaction_button.h"
#include "interaction_util.hpp"
#include "interaction_manager.h"
#include <obs-data.h>
#include <memory>

using namespace interaction;

class BrowserClient;
class BrowserInteractionMain {
public:
	static std::vector<std::string> font_list;
	static HWND prism_main;
	static SIZE user_size; // as 100% scale
	static HICON interaction_icon;

	static void SetGlobalParam(obs_data_t *private_data);
	static void RegisterClassName();
	static void UnregisterClassName();

	BrowserInteractionMain(SOURCE_HANDLE src);
	virtual ~BrowserInteractionMain();

	// Warning : Should be invoked in CEF's main thread
	void CreateInteractionUI();
	void WaitDestroyInteractionUI();
	void ShowInteractionUI(bool show);
	void BringUIToTop();
	void SetInteractionInfo(int source_width, int source_height,
				CefRefPtr<CefBrowser> cefBrowser);
	void GetInteractionInfo(int &source_width, int &source_height,
				CefRefPtr<CefBrowser> &cefBrowser);
	void OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser, const CefRange &selection_range,
		const CefRenderHandler::RectList &character_bounds);

	// Can be invoked out of CEF's main thread
	void PostWindowTitle(const char *str);
	void PostDestroyInteractionUI();
	HWND GetInteractionView();
	HWND GetInteractionMain();
	bool IsResizing();

protected:
	static LRESULT CALLBACK InteractionMainProc(HWND hWnd, UINT message,
						    WPARAM wParam,
						    LPARAM lParam);

	void OnDestroy();
	void OnPaint();
	void OnSizing();
	void OnSize();
	void OnGetMinMaxInfo(LPARAM lParam);
	void OnDpiChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnNCHitTest(HWND hWnd, UINT message, WPARAM wParam,
			    LPARAM lParam);
	LRESULT OnSetCursor(HWND hWnd, UINT message, WPARAM wParam,
			    LPARAM lParam);
	LRESULT OnNcLButtonDown(HWND hWnd, UINT message, WPARAM wParam,
				LPARAM lParam);
	bool OnLButtonClick(UINT id, HWND hWnd);
	void OnSetTitle(WPARAM wParam, LPARAM lParam);
	void OnCloseButton();

	bool ShouldClose();
	void SaveUserSize();
	void UpdateLayout();
	void UpdateImage(float dpi);
	void ShowUI();
	void MovePosition();
	void DrawTitle(HDC &hdc, RECT &rc, float &dpi);
	void SetWindowSize(int left, int top, float dpi);

private:
	// We won't destroy window during browser source's lifetime
	bool should_close = false;

	// Set true after creating UI.
	// Because there is no lock for create/read interaction's HWND, so we won't return its HWND until finishing creating UI.
	bool window_created = false;

	SOURCE_HANDLE source = NULL;

	// Manage logic about resizing.
	// While resizing window and d3d render, UI will flick. As before, we will stop d3d render for interaction while resizing.
	SIZE previous_size = {0, 0};
	volatile DWORD previous_resize_time = 0;

	std::string utf8_title = "";
	HWND hWnd = NULL;

	InteractionButton btn_close;
	InteractionView view_wnd;
};

typedef std::shared_ptr<BrowserInteractionMain> INTERACTION_PTR;
typedef std::weak_ptr<BrowserInteractionMain> INTERACTION_WEAK_PTR;
