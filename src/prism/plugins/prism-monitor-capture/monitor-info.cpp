#include "monitor-info.h"
#include "window-version.h"
#include <util/windows/ComPtr.hpp>
#include <assert.h>
#include <obs-module.h>
#include <log.h>

static const IID dxgi_factory2 = {0x50c83a1c, 0xe072, 0x4c48, {0x87, 0xb0, 0x36, 0x30, 0xfa, 0x36, 0xa6, 0xd0}};

PLSMonitorManager::PLSMonitorManager()
{
	reload_monitor(true);
}

PLSMonitorManager::~PLSMonitorManager() {}

PLSMonitorManager *PLSMonitorManager::get_instance()
{
	static PLSMonitorManager monitor_manager;
	return &monitor_manager;
}

std::string wchar_to_string(const wchar_t *str)
{
	if (!str)
		return "";
	int n = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	char *pBuffer = new (std::nothrow) char[n + 1];
	n = WideCharToMultiByte(CP_UTF8, 0, str, -1, pBuffer, n, NULL, NULL);
	pBuffer[n] = 0;
	std::string ret = pBuffer;
	delete[] pBuffer;
	pBuffer = NULL;
	return ret;
}

int get_rotation_degree(DISPLAYCONFIG_ROTATION config_rotation)
{
	switch (config_rotation) {
	case DISPLAYCONFIG_ROTATION_IDENTITY:
		return 0;
	case DISPLAYCONFIG_ROTATION_ROTATE90:
		return 90;
	case DISPLAYCONFIG_ROTATION_ROTATE180:
		return 180;
	case DISPLAYCONFIG_ROTATION_ROTATE270:
		return 270;
	default:
		return 0;
	}
	return 0;
}

void enum_monitor_detail(const WCHAR *destDevice, monitor_info *info)
{
	UINT32 requiredPaths, requiredModes;
	if (ERROR_SUCCESS != GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &requiredPaths, &requiredModes))
		return;

	std::vector<DISPLAYCONFIG_PATH_INFO> paths(requiredPaths);
	std::vector<DISPLAYCONFIG_MODE_INFO> modes(requiredModes);
	if (ERROR_SUCCESS != QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &requiredPaths, paths.data(), &requiredModes, modes.data(), nullptr))
		return;

	for (auto &pathTemp : paths) {
		DISPLAYCONFIG_SOURCE_DEVICE_NAME sourceName;
		sourceName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
		sourceName.header.size = sizeof(sourceName);
		sourceName.header.adapterId = pathTemp.sourceInfo.adapterId;
		sourceName.header.id = pathTemp.sourceInfo.id;
		if (ERROR_SUCCESS == DisplayConfigGetDeviceInfo(&sourceName.header)) {
			if (0 == wcscmp(destDevice, sourceName.viewGdiDeviceName)) {
				DISPLAYCONFIG_TARGET_DEVICE_NAME name;
				name.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
				name.header.size = sizeof(name);
				name.header.adapterId = pathTemp.sourceInfo.adapterId;
				name.header.id = pathTemp.targetInfo.id;
				if (ERROR_SUCCESS == DisplayConfigGetDeviceInfo(&name.header)) {
					info->friendly_name = wchar_to_string(name.monitorFriendlyDeviceName);
					info->display_id = pathTemp.targetInfo.id;
					info->rotation = get_rotation_degree(pathTemp.targetInfo.rotation);
				}

				return;
			}
		}
	}
}

BOOL CALLBACK enum_monitor_proc(HMONITOR handle, HDC hdc, LPRECT rect, LPARAM lParam)
{
	vector<monitor_info> *info_array = reinterpret_cast<vector<monitor_info> *>(lParam);

	MONITORINFOEX mi = {};
	mi.cbSize = sizeof(mi);
	if (!GetMonitorInfo(handle, &mi)) {
		return TRUE;
	}

	monitor_info info;
	info.offset_x = mi.rcMonitor.left;
	info.offset_y = mi.rcMonitor.top;
	info.width = (mi.rcMonitor.right - mi.rcMonitor.left);
	info.height = (mi.rcMonitor.bottom - mi.rcMonitor.top);

	enum_monitor_detail(mi.szDevice, &info);

	if (mi.dwFlags == MONITORINFOF_PRIMARY) {
		info.is_primary = true;
		info_array->insert(info_array->begin(), info);
	} else {
		info.is_primary = false;
		info_array->push_back(info);
	}

	return TRUE;
}

void fill_monitor_id(vector<monitor_info> &list_info_out, monitor_info match_info)
{
	for (unsigned i = 0; i < static_cast<int>(list_info_out.size()); ++i) {
		if (list_info_out.at(i).width == match_info.width && list_info_out.at(i).height == match_info.height && list_info_out.at(i).offset_x == match_info.offset_x &&
		    list_info_out.at(i).offset_y == match_info.offset_y) {
			list_info_out.at(i).adapter_index = match_info.adapter_index;
			list_info_out.at(i).adapter_output_index = match_info.adapter_output_index;
		}
	}
}

void PLSMonitorManager::reload_monitor(bool save_log)
{
	CAutoLockCS alock(list_lock);

	monitor_info_array.clear();
	EnumDisplayMonitors(NULL, NULL, enum_monitor_proc, (LPARAM)&monitor_info_array);

	vector<monitor_info> d3dList;
	if (enum_duplicator_array(d3dList)) {
		for (int i = 0; i < static_cast<int>(d3dList.size()); ++i) {
			fill_monitor_id(monitor_info_array, d3dList.at(i));
		}
	}

	if (save_log) {
		PLS_PLUGIN_INFO("------------------- monitor list : %d --------------------", monitor_info_array.size());
		for (auto item : monitor_info_array) {
			PLS_PLUGIN_INFO("[%s] %dx%d is_primary:%d display_id:%d adapter:%d output:%d", item.friendly_name.c_str(), item.width, item.height, item.is_primary, item.display_id,
					item.adapter_index, item.adapter_output_index);
		}
	}
}

vector<monitor_info> PLSMonitorManager::get_monitor()
{
	CAutoLockCS alock(list_lock);
	return monitor_info_array;
}

bool PLSMonitorManager::enum_duplicator_array(vector<monitor_info> &outputList)
{
	ComPtr<IDXGIFactory1> m_pFactory;
	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIOutput> output;

	IID factoryIID = (PLSWindowVersion::get_win_version() >= 0x602) ? dxgi_factory2 : __uuidof(IDXGIFactory1);
	HRESULT hr = CreateDXGIFactory1(factoryIID, (void **)m_pFactory.Assign());
	if (FAILED(hr)) {
		PLS_PLUGIN_ERROR("create dxgi factory failed, error code:%ld", hr);
		return false;
	}

	int adapter_index = 0;
	while (m_pFactory->EnumAdapters1(adapter_index, adapter.Assign()) == S_OK) {
		int output_index = 0;
		while (adapter->EnumOutputs(output_index, &output) == S_OK) {
			DXGI_OUTPUT_DESC desc;
			if (SUCCEEDED(output->GetDesc(&desc))) {
				const RECT &rect = desc.DesktopCoordinates;
				monitor_info info;
				info.adapter_index = adapter_index;
				info.adapter_output_index = output_index;
				info.offset_x = rect.left;
				info.offset_y = rect.top;
				info.width = rect.right - rect.left;
				info.height = rect.bottom - rect.top;
				outputList.push_back(info);
			}
			++output_index;
		}
		++adapter_index;
	}

	return outputList.size() > 0;
}

bool PLSMonitorManager::find_monitor(int index, int display_id, monitor_info &info)
{
	CAutoLockCS alock(list_lock);

	info = monitor_info();

	// we should check display_id firstly
	if (display_id > 0) {
		for (auto temp : monitor_info_array) {
			if (temp.display_id == display_id) {
				info = temp;
				return true;
			}
		}
	}

	if (index < 0 || index >= (int)monitor_info_array.size())
		return false;

	info = monitor_info_array.at(index);
	return true;
}
