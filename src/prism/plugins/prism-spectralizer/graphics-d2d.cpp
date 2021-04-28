#include "graphics-d2d.h"
#include <obs.h>
#include <log.h>
#include <assert.h>
#include <string>
#include <inttypes.h>
#include <util/platform.h>

#include <util/windows/win-version.h>

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "d2d1.lib")

#define write_log_d2d(log_level, format, ...) blog(log_level, "[PLSGraphicsD2D] " format, ##__VA_ARGS__)

#define debug(format, ...) write_log_d2d(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) write_log_d2d(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) write_log_d2d(LOG_WARNING, format, ##__VA_ARGS__)
#define error(format, ...) write_log_d2d(LOG_ERROR, format, ##__VA_ARGS__)

static const IID dxgiFactory2 = {0x50c83a1c, 0xe072, 0x4c48, {0x87, 0xb0, 0x36, 0x30, 0xfa, 0x36, 0xa6, 0xd0}};
const static D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
};

static inline void colorf_from_rgba(D2D1_COLOR_F *dst, uint32_t rgba)
{
	dst->r = (float)((double)(rgba & 0xFF) * (1.0 / 255.0));
	rgba >>= 8;
	dst->g = (float)((double)(rgba & 0xFF) * (1.0 / 255.0));
	rgba >>= 8;
	dst->b = (float)((double)(rgba & 0xFF) * (1.0 / 255.0));
	rgba >>= 8;
	dst->a = (float)((double)(rgba & 0xFF) * (1.0 / 255.0));
}

static inline uint32_t get_win_version()
{
	static uint32_t ver = 0;

	if (ver == 0) {
		struct win_version_info ver_info;

		get_win_ver(&ver_info);
		ver = (ver_info.major << 8) | ver_info.minor;
	}
	return ver;
}

PLSGraphicsD2D::PLSGraphicsD2D(int width, int height) : inited(false), dxgi_factory(), d3d11_device(), d3d11_device_context(), shared_texture(), shared_handle(), cw(width), ch(height)
{
	inited = init();
}

PLSGraphicsD2D::~PLSGraphicsD2D()
{
	uninit();
}

bool PLSGraphicsD2D::init()
{
	if (inited)
		uninit();

	info("Starting init d2d1 resources.");

	if (!init_device())
		return false;

	if (!create_shared_texture(cw, ch))
		return false;

	if (!create_dxgi_render_target())
		return false;

	inited = true;
	info("Finished init d2d1 resources.");
	return true;
}

void PLSGraphicsD2D::uninit()
{
	inited = false;
	shared_handle = 0;
	shared_texture.Release();
	d3d11_device_context.Release();
	d3d11_device.Release();
	dxgi_factory.Release();

	d2d1_factory.Release();
	d2d1_render_target.Release();
	d2d1_color_brush.Release();
	d2d1_stroke_style.Release();
	ZeroMemory(&texture_desc, sizeof(texture_desc));
	info("Destroyed d2d1 resources.");
}

bool PLSGraphicsD2D::upload_shared_texture(gs_texture *&gs_frame, int cw, int ch)
{
	if (!inited || !shared_texture.Get())
		return false;

	if (check_device() || update_shared_handle(gs_frame, cw, ch)) {
		if (gs_frame) {
			gs_texture_destroy(gs_frame);
			gs_frame = NULL;
		}
		gs_frame = gs_texture_open_shared((uint32_t)shared_handle);
		if (!gs_frame)
			return false;
	}

	return true;
}

void PLSGraphicsD2D::begin_draw()
{
	if (d2d1_render_target) {
		d2d1_render_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		d2d1_render_target->BeginDraw();
		d2d1_render_target->Clear();
	}
}

void PLSGraphicsD2D::end_draw()
{
	if (d2d1_render_target) {
		HRESULT hr = d2d1_render_target->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
			init();
			need_update_texture = true;
		}

		d3d11_device_context->Flush();
	}
}

void PLSGraphicsD2D::draw_rectangle(D2D1_ROUNDED_RECT rRect, uint32_t rgba)
{
	if (!d2d1_render_target)
		return;

	D2D1_COLOR_F color;
	colorf_from_rgba(&color, rgba);

	if (!d2d1_color_brush) {
		d2d1_render_target->CreateSolidColorBrush(color, d2d1_color_brush.Assign());
	} else {
		d2d1_color_brush->SetColor(color);
	}

	d2d1_render_target->FillRoundedRectangle(rRect, d2d1_color_brush.Get());
}

bool PLSGraphicsD2D::init_device()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d1_factory.Assign());
	if (FAILED(hr)) {
		error("Failed create direct2d factory.");
		return false;
	}

	obs_video_info ovi;
	obs_get_video_info(&ovi);

	ComPtr<IDXGIAdapter1> adapter_ptr;
	if (!init_factory(ovi.adapter, adapter_ptr.Assign())) {
		error("Failed init factory.");
		return false;
	}

	DXGI_ADAPTER_DESC desc;
	std::wstring adapterName = (adapter_ptr->GetDesc(&desc) == S_OK) ? desc.Description : L"<unknown>";

	char *adapterNameUTF8;
	os_wcs_to_utf8_ptr(adapterName.c_str(), 0, &adapterNameUTF8);
	info("Loading up D3D11 on adapter %s (%" PRIu32 ")", adapterNameUTF8, ovi.adapter);

	hr = D3D11CreateDevice(adapter_ptr.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION,
			       d3d11_device.Assign(), nullptr, d3d11_device_context.Assign());

	if (FAILED(hr)) {
		error("Failed create direct3d device.");
		return false;
	}

	hr = adapter_ptr->GetDesc(&desc);
	memset(&adapter_luid, 0, sizeof(adapter_luid));
	if (FAILED(hr))
		warn("Failed to get adapter (index %d) LUID, err %ld", ovi.adapter, hr);
	else {
		adapter_luid.low_part = desc.AdapterLuid.LowPart;
		adapter_luid.high_part = desc.AdapterLuid.HighPart;
	}

	adapter = ovi.adapter;

	return true;
}

bool PLSGraphicsD2D::init_factory(unsigned adapterIdx, IDXGIAdapter1 **ppAdapter)
{
	IID factoryIID = (get_win_version() >= 0x602) ? dxgiFactory2 : __uuidof(IDXGIFactory1);

	HRESULT hr = CreateDXGIFactory1(factoryIID, (void **)dxgi_factory.Assign());
	if (FAILED(hr)) {
		error("Failed create dxgi factory1.");
		return false;
	}

	hr = dxgi_factory->EnumAdapters1(adapterIdx, ppAdapter);
	if (FAILED(hr)) {
		error("Failed enunm adapters1 for dxgi factory1.");
		return false;
	}

	return true;
}

bool PLSGraphicsD2D::create_shared_texture(int w, int h)
{
	shared_handle = 0;
	shared_texture.Release();

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = cw = w;
	desc.Height = ch = h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.CPUAccessFlags = 0;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Usage = D3D11_USAGE_DEFAULT;
	//set share texture
	desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

	ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = d3d11_device->CreateTexture2D(&desc, NULL, shared_texture.Assign());
	if (!shared_texture) {
		error("Failed create texture2D.");
		return false;
	}

	ComPtr<IDXGIResource> dxgi_res;
	hr = shared_texture->QueryInterface(__uuidof(IDXGIResource), reinterpret_cast<LPVOID *>(dxgi_res.Assign()));
	if (!dxgi_res) {
		error("Failed query interface for shared texture2D.");
		return false;
	}

	hr = dxgi_res->GetSharedHandle(&shared_handle);

	if (!shared_handle) {
		error("Failed get shared handle for shared texture2D.");
		return false;
	}

	shared_texture->GetDesc(&texture_desc);
	return true;
}

bool PLSGraphicsD2D::create_dxgi_render_target()
{
	ComPtr<IDXGISurface1> dxgi_surface;
	auto hr = d3d11_device->OpenSharedResource(shared_handle, __uuidof(IDXGISurface1), reinterpret_cast<LPVOID *>(dxgi_surface.Assign()));
	if (FAILED(hr)) {
		error("Failed open shared resource.");
		return false;
	}

	auto dsProps = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	hr = d2d1_factory->CreateDxgiSurfaceRenderTarget(dxgi_surface.Get(), &dsProps, &d2d1_render_target);
	if (FAILED(hr)) {
		error("Failed create dxgi surface render target.");
		return false;
	}

	hr = d2d1_render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), d2d1_color_brush.Assign());
	if (FAILED(hr)) {
		error("Failed create solid color brush.");
	}

	hr = d2d1_factory->CreateStrokeStyle(D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 1.0f,
									 //0.5f,
									 D2D1_DASH_STYLE_SOLID, // 有多种风格可以设置（dash,dot....)
									 10.0f),
					     NULL, 0, d2d1_stroke_style.Assign());
	if (FAILED(hr)) {
		error("Failed create stroke style for d2d1factory.");
	}

	return true;
}

bool PLSGraphicsD2D::update_shared_handle(gs_texture *gs_frame, int cw, int ch)
{
	uint32_t cx = gs_frame ? gs_texture_get_width(gs_frame) : 0;
	uint32_t cy = gs_frame ? gs_texture_get_height(gs_frame) : 0;

	if (!gs_frame || cx != texture_desc.Width || cy != texture_desc.Height) {
		if (!create_shared_texture(cw, ch) || !create_dxgi_render_target())
			return false;
		return true;
	} else if (need_update_texture) {
		need_update_texture = false;
		return true;
	} else
		return false;
}

bool PLSGraphicsD2D::check_device()
{
	obs_video_info ovi;
	obs_get_video_info(&ovi);

	gs_luid luid;
	memset(&luid, 0, sizeof(luid));
	gs_adapter_get_luid(&luid);
	bool ulid_changed = (adapter_luid.high_part == luid.high_part && adapter_luid.low_part == luid.low_part) ? false : true;

	if (ulid_changed && !luid.low_part && !luid.high_part) {
		if (!init()) {
			return false;
		}
		adapter_luid.high_part = luid.high_part;
		adapter_luid.low_part = luid.low_part;
		return true;
	}

	if (ulid_changed || ovi.adapter != adapter) {
		if (!init()) {
			return false;
		}
		return true;
	}
	return false;
}
