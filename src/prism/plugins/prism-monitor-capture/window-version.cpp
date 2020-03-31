#include "window-version.h"

unsigned PLSWindowVersion::get_win_version()
{
	VS_FIXEDFILEINFO info = PLSWindowVersion::get_dll_versiton(L"kernel32");

	int major = (int)HIWORD(info.dwFileVersionMS);
	int minor = (int)LOWORD(info.dwFileVersionMS);

	return (major << 8) | minor;
}

PLSWindowVersion::PLSWindowVersion()
{
	is_inited = false;
	is_support = false;
	free_library = LoadLibraryW(L"version");
}

PLSWindowVersion::PLSWindowVersion(PLSWindowVersion &obj)
{
	is_inited = obj.is_inited;
	is_support = obj.is_support;
	free_library = NULL;
}

PLSWindowVersion &PLSWindowVersion::operator=(PLSWindowVersion &obj)
{
	is_inited = obj.is_inited;
	is_support = obj.is_support;
	free_library = NULL;
	return *this;
}

PLSWindowVersion::~PLSWindowVersion()
{
	if (free_library)
		FreeLibrary(free_library);
}

bool PLSWindowVersion::init_function(get_file_version_info_size_wt &pFunc1, get_file_version_info_wt &pFunc2, ver_query_value_wt &pFunc3)
{
	HMODULE ver = GetModuleHandleW(L"version");
	if (!ver)
		return false;

	pFunc1 = (get_file_version_info_size_wt)GetProcAddress(ver, "GetFileVersionInfoSizeW");
	pFunc2 = (get_file_version_info_wt)GetProcAddress(ver, "GetFileVersionInfoW");
	pFunc3 = (ver_query_value_wt)GetProcAddress(ver, "VerQueryValueW");

	if (!pFunc1 || !pFunc2 || !pFunc3)
		return false;

	return true;
}

VS_FIXEDFILEINFO PLSWindowVersion::get_dll_versiton(const wchar_t *pDllName)
{
	LPVOID data = 0;
	VS_FIXEDFILEINFO ret = {};

	do {
		get_file_version_info_size_wt pGetFileVersionInfoSize;
		get_file_version_info_wt pGetFileVersionInfo;
		ver_query_value_wt pVerQueryValue;

		if (!init_function(pGetFileVersionInfoSize, pGetFileVersionInfo, pVerQueryValue))
			break;

		DWORD size = pGetFileVersionInfoSize(pDllName, NULL);
		if (!size)
			break;

		data = malloc(size);
		if (!pGetFileVersionInfo(L"kernel32", 0, size, data))
			break;

		UINT len = 0;
		VS_FIXEDFILEINFO *info = NULL;
		pVerQueryValue(data, L"\\", (LPVOID *)&info, &len);

		ret = *info;
	} while (0);

	if (data)
		free(data);

	return ret;
}

bool PLSWindowVersion::is_support_monitor_duplicate()
{
	static PLSWindowVersion s_Ins;
	return s_Ins.is_support_monitor_duplicate_inner();
}

bool PLSWindowVersion::is_support_monitor_duplicate_inner()
{
	if (is_inited)
		return is_support;

	is_inited = true;
	is_support = false;

	VS_FIXEDFILEINFO info = get_dll_versiton(L"kernel32");
	int major = (int)HIWORD(info.dwFileVersionMS);
	int minor = (int)LOWORD(info.dwFileVersionMS);

	if (major > 6 || (major == 6 && minor >= 2)) {
		is_support = true;
	}

	return is_support;
}
