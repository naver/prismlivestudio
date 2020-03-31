#include "gdi-data.h"

PLSGdiData::PLSGdiData(HWND hwnd) : hwn_window(hwnd), source_dc(), dest_dc(), bitmap(), buffer(NULL)
{
	InitializeCriticalSection(&cs);
}

PLSGdiData::~PLSGdiData()
{
	if (bitmap)
		::DeleteObject(bitmap);

	if (dest_dc)
		::DeleteDC(dest_dc);

	if (hwn_window) {
		if (source_dc)
			::ReleaseDC(hwn_window, source_dc);
	} else {
		if (source_dc)
			::DeleteDC(source_dc);
	}

	DeleteCriticalSection(&cs);
}

void PLSGdiData::lock()
{
	EnterCriticalSection(&cs);
}

void PLSGdiData::unlock()
{
	LeaveCriticalSection(&cs);
}
