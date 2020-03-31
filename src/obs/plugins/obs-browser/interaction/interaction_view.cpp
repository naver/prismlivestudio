#include "interaction_view.h"
#include "interaction_util.hpp"
#include "interaction_hdc.hpp"
#include "interaction_main.h"
#include <util/util.hpp>

using namespace interaction;

#define INTERACTION_CLASS_VIEW L"PrismInteractionClassName_View"

//-------------------------------------------------------
void InteractionView::RegisterClassName()
{
	if (!RegisterInteractionClass(INTERACTION_CLASS_VIEW,
				      InteractionViewProc, NULL)) {
		assert(false);
		blog(LOG_ERROR,
		     "Failed to register class for interaction view. error:%u",
		     GetLastError());
	}
}

void InteractionView::UnregisterClassName()
{
	::UnregisterClass(INTERACTION_CLASS_VIEW, GetModuleHandle(NULL));
}

LRESULT CALLBACK InteractionView::InteractionViewProc(HWND hWnd, UINT message,
						      WPARAM wParam,
						      LPARAM lParam)
{
	HOOK_MOUSE_EVENT(hWnd, message);

	InteractionView *self = GetUserDataPtr<InteractionView *>(hWnd);

	switch (message) {
	case WM_DESTROY:
		if (self) {
			self->OnDestroy();
		}
		break;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_PAINT:
		if (self) {
			self->OnPaint();
		}
		break;

	case WM_SETCURSOR:
		return TRUE;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
	case WM_MOUSEWHEEL:
		if (self) {
			self->OnMouseEvent(message, wParam, lParam);
		}
		break;

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		if (self) {
			self->OnFocus(message == WM_SETFOCUS);
		}
		break;

	case WM_CAPTURECHANGED:
	case WM_CANCELMODE:
		if (self) {
			self->OnCaptureLost();
		}
		break;

	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		if (self) {
			self->OnKeyEvent(message, wParam, lParam);
		}
		break;

	case WM_IME_SETCONTEXT:
		if (self) {
			self->OnIMESetContext(message, wParam, lParam);
			return 0;
		}
		break;

	case WM_IME_STARTCOMPOSITION:
		if (self) {
			self->OnIMEStartComposition();
			return 0;
		}
		break;

	case WM_IME_COMPOSITION:
		if (self) {
			self->OnIMEComposition(message, wParam, lParam);
			return 0;
		}
		break;

	case WM_IME_ENDCOMPOSITION:
		if (self) {
			self->OnIMECancelCompositionEvent();
			// Never return here.
			// Let WTL call::DefWindowProc() and release its resources.
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//------------------------------------------------------------------------
InteractionView::InteractionView(BrowserInteractionMain &interaction_main)
	: main_ui_(interaction_main)
{
	ResetInteraction();
}

InteractionView::~InteractionView()
{
	assert(!::IsWindow(hwnd_));
}

void InteractionView::Create(HWND hp)
{
	assert(!::IsWindow(hwnd_));

	hwnd_ = ::CreateWindow(
		INTERACTION_CLASS_VIEW, L"",
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0,
		0, 0, hp, NULL, GetModuleHandle(NULL), NULL);

	if (!::IsWindow(hwnd_)) {
		blog(LOG_ERROR, "Failed to create interaction view. error:%u",
		     GetLastError());

		assert(false);
		return;
	}

	ime_handler_.reset(new client::OsrImeHandlerWin(hwnd_));

	::SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);
	::SetWindowLongPtr(hwnd_, GWLP_WNDPROC, (LONG_PTR)InteractionViewProc);
}

void InteractionView::ResetInteraction()
{
	last_mouse_pos_ = {0, 0};
	current_mouse_pos_ = {0, 0};
	mouse_rotation_ = false;
	mouse_tracking_ = false;
	last_click_x_ = 0;
	last_click_y_ = 0;
	last_click_button_ = MBT_LEFT;
	last_click_count_ = 0;
	last_click_time_ = 0;
	last_mouse_down_on_view_ = false;
}

CefRect LogicalToDevice(const CefRect &value, float device_scale_factor)
{
	return CefRect(LogicalToDevice(value.x, device_scale_factor),
		       LogicalToDevice(value.y, device_scale_factor),
		       LogicalToDevice(value.width, device_scale_factor),
		       LogicalToDevice(value.height, device_scale_factor));
}

void InteractionView::OnImeCompositionRangeChanged(
	CefRefPtr<CefBrowser> browser, const CefRange &selection_range,
	const CefRenderHandler::RectList &character_bounds)
{
	DCHECK(CefCurrentlyOn(TID_UI));

	if (ime_handler_) {
		RECT rc;
		GetClientRect(hwnd_, &rc);

		int left, top;
		float scale;
		GetScaleAndCenterPos(source_cx_, source_cy_, RectWidth(rc),
				     RectHeight(rc), left, top, scale);

		// Convert from view coordinates to device coordinates.
		CefRenderHandler::RectList device_bounds;
		CefRenderHandler::RectList::const_iterator it =
			character_bounds.begin();
		for (; it != character_bounds.end(); ++it) {
			CefRect temp = LogicalToDevice(*it, scale);
			temp.x += left;
			temp.y += top;
			device_bounds.push_back(temp);
		}

		ime_handler_->ChangeCompositionRange(selection_range,
						     device_bounds);
	}
}

void InteractionView::OnDestroy()
{
	source_cx_ = 0;
	source_cy_ = 0;
	browser_ = NULL;
	ime_handler_.reset();
}

void InteractionView::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd_, &ps);

	if (browser_) {
		browser_->GetHost()->Invalidate(PET_VIEW);
	}

	if (main_ui_.IsResizing()) {
		RECT rc;
		GetClientRect(hwnd_, &rc);

		int left, top;
		float scale;
		GetScaleAndCenterPos(source_cx_, source_cy_, RectWidth(rc),
				     RectHeight(rc), left, top, scale);
		RECT center;
		center.left = left;
		center.top = top;
		center.right = left + (scale * float(source_cx_));
		center.bottom = top + (scale * float(source_cy_));

		DoubleBufferHDC memdc(ps.hdc, rc);
		FillSolidRect(memdc.canvas_hdc, &rc, VIEW_BK_RENDER_COLOR);
		FillSolidRect(memdc.canvas_hdc, &center, VIEW_BK_RESIZE_COLOR);
	}

	EndPaint(hwnd_, &ps);
}

// Helper function to detect mouse messages coming from emulation of touch
// events. These should be ignored.
bool IsMouseEventFromTouch(UINT message)
{
#define MOUSEEVENTF_FROMTOUCH 0xFF515700
	return (message >= WM_MOUSEFIRST) && (message <= WM_MOUSELAST) &&
	       (GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCH) ==
		       MOUSEEVENTF_FROMTOUCH;
}

POINT InteractionView::TransMousePointToCef(const POINT &client_point)
{
	RECT rc;
	GetClientRect(hwnd_, &rc);

	int left, top;
	float scale;
	GetScaleAndCenterPos(source_cx_, source_cy_, RectWidth(rc),
			     RectHeight(rc), left, top, scale);

	if (scale > 0.f) {
		POINT cef_point;
		cef_point.x = float(client_point.x - left) / scale;
		cef_point.y = float(client_point.y - top) / scale;
		return cef_point;
	} else {
		return client_point;
	}
}

static bool IsKeyDown(WPARAM wparam)
{
	return (GetKeyState(wparam) & 0x8000) != 0;
}

void InteractionView::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsMouseEventFromTouch(message))
		return;

	CefRefPtr<CefBrowserHost> browser_host;
	if (browser_)
		browser_host = browser_->GetHost();

	POINT screen_point, client_point, browser_point;

	GetCursorPos(&screen_point);

	client_point = screen_point;
	ScreenToClient(hwnd_, &client_point);

	browser_point = TransMousePointToCef(client_point);

	LONG currentTime = 0;
	bool cancelPreviousClick = false;
	float device_scale_factor_ = GetWindowScaleFactor(hwnd_);

	if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN ||
	    message == WM_MBUTTONDOWN || message == WM_MOUSEMOVE ||
	    message == WM_MOUSELEAVE) {
		currentTime = GetMessageTime();
		int x = client_point.x;
		int y = client_point.y;
		cancelPreviousClick =
			(abs(last_click_x_ - x) >
			 (GetSystemMetrics(SM_CXDOUBLECLK) / 2)) ||
			(abs(last_click_y_ - y) >
			 (GetSystemMetrics(SM_CYDOUBLECLK) / 2)) ||
			((currentTime - last_click_time_) >
			 GetDoubleClickTime());
		if (cancelPreviousClick &&
		    (message == WM_MOUSEMOVE || message == WM_MOUSELEAVE)) {
			last_click_count_ = 0;
			last_click_x_ = 0;
			last_click_y_ = 0;
			last_click_time_ = 0;
		}
	}

	switch (message) {
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN: {
		::SetCapture(hwnd_);
		::SetFocus(hwnd_);
		int x = client_point.x;
		int y = client_point.y;
		if (wParam & MK_SHIFT) {
			// Start rotation effect.
			last_mouse_pos_.x = current_mouse_pos_.x = x;
			last_mouse_pos_.y = current_mouse_pos_.y = y;
			mouse_rotation_ = true;
		} else {
			CefBrowserHost::MouseButtonType btnType =
				(message == WM_LBUTTONDOWN
					 ? MBT_LEFT
					 : (message == WM_RBUTTONDOWN
						    ? MBT_RIGHT
						    : MBT_MIDDLE));
			if (!cancelPreviousClick &&
			    (btnType == last_click_button_)) {
				++last_click_count_;
			} else {
				last_click_count_ = 1;
				last_click_x_ = x;
				last_click_y_ = y;
			}
			last_click_time_ = currentTime;
			last_click_button_ = btnType;

			if (browser_host) {
				CefMouseEvent mouse_event;
				mouse_event.x = browser_point.x;
				mouse_event.y = browser_point.y;
				last_mouse_down_on_view_ = true;
				mouse_event.modifiers =
					GetCefMouseModifiers(wParam);
				browser_host->SendMouseClickEvent(
					mouse_event, btnType, false,
					last_click_count_);
			}
		}
	} break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		if (GetCapture() == hwnd_)
			ReleaseCapture();
		if (mouse_rotation_) {
			// End rotation effect.
			mouse_rotation_ = false;
			//render_handler_->SetSpin(0, 0);
		} else {
			CefBrowserHost::MouseButtonType btnType =
				(message == WM_LBUTTONUP
					 ? MBT_LEFT
					 : (message == WM_RBUTTONUP
						    ? MBT_RIGHT
						    : MBT_MIDDLE));
			if (browser_host) {
				CefMouseEvent mouse_event;
				mouse_event.x = browser_point.x;
				mouse_event.y = browser_point.y;
				mouse_event.modifiers =
					GetCefMouseModifiers(wParam);
				browser_host->SendMouseClickEvent(
					mouse_event, btnType, true,
					last_click_count_);
			}
		}
		break;

	case WM_MOUSEMOVE: {
		if (mouse_rotation_) {
			// Apply rotation effect.
			current_mouse_pos_.x = client_point.x;
			current_mouse_pos_.y = client_point.y;
			//render_handler_->IncrementSpin(current_mouse_pos_.x - last_mouse_pos_.x, current_mouse_pos_.y - last_mouse_pos_.y);
			last_mouse_pos_.x = current_mouse_pos_.x;
			last_mouse_pos_.y = current_mouse_pos_.y;
		} else {
			if (!mouse_tracking_) {
				// Start tracking mouse leave. Required for the WM_MOUSELEAVE event to
				// be generated.
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hwnd_;
				TrackMouseEvent(&tme);
				mouse_tracking_ = true;
			}

			if (browser_host) {
				CefMouseEvent mouse_event;
				mouse_event.x = browser_point.x;
				mouse_event.y = browser_point.y;
				mouse_event.modifiers =
					GetCefMouseModifiers(wParam);
				browser_host->SendMouseMoveEvent(mouse_event,
								 false);
			}
		}
		break;
	}

	case WM_MOUSELEAVE: {
		if (mouse_tracking_) {
			// Stop tracking mouse leave.
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE & TME_CANCEL;
			tme.hwndTrack = hwnd_;
			TrackMouseEvent(&tme);
			mouse_tracking_ = false;
		}

		if (browser_host) {
			// Determine the cursor position in screen coordinates.
			CefMouseEvent mouse_event;
			mouse_event.x = browser_point.x;
			mouse_event.y = browser_point.y;
			mouse_event.modifiers = GetCefMouseModifiers(wParam);
			browser_host->SendMouseMoveEvent(mouse_event, true);
		}
	} break;

	case WM_MOUSEWHEEL:
		if (browser_host) {
			if (::WindowFromPoint(screen_point) != hwnd_) {
				break;
			}

			int delta = GET_WHEEL_DELTA_WPARAM(wParam);

			CefMouseEvent mouse_event;
			mouse_event.x = browser_point.x;
			mouse_event.y = browser_point.y;
			mouse_event.modifiers = GetCefMouseModifiers(wParam);
			browser_host->SendMouseWheelEvent(
				mouse_event, IsKeyDown(VK_SHIFT) ? delta : 0,
				!IsKeyDown(VK_SHIFT) ? delta : 0);
		}
		break;
	}
}

void InteractionView::OnFocus(bool setFocus)
{
	if (browser_)
		browser_->GetHost()->SendFocusEvent(setFocus);
}

void InteractionView::OnCaptureLost()
{
	if (mouse_rotation_)
		return;

	if (browser_)
		browser_->GetHost()->SendCaptureLostEvent();
}

void InteractionView::OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!browser_)
		return;

	CefKeyEvent event;
	event.windows_key_code = wParam;
	event.native_key_code = lParam;
	event.is_system_key = message == WM_SYSCHAR ||
			      message == WM_SYSKEYDOWN ||
			      message == WM_SYSKEYUP;

	if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
		event.type = KEYEVENT_RAWKEYDOWN;
	else if (message == WM_KEYUP || message == WM_SYSKEYUP)
		event.type = KEYEVENT_KEYUP;
	else
		event.type = KEYEVENT_CHAR;
	event.modifiers = GetCefKeyboardModifiers(wParam, lParam);

	browser_->GetHost()->SendKeyEvent(event);
}

void InteractionView::OnIMESetContext(UINT message, WPARAM wParam,
				      LPARAM lParam)
{
	// We handle the IME Composition Window ourselves (but let the IME Candidates
	// Window be handled by IME through DefWindowProc()), so clear the
	// ISC_SHOWUICOMPOSITIONWINDOW flag:
	lParam &= ~ISC_SHOWUICOMPOSITIONWINDOW;
	::DefWindowProc(hwnd_, message, wParam, lParam);

	// Create Caret Window if required
	if (ime_handler_) {
		ime_handler_->CreateImeWindow();
		ime_handler_->MoveImeWindow();
	}
}

void InteractionView::OnIMEStartComposition()
{
	if (ime_handler_) {
		ime_handler_->CreateImeWindow();
		ime_handler_->MoveImeWindow();
		ime_handler_->ResetComposition();
	}
}

void InteractionView::OnIMEComposition(UINT message, WPARAM wParam,
				       LPARAM lParam)
{
	if (browser_ && ime_handler_) {
		CefString cTextStr;
		if (ime_handler_->GetResult(lParam, cTextStr)) {
			// Send the text to the browser. The |replacement_range| and
			// |relative_cursor_pos| params are not used on Windows, so provide
			// default invalid values.
			browser_->GetHost()->ImeCommitText(
				cTextStr, CefRange(UINT32_MAX, UINT32_MAX), 0);
			ime_handler_->ResetComposition();
			// Continue reading the composition string - Japanese IMEs send both
			// GCS_RESULTSTR and GCS_COMPSTR.
		}

		std::vector<CefCompositionUnderline> underlines;
		int composition_start = 0;

		if (ime_handler_->GetComposition(lParam, cTextStr, underlines,
						 composition_start)) {
			// Send the composition string to the browser. The |replacement_range|
			// param is not used on Windows, so provide a default invalid value.
			browser_->GetHost()->ImeSetComposition(
				cTextStr, underlines,
				CefRange(UINT32_MAX, UINT32_MAX),
				CefRange(composition_start,
					 static_cast<int>(composition_start +
							  cTextStr.length())));

			// Update the Candidate Window position. The cursor is at the end so
			// subtract 1. This is safe because IMM32 does not support non-zero-width
			// in a composition. Also,  negative values are safely ignored in
			// MoveImeWindow
			ime_handler_->UpdateCaretPosition(composition_start -
							  1);
		} else {
			OnIMECancelCompositionEvent();
		}
	}
}

void InteractionView::OnIMECancelCompositionEvent()
{
	if (browser_ && ime_handler_) {
		browser_->GetHost()->ImeCancelComposition();
		ime_handler_->ResetComposition();
		ime_handler_->DestroyImeWindow();
	}
}

int InteractionView::GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam)
{
	int modifiers = 0;
	if (IsKeyDown(VK_SHIFT))
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (IsKeyDown(VK_CONTROL))
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (IsKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;

	// Low bit set from GetKeyState indicates "toggled".
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;

	switch (wparam) {
	case VK_RETURN:
		if ((lparam >> 16) & KF_EXTENDED)
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		if (!((lparam >> 16) & KF_EXTENDED))
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_NUMLOCK:
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
	case VK_DIVIDE:
	case VK_MULTIPLY:
	case VK_SUBTRACT:
	case VK_ADD:
	case VK_DECIMAL:
	case VK_CLEAR:
		modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_SHIFT:
		if (IsKeyDown(VK_LSHIFT))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RSHIFT))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_CONTROL:
		if (IsKeyDown(VK_LCONTROL))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RCONTROL))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_MENU:
		if (IsKeyDown(VK_LMENU))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RMENU))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_LWIN:
		modifiers |= EVENTFLAG_IS_LEFT;
		break;
	case VK_RWIN:
		modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	}
	return modifiers;
}

int InteractionView::GetCefMouseModifiers(WPARAM wparam)
{
	int modifiers = 0;
	if (wparam & MK_CONTROL)
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (wparam & MK_SHIFT)
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (IsKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;
	if (wparam & MK_LBUTTON)
		modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
	if (wparam & MK_MBUTTON)
		modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
	if (wparam & MK_RBUTTON)
		modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

	// Low bit set from GetKeyState indicates "toggled".
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;
	return modifiers;
}
