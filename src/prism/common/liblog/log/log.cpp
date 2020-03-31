#include "log.h"

#include <obs-module.h>

void pls_log(pls_log_level_t log_level, const char *file_name, int file_line,
	     const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(log_level, obs_module_name(), file_name, file_line, format,
		  args);
	va_end(args);
}
void pls_logex(pls_log_level_t log_level, const char *file_name, int file_line,
	       const char *fields[][2], int field_count, const char *format,
	       ...)
{
	va_list args;
	va_start(args, format);
	pls_logvaex(log_level, obs_module_name(), file_name, file_line, fields,
		    field_count, format, args);
	va_end(args);
}
void pls_error(const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_ERROR, obs_module_name(), file_name, file_line,
		  format, args);
	va_end(args);
}
void pls_warn(const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_WARN, obs_module_name(), file_name, file_line, format,
		  args);
	va_end(args);
}
void pls_info(const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_INFO, obs_module_name(), file_name, file_line, format,
		  args);
	va_end(args);
}
void pls_debug(const char *file_name, int file_line, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	pls_logva(PLS_LOG_DEBUG, obs_module_name(), file_name, file_line,
		  format, args);
	va_end(args);
}
void pls_action_log(const char *controls, const char *action,
		    const char *file_name, int file_line)
{
	pls_action_log(obs_module_name(), controls, action, file_name,
		       file_line);
}
void pls_ui_step(const char *controls, const char *action,
		 const char *file_name, int file_line)
{
	pls_ui_step(obs_module_name(), controls, action, file_name, file_line);
}
