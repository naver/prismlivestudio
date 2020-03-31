#include <nelo2.h>
#include <liblog.h>
#include <Windows.h>
#include <process.h>

#define ConsolePrint(format, ...) printf(format, __VA_ARGS__)

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

enum ExitCode {
	EC_Ok,
	EC_Nelo2InitFailed,
	EC_,
};

static void log_process(NELO2Log *nelo2_log, generic_log_info_t *log_info);
static void set_user_id(NELO2Log *nelo2_log, const char *user_id);
static void add_global_field(NELO2Log *nelo2_log, add_global_field_t *field);
static void remove_global_field(NELO2Log *nelo2_log, const char *key);
static bool read_n(HANDLE named_pipe, char *buffer, int count);
static message_t *read_message(HANDLE named_pipe, message_t *msg);

int main(int argc, char **argv)
{
	/**
	  *     [in]1 projectName: project name
	  *     [in]2 projectVersion: project version
	  *     [in]3 logSource: log source
	  *     [in]4 clientPid: client pid
	  */
	const char *projectName = argv[1];
	const char *projectVersion = argv[2];
	const char *logSource = argv[3];
	const char *clientPid = argv[4];

	int retval = EC_Ok;
	NELO2Log *nelo2_log = nullptr;
	HANDLE named_pipe = nullptr;
	message_t message;

	ConsolePrint("projectName: %s\n", projectName);
	ConsolePrint("projectVersion: %s\n", projectVersion);
	ConsolePrint("logSource: %s\n", logSource);
	ConsolePrint("clientPid: %s\n", clientPid);

	char loggerRunEventName[128];
	sprintf_s(loggerRunEventName, "PRISMLoggerRunEvent%s", clientPid);
	HANDLE logger_run_event = OpenEventA(EVENT_ALL_ACCESS, FALSE, loggerRunEventName);
	if (!logger_run_event) {
		ConsolePrint("open PRISMLoggerRunEvent failed\n");
		return false;
	}

	nelo2_log = new NELO2Log();
	if (NELO2_OK != nelo2_log->initialize(projectName, projectVersion, logSource, "nelo2-app", "nelo2-col.navercorp.com", 80)) {
		ConsolePrint("NELO2 initialize failed\n");
		retval = EC_Nelo2InitFailed;
		goto EXIT_APP;
	} else if (!nelo2_log->openCrashCatcher(false, NELO_LANG_ENGLISH)) {
		ConsolePrint("NELO2 openCrashCatcher failed\n");
		retval = EC_Nelo2InitFailed;
		goto EXIT_APP;
	}

	char pipeName[128];
	sprintf_s(pipeName, "\\\\.\\pipe\\PRISMLoggerPipe%s", clientPid);
	if ((named_pipe = CreateNamedPipeA(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 10240, 10240, 1000, NULL)) ==
	    INVALID_HANDLE_VALUE) {
		ConsolePrint("CreateNamedPipe failed\n");
		retval = 3;
		goto EXIT_APP;
	}

	if (logger_run_event) {
		SetEvent(logger_run_event);
		CloseHandle(logger_run_event);
		logger_run_event = NULL;
	}

	if (!ConnectNamedPipe(named_pipe, NULL) && (GetLastError() != ERROR_PIPE_CONNECTED)) {
		ConsolePrint("ConnectNamedPipe failed\n");
		retval = 3;
		goto EXIT_APP;
	}

	while (true) {
		if (!read_n(named_pipe, (char *)&message, sizeof(message))) {
			ConsolePrint("read message failed\n");
			break;
		}

		if (message.type == message_type_t::MT_LOG) {
			message_t *msg = read_message(named_pipe, &message);
			if (msg) {
				log_process(nelo2_log, (generic_log_info_t *)msg);
				free(msg);
			} else {
				break;
			}
		} else if (message.type == message_type_t::MT_SET_USER_ID) {
			message_t *msg = read_message(named_pipe, &message);
			if (msg) {
				set_user_id(nelo2_log, (char *)(msg + 1));
				free(msg);
			} else {
				break;
			}
		} else if (message.type == message_type_t::MT_ADD_GLOBAL_FIELD) {
			message_t *msg = read_message(named_pipe, &message);
			if (msg) {
				add_global_field(nelo2_log, (add_global_field_t *)msg);
				free(msg);
			} else {
				break;
			}
		} else if (message.type == message_type_t::MT_REMOVE_GLOBAL_FIELD) {
			message_t *msg = read_message(named_pipe, &message);
			if (msg) {
				remove_global_field(nelo2_log, (char *)(msg + 1));
				free(msg);
			} else {
				break;
			}
		} else if (message.type == message_type_t::MT_EXIT) {
			ConsolePrint("read exit message\n");
			break;
		}
	}

EXIT_APP:
	ConsolePrint("app exit\n");

	if (logger_run_event) {
		SetEvent(logger_run_event);
		CloseHandle(logger_run_event);
		logger_run_event = NULL;
	}

	if (named_pipe) {
		CloseHandle(named_pipe);
		named_pipe = nullptr;
	}

	if (nelo2_log) {
		nelo2_log->closeCrashCatcher();
		nelo2_log->destory();
		delete nelo2_log;
		nelo2_log = nullptr;
	}

	return retval;
}

static void log_process(NELO2Log *nelo2_log, generic_log_info_t *log_info)
{
	char *module_name = ((char *)log_info) + log_info->module_name_offset;
	char *file_name = log_info->file_name_offset > 0 ? (((char *)log_info) + log_info->file_name_offset) : nullptr;
	char *message = ((char *)log_info) + log_info->message_offset;

	char msg[4096];
	if (file_name && (log_info->file_line > 0)) {
		snprintf(msg, sizeof(msg) - 1, "%04u-%02u-%02u_%02u:%02u:%02u.%03u%+03d:%02d [%s:%d] (%u) %s", log_info->time.year, log_info->time.month, log_info->time.day, log_info->time.hour,
			 log_info->time.minute, log_info->time.second, log_info->time.milliseconds, (0 - log_info->time.timezone / 60), (abs(log_info->time.timezone) % 60), file_name,
			 log_info->file_line, log_info->tid, message);
	} else {
		snprintf(msg, sizeof(msg) - 1, "%04u-%02u-%02u_%02u:%02u:%02u.%03u%+03d:%02d (%u) %s", log_info->time.year, log_info->time.month, log_info->time.day, log_info->time.hour,
			 log_info->time.minute, log_info->time.second, log_info->time.milliseconds, (0 - log_info->time.timezone / 60), (abs(log_info->time.timezone) % 60), log_info->tid, message);
	}

	ConsolePrint("%s\n", msg);

	NELO2Log::CustomField customField;
	customField.addField("module", module_name);
	if (log_info->field_count > 0) {
		char *fields = ((char *)log_info) + log_info->fields_offset;
		for (int i = 0; i < log_info->field_count; ++i) {
			generic_log_field_t *field = (generic_log_field_t *)fields;
			customField.addField(((char *)log_info) + field->key_offset, ((char *)log_info) + field->value_offset);
			fields = ((char *)log_info) + field->next_offset;
		}
	}

	switch (log_info->log_level) {
	case PLS_LOG_ERROR:
		nelo2_log->sendLog(NELO_LL_ERROR, msg, customField);
		break;
	case PLS_LOG_WARN:
		nelo2_log->sendLog(NELO_LL_WARN, msg, customField);
		break;
	case PLS_LOG_INFO:
		nelo2_log->sendLog(NELO_LL_INFO, msg, customField);
		break;
	case PLS_LOG_DEBUG:
		nelo2_log->sendLog(NELO_LL_DEBUG, msg, customField);
		break;
	}
}
static void set_user_id(NELO2Log *nelo2_log, const char *user_id)
{
	ConsolePrint("set user id: %s\n", user_id);
	nelo2_log->setUserId(user_id);
}
static void add_global_field(NELO2Log *nelo2_log, add_global_field_t *field)
{
	char *key = ((char *)field) + field->key_offset;
	char *value = ((char *)field) + field->value_offset;
	ConsolePrint("add global field: key = %s, value = %s\n", key, value);
	nelo2_log->addGlobalField(key, value);
}
static void remove_global_field(NELO2Log *nelo2_log, const char *key)
{
	ConsolePrint("remove global field: key = %s\n", key);
	nelo2_log->delGlobalField(key);
}
static bool read_n(HANDLE named_pipe, char *buffer, int count)
{
	DWORD read_bytes = 0;
	for (DWORD readed = 0; count > 0; count -= readed, read_bytes += readed) {
		if (!ReadFile(named_pipe, buffer + read_bytes, count, &readed, NULL)) {
			return false;
		}
	}
	return true;
}
static message_t *read_message(HANDLE named_pipe, message_t *message)
{
	char *buffer = (char *)malloc(message->total_length);
	if (!buffer) {
		ConsolePrint("malloc buffer failed\n");
		return nullptr;
	}

	if (read_n(named_pipe, buffer + sizeof(message_t), message->total_length - sizeof(message_t))) {
		memcpy(buffer, message, sizeof(message_t));
		return (message_t *)buffer;
	} else {
		ConsolePrint("read message failed\n");
		free(buffer);
		return nullptr;
	}
}
