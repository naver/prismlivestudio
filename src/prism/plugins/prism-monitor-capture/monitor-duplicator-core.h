#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DXGI1_2.h>
#include <windows.h>
#include "win_lock.hpp"
#include <util/windows/ComPtr.hpp>
#include "graphics/graphics.h"
#include <obs.h>

enum DuplicatorType {
	DUPLICATOR_SHARED_HANDLE = 0,
	DUPLICATOR_MEMORY_COPY,
};

class PLSAutoLockRender {
public:
	PLSAutoLockRender() { obs_enter_graphics(); }
	virtual ~PLSAutoLockRender() { obs_leave_graphics(); }
};

struct PLSDuplicatorCore {
public:
	explicit PLSDuplicatorCore(DuplicatorType duplicator_type, int adapter_index, int adapter_output_index);
	virtual ~PLSDuplicatorCore();

	bool init();
	void uninit();
	bool check_update_texture();

	bool upload_memory_texture(gs_texture *&gs_frame);
	bool upload_shared_texture(gs_texture *&gs_frame, HANDLE &handle_output);

private:
	bool init_factory(unsigned adapterIdx, IDXGIAdapter1 **ppAdapter);
	bool init_device(IDXGIAdapter *pAdapter);
	bool init_output(int monitor_idx, IDXGIOutput **dxgiOutput);

	bool update_texture();
	bool check_device_error(HRESULT hr);
	bool copy_texture(ComPtr<IDXGIResource> res, DXGI_OUTDUPL_FRAME_INFO &info);
	bool create_read_texture(D3D11_TEXTURE2D_DESC &td);
	bool create_shared_texture(D3D11_TEXTURE2D_DESC &descTemp);

	bool is_size_changed(gs_texture *gs_frame);

public:
	DuplicatorType type;
	int adapter;
	int adapter_output;

protected:
	bool inited;

	CCSection duplicator_lock;
	ComPtr<IDXGIFactory1> dxgi_factory;
	ComPtr<ID3D11Device> d3d11_device;
	ComPtr<ID3D11DeviceContext> d3d11_device_context;
	ComPtr<IDXGIOutputDuplication> output_duplicator;
	ComPtr<ID3D11Texture2D> monitor_texture;
	D3D11_TEXTURE2D_DESC texture_desc;
	HANDLE shared_handle;
};
