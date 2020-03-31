#pragma once
#include <windows.h>
#include <stdint.h>
#include "obs.h"

struct capture_cursor_info {
	uint8_t *bitmap;
	HCURSOR current_cursor;
	bool same_as_previous;
	bool visible;
	long hotspot_x;
	long hotspot_y;
	uint32_t width;
	uint32_t height;
	POINT pos;
	capture_cursor_info()
	{
		same_as_previous = false;
		bitmap = NULL;
		visible = false;
		hotspot_x = 0;
		hotspot_y = 0;
		width = 0;
		height = 0;
		pos = {0};
	}
};

class PLSCursorCapture {
public:
	PLSCursorCapture();
	~PLSCursorCapture();

	bool cursor_render(long x_offset, long y_offset, float x_scale, float y_scale, long width, long height);

private:
	bool cursor_capture_icon(HICON icon);
	uint8_t *cursor_capture_icon_bitmap(ICONINFO *ii, uint32_t &width, uint32_t &height);
	uint8_t *copy_from_color(ICONINFO *ii, uint32_t &width, uint32_t &height);
	uint8_t *copy_from_mask(ICONINFO *ii, uint32_t &width, uint32_t &height);
	uint8_t *get_bitmap_data(HBITMAP hbmp, BITMAP *bmp);
	uint8_t bit_to_alpha(uint8_t *data, long pixel, bool invert);
	bool bitmap_has_alpha(uint8_t *data, long num_pixels);
	void apply_mask(uint8_t *color, uint8_t *mask, long num_pixels);

	bool update_texture();
	bool cursor_capture();
	void release_cursor_buffer();

private:
	capture_cursor_info cursor_info;
	int texture_width;
	int texture_height;
	gs_texture *texture;
};
