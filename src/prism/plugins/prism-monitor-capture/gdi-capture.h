#pragma once
#include <Windows.h>
#include "gdi-data.h"
#include "obs.h"

class PLSGdiCapture {
public:
	PLSGdiCapture();
	~PLSGdiCapture();

	bool check_init(int cx, int cy, HWND hWnd = 0);
	void capture(int bkOffsetX, int bkOffsetY, bool bDrawCursor, int curOffsetX, int curOffsetY, float fZoom);
	void uninit();
	gs_texture *get_texture() { return texture; };
	void get_width_height(int &width, int &height);

protected:
	void draw_window(int offset_x, int offset_y);
	void draw_window_cursor(HDC &mem_dc, LONG offset_x, LONG offset_y, float zoom);

private:
	HWND hwnd_window;
	int width;
	int height;
	PLSGdiData *gdi_data_handle;
	gs_texture *texture;
	int texture_width;
	int texture_height;
};
