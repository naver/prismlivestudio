#pragma once
#include <Windows.h>
#include <memory>
#include "prism-ipc-buffer.h"

/*
 command line for startup effect process:
 "${guid}" "${prismVersion}" "${neloSession}" "${userPath}" "${HWND}"
 effect process should register class name with guid, and send heartbeat to HWND periodically.
*/

/*
 EVENT HANDLE
 full name : guid + ${SOURCE_ALIVE_FLAG}
 check already openned in effect process.
 it is created/deleted by camera source. Effect process should exit itselft when this HANDLE is not openned.
*/
#define SOURCE_ALIVE_FLAG "camera_source_alive"

/*
 EVENT HANDLE
 full name : guid + ${SOURCE_ACTIVE_FLAG}
 check already signed in effect process.
 it is created/deleted and signed/unsigned by camera source. Effect process should clear texture and sleep itselft when this HANDLE is not signed.
*/
#define SOURCE_ACTIVE_FLAG "camera_source_active"

/*
 EVENT HANDLE
 full name : guid + ${PROCESS_VALID_FLAG}
 check already signed in camera source.
 it is created by camera source and signed/unsigned by effect process.
 Effect process should set it with false when exception happened, such as sensetime license error or d3d11 error.
 Otherwise, its state must be true.
*/
#define PROCESS_VALID_FLAG "effect_process_valid"

/*
 file mapping HANDLE
 full name : guid + ${VIDEO_INPUT_BUFFER}
*/
#define VIDEO_INPUT_BUFFER "camera_video_input"

/*
 file mapping HANDLE
 full name : guid + ${VIDEO_OUTPUT_BUFFER}
*/
#define VIDEO_OUTPUT_BUFFER "camera_video_output"

/*
in milliseconds.
Effect process must send heartbeat to plugin.
If heatbeat is timeout, camera plugin won't pass video for effect filter.
*/
#define HEART_BEAT_TIMEOUT 5000

/*
in milliseconds
Used for SendMessageTimeout(...)
*/
#define MESSAGE_TIMEOUT_MS 2000

/*
file name of effect process
*/
#define CAMERA_EFFECT_PROCESS_NAME L"cam-effect.exe"

/*
in milliseconds
Everytime while pushing new video data, we should update its time in header.
On the side of reader module, we should check its time and discard this frame if it is obsoleted.
*/
#define VIDEO_OBSOLETE_TIME 1000

#define EXCEPTION_DESC_MAX_LEN 40

//--------------------------------------------------------------
// support bit operation
enum cam_effect_message_type {
	// effect process ==> camera source
	// process should send heartbeat to PRISM every 1 second.
	// effect won't be applied if heartbeat is timeout.
	effect_message_process_heartbeat = 0,

	// camera source ==> effect process
	effect_message_face_beauty = 0X00000001,

	// effect process ==> camera source
	effect_message_process_exception = 0X00000002,
};

enum cam_effect_process_exit_code {
	cam_effect_process_param_error = 1000,
	cam_effect_process_register_class_failed,
	cam_effect_process_create_window_failed,
	cam_effect_process_init_failed,
};

enum cam_effect_exception_type {
	effect_exception_unknown = 0,
	effect_exception_sensetime,
	effect_exception_d3d,
};

//--------------------------------------------------------------
#pragma pack(1)

struct shared_handle_header {
	shared_handle_header() : flip(false), push_time(GetTickCount()) {}

	bool is_handle_available()
	{
		DWORD current_time = GetTickCount();
		if (current_time < push_time) {
			return false; // Generally, this situation should not happen
		} else {
			DWORD time_interval = current_time - push_time;
			if (time_interval <= VIDEO_OBSOLETE_TIME) {
				return true;
			} else {
				return false; // data is too old
			}
		}
	}

	bool flip;
	DWORD push_time; // returned by GetTickCount(), its value should be set before writing into IPC
};

struct shared_handle_adapter_luid {
	DWORD low_part = 0;
	LONG high_part = 0;
};

struct shared_handle_sample {
	shared_handle_sample() : handle(0) {}

	ULONG64 handle;
	shared_handle_adapter_luid luid;
};

struct cam_effect_message {
	cam_effect_message(cam_effect_message_type t) : type(t)
	{
		memset(guid, 0, sizeof(guid));
	}
	virtual ~cam_effect_message() {}

	char guid[64];
	cam_effect_message_type type;
};

struct face_beauty_message : public cam_effect_message {
	face_beauty_message()
		: cam_effect_message(effect_message_face_beauty),
		  enable(false),
		  chin(1.0),
		  cheek(1.0),
		  cheekbone(1.0),
		  eyes(1.0),
		  nose(1.0),
		  smooth(1.0)
	{
		memset(id, 0, sizeof(id));
		memset(category, 0, sizeof(category));
	}

	void CopyData(const face_beauty_message *face_msg)
	{
		if (!face_msg)
			return;
		memcpy(guid, face_msg->guid, sizeof(guid));
		type = face_msg->type;
		enable = face_msg->enable;
		memcpy(id, face_msg->id, sizeof(id));
		memcpy(category, face_msg->category, sizeof(category));
		chin = face_msg->chin;
		cheek = face_msg->cheek;
		cheekbone = face_msg->cheekbone;
		eyes = face_msg->eyes;
		nose = face_msg->nose;
		smooth = face_msg->smooth;
	}

	bool enable; // if false, do not need other params
	char id[256];
	char category[256];
	float chin;
	float cheek;
	float cheekbone;
	float eyes;
	float nose;
	float smooth;
};

struct heart_beat_message : public cam_effect_message {
	heart_beat_message()
		: cam_effect_message(effect_message_process_heartbeat)
	{
	}
};

struct process_exception_message : public cam_effect_message {
	process_exception_message()
		: cam_effect_message(effect_message_process_exception),
		  exception_type(effect_exception_unknown),
		  sub_code(0)
	{
		memset(desc, 0, sizeof(desc));
	}

	cam_effect_exception_type exception_type;
	int sub_code;
	char desc[EXCEPTION_DESC_MAX_LEN];
};

typedef std::shared_ptr<cam_effect_message> EFFECT_MESSAGE_PTR;

#pragma pack()
//--------------------------------------------------------------

class SharedHandleBuffer : public CircleBufferIPC {
public:
	SharedHandleBuffer(const char *queueName)
		: CircleBufferIPC(queueName, 0, sizeof(shared_handle_header), 1,
				  sizeof(shared_handle_sample))
	{
	}
};
