#include <objbase.h>

#include <obs-module.h>
#include <obs.hpp>
#include <util/dstr.hpp>
#include <util/platform.h>
#include <util/windows/WinHandle.hpp>
#include <util/threading.h>
#include "libdshowcapture/dshowcapture.hpp"
#include "ffmpeg-decode.h"
#include "encode-dstr.hpp"
#include "pls-cam-effect/prism-cam-effect.h"

#include <algorithm>
#include <limits>
#include <set>
#include <string>
#include <vector>

/*
 * TODO:
 *   - handle disconnections and reconnections
 *   - if device not present, wait for device to be plugged in
 */

#undef min
#undef max

using namespace std;
using namespace DShow;

/* clang-format off */

/* settings defines that will cause errors if there are typos */
#define VIDEO_DEVICE_ID   "video_device_id"
#define RES_TYPE          "res_type"
#define RESOLUTION        "resolution"
#define FRAME_INTERVAL    "frame_interval"
#define VIDEO_FORMAT      "video_format"
#define LAST_VIDEO_DEV_ID "last_video_device_id"
#define LAST_RESOLUTION   "last_resolution"
#define BUFFERING_VAL     "buffering"
#define FLIP_IMAGE        "flip_vertically"
#define AUDIO_OUTPUT_MODE "audio_output_mode"
#define USE_CUSTOM_AUDIO  "use_custom_audio_device"
#define AUDIO_DEVICE_ID   "audio_device_id"
#define COLOR_SPACE       "color_space"
#define COLOR_RANGE       "color_range"
#define DEACTIVATE_WNS    "deactivate_when_not_showing"
#define CAMERA_BEAUTY	  "camera_beauty"
#define BUTTON_GROUP_TYPE   "button group"

#define TEXT_INPUT_NAME     obs_module_text("VideoCaptureDevice")
#define TEXT_DEVICE         obs_module_text("Device")
#define TEXT_CONFIG_VIDEO   obs_module_text("ConfigureVideo")
#define TEXT_CONFIG_XBAR    obs_module_text("ConfigureCrossbar")
#define TEXT_RES_FPS_TYPE   obs_module_text("ResFPSType")
#define TEXT_CUSTOM_RES     obs_module_text("ResFPSType.Custom")
#define TEXT_PREFERRED_RES  obs_module_text("ResFPSType.DevPreferred")
#define TEXT_FPS_MATCHING   obs_module_text("FPS.Matching")
#define TEXT_FPS_HIGHEST    obs_module_text("FPS.Highest")
#define TEXT_RESOLUTION     obs_module_text("Resolution")
#define TEXT_VIDEO_FORMAT   obs_module_text("VideoFormat")
#define TEXT_FORMAT_UNKNOWN obs_module_text("VideoFormat.Unknown")
#define TEXT_BUFFERING      obs_module_text("Buffering")
#define TEXT_BUFFERING_AUTO obs_module_text("Buffering.AutoDetect")
#define TEXT_BUFFERING_ON   obs_module_text("Buffering.Enable")
#define TEXT_BUFFERING_OFF  obs_module_text("Buffering.Disable")
#define TEXT_FLIP_IMAGE     obs_module_text("FlipVertically")
#define TEXT_AUDIO_MODE     obs_module_text("AudioOutputMode")
#define TEXT_MODE_CAPTURE   obs_module_text("AudioOutputMode.Capture")
#define TEXT_MODE_DSOUND    obs_module_text("AudioOutputMode.DirectSound")
#define TEXT_MODE_WAVEOUT   obs_module_text("AudioOutputMode.WaveOut")
#define TEXT_CUSTOM_AUDIO   obs_module_text("UseCustomAudioDevice")
#define TEXT_AUDIO_DEVICE   obs_module_text("AudioDevice")
#define TEXT_ACTIVATE       obs_module_text("Activate")
#define TEXT_DEACTIVATE     obs_module_text("Deactivate")
#define TEXT_COLOR_SPACE    obs_module_text("ColorSpace")
#define TEXT_COLOR_DEFAULT  obs_module_text("ColorSpace.Default")
#define TEXT_COLOR_RANGE    obs_module_text("ColorRange")
#define TEXT_RANGE_DEFAULT  obs_module_text("ColorRange.Default")
#define TEXT_RANGE_PARTIAL  obs_module_text("ColorRange.Partial")
#define TEXT_RANGE_FULL     obs_module_text("ColorRange.Full")
#define TEXT_DWNS           obs_module_text("DeactivateWhenNotShowing")
#define TEXT_BEAUTY         obs_module_text("CameraBeauty")
#define TEXT_BEAUTY_CUTE    obs_module_text("Beauty.cute")
#define TEXT_BEAUTY_NATURAL obs_module_text("Beauty.natural")
#define TEXT_BEAUTY_SHARP   obs_module_text("Beauty.sharp")

/* clang-format on */

enum ResType {
	ResType_Preferred,
	ResType_Custom,
};

enum class BufferingType : int64_t {
	Auto,
	On,
	Off,
};

void ffmpeg_log(void *bla, int level, const char *msg, va_list args)
{
	DStr str;
	if (level == AV_LOG_WARNING)
		dstr_copy(str, "warning: ");
	else if (level == AV_LOG_ERROR)
		dstr_copy(str, "error:   ");
	else if (level < AV_LOG_ERROR)
		dstr_copy(str, "fatal:   ");
	else
		return;

	dstr_cat(str, msg);
	if (dstr_end(str) == '\n')
		dstr_resize(str, str->len - 1);

	blogva(LOG_WARNING, str, args);
	av_log_default_callback(bla, level, msg, args);
}

class Decoder {
	struct ffmpeg_decode decode;

public:
	inline Decoder() { memset(&decode, 0, sizeof(decode)); }
	inline ~Decoder() { ffmpeg_decode_free(&decode); }

	inline operator ffmpeg_decode *() { return &decode; }
	inline ffmpeg_decode *operator->() { return &decode; }
};

class CriticalSection {
	CRITICAL_SECTION mutex;

public:
	inline CriticalSection() { InitializeCriticalSection(&mutex); }
	inline ~CriticalSection() { DeleteCriticalSection(&mutex); }

	inline operator CRITICAL_SECTION *() { return &mutex; }
};

class CriticalScope {
	CriticalSection &mutex;

	CriticalScope() = delete;
	CriticalScope &operator=(CriticalScope &cs) = delete;

public:
	inline CriticalScope(CriticalSection &mutex_) : mutex(mutex_)
	{
		EnterCriticalSection(mutex);
	}

	inline ~CriticalScope() { LeaveCriticalSection(mutex); }
};

//PRISM/WangShaohui/20200117/#281/for source unavailable
enum Action {
	None = 0,
	Activate = 0x00000001,
	ActivateBlock = 0x00000002,
	Deactivate = 0x00000004,
	Shutdown = 0x00000008,
	ConfigVideo = 0x00000010,
	ConfigAudio = 0x00000020,
	ConfigCrossbar1 = 0x00000040,
	ConfigCrossbar2 = 0x00000080,
	OnRemoved = 0x00000100,
	OnInserted = 0x00000200,
	OnRetry = 0x00000400,
};

enum CamOperation {
	OptNone = 0,
	SwitchEffect,
	SwitchActive,
};

//PRISM/WangShaohui/20200117/#281/for source unavailable
#define INIT_DEVICE_FLAGS                                          \
	(Action::OnInserted | Action::OnRetry | Action::Activate | \
	 Action::ActivateBlock)

static DWORD CALLBACK DShowThread(LPVOID ptr);

static DWORD CALLBACK render_dest_capture(LPVOID ptr);

struct DShowInput {
	obs_source_t *source;
	Device device;
	bool deactivateWhenNotShowing = false;
	bool deviceHasAudio = false;
	bool deviceHasSeparateAudioFilter = false;
	bool flip = false;
	bool active = false;

	Decoder audio_decoder;
	Decoder video_decoder;

	VideoConfig videoConfig;
	AudioConfig audioConfig;

	video_range_type range;
	obs_source_frame2 frame;
	obs_source_audio audio;

	//PRISM/WangShaohui/20200117/#281/for source unavailable
	bool released;

	WinHandle semaphore;
	WinHandle activated_event;
	WinHandle thread;
	CriticalSection mutex;
	vector<Action> actions;

	//PRISM/Wang.Chuanjing/20200313/#990/for camera effect
	PLSCamEffect cam_effect;
	CamOperation previous_operation;

	//PRISM/WangShaohui/20200709/#3244/open camera's window as model window
	HWND property_hwnd;

	inline void QueueAction(Action action)
	{
		CriticalScope scope(mutex);
		actions.push_back(action);
		ReleaseSemaphore(semaphore, 1, nullptr);
	}

	//PRISM/WangShaohui/20200117/#281/for source unavailable
	inline void EraseAction(DWORD acts)
	{
		CriticalScope scope(mutex);

		vector<Action>::iterator itr = actions.begin();
		while (itr != actions.end()) {
			if ((*itr) & acts) {
				itr = actions.erase(itr);
				continue;
			}
			++itr;
		}
	}

	//PRISM/WangShaohui/20200117/#281/for source unavailable
	inline bool IncludeAction(DWORD acts)
	{
		CriticalScope scope(mutex);

		vector<Action>::iterator itr = actions.begin();
		for (; itr != actions.end(); ++itr) {
			if ((*itr) & acts)
				return true;
		}

		return false;
	}

	inline void QueueActivate(obs_data_t *settings)
	{
		bool block =
			obs_data_get_bool(settings, "synchronous_activate");
		QueueAction(block ? Action::ActivateBlock : Action::Activate);
		if (block) {
			obs_data_erase(settings, "synchronous_activate");
			WaitForSingleObject(activated_event, INFINITE);
		}
	}

	inline DShowInput(obs_source_t *source_, obs_data_t *settings)
		: source(source_),
		  previous_operation(OptNone),
		  property_hwnd(0),
		  device(InitGraph::False,
			 std::bind(&DShowInput::OnDShowEvent, this,
				   placeholders::_1, placeholders::_2))
	{
		memset(&audio, 0, sizeof(audio));
		memset(&frame, 0, sizeof(frame));

		av_log_set_level(AV_LOG_WARNING);
		av_log_set_callback(ffmpeg_log);

		//PRISM/WangShaohui/20200117/#281/for source unavailable
		released = false;
		semaphore = CreateSemaphore(nullptr, 0, 0x7FFFFFFF, nullptr);
		if (!semaphore)
			throw "Failed to create semaphore";

		activated_event = CreateEvent(nullptr, false, false, nullptr);
		if (!activated_event)
			throw "Failed to create activated_event";

		thread =
			CreateThread(nullptr, 0, DShowThread, this, 0, nullptr);
		if (!thread)
			throw "Failed to create thread";

		deactivateWhenNotShowing =
			obs_data_get_bool(settings, DEACTIVATE_WNS);

		if (obs_data_get_bool(settings, "active")) {
			bool showing = obs_source_showing(source);
			if (!deactivateWhenNotShowing || showing)
				QueueActivate(settings);

			active = true;
		}
	}

	inline ~DShowInput()
	{
		{
			CriticalScope scope(mutex);
			actions.resize(1);
			actions[0] = Action::Shutdown;
		}

		//PRISM/WangShaohui/20200117/#281/for source unavailable
		released = true;
		ReleaseSemaphore(semaphore, 1, nullptr);

		//PRISM/WangShaohui/20200612/NoIssue/for checking delete source
		blog(LOG_INFO, "[DShow Device: '%s'] Wait thread end",
		     obs_source_get_name(source));

		WaitForSingleObject(thread, INFINITE);

		//PRISM/WangShaohui/20200612/NoIssue/for checking delete source
		blog(LOG_INFO, "[DShow Device: '%s'] deleted completely",
		     obs_source_get_name(source));
	}

	void OnEncodedVideoData(enum AVCodecID id, unsigned char *data,
				size_t size, long long ts, uint64_t sys_time);
	void OnEncodedAudioData(enum AVCodecID id, unsigned char *data,
				size_t size, long long ts);

	//PRISM/WangShaohui/20200117/#281/for source unavailable
	void OnDShowEvent(DShowEvent evt, void *params);
	void OnVideoData(const VideoConfig &config, unsigned char *data,
			 size_t size, long long startTime, long long endTime);
	void OnAudioData(const AudioConfig &config, unsigned char *data,
			 size_t size, long long startTime, long long endTime);

	//PRISM/WangShaohui/20201013/NoIssue/adjust log number
	bool UpdateVideoConfig(obs_data_t *settings, bool saveLog);
	bool UpdateAudioConfig(obs_data_t *settings);
	void SetActive(bool active);
	inline enum video_colorspace GetColorSpace(obs_data_t *settings) const;
	inline enum video_range_type GetColorRange(obs_data_t *settings) const;
	//PRISM/WangShaohui/20200210/#281/for source unavailable
	inline enum obs_source_error Activate(obs_data_t *settings,
					      bool saveLog);
	inline void Deactivate();

	inline void SetupBuffering(obs_data_t *settings);

	void DShowLoop();

	//PRISM/WangShaohui/20200210/#281/for source unavailable
	bool IsDevceExist();

	//PRISM/Wang.Chuanjing/20200313/#990/for camera effect
};

static DWORD CALLBACK DShowThread(LPVOID ptr)
{
	DShowInput *dshowInput = (DShowInput *)ptr;

	os_set_thread_name("win-dshow: DShowThread");

	CoInitialize(nullptr);
	dshowInput->DShowLoop();
	CoUninitialize();
	return 0;
}

static inline void ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//PRISM/WangShaohui/20200117/#281/for source unavailable
#define RETRY_INTERVAL 3000
#define WAIT_ACTION_TIMEOUT RETRY_INTERVAL

//PRISM/WangShaohui/20200210/#281/for source unavailable
bool DShowInput::IsDevceExist()
{
	vector<VideoDevice> devices;
	Device::EnumVideoDevices(devices);

	size_t count = devices.size();
	for (size_t i = 0; i < count; ++i) {
		if (devices[i].path == videoConfig.path)
			return true;
	}

	return false;
}

/* Always keep directshow in a single thread for a given device */
void DShowInput::DShowLoop()
{
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	DWORD preInitTime = 0; // millisecond

	while (true) {
		//PRISM/WangShaohui/20200117/#281/for source unavailable
		DWORD ret = MsgWaitForMultipleObjects(
			1, &semaphore, false, WAIT_ACTION_TIMEOUT, QS_ALLINPUT);
		if (ret == (WAIT_OBJECT_0 + 1)) {
			ProcessMessages();
			continue;
		} else if (ret != WAIT_OBJECT_0) {
			//PRISM/WangShaohui/20200117/#281/for source unavailable
			if (released) {
				device.ShutdownGraph();
				break;
			}
		}

		Action action = Action::None;
		{
			CriticalScope scope(mutex);
			if (actions.size()) {
				action = actions.front();
				actions.erase(actions.begin());
			}
		}

		switch (action) {
		//PRISM/WangShaohui/20200117/#281/for source unavailable
		case Action::OnRemoved:
			obs_source_output_video2(source, nullptr);
			obs_source_set_capture_valid(
				source, false, OBS_SOURCE_ERROR_NOT_FOUND);

			//PRISM/WangChuanjing/20200507/for beauty
			obs_source_set_image_status(source, false);
			cam_effect.SetCaptureState(false);

			break;

		case Action::OnRetry:
			if (!active) {
				break;
			}

		case Action::OnInserted:
		case Action::Activate:
		case Action::ActivateBlock: {
			bool block = action == Action::ActivateBlock;

			//PRISM/WangShaohui/20200117/#281/for source unavailable
			obs_data_t *settings = obs_source_get_settings(source);
			//PRISM/WangShaohui/20201013/NoIssue/adjust log number
			enum obs_source_error error =
				Activate(settings, action != Action::OnRetry);
			if (error != OBS_SOURCE_ERROR_OK) {
				//PRISM/WangShaohui/20200507/#2570/for uninit device
				device.ResetGraph();
				cam_effect.SetCaptureState(false);

				obs_source_output_video2(source, nullptr);
				if (error == OBS_SOURCE_ERROR_BE_USING) {
					obs_source_set_capture_valid(
						source, false, error);
				} else {
					string video_device_id =
						obs_data_get_string(
							settings,
							VIDEO_DEVICE_ID);
					if (video_device_id.empty()) {
						obs_source_set_capture_valid(
							source, true,
							OBS_SOURCE_ERROR_OK);
					} else {
						obs_source_set_capture_valid(
							source, false,
							IsDevceExist()
								? OBS_SOURCE_ERROR_UNKNOWN
								: OBS_SOURCE_ERROR_NOT_FOUND);
					}
				}
			} else {
				// cancel the delayed event because device becomes available
				EraseAction(Action::OnRemoved);
				obs_source_set_capture_valid(
					source, true, OBS_SOURCE_ERROR_OK);
			}

			if (block)
				SetEvent(activated_event);
			obs_data_release(settings);

			//PRISM/WangShaohui/20200117/#281/for source unavailable
			preInitTime = GetTickCount();
			break;
		}

		case Action::Deactivate:
			Deactivate();
			break;

		case Action::Shutdown:
			device.ShutdownGraph();
			return;

		case Action::ConfigVideo:
			//PRISM/WangShaohui/20200709/#3244/open camera's window as model window
			device.OpenDialog(property_hwnd,
					  DialogType::ConfigVideo);
			break;

		case Action::ConfigAudio:
			//PRISM/WangShaohui/20200709/#3244/open camera's window as model window
			device.OpenDialog(property_hwnd,
					  DialogType::ConfigAudio);
			break;

		case Action::ConfigCrossbar1:
			//PRISM/WangShaohui/20200709/#3244/open camera's window as model window
			device.OpenDialog(property_hwnd,
					  DialogType::ConfigCrossbar);
			break;

		case Action::ConfigCrossbar2:
			//PRISM/WangShaohui/20200709/#3244/open camera's window as model window
			device.OpenDialog(property_hwnd,
					  DialogType::ConfigCrossbar2);
			break;

		case Action::None:
			//PRISM/WangShaohui/20200117/#281/for source unavailable
			if (!obs_source_get_capture_valid(source, NULL) &&
			    !IncludeAction(INIT_DEVICE_FLAGS) &&
			    GetTickCount() - preInitTime >= RETRY_INTERVAL) {
				preInitTime = GetTickCount();
				QueueAction(Action::OnRetry);
			}
			break;
		}
	}
}

#define FPS_HIGHEST 0LL
#define FPS_MATCHING -1LL

template<typename T, typename U, typename V>
static bool between(T &&lower, U &&value, V &&upper)
{
	return value >= lower && value <= upper;
}

static bool ResolutionAvailable(const VideoInfo &cap, int cx, int cy)
{
	return between(cap.minCX, cx, cap.maxCX) &&
	       between(cap.minCY, cy, cap.maxCY);
}

#define DEVICE_INTERVAL_DIFF_LIMIT 20

static bool FrameRateAvailable(const VideoInfo &cap, long long interval)
{
	return interval == FPS_HIGHEST || interval == FPS_MATCHING ||
	       between(cap.minInterval - DEVICE_INTERVAL_DIFF_LIMIT, interval,
		       cap.maxInterval + DEVICE_INTERVAL_DIFF_LIMIT);
}

static long long FrameRateInterval(const VideoInfo &cap,
				   long long desired_interval)
{
	return desired_interval < cap.minInterval
		       ? cap.minInterval
		       : min(desired_interval, cap.maxInterval);
}

static inline video_format ConvertVideoFormat(VideoFormat format)
{
	switch (format) {
	case VideoFormat::ARGB:
		return VIDEO_FORMAT_BGRA;
	case VideoFormat::XRGB:
		return VIDEO_FORMAT_BGRX;
	case VideoFormat::I420:
		return VIDEO_FORMAT_I420;
	case VideoFormat::YV12:
		return VIDEO_FORMAT_I420;
	case VideoFormat::NV12:
		return VIDEO_FORMAT_NV12;
	case VideoFormat::Y800:
		return VIDEO_FORMAT_Y800;
	case VideoFormat::YVYU:
		return VIDEO_FORMAT_YVYU;
	case VideoFormat::YUY2:
		return VIDEO_FORMAT_YUY2;
	case VideoFormat::UYVY:
		return VIDEO_FORMAT_UYVY;
	case VideoFormat::HDYC:
		return VIDEO_FORMAT_UYVY;
	default:
		return VIDEO_FORMAT_NONE;
	}
}

static inline audio_format ConvertAudioFormat(AudioFormat format)
{
	switch (format) {
	case AudioFormat::Wave16bit:
		return AUDIO_FORMAT_16BIT;
	case AudioFormat::WaveFloat:
		return AUDIO_FORMAT_FLOAT;
	default:
		return AUDIO_FORMAT_UNKNOWN;
	}
}

static inline enum speaker_layout convert_speaker_layout(uint8_t channels)
{
	switch (channels) {
	case 0:
		return SPEAKERS_UNKNOWN;
	case 1:
		return SPEAKERS_MONO;
	case 2:
		return SPEAKERS_STEREO;
	case 3:
		return SPEAKERS_2POINT1;
	case 4:
		return SPEAKERS_4POINT0;
	case 5:
		return SPEAKERS_4POINT1;
	case 6:
		return SPEAKERS_5POINT1;
	case 8:
		return SPEAKERS_7POINT1;
	default:
		return SPEAKERS_UNKNOWN;
	}
}

//PRISM/WangShaohui/20200117/#281/for source unavailable
static inline void CheckResetDecoder(Decoder &dec, enum AVCodecID id)
{
	if (ffmpeg_decode_valid(dec)) {
		if (dec->codec->id != id)
			ffmpeg_decode_free(dec);
	}
}

//#define LOG_ENCODED_VIDEO_TS 1
//#define LOG_ENCODED_AUDIO_TS 1

#define MAX_SW_RES_INT (1920 * 1080)

void DShowInput::OnEncodedVideoData(enum AVCodecID id, unsigned char *data,
				    size_t size, long long ts,
				    uint64_t sys_time)
{
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	CheckResetDecoder(video_decoder, id);
	if (!ffmpeg_decode_valid(video_decoder)) {
		/* Only use MJPEG hardware decoding on resolutions higher
		 * than 1920x1080.  The reason why is because we want to strike
		 * a reasonable balance between hardware and CPU usage. */
		bool useHW = videoConfig.format != VideoFormat::MJPEG ||
			     (videoConfig.cx * videoConfig.cy) > MAX_SW_RES_INT;
		if (ffmpeg_decode_init(video_decoder, id, useHW) < 0) {
			blog(LOG_WARNING, "Could not initialize video decoder");
			return;
		}
	}

	bool got_output;
	bool success = ffmpeg_decode_video(video_decoder, data, size, &ts,
					   range, &frame, &got_output);
	if (!success) {
		blog(LOG_WARNING, "Error decoding video");
		return;
	}

	if (got_output) {
		//PRISM/LiuHaibin/20200609/#3174/camera effect
		frame.sys_timestamp = sys_time;
		frame.timestamp = (uint64_t)ts * 100;
		if (flip)
			frame.flip = !frame.flip;
#if LOG_ENCODED_VIDEO_TS
		blog(LOG_DEBUG, "video ts: %llu", frame.timestamp);
#endif
		obs_source_output_video2(source, &frame);
	}
}

//PRISM/WangShaohui/20200117/#281/for source unavailable
void DShowInput::OnDShowEvent(DShowEvent evt, void *params)
{
	switch (evt) {
	case DShow::DShowEvent::DeviceErrorAbort:
	case DShow::DShowEvent::DeviceRemoved:
		QueueAction(Action::OnRemoved);
		break;

	case DShow::DShowEvent::DeviceInserted:
		if (!IncludeAction(INIT_DEVICE_FLAGS))
			QueueAction(Action::OnInserted);
		break;

	default:
		break;
	}
}

void DShowInput::OnVideoData(const VideoConfig &config, unsigned char *data,
			     size_t size, long long startTime,
			     long long endTime)
{
	//PRISM/LiuHaibin/20200609/#3174/camera effect
	uint64_t sys_time = os_gettime_ns();

	//PRISM/Wang.Chuanjing/20200506/for camera effect
	obs_source_set_image_status(source, true);
	cam_effect.SetCaptureState(true);

	if (videoConfig.format == VideoFormat::H264) {
		OnEncodedVideoData(AV_CODEC_ID_H264, data, size, startTime,
				   sys_time);
		return;
	}

	if (videoConfig.format == VideoFormat::MJPEG) {
		OnEncodedVideoData(AV_CODEC_ID_MJPEG, data, size, startTime,
				   sys_time);
		return;
	}

	const int cx = config.cx;
	const int cy = config.cy;

	frame.timestamp = (uint64_t)startTime * 100;
	frame.width = config.cx;
	frame.height = config.cy;
	frame.format = ConvertVideoFormat(config.format);

	//PRISM/LiuHaibin/20200609/#3174/camera effect
	frame.flip = frame.ori_img_flip = (config.format == VideoFormat::XRGB ||
					   config.format == VideoFormat::ARGB);
	frame.sys_timestamp = sys_time;

	if (flip)
		frame.flip = !frame.flip;

	if (videoConfig.format == VideoFormat::XRGB ||
	    videoConfig.format == VideoFormat::ARGB) {
		frame.data[0] = data;
		frame.linesize[0] = cx * 4;

		//PRISM/WangShaohui/20200424/#2317/for checking "Iriun Webcam"
		int expect_size = cx * cy * 4;
		if (size < expect_size) {
			// #2317 Iriun Webcam will push incorrect buffer, so we have to check its buffer size here.
			return;
		}

	} else if (videoConfig.format == VideoFormat::YVYU ||
		   videoConfig.format == VideoFormat::YUY2 ||
		   videoConfig.format == VideoFormat::HDYC ||
		   videoConfig.format == VideoFormat::UYVY) {
		frame.data[0] = data;
		frame.linesize[0] = cx * 2;

	} else if (videoConfig.format == VideoFormat::I420) {
		frame.data[0] = data;
		frame.data[1] = frame.data[0] + (cx * cy);
		frame.data[2] = frame.data[1] + (cx * cy / 4);
		frame.linesize[0] = cx;
		frame.linesize[1] = cx / 2;
		frame.linesize[2] = cx / 2;

	} else if (videoConfig.format == VideoFormat::YV12) {
		frame.data[0] = data;
		frame.data[2] = frame.data[0] + (cx * cy);
		frame.data[1] = frame.data[2] + (cx * cy / 4);
		frame.linesize[0] = cx;
		frame.linesize[1] = cx / 2;
		frame.linesize[2] = cx / 2;

	} else if (videoConfig.format == VideoFormat::NV12) {
		frame.data[0] = data;
		frame.data[1] = frame.data[0] + (cx * cy);
		frame.linesize[0] = cx;
		frame.linesize[1] = cx;

	} else if (videoConfig.format == VideoFormat::Y800) {
		frame.data[0] = data;
		frame.linesize[0] = cx;

	} else {
		/* TODO: other formats */
		return;
	}

	obs_source_output_video2(source, &frame);

	UNUSED_PARAMETER(endTime); /* it's the enndd tiimmes! */
	UNUSED_PARAMETER(size);
}

void DShowInput::OnEncodedAudioData(enum AVCodecID id, unsigned char *data,
				    size_t size, long long ts)
{
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	CheckResetDecoder(audio_decoder, id);
	if (!ffmpeg_decode_valid(audio_decoder)) {
		if (ffmpeg_decode_init(audio_decoder, id, false) < 0) {
			blog(LOG_WARNING, "Could not initialize audio decoder");
			return;
		}
	}

	bool got_output = false;
	do {
		bool success = ffmpeg_decode_audio(audio_decoder, data, size,
						   &audio, &got_output);
		if (!success) {
			blog(LOG_WARNING, "Error decoding audio");
			return;
		}

		if (got_output) {
			audio.timestamp = (uint64_t)ts * 100;
#if LOG_ENCODED_AUDIO_TS
			blog(LOG_DEBUG, "audio ts: %llu", audio.timestamp);
#endif
			obs_source_output_audio(source, &audio);
		} else {
			break;
		}

		ts += int64_t(audio_decoder->frame->nb_samples) * 10000000LL /
		      int64_t(audio_decoder->frame->sample_rate);
		size = 0;
		data = nullptr;
	} while (got_output);
}

void DShowInput::OnAudioData(const AudioConfig &config, unsigned char *data,
			     size_t size, long long startTime,
			     long long endTime)
{
	size_t block_size;

	if (config.format == AudioFormat::AAC) {
		OnEncodedAudioData(AV_CODEC_ID_AAC, data, size, startTime);
		return;
	} else if (config.format == AudioFormat::AC3) {
		OnEncodedAudioData(AV_CODEC_ID_AC3, data, size, startTime);
		return;
	} else if (config.format == AudioFormat::MPGA) {
		OnEncodedAudioData(AV_CODEC_ID_MP2, data, size, startTime);
		return;
	}

	audio.speakers = convert_speaker_layout((uint8_t)config.channels);
	audio.format = ConvertAudioFormat(config.format);
	audio.samples_per_sec = (uint32_t)config.sampleRate;
	audio.data[0] = data;

	block_size = get_audio_bytes_per_channel(audio.format) *
		     get_audio_channels(audio.speakers);

	audio.frames = (uint32_t)(size / block_size);
	audio.timestamp = (uint64_t)startTime * 100;

	if (audio.format != AUDIO_FORMAT_UNKNOWN)
		obs_source_output_audio(source, &audio);

	UNUSED_PARAMETER(endTime);
}

struct PropertiesData {
	DShowInput *input;
	vector<VideoDevice> devices;
	vector<AudioDevice> audioDevices;

	bool GetDevice(VideoDevice &device, const char *encoded_id) const
	{
		DeviceId deviceId;
		DecodeDeviceId(deviceId, encoded_id);

		for (const VideoDevice &curDevice : devices) {
			if (deviceId.name == curDevice.name &&
			    deviceId.path == curDevice.path) {
				device = curDevice;
				return true;
			}
		}

		return false;
	}
};

static inline bool ConvertRes(int &cx, int &cy, const char *res)
{
	return sscanf(res, "%dx%d", &cx, &cy) == 2;
}

static inline bool FormatMatches(VideoFormat left, VideoFormat right)
{
	return left == VideoFormat::Any || right == VideoFormat::Any ||
	       left == right;
}

static inline bool ResolutionValid(string res, int &cx, int &cy)
{
	if (!res.size())
		return false;

	return ConvertRes(cx, cy, res.c_str());
}

static inline bool CapsMatch(const VideoInfo &)
{
	return true;
}

template<typename... F> static bool CapsMatch(const VideoDevice &dev, F... fs);

template<typename F, typename... Fs>
static inline bool CapsMatch(const VideoInfo &info, F &&f, Fs... fs)
{
	return f(info) && CapsMatch(info, fs...);
}

template<typename... F> static bool CapsMatch(const VideoDevice &dev, F... fs)
{
	// no early exit, trigger all side effects.
	bool match = false;
	for (const VideoInfo &info : dev.caps)
		if (CapsMatch(info, fs...))
			match = true;
	return match;
}

static inline bool MatcherMatchVideoFormat(VideoFormat format, bool &did_match,
					   const VideoInfo &info)
{
	bool match = FormatMatches(format, info.format);
	did_match = did_match || match;
	return match;
}

static inline bool MatcherClosestFrameRateSelector(long long interval,
						   long long &best_match,
						   const VideoInfo &info)
{
	long long current = FrameRateInterval(info, interval);
	if (llabs(interval - best_match) > llabs(interval - current))
		best_match = current;
	return true;
}

#if 0
auto ResolutionMatcher = [](int cx, int cy)
{
	return [cx, cy](const VideoInfo &info)
	{
		return ResolutionAvailable(info, cx, cy);
	};
};

auto FrameRateMatcher = [](long long interval)
{
	return [interval](const VideoInfo &info)
	{
		return FrameRateAvailable(info, interval);
	};
};

auto VideoFormatMatcher = [](VideoFormat format, bool &did_match)
{
	return [format, &did_match](const VideoInfo &info)
	{
		return MatcherMatchVideoFormat(format, did_match, info);
	};
};

auto ClosestFrameRateSelector = [](long long interval, long long &best_match)
{
	return [interval, &best_match](const VideoInfo &info) mutable -> bool
	{
		MatcherClosestFrameRateSelector(interval, best_match, info);
	};
}
#else
#define ResolutionMatcher(cx, cy)                         \
	[cx, cy](const VideoInfo &info) -> bool {         \
		return ResolutionAvailable(info, cx, cy); \
	}
#define FrameRateMatcher(interval)                         \
	[interval](const VideoInfo &info) -> bool {        \
		return FrameRateAvailable(info, interval); \
	}
#define VideoFormatMatcher(format, did_match)                            \
	[format, &did_match](const VideoInfo &info) mutable -> bool {    \
		return MatcherMatchVideoFormat(format, did_match, info); \
	}
#define ClosestFrameRateSelector(interval, best_match)                       \
	[interval, &best_match](const VideoInfo &info) mutable -> bool {     \
		return MatcherClosestFrameRateSelector(interval, best_match, \
						       info);                \
	}
#endif

static bool ResolutionAvailable(const VideoDevice &dev, int cx, int cy)
{
	return CapsMatch(dev, ResolutionMatcher(cx, cy));
}

static bool DetermineResolution(int &cx, int &cy, obs_data_t *settings,
				VideoDevice dev)
{
	const char *res = obs_data_get_autoselect_string(settings, RESOLUTION);
	if (obs_data_has_autoselect_value(settings, RESOLUTION) &&
	    ConvertRes(cx, cy, res) && ResolutionAvailable(dev, cx, cy))
		return true;

	res = obs_data_get_string(settings, RESOLUTION);
	if (ConvertRes(cx, cy, res) && ResolutionAvailable(dev, cx, cy))
		return true;

	res = obs_data_get_string(settings, LAST_RESOLUTION);
	if (ConvertRes(cx, cy, res) && ResolutionAvailable(dev, cx, cy))
		return true;

	return false;
}

static long long GetOBSFPS();

static inline bool IsDelayedDevice(const VideoConfig &config)
{
	return config.format > VideoFormat::MJPEG ||
	       wstrstri(config.name.c_str(), L"elgato") != NULL ||
	       wstrstri(config.name.c_str(), L"stream engine") != NULL;
}

static inline bool IsDecoupled(const VideoConfig &config)
{
	return wstrstri(config.name.c_str(), L"GV-USB2") != NULL;
}

inline void DShowInput::SetupBuffering(obs_data_t *settings)
{
	BufferingType bufType;
	bool useBuffering;

	bufType = (BufferingType)obs_data_get_int(settings, BUFFERING_VAL);

	if (bufType == BufferingType::Auto)
		useBuffering = IsDelayedDevice(videoConfig);
	else
		useBuffering = bufType == BufferingType::On;

	obs_source_set_async_unbuffered(source, !useBuffering);
	obs_source_set_async_decoupled(source, IsDecoupled(videoConfig));
}

static DStr GetVideoFormatName(VideoFormat format);

bool DShowInput::UpdateVideoConfig(obs_data_t *settings, bool saveLog)
{
	string video_device_id = obs_data_get_string(settings, VIDEO_DEVICE_ID);
	deactivateWhenNotShowing = obs_data_get_bool(settings, DEACTIVATE_WNS);
	flip = obs_data_get_bool(settings, FLIP_IMAGE);

	DeviceId id;
	if (!DecodeDeviceId(id, video_device_id.c_str())) {
		//PRISM/WangShaohui/20201013/NoIssue/adjust log number
		if (saveLog) {
			blog(LOG_WARNING,
			     "%s: DecodeDeviceId failed (cannot parse device id)",
			     obs_source_get_name(source));
		}
		return false;
	}

	PropertiesData data;
	Device::EnumVideoDevices(data.devices);
	VideoDevice dev;
	if (!data.GetDevice(dev, video_device_id.c_str())) {
		//PRISM/WangShaohui/20201013/NoIssue/adjust log number
		if (saveLog) {
			blog(LOG_WARNING,
			     "%s: data.GetDevice failed (cannot find device)",
			     obs_source_get_name(source));
		}
		return false;
	}

	int resType = (int)obs_data_get_int(settings, RES_TYPE);
	int cx = 0, cy = 0;
	long long interval = 0;
	VideoFormat format = VideoFormat::Any;

	if (resType == ResType_Custom) {
		bool has_autosel_val;
		string resolution = obs_data_get_string(settings, RESOLUTION);
		if (!ResolutionValid(resolution, cx, cy)) {
			//PRISM/WangShaohui/20201013/NoIssue/adjust log number
			if (saveLog) {
				blog(LOG_WARNING, "%s: ResolutionValid failed",
				     obs_source_get_name(source));
			}
			return false;
		}

		has_autosel_val =
			obs_data_has_autoselect_value(settings, FRAME_INTERVAL);
		interval = has_autosel_val
				   ? obs_data_get_autoselect_int(settings,
								 FRAME_INTERVAL)
				   : obs_data_get_int(settings, FRAME_INTERVAL);

		if (interval == FPS_MATCHING)
			interval = GetOBSFPS();

		format = (VideoFormat)obs_data_get_int(settings, VIDEO_FORMAT);

		long long best_interval = numeric_limits<long long>::max();
		bool video_format_match = false;
		bool caps_match = CapsMatch(
			dev, ResolutionMatcher(cx, cy),
			VideoFormatMatcher(format, video_format_match),
			ClosestFrameRateSelector(interval, best_interval),
			FrameRateMatcher(interval));

		if (!caps_match && !video_format_match) {
			//PRISM/WangShaohui/20201013/NoIssue/adjust log number
			if (saveLog) {
				blog(LOG_WARNING,
				     "%s: Video format match failed",
				     obs_source_get_name(source));
			}
			return false;
		}

		interval = best_interval;
	}

	//PRISM/WangShaohui/20200312/#1490/for clear texture
	if (videoConfig.path != id.path || videoConfig.name != id.name)
		obs_source_output_video2(source, NULL);

	videoConfig.name = id.name.c_str();
	videoConfig.path = id.path.c_str();
	videoConfig.useDefaultConfig = resType == ResType_Preferred;
	videoConfig.cx = cx;
	videoConfig.cy = cy;
	videoConfig.frameInterval = interval;
	videoConfig.internalFormat = format;

	deviceHasAudio = dev.audioAttached;
	deviceHasSeparateAudioFilter = dev.separateAudioFilter;

	videoConfig.callback = std::bind(&DShowInput::OnVideoData, this,
					 placeholders::_1, placeholders::_2,
					 placeholders::_3, placeholders::_4,
					 placeholders::_5);

	videoConfig.format = videoConfig.internalFormat;

	if (!device.SetVideoConfig(&videoConfig)) {
		//PRISM/WangShaohui/20201013/NoIssue/adjust log number
		if (saveLog) {
			blog(LOG_WARNING, "%s: device.SetVideoConfig failed",
			     obs_source_get_name(source));
		}
		return false;
	}

	DStr formatName = GetVideoFormatName(videoConfig.internalFormat);

	double fps = 0.0;

	if (videoConfig.frameInterval)
		fps = 10000000.0 / double(videoConfig.frameInterval);

	BPtr<char> name_utf8;
	BPtr<char> path_utf8;
	os_wcs_to_utf8_ptr(videoConfig.name.c_str(), videoConfig.name.size(),
			   &name_utf8);
	os_wcs_to_utf8_ptr(videoConfig.path.c_str(), videoConfig.path.size(),
			   &path_utf8);

	blog(LOG_INFO, "---------------------------------");
	blog(LOG_INFO,
	     "[DShow Device: '%s'] settings updated: \n"
	     "\tvideo device: %s\n"
	     "\tvideo path: %s\n"
	     "\tresolution: %dx%d\n"
	     "\tfps: %0.2f (interval: %lld)\n"
	     "\tformat: %s",
	     obs_source_get_name(source), (const char *)name_utf8,
	     (const char *)path_utf8, videoConfig.cx, videoConfig.cy, fps,
	     videoConfig.frameInterval, formatName->array);

	SetupBuffering(settings);

	return true;
}

bool DShowInput::UpdateAudioConfig(obs_data_t *settings)
{
	string audio_device_id = obs_data_get_string(settings, AUDIO_DEVICE_ID);
	bool useCustomAudio = obs_data_get_bool(settings, USE_CUSTOM_AUDIO);

	if (useCustomAudio) {
		DeviceId id;
		if (!DecodeDeviceId(id, audio_device_id.c_str()))
			return false;

		audioConfig.name = id.name.c_str();
		audioConfig.path = id.path.c_str();

	} else if (!deviceHasAudio) {
		return true;
	}

	audioConfig.useVideoDevice = !useCustomAudio &&
				     !deviceHasSeparateAudioFilter;
	audioConfig.useSeparateAudioFilter = deviceHasSeparateAudioFilter;

	audioConfig.callback = std::bind(&DShowInput::OnAudioData, this,
					 placeholders::_1, placeholders::_2,
					 placeholders::_3, placeholders::_4,
					 placeholders::_5);

	audioConfig.mode =
		(AudioMode)obs_data_get_int(settings, AUDIO_OUTPUT_MODE);

	bool success = device.SetAudioConfig(&audioConfig);
	if (!success)
		return false;

	BPtr<char> name_utf8;
	os_wcs_to_utf8_ptr(audioConfig.name.c_str(), audioConfig.name.size(),
			   &name_utf8);

	blog(LOG_INFO, "\tusing video device audio: %s",
	     audioConfig.useVideoDevice ? "yes" : "no");

	if (!audioConfig.useVideoDevice) {
		if (audioConfig.useSeparateAudioFilter)
			blog(LOG_INFO, "\tseparate audio filter");
		else
			blog(LOG_INFO, "\taudio device: %s",
			     (const char *)name_utf8);
	}

	const char *mode = "";

	switch (audioConfig.mode) {
	case AudioMode::Capture:
		mode = "Capture";
		break;
	case AudioMode::DirectSound:
		mode = "DirectSound";
		break;
	case AudioMode::WaveOut:
		mode = "WaveOut";
		break;
	}

	blog(LOG_INFO,
	     "\tsample rate: %d\n"
	     "\tchannels: %d\n"
	     "\taudio type: %s",
	     audioConfig.sampleRate, audioConfig.channels, mode);
	return true;
}

void DShowInput::SetActive(bool active_)
{
	obs_data_t *settings = obs_source_get_settings(source);
	QueueAction(active_ ? Action::Activate : Action::Deactivate);
	obs_data_set_bool(settings, "active", active_);
	active = active_;
	obs_data_release(settings);
}

inline enum video_colorspace
DShowInput::GetColorSpace(obs_data_t *settings) const
{
	const char *space = obs_data_get_string(settings, COLOR_SPACE);

	if (astrcmpi(space, "709") == 0)
		return VIDEO_CS_709;
	else if (astrcmpi(space, "601") == 0)
		return VIDEO_CS_601;
	else
		return (videoConfig.format == VideoFormat::HDYC) ? VIDEO_CS_709
								 : VIDEO_CS_601;
}

inline enum video_range_type
DShowInput::GetColorRange(obs_data_t *settings) const
{
	const char *range = obs_data_get_string(settings, COLOR_RANGE);

	if (astrcmpi(range, "full") == 0)
		return VIDEO_RANGE_FULL;
	if (astrcmpi(range, "partial") == 0)
		return VIDEO_RANGE_PARTIAL;
	return VIDEO_RANGE_DEFAULT;
}

//PRISM/WangShaohui/20200210/#281/for source unavailable
inline enum obs_source_error DShowInput::Activate(obs_data_t *settings,
						  bool saveLog)
{
	//PRISM/Wang.Chuanjing/20200511//for camera image state
	obs_source_set_image_status(source, false);

	if (!device.ResetGraph())
		return OBS_SOURCE_ERROR_UNKNOWN;

	if (!UpdateVideoConfig(settings, saveLog)) {
		//PRISM/WangShaohui/20201013/NoIssue/adjust log number
		if (saveLog) {
			blog(LOG_WARNING, "%s: Video configuration failed",
			     obs_source_get_name(source));
		}
		return OBS_SOURCE_ERROR_UNKNOWN;
	}

	if (!UpdateAudioConfig(settings)) {
		//PRISM/WangShaohui/20201013/NoIssue/adjust log number
		if (saveLog) {
			blog(LOG_WARNING,
			     "%s: Audio configuration failed, ignoring "
			     "audio",
			     obs_source_get_name(source));
		}
	}

	if (!device.ConnectFilters())
		return OBS_SOURCE_ERROR_UNKNOWN;

	//PRISM/WangShaohui/20200210/#281/for source unavailable
	Result res = device.Start();
	if (res != Result::Success) {
		if (res == Result::InUse)
			return OBS_SOURCE_ERROR_BE_USING;
		else
			return OBS_SOURCE_ERROR_UNKNOWN;
	}

	enum video_colorspace cs = GetColorSpace(settings);
	range = GetColorRange(settings);
	frame.range = range;

	bool success = video_format_get_parameters(cs, range,
						   frame.color_matrix,
						   frame.color_range_min,
						   frame.color_range_max);
	if (!success) {
		//PRISM/WangShaohui/20201013/NoIssue/adjust log number
		if (saveLog) {
			blog(LOG_ERROR,
			     "Failed to get video format parameters for "
			     "video format %u",
			     cs);
		}
	}

	//PRISM/WangShaohui/20200210/#281/for source unavailable
	return OBS_SOURCE_ERROR_OK;
}

inline void DShowInput::Deactivate()
{
	device.ResetGraph();
	obs_source_output_video2(source, nullptr);

	//PRISM/WangChuanjing/20200429/#2516/for beauty
	obs_source_set_image_status(source, false);
	cam_effect.SetCaptureState(false);
}

/* ------------------------------------------------------------------------- */

static const char *GetDShowInputName(void *)
{
	return TEXT_INPUT_NAME;
}

static void *CreateDShowInput(obs_data_t *settings, obs_source_t *source)
{
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);
	//PRISM/WangChuanjing/20200429/#2517/for beauty
	obs_source_set_image_status(source, false);

	DShowInput *dshow = nullptr;

	try {
		dshow = new DShowInput(source, settings);
	} catch (const char *error) {
		blog(LOG_ERROR, "Could not create device '%s': %s",
		     obs_source_get_name(source), error);
	}

	return dshow;
}

static void DestroyDShowInput(void *data)
{
	delete reinterpret_cast<DShowInput *>(data);
}

static void UpdateDShowInput(void *data, obs_data_t *settings)
{
	DShowInput *input = reinterpret_cast<DShowInput *>(data);
	if (input->active)
		input->QueueActivate(settings);
}

static void GetDShowDefaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, FRAME_INTERVAL, FPS_MATCHING);
	obs_data_set_default_int(settings, RES_TYPE, ResType_Preferred);
	obs_data_set_default_int(settings, VIDEO_FORMAT, (int)VideoFormat::Any);
	obs_data_set_default_bool(settings, "active", true);
	obs_data_set_default_string(settings, COLOR_SPACE, "default");
	obs_data_set_default_string(settings, COLOR_RANGE, "default");
	obs_data_set_default_int(settings, AUDIO_OUTPUT_MODE,
				 (int)AudioMode::Capture);
}

struct Resolution {
	int cx, cy;

	inline Resolution(int cx, int cy) : cx(cx), cy(cy) {}
};

static void InsertResolution(vector<Resolution> &resolutions, int cx, int cy)
{
	int bestCY = 0;
	size_t idx = 0;

	for (; idx < resolutions.size(); idx++) {
		const Resolution &res = resolutions[idx];
		if (res.cx > cx)
			break;

		if (res.cx == cx) {
			if (res.cy == cy)
				return;

			if (!bestCY)
				bestCY = res.cy;
			else if (res.cy > bestCY)
				break;
		}
	}

	resolutions.insert(resolutions.begin() + idx, Resolution(cx, cy));
}

static inline void AddCap(vector<Resolution> &resolutions, const VideoInfo &cap)
{
	InsertResolution(resolutions, cap.minCX, cap.minCY);
	InsertResolution(resolutions, cap.maxCX, cap.maxCY);
}

#define MAKE_DSHOW_FPS(fps) (10000000LL / (fps))
#define MAKE_DSHOW_FRACTIONAL_FPS(den, num) ((num)*10000000LL / (den))

static long long GetOBSFPS()
{
	obs_video_info ovi;
	if (!obs_get_video_info(&ovi))
		return 0;

	return MAKE_DSHOW_FRACTIONAL_FPS(ovi.fps_num, ovi.fps_den);
}

struct FPSFormat {
	const char *text;
	long long interval;
};

static const FPSFormat validFPSFormats[] = {
	{"60", MAKE_DSHOW_FPS(60)},
	{"59.94 NTSC", MAKE_DSHOW_FRACTIONAL_FPS(60000, 1001)},
	{"50", MAKE_DSHOW_FPS(50)},
	{"48 film", MAKE_DSHOW_FRACTIONAL_FPS(48000, 1001)},
	{"40", MAKE_DSHOW_FPS(40)},
	{"30", MAKE_DSHOW_FPS(30)},
	{"29.97 NTSC", MAKE_DSHOW_FRACTIONAL_FPS(30000, 1001)},
	{"25", MAKE_DSHOW_FPS(25)},
	{"24 film", MAKE_DSHOW_FRACTIONAL_FPS(24000, 1001)},
	{"20", MAKE_DSHOW_FPS(20)},
	{"15", MAKE_DSHOW_FPS(15)},
	{"10", MAKE_DSHOW_FPS(10)},
	{"5", MAKE_DSHOW_FPS(5)},
	{"4", MAKE_DSHOW_FPS(4)},
	{"3", MAKE_DSHOW_FPS(3)},
	{"2", MAKE_DSHOW_FPS(2)},
	{"1", MAKE_DSHOW_FPS(1)},
};

static bool DeviceIntervalChanged(obs_properties_t *props, obs_property_t *p,
				  obs_data_t *settings);

static bool TryResolution(VideoDevice &dev, string res)
{
	int cx, cy;
	if (!ConvertRes(cx, cy, res.c_str()))
		return false;

	return ResolutionAvailable(dev, cx, cy);
}

static bool SetResolution(obs_properties_t *props, obs_data_t *settings,
			  string res, bool autoselect = false)
{
	if (autoselect)
		obs_data_set_autoselect_string(settings, RESOLUTION,
					       res.c_str());
	else
		obs_data_unset_autoselect_value(settings, RESOLUTION);

	DeviceIntervalChanged(props, obs_properties_get(props, FRAME_INTERVAL),
			      settings);

	if (!autoselect)
		obs_data_set_string(settings, LAST_RESOLUTION, res.c_str());
	return true;
}

static bool DeviceResolutionChanged(obs_properties_t *props, obs_property_t *p,
				    obs_data_t *settings)
{
	UNUSED_PARAMETER(p);

	PropertiesData *data =
		(PropertiesData *)obs_properties_get_param(props);
	const char *id;
	VideoDevice device;

	id = obs_data_get_string(settings, VIDEO_DEVICE_ID);
	string res = obs_data_get_string(settings, RESOLUTION);
	string last_res = obs_data_get_string(settings, LAST_RESOLUTION);

	if (!data->GetDevice(device, id))
		return false;

	if (TryResolution(device, res))
		return SetResolution(props, settings, res);

	if (TryResolution(device, last_res))
		return SetResolution(props, settings, last_res, true);

	return false;
}

struct VideoFormatName {
	VideoFormat format;
	const char *name;
};

static const VideoFormatName videoFormatNames[] = {
	/* autoselect format*/
	{VideoFormat::Any, "VideoFormat.Any"},

	/* raw formats */
	{VideoFormat::ARGB, "ARGB"},
	{VideoFormat::XRGB, "XRGB"},

	/* planar YUV formats */
	{VideoFormat::I420, "I420"},
	{VideoFormat::NV12, "NV12"},
	{VideoFormat::YV12, "YV12"},
	{VideoFormat::Y800, "Y800"},

	/* packed YUV formats */
	{VideoFormat::YVYU, "YVYU"},
	{VideoFormat::YUY2, "YUY2"},
	{VideoFormat::UYVY, "UYVY"},
	{VideoFormat::HDYC, "HDYC"},

	/* encoded formats */
	{VideoFormat::MJPEG, "MJPEG"},
	{VideoFormat::H264, "H264"}};

static bool ResTypeChanged(obs_properties_t *props, obs_property_t *p,
			   obs_data_t *settings);

static size_t AddDevice(obs_property_t *device_list, const string &id)
{
	DStr name, path;
	if (!DecodeDeviceDStr(name, path, id.c_str()))
		return numeric_limits<size_t>::max();

	return obs_property_list_add_string(device_list, name, id.c_str());
}

static bool UpdateDeviceList(obs_property_t *list, const string &id)
{
	size_t size = obs_property_list_item_count(list);
	bool found = false;
	bool disabled_unknown_found = false;

	for (size_t i = 0; i < size; i++) {
		if (obs_property_list_item_string(list, i) == id) {
			found = true;
			continue;
		}
		if (obs_property_list_item_disabled(list, i))
			disabled_unknown_found = true;
	}

	if (!found && !disabled_unknown_found) {
		size_t idx = AddDevice(list, id);
		obs_property_list_item_disable(list, idx, true);
		return true;
	}

	if (found && !disabled_unknown_found)
		return false;

	for (size_t i = 0; i < size;) {
		if (obs_property_list_item_disabled(list, i)) {
			obs_property_list_item_remove(list, i);
			continue;
		}
		i += 1;
	}

	return true;
}

static bool DeviceSelectionChanged(obs_properties_t *props, obs_property_t *p,
				   obs_data_t *settings)
{
	PropertiesData *data =
		(PropertiesData *)obs_properties_get_param(props);
	VideoDevice device;

	string id = obs_data_get_string(settings, VIDEO_DEVICE_ID);
	string old_id = obs_data_get_string(settings, LAST_VIDEO_DEV_ID);

	bool device_list_updated = UpdateDeviceList(p, id);

	if (!data->GetDevice(device, id.c_str()))
		return !device_list_updated;

	vector<Resolution> resolutions;
	for (const VideoInfo &cap : device.caps)
		AddCap(resolutions, cap);

	p = obs_properties_get(props, RESOLUTION);
	obs_property_list_clear(p);

	for (size_t idx = resolutions.size(); idx > 0; idx--) {
		const Resolution &res = resolutions[idx - 1];

		string strRes;
		strRes += to_string(res.cx);
		strRes += "x";
		strRes += to_string(res.cy);

		obs_property_list_add_string(p, strRes.c_str(), strRes.c_str());
	}

	/* only refresh properties if device legitimately changed */
	if (!id.size() || !old_id.size() || id != old_id) {
		p = obs_properties_get(props, RES_TYPE);
		ResTypeChanged(props, p, settings);
		obs_data_set_string(settings, LAST_VIDEO_DEV_ID, id.c_str());
	}

	return true;
}

static bool VideoConfigClicked(obs_properties_t *props, obs_property_t *p,
			       void *data)
{
	DShowInput *input = reinterpret_cast<DShowInput *>(data);
	input->QueueAction(Action::ConfigVideo);

	UNUSED_PARAMETER(props);
	UNUSED_PARAMETER(p);
	return false;
}

/*static bool AudioConfigClicked(obs_properties_t *props, obs_property_t *p,
		void *data)
{
	DShowInput *input = reinterpret_cast<DShowInput*>(data);
	input->QueueAction(Action::ConfigAudio);

	UNUSED_PARAMETER(props);
	UNUSED_PARAMETER(p);
	return false;
}*/

static bool CrossbarConfigClicked(obs_properties_t *props, obs_property_t *p,
				  void *data)
{
	DShowInput *input = reinterpret_cast<DShowInput *>(data);
	input->QueueAction(Action::ConfigCrossbar1);

	UNUSED_PARAMETER(props);
	UNUSED_PARAMETER(p);
	return false;
}

/*static bool Crossbar2ConfigClicked(obs_properties_t *props, obs_property_t *p,
		void *data)
{
	DShowInput *input = reinterpret_cast<DShowInput*>(data);
	input->QueueAction(Action::ConfigCrossbar2);

	UNUSED_PARAMETER(props);
	UNUSED_PARAMETER(p);
	return false;
}*/

static bool AddDevice(obs_property_t *device_list, const VideoDevice &device)
{
	DStr name, path, device_id;

	dstr_from_wcs(name, device.name.c_str());
	dstr_from_wcs(path, device.path.c_str());

	encode_dstr(path);

	dstr_copy_dstr(device_id, name);
	encode_dstr(device_id);
	dstr_cat(device_id, ":");
	dstr_cat_dstr(device_id, path);

	obs_property_list_add_string(device_list, name, device_id);

	return true;
}

static bool AddAudioDevice(obs_property_t *device_list,
			   const AudioDevice &device)
{
	DStr name, path, device_id;

	dstr_from_wcs(name, device.name.c_str());
	dstr_from_wcs(path, device.path.c_str());

	encode_dstr(path);

	dstr_copy_dstr(device_id, name);
	encode_dstr(device_id);
	dstr_cat(device_id, ":");
	dstr_cat_dstr(device_id, path);

	obs_property_list_add_string(device_list, name, device_id);

	return true;
}

static void PropertiesDataDestroy(void *data)
{
	delete reinterpret_cast<PropertiesData *>(data);
}

static bool ResTypeChanged(obs_properties_t *props, obs_property_t *p,
			   obs_data_t *settings)
{
	int val = (int)obs_data_get_int(settings, RES_TYPE);
	bool enabled = (val != ResType_Preferred);

	p = obs_properties_get(props, RESOLUTION);
	obs_property_set_enabled(p, enabled);

	p = obs_properties_get(props, FRAME_INTERVAL);
	obs_property_set_enabled(p, enabled);

	p = obs_properties_get(props, VIDEO_FORMAT);
	obs_property_set_enabled(p, enabled);

	if (val == ResType_Custom) {
		p = obs_properties_get(props, RESOLUTION);
		DeviceResolutionChanged(props, p, settings);
	} else {
		obs_data_unset_autoselect_value(settings, FRAME_INTERVAL);
	}

	return true;
}

static DStr GetFPSName(long long interval)
{
	DStr name;

	if (interval == FPS_MATCHING) {
		dstr_cat(name, TEXT_FPS_MATCHING);
		return name;
	}

	if (interval == FPS_HIGHEST) {
		dstr_cat(name, TEXT_FPS_HIGHEST);
		return name;
	}

	for (const FPSFormat &format : validFPSFormats) {
		if (format.interval != interval)
			continue;

		dstr_cat(name, format.text);
		return name;
	}

	dstr_cat(name, to_string(10000000. / interval).c_str());
	return name;
}

static void UpdateFPS(VideoDevice &device, VideoFormat format,
		      long long interval, int cx, int cy,
		      obs_properties_t *props)
{
	obs_property_t *list = obs_properties_get(props, FRAME_INTERVAL);

	obs_property_list_clear(list);

	obs_property_list_add_int(list, TEXT_FPS_MATCHING, FPS_MATCHING);
	obs_property_list_add_int(list, TEXT_FPS_HIGHEST, FPS_HIGHEST);

	bool interval_added = interval == FPS_HIGHEST ||
			      interval == FPS_MATCHING;
	for (const FPSFormat &fps_format : validFPSFormats) {
		bool video_format_match = false;
		long long format_interval = fps_format.interval;

		bool available = CapsMatch(
			device, ResolutionMatcher(cx, cy),
			VideoFormatMatcher(format, video_format_match),
			FrameRateMatcher(format_interval));

		if (!available && interval != fps_format.interval)
			continue;

		if (interval == fps_format.interval)
			interval_added = true;

		size_t idx = obs_property_list_add_int(list, fps_format.text,
						       fps_format.interval);
		obs_property_list_item_disable(list, idx, !available);
	}

	if (interval_added)
		return;

	size_t idx =
		obs_property_list_add_int(list, GetFPSName(interval), interval);
	obs_property_list_item_disable(list, idx, true);
}

static DStr GetVideoFormatName(VideoFormat format)
{
	DStr name;
	for (const VideoFormatName &format_ : videoFormatNames) {
		if (format_.format == format) {
			dstr_cat(name, obs_module_text(format_.name));
			return name;
		}
	}

	dstr_cat(name, TEXT_FORMAT_UNKNOWN);
	dstr_replace(name, "%1", std::to_string((long long)format).c_str());
	return name;
}

static void UpdateVideoFormats(VideoDevice &device, VideoFormat format_, int cx,
			       int cy, long long interval,
			       obs_properties_t *props)
{
	set<VideoFormat> formats = {VideoFormat::Any};
	auto format_gatherer =
		[&formats](const VideoInfo &info) mutable -> bool {
		formats.insert(info.format);
		return false;
	};

	CapsMatch(device, ResolutionMatcher(cx, cy), FrameRateMatcher(interval),
		  format_gatherer);

	obs_property_t *list = obs_properties_get(props, VIDEO_FORMAT);
	obs_property_list_clear(list);

	bool format_added = false;
	for (const VideoFormatName &format : videoFormatNames) {
		bool available = formats.find(format.format) != end(formats);

		if (!available && format.format != format_)
			continue;

		if (format.format == format_)
			format_added = true;

		size_t idx = obs_property_list_add_int(
			list, obs_module_text(format.name),
			(long long)format.format);
		obs_property_list_item_disable(list, idx, !available);
	}

	if (format_added)
		return;

	size_t idx = obs_property_list_add_int(
		list, GetVideoFormatName(format_), (long long)format_);
	obs_property_list_item_disable(list, idx, true);
}

static bool UpdateFPS(long long interval, obs_property_t *list)
{
	size_t size = obs_property_list_item_count(list);
	DStr name;

	for (size_t i = 0; i < size; i++) {
		if (obs_property_list_item_int(list, i) != interval)
			continue;

		obs_property_list_item_disable(list, i, true);
		if (size == 1)
			return false;

		dstr_cat(name, obs_property_list_item_name(list, i));
		break;
	}

	obs_property_list_clear(list);

	if (!name->len)
		name = GetFPSName(interval);

	obs_property_list_add_int(list, name, interval);
	obs_property_list_item_disable(list, 0, true);

	return true;
}

static bool DeviceIntervalChanged(obs_properties_t *props, obs_property_t *p,
				  obs_data_t *settings)
{
	long long val = obs_data_get_int(settings, FRAME_INTERVAL);

	PropertiesData *data =
		(PropertiesData *)obs_properties_get_param(props);
	const char *id = obs_data_get_string(settings, VIDEO_DEVICE_ID);
	VideoDevice device;

	if (!data->GetDevice(device, id))
		return UpdateFPS(val, p);

	int cx = 0, cy = 0;
	if (!DetermineResolution(cx, cy, settings, device)) {
		UpdateVideoFormats(device, VideoFormat::Any, 0, 0, 0, props);
		UpdateFPS(device, VideoFormat::Any, 0, 0, 0, props);
		return true;
	}

	int resType = (int)obs_data_get_int(settings, RES_TYPE);
	if (resType != ResType_Custom)
		return true;

	if (val == FPS_MATCHING)
		val = GetOBSFPS();

	VideoFormat format =
		(VideoFormat)obs_data_get_int(settings, VIDEO_FORMAT);

	bool video_format_matches = false;
	long long best_interval = numeric_limits<long long>::max();
	bool frameRateSupported =
		CapsMatch(device, ResolutionMatcher(cx, cy),
			  VideoFormatMatcher(format, video_format_matches),
			  ClosestFrameRateSelector(val, best_interval),
			  FrameRateMatcher(val));

	if (video_format_matches && !frameRateSupported &&
	    best_interval != val) {
		long long listed_val = 0;
		for (const FPSFormat &format : validFPSFormats) {
			long long diff = llabs(format.interval - best_interval);
			if (diff < DEVICE_INTERVAL_DIFF_LIMIT) {
				listed_val = format.interval;
				break;
			}
		}

		if (listed_val != val) {
			obs_data_set_autoselect_int(settings, FRAME_INTERVAL,
						    listed_val);
			val = listed_val;
		}

	} else {
		obs_data_unset_autoselect_value(settings, FRAME_INTERVAL);
	}

	UpdateVideoFormats(device, format, cx, cy, val, props);
	UpdateFPS(device, format, val, cx, cy, props);

	UNUSED_PARAMETER(p);
	return true;
}

static bool UpdateVideoFormats(VideoFormat format, obs_property_t *list)
{
	size_t size = obs_property_list_item_count(list);
	DStr name;

	for (size_t i = 0; i < size; i++) {
		if ((VideoFormat)obs_property_list_item_int(list, i) != format)
			continue;

		if (size == 1)
			return false;

		dstr_cat(name, obs_property_list_item_name(list, i));
		break;
	}

	obs_property_list_clear(list);

	if (!name->len)
		name = GetVideoFormatName(format);

	obs_property_list_add_int(list, name, (long long)format);
	obs_property_list_item_disable(list, 0, true);

	return true;
}

static bool VideoFormatChanged(obs_properties_t *props, obs_property_t *p,
			       obs_data_t *settings)
{
	PropertiesData *data =
		(PropertiesData *)obs_properties_get_param(props);
	const char *id = obs_data_get_string(settings, VIDEO_DEVICE_ID);
	VideoDevice device;

	VideoFormat curFormat =
		(VideoFormat)obs_data_get_int(settings, VIDEO_FORMAT);

	if (!data->GetDevice(device, id))
		return UpdateVideoFormats(curFormat, p);

	int cx, cy;
	if (!DetermineResolution(cx, cy, settings, device)) {
		UpdateVideoFormats(device, VideoFormat::Any, cx, cy, 0, props);
		UpdateFPS(device, VideoFormat::Any, 0, 0, 0, props);
		return true;
	}

	long long interval = obs_data_get_int(settings, FRAME_INTERVAL);

	UpdateVideoFormats(device, curFormat, cx, cy, interval, props);
	UpdateFPS(device, curFormat, interval, cx, cy, props);
	return true;
}

static bool CustomAudioClicked(obs_properties_t *props, obs_property_t *p,
			       obs_data_t *settings)
{
	bool useCustomAudio = obs_data_get_bool(settings, USE_CUSTOM_AUDIO);
	p = obs_properties_get(props, AUDIO_DEVICE_ID);
	obs_property_set_visible(p, useCustomAudio);
	return true;
}

static bool ActivateClicked(obs_properties_t *, obs_property_t *p, void *data)
{
	DShowInput *input = reinterpret_cast<DShowInput *>(data);
	input->previous_operation = SwitchActive;

	if (input->active) {
		input->SetActive(false);
		//PRISM/Liuying/20200617/No issue/for the same row of buttons
		obs_property_button_group_set_item_text(p, 0, TEXT_ACTIVATE);
	} else {
		input->SetActive(true);
		//PRISM/Liuying/20200617/No issue/for the same row of buttons
		obs_property_button_group_set_item_text(p, 0, TEXT_DEACTIVATE);
	}

	return true;
}

static obs_properties_t *GetDShowProperties(void *obj)
{
	DShowInput *input = reinterpret_cast<DShowInput *>(obj);
	obs_properties_t *ppts = obs_properties_create();
	PropertiesData *data = new PropertiesData;

	data->input = input;

	obs_properties_set_param(ppts, data, PropertiesDataDestroy);

	obs_property_t *p = obs_properties_add_list(ppts, VIDEO_DEVICE_ID,
						    TEXT_DEVICE,
						    OBS_COMBO_TYPE_LIST,
						    OBS_COMBO_FORMAT_STRING);

	obs_property_set_modified_callback(p, DeviceSelectionChanged);

	Device::EnumVideoDevices(data->devices);
	for (const VideoDevice &device : data->devices)
		AddDevice(p, device);

	const char *activateText = TEXT_ACTIVATE;
	if (input) {
		if (input->active)
			activateText = TEXT_DEACTIVATE;
	}

	//PRISM/Liuying/20200617/No issue/for the same row of buttons
	p = obs_properties_add_button_group(ppts, BUTTON_GROUP_TYPE,
					    BUTTON_GROUP_TYPE);

	obs_property_button_group_add_item(p, "activate", activateText,
					   ActivateClicked);
	obs_property_button_group_add_item(p, "video_config", TEXT_CONFIG_VIDEO,
					   VideoConfigClicked);
	obs_property_button_group_add_item(p, "xbar_config", TEXT_CONFIG_XBAR,
					   CrossbarConfigClicked);

	obs_properties_add_bool(ppts, DEACTIVATE_WNS, TEXT_DWNS);

	/* ------------------------------------- */
	/* video settings */

	p = obs_properties_add_list(ppts, RES_TYPE, TEXT_RES_FPS_TYPE,
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_set_modified_callback(p, ResTypeChanged);

	obs_property_list_add_int(p, TEXT_PREFERRED_RES, ResType_Preferred);
	obs_property_list_add_int(p, TEXT_CUSTOM_RES, ResType_Custom);

	//PRISM/WangShaohui/20200324/#2037/for disable edit of resolution list
	p = obs_properties_add_list(ppts, RESOLUTION, TEXT_RESOLUTION,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);

	obs_property_set_modified_callback(p, DeviceResolutionChanged);

	p = obs_properties_add_list(ppts, FRAME_INTERVAL, "FPS",
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_set_modified_callback(p, DeviceIntervalChanged);

	p = obs_properties_add_list(ppts, VIDEO_FORMAT, TEXT_VIDEO_FORMAT,
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_set_modified_callback(p, VideoFormatChanged);

	p = obs_properties_add_list(ppts, COLOR_SPACE, TEXT_COLOR_SPACE,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, TEXT_COLOR_DEFAULT, "default");
	obs_property_list_add_string(p, "709", "709");
	obs_property_list_add_string(p, "601", "601");

	p = obs_properties_add_list(ppts, COLOR_RANGE, TEXT_COLOR_RANGE,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, TEXT_RANGE_DEFAULT, "default");
	obs_property_list_add_string(p, TEXT_RANGE_PARTIAL, "partial");
	obs_property_list_add_string(p, TEXT_RANGE_FULL, "full");

	p = obs_properties_add_list(ppts, BUFFERING_VAL, TEXT_BUFFERING,
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, TEXT_BUFFERING_AUTO,
				  (int64_t)BufferingType::Auto);
	obs_property_list_add_int(p, TEXT_BUFFERING_ON,
				  (int64_t)BufferingType::On);
	obs_property_list_add_int(p, TEXT_BUFFERING_OFF,
				  (int64_t)BufferingType::Off);

	obs_property_set_long_description(p,
					  obs_module_text("Buffering.ToolTip"));

	obs_properties_add_bool(ppts, FLIP_IMAGE, TEXT_FLIP_IMAGE);

	/* ------------------------------------- */
	/* audio settings */

	Device::EnumAudioDevices(data->audioDevices);

	p = obs_properties_add_list(ppts, AUDIO_OUTPUT_MODE, TEXT_AUDIO_MODE,
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, TEXT_MODE_CAPTURE,
				  (int64_t)AudioMode::Capture);
	obs_property_list_add_int(p, TEXT_MODE_DSOUND,
				  (int64_t)AudioMode::DirectSound);
	obs_property_list_add_int(p, TEXT_MODE_WAVEOUT,
				  (int64_t)AudioMode::WaveOut);

	if (!data->audioDevices.size())
		return ppts;

	p = obs_properties_add_bool(ppts, USE_CUSTOM_AUDIO, TEXT_CUSTOM_AUDIO);

	obs_property_set_modified_callback(p, CustomAudioClicked);

	p = obs_properties_add_list(ppts, AUDIO_DEVICE_ID, TEXT_AUDIO_DEVICE,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);

	for (const AudioDevice &device : data->audioDevices)
		AddAudioDevice(p, device);

	return ppts;
}

void DShowModuleLogCallback(LogType type, const wchar_t *msg, void *param)
{
	int obs_type = LOG_DEBUG;

	switch (type) {
	case LogType::Error:
		obs_type = LOG_ERROR;
		break;
	case LogType::Warning:
		obs_type = LOG_WARNING;
		break;
	case LogType::Info:
		obs_type = LOG_INFO;
		break;
	case LogType::Debug:
		obs_type = LOG_DEBUG;
		break;
	}

	DStr dmsg;

	dstr_from_wcs(dmsg, msg);
	blog(obs_type, "DShow: %s", dmsg->array);

	UNUSED_PARAMETER(param);
}

static void HideDShowInput(void *data)
{
	DShowInput *input = reinterpret_cast<DShowInput *>(data);

	if (input->deactivateWhenNotShowing && input->active)
		input->QueueAction(Action::Deactivate);
}

static void ShowDShowInput(void *data)
{
	DShowInput *input = reinterpret_cast<DShowInput *>(data);

	if (input->deactivateWhenNotShowing && input->active)
		input->QueueAction(Action::Activate);
}

//PRISM/Wang.Chuanjing/20200409/#2330/for beauty
static bool SetPrivateData(void *data, obs_data_t *private_data)
{
	if (!private_data)
		return false;

	if (!data) {
		return PLSCamEffect::SetGlobalParam(private_data);
	}

	const char *method = obs_data_get_string(private_data, "method");
	if (!method) {
		return false;
	}

	DShowInput *input = reinterpret_cast<DShowInput *>(data);

	if (0 == strcmp(method, "owner")) {
		input->property_hwnd =
			(HWND)obs_data_get_int(private_data, "hwnd");
		return true;
	} else {
		bool effect_onoff_changed = false;
		bool ret = input->cam_effect.SetEffectParam(
			method, private_data, effect_onoff_changed);

		if (effect_onoff_changed) {
			input->previous_operation = SwitchEffect;
		}
		return ret;
	}
}

//PRISM/LiuHaibin/20200609/#3174/camera effect
static void OnSharedHandle(void *data, uint32_t shared_handle,
			   struct gs_luid *luid, bool flip, uint64_t sys_time)
{
	if (data) {
		shared_handle_header hdr = {};
		hdr.flip = flip;
		hdr.push_time = GetTickCount();

		shared_handle_sample body = {};
		body.handle = shared_handle;
		memcpy(&body.luid, luid, sizeof(shared_handle_adapter_luid));

		DShowInput *input = reinterpret_cast<DShowInput *>(data);
		input->cam_effect.SetInputVideo(hdr, body);
	}
}

//PRISM/LiuHaibin/20200609/#3174/camera effect
static bool IsCamEffectOn(void *data)
{
	if (data) {
		DShowInput *input = reinterpret_cast<DShowInput *>(data);
		return input->cam_effect.UseVideoEffect();
	}
	return false;
}

//PRISM/Wangshaohui/20200609/#3174/camera effect
static void CamAsyncTick(void *data)
{
	if (data) {
		DShowInput *input = reinterpret_cast<DShowInput *>(data);
		if (input->cam_effect.UseVideoEffect()) {
			input->cam_effect.EffectTick();
		}

		std::vector<PLSExceptionInfo> exceptions;
		size_t count = input->cam_effect.PopExceptions(exceptions);
		for (size_t i = 0; i < count; i++) {
			PLSExceptionInfo &temp = exceptions[i];
			obs_source_send_notify(input->source, temp.type,
					       temp.sub_code);
		}
	}
}

//PRISM/LiuHaibin/20200618/#3174/camera effect
static gs_texture_t *RetrieveTexture(void *data)
{
	// if we return NULL, libobs will render original texture directly.
	if (data) {
		DShowInput *input = reinterpret_cast<DShowInput *>(data);
		return input->cam_effect.GetOutputVideo();
	}
	return NULL;
}

//PRISM/Wangshaohui/20200701/#3174/camera effect
static bool IsEffectSwitching(void *data)
{
	if (data) {
		DShowInput *input = reinterpret_cast<DShowInput *>(data);
		return (SwitchEffect == input->previous_operation);
	}
	return false;
}

//PRISM/Wangshaohui/20200701/#3174/camera effect
static unsigned SourceStateFlags(void *data)
{
	unsigned flags = 0;

	if (data) {
		DShowInput *input = reinterpret_cast<DShowInput *>(data);
		if (input->active && input->cam_effect.IsCaptureNormal()) {
			flags |= OBS_SOURCE_STATE_ACTIVE;
		}
	}

	return flags;
}

void RegisterDShowSource()
{
	SetLogCallback(DShowModuleLogCallback, nullptr);

	obs_source_info info = {};
	info.id = "dshow_input";
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_AUDIO |
			    OBS_SOURCE_ASYNC | OBS_SOURCE_DO_NOT_DUPLICATE;
	info.show = ShowDShowInput;
	info.hide = HideDShowInput;
	info.get_name = GetDShowInputName;
	info.create = CreateDShowInput;
	info.destroy = DestroyDShowInput;
	info.update = UpdateDShowInput;
	info.get_defaults = GetDShowDefaults;
	info.get_properties = GetDShowProperties;
	info.set_private_data = SetPrivateData;
	info.push_shared_handle = OnSharedHandle;
	info.cam_effect_on = IsCamEffectOn;
	info.on_async_tick = CamAsyncTick;
	info.retrieve_texture = RetrieveTexture;
	info.cam_effect_switching = IsEffectSwitching;
	info.source_state_flags = SourceStateFlags;
	info.icon_type = OBS_ICON_TYPE_CAMERA;
	obs_register_source(&info);
}
