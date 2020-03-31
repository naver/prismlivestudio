#include "log.h"

#include <qglobal.h>

#include "ui-config.h"

static log_handler_t log_handler = nullptr;
static void *log_param = nullptr;

static int to_obs_level(pls_log_level_t log_level)
{
	switch (log_level) {
	case PLS_LOG_ERROR:
		return LOG_ERROR;
	case PLS_LOG_WARN:
		return LOG_WARNING;
	case PLS_LOG_INFO:
		return LOG_INFO;
	case PLS_LOG_DEBUG:
	default:
		return LOG_DEBUG;
	}
}
static void call_obs_handler(pls_log_level_t log_level, const char *module_name, uint32_t tid, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	const char *formats[] = {module_name, format, (const char *)&tid};
	log_handler(to_obs_level(log_level), (const char *)(void *)&formats[0], args, log_param);
	va_end(args);
}

static void def_obs_log_handler(int log_level, const char *format, va_list args, void *)
{
	switch (log_level) {
	case LOG_ERROR:
		pls_logva(PLS_LOG_ERROR, "obs", nullptr, 0, format, args);
		break;
	case LOG_WARNING:
		pls_logva(PLS_LOG_WARN, "obs", nullptr, 0, format, args);
		break;
	case LOG_INFO:
		pls_logva(PLS_LOG_INFO, "obs", nullptr, 0, format, args);
		break;
	case LOG_DEBUG:
		pls_logva(PLS_LOG_DEBUG, "obs", nullptr, 0, format, args);
		break;
	}
}
static void def_obs_crash_handler(const char *, va_list, void *) {}

static void def_pls_log_handler(pls_log_level_t log_level, const char *module_name, const pls_time_t &time, uint32_t tid, const char *file_name, int file_line, const char *message, void *param)
{
	Q_UNUSED(module_name)
	Q_UNUSED(time)
	Q_UNUSED(tid)
	Q_UNUSED(file_name)
	Q_UNUSED(file_line)
	Q_UNUSED(param)

	call_obs_handler(log_level, module_name, tid, "%s", message);
}
static void def_pls_action_log_handler(const char *module_name, const pls_time_t &time, uint32_t tid, const char *controls, const char *action, const char *file_name, int file_line, void *param)
{
	Q_UNUSED(module_name)
	Q_UNUSED(time)
	Q_UNUSED(tid)
	Q_UNUSED(controls)
	Q_UNUSED(action)
	Q_UNUSED(file_name)
	Q_UNUSED(file_line)
	Q_UNUSED(param)
}

bool log_init(const char *session_id)
{
	base_get_log_handler(&log_handler, &log_param);
	base_set_log_handler(def_obs_log_handler, nullptr);

	base_set_crash_handler(def_obs_crash_handler, nullptr);

	pls_log_init("PRISMLiveStudio", PLS_VERSION, "prism-log");
	pls_add_global_field("prismSession", session_id);

	pls_set_log_handler(def_pls_log_handler, nullptr);
	pls_set_action_log_handler(def_pls_action_log_handler, nullptr);
	return true;
}
void log_cleanup()
{
	pls_reset_log_handler();
	pls_reset_action_log_handler();
	pls_log_cleanup();
}

void set_log_handler(log_handler_t handler, void *param)
{
	log_handler = handler;
	log_param = param;
}
