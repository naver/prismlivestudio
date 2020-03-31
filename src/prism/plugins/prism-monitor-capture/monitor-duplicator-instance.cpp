#include "monitor-duplicator-instance.h"
#include <assert.h>
#include <process.h>
#include <log.h>
#include <obs.h>

#define MONITOR_ACTIVE_INTERVAL 2000
#define MONITOR_EXCEPTION_SLEEP 2000

PLSDuplicatorInstance::PLSDuplicatorInstance(DuplicatorType type, int adapter, int output_id, int did)
	: gs_frame(NULL), shared_handle(0), monitor_capture(type, adapter, output_id), previous_active_time(GetTickCount()), display_id(did)
{
	exit_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	active_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	::ResetEvent(exit_event);
	capture_thread = (HANDLE)_beginthreadex(0, 0, thread_func, this, 0, 0);
}

PLSDuplicatorInstance::~PLSDuplicatorInstance()
{
	release_frame();

	::SetEvent(exit_event);
	::WaitForSingleObject(capture_thread, INFINITE);

	::CloseHandle(capture_thread);
	::CloseHandle(exit_event);
	::CloseHandle(active_event);
}

bool is_handle_signed(const HANDLE &hEvent, DWORD dwMilliSecond)
{
	if (!hEvent)
		return false;

	DWORD res = WaitForSingleObject(hEvent, dwMilliSecond);
	return (res == WAIT_OBJECT_0);
}

unsigned __stdcall PLSDuplicatorInstance::thread_func(void *pParam)
{
	PLSDuplicatorInstance *self = reinterpret_cast<PLSDuplicatorInstance *>(pParam);
	CoInitialize(NULL);
	self->thread_func_inner();
	CoUninitialize();
	return 0;
}

void PLSDuplicatorInstance::thread_func_inner()
{
	if (!monitor_capture.init()) {
		PLS_PLUGIN_ERROR("Failed to init prism duplicator. adp:%d output:%d", monitor_capture.adapter, monitor_capture.adapter_output);
	}

	HANDLE events[] = {
		active_event,
		exit_event,
	};

	DWORD events_count = sizeof(events) / sizeof(events[0]);
	DWORD render_fps = 60; // default fps for monitor

	while (!is_handle_signed(exit_event, 1000 / render_fps)) {
		if (!need_update_frame()) {
			WaitForMultipleObjects(events_count, events, FALSE, MONITOR_EXCEPTION_SLEEP);
		}

		if (!monitor_capture.check_update_texture()) {
			WaitForSingleObject(exit_event, MONITOR_EXCEPTION_SLEEP);
			continue;
		}

		obs_video_info ovi;
		if (obs_get_video_info(&ovi)) {
			if (ovi.fps_num > 0 && ovi.fps_den > 0) {
				uint32_t fps = ovi.fps_num / ovi.fps_den;
				if (fps > 0 && render_fps != fps) {
					render_fps = fps;
				}
			}
		}
	}

	monitor_capture.uninit();
}

int PLSDuplicatorInstance::get_hardware_id()
{
	return display_id;
}

int PLSDuplicatorInstance::get_adapter_id()
{
	return monitor_capture.adapter;
}

int PLSDuplicatorInstance::get_monitor_dev_id()
{
	return monitor_capture.adapter_output;
}

bool PLSDuplicatorInstance::is_adapter_valid()
{
	obs_video_info ovi;
	if (!obs_get_video_info(&ovi))
		return true;

	DuplicatorType type;
	if (ovi.adapter == get_adapter_id())
		type = DUPLICATOR_SHARED_HANDLE;
	else
		type = DUPLICATOR_MEMORY_COPY;

	return (type == monitor_capture.type);
}

bool PLSDuplicatorInstance::update_frame()
{
	previous_active_time = GetTickCount();
	::SetEvent(active_event); // enable event after set active time

	PLSAutoLockRender alr;

	bool is_ok;
	switch (monitor_capture.type) {
	case DUPLICATOR_MEMORY_COPY:
		is_ok = monitor_capture.upload_memory_texture(gs_frame);
		break;

	case DUPLICATOR_SHARED_HANDLE:
	default:
		is_ok = monitor_capture.upload_shared_texture(gs_frame, shared_handle);
		break;
	}

	if (!is_ok)
		release_frame();

	return is_ok;
}

gs_texture *PLSDuplicatorInstance::get_texture()
{
	previous_active_time = GetTickCount();
	::SetEvent(active_event); // enable event after set active time

	PLSAutoLockRender alr;
	return gs_frame;
}

bool PLSDuplicatorInstance::need_update_frame()
{
	DWORD current = GetTickCount();
	DWORD previous = previous_active_time;
	if (current > previous && (current - previous) > MONITOR_ACTIVE_INTERVAL)
		return false;
	else
		return true;
}

void PLSDuplicatorInstance::release_frame()
{
	PLSAutoLockRender alr;

	shared_handle = 0;
	if (gs_frame) {
		gs_texture_destroy(gs_frame);
		gs_frame = NULL;
	}
}
