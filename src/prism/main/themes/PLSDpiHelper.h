#ifndef PLSDPIHELPER_H
#define PLSDPIHELPER_H

#include <memory>

#include <QList>
#include <QWidget>
#include <QLayout>
#include <QSpacerItem>

#include "PLSThemeManager.h"

class PLSDpiHelperImpl;
class PLSWidgetDpiAdapter;

class FRONTEND_API PLSDpiHelper {
public:
	PLSDpiHelper();
	~PLSDpiHelper();

public:
	PLSDpiHelper(const PLSDpiHelper &helper);
	PLSDpiHelper &operator=(const PLSDpiHelper &helper);

public:
	struct Screen;

public:
	static double getDpi(Screen *screen);
	static double getDpi(QWidget *widget);
	static double getDpi(QWidget *widget, Screen *&screen);
	static int calculate(double dpi, int value);
	static int calculate(QWidget *widget, int value);
	static double calculate(double dpi, double value);
	static double calculate(QWidget *widget, double value);
	static QPoint calculate(double dpi, const QPoint &value);
	static QPoint calculate(QWidget *widget, const QPoint &value);
	static QSize calculate(double dpi, const QSize &value);
	static QSize calculate(QWidget *widget, const QSize &value);
	static QMargins calculate(double dpi, const QMargins &value);
	static QMargins calculate(QWidget *widget, const QMargins &value);

	static void updateLayout(QWidget *widget);
	static void updateLayout(QWidget *widget, double dpi);
	static void updateMinimumSize(QWidget *widget, const QSize &minimumSize, double dpi);
	static void updateMinimumWidth(QWidget *widget, int minimumWidth, double dpi);
	static void updateMinimumHeight(QWidget *widget, int minimumHeight, double dpi);
	static void updateMaximumSize(QWidget *widget, const QSize &maximumSize, double dpi);
	static void updateMaximumWidth(QWidget *widget, int maximumWidth, double dpi);
	static void updateMaximumHeight(QWidget *widget, int maximumHeight, double dpi);
	static void updateContentMargins(QWidget *widget, const QMargins &contentMargins, double dpi);

	static void dpiDynamicUpdate(QWidget *widget, bool updateCss = true);
	static void checkStatusChanged(QWidget *widget, Screen *screen = nullptr);
	static void checkStatusChanged(PLSWidgetDpiAdapter *adapter, Screen *screen = nullptr);
	static void dpiDynamicUpdateBeforeFirstShow(double dpi, QWidget *widget);

public:
	void setUpdateLayout(QWidget *widget, bool updateLayout);

	QSize getInitSize(QWidget *widget) const;
	void setInitSize(QWidget *widget, const QSize &initSize);
	void clearInitSize(QWidget *widget);

	void setFixedWidth(QWidget *widget, int fixedWidth);
	void setFixedHeight(QWidget *widget, int fixedHeight);
	void setFixedSize(QWidget *widget, const QSize &fixedSize);

	void setMinimumWidth(QWidget *widget, int minimumWidth);
	void setMinimumHeight(QWidget *widget, int minimumHeight);
	void setMinimumSize(QWidget *widget, const QSize &minimumSize);

	void setMaximumWidth(QWidget *widget, int maximumWidth);
	void setMaximumHeight(QWidget *widget, int maximumHeight);
	void setMaximumSize(QWidget *widget, const QSize &maximumSize);

	void setContentsMargins(QWidget *widget, const QMargins &margins);

	void setCss(QWidget *widget, const QList<PLSCssIndex> &cssIndexes);

	void setStyleSheet(QWidget *widget, const QString &styleSheet);

	void setDynamicStyleSheet(QWidget *widget, std::function<QString(double, bool)> dynamicStyleSheetCallback);

	void notifyDpiChanged(QWidget *widget, std::function<void()> callback);
	void notifyDpiChanged(QWidget *widget, std::function<void(double dpi)> callback);
	void notifyDpiChanged(QWidget *widget, std::function<void(double dpi, double oldDpi)> callback);
	void notifyDpiChanged(QWidget *widget, std::function<void(double dpi, double oldDpi, bool firstShow)> callback);

	void notifyScreenAvailableGeometryChanged(QWidget *widget, std::function<void(const QRect &screenAvailableGeometry)> callback);

	static void setDynamicMinimumWidth(QWidget *widget, bool isDynamic = true);
	static void setDynamicMinimumHeight(QWidget *widget, bool isDynamic = true);
	static void setDynamicMaximumWidth(QWidget *widget, bool isDynamic = true);
	static void setDynamicMaximumHeight(QWidget *widget, bool isDynamic = true);

	static void setDynamicContentsMargins(QWidget *widget, bool isDynamic = true);
	static void setDynamicContentsMargins(QLayout *layout, bool isDynamic = true);
	static void setDynamicSpacerItem(QLayout *layout, QSpacerItem *spacerItem, bool isDynamic = true);
	static void dynamicDeleteSpacerItem(QLayout *layout, QSpacerItem *spacerItem);

private:
	std::shared_ptr<PLSDpiHelperImpl> impl;
};

#endif // PLSDPIHELPER_H
