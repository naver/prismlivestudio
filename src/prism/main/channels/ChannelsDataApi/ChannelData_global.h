#pragma once
#include <QtCore/qglobal.h>

#if defined(CHANNEL_DATA_LIBRARY)
#define CHANNEL_DATA_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#define CHANNEL_DATA_LIBRARY_EXPORT Q_DECL_IMPORT
#endif
