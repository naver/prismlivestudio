#if !defined(_PRISM_COMMON_LIBLOG_LOG_LOG_H)
#define _PRISM_COMMON_LIBLOG_LOG_LOG_H

#include "../liblog/liblog.h"

#if defined(_PRISM_PLUGIN)
/**
  * print log
  * param:
  *     [in] log_level: log level
  *     [in] file_name: file name
  *     [in] file_line: file line
  *     [in] format: format string
  *     [in] ...: variadic params
  */
void pls_log(pls_log_level_t log_level, const char *file_name, int file_line, const char *format, ...);
/**
  * print log
  * param:
  *     [in] log_level: log level
  *     [in] file_name: file name
  *     [in] file_line: file line
  *     [in] fields: custom nelo log field, example: {{"key1", "value1"}, {"key2", "value2"}}
  *     [in] field_count: custom nelo log field count
  *     [in] format: format string
  *     [in] ...: variadic params
  */
void pls_logex(pls_log_level_t log_level, const char *file_name, int file_line, const char *fields[][2], int field_count, const char *format, ...);
/**
  * print error log
  * param:
  *     [in] file_name: file name
  *     [in] file_line: file line
  *     [in] format: format string
  *     [in] ...: variadic params
  */
void pls_error(const char *file_name, int file_line, const char *format, ...);
/**
  * print warn log
  * param:
  *     [in] file_name: file name
  *     [in] file_line: file line
  *     [in] format: format string
  *     [in] ...: variadic params
  */
void pls_warn(const char *file_name, int file_line, const char *format, ...);
/**
  * print info log
  * param:
  *     [in] file_name: file name
  *     [in] file_line: file line
  *     [in] format: format string
  *     [in] ...: variadic params
  */
void pls_info(const char *file_name, int file_line, const char *format, ...);
/**
  * print debug log
  * param:
  *     [in] file_name: file name
  *     [in] file_line: file line
  *     [in] format: format string
  *     [in] ...: variadic params
  */
void pls_debug(const char *file_name, int file_line, const char *format, ...);
/**
  * print action log
  * param:
  *     [in] controls: trigger controls
  *     [in] action: trigger action
  *     [in] file_name: file name
  *     [in] file_line: file line
  */
void pls_action_log(const char *controls, const char *action, const char *file_name, int file_line);
/**
  * print ui step log
  * param:
  *     [in] controls: trigger controls
  *     [in] action: trigger action
  *     [in] file_name: file name
  *     [in] file_line: file line
  */
void pls_ui_step(const char *controls, const char *action, const char *file_name, int file_line);

/**
  * print log
  * param:
  *     [in] log_level: log level
  *     [in] format: format string
  *     [in] ...: variadic params
  */
#define PLS_PLUGIN_LOG(log_level, format, ...) pls_log(log_level, __FILE__, __LINE__, format, __VA_ARGS__)
/**
  * print log
  * param:
  *     [in] log_level: log level
  *     [in] fields: custom nelo log field, example: {{"key1", "value1"}, {"key2", "value2"}}
  *     [in] field_count: custom nelo log field count
  *     [in] format: format string
  *     [in] ...: variadic params
  */
#define PLS_PLUGIN_LOGEX(log_level, fields, field_count, format, ...) pls_logex(log_level, __FILE__, __LINE__, fields, field_count, format, __VA_ARGS__)
/**
  * print error log
  * param:
  *     [in] format: format string
  *     [in] ...: variadic params
  */
#define PLS_PLUGIN_ERROR(format, ...) pls_error(__FILE__, __LINE__, format, __VA_ARGS__)
/**
  * print warn log
  * param:
  *     [in] format: format string
  *     [in] ...: variadic params
  */
#define PLS_PLUGIN_WARN(format, ...) pls_warn(__FILE__, __LINE__, format, __VA_ARGS__)
/**
  * print info log
  * param:
  *     [in] format: format string
  *     [in] ...: variadic params
  */
#define PLS_PLUGIN_INFO(format, ...) pls_info(__FILE__, __LINE__, format, __VA_ARGS__)
/**
  * print debug log
  * param:
  *     [in] format: format string
  *     [in] ...: variadic params
  */
#define PLS_PLUGIN_DEBUG(format, ...) pls_debug(__FILE__, __LINE__, format, __VA_ARGS__)
/**
  * print action log
  * param:
  *     [in] controls: trigger controls
  *     [in] action: trigger action
  */
#define PLS_PLUGIN_ACTION_LOG(controls, action) pls_action_log(controls, action, __FILE__, __LINE__)
/**
  * print ui step log
  * param:
  *     [in] controls: trigger controls
  *     [in] action: trigger action
  */
#define PLS_PLUGIN_UI_STEP(controls, action) pls_ui_step(controls, action, __FILE__, __LINE__)
#endif // _PRISM_PLUGIN

#endif // _PRISM_COMMON_LIBLOG_LOG_LOG_H
