#include "cursor-capture.h"
#include "obs.h"
#include <log.h>

PLSCursorCapture::PLSCursorCapture() : texture(NULL), texture_width(0), texture_height(0) {}

PLSCursorCapture::~PLSCursorCapture()
{
	release_cursor_buffer();
	if (texture) {
		obs_enter_graphics();
		gs_texture_destroy(texture);
		texture = NULL;
		obs_leave_graphics();
	}
}

void PLSCursorCapture::release_cursor_buffer()
{
	if (cursor_info.bitmap) {
		bfree(cursor_info.bitmap);
		cursor_info.bitmap = NULL;
	}
}

bool PLSCursorCapture::cursor_capture()
{
	CURSORINFO ci = {0};
	HICON icon;

	ci.cbSize = sizeof(ci);

	if (!GetCursorInfo(&ci)) {
		cursor_info.visible = false;
		return false;
	}

	cursor_info.same_as_previous = false;

	memcpy(&cursor_info.pos, &ci.ptScreenPos, sizeof(cursor_info.pos));

	if (cursor_info.current_cursor == ci.hCursor) {
		cursor_info.same_as_previous = true;
		return true;
	}

	icon = CopyIcon(ci.hCursor);
	cursor_info.visible = cursor_capture_icon(icon);
	cursor_info.current_cursor = ci.hCursor;
	if ((ci.flags & CURSOR_SHOWING) == 0)
		cursor_info.visible = false;
	DestroyIcon(icon);
	return true;
}

bool PLSCursorCapture::cursor_capture_icon(HICON icon)
{
	ICONINFO ii;

	if (!icon)
		return false;

	if (!GetIconInfo(icon, &ii))
		return false;

	uint32_t width = 0;
	uint32_t height = 0;
	uint8_t *buffer = cursor_capture_icon_bitmap(&ii, width, height);
	if (width != 0 && height != 0 && (width != cursor_info.width || height != cursor_info.height)) {
		if (cursor_info.bitmap) {
			bfree(cursor_info.bitmap);
			cursor_info.bitmap = NULL;
		}
		cursor_info.bitmap = reinterpret_cast<uint8_t *>(bzalloc(width * height * 4));
		cursor_info.width = width;
		cursor_info.height = height;
	}

	if (buffer && cursor_info.bitmap) {
		memcpy(cursor_info.bitmap, buffer, width * height * 4);
		cursor_info.hotspot_x = ii.xHotspot;
		cursor_info.hotspot_y = ii.yHotspot;
		bfree(buffer);
		buffer = NULL;
	}

	DeleteObject(ii.hbmColor);
	DeleteObject(ii.hbmMask);

	return true;
}

uint8_t *PLSCursorCapture::cursor_capture_icon_bitmap(ICONINFO *ii, uint32_t &width, uint32_t &height)
{
	uint8_t *output = NULL;

	output = copy_from_color(ii, width, height);
	if (!output)
		output = copy_from_mask(ii, width, height);

	return output;
}

uint8_t *PLSCursorCapture::copy_from_color(ICONINFO *ii, uint32_t &width, uint32_t &height)
{
	BITMAP bmp_color;
	BITMAP bmp_mask;
	uint8_t *color;
	uint8_t *mask;

	color = get_bitmap_data(ii->hbmColor, &bmp_color);
	if (!color)
		return NULL;

	if (bmp_color.bmBitsPixel < 32) {
		bfree(color);
		return NULL;
	}

	mask = get_bitmap_data(ii->hbmMask, &bmp_mask);
	if (mask) {
		long pixels = bmp_color.bmHeight * bmp_color.bmWidth;

		if (!bitmap_has_alpha(color, pixels))
			apply_mask(color, mask, pixels);

		bfree(mask);
	}

	width = bmp_color.bmWidth;
	height = bmp_color.bmHeight;
	return color;
}

uint8_t *PLSCursorCapture::copy_from_mask(ICONINFO *ii, uint32_t &width, uint32_t &height)
{
	uint8_t *output;
	uint8_t *mask;
	long pixels;
	long bottom;
	BITMAP bmp;

	mask = get_bitmap_data(ii->hbmMask, &bmp);
	if (!mask)
		return NULL;

	bmp.bmHeight /= 2;

	pixels = bmp.bmHeight * bmp.bmWidth;
	output = reinterpret_cast<uint8_t *>(bzalloc(pixels * 4));

	bottom = bmp.bmWidthBytes * bmp.bmHeight;

	for (long i = 0; i < pixels; i++) {
		uint8_t alpha = bit_to_alpha(mask, i, false);
		uint8_t color = bit_to_alpha(mask + bottom, i, true);

		if (!alpha) {
			output[i * 4 + 3] = color;
		} else {
			*(uint32_t *)&output[i * 4] = !!color ? 0xFFFFFFFF : 0xFF000000;
		}
	}

	bfree(mask);

	width = bmp.bmWidth;
	height = bmp.bmHeight;
	return output;
}

uint8_t *PLSCursorCapture::get_bitmap_data(HBITMAP hbmp, BITMAP *bmp)
{
	if (GetObject(hbmp, sizeof(*bmp), bmp) != 0) {
		uint8_t *output;
		unsigned int size = (bmp->bmHeight * bmp->bmWidth * bmp->bmBitsPixel) / 8;

		output = reinterpret_cast<uint8_t *>(bmalloc(size));
		GetBitmapBits(hbmp, size, output);
		return output;
	}

	return NULL;
}

uint8_t PLSCursorCapture::bit_to_alpha(uint8_t *data, long pixel, bool invert)
{
	uint8_t pix_byte = data[pixel / 8];
	bool alpha = (pix_byte >> (7 - pixel % 8) & 1) != 0;

	if (invert) {
		return alpha ? 0xFF : 0;
	} else {
		return alpha ? 0 : 0xFF;
	}
}

bool PLSCursorCapture::bitmap_has_alpha(uint8_t *data, long num_pixels)
{
	for (long i = 0; i < num_pixels; i++) {
		if (data[i * 4 + 3] != 0) {
			return true;
		}
	}

	return false;
}

void PLSCursorCapture::apply_mask(uint8_t *color, uint8_t *mask, long num_pixels)
{
	for (long i = 0; i < num_pixels; i++)
		color[i * 4 + 3] = bit_to_alpha(mask, i, false);
}

bool PLSCursorCapture::update_texture()
{
	bool res = cursor_capture();
	if (!res)
		return false;
	if (!texture || texture_width != cursor_info.width || texture_height != cursor_info.height) {
		gs_texture_destroy(texture);
		texture = gs_texture_create(cursor_info.width, cursor_info.height, GS_BGRA, 1, NULL, GS_DYNAMIC);
		texture_width = cursor_info.width;
		texture_height = cursor_info.height;
	}
	gs_texture_set_image(texture, cursor_info.bitmap, cursor_info.width * 4, false);
	return true;
}

bool PLSCursorCapture::cursor_render(long x_offset, long y_offset, float x_scale, float y_scale, long width, long height)
{
	bool res = update_texture();
	if (!res && !cursor_info.same_as_previous)
		return false;
	long x = cursor_info.pos.x + x_offset;
	long y = cursor_info.pos.y + y_offset;
	long x_draw = x - cursor_info.hotspot_x;
	long y_draw = y - cursor_info.hotspot_y;

	if (x < 0 || x > width || y < 0 || y > height)
		return false;

	if (cursor_info.visible && !!texture) {
		gs_blend_state_push();
		gs_blend_function(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA);
		gs_enable_color(true, true, true, false);

		gs_matrix_push();
		gs_matrix_scale3f(x_scale, y_scale, 1.0f);
		obs_source_draw(texture, x_draw, y_draw, 0, 0, false);
		gs_matrix_pop();

		gs_enable_color(true, true, true, true);
		gs_blend_state_pop();
	}
	return true;
}
