#include "interaction_button.h"
#include "interaction_util.hpp"
#include "interaction_hdc.hpp"
#include <util/util.hpp>

//-------------------------------------------------------
LRESULT CALLBACK InteractionButton::InteractionButtonProc(HWND hWnd,
							  UINT message,
							  WPARAM wParam,
							  LPARAM lParam)
{
	HOOK_MOUSE_EVENT(hWnd, message);

	InteractionButton *self = GetUserDataPtr<InteractionButton *>(hWnd);

	switch (message) {
	case WM_ERASEBKGND:
		return TRUE;

	case WM_PAINT:
		if (self) {
			self->OnPaint();
		}
		break;

	case WM_LBUTTONDOWN:
		if (self) {
			self->OnLButtonDown();
		}
		break;

	case WM_LBUTTONUP:
		if (self) {
			self->OnLButtonUp();
		}
		break;

	case WM_MOUSEMOVE:
		if (self) {
			self->OnMouseMove();
		}
		break;

	case WM_MOUSELEAVE:
		if (self) {
			self->OnMouseLeave();
		}
		break;

	case WM_ENABLE:
		if (self) {
			self->OnWindowEnable((BOOL)wParam);
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//------------------------------------------------------------------------
InteractionButton::InteractionButton() {}

InteractionButton::~InteractionButton()
{
	assert(!::IsWindow(hWnd));
}

void InteractionButton::Create(HWND hp, int id)
{
	assert(!::IsWindow(hWnd));
	hWnd = ::CreateWindow(L"BUTTON", L"",
			      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
			      hp, reinterpret_cast<HMENU>(id),
			      GetModuleHandle(NULL), 0);

	if (!::IsWindow(hWnd)) {
		assert(false);
		return;
	}

	button_id = id;

	::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)InteractionButtonProc);
}

void InteractionButton::SetBkColor(DWORD normal, DWORD hover, DWORD click,
				   DWORD disable)
{
	bk_colors[ButtonNormal] = normal;
	bk_colors[ButtonHover] = hover;
	bk_colors[ButtonClick] = click;
	bk_colors[ButtonDisable] = disable;

	if (::IsWindow(hWnd)) {
		::InvalidateRect(hWnd, NULL, TRUE);
	}
}

void InteractionButton::SetImage(GDIP_IMAGE_PTR normal_image,
				 GDIP_IMAGE_PTR hover_image,
				 GDIP_IMAGE_PTR push_image,
				 GDIP_IMAGE_PTR disable_image)
{
	bk_images[ButtonNormal] = normal_image;
	bk_images[ButtonHover] = hover_image;
	bk_images[ButtonClick] = push_image;
	bk_images[ButtonDisable] = disable_image;

	if (::IsWindow(hWnd)) {
		::InvalidateRect(hWnd, NULL, TRUE);
	}
}

void InteractionButton::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		DoubleBufferHDC memdc(ps.hdc, rc);

		interaction::FillSolidRect(memdc.canvas_hdc, &rc,
					   bk_colors[button_state]);

		GDIP_IMAGE_PTR img = bk_images[button_state];
		if (img.get()) {
			Gdiplus::Graphics graphics(memdc.canvas_hdc);
			img->RenderImage(graphics, rc);
		}
	}

	EndPaint(hWnd, &ps);
}

void InteractionButton::OnLButtonDown()
{
	left_button_down = true;
	UpdateButtonState(ButtonClick);
}

void InteractionButton::OnLButtonUp()
{
	if (left_button_down) {
		left_button_down = false;

		if (interaction::IsCursorHitWnd(hWnd)) {
			::SendMessage(::GetParent(hWnd),
				      WM_INTERACTION_LBUTTON_CLICK, button_id,
				      (LPARAM)hWnd);
		}

		UpdateButtonState(ButtonNormal);
	}
}

void InteractionButton::OnMouseMove()
{
	if (left_button_down) {
		if (interaction::IsCursorHitWnd(hWnd)) {
			UpdateButtonState(ButtonClick);
		} else {
			UpdateButtonState(ButtonHover);
		}
	} else {
		UpdateButtonState(ButtonHover);
	}
}

void InteractionButton::OnMouseLeave()
{
	UpdateButtonState(ButtonNormal);
}

void InteractionButton::OnWindowEnable(BOOL enable)
{
	if (enable) {
		UpdateButtonState(ButtonNormal);
	} else {
		UpdateButtonState(ButtonDisable);
	}
}

void InteractionButton::UpdateButtonState(InteractionButtonState state)
{
	if (button_state != state) {
		button_state = state;
		if (::IsWindow(hWnd)) {
			::InvalidateRect(hWnd, NULL, TRUE);
		}
	}
}
