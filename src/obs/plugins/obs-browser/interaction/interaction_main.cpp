#include "interaction_main.h"
#include "browser-client.hpp"
#include "interaction_hdc.hpp"
#include <util/util.hpp>
#include <util/platform.h>
#include <obs-module.h>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "WinMM.lib")

// Need DPI scale
#define INTERACTION_DEFAULT_CX 664
#define INTERACTION_DEFAULT_CY 562
#define INTERACTION_SIZE_IGNORE 400
#define INTERACTION_MIN_CX 332
#define INTERACTION_MIN_CY 280
#define INTERACTION_MAX_CX 7680
#define INTERACTION_MAX_CY 4320
#define CLOSE_BUTTON_CX 22
#define CLOSE_BUTTON_CY 22
#define CLOSE_BUTTON_MARGIN_TOP 10
#define CLOSE_BUTTON_MARGIN_RIGHT 10
#define TOP_REGION_HEIGHT 40
#define BORDER_SIZE 1
#define VIEW_MARGIN 10
#define RESIZE_REGION_SIZE 5
#define TITLE_OFFSET_X 17
#define TITLE_OFFSET_Y 11
#define TITLE_FONT_SIZE 12

#define NORMAL_BK_COLOR 0X272727
#define BORDER_COLOR 0X111111
#define TITLE_TEXT_COLOR 0XFFFFFF
#define RESIZING_DELAY_KEEP 300 // ms
#define RESIZING_DELAY_END 100  // ms

#define INTERACTION_CLASS_MAIN L"PrismInteractionClassName_Main"
#define PRISM_ICON "images/PRISMLiveStudio.ico"
#define CLOSE_BTN_NORMAL "btn-close-normal.png"
#define CLOSE_BTN_HOVER "btn-close-over.png"
#define CLOSE_BTN_CLICK "btn-close-click.png"
#define CLOSE_BTN_DISABLE "close-btn-disable.png"

//-------------------------------------------------------
std::vector<std::string> BrowserInteractionMain::font_list = {
	"Segoe UI", "MalgunGothic", "Malgun Gothic", "Dotum", "Gulin"};
HWND BrowserInteractionMain::prism_main = NULL;
SIZE BrowserInteractionMain::user_size = {INTERACTION_DEFAULT_CX,
					  INTERACTION_DEFAULT_CY};
HICON BrowserInteractionMain::interaction_icon = NULL;

void BrowserInteractionMain::SetGlobalParam(obs_data_t *private_data)
{
	int cx = obs_data_get_int(private_data, "interaction_cx");
	int cy = obs_data_get_int(private_data, "interaction_cy");
	if (cx >= INTERACTION_MIN_CX && cx <= INTERACTION_MAX_CX &&
	    cy >= INTERACTION_MIN_CY && cy <= INTERACTION_MAX_CY) {
		BrowserInteractionMain::user_size.cx = cx;
		BrowserInteractionMain::user_size.cy = cy;
	}

	HWND hWnd = (HWND)obs_data_get_int(private_data, "prism_hwnd");
	if (::IsWindow(hWnd)) {
		BrowserInteractionMain::prism_main = hWnd;
	} else {
		blog(LOG_WARNING, "Invalid prism HWND for interaction");
		assert(false);
	}
}

void BrowserInteractionMain::RegisterClassName()
{
	char *path = obs_module_file(PRISM_ICON);
	if (path) {
		interaction_icon = (HICON)LoadImageA(
			NULL, // hInstance must be NULL when loading from a file
			path, // the icon file name
			IMAGE_ICON, // specifies that the file is an icon
			0, // width of the image (we'll specify default later on)
			0,                // height of the image
			LR_LOADFROMFILE | // we want to load a file (as opposed to a resource)
				LR_DEFAULTSIZE | // default metrics based on the type (IMAGE_ICON, 32x32)
				LR_SHARED // let the system release the handle when it's no longer used
		);

		assert(interaction_icon);
		bfree(path);
		path = NULL;
	}

	if (!RegisterInteractionClass(INTERACTION_CLASS_MAIN,
				      InteractionMainProc, interaction_icon)) {
		assert(false);
		blog(LOG_ERROR,
		     "Failed to register class for interaction main. error:%u",
		     GetLastError());
	}
}

void BrowserInteractionMain::UnregisterClassName()
{
	::UnregisterClass(INTERACTION_CLASS_MAIN, GetModuleHandle(NULL));

	if (interaction_icon) {
		DestroyIcon(interaction_icon);
		interaction_icon = NULL;
	}
}

LRESULT CALLBACK BrowserInteractionMain::InteractionMainProc(HWND hWnd,
							     UINT message,
							     WPARAM wParam,
							     LPARAM lParam)
{
	HOOK_MOUSE_EVENT(hWnd, message);

	BrowserInteractionMain *self =
		GetUserDataPtr<BrowserInteractionMain *>(hWnd);

	switch (message) {
	case WM_CLOSE:
		if (self && !self->ShouldClose()) {
			blog(LOG_INFO,
			     "UI: [UI STEP] User close interaction by taskbar.");
			self->OnCloseButton();
			return false;
		}
		break;

	case WM_DESTROY:
		if (self) {
			self->OnDestroy();
		}
		break;

	case WM_INTERACTION_LBUTTON_CLICK:
		if (self && self->OnLButtonClick(wParam, (HWND)lParam)) {
			return 0;
		}
		break;

	case WM_INTERACTION_SET_TITLE:
		if (self) {
			self->OnSetTitle(wParam, lParam);
		}
		break;

	case WM_NCHITTEST:
		if (self) {
			return self->OnNCHitTest(hWnd, message, wParam, lParam);
		}
		break;

	case WM_SETCURSOR:
		if (self) {
			return self->OnSetCursor(hWnd, message, wParam, lParam);
		}
		break;

	case WM_NCLBUTTONDOWN:
		if (self) {
			return self->OnNcLButtonDown(hWnd, message, wParam,
						     lParam);
		}
		break;

	case WM_SIZING:
		if (self) {
			self->OnSizing();
		}
		break;

	case WM_SIZE:
		if (self) {
			self->OnSize();
		}
		break;

	case WM_GETMINMAXINFO:
		if (self) {
			self->OnGetMinMaxInfo(lParam);
		}
		break;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_PAINT:
		if (self) {
			self->OnPaint();
		}
		break;

	case WM_DPICHANGED:
		if (self) {
			self->OnDpiChanged(wParam, lParam);
		}
		break;

	case WM_SETFOCUS:
		if (self) {
			::SetFocus(self->view_wnd.hwnd_);
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//------------------------------------------------------------------------
BrowserInteractionMain::BrowserInteractionMain(SOURCE_HANDLE src)
	: source(src), view_wnd(*this)
{
}

BrowserInteractionMain::~BrowserInteractionMain()
{
	assert(!::IsWindow(hWnd));
	if (::IsWindow(hWnd)) {
		should_close = true;
		::PostMessage(
			hWnd, WM_CLOSE, NULL,
			NULL); // Here we'd better use PostMessage instead of SendMessage to avoid deadlock.

		Sleep(200); // For waiting PostMessage be done

		blog(LOG_ERROR, "Interaction is not destroied ! title:%s",
		     utf8_title.c_str());
		assert(false);
	}
}

void BrowserInteractionMain::CreateInteractionUI()
{
	DCHECK(CefCurrentlyOn(TID_UI));

	assert(false == should_close);
	if (::IsWindow(hWnd)) {
		return;
	}

	hWnd = ::CreateWindowEx(0, INTERACTION_CLASS_MAIN, L"",
				WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0,
				0, 0, 0, NULL, NULL, GetModuleHandle(NULL),
				NULL);

	if (!::IsWindow(hWnd)) {
		blog(LOG_ERROR,
		     "Failed to create interaction main window. error:%u",
		     GetLastError());

		assert(false);
		return;
	}

	blog(LOG_INFO, "Interaction is created. title:%s", utf8_title.c_str());

	::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

	btn_close.Create(hWnd, UI_IDC_BUTTON_CLOSE);
	btn_close.SetBkColor(NORMAL_BK_COLOR, NORMAL_BK_COLOR, NORMAL_BK_COLOR,
			     NORMAL_BK_COLOR);

	view_wnd.Create(hWnd);

	float dpi = GetWindowScaleFactor(hWnd);
	SetWindowSize(0, 0, dpi);
	UpdateImage(dpi);

	::ShowWindow(hWnd, SW_HIDE);

	window_created = true;
}

void BrowserInteractionMain::WaitDestroyInteractionUI()
{
	DCHECK(CefCurrentlyOn(TID_UI));

	should_close = true;
	if (::IsWindow(hWnd)) {
		::SendMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
}

void BrowserInteractionMain::ShowInteractionUI(bool show)
{
	DCHECK(CefCurrentlyOn(TID_UI));

	if (!::IsWindow(hWnd)) {
		return;
	}

	view_wnd.ResetInteraction();
	if (show) {
		ShowUI();
	} else {
		::ShowWindow(hWnd, SW_HIDE);
	}
}

void BrowserInteractionMain::BringUIToTop()
{
	DCHECK(CefCurrentlyOn(TID_UI));

	if (!::IsWindow(hWnd)) {
		return;
	}

	::ShowWindow(hWnd, SW_HIDE);

	if (::IsWindow(prism_main)) {
		MovePosition();
	}

	// Firstly bring to top, then show window
	BringWndToTop(hWnd);

	::ShowWindow(hWnd, SW_SHOW);
}

void BrowserInteractionMain::SetInteractionInfo(
	int source_width, int source_height, CefRefPtr<CefBrowser> cefBrowser)
{
	DCHECK(CefCurrentlyOn(TID_UI));

	view_wnd.source_cx_ = source_width;
	view_wnd.source_cy_ = source_height;
	view_wnd.browser_ = cefBrowser;
}

void BrowserInteractionMain::GetInteractionInfo(
	int &source_width, int &source_height,
	CefRefPtr<CefBrowser> &cefBrowser)
{
	DCHECK(CefCurrentlyOn(TID_UI));

	source_width = view_wnd.source_cx_;
	source_height = view_wnd.source_cy_;
	cefBrowser = view_wnd.browser_;
}

void BrowserInteractionMain::OnImeCompositionRangeChanged(
	CefRefPtr<CefBrowser> browser, const CefRange &selection_range,
	const CefRenderHandler::RectList &character_bounds)
{
	DCHECK(CefCurrentlyOn(TID_UI));
	view_wnd.OnImeCompositionRangeChanged(browser, selection_range,
					      character_bounds);
}

void BrowserInteractionMain::PostWindowTitle(const char *str)
{
	if (str && ::IsWindow(hWnd)) {
		// Note : remember to free memory in handler
		::PostMessage(hWnd, WM_INTERACTION_SET_TITLE,
			      (WPARAM)bstrdup(str), 0);
	}
}

void BrowserInteractionMain::PostDestroyInteractionUI()
{
	should_close = true;
	if (::IsWindow(hWnd)) {
		::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
}

HWND BrowserInteractionMain::GetInteractionView()
{
	if (window_created) {
		return view_wnd.hwnd_;
	} else {
		return NULL;
	}
}

HWND BrowserInteractionMain::GetInteractionMain()
{
	if (window_created) {
		return hWnd;
	} else {
		return NULL;
	}
}

bool BrowserInteractionMain::IsResizing()
{
	DWORD current = timeGetTime();
	DWORD previous = previous_resize_time;

	if (current <= previous) {
		return true;
	}

	DWORD space = current - previous;
	if (space > RESIZING_DELAY_KEEP) {
		return false;
	} else {
		if (::GetAsyncKeyState(VK_LBUTTON)) {
			return true;
		} else {
			if (space <= RESIZING_DELAY_END) {
				return true;
			} else {
				return false;
			}
		}
	}
}

void BrowserInteractionMain::OnDestroy()
{
	hWnd = NULL;
	window_created = false;

	blog(LOG_INFO, "Interaction is destroied. title:%s",
	     utf8_title.c_str());
}

void BrowserInteractionMain::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	{
		float dpi = GetWindowScaleFactor(hWnd);

		RECT rc;
		GetClientRect(hWnd, &rc);

		RECT top_line = {
			0, LogicalToDevice(TOP_REGION_HEIGHT, dpi), rc.right,
			LogicalToDevice(TOP_REGION_HEIGHT + BORDER_SIZE, dpi)};

		DoubleBufferHDC memdc(ps.hdc, rc);

		FillSolidRect(memdc.canvas_hdc, &rc, NORMAL_BK_COLOR);
		FillSolidRect(memdc.canvas_hdc, &top_line, BORDER_COLOR);
		DrawBorder(memdc.canvas_hdc, rc,
			   LogicalToDevice(BORDER_SIZE, dpi), BORDER_COLOR);

		DrawTitle(memdc.canvas_hdc, rc, dpi);
	}

	EndPaint(hWnd, &ps);
}

void BrowserInteractionMain::OnSizing()
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	int new_cx = RectWidth(rc);
	int new_cy = RectHeight(rc);

	if (new_cx != previous_size.cx || new_cy != previous_size.cy) {
		bool old_resizing = IsResizing();

		previous_size = {new_cx, new_cy};
		previous_resize_time = timeGetTime();

		if (!old_resizing) {
			// Because of UI flicker whle resizing and d3d render, we set resizing state and handle it in video render thread.
			// Here we block current thread for a while to make sure video render thread can handle the resizing state.
			// By this way, we can reduce the chance much about UI flicker.
			Sleep(60);
			previous_resize_time = timeGetTime();
		}
	}
}

void BrowserInteractionMain::OnSize()
{
	SaveUserSize();
	UpdateLayout();
}

void BrowserInteractionMain::OnGetMinMaxInfo(LPARAM lParam)
{
	float dpi = GetWindowScaleFactor(hWnd);

	MINMAXINFO *lpMMI = (MINMAXINFO *)lParam;

	lpMMI->ptMinTrackSize.x = LogicalToDevice(INTERACTION_MIN_CX, dpi);
	lpMMI->ptMinTrackSize.y = LogicalToDevice(INTERACTION_MIN_CY, dpi);

	lpMMI->ptMaxTrackSize.x = LogicalToDevice(INTERACTION_MAX_CX, dpi);
	lpMMI->ptMaxTrackSize.y = LogicalToDevice(INTERACTION_MAX_CY, dpi);
}

void BrowserInteractionMain::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	UpdateImage(GetWindowScaleFactor(hWnd));

	RECT *rect = reinterpret_cast<RECT *>(lParam);
	SetWindowPos(hWnd, NULL, rect->left, rect->top, RectWidth(*rect),
		     RectHeight(*rect), SWP_NOZORDER);
}

LRESULT BrowserInteractionMain::OnNCHitTest(HWND hWnd, UINT message,
					    WPARAM wParam, LPARAM lParam)
{
	// With multiple monitors, sometimes the value of lParam is incorrect, so we have to get position by API
	POINT pt;
	::GetCursorPos(&pt);

	RECT wrc;
	::GetWindowRect(hWnd, &wrc);

	float dpi = GetWindowScaleFactor(hWnd);
	int resize_region = LogicalToDevice(RESIZE_REGION_SIZE, dpi);
	int top_region = LogicalToDevice(TOP_REGION_HEIGHT, dpi);

	if (pt.x <= wrc.left + resize_region && pt.y <= wrc.top + resize_region)
		return HTTOPLEFT;
	else if (pt.x >= wrc.right - resize_region &&
		 pt.y <= wrc.top + resize_region)
		return HTTOPRIGHT;
	else if (pt.x <= wrc.left + resize_region &&
		 pt.y >= wrc.bottom - resize_region)
		return HTBOTTOMLEFT;
	else if (pt.x >= wrc.right - resize_region &&
		 pt.y >= wrc.bottom - resize_region)
		return HTBOTTOMRIGHT;
	else if (pt.x <= wrc.left + resize_region)
		return HTLEFT;
	else if (pt.x >= wrc.right - resize_region)
		return HTRIGHT;
	else if (pt.y <= wrc.top + resize_region)
		return HTTOP;
	else if (pt.y >= wrc.bottom - resize_region)
		return HTBOTTOM;
	else if (pt.y > wrc.top && pt.y < (wrc.top + top_region))
		return HTCAPTION;

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT BrowserInteractionMain::OnSetCursor(HWND hWnd, UINT message,
					    WPARAM wParam, LPARAM lParam)
{
	int hit_test = LOWORD_EX(lParam);
	switch (hit_test) {
	case HTTOP:
	case HTBOTTOM:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		return TRUE;

	case HTLEFT:
	case HTRIGHT:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		return TRUE;

	case HTTOPLEFT:
	case HTBOTTOMRIGHT:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
		return TRUE;

	case HTTOPRIGHT:
	case HTBOTTOMLEFT:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
		return TRUE;

	case HTCLIENT:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return TRUE;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT BrowserInteractionMain::OnNcLButtonDown(HWND hWnd, UINT message,
						WPARAM wParam, LPARAM lParam)
{
	// reset variable before resizing
	previous_size = {-1, -1};

	int hit_test = wParam;
	switch (hit_test) {
	case HTTOP:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, lParam);
		break;
	case HTBOTTOM:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, lParam);
		break;
	case HTLEFT:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, lParam);
		break;
	case HTRIGHT:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, lParam);
		break;
	case HTTOPLEFT:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT,
			    lParam);
		break;
	case HTTOPRIGHT:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT,
			    lParam);
		break;
	case HTBOTTOMLEFT:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT,
			    lParam);
		break;
	case HTBOTTOMRIGHT:
		SendMessage(hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT,
			    lParam);
		break;
	default:
		DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return TRUE;
}

bool BrowserInteractionMain::OnLButtonClick(UINT id, HWND hWnd)
{
	switch (id) {
	case UI_IDC_BUTTON_CLOSE:
		blog(LOG_INFO,
		     "UI: [UI STEP] User Click close button of interaction. [%s]",
		     utf8_title.c_str());

		OnCloseButton();
		return true;

	default:
		return false;
	}
}

void BrowserInteractionMain::OnSetTitle(WPARAM wParam, LPARAM lParam)
{
	char *title = (char *)wParam;
	if (!title) {
		assert(false);
		return;
	}

	utf8_title = title;
	bfree(title);

	::InvalidateRect(hWnd, NULL, TRUE);
	::SetWindowTextW(hWnd, TransUtf8ToUnicode(utf8_title.c_str()).c_str());
}

void BrowserInteractionMain::OnCloseButton()
{
	// Handle this request in BrowserSource::Tick()
	InteractionManager::Instance()->RequestHideInteraction(source);
}

bool BrowserInteractionMain::ShouldClose()
{
	return should_close;
}

void BrowserInteractionMain::SaveUserSize()
{
	if (!IsFocusInWnd(hWnd)) {
		return;
	}

	float dpi = GetWindowScaleFactor(hWnd);
	if (dpi < 1.0) {
		return;
	}

	RECT rc;
	GetClientRect(hWnd, &rc);

	int cx = (float)RectWidth(rc) / dpi;
	int cy = (float)RectHeight(rc) / dpi;

	if (cx < INTERACTION_MIN_CX) {
		cx = INTERACTION_MIN_CX;
	}

	if (cy < INTERACTION_MIN_CY) {
		cy = INTERACTION_MIN_CY;
	}

	BrowserInteractionMain::user_size.cx = cx;
	BrowserInteractionMain::user_size.cy = cy;
}

void BrowserInteractionMain::UpdateLayout()
{
	if (!::IsWindow(view_wnd.hwnd_)) {
		return;
	}

	float dpi = GetWindowScaleFactor(hWnd);

	RECT rc;
	GetClientRect(hWnd, &rc);

	int close_top = LogicalToDevice(CLOSE_BUTTON_MARGIN_TOP, dpi);
	int close_left =
		rc.right -
		LogicalToDevice(CLOSE_BUTTON_CX + CLOSE_BUTTON_MARGIN_RIGHT,
				dpi);

	int view_left = LogicalToDevice((BORDER_SIZE + VIEW_MARGIN), dpi);
	int view_top = LogicalToDevice((TOP_REGION_HEIGHT + VIEW_MARGIN), dpi);
	int view_right = rc.right - LogicalToDevice(VIEW_MARGIN, dpi);
	int view_bottom = rc.bottom - LogicalToDevice(VIEW_MARGIN, dpi);

	HDWP hdwp = BeginDeferWindowPos(2);
	hdwp = DeferWindowPos(hdwp, btn_close.hWnd, NULL, close_left, close_top,
			      LogicalToDevice(CLOSE_BUTTON_CX, dpi),
			      LogicalToDevice(CLOSE_BUTTON_CY, dpi),
			      SWP_NOACTIVATE | SWP_NOZORDER);
	hdwp = DeferWindowPos(hdwp, view_wnd.hwnd_, NULL, view_left, view_top,
			      view_right - view_left, view_bottom - view_top,
			      SWP_NOACTIVATE | SWP_NOZORDER);
	EndDeferWindowPos(hdwp);
}

std::wstring GetModuleFile(const char *file)
{
	char *path = obs_module_file(file);
	if (path) {
		std::wstring ret = TransUtf8ToUnicode(path);
		bfree(path);
		return ret;
	} else {
		return L"";
	}
}

void BrowserInteractionMain::UpdateImage(float dpi)
{
	blog(LOG_INFO, "Interaction update image by dpi(%f) title:%s", dpi,
	     utf8_title.c_str());

	std::string dir;
	if (dpi > 2.4) {
		dir = "images/x3/";
	} else if (dpi > 1.4) {
		dir = "images/x2/";
	} else {
		dir = "images/x1/";
	}

	std::string close_normal = dir + std::string(CLOSE_BTN_NORMAL);
	std::string close_hover = dir + std::string(CLOSE_BTN_HOVER);
	std::string close_click = dir + std::string(CLOSE_BTN_CLICK);
	std::string close_disable = dir + std::string(CLOSE_BTN_DISABLE);

	btn_close.SetImage(
		ImageManager::Instance()->LoadImageFile(
			GetModuleFile(close_normal.c_str()).c_str()),
		ImageManager::Instance()->LoadImageFile(
			GetModuleFile(close_hover.c_str()).c_str()),
		ImageManager::Instance()->LoadImageFile(
			GetModuleFile(close_click.c_str()).c_str()),
		ImageManager::Instance()->LoadImageFile(
			GetModuleFile(close_disable.c_str()).c_str()));
}

void BrowserInteractionMain::ShowUI()
{
	// from hide to show, set user's size
	if (!::IsWindowVisible(hWnd)) {
		float dpi = GetWindowScaleFactor(hWnd);

		RECT wrc;
		GetWindowRect(hWnd, &wrc);

		SetWindowSize(wrc.left, wrc.top, dpi);
	}

	// update position with main dialog
	assert(::IsWindow(prism_main));
	if (::IsWindow(prism_main)) {
		MovePosition();
	}

	::RedrawWindow(view_wnd.hwnd_, NULL, NULL, RDW_INTERNALPAINT);

	// Firstly bring to top, then show window
	BringWndToTop(hWnd);

	::ShowWindow(hWnd, SW_SHOW);
}

void BrowserInteractionMain::MovePosition()
{
	RECT prc;
	GetWindowRect(prism_main, &prc);
	float pdpi = GetWindowScaleFactor(prism_main);

	RECT rc;
	GetClientRect(hWnd, &rc);
	float dpi = GetWindowScaleFactor(hWnd);

	/*
	NOTE:
	Here when to calculate new position of interaction aligement with PRISM's main dialog, we should consider their dpi values are different.
	So, we should transform interaction's resolution to match PRISM's while getting its new lef/top.
	But we should use interaction's current resolution while moving its position, because it can receive WM_DPICHANGED later for resizing.
	*/

	int best_cx;
	int best_cy;
	if (pdpi > 0.f && dpi > 0.f) {
		best_cx = (float)RectWidth(rc) * (pdpi / dpi);
		best_cy = (float)RectHeight(rc) * (pdpi / dpi);
	} else {
		int best_cx = RectWidth(rc);
		int best_cy = RectHeight(rc);
	}

	int left = prc.left + (RectWidth(prc) - best_cx) / 2;
	int top = prc.top + (RectHeight(prc) - best_cy) / 2;

	bool top_checked = false;
	HMONITOR monitor = MonitorFromWindow(prism_main, MONITOR_DEFAULTTONULL);
	if (monitor) {
		MONITORINFOEX mi = {};
		mi.cbSize = sizeof(mi);
		if (GetMonitorInfo(monitor, &mi)) {
			top_checked = true;
			if (top < mi.rcMonitor.top) {
				top = mi.rcMonitor.top;
			}
		}
	}

	if (!top_checked) {
		if (top < prc.top) {
			top = prc.top;
		}
	}

	::MoveWindow(hWnd, left, top, RectWidth(rc), RectHeight(rc), TRUE);
}

void BrowserInteractionMain::DrawTitle(HDC &hdc, RECT &rc, float &dpi)
{
	std::wstring title = TransUtf8ToUnicode(utf8_title.c_str());
	if (title.empty()) {
		return;
	}

	const int font_height = LogicalToDevice(TITLE_FONT_SIZE, dpi);
	HFONT font = CreateCustomFont(font_height, font_list);
	HGDIOBJ old_font = ::SelectObject(hdc, font);

	RECT text_region;
	text_region.left = LogicalToDevice(TITLE_OFFSET_X, dpi);
	text_region.top = LogicalToDevice(TITLE_OFFSET_Y, dpi);
	text_region.bottom = LogicalToDevice(TOP_REGION_HEIGHT, dpi);
	text_region.right =
		rc.right - LogicalToDevice(CLOSE_BUTTON_CX * 2, dpi);

	::SetTextColor(hdc, TITLE_TEXT_COLOR);
	::SetBkMode(hdc, TRANSPARENT);
	::DrawTextW(hdc, title.c_str(), title.length(), &text_region,
		    DT_LEFT | DT_TOP | DT_END_ELLIPSIS);

	::SelectObject(hdc, old_font);
	if (font) {
		::DeleteObject(font);
	}
}

void BrowserInteractionMain::SetWindowSize(int left, int top, float dpi)
{
	if (user_size.cx > INTERACTION_SIZE_IGNORE &&
	    user_size.cy > INTERACTION_SIZE_IGNORE) {
		::MoveWindow(hWnd, left, top,
			     LogicalToDevice(user_size.cx, dpi),
			     LogicalToDevice(user_size.cy, dpi), TRUE);
	} else {
		::MoveWindow(hWnd, left, top,
			     LogicalToDevice(INTERACTION_DEFAULT_CX, dpi),
			     LogicalToDevice(INTERACTION_DEFAULT_CY, dpi),
			     TRUE);
	}
}
