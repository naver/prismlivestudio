#pragma once
#include <Windows.h>
#include <assert.h>

class DoubleBufferHDC {
public:
	DoubleBufferHDC(HDC hdc, const RECT &rc) : dest_hdc(hdc), dest_rect(rc)
	{
		canvas_hdc = ::CreateCompatibleDC(hdc);
		canvas_bmp = ::CreateCompatibleBitmap(hdc, (rc.right - rc.left),
						      (rc.bottom - rc.top));

		assert(canvas_hdc && canvas_bmp);

		old_bmp = ::SelectObject(canvas_hdc, canvas_bmp);
	}

	~DoubleBufferHDC()
	{
		::BitBlt(dest_hdc, dest_rect.left, dest_rect.top,
			 (dest_rect.right - dest_rect.left),
			 (dest_rect.bottom - dest_rect.top), canvas_hdc, 0, 0,
			 SRCCOPY);

		::SelectObject(canvas_hdc, old_bmp);

		if (canvas_bmp) {
			::DeleteObject(canvas_bmp);
		}

		if (canvas_hdc) {
			DeleteDC(canvas_hdc);
		}
	}

public:
	RECT dest_rect;
	HDC dest_hdc;

	HDC canvas_hdc;
	HBITMAP canvas_bmp;
	HGDIOBJ old_bmp;
};
