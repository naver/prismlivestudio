#pragma once
#include <windows.h>

typedef DWORD(WINAPI *get_file_version_info_size_wt)(LPCWSTR module, LPDWORD unused);
typedef BOOL(WINAPI *get_file_version_info_wt)(LPCWSTR module, DWORD unused, DWORD len, LPVOID data);
typedef BOOL(WINAPI *ver_query_value_wt)(LPVOID data, LPCWSTR subblock, LPVOID *buf, PUINT sizeout);

class PLSWindowVersion {
public:
	PLSWindowVersion();
	PLSWindowVersion(PLSWindowVersion &obj);
	PLSWindowVersion &operator=(PLSWindowVersion &obj);
	virtual ~PLSWindowVersion();

	static bool is_support_monitor_duplicate();
	static VS_FIXEDFILEINFO get_dll_versiton(const wchar_t *dll_name);
	static unsigned get_win_version();

private:
	bool is_support_monitor_duplicate_inner();
	static bool init_function(get_file_version_info_size_wt &pFunc1, get_file_version_info_wt &pFunc2, ver_query_value_wt &pFunc3);

private:
	HMODULE free_library;
	bool is_inited;
	bool is_support;
};
