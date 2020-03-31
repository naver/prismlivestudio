#pragma once

#include <qglobal.h>

#ifdef FRONTEND_API_LIB
#define FRONTEND_API Q_DECL_EXPORT
#else
#define FRONTEND_API Q_DECL_IMPORT
#endif

#define MODULE_FRONTEND_API "frontend-api"
#define MODULE_FRONTEND_APIW L"frontend-api"
