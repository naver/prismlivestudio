#pragma once
#include <Windows.h>
#include <commctrl.h>
#include <cmath>
#include <string>
#include <vector>
#include <util/platform.h>
#include <util/dstr.h>
#include <assert.h>

#define LOWORD_EX(l) ((int)(((int)(l)) & 0xffff))
#define HIWORD_EX(l) ((int)((((int)(l)) >> 16) & 0xffff))

#define HOOK_MOUSE_EVENT(hWnd, msg)                   \
	switch (msg) {                                \
	case WM_LBUTTONDOWN:                          \
	case WM_RBUTTONDOWN:                          \
	case WM_MBUTTONDOWN:                          \
		SetCapture(hWnd);                     \
		break;                                \
	case WM_LBUTTONUP:                            \
	case WM_RBUTTONUP:                            \
	case WM_MBUTTONUP:                            \
		ReleaseCapture();                     \
		break;                                \
	case WM_MOUSEMOVE: {                          \
		TRACKMOUSEEVENT tme;                  \
		tme.cbSize = sizeof(TRACKMOUSEEVENT); \
		tme.dwFlags = TME_HOVER | TME_LEAVE;  \
		tme.dwHoverTime = HOVER_DEFAULT;      \
		tme.hwndTrack = hWnd;                 \
		_TrackMouseEvent(&tme);               \
		break;                                \
	}                                             \
	}

enum INTERACTION_UI_IDC {
	UI_IDC_BUTTON_CLOSE = 200,
};

enum INTERACTION_MESSAGE {
	// wp:buttonid, lp:buttonHWND
	WM_INTERACTION_LBUTTON_CLICK = WM_USER + 1,
	// wp:char* (bfree), lp:no-use
	WM_INTERACTION_SET_TITLE,
};

template<typename T> T GetUserDataPtr(HWND hWnd)
{
	return reinterpret_cast<T>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

//---------------------------- namespace interaction begin ------------------------------------
namespace interaction {
static int RectWidth(RECT &rc)
{
	return rc.right - rc.left;
}

static int RectHeight(RECT &rc)
{
	return rc.bottom - rc.top;
}

static float GetDeviceScaleFactor()
{
	HDC screen_dc = ::GetDC(NULL);
	int dpi_x = GetDeviceCaps(screen_dc, LOGPIXELSX);
	float scale_factor = static_cast<float>(dpi_x) / 96.0f;
	::ReleaseDC(NULL, screen_dc);

	return scale_factor;
}

static float GetWindowScaleFactor(HWND hwnd)
{
	if (::IsWindow(hwnd)) {
		typedef UINT(WINAPI * GetDpiForWindowPtr)(HWND);
		static GetDpiForWindowPtr func_ptr =
			reinterpret_cast<GetDpiForWindowPtr>(
				GetProcAddress(GetModuleHandle(L"user32.dll"),
					       "GetDpiForWindow"));
		if (func_ptr)
			return static_cast<float>(func_ptr(hwnd)) / 96.0f;
	}

	return GetDeviceScaleFactor();
}

static int LogicalToDevice(int src, float device_scale_factor)
{
	float scaled_val = static_cast<float>(src) * device_scale_factor;
	return static_cast<int>(std::floor(scaled_val));
}

static void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
{
	COLORREF old_color = ::SetBkColor(hdc, clr);
	if (old_color != CLR_INVALID) {
		::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
		::SetBkColor(hdc, old_color);
	}
}

static void DrawBorder(HDC hdc, RECT rc, int border_size, COLORREF clr)
{
	RECT left_line = {0, 0, border_size, rc.bottom};
	RECT top_line = {0, 0, rc.right, border_size};
	RECT right_line = {rc.right - border_size, 0, rc.right, rc.bottom};
	RECT bottom_line = {0, rc.bottom - border_size, rc.right, rc.bottom};

	FillSolidRect(hdc, &left_line, clr);
	FillSolidRect(hdc, &top_line, clr);
	FillSolidRect(hdc, &right_line, clr);
	FillSolidRect(hdc, &bottom_line, clr);
}

static bool RegisterInteractionClass(const wchar_t *pClassName, WNDPROC WndProc,
				     HICON icon = NULL)
{
	WNDCLASSEX wcx;
	memset(&wcx, 0, sizeof(wcx));
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.hInstance = GetModuleHandle(NULL);
	wcx.lpszClassName = pClassName;
	wcx.lpfnWndProc = WndProc;
	wcx.hIcon = icon;
	wcx.hIconSm = NULL;

	ATOM atom = RegisterClassEx(&wcx);
	if (0 == atom) {
		return false;
	} else {
		return true;
	}
}

static HFONT CreateCustomFont(int font_height, std::vector<std::string> &fonts)
{
	if (font_height > 0) {
		font_height = -font_height;
	}

	size_t count = fonts.size();
	for (size_t i = 0; i < count; i++) {
		HFONT font = ::CreateFontA(font_height, 0, 0, 0, FW_DONTCARE,
					   FALSE, FALSE, FALSE, DEFAULT_CHARSET,
					   OUT_OUTLINE_PRECIS,
					   CLIP_DEFAULT_PRECIS,
					   CLEARTYPE_QUALITY, VARIABLE_PITCH,
					   fonts[i].c_str());
		if (font) {
			// Note : Should be release by DeleteObject()
			return font;
		}
	}

	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (font) {
		// It is not necessary (but it is not harmful) to delete stock objects by calling DeleteObject.
		return font;
	}

	font = (HFONT)GetStockObject(SYSTEM_FONT);
	if (font) {
		// It is not necessary (but it is not harmful) to delete stock objects by calling DeleteObject.
		return font;
	}

	assert(false);
	return NULL;
}

static void BringWndToTop(HWND hWnd)
{
	if (::IsWindow(hWnd) == FALSE)
		return;

	DWORD fpid = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
	DWORD ctid = GetCurrentThreadId();

	AttachThreadInput(ctid, fpid, TRUE);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetForegroundWindow(hWnd);
	AttachThreadInput(ctid, fpid, FALSE);
}

static inline void GetScaleAndCenterPos(int frame_cx, int frame_cy,
					int window_cx, int window_cy, int &x,
					int &y, float &scale)
{
	int new_cx, new_cy;

	double window_aspect = double(window_cx) / double(window_cy);
	double base_aspect = double(frame_cx) / double(frame_cy);

	if (window_aspect > base_aspect) {
		scale = float(window_cy) / float(frame_cy);
		new_cx = int(double(window_cy) * base_aspect);
		new_cy = window_cy;
	} else {
		scale = float(window_cx) / float(frame_cx);
		new_cx = window_cx;
		new_cy = int(float(window_cx) / base_aspect);
	}

	x = window_cx / 2 - new_cx / 2;
	y = window_cy / 2 - new_cy / 2;
}

static std::wstring TransUtf8ToUnicode(const char *utf8)
{
	std::wstring ret = L"";

	wchar_t *unicode_title = NULL;
	os_utf8_to_wcs_ptr(utf8, 0, &unicode_title);

	if (unicode_title) {
		ret = unicode_title;
		bfree(unicode_title);
	}

	return ret;
}

static bool IsCursorHitWnd(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);

	return (::WindowFromPoint(pt) == hWnd);
}

static bool IsFocusInWnd(HWND top_hwnd)
{
	HWND current = ::GetFocus();
	while (::IsWindow(current)) {
		if (current == top_hwnd) {
			return true;
		}

		if (WS_POPUP & ::GetWindowLong(current, GWL_STYLE)) {
			break;
		}

		current = ::GetParent(current);
	}

	return false;
}

};
//---------------------------- namespace interaction end ------------------------------------
