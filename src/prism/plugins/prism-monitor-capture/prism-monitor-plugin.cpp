#include <obs-module.h>
#include <Windows.h>
#include <obs-module.h>
#include <log.h>
#include "monitor-info.h"
#include "monitor-duplicator-pool.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("prism-monitor-capture", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Prism monitor capture";
}

static HHOOK size_change_hook = NULL;
static bool pls_monitor_size_changed = false;

extern void register_prism_monitor_source();
extern void register_prism_region_source();
LRESULT CALLBACK pls_sizechange_hook_pro(int nCode, WPARAM wParam, LPARAM lParam);

bool obs_module_load(void)
{
	size_change_hook = ::SetWindowsHookEx(WH_CALLWNDPROC, pls_sizechange_hook_pro, NULL, ::GetCurrentThreadId());
	if (!size_change_hook) {
		PLS_PLUGIN_ERROR("Failed to hook display size change. error:%u", GetLastError());
	}

	register_prism_monitor_source();
	register_prism_region_source();

	return true;
}

void obs_module_unload(void)
{
	if (size_change_hook) {
		UnhookWindowsHookEx(size_change_hook);
		size_change_hook = NULL;
	}

	PLSMonitorDuplicatorPool::get_instance()->clear();
}

const char *obs_module_name(void)
{
	return obs_module_description();
}

LRESULT CALLBACK pls_sizechange_hook_pro(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (!pls_monitor_size_changed) {
		CWPSTRUCT *cwp = reinterpret_cast<CWPSTRUCT *>(lParam);
		if (cwp->message == WM_DISPLAYCHANGE) {
			pls_monitor_size_changed = true;
		}
	}

	return CallNextHookEx(size_change_hook, nCode, wParam, lParam);
}

void handle_hooked_message()
{
	// When state of display is changed, all top windows will receive message.
	// So our hooked callback will be invoked many times. We have to handle it with interval time
#define CHECK_MONITOR_MSG_INTERVAL 2000 // millisecond

	if (pls_monitor_size_changed) {
		static unsigned pls_previous_check_time = 0;
		if ((GetTickCount() - pls_previous_check_time) >= CHECK_MONITOR_MSG_INTERVAL) {
			pls_monitor_size_changed = false;
			pls_previous_check_time = GetTickCount();
			PLSMonitorManager::get_instance()->reload_monitor(true);
		}
	}
}
