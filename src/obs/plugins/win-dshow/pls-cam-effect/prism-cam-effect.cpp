#include "prism-cam-effect.h"
#include <util/platform.h>
#include <util/util.hpp>

#ifdef DEBUG
//#define DEBUG_EFFECT_FPS
#endif

std::string PLSCamEffect::nelo_session = "";
std::string PLSCamEffect::prism_version = "";
std::string PLSCamEffect::prism_cfg_path = "";

//----------------------------------------------------------------------
PLSFpsChecker::PLSFpsChecker(char *flag)
	: flag_name(flag ? flag : ""), start_time(0), frame_count(0)
{
}

void PLSFpsChecker::Reset()
{
#ifdef DEBUG_EFFECT_FPS
	frame_count = 0;
	start_time = 0;
#endif
}

void PLSFpsChecker::OnFrame()
{
#ifdef DEBUG_EFFECT_FPS
	if (frame_count <= 0 || start_time <= 0) {
		frame_count = 0;
		start_time = timeGetTime();
	}

	++frame_count;

	DWORD time_interval = timeGetTime() - start_time;
	if (time_interval < 3000) {
		return; // calculate fps every 3 seconds
	}

	double fps = (double(frame_count) * 1000.0) / double(time_interval);
	blog(LOG_INFO, "FPF of %s : %d", flag_name.c_str(), (int)fps);

	Reset();
#endif
}

//----------------------------------------------------------------------
void PLSCamEffect::SaveTexture(gs_texture_t *tex, char *path)
{
	obs_enter_graphics();

	uint32_t cx = gs_texture_get_width(tex);
	uint32_t cy = gs_texture_get_height(tex);
	enum gs_color_format fmt = gs_texture_get_color_format(tex);

	if (fmt == GS_RGBA || fmt == GS_BGRX || fmt == GS_BGRA) {
		gs_stagesurf_t *surface = gs_stagesurface_create(cx, cy, fmt);
		if (surface) {
			gs_stage_texture(surface, tex);

			uint8_t *data = NULL;
			uint32_t linesize = 0;
			if (gs_stagesurface_map(surface, &data, &linesize)) {
				save_as_bitmap_file(path, data, linesize, cx,
						    cy, 4, true);
				gs_stagesurface_unmap(surface);
			}

			gs_stagesurface_destroy(surface);
		}
	}

	obs_leave_graphics();
}

bool PLSCamEffect::SetGlobalParam(obs_data_t *data)
{
	const char *nelo = obs_data_get_string(data, "nelo");
	const char *version = obs_data_get_string(data, "version");
	const char *path = obs_data_get_string(data, "path");

	if (!nelo || !version || !path) {
		blog(LOG_ERROR, "Receive incorrect params for beauty!");
		assert(false);
	}

	if (nelo) {
		nelo_session = nelo;
	}
	if (version) {
		prism_version = version;
	}
	if (path) {
		prism_cfg_path = path;
	}

	return (nelo && version && path);
}

unsigned PLSCamEffect::WindowLoopThread(void *pParam)
{
	PLSCamEffect *self = reinterpret_cast<PLSCamEffect *>(pParam);
	self->WindowLoopThreadInner();
	return 0;
}

unsigned PLSCamEffect::MessageLoopThread(void *pParam)
{
	PLSCamEffect *self = reinterpret_cast<PLSCamEffect *>(pParam);
	self->MessageLoopThreadInner();
	return 0;
}

LRESULT PLSCamEffect::EffectWndProc(HWND hWnd, UINT nMsg, WPARAM wParam,
				    LPARAM lParam)
{
	switch (nMsg) {
	case WM_DESTROY: {
		blog(LOG_INFO, "PLSCamEffect: Window is destroied.");
		PostQuitMessage(0); // exit message loop
		break;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_COPYDATA: {
		PLSCamEffect *self =
			(PLSCamEffect *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (self) {
			self->OnCopyData((COPYDATASTRUCT *)lParam);
		}
		break;
	}
	}

	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

//----------------------------------------------------------------------
PLSCamEffect::PLSCamEffect()
	: process_hwnd(0),
	  process_handle(0),
	  previous_heartbeat(),
	  output_video_handle(0),
	  output_video_tex(NULL),
	  effect_state(""),
	  fps_device("video-device"),
	  fps_push_handle("input-handle"),
	  fps_get_handle("output-handle"),
	  waiting_onoff_change(false),
	  source_hwnd(0)
{
	session_id = GenerateGuid();

	char temp[20];
	sprintf_s(temp, ARRAY_COUNT(temp), "%p", this);
	source_hwnd_class = session_id + std::string(temp);
	window_thread =
		(HANDLE)_beginthreadex(0, 0, WindowLoopThread, this, 0, 0);

	std::string name = session_id + std::string(VIDEO_INPUT_BUFFER);
	video_input_writer = new SharedHandleBuffer(name.c_str());
	DWORD ret = video_input_writer->InitMapBuffer();
	if (ret != 0) {
		blog(LOG_ERROR, "PLSCamEffect fail to init input map! error:%u",
		     ret);
		assert(false);
	}

	name = session_id + std::string(VIDEO_OUTPUT_BUFFER);
	video_output_reader = new SharedHandleBuffer(name.c_str());
	ret = video_output_reader->InitMapBuffer();
	if (ret != 0) {
		blog(LOG_ERROR,
		     "PLSCamEffect fail to init output map! error:%u", ret);
		assert(false);
	}

	name = session_id + std::string(SOURCE_ALIVE_FLAG);
	camera_alive_flag = CHandleWrapper::GetEvent(name.c_str(), true);

	name = session_id + std::string(SOURCE_ACTIVE_FLAG);
	camera_active_flag = CHandleWrapper::GetEvent(name.c_str(), true);
	SetCaptureState(false);

	name = session_id + std::string(PROCESS_VALID_FLAG);
	effect_valid_flag = CHandleWrapper::GetEvent(name.c_str(), true);
	::ResetEvent(effect_valid_flag);

	onoff_changed_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	::ResetEvent(onoff_changed_event);

	thread_exit_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	::ResetEvent(thread_exit_event);
	message_thread =
		(HANDLE)_beginthreadex(0, 0, MessageLoopThread, this, 0, 0);
}

PLSCamEffect::~PLSCamEffect()
{
	::SetEvent(thread_exit_event);
	CHandleWrapper::WaitThreadEnd(message_thread);
	CHandleWrapper::CloseHandleEx(thread_exit_event);
	CHandleWrapper::CloseHandleEx(onoff_changed_event);

	if (::IsWindow(source_hwnd)) {
		SendMessage(source_hwnd, WM_CLOSE, 0, 0);
	}

	CHandleWrapper::WaitThreadEnd(window_thread);

	ClearOutputVideo();

	CHandleWrapper::CloseHandleEx(process_handle);

	SAFE_DELETE_PTR(video_input_writer);
	SAFE_DELETE_PTR(video_output_reader);

	CHandleWrapper::CloseHandleEx(camera_alive_flag);
	CHandleWrapper::CloseHandleEx(camera_active_flag);
	CHandleWrapper::CloseHandleEx(effect_valid_flag);
}

bool PLSCamEffect::SetEffectParam(const char *method, obs_data_t *data,
				  bool &effect_onoff_changed)
{
	effect_onoff_changed = false;

	bool ret = false;
	if (0 == strcmp(method, "beauty")) {
		ret = ParseBeautyParam(data, effect_onoff_changed);
	} else {
		ret = false;
	}

	if (effect_onoff_changed && waiting_onoff_change && IsEffectOn()) {
		::SetEvent(onoff_changed_event);
	}

	return ret;
}

size_t PLSCamEffect::PopExceptions(std::vector<PLSExceptionInfo> &output)
{
	output.clear();

	if (!IsEffectOn()) {
		return 0;
	}

	{
		CAutoLockCS autoLock(exception_lock);

		std::map<obs_source_exception_type, PLSExceptionInfo>::iterator
			itr = exceptions.begin();

		while (itr != exceptions.end()) {
			if (itr->second.exception_happen &&
			    !itr->second.notify_done) {
				output.push_back(itr->second);
				itr->second.notify_done =
					true; // make sure one exception won't be push again
			}
			++itr;
		}
	}

	return output.size();
}

bool PLSCamEffect::IsEffectOn()
{
	CAutoLockCS autoLock(param_lock);
	if (beauty_params.enable) {
		return true;
	} else {
		return false;
	}
}

bool PLSCamEffect::ParseBeautyParam(obs_data_t *data,
				    bool &effect_onoff_changed)
{
	face_beauty_message *temp = new face_beauty_message();
	EFFECT_MESSAGE_PTR msg(temp);

	const char *id = obs_data_get_string(data, "id");
	assert(id);
	if (id) {
		int len = min(ARRAY_COUNT(temp->id), strlen(id) + 1);
		memmove(temp->id, id, len);
	}

	const char *category = obs_data_get_string(data, "category");
	assert(category);
	if (category) {
		int len =
			min(ARRAY_COUNT(temp->category), strlen(category) + 1);
		memmove(temp->category, category, len);
	}

	temp->chin = obs_data_get_double(data, "chin");
	temp->cheek = obs_data_get_double(data, "cheek");
	temp->cheekbone = obs_data_get_double(data, "cheekbone");
	temp->eyes = obs_data_get_double(data, "eyes");
	temp->nose = obs_data_get_double(data, "nose");
	temp->smooth = obs_data_get_double(data, "smooth");
	temp->enable = obs_data_get_bool(data, "enable");

	int len = min(ARRAY_COUNT(temp->guid), session_id.length() + 1);
	memmove(temp->guid, session_id.c_str(), len);

	{
		CAutoLockCS autoLock(param_lock);
		effect_onoff_changed = (temp->enable != beauty_params.enable);
		memmove(&beauty_params, temp, sizeof(face_beauty_message));
	}

	if (effect_onoff_changed) {
		PushEmptyInputVideo();
		ClearOutputVideo();

		if (temp->enable) {
			blog(LOG_INFO, "PLSCamEffect: Face beauty is enable");
		} else {
			blog(LOG_INFO, "PLSCamEffect: Face beauty is disable");
		}
	}

	if (!temp->enable) {
		ClearOutputVideo();
	}

	message_queue.PushMessage(msg, sizeof(face_beauty_message), true);
	return true;
}

void PLSCamEffect::SetCaptureState(bool actived)
{
	if (actived) {
		if (!CHandleWrapper::IsHandleSigned(camera_active_flag)) {
			blog(LOG_INFO, "Capture state is active");
		}

		fps_device.OnFrame();

		// should clear cache when receive first video
		if (!CHandleWrapper::IsHandleSigned(camera_active_flag)) {
			PushEmptyInputVideo();
			ClearOutputVideo();
		}

		::SetEvent(camera_active_flag);

	} else {
		fps_device.Reset();
		fps_push_handle.Reset();
		fps_get_handle.Reset();

		if (CHandleWrapper::IsHandleSigned(camera_active_flag)) {
			blog(LOG_INFO, "Capture state is deactive");
		}

		::ResetEvent(camera_active_flag);

		PushEmptyInputVideo();
		ClearOutputVideo();
	}
}

bool PLSCamEffect::IsCaptureNormal()
{
	return CHandleWrapper::IsHandleSigned(camera_active_flag);
}

// return true if effect process is ready && effect is on
bool PLSCamEffect::UseVideoEffect()
{
	// effect is off
	if (!IsEffectOn()) {
		SetEffectState("beaufy-off");
		return false;
	}

	// circle buffer is not valid
	if (!video_input_writer->IsBufferValid() ||
	    !video_output_reader->IsBufferValid()) {
		SetEffectState("buffer-invalid");
		return false;
	}

	// effect process is not running, not exited
	if (!CHandleWrapper::IsHandleValid(process_handle) ||
	    CHandleWrapper::IsHandleSigned(process_handle)) {
		SetEffectState("process-not-run");
		return false;
	}

	// cann't find process window
	if (!::IsWindow(process_hwnd)) {
		SetEffectState("process-nowindow");
		return false;
	}

	// effect process is not ready
	if (!CHandleWrapper::IsHandleSigned(effect_valid_flag)) {
		SetEffectState("process-not-ready");
		return false;
	}

	// effect process may be blocked or exception happens
	if (GetTickCount() - previous_heartbeat >= HEART_BEAT_TIMEOUT) {
		SetEffectState("process-noheartbeat");
		return false;
	}

	return true;
}

void PLSCamEffect::EffectTick()
{
	if (!CHandleWrapper::IsHandleSigned(camera_active_flag)) {
		SetEffectState("camera-nodata");
		PushEmptyInputVideo();
		ClearOutputVideo();
	}
}

void PLSCamEffect::SetInputVideo(shared_handle_header &hdr,
				 shared_handle_sample &body)
{
	if (!IsEffectOn()) {
		return;
	}

	if (body.handle) {
		fps_push_handle.OnFrame();
	}

	video_input_writer->WriteItemData(&hdr, sizeof(hdr), &body,
					  sizeof(body));
}

gs_texture *PLSCamEffect::GetOutputVideo()
{
	if (!IsEffectOn()) {
		return NULL;
	}

	if (!CHandleWrapper::IsHandleSigned(camera_active_flag)) {
		return NULL; // currently no video, we should not render any frame
	}

	shared_handle_header header;
	shared_handle_sample sample;
	sample.handle = 0;

	bool bOk = video_output_reader->ReadItemData(&header, sizeof(header),
						     &sample, sizeof(sample));
	if (bOk) {
		if (header.is_handle_available()) {
			if (output_video_handle != sample.handle) {
				blog(LOG_INFO,
				     "Received new video handle. video:%llu",
				     sample.handle);
			}

			output_video_handle = sample.handle;
			if (output_video_handle) {
				fps_get_handle.OnFrame();
			}
		} else {
			blog(LOG_INFO,
			     "Received obsoleted video handle(%llu). It is updated before %ums.",
			     sample.handle, GetTickCount() - header.push_time);
		}
	}

	if (!output_video_handle) {
		ClearOutputVideo();
		return NULL;
	}

	if (output_video_tex) {
		obs_enter_graphics();
		ULONG64 current_handle =
			(ULONG64)gs_texture_get_shared_handle(output_video_tex);
		obs_leave_graphics();

		// Output video never change, use the openned texture directly.
		if (current_handle == output_video_handle) {
			SetEffectState("sharedhandle-ok");
			return output_video_tex;
		}

		// Output video is changed, release previou texture.
		ReleaseOpennedTexture();
	}

	obs_enter_graphics();
	output_video_tex =
		gs_texture_open_shared((uint32_t)output_video_handle);
	obs_leave_graphics();

	if (!output_video_tex) {
		SetEffectState("sharedhandle-invalid");
		blog(LOG_ERROR, "Fail to open shared handle! HANDLE:%llu",
		     output_video_handle);
		//assert(output_video_tex && "fail to open output handle");
	}

	return output_video_tex;
}

std::string PLSCamEffect::GenerateGuid()
{
	GUID guid;
	CoCreateGuid(&guid);

	char buf[64] = {0};
	sprintf_s(buf, ARRAY_COUNT(buf),
		  "{%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X}", guid.Data1,
		  guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1],
		  guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
		  guid.Data4[6], guid.Data4[7]);

	return std::string(buf);
}

std::wstring PLSCamEffect::GetProcessPath()
{
	WCHAR szFilePath[MAX_PATH] = {};
	GetModuleFileNameW(NULL, szFilePath, MAX_PATH);

	int nLen = (int)wcslen(szFilePath);
	for (int i = nLen - 1; i >= 0; --i) {
		if (szFilePath[i] == '\\') {
			szFilePath[i + 1] = 0;
			break;
		}
	}

	return std::wstring(szFilePath) +
	       std::wstring(CAMERA_EFFECT_PROCESS_NAME);
}

HWND PLSCamEffect::CreateMessageWindow()
{
	WNDCLASSA wc = {0};
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = EffectWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = source_hwnd_class.c_str();

	ATOM nAtom = RegisterClassA(&wc);
	if (0 == nAtom) {
		blog(LOG_ERROR,
		     "PLSCamEffect failed to register window! error:%u",
		     GetLastError());

		assert(false);
		return 0;
	}

	HWND hWnd = CreateWindowA(source_hwnd_class.c_str(),
				  source_hwnd_class.c_str(), WS_POPUPWINDOW, 0,
				  0, 0, 0, NULL, NULL, GetModuleHandle(NULL),
				  NULL);
	if (!::IsWindow(hWnd)) {
		UnregisterClassA(source_hwnd_class.c_str(),
				 GetModuleHandle(NULL));

		blog(LOG_ERROR,
		     "PLSCamEffect failed to create window! error:%u",
		     GetLastError());

		assert(false);
		return 0;
	}

	::SetWindowLongPtr(hWnd, GWLP_USERDATA,
			   reinterpret_cast<LONG_PTR>(this));
	return hWnd;
}

void PLSCamEffect::ClearOutputVideo()
{
	if (output_video_handle) {
		obs_enter_graphics();
		ReleaseOpennedTexture();
		output_video_handle = 0;
		obs_leave_graphics();
	}
}

void PLSCamEffect::ReleaseOpennedTexture()
{
	obs_enter_graphics();
	if (output_video_tex) {
		gs_texture_destroy(output_video_tex);
		output_video_tex = NULL;
	}
	obs_leave_graphics();
}

void PLSCamEffect::PushEmptyInputVideo()
{
	shared_handle_header hdr = {};
	shared_handle_sample body = {};
	body.handle = 0;
	SetInputVideo(hdr, body);
}

void PLSCamEffect::SetEffectState(std::string state)
{
	if (effect_state != state) {
		effect_state = state;
		blog(LOG_INFO, "Update effect state : %s", state.c_str());
	}
}

void PLSCamEffect::PushRestoreMessage()
{
	face_beauty_message *temp = new face_beauty_message();
	EFFECT_MESSAGE_PTR msg(temp);

	{
		CAutoLockCS autoLock(param_lock);
		memmove(temp, &beauty_params, sizeof(face_beauty_message));
	}

	message_queue.PushMessage(msg, sizeof(face_beauty_message), true);
}

bool PLSCamEffect::RunEffectProcess()
{
	assert(!CHandleWrapper::IsHandleValid(process_handle));
	CHandleWrapper::CloseHandleEx(process_handle);
	process_hwnd = 0;

	::ResetEvent(
		effect_valid_flag); // Effect process will sign this flag if success to init
	PushRestoreMessage(); // send effect params to effect process for restoring
	PushEmptyInputVideo();
	ClearOutputVideo();

	if (!::IsWindow(source_hwnd)) {
		CHandleWrapper::IsHandleSigned(thread_exit_event, 1000);
		return false;
	}

	std::wstring process_path = GetProcessPath();
	if (process_path.empty()) {
		blog(LOG_ERROR, "Get beauty process path failed");
		CHandleWrapper::IsHandleSigned(thread_exit_event, 5000);
		assert(false);
		return false;
	}

	if (!os_is_file_exist_ex(process_path.c_str())) {
		PLSExceptionInfo ecp;
		ecp.exception_happen = true;
		ecp.sub_code = 0;
		ecp.type = OBS_SOURCE_EXCEPTION_NO_FILE;
		PushException(ecp);

		blog(LOG_ERROR, "Beauty process file not exist");
		CHandleWrapper::IsHandleSigned(thread_exit_event, 5000);
		return false;
	}

	{
		CAutoLockCS autoLock(exception_lock);
		exceptions.clear();
	}

	wchar_t cmd[2048];
	swprintf_s(cmd, ARRAY_COUNT(cmd),
		   L"\"%s\" \"%hs\" \"%hs\" \"%hs\" \"%hs\" %d",
		   process_path.c_str(), session_id.c_str(),
		   PLSCamEffect::prism_version.c_str(),
		   PLSCamEffect::nelo_session.c_str(),
		   PLSCamEffect::prism_cfg_path.c_str(), source_hwnd);

	PROCESS_INFORMATION pi = {};
	STARTUPINFO si = {};
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = SW_HIDE;

	BOOL bOK = CreateProcessW((LPWSTR)process_path.c_str(), cmd, NULL, NULL,
				  TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	if (!bOK) {
		blog(LOG_ERROR, "Fail to run effect process! error:%u",
		     GetLastError());
		CHandleWrapper::IsHandleSigned(thread_exit_event, 5000);
		assert(false);
		return false;
	}

	CloseHandle(pi.hThread);
	process_handle = pi.hProcess;

	blog(LOG_INFO, "Success to run effect process. HANDLE:%llu PID:%u",
	     pi.hProcess, pi.dwProcessId);

	return true;
}

bool PLSCamEffect::CheckFindWindow()
{
	if (::IsWindow(process_hwnd)) {
		return true;
	}

	std::string process_hwnd_class = session_id;
	process_hwnd = ::FindWindowA(process_hwnd_class.c_str(), NULL);
	if (::IsWindow(process_hwnd)) {
		blog(LOG_INFO,
		     "Find window of effect process. HANDLE:%llu HWND:%llu",
		     process_handle, process_hwnd);
		previous_heartbeat = GetTickCount();
		return true;
	} else {
		return false;
	}
}

bool PLSCamEffect::CheckRunProcess()
{
	if (CHandleWrapper::IsHandleValid(process_handle)) {
		if (!CHandleWrapper::IsHandleSigned(process_handle)) {
			// effect process is running
			return CheckFindWindow();
		}

		// effect process exited because of an exception !

		DWORD exit_code = 0;
		GetExitCodeProcess(process_handle, &exit_code);

		blog(LOG_ERROR,
		     "Effect process exited because of an exception! code:%u HANDLE:%llu",
		     exit_code, process_handle);

		PLSExceptionInfo ecp;
		ecp.exception_happen = true;
		ecp.sub_code = exit_code;
		ecp.type = OBS_SOURCE_EXCEPTION_PROCESS_EXIT;
		PushException(ecp);

		process_hwnd = 0;
		CHandleWrapper::CloseHandleEx(process_handle);

		HANDLE evt[] = {
			thread_exit_event,
			onoff_changed_event,
		};

		waiting_onoff_change = true;
		WaitForMultipleObjects(ARRAY_COUNT(evt), evt, FALSE, INFINITE);
		waiting_onoff_change = false;

		if (CHandleWrapper::IsHandleSigned(thread_exit_event, 0)) {
			return false;
		}
	}

	// Won't return true until find HWND of effect process
	RunEffectProcess();

	return false;
}

void PLSCamEffect::CheckHeartBeat(bool &heartbeat_timeout)
{
	bool timeout =
		(GetTickCount() - previous_heartbeat >= HEART_BEAT_TIMEOUT);

	if (heartbeat_timeout != timeout) {
		heartbeat_timeout = timeout;
		if (heartbeat_timeout) {
			blog(LOG_ERROR, "Heartbeat is timeout. HANDLE:%llu",
			     process_handle);
		} else {
			blog(LOG_INFO, "Heartbeat is normal. HANDLE:%llu",
			     process_handle);
		}
	}
}

void PLSCamEffect::TrySendMessage(cam_effect_message_info &info)
{
	COPYDATASTRUCT temp = {};
	temp.dwData = info.msg->type;
	temp.lpData = info.msg.get();
	temp.cbData = info.length;

	LRESULT res = ::SendMessageTimeout(process_hwnd, WM_COPYDATA, NULL,
					   (LPARAM)&temp,
					   SMTO_BLOCK | SMTO_ABORTIFHUNG,
					   MESSAGE_TIMEOUT_MS, NULL);
	if (!res) {
		blog(LOG_ERROR,
		     "Timeout while sending message! hr:%X error:%u HANDLE:%llu",
		     res, GetLastError(), process_handle);

		if (info.msg->type == effect_message_face_beauty) {
			PushRestoreMessage(); // push another message with latest params
		} else {
			message_queue.PushMessage(info.msg, info.length, false);
		}

		CHandleWrapper::IsHandleSigned(thread_exit_event, 1000);
	}
}

void PLSCamEffect::WindowLoopThreadInner()
{
	source_hwnd = CreateMessageWindow();
	if (!::IsWindow(source_hwnd)) {
		return;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClassA(source_hwnd_class.c_str(), GetModuleHandle(NULL));
}

void PLSCamEffect::MessageLoopThreadInner()
{
	bool heartbeat_timeout = false;
	while (!CHandleWrapper::IsHandleSigned(thread_exit_event, 50)) {
		if (!CheckRunProcess()) {
			heartbeat_timeout = false; // reset flag
			continue;
		}

		CheckHeartBeat(heartbeat_timeout);
		if (!heartbeat_timeout) {
			cam_effect_message_info info;
			if (message_queue.PopMessage(info)) {
				TrySendMessage(info);
			}
		}
	}
}

obs_source_exception_type TransException(cam_effect_exception_type src)
{
	switch (src) {
	case effect_exception_sensetime:
		return OBS_SOURCE_EXCEPTION_SENSETIME;

	case effect_exception_d3d:
		return OBS_SOURCE_EXCEPTION_D3D;

	case effect_exception_unknown:
	default:
		assert(false);
		return OBS_SOURCE_EXCEPTION_NONE;
	}
}

void PLSCamEffect::OnCopyData(COPYDATASTRUCT *cd)
{
	int min_msg_len = sizeof(cam_effect_message);
	if (!cd->lpData || (cd->cbData < min_msg_len)) {
		return;
	}

	cam_effect_message *msg = (cam_effect_message *)cd->lpData;
	std::string temp = msg->guid;
	if (temp != session_id) {
		return;
	}

	if (msg->type != cd->dwData) {
		assert(false && "incorrect message type");
		return;
	}

	switch (cd->dwData) {
	case effect_message_process_heartbeat:
		assert(cd->cbData == sizeof(heart_beat_message));
		if (cd->cbData == sizeof(heart_beat_message)) {
			previous_heartbeat = GetTickCount();
		}
		break;

	case effect_message_process_exception:
		assert(cd->cbData == sizeof(process_exception_message));
		if (cd->cbData == sizeof(process_exception_message)) {
			process_exception_message *src =
				(process_exception_message *)cd->lpData;

			obs_source_exception_type type =
				TransException(src->exception_type);

			if (type != OBS_SOURCE_EXCEPTION_NONE) {
				PLSExceptionInfo ecp;
				ecp.exception_happen = true;
				ecp.type = type;
				ecp.sub_code = src->sub_code;
				PushException(ecp);
			} else {
				blog(LOG_WARNING,
				     "Received unknown exception type. type:%d",
				     type);
			}
		}
		break;

	default:
		blog(LOG_WARNING, "Received unknown message type. type:%d",
		     (int)cd->dwData);
		break;
	}
}

void PLSCamEffect::PushException(PLSExceptionInfo &ecp)
{
	CAutoLockCS autoLock(exception_lock);

	PLSExceptionInfo info = exceptions[ecp.type];
	if (info.exception_happen && info.notify_done) {
		return; // exception has been notified before
	}

	exceptions[ecp.type] = ecp;
}
