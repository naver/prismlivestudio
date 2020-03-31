#include "monitor-duplicator-core.h"
#include "window-version.h"
#include <log.h>
#include <assert.h>

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D11.lib")

static const IID dxgiFactory2 = {0x50c83a1c, 0xe072, 0x4c48, {0x87, 0xb0, 0x36, 0x30, 0xfa, 0x36, 0xa6, 0xd0}};
const static D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
};

//------------------------------------------------------------------------
PLSDuplicatorCore::PLSDuplicatorCore(DuplicatorType duplicator_type, int adapter_index, int adapter_output_index)
	: inited(false),
	  type(duplicator_type),
	  adapter(adapter_index),
	  adapter_output(adapter_output_index),
	  dxgi_factory(),
	  d3d11_device(),
	  d3d11_device_context(),
	  output_duplicator(),
	  monitor_texture(),
	  shared_handle()
{
}

PLSDuplicatorCore::~PLSDuplicatorCore()
{
	uninit();
}

bool PLSDuplicatorCore::init()
{
	CAutoLockCS AutoLock(duplicator_lock);

	uninit();

	ComPtr<IDXGIAdapter1> adp;
	if (!init_factory(adapter, adp.Assign()) || !init_device(adp)) {
		assert(false);
		return false;
	}

	ComPtr<IDXGIOutput> output;
	if (!init_output(adapter_output, output.Assign()))
		return false;

	ComPtr<IDXGIOutput1> output1;
	HRESULT hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void **)output1.Assign());
	if (FAILED(hr)) {
		PLS_PLUGIN_ERROR("Failed to get IDXGIOutput1, error:0X%X", hr);
		assert(false);
		return false;
	}

	hr = output1->DuplicateOutput(d3d11_device, output_duplicator.Assign());
	if (FAILED(hr)) {
		return false;
	}

	PLS_PLUGIN_INFO("Duplicator is inited. ad:%d output:%d", adapter, adapter_output);
	inited = true;
	return true;
}

void PLSDuplicatorCore::uninit()
{
	CAutoLockCS AutoLock(duplicator_lock);

	inited = false;
	shared_handle = 0;
	monitor_texture.Release();
	output_duplicator.Release();
	d3d11_device_context.Release();
	d3d11_device.Release();
	dxgi_factory.Release();
	ZeroMemory(&texture_desc, sizeof(texture_desc));
}

bool PLSDuplicatorCore::check_update_texture()
{
	CAutoLockCS AutoLock(duplicator_lock);

	if (!inited || !update_texture()) {
		uninit();
		if (!init())
			return false;
		else
			PLS_PLUGIN_INFO("Success to reset duplicator");
	}

	return !!monitor_texture.Get();
}

bool PLSDuplicatorCore::is_size_changed(gs_texture *gs_frame)
{
	if (!gs_frame) {
		assert(false);
		return true;
	}

	uint32_t cx = gs_frame ? gs_texture_get_width(gs_frame) : 0;
	uint32_t cy = gs_frame ? gs_texture_get_height(gs_frame) : 0;

	if (cx != texture_desc.Width || cy != texture_desc.Height) {
		return true;
	} else {
		return false;
	}
}

bool PLSDuplicatorCore::upload_memory_texture(gs_texture *&gs_frame)
{
	CAutoLockCS AutoLock(duplicator_lock);

	assert(type == DUPLICATOR_MEMORY_COPY);
	if (!inited || !monitor_texture.Get() || type != DUPLICATOR_MEMORY_COPY)
		return false;

	if (!gs_frame || is_size_changed(gs_frame)) {
		if (gs_frame) {
			gs_texture_destroy(gs_frame);
			gs_frame = NULL;
		}

		gs_frame = gs_texture_create(texture_desc.Width, texture_desc.Height, GS_BGRA, 1, NULL, GS_DYNAMIC);
		if (!gs_frame)
			return false;
	}

	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = d3d11_device_context->Map(monitor_texture, 0, D3D11_MAP_READ, 0, &map);
	if (FAILED(hr)) {
		return false;
	}

	gs_texture_set_image(gs_frame, (uint8_t *)map.pData, map.RowPitch, false);
	d3d11_device_context->Unmap(monitor_texture, 0);

	return true;
}

bool PLSDuplicatorCore::upload_shared_texture(gs_texture *&gs_frame, HANDLE &handle_output)
{
	CAutoLockCS AutoLock(duplicator_lock);

	assert(type == DUPLICATOR_SHARED_HANDLE);
	if (!inited || !monitor_texture.Get() || type != DUPLICATOR_SHARED_HANDLE)
		return false;

	if (!gs_frame || is_size_changed(gs_frame) || handle_output != shared_handle) {
		if (gs_frame) {
			gs_texture_destroy(gs_frame);
			gs_frame = NULL;
			handle_output = 0;
		}

		gs_frame = gs_texture_open_shared((uint32_t)shared_handle);
		if (!gs_frame)
			return false;

		handle_output = shared_handle;
	}

	return true;
}

bool PLSDuplicatorCore::check_device_error(HRESULT hr)
{
	HRESULT translatedHr;

	HRESULT deviceRemovedReason = d3d11_device->GetDeviceRemovedReason();
	switch (deviceRemovedReason) {
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DEVICE_RESET:
	case static_cast<HRESULT>(E_OUTOFMEMORY): {
		// Our device has been stopped due to an external event on the GPU so map them all to
		// device removed and continue processing the condition
		translatedHr = DXGI_ERROR_DEVICE_REMOVED;
		break;
	}

	case S_OK: {
		// Device is not removed so use original error
		translatedHr = hr;
		break;
	}

	default: {
		// Device is removed but not a error we want to remap
		translatedHr = deviceRemovedReason;
		break;
	}
	}

	switch (translatedHr) {
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_ACCESS_LOST:
	case DXGI_ERROR_INVALID_CALL: {
		inited = false; // should reset capture
		return false;
	}

	default: {
		return true;
	}
	}
}

bool PLSDuplicatorCore::update_texture()
{
	DXGI_OUTDUPL_FRAME_INFO info;
	ComPtr<IDXGIResource> res;

	HRESULT hr = output_duplicator->AcquireNextFrame(0, &info, res.Assign());
	if (hr == DXGI_ERROR_ACCESS_LOST) {
		inited = false; // should reset capture
		return false;
	} else if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
		return true;
	} else if (FAILED(hr)) {
		return check_device_error(hr);
	}

	bool bRet = copy_texture(res, info);
	output_duplicator->ReleaseFrame(); // must be invoked if we successed to call AcquireNextFrame()
	return bRet;
}

bool PLSDuplicatorCore::copy_texture(ComPtr<IDXGIResource> res, DXGI_OUTDUPL_FRAME_INFO &info)
{
	if (0 == info.TotalMetadataBufferSize)
		return true;

	ComPtr<ID3D11Texture2D> pNewText;
	HRESULT hr = res->QueryInterface(__uuidof(ID3D11Texture2D), (void **)pNewText.Assign());
	if (FAILED(hr))
		return check_device_error(hr);

	D3D11_TEXTURE2D_DESC descTemp = {};
	pNewText->GetDesc(&descTemp);
	if (!monitor_texture.Get() || texture_desc.Width != descTemp.Width || texture_desc.Height != descTemp.Height || texture_desc.Format != descTemp.Format) {
		switch (type) {
		case DUPLICATOR_MEMORY_COPY:
			if (!create_read_texture(descTemp))
				return false;
			break;
		case DUPLICATOR_SHARED_HANDLE:
		default:
			if (!create_shared_texture(descTemp))
				return false;
			break;
		}
	}

	d3d11_device_context->CopyResource(monitor_texture, pNewText);
	return true;
}

bool PLSDuplicatorCore::init_factory(unsigned adapterIdx, IDXGIAdapter1 **ppAdapter)
{
	IID factoryIID = (PLSWindowVersion::get_win_version() >= 0x602) ? dxgiFactory2 : __uuidof(IDXGIFactory1);

	HRESULT hr = CreateDXGIFactory1(factoryIID, (void **)dxgi_factory.Assign());
	if (FAILED(hr))
		return false;

	hr = dxgi_factory->EnumAdapters1(adapterIdx, ppAdapter);
	if (FAILED(hr))
		return false;

	return true;
}

bool PLSDuplicatorCore::init_device(IDXGIAdapter *pAdapter)
{
	D3D_FEATURE_LEVEL levelUsed = D3D_FEATURE_LEVEL_9_3;

	HRESULT hr = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION,
				       d3d11_device.Assign(), &levelUsed, d3d11_device_context.Assign());

	if (FAILED(hr))
		return false;

	return true;
}

bool PLSDuplicatorCore::init_output(int monitor_idx, IDXGIOutput **dxgiOutput)
{
	ComPtr<IDXGIAdapter> dxgiAdapter;
	ComPtr<IDXGIDevice> dxgiDevice;
	HRESULT hr;

	assert(d3d11_device);
	hr = d3d11_device->QueryInterface(__uuidof(IDXGIDevice), (void **)dxgiDevice.Assign());
	if (FAILED(hr))
		return false;

	hr = dxgiDevice->GetAdapter(dxgiAdapter.Assign());
	if (FAILED(hr))
		return false;

	hr = dxgiAdapter->EnumOutputs(monitor_idx, dxgiOutput);
	if (FAILED(hr))
		return false;

	return true;
}

bool PLSDuplicatorCore::create_read_texture(D3D11_TEXTURE2D_DESC &td)
{
	td.BindFlags = 0;
	td.MiscFlags = 0;
	td.Usage = D3D11_USAGE_STAGING;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = d3d11_device->CreateTexture2D(&td, NULL, texture.Assign());
	if (FAILED(hr)) {
		PLS_PLUGIN_ERROR("create dump texture failed, error:0X%X", hr);
		return false;
	}

	monitor_texture = texture;
	monitor_texture->GetDesc(&texture_desc);

	return true;
}

bool PLSDuplicatorCore::create_shared_texture(D3D11_TEXTURE2D_DESC &descTemp)
{
	DXGI_FORMAT fmt;
	switch (descTemp.Format) {
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		fmt = DXGI_FORMAT_B8G8R8A8_UNORM;
		break;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		fmt = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	default:
		fmt = descTemp.Format;
	}

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = descTemp.Width;
	desc.Height = descTemp.Height;
	desc.Format = fmt;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

	ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = d3d11_device->CreateTexture2D(&desc, NULL, texture.Assign());
	if (!texture) {
		PLS_PLUGIN_ERROR("create shared texture failed, error:0X%X", hr);
		assert(false);
		return false;
	}

	ComPtr<IDXGIResource> dxgi_res;
	hr = texture->QueryInterface(__uuidof(IDXGIResource), (void **)dxgi_res.Assign());
	if (!dxgi_res) {
		assert(false);
		return false;
	}

	hr = dxgi_res->GetSharedHandle(&shared_handle);
	if (!shared_handle) {
		assert(false);
		return false;
	}

	monitor_texture = texture;
	monitor_texture->GetDesc(&texture_desc);

	return true;
}
