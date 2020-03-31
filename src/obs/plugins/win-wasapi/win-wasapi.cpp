#include "enum-wasapi.hpp"

#include <obs-module.h>
#include <obs.h>
#include <util/platform.h>
#include <util/windows/HRError.hpp>
#include <util/windows/ComPtr.hpp>
#include <util/windows/WinHandle.hpp>
#include <util/windows/CoTaskMemPtr.hpp>
#include <util/threading.h>
//PRISM/LiuHaibin/20200803/#None/https://github.com/obsproject/obs-studio/pull/2657
#include <util/util_uint64.h>

using namespace std;

#define OPT_DEVICE_ID "device_id"
#define OPT_USE_DEVICE_TIMING "use_device_timing"

static void GetWASAPIDefaults(obs_data_t *settings);

#define OBS_KSAUDIO_SPEAKER_4POINT1 \
	(KSAUDIO_SPEAKER_SURROUND | SPEAKER_LOW_FREQUENCY)

class WASAPISource {
	ComPtr<IMMDevice> device;
	ComPtr<IAudioClient> client;
	ComPtr<IAudioCaptureClient> capture;
	ComPtr<IAudioRenderClient> render;

	obs_source_t *source;
	string device_id;
	string device_name;
	bool isInputDevice;
	bool useDeviceTiming = false;
	bool isDefaultDevice = false;

	bool reconnecting = false;
	bool previouslyFailed = false;
	WinHandle reconnectThread;

	bool active = false;
	WinHandle captureThread;

	WinHandle stopSignal;
	WinHandle receiveSignal;

	speaker_layout speakers;
	audio_format format;
	uint32_t sampleRate;

	static DWORD WINAPI ReconnectThread(LPVOID param);
	static DWORD WINAPI CaptureThread(LPVOID param);

	bool ProcessCaptureData();

	//PRISM/WangShaohui/20200210/#281/for source unavailable
	bool IsAudioExist();

	inline void Start();
	inline void Stop();
	void Reconnect();

	bool InitDevice(IMMDeviceEnumerator *enumerator);
	void InitName();
	void InitClient();
	void InitRender();
	void InitFormat(WAVEFORMATEX *wfex);
	void InitCapture();
	void Initialize();

	//PRISM/WangShaohui/20200219/#468/for checking source invalid
	void SetInvalidState();

	bool TryInitialize();

	void UpdateSettings(obs_data_t *settings);

public:
	WASAPISource(obs_data_t *settings, obs_source_t *source_, bool input);
	inline ~WASAPISource();

	void Update(obs_data_t *settings);
};

WASAPISource::WASAPISource(obs_data_t *settings, obs_source_t *source_,
			   bool input)
	: source(source_), isInputDevice(input)
{
	UpdateSettings(settings);

	stopSignal = CreateEvent(nullptr, true, false, nullptr);
	if (!stopSignal.Valid())
		throw "Could not create stop signal";

	receiveSignal = CreateEvent(nullptr, false, false, nullptr);
	if (!receiveSignal.Valid())
		throw "Could not create receive signal";

	Start();
}

//PRISM/WangShaohui/20200210/#281/for source unavailable
bool WASAPISource::IsAudioExist()
{
	vector<AudioDeviceInfo> devices;
	GetWASAPIAudioDevices(devices, isInputDevice);

	size_t count = devices.size();
	for (size_t i = 0; i < count; ++i) {
		if (devices[i].id == device_id)
			return true;
	}

	return false;
}

inline void WASAPISource::Start()
{
	if (!TryInitialize()) {
		blog(LOG_INFO,
		     "[WASAPISource::WASAPISource] "
		     "Device '%s' not found.  Waiting for device",
		     device_id.c_str());
		Reconnect();
	}
}

inline void WASAPISource::Stop()
{
	SetEvent(stopSignal);

	if (active) {
		blog(LOG_INFO, "WASAPI: Device '%s' Terminated",
		     device_name.c_str());
		WaitForSingleObject(captureThread, INFINITE);
	}

	if (reconnecting)
		WaitForSingleObject(reconnectThread, INFINITE);

	ResetEvent(stopSignal);
}

inline WASAPISource::~WASAPISource()
{
	Stop();
}

void WASAPISource::UpdateSettings(obs_data_t *settings)
{
	//PRISM/WangShaohui/20200414/#2224/for lost audio device
	string name;
	DecodeAudioString(obs_data_get_string(settings, OPT_DEVICE_ID), name,
			  device_id);

	useDeviceTiming = obs_data_get_bool(settings, OPT_USE_DEVICE_TIMING);
	isDefaultDevice = _strcmpi(device_id.c_str(), "default") == 0;
}

void WASAPISource::Update(obs_data_t *settings)
{
	//PRISM/WangShaohui/20200414/#2224/for lost audio device
	string name;
	string newDevice;
	DecodeAudioString(obs_data_get_string(settings, OPT_DEVICE_ID), name,
			  newDevice);

	bool restart = newDevice.compare(device_id) != 0;

	if (restart)
		Stop();

	UpdateSettings(settings);

	if (restart)
		Start();
}

bool WASAPISource::InitDevice(IMMDeviceEnumerator *enumerator)
{
	HRESULT res;

	if (isDefaultDevice) {
		res = enumerator->GetDefaultAudioEndpoint(
			isInputDevice ? eCapture : eRender,
			isInputDevice ? eCommunications : eConsole,
			device.Assign());
	} else {
		wchar_t *w_id;
		os_utf8_to_wcs_ptr(device_id.c_str(), device_id.size(), &w_id);

		res = enumerator->GetDevice(w_id, device.Assign());

		bfree(w_id);
	}

	return SUCCEEDED(res);
}

#define BUFFER_TIME_100NS (5 * 10000000)

void WASAPISource::InitClient()
{
	CoTaskMemPtr<WAVEFORMATEX> wfex;
	HRESULT res;
	DWORD flags = AUDCLNT_STREAMFLAGS_EVENTCALLBACK;

	res = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
			       (void **)client.Assign());
	if (FAILED(res))
		throw HRError("Failed to activate client context", res);

	res = client->GetMixFormat(&wfex);
	if (FAILED(res))
		throw HRError("Failed to get mix format", res);

	InitFormat(wfex);

	if (!isInputDevice)
		flags |= AUDCLNT_STREAMFLAGS_LOOPBACK;

	res = client->Initialize(AUDCLNT_SHAREMODE_SHARED, flags,
				 BUFFER_TIME_100NS, 0, wfex, nullptr);
	if (FAILED(res))
		throw HRError("Failed to get initialize audio client", res);
}

void WASAPISource::InitRender()
{
	CoTaskMemPtr<WAVEFORMATEX> wfex;
	HRESULT res;
	LPBYTE buffer;
	UINT32 frames;
	ComPtr<IAudioClient> client;

	res = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
			       (void **)client.Assign());
	if (FAILED(res))
		throw HRError("Failed to activate client context", res);

	res = client->GetMixFormat(&wfex);
	if (FAILED(res))
		throw HRError("Failed to get mix format", res);

	res = client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, BUFFER_TIME_100NS,
				 0, wfex, nullptr);
	if (FAILED(res))
		throw HRError("Failed to get initialize audio client", res);

	/* Silent loopback fix. Prevents audio stream from stopping and */
	/* messing up timestamps and other weird glitches during silence */
	/* by playing a silent sample all over again. */

	res = client->GetBufferSize(&frames);
	if (FAILED(res))
		throw HRError("Failed to get buffer size", res);

	res = client->GetService(__uuidof(IAudioRenderClient),
				 (void **)render.Assign());
	if (FAILED(res))
		throw HRError("Failed to get render client", res);

	res = render->GetBuffer(frames, &buffer);
	if (FAILED(res))
		throw HRError("Failed to get buffer", res);

	memset(buffer, 0, frames * wfex->nBlockAlign);

	render->ReleaseBuffer(frames, 0);
}

static speaker_layout ConvertSpeakerLayout(DWORD layout, WORD channels)
{
	switch (layout) {
	case KSAUDIO_SPEAKER_2POINT1:
		return SPEAKERS_2POINT1;
	case KSAUDIO_SPEAKER_SURROUND:
		return SPEAKERS_4POINT0;
	case OBS_KSAUDIO_SPEAKER_4POINT1:
		return SPEAKERS_4POINT1;
	case KSAUDIO_SPEAKER_5POINT1_SURROUND:
		return SPEAKERS_5POINT1;
	case KSAUDIO_SPEAKER_7POINT1_SURROUND:
		return SPEAKERS_7POINT1;
	}

	return (speaker_layout)channels;
}

void WASAPISource::InitFormat(WAVEFORMATEX *wfex)
{
	DWORD layout = 0;

	if (wfex->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
		WAVEFORMATEXTENSIBLE *ext = (WAVEFORMATEXTENSIBLE *)wfex;
		layout = ext->dwChannelMask;
	}

	/* WASAPI is always float */
	sampleRate = wfex->nSamplesPerSec;
	format = AUDIO_FORMAT_FLOAT;
	speakers = ConvertSpeakerLayout(layout, wfex->nChannels);
}

void WASAPISource::InitCapture()
{
	HRESULT res = client->GetService(__uuidof(IAudioCaptureClient),
					 (void **)capture.Assign());
	if (FAILED(res))
		throw HRError("Failed to create capture context", res);

	res = client->SetEventHandle(receiveSignal);
	if (FAILED(res))
		throw HRError("Failed to set event handle", res);

	captureThread = CreateThread(nullptr, 0, WASAPISource::CaptureThread,
				     this, 0, nullptr);
	if (!captureThread.Valid())
		throw "Failed to create capture thread";

	client->Start();
	active = true;

	blog(LOG_INFO, "WASAPI: Device '%s' initialized", device_name.c_str());
}

void WASAPISource::Initialize()
{
	ComPtr<IMMDeviceEnumerator> enumerator;
	HRESULT res;

	res = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
			       CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
			       (void **)enumerator.Assign());
	if (FAILED(res))
		throw HRError("Failed to create enumerator", res);

	if (!InitDevice(enumerator))
		return;

	device_name = GetDeviceName(device);

	InitClient();
	if (!isInputDevice)
		InitRender();
	InitCapture();
}

//PRISM/WangShaohui/20200219/#468/for checking source invalid
void WASAPISource::SetInvalidState()
{
	if (isDefaultDevice) {
		obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);
	} else {
		if (device_id.empty()) {
			obs_source_set_capture_valid(source, true,
						     OBS_SOURCE_ERROR_OK);
		} else {
			obs_source_set_capture_valid(
				source, false,
				IsAudioExist() ? OBS_SOURCE_ERROR_UNKNOWN
					       : OBS_SOURCE_ERROR_NOT_FOUND);
		}
	}
}

bool WASAPISource::TryInitialize()
{
	try {
		Initialize();

	} catch (HRError &error) {
		if (previouslyFailed)
			return active;

		blog(LOG_WARNING, "[WASAPISource::TryInitialize]:[%s] %s: %lX",
		     device_name.empty() ? device_id.c_str()
					 : device_name.c_str(),
		     error.str, error.hr);

	} catch (const char *error) {
		if (previouslyFailed)
			return active;

		blog(LOG_WARNING, "[WASAPISource::TryInitialize]:[%s] %s",
		     device_name.empty() ? device_id.c_str()
					 : device_name.c_str(),
		     error);
	}

	//PRISM/WangShaohui/20200117/#281/for source unavailable
	if (active) {
		obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);
	} else {
		SetInvalidState();
	}

	previouslyFailed = !active;
	return active;
}

void WASAPISource::Reconnect()
{
	reconnecting = true;
	reconnectThread = CreateThread(
		nullptr, 0, WASAPISource::ReconnectThread, this, 0, nullptr);

	if (!reconnectThread.Valid())
		blog(LOG_WARNING,
		     "[WASAPISource::Reconnect] "
		     "Failed to initialize reconnect thread: %lu",
		     GetLastError());
}

static inline bool WaitForSignal(HANDLE handle, DWORD time)
{
	return WaitForSingleObject(handle, time) != WAIT_TIMEOUT;
}

#define RECONNECT_INTERVAL 3000

DWORD WINAPI WASAPISource::ReconnectThread(LPVOID param)
{
	WASAPISource *source = (WASAPISource *)param;

	os_set_thread_name("win-wasapi: reconnect thread");

	CoInitializeEx(0, COINIT_MULTITHREADED);

	obs_monitoring_type type =
		obs_source_get_monitoring_type(source->source);
	obs_source_set_monitoring_type(source->source,
				       OBS_MONITORING_TYPE_NONE);

	while (!WaitForSignal(source->stopSignal, RECONNECT_INTERVAL)) {
		if (source->TryInitialize())
			break;
	}

	obs_source_set_monitoring_type(source->source, type);

	source->reconnectThread = nullptr;
	source->reconnecting = false;
	return 0;
}

bool WASAPISource::ProcessCaptureData()
{
	HRESULT res;
	LPBYTE buffer;
	UINT32 frames;
	DWORD flags;
	UINT64 pos, ts;
	UINT captureSize = 0;

	while (true) {
		res = capture->GetNextPacketSize(&captureSize);

		if (FAILED(res)) {
			if (res != AUDCLNT_E_DEVICE_INVALIDATED)
				blog(LOG_WARNING,
				     "[WASAPISource::GetCaptureData]"
				     " capture->GetNextPacketSize"
				     " failed: %lX",
				     res);
			return false;
		}

		if (!captureSize)
			break;

		res = capture->GetBuffer(&buffer, &frames, &flags, &pos, &ts);
		if (FAILED(res)) {
			if (res != AUDCLNT_E_DEVICE_INVALIDATED)
				blog(LOG_WARNING,
				     "[WASAPISource::GetCaptureData]"
				     " capture->GetBuffer"
				     " failed: %lX",
				     res);
			return false;
		}

		obs_source_audio data = {};
		data.data[0] = (const uint8_t *)buffer;
		data.frames = (uint32_t)frames;
		data.speakers = speakers;
		data.samples_per_sec = sampleRate;
		data.format = format;
		data.timestamp = useDeviceTiming ? ts * 100 : os_gettime_ns();

		if (!useDeviceTiming)
			//PRISM/LiuHaibin/20200803/#None/https://github.com/obsproject/obs-studio/pull/2657
			//data.timestamp -= (uint64_t)frames * 1000000000ULL /
			//		  (uint64_t)sampleRate;
			data.timestamp -= util_mul_div64(frames, 1000000000ULL,
							 sampleRate);

		obs_source_output_audio(source, &data);

		capture->ReleaseBuffer(frames);
	}

	return true;
}

static inline bool WaitForCaptureSignal(DWORD numSignals, const HANDLE *signals,
					DWORD duration)
{
	DWORD ret;
	ret = WaitForMultipleObjects(numSignals, signals, false, duration);

	return ret == WAIT_OBJECT_0 || ret == WAIT_TIMEOUT;
}

DWORD WINAPI WASAPISource::CaptureThread(LPVOID param)
{
	WASAPISource *source = (WASAPISource *)param;
	bool reconnect = false;

	/* Output devices don't signal, so just make it check every 10 ms */
	DWORD dur = source->isInputDevice ? RECONNECT_INTERVAL : 10;

	HANDLE sigs[2] = {source->receiveSignal, source->stopSignal};

	os_set_thread_name("win-wasapi: capture thread");

	while (WaitForCaptureSignal(2, sigs, dur)) {
		if (!source->ProcessCaptureData()) {
			//PRISM/WangShaohui/20200117/#281/for source unavailable
			source->SetInvalidState();
			reconnect = true;
			break;
		}
	}

	source->client->Stop();

	source->captureThread = nullptr;
	source->active = false;

	if (reconnect) {
		blog(LOG_INFO, "Device '%s' invalidated.  Retrying",
		     source->device_name.c_str());
		source->Reconnect();
	}

	return 0;
}

/* ------------------------------------------------------------------------- */

static const char *GetWASAPIInputName(void *)
{
	return obs_module_text("AudioInput");
}

static const char *GetWASAPIOutputName(void *)
{
	return obs_module_text("AudioOutput");
}

static void GetWASAPIDefaultsInput(obs_data_t *settings)
{
	obs_data_set_default_string(settings, OPT_DEVICE_ID, "default");
	obs_data_set_default_bool(settings, OPT_USE_DEVICE_TIMING, false);
}

static void GetWASAPIDefaultsOutput(obs_data_t *settings)
{
	obs_data_set_default_string(settings, OPT_DEVICE_ID, "default");
	obs_data_set_default_bool(settings, OPT_USE_DEVICE_TIMING, true);
}

static void *CreateWASAPISource(obs_data_t *settings, obs_source_t *source,
				bool input)
{
	try {
		return new WASAPISource(settings, source, input);
	} catch (const char *error) {
		blog(LOG_ERROR, "[CreateWASAPISource] %s", error);
	}

	return nullptr;
}

static void *CreateWASAPIInput(obs_data_t *settings, obs_source_t *source)
{
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);
	return CreateWASAPISource(settings, source, true);
}

static void *CreateWASAPIOutput(obs_data_t *settings, obs_source_t *source)
{
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);
	return CreateWASAPISource(settings, source, false);
}

static void DestroyWASAPISource(void *obj)
{
	delete static_cast<WASAPISource *>(obj);
}

static void UpdateWASAPISource(void *obj, obs_data_t *settings)
{
	static_cast<WASAPISource *>(obj)->Update(settings);
}

//PRISM/WangShaohui/20200414/#2224/for lost audio device
static bool on_audio_list_changed(obs_properties_t *ppts, obs_property_t *p,
				     obs_data_t *settings)
{
	const char *cur_val = obs_data_get_string(settings, OPT_DEVICE_ID);
	if (!cur_val) {
		return false;
	}

	string current_name;
	string current_id;
	DecodeAudioString(cur_val, current_name, current_id);

	if (current_name.empty()) {
		return false;
	}

	bool match = false;
	for (size_t i = 0;;) {
		const char *val = obs_property_list_item_string(p, i++);
		if (!val)
			break;

		string temp_name;
		string temp_id;
		DecodeAudioString(val, temp_name, temp_id);

		if (current_id == temp_id) {
			match = true;
			break;
		}
	}

	if (!match) {
		size_t count = obs_property_list_item_count(p);
		int index = (count > 0) ? 1 : 0; // after "default"
		obs_property_list_insert_string(p, index, current_name.c_str(),
						cur_val);
		obs_property_list_item_disable(p, index, true);
		return true;
	}

	UNUSED_PARAMETER(ppts);
	return false;
}

static obs_properties_t *GetWASAPIProperties(bool input)
{
	obs_properties_t *props = obs_properties_create();
	vector<AudioDeviceInfo> devices;

	obs_property_t *device_prop = obs_properties_add_list(
		props, OPT_DEVICE_ID, obs_module_text("Device"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	GetWASAPIAudioDevices(devices, input);

	//PRISM/WangShaohui/20200414/#2224/for lost audio device
	/*if (devices.size())*/ {
		obs_property_list_add_string(
			device_prop, obs_module_text("Default"), "default");
	}

	for (size_t i = 0; i < devices.size(); i++) {
		AudioDeviceInfo &device = devices[i];
		//PRISM/WangShaohui/20200414/#2224/for lost audio device
		obs_property_list_add_string(device_prop, device.name.c_str(),
			EncodeAudioString(device.name.c_str(),
						device.id.c_str()).c_str());
	}

	//PRISM/WangShaohui/20200414/#2224/for lost audio device
	obs_property_set_modified_callback(device_prop, on_audio_list_changed);

	obs_properties_add_bool(props, OPT_USE_DEVICE_TIMING,
				obs_module_text("UseDeviceTiming"));

	return props;
}

static obs_properties_t *GetWASAPIPropertiesInput(void *)
{
	return GetWASAPIProperties(true);
}

static obs_properties_t *GetWASAPIPropertiesOutput(void *)
{
	return GetWASAPIProperties(false);
}

void RegisterWASAPIInput()
{
	obs_source_info info = {};
	info.id = "wasapi_input_capture";
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_AUDIO | OBS_SOURCE_DO_NOT_DUPLICATE;
	info.get_name = GetWASAPIInputName;
	info.create = CreateWASAPIInput;
	info.destroy = DestroyWASAPISource;
	info.update = UpdateWASAPISource;
	info.get_defaults = GetWASAPIDefaultsInput;
	info.get_properties = GetWASAPIPropertiesInput;
	info.icon_type = OBS_ICON_TYPE_AUDIO_INPUT;
	obs_register_source(&info);
}

void RegisterWASAPIOutput()
{
	obs_source_info info = {};
	info.id = "wasapi_output_capture";
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_AUDIO | OBS_SOURCE_DO_NOT_DUPLICATE |
			    OBS_SOURCE_DO_NOT_SELF_MONITOR;
	info.get_name = GetWASAPIOutputName;
	info.create = CreateWASAPIOutput;
	info.destroy = DestroyWASAPISource;
	info.update = UpdateWASAPISource;
	info.get_defaults = GetWASAPIDefaultsOutput;
	info.get_properties = GetWASAPIPropertiesOutput;
	info.icon_type = OBS_ICON_TYPE_AUDIO_OUTPUT;
	obs_register_source(&info);
}
