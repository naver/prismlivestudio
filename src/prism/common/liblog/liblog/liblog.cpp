#include "liblog.h"

#include <stdarg.h>
#include <stdio.h>
#include <process.h>
#include <malloc.h>
#include <Windows.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#define LIBLOG_MODULE "liblog"

#pragma pack(push, 1)
enum class message_type_t : int { MT_SET_USER_ID, MT_ADD_GLOBAL_FIELD, MT_REMOVE_GLOBAL_FIELD, MT_LOG, MT_EXIT };

struct message_t {
	message_type_t type;
	int total_length;
};

struct add_global_field_t {
	message_t message;
	int key_offset;
	int value_offset;
};

enum class log_type_t : int { GENERIC_LOG, ACTION_LOG };

struct list_t {
	list_t *prev;
	list_t *next;
};
struct log_info_t {
	union {
		message_t message;
		list_t listpos;
	};
	log_type_t type;
	int total_length;
};
struct generic_log_info_t {
	log_info_t log_info;
	pls_log_level_t log_level;
	int file_line;
	uint32_t tid;
	int field_count;
	pls_time_t time;
	int module_name_offset;
	int file_name_offset;
	int fields_offset;
	int message_offset;
	//char *module_name;
	//char *file_name;
	//char ***fields;
	//char *message;
};
struct generic_log_field_t {
	int key_offset;
	int value_offset;
	int next_offset;
};
struct action_log_info_t {
	log_info_t log_info;
	int file_line;
	uint32_t tid;
	pls_time_t time;
	char *module_name;
	char *controls;
	char *action;
	char *file_name;
};
#pragma pack(pop)

#define UNUSED_PARAM(p) (void)p

static void def_log_handler(pls_log_level_t log_level, const char *module_name, const pls_time_t &time, uint32_t tid, const char *file_name, int file_line, const char *message, void *param)
{
	UNUSED_PARAM(log_level);
	UNUSED_PARAM(module_name);
	UNUSED_PARAM(time);
	UNUSED_PARAM(tid);
	UNUSED_PARAM(file_name);
	UNUSED_PARAM(file_line);
	UNUSED_PARAM(message);
	UNUSED_PARAM(param);
}
static void def_action_log_handler(const char *module_name, const pls_time_t &time, uint32_t tid, const char *controls, const char *action, const char *file_name, int file_line, void *param)
{
	UNUSED_PARAM(module_name);
	UNUSED_PARAM(time);
	UNUSED_PARAM(tid);
	UNUSED_PARAM(controls);
	UNUSED_PARAM(action);
	UNUSED_PARAM(file_name);
	UNUSED_PARAM(file_line);
	UNUSED_PARAM(param);
}

static bool is_initialized = false;

static PROCESS_INFORMATION logger_process_info = {NULL, NULL, 0, 0};
static HANDLE named_pipe = nullptr;
static HANDLE named_pipe_send_mutex = nullptr;
static pls_log_handler_t log_handler = def_log_handler;
static void *log_param = nullptr;
static pls_action_log_handler_t action_log_handler = def_action_log_handler;
static void *action_log_param = nullptr;

static bool async_thread_running = false;
static HANDLE async_thread = nullptr;
static HANDLE log_queue_sem = nullptr;
static HANDLE log_queue_mutex = nullptr;
static list_t log_queue = {&log_queue, &log_queue};
static HANDLE log_process_mutex = nullptr;
static generic_log_info_t *last_log_info = nullptr;
static int last_same_log_count = 0;
static const char repeat_log_message_format[] = "same log message, count: %d";
static const int repeat_log_message_length = sizeof(repeat_log_message_format) + 20;

namespace {
class MutexGuard {
	HANDLE mutex;

public:
	MutexGuard(HANDLE mutex_) : mutex(mutex_) { WaitForSingleObject(mutex, INFINITE); }
	~MutexGuard() { ReleaseMutex(mutex); }
};
}

static const char *get_file_name(const char *path)
{
	for (const char *pos = path + strlen(path) - 1; pos >= path; --pos) {
		if ((*pos == '\\') || (*pos == '/')) {
			return pos + 1;
		}
	}
	return path;
}
static void get_time(pls_time_t &time)
{
	SYSTEMTIME st = {0};
	::GetLocalTime(&st);
	time.year = st.wYear;
	time.month = st.wMonth;
	time.day = st.wDay;
	time.hour = st.wHour;
	time.minute = st.wMinute;
	time.second = st.wSecond;
	time.milliseconds = st.wMilliseconds;

	TIME_ZONE_INFORMATION tzi = {0};
	::GetTimeZoneInformation(&tzi);
	time.timezone = tzi.Bias;
}

static inline char *get_log_info_module_name(generic_log_info_t *log_info)
{
	return ((char *)log_info) + log_info->module_name_offset;
}

static inline char *get_log_info_file_name(generic_log_info_t *log_info)
{
	return log_info->file_name_offset > 0 ? (((char *)log_info) + log_info->file_name_offset) : nullptr;
}

static inline char *get_log_info_message(generic_log_info_t *log_info)
{
	return ((char *)log_info) + log_info->message_offset;
}

static inline bool is_equal_string(const char *str1, const char *str2)
{
	if (str1 == str2) {
		return true;
	} else if (str1 && str2 && !strcmp(str1, str2)) {
		return true;
	}
	return false;
}

static log_info_t *new_generic_log_info(pls_log_level_t log_level, const char *module_name, const pls_time_t &time, uint32_t tid, const char *file_name, int file_line, const char *fields[][2],
					int field_count, const char *format, va_list args)
{
	int total_length = sizeof(generic_log_info_t);
	int module_name_length = 0;
	int file_name_length = 0;
	int fields_length = 0;
	int filed_lengths[100][2] = {0};
	int message_length = 0;

	module_name_length = int(strlen(module_name) + 1);
	total_length += module_name_length;

	if (file_name) {
		file_name_length = int(strlen(file_name) + 1);
		total_length += file_name_length;
	}

	if (field_count > 0) {
		field_count = (field_count < 100) ? field_count : 100;

		fields_length = sizeof(generic_log_field_t) * field_count;
		total_length += fields_length;

		for (int i = 0; i < field_count; ++i) {
			for (int j = 0; j < 2; ++j) {
				filed_lengths[i][j] = int(strlen(fields[i][j]) + 1);
				total_length += filed_lengths[i][j];
			}
		}
	}

	message_length = vsnprintf(nullptr, 0, format, args) + 1;
	message_length = message_length > repeat_log_message_length ? message_length : repeat_log_message_length;
	total_length += message_length;

	generic_log_info_t *generic_log_info = (generic_log_info_t *)malloc(total_length);
	if (!generic_log_info) {
		return nullptr;
	}

	memset(generic_log_info, 0, total_length);

	generic_log_info->log_info.type = log_type_t::GENERIC_LOG;
	generic_log_info->log_info.total_length = total_length;

	generic_log_info->log_level = log_level;
	generic_log_info->file_line = file_line;
	generic_log_info->tid = tid;
	generic_log_info->field_count = field_count;
	generic_log_info->time = time;

	char *buffer = (char *)(generic_log_info + 1);

	generic_log_info->module_name_offset = buffer - (char *)generic_log_info;
	strcpy(buffer, module_name);
	buffer += module_name_length;

	if (file_name) {
		generic_log_info->file_name_offset = buffer - (char *)generic_log_info;
		strcpy(buffer, file_name);
		buffer += file_name_length;
	} else {
		generic_log_info->file_name_offset = 0;
	}

	if (field_count > 0) {
		generic_log_info->fields_offset = buffer - (char *)generic_log_info;

		for (int i = 0; i < field_count; ++i) {
			generic_log_field_t *field = (generic_log_field_t *)buffer;

			buffer += sizeof(generic_log_field_t);
			field->key_offset = buffer - (char *)generic_log_info;
			strcpy(buffer, fields[i][0]);

			buffer += filed_lengths[i][0];
			field->value_offset = buffer - (char *)generic_log_info;
			strcpy(buffer, fields[i][1]);

			buffer += filed_lengths[i][1];
			field->next_offset = buffer - (char *)generic_log_info;
		}
	} else {
		generic_log_info->fields_offset = 0;
	}

	generic_log_info->message_offset = buffer - (char *)generic_log_info;
	vsnprintf(buffer, message_length, format, args);

	return &generic_log_info->log_info;
}
static log_info_t *new_action_log_info(const char *module_name, const pls_time_t &time, uint32_t tid, const char *controls, const char *action, const char *file_name, int file_line)
{
	int total_length = sizeof(action_log_info_t);
	int module_name_length = 0;
	int controls_length = 0;
	int action_length = 0;
	int file_name_length = 0;

	module_name_length = int(strlen(module_name) + 1);
	total_length += module_name_length;

	controls_length = int(strlen(controls) + 1);
	total_length += controls_length;

	action_length = int(strlen(action) + 1);
	total_length += action_length;

	if (file_name) {
		file_name_length = int(strlen(file_name) + 1);
		total_length += file_name_length;
	}

	action_log_info_t *action_log_info = (action_log_info_t *)malloc(total_length);
	if (!action_log_info) {
		return nullptr;
	}

	memset(action_log_info, 0, total_length);

	action_log_info->log_info.type = log_type_t::ACTION_LOG;

	action_log_info->file_line = file_line;
	action_log_info->tid = tid;
	action_log_info->time = time;

	char *buffer = (char *)(action_log_info + 1);
	strcpy(action_log_info->module_name = buffer, module_name);
	buffer += module_name_length;

	strcpy(action_log_info->controls = buffer, controls);
	buffer += controls_length;

	strcpy(action_log_info->action = buffer, action);
	buffer += action_length;

	if (file_name) {
		strcpy(action_log_info->file_name = buffer, file_name);
		buffer += file_name_length;
	}

	return &action_log_info->log_info;
}

static bool list_empty(list_t *list)
{
	return list->next == list;
}
static void list_push(list_t *list, list_t *one)
{
	list->prev->next = one;
	one->prev = list->prev;
	one->next = list;
	list->prev = one;
}
static list_t *list_pop(list_t *list)
{
	if (list_empty(list)) {
		return nullptr;
	}

	list_t *one = list->next;
	one->next->prev = one->prev;
	one->prev->next = one->next;
	return one;
}

static void push_log_info(log_info_t *log_info)
{
	WaitForSingleObject(log_queue_mutex, INFINITE);
	list_push(&log_queue, &log_info->listpos);
	ReleaseMutex(log_queue_mutex);

	ReleaseSemaphore(log_queue_sem, 1, nullptr);
}
static log_info_t *pop_log_info(uint32_t timeout = INFINITE)
{
	if (WaitForSingleObject(log_queue_sem, timeout) == WAIT_OBJECT_0) {
		WaitForSingleObject(log_queue_mutex, INFINITE);
		log_info_t *one = (log_info_t *)list_pop(&log_queue);
		ReleaseMutex(log_queue_mutex);
		return one;
	}
	return nullptr;
}
static void send_n_i(char *buffer, int count)
{
	if (named_pipe) {
		int byte_write = 0;
		for (DWORD written = 0; count > 0; count -= written, byte_write += written) {
			if (!WriteFile(named_pipe, buffer + byte_write, count, &written, NULL)) {
				CloseHandle(named_pipe);
				named_pipe = nullptr;
				return;
			}
		}
	}
}
static void send_n(char *buffer, int count)
{
	WaitForSingleObject(named_pipe_send_mutex, INFINITE);
	send_n_i(buffer, count);
	ReleaseMutex(named_pipe_send_mutex);
}

static bool is_same_log_info(generic_log_info_t *log_info1, generic_log_info_t *log_info2)
{
	// log level
	if (log_info1->log_level != log_info2->log_level) {
		return false;
	}

	// thread id
	if (log_info1->tid != log_info2->tid) {
		return false;
	}

	// module name
	if (!is_equal_string(get_log_info_module_name(log_info1), get_log_info_module_name(log_info2))) {
		return false;
	}

	// file name
	if (!is_equal_string(get_log_info_file_name(log_info1), get_log_info_file_name(log_info2))) {
		return false;
	}

	// file line
	if (log_info1->file_line != log_info2->file_line) {
		return false;
	}

	// message
	if (!is_equal_string(get_log_info_message(log_info1), get_log_info_message(log_info2))) {
		return false;
	}
	return true;
}

static void log_process_i(generic_log_info_t *log_info)
{
	char *module_name = get_log_info_module_name(log_info);
	char *file_name = get_log_info_file_name(log_info);
	char *message = get_log_info_message(log_info);

	log_handler(log_info->log_level, module_name, log_info->time, log_info->tid, file_name, log_info->file_line, message, log_param);

	if (named_pipe) {
		log_info->log_info.message.type = message_type_t::MT_LOG;
		log_info->log_info.message.total_length = log_info->log_info.total_length;
		send_n((char *)log_info, log_info->log_info.total_length);
	}
}

static void log_process(generic_log_info_t *log_info)
{
	MutexGuard guard(log_process_mutex);

	if (last_log_info) {
		if (is_same_log_info(last_log_info, log_info)) {
			++last_same_log_count;
			free(log_info);
			return;
		} else if (last_same_log_count > 0) {
			get_time(last_log_info->time);
			snprintf(get_log_info_message(last_log_info), repeat_log_message_length - 1, repeat_log_message_format, last_same_log_count);
			log_process_i(last_log_info);
			free(last_log_info);
		}
	}

	last_log_info = log_info;
	last_same_log_count = 0;
	log_process_i(log_info);
}
static void log_process(action_log_info_t *log_info)
{
	MutexGuard guard(log_process_mutex);

	action_log_handler(log_info->module_name, log_info->time, log_info->tid, log_info->controls, log_info->action, log_info->file_name, log_info->file_line, log_param);

	free(log_info);
}

static void generic_logva(pls_log_level_t log_level, const char *module_name, const pls_time_t &time, uint32_t tid, const char *file_name, int file_line, const char *fields[][2], int field_count,
			  const char *format, va_list args)
{
	if (!is_initialized) {
		return;
	}

	log_info_t *log_info = new_generic_log_info(log_level, module_name, time, tid, file_name, file_line, fields, field_count, format, args);
	if (log_info) {
		if (async_thread) {
			push_log_info(log_info);
		} else {
			log_process((generic_log_info_t *)log_info);
		}
	}
}
static void generic_log(pls_log_level_t log_level, const char *module_name, const pls_time_t &time, uint32_t tid, const char *file_name, int file_line, const char *fields[][2], int field_count,
			const char *format, ...)
{
	va_list args;
	va_start(args, format);
	generic_logva(log_level, module_name, time, tid, file_name, file_line, fields, field_count, format, args);
	va_end(args);
}
static void action_log(const char *module_name, const pls_time_t &time, uint32_t tid, const char *controls, const char *action, const char *file_name, int file_line)
{
	if (!is_initialized) {
		return;
	}

	log_info_t *log_info = new_action_log_info(module_name, time, tid, controls, action, file_name, file_line);
	if (log_info) {
		if (async_thread) {
			push_log_info(log_info);
		} else {
			log_process((action_log_info_t *)log_info);
		}
	}
}

static unsigned __stdcall async_thread_proc(void *param)
{
	while (async_thread_running) {
		log_info_t *log_info = pop_log_info();
		if (!log_info) {
			continue;
		}

		switch (log_info->type) {
		case log_type_t::GENERIC_LOG:
			log_process((generic_log_info_t *)log_info);
			break;
		case log_type_t::ACTION_LOG:
			log_process((action_log_info_t *)log_info);
			break;
		default:
			free(log_info);
			break;
		}
	}

	while (log_info_t *log_info = pop_log_info(0)) {
		switch (log_info->type) {
		case log_type_t::GENERIC_LOG:
			log_process((generic_log_info_t *)log_info);
			break;
		case log_type_t::ACTION_LOG:
			log_process((action_log_info_t *)log_info);
			break;
		default:
			free(log_info);
			break;
		}
	}

	if (last_log_info) {
		free(last_log_info);
		last_log_info = nullptr;
	}
	return 0;
}

static void log_cleanup()
{
	async_thread_running = false;

	if (async_thread) {
		ReleaseSemaphore(log_queue_sem, 1, nullptr);
		WaitForSingleObject(async_thread, INFINITE);
		CloseHandle(async_thread);
		async_thread = nullptr;
	}

	if (log_queue_sem) {
		CloseHandle(log_queue_sem);
		log_queue_sem = nullptr;
	}

	if (log_queue_mutex) {
		CloseHandle(log_queue_mutex);
		log_queue_mutex = nullptr;
	}

	if (log_process_mutex) {
		CloseHandle(log_process_mutex);
		log_process_mutex = nullptr;
	}

	if (named_pipe) {
		message_t message = {message_type_t::MT_EXIT, sizeof(message_t)};
		send_n((char *)&message, sizeof(message));
	}

	if (logger_process_info.hProcess) {
		WaitForSingleObject(logger_process_info.hProcess, 1000);
		CloseHandle(logger_process_info.hProcess);
		logger_process_info.hProcess = nullptr;
	}

	if (logger_process_info.hThread) {
		CloseHandle(logger_process_info.hThread);
		logger_process_info.hThread = nullptr;
	}

	if (named_pipe) {
		CloseHandle(named_pipe);
		named_pipe = nullptr;
	}

	if (named_pipe_send_mutex) {
		CloseHandle(named_pipe_send_mutex);
		named_pipe_send_mutex = nullptr;
	}

	if (last_log_info) {
		free(last_log_info);
		last_log_info = nullptr;
	}
}

LIBLOG_API void pls_log_init(const char *project_name, const char *project_version, const char *log_source)
{
	if (is_initialized) {
		return;
	}

	if (!(log_process_mutex = CreateMutexW(nullptr, FALSE, nullptr))) {
		log_cleanup();
		return;
	}

	if (!(log_queue_sem = CreateSemaphoreW(nullptr, 0, INT_MAX, nullptr)) || !(log_queue_mutex = CreateMutexW(nullptr, FALSE, nullptr))) {
		log_cleanup();
		return;
	}

	async_thread_running = true;
	if (!(async_thread = (HANDLE)_beginthreadex(nullptr, 0, &async_thread_proc, nullptr, 0, nullptr))) {
		log_cleanup();
		return;
	}

	is_initialized = true;

	uint32_t pid = GetCurrentProcessId();

	char loggerApp[512];
	GetModuleFileNameA(NULL, loggerApp, 512);
	PathRemoveFileSpecA(loggerApp);
	strcat_s(loggerApp, "\\PRISMLogger.exe");

	char commandLine[1024];
	sprintf_s(commandLine, "\"%s\" \"%s\" \"%s\" \"%s\" \"%u\"", loggerApp, project_name, project_version, log_source, pid);

	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.lpTitle = "PRISMLogger";

	char loggerRunEventName[128];
	sprintf_s(loggerRunEventName, "PRISMLoggerRunEvent%u", pid);
	HANDLE logger_run_event = CreateEventA(NULL, TRUE, FALSE, loggerRunEventName);
	if (!logger_run_event) {
		return;
	}

	if (!CreateProcessA(NULL, commandLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &logger_process_info)) {
		CloseHandle(logger_run_event);
		return;
	}

	WaitForSingleObject(logger_run_event, 5000);
	CloseHandle(logger_run_event);

	char pipeName[128];
	sprintf_s(pipeName, "\\\\.\\pipe\\PRISMLoggerPipe%u", pid);
	if ((named_pipe = CreateFileA(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) {
		log_cleanup();
		return;
	}

	if (!(named_pipe_send_mutex = CreateMutexW(nullptr, FALSE, nullptr))) {
		log_cleanup();
		return;
	}
}
LIBLOG_API void pls_log_cleanup()
{
	if (!is_initialized) {
		return;
	}

	is_initialized = false;
	log_cleanup();
}

LIBLOG_API void pls_set_log_handler(pls_log_handler_t handler, void *param)
{
	log_handler = handler;
	log_param = param;
}
LIBLOG_API void pls_get_log_handler(pls_log_handler_t *handler, void **param)
{
	if (handler) {
		*handler = log_handler;
	}
	if (param) {
		*param = log_param;
	}
}
LIBLOG_API void pls_reset_log_handler()
{
	log_handler = def_log_handler;
	log_param = nullptr;
}

LIBLOG_API void pls_set_action_log_handler(pls_action_log_handler_t handler, void *param)
{
	action_log_handler = handler;
	action_log_param = param;
}
LIBLOG_API void pls_get_action_log_handler(pls_action_log_handler_t *handler, void **param)
{
	if (handler) {
		*handler = action_log_handler;
	}
	if (param) {
		*param = action_log_param;
	}
}
LIBLOG_API void pls_reset_action_log_handler()
{
	action_log_handler = def_action_log_handler;
	action_log_param = nullptr;
}

LIBLOG_API void pls_set_user_id(const char *user_id)
{
	if (!user_id) {
		return;
	}

	if (named_pipe) {
		int total_length = sizeof(message_t) + strlen(user_id) + 1;
		message_t *msg = (message_t *)malloc(total_length);
		if (msg) {
			msg->type = message_type_t::MT_SET_USER_ID;
			msg->total_length = total_length;
			strcpy((char *)(msg + 1), user_id);
			send_n((char *)msg, total_length);
			free(msg);
		} else {
			PLS_ERROR(LIBLOG_MODULE, "nelo2 set user id failed, because malloc memory failed.");
		}
	} else {
		PLS_ERROR(LIBLOG_MODULE, "nelo2 set user id failed, because named pipe invalid failed.");
	}
}

LIBLOG_API void pls_add_global_field(const char *key, const char *value)
{
	if (!key || !value) {
		return;
	}

	if (named_pipe) {
		int key_length = strlen(key) + 1;
		int value_length = strlen(value) + 1;
		int total_length = sizeof(add_global_field_t) + key_length + value_length;
		add_global_field_t *msg = (add_global_field_t *)malloc(total_length);
		if (msg) {
			msg->message.type = message_type_t::MT_ADD_GLOBAL_FIELD;
			msg->message.total_length = total_length;

			char *buffer = (char *)(msg + 1);

			msg->key_offset = buffer - (char *)msg;
			strcpy(buffer, key);
			buffer += key_length;

			msg->value_offset = buffer - (char *)msg;
			strcpy(buffer, value);

			send_n((char *)msg, total_length);
			free(msg);
		} else {
			PLS_ERROR(LIBLOG_MODULE, "nelo2 add global field failed, because malloc memory failed.");
		}
	} else {
		PLS_ERROR(LIBLOG_MODULE, "nelo2 add global field failed, because named pipe invalid failed.");
	}
}

LIBLOG_API void pls_remove_global_field(const char *key)
{
	if (!key) {
		return;
	}

	if (named_pipe) {
		int total_length = sizeof(message_t) + strlen(key) + 1;
		message_t *msg = (message_t *)malloc(total_length);
		if (msg) {
			msg->type = message_type_t::MT_REMOVE_GLOBAL_FIELD;
			msg->total_length = total_length;
			strcpy((char *)(msg + 1), key);
			send_n((char *)msg, total_length);
			free(msg);
		} else {
			PLS_ERROR(LIBLOG_MODULE, "nelo2 remove global field failed, because malloc memory failed.");
		}
	} else {
		PLS_ERROR(LIBLOG_MODULE, "nelo2 remove global field failed, because named pipe invalid failed.");
	}
}

LIBLOG_API void pls_logva(pls_log_level_t log_level, const char *module_name, const char *file_name, int file_line, const char *format, va_list args)
{
	pls_logvaex(log_level, module_name, file_name, file_line, nullptr, 0, format, args);
}
LIBLOG_API void pls_logvaex(pls_log_level_t log_level, const char *module_name, const char *file_name, int file_line, const char *fields[][2], int field_count, const char *format, va_list args)
{
	pls_time_t time;
	get_time(time);

	uint32_t tid = GetCurrentThreadId();

	file_name = file_name ? get_file_name(file_name) : file_name;

	generic_logva(log_level, module_name, time, tid, file_name, file_line, fields, field_count, format, args);
}
LIBLOG_API void pls_log(pls_log_level_t log_level, const char *module_name, const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(log_level, module_name, file_name, file_line, format, args);
	va_end(args);
}
LIBLOG_API void pls_logex(pls_log_level_t log_level, const char *module_name, const char *file_name, int file_line, const char *fields[][2], int field_count, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logvaex(log_level, module_name, file_name, file_line, fields, field_count, format, args);
	va_end(args);
}
LIBLOG_API void pls_error(const char *module_name, const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_ERROR, module_name, file_name, file_line, format, args);
	va_end(args);
}
LIBLOG_API void pls_warn(const char *module_name, const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_WARN, module_name, file_name, file_line, format, args);
	va_end(args);
}
LIBLOG_API void pls_info(const char *module_name, const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_INFO, module_name, file_name, file_line, format, args);
	va_end(args);
}
LIBLOG_API void pls_debug(const char *module_name, const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_DEBUG, module_name, file_name, file_line, format, args);
	va_end(args);
}
LIBLOG_API void pls_action_log(const char *module_name, const char *controls, const char *action, const char *file_name, int file_line)
{
	pls_time_t time;
	get_time(time);

	uint32_t tid = GetCurrentThreadId();

	file_name = file_name ? get_file_name(file_name) : file_name;

	action_log(module_name, time, tid, controls, action, file_name, file_line);
}
LIBLOG_API void pls_ui_step(const char *module_name, const char *controls, const char *action, const char *file_name, int file_line)
{
	pls_time_t time;
	get_time(time);

	uint32_t tid = GetCurrentThreadId();

	file_name = file_name ? get_file_name(file_name) : file_name;

	const char *fields[][2] = {{"ui-step", "all"}};
	generic_log(PLS_LOG_INFO, module_name, time, tid, file_name, file_line, fields, 1, "UI: [UI STEP] %s %s", controls, action);
}
