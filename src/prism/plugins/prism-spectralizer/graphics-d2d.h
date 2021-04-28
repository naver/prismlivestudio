#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DXGI1_2.h>
#include <d2d1.h>
#include <util/windows/ComPtr.hpp>
#include <vector>
#include "graphics/graphics.h"

class PLSGraphicsD2D {
public:
	explicit PLSGraphicsD2D(int width, int height);
	virtual ~PLSGraphicsD2D();

	bool upload_shared_texture(gs_texture *&gs_frame, int cw, int ch);

	void begin_draw();
	void end_draw();
	void draw_rectangle(D2D1_ROUNDED_RECT rRect, uint32_t rgba = 0xffffffff);

private:
	bool init();
	void uninit();
	bool init_device();
	bool init_factory(unsigned adapterIdx, IDXGIAdapter1 **ppAdapter);
	bool create_shared_texture(int w, int h);
	bool create_dxgi_render_target();

	bool update_shared_handle(gs_texture *gs_frame, int cw, int ch);
	bool device_removed();
	bool check_device();

protected:
	bool inited;
	int cw;
	int ch;
	uint32_t adapter;
	bool need_update_texture = false;
	gs_luid adapter_luid{};

	// Direct3D
	ComPtr<IDXGIFactory1> dxgi_factory;
	ComPtr<ID3D11Device> d3d11_device;
	ComPtr<ID3D11DeviceContext> d3d11_device_context;
	ComPtr<ID3D11Texture2D> shared_texture;
	D3D11_TEXTURE2D_DESC texture_desc;
	HANDLE shared_handle;

	// Direct2D
	ComPtr<ID2D1Factory> d2d1_factory;
	ComPtr<ID2D1RenderTarget> d2d1_render_target;
	ComPtr<ID2D1SolidColorBrush> d2d1_color_brush;
	ComPtr<ID2D1StrokeStyle> d2d1_stroke_style;
};
