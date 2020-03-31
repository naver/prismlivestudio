#pragma once

#include <QDateTime>
#include <QDebug>
#include <QString>
#include <typeinfo>

#include "log.h"
/* this file just for log anchor while log module is in progress */

#define PRE_LOG(x, type)                                     \
	{                                                    \
		PLS_##type("Channels", "Channels :%s ", #x); \
	}

#define PRE_LOG_MSG(x, type)                                \
	{                                                   \
		PLS_##type("Channels", "Channels :%s ", x); \
	}

#define PRE_LOG_UI(x, type)                                       \
	{                                                         \
		PLS_UI_STEP("Channels", typeid(type).name(), #x); \
	}

#define PRE_LOG_UI_MSG(x, type)                                  \
	{                                                        \
		PLS_UI_STEP("Channels", typeid(type).name(), x); \
	}
