#include "PLSAddSourceMenuStyle.hpp"
#include "PLSDpiHelper.h"
#include <qdebug.h>

#define ADD_SOURCE_MENU_ICON_SIZE 30

PLSAddSourceMenuStyle::PLSAddSourceMenuStyle(double dpi, QStyle *baseStyle) : QProxyStyle(baseStyle), m_dpi(dpi) {}

int PLSAddSourceMenuStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
	if (metric == QStyle::PM_SmallIconSize)
		return m_dpi * ADD_SOURCE_MENU_ICON_SIZE;
	else
		return QCommonStyle::pixelMetric(metric, option, widget);
}
