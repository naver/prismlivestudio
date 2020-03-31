#ifndef __NELO2__CPPSDK__H__
#define __NELO2__CPPSDK__H__

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <crtdbg.h>
#if (defined(NELO2_DYNAMIC_MODE) || defined(NELO2_CLANG_MODE))
#define NELO2_API
#else
#ifdef NELO2_EXPORT_API
#define NELO2_API __declspec(dllexport)
#else
#define NELO2_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "libnelo2d.lib")
#else
#pragma comment(lib, "libnelo2.lib")
#endif
#endif
#endif
#else
#define NELO2_API
#endif

//init function error code list
#define NELO2_OK 0
#define NELO2_ERROR -1
#define INVALID_PROJECT -2
#define INVALID_VERSION -3
#define INVALID_SERVER -4
#define INVALID_PORT -5

//default variant
#define NELO2_DEF_PROJECTVERSION "1.0.0"
#ifdef WIN32
#define NELO2_DEF_LOGSOURCE "nelo2-windows"
#else
#ifdef ANDROID
#define NELO2_DEF_LOGSOURCE "nelo2-AndroidNDK"
#else
#define NELO2_DEF_LOGSOURCE "nelo2-linux"
#endif
#endif
#define NELO2_DEF_LOGTYPE "nelo2-log"
#define NELO2_DEF_SERVERHOST "nelo2-col.nhncorp.com"
#define NELO2_DEF_SERVERPORT 80

//log level
typedef enum _NELO_LOG_LEVEL {
	NELO_LL_FATAL = 0,
	NELO_LL_ERROR = 3,
	NELO_LL_WARN = 4,
	NELO_LL_INFO = 5,
	NELO_LL_DEBUG = 7
#define NELO_LL_TRACE NELO_LL_DEBUG
} NELO_LOG_LEVEL;

//language support
typedef enum {
	NELO_LANG_DEFAULT = 0,
	NELO_LANG_ENGLISH = 1,
	NELO_LANG_KOREAN = 2,
	NELO_LANG_JAPANESE = 3,
	NELO_LANG_CHINESE = 4,
	NELO_LANG_CHINESE_TRADITIONAL = 5,
#define NELO_LANG_CHINESE_SIMPLIFIED NELO_LANG_CHINESE
} NELO_LANG_TYPE;

////////////////////////////////////////////////////////////////////////////////
//BEGIN: C interface
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//crash callback function type
#ifdef WIN32
typedef void(__cdecl *crash_func_type)(void *content);
#else
typedef void (*crash_func_type)(void *content);
#endif

//init/destory function
//[WARNING]: please call destory() function before you close your application.
NELO2_API int initialize(const char *projectName, const char *projectVersion = NELO2_DEF_PROJECTVERSION, const char *logSource = NELO2_DEF_LOGSOURCE, const char *logType = NELO2_DEF_LOGTYPE,
			 const char *serverAddr = NELO2_DEF_SERVERHOST, const int serverPort = NELO2_DEF_SERVERPORT, const int https = 0);
NELO2_API void destory();

#ifdef WIN32
typedef int(__cdecl *initialize_func_type)(const char *, const char *, const char *, const char *, const char *, const int, const int);
typedef void(__cdecl *destory_func_type)();
#endif

//enable/disable collect the host information
NELO2_API void enableHostField();
NELO2_API void disableHostField();

#ifdef WIN32
typedef void(__cdecl *enableHostField_func_type)();
typedef void(__cdecl *disableHostField_func_type)();
#endif

//enable/disable collect the platform information
NELO2_API void enablePlatformField();
NELO2_API void disablePlatformField();

#ifdef WIN32
typedef void(__cdecl *enablePlatformField_func_type)();
typedef void(__cdecl *disablePlatformField_func_type)();
#endif

//add/delete custom field
NELO2_API bool addField(const char *key, const char *val);
NELO2_API void removeField(const char *key);
NELO2_API void removeAllFields();

#ifdef WIN32
typedef bool(__cdecl *addField_func_type)(const char *, const char *);
typedef void(__cdecl *removeField_func_type)(const char *);
typedef void(__cdecl *removeAllFields_func_type)();
#endif

//get/set log level function, the default log level is NELO_LL_INFO
NELO2_API NELO_LOG_LEVEL getLogLevel();
NELO2_API void setLogLevel(const NELO_LOG_LEVEL eLevel = NELO_LL_INFO);

#ifdef WIN32
typedef NELO_LOG_LEVEL(__cdecl *getLogLevel_func_type)();
typedef void(__cdecl *setLogLevel_func_type)(const NELO_LOG_LEVEL);
#endif

//get/set user id function
NELO2_API const char *getUserId();
NELO2_API void setUserId(const char *userID);

#ifdef WIN32
typedef const char *(__cdecl *getUserId_func_type)();
typedef void(__cdecl *setUserId_func_type)(const char *);
#endif

//set crash report UI window
NELO2_API bool openCrashCatcher(const bool bBackground = false, const NELO_LANG_TYPE eLangType = NELO_LANG_DEFAULT);
NELO2_API void setCrashCallback(const crash_func_type fnCrashCallback, void *content = NULL);
NELO2_API void closeCrashCatcher();

#ifdef WIN32
typedef bool(__cdecl *openCrashCatcher_func_type)(const bool, const NELO_LANG_TYPE);
typedef void(__cdecl *setCrashCallback_func_type)(const crash_func_type, void *content);
typedef void(__cdecl *closeCrashCatcher_func_type)();
#endif

//send log messages
NELO2_API bool sendLog(const NELO_LOG_LEVEL eLevel, const char *strMsg);
NELO2_API bool sendLogEx(const time_t tUTCTime, const NELO_LOG_LEVEL eLevel, const char *strMsg);

#ifdef WIN32
typedef bool(__cdecl *sendLog_func_type)(const NELO_LOG_LEVEL, const char *);
typedef bool(__cdecl *sendLogEx_func_type)(const time_t, const NELO_LOG_LEVEL, const char *);
#endif

//send file
NELO2_API bool sendFile(const char *filePath, const char *customMsg = NULL);
NELO2_API bool sendFileA(const char *filePath, const char *customMsg = NULL);
NELO2_API bool sendFileW(const wchar_t *filePath, const wchar_t *customMsg = NULL);

#ifdef WIN32
typedef bool(__cdecl *sendFile_func_type)(const char *, const char *);
typedef bool(__cdecl *sendFileA_func_type)(const char *, const char *);
typedef bool(__cdecl *sendFileW_func_type)(const wchar_t *, const wchar_t *);
#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
//END: C interface
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//BEGIN: C++ interface
#if !defined(NELO2_DYNAMIC_MODE)
class NELO2_API NELO2Log {
public:
	//custom field type define
	class NELO2_API CustomField {
	public:
		CustomField();
		~CustomField();

	public:
		bool addField(const char *key, const char *value);

		void delField(const char *key);

		void clearAll();

	private:
		friend class NELO2Log;
		void *m_customFiled;
	};

	////////////////////////////////////////////////////////////////////////////
private:
	void *m_pIniter;
	void *m_pCustom;
	void *m_pLocker;
	void *m_pSender;
	NELO_LOG_LEVEL m_eLevel;

public:
	NELO2Log();

	~NELO2Log();

private:
	NELO2Log(const NELO2Log &rhs);
	NELO2Log &operator=(const NELO2Log &rhs);

public:
	//return code:
	//  NELO2_OK: initial resources is successful.
	//  INVALID_PROJECT: the project name is empty
	//  INVALID_SERVER: the server's address is empty
	//  INVALID_PORT: the server's port is invalid
	int initialize(const char *projectName, const char *projectVersion = NELO2_DEF_PROJECTVERSION, const char *logSource = NELO2_DEF_LOGSOURCE, const char *logType = NELO2_DEF_LOGTYPE,
		       const char *serverAddr = NELO2_DEF_SERVERHOST, const int serverPort = NELO2_DEF_SERVERPORT, const bool https = false);

	//[WARNING]: you must call destory() before exit your application
	void destory();

public:
	//enable/disable collect the host information
	void enableHostField();
	void disableHostField();

	//enable/disable collect the platform information
	void enablePlatformField();
	void disablePlatformField();

public:
	//open crash catch function and set crash report's cache path
	//return true: open crash catch function is successful.
	//      false: open crash catch function is failed, maybe can't access the cache path.
	bool openCrashCatcher(const bool bBackground = false, const NELO_LANG_TYPE eLangType = NELO_LANG_DEFAULT);

	//call the callback function when crash is happened and user already set it.
	void setCrashCallback(const crash_func_type fnCrashCallback, void *content = NULL);

	//close crash catch function
	void closeCrashCatcher();

public:
	//add global custom field
	bool addGlobalField(const char *key, const char *value);

	//delete a specific field
	void delGlobalField(const char *key);

	//clear all global custom fields
	void clearGlobalFields();

public:
	//set send log level, if priority of log level is higher than set value, the log can send to nelo2 system, if less, don't send it
	// default value is INFO
	void setLogLevel(const NELO_LOG_LEVEL eLevel = NELO_LL_INFO);

	//get currently log level
	NELO_LOG_LEVEL getLogLevel();

	//get user id
	const char *getUserId();

	//set user id
	void setUserId(const char *userID);

public:
	//true:  send log message is successful
	//false: send log message is failed
	//[WARNING]: SDK will use system UTC time as logTime
	bool sendLog(const NELO_LOG_LEVEL eLevel, const char *strMsg);
	bool sendLog(const NELO_LOG_LEVEL eLevel, const char *strMsg, const CustomField &customFields);

public:
	//true:  send log message is successful
	//false: send log message is failed
	//[WARNING]: you can set log time, but the tUTCTime must is second/millisecond level
	bool sendLog(const time_t tUTCTime, const NELO_LOG_LEVEL eLevel, const char *strMsg);
	bool sendLog(const time_t tUTCTime, const NELO_LOG_LEVEL eLevel, const char *strMsg, const CustomField &customFields);

public:
	//true:  send file is successful
	//false: send file is failed
	bool sendFile(const char *filePath, const char *customMsg = NULL);
	bool sendFileA(const char *filePath, const char *customMsg = NULL);
	bool sendFileW(const wchar_t *filePath, const wchar_t *customMsg = NULL);

public:
	//the value come from property of android's [ro.build.version.release]
	void setAndroidVersion(const char *osVersion);

	//the value come from property of android's [ro.product.model]
	void setAndroidProductModel(const char *osModel);

	//the value come from property of android's [ro.product.locale.region]
	void setAndroidLocaleRegion(const char *osRegion);

	//the value come from property of android's [ro.product.locale.language]
	void setAndroidLocaleLanguage(const char *osLanguage);

public:
#ifdef WIN32
	static void handleWindowsPureVitualCall();

	static void handleWindowsInvalidParameter(const wchar_t *expression, const wchar_t *function, const wchar_t *file, unsigned int line, uintptr_t reserved);
#endif
};

#ifdef WIN32
#define INIT_NELO2_WINDOWS_ENVIRONMENT                                                   \
	_CrtSetReportMode(_CRT_ASSERT, 0);                                               \
	_set_purecall_handler((_purecall_handler)NELO2Log::handleWindowsPureVitualCall); \
	_set_invalid_parameter_handler((_invalid_parameter_handler)NELO2Log::handleWindowsInvalidParameter);
#else
#define INIT_NELO2_WINDOWS_ENVIRONMENT
#endif

#if defined(__cplusplus) || defined(c_plusplus)
class Nelo2WindowsIniter {
public:
	Nelo2WindowsIniter()
	{
#ifdef WIN32
		_CrtSetReportMode(_CRT_ASSERT, 0);
		_set_purecall_handler((_purecall_handler)NELO2Log::handleWindowsPureVitualCall);
		_set_invalid_parameter_handler((_invalid_parameter_handler)NELO2Log::handleWindowsInvalidParameter);
#endif
	}
};
static Nelo2WindowsIniter __global__nelo2__initer__;
#endif

#endif
//END: C++ interface
////////////////////////////////////////////////////////////////////////////////

#endif //__NELO2__CPPSDK__H__
