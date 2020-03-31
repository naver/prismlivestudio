#pragma once
#include <Windows.h>

class PLSGdiData {
public:
	PLSGdiData(HWND hwnd);
	~PLSGdiData();

public:
	void lock();
	void unlock();

public:
	CRITICAL_SECTION cs;
	HWND hwn_window;
	HDC source_dc;
	HDC dest_dc;
	BYTE *buffer;
	HBITMAP bitmap;
};
