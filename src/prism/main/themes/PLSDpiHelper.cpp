#include "PLSDpiHelper.h"

#include <map>
#include <tuple>

#include <QMetaEnum>
#include <QFile>
#include <QScreen>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QDockWidget>

#include <Windows.h>

#include "PLSToplevelView.hpp"
#include "log.h"
#include "frontend-api.h"

// #define DUMP_LAYOUT_PROCESS_DETAILS

#define CSTR_DPI "dpi"

#define CSTR_HDPI_ORIGINAL "__hdpi_original_"
#define CSTR_HDPI_ORIGINAL_SPACER "__hdpi_original_spacer_"
#define CSTR_CONTENTS_MARGINS "contentsMargins"
#define CSTR_SPACING "spacing"
#define CSTR_HORIZONTAL_SPACING "horizontalSpacing"
#define CSTR_VERTICAL_SPACING "verticalSpacing"
#define CSTR_ROW_MINIMUM_HEIGHT "rowMinimumHeight"
#define CSTR_COLUMN_MINIMUM_HEIGHT "columnMinimumHeight"
#define CSTR_EVENT_FILTER_REMOVED "eventFilterRemoved_"
#define CSTR_DYNAMIC_MINIMUM_WIDTH "dynamic_minimumWidth"
#define CSTR_DYNAMIC_MINIMUM_HEIGHT "dynamic_minimumHeight"
#define CSTR_DYNAMIC_MAXIMUM_WIDTH "dynamic_maximumWidth"
#define CSTR_DYNAMIC_MAXIMUM_HEIGHT "dynamic_maximumHeight"
#define CSTR_DYNAMIC_CONTENTS_MARGINS "dynamic_contentsMargins"
#define CSTR_HDPI_ORIGINAL_SPACER_DYNAMIC "__hdpi_original_spacer_dynamic_"

extern double getMonitorDpi(HMONITOR monitor);
extern HMONITOR getMonitor(PLSWidgetDpiAdapter *adapter);
extern HMONITOR getMonitor(QWidget *widget);

PLSWidgetDpiAdapter *getDpiAdapter(QWidget *widget)
{
	for (; widget; widget = widget->parentWidget()) {
		if (PLSWidgetDpiAdapter *dpiAdapter = dynamic_cast<PLSWidgetDpiAdapter *>(widget); dpiAdapter) {
			return dpiAdapter;
		}
	}
	return nullptr;
}

namespace {
template<typename T> T getProperty(QObject *object, const char *name, const T &defaultValue)
{
	if (object->dynamicPropertyNames().contains(name)) {
		return object->property(name).value<T>();
	}
	return defaultValue;
}
template<typename T> void setProperty(QObject *object, const char *name, const T &value)
{
	object->setProperty(name, QVariant::fromValue(value));
}

template<typename T> T getAndSetProperty(QObject *object, const char *name, const T &defaultValue)
{
	if (object->dynamicPropertyNames().contains(name)) {
		return object->property(name).value<T>();
	}

	object->setProperty(name, QVariant::fromValue(defaultValue));
	return defaultValue;
}

template<typename T> inline void setContentsMargins(T *object, double dpi)
{
	if (QMargins contentsMargins = object->contentsMargins(); !contentsMargins.isNull() && !getProperty(object, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_CONTENTS_MARGINS, false)) {
		contentsMargins = getAndSetProperty(object, CSTR_HDPI_ORIGINAL CSTR_CONTENTS_MARGINS, contentsMargins);
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << "setContentsMargins" << object->metaObject()->className() << object->objectName() << "contentsMargins" << PLSDpiHelper::calculate(dpi, contentsMargins);
#endif
		object->setContentsMargins(PLSDpiHelper::calculate(dpi, contentsMargins));
	}
}

template<typename T> inline void setSpacing(T *object, double dpi)
{
	if (int spacing = object->spacing(); spacing > 0) {
		spacing = getAndSetProperty(object, CSTR_HDPI_ORIGINAL CSTR_SPACING, spacing);
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << "setSpacing" << object->metaObject()->className() << object->objectName() << "spacing" << PLSDpiHelper::calculate(dpi, spacing);
#endif
		object->setSpacing(PLSDpiHelper::calculate(dpi, spacing));
	}
}

template<typename T> inline void setHorizontalSpacing(T *object, double dpi)
{
	if (int horizontalSpacing = object->horizontalSpacing(); horizontalSpacing > 0) {
		horizontalSpacing = getAndSetProperty(object, CSTR_HDPI_ORIGINAL CSTR_HORIZONTAL_SPACING, horizontalSpacing);
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << "setHorizontalSpacing" << object->metaObject()->className() << object->objectName() << "horizontalSpacing" << PLSDpiHelper::calculate(dpi, horizontalSpacing);
#endif
		object->setHorizontalSpacing(PLSDpiHelper::calculate(dpi, horizontalSpacing));
	}
}

template<typename T> inline void setVerticalSpacing(T *object, double dpi)
{
	if (int verticalSpacing = object->verticalSpacing(); verticalSpacing > 0) {
		verticalSpacing = getAndSetProperty(object, CSTR_HDPI_ORIGINAL CSTR_VERTICAL_SPACING, verticalSpacing);
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << "setVerticalSpacing" << object->metaObject()->className() << object->objectName() << "verticalSpacing" << PLSDpiHelper::calculate(dpi, verticalSpacing);
#endif
		object->setVerticalSpacing(PLSDpiHelper::calculate(dpi, verticalSpacing));
	}
}

template<typename T> inline void setRowMinimumHeight(T *object, double dpi, int row)
{
	if (int rowMinimumHeight = object->rowMinimumHeight(row); rowMinimumHeight > 0) {
		rowMinimumHeight = getAndSetProperty(object, CSTR_HDPI_ORIGINAL CSTR_ROW_MINIMUM_HEIGHT, rowMinimumHeight);
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << "setRowMinimumHeight" << object->metaObject()->className() << object->objectName() << "rowMinimumHeight" << row << PLSDpiHelper::calculate(dpi, rowMinimumHeight);
#endif
		object->setRowMinimumHeight(row, PLSDpiHelper::calculate(dpi, rowMinimumHeight));
	}
}

template<typename T> inline void setColumnMinimumWidth(T *object, double dpi, int column)
{
	if (int columnMinimumWidth = object->columnMinimumWidth(column); columnMinimumWidth > 0) {
		columnMinimumWidth = getAndSetProperty(object, CSTR_HDPI_ORIGINAL CSTR_COLUMN_MINIMUM_HEIGHT, columnMinimumWidth);
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << "setColumnMinimumWidth" << object->metaObject()->className() << object->objectName() << "columnMinimumWidth" << column << PLSDpiHelper::calculate(dpi, columnMinimumWidth);
#endif
		object->setColumnMinimumWidth(column, PLSDpiHelper::calculate(dpi, columnMinimumWidth));
	}
}

void setSpacerItem(QLayout *layout, QSpacerItem *spacerItem, double dpi)
{
	char name[128];
	sprintf_s(name, CSTR_HDPI_ORIGINAL_SPACER_DYNAMIC "%p", spacerItem);
	if (!getProperty(layout, name, false)) {
		sprintf_s(name, CSTR_HDPI_ORIGINAL_SPACER "%p", spacerItem);
		QSize originalSize = getAndSetProperty(layout, name, spacerItem->sizeHint());
		if (!originalSize.isValid()) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
			qInfo() << "setSpacerItem" << layout->metaObject()->className() << layout->objectName() << "spacerItem originalSize is empty" << (void *)spacerItem;
#endif
			originalSize = spacerItem->sizeHint();
			setProperty(layout, name, originalSize);
		}

#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << "setSpacerItem" << layout->metaObject()->className() << layout->objectName() << "spacerItem" << (void *)spacerItem << PLSDpiHelper::calculate(dpi, originalSize);
#endif
		QSize newSize = PLSDpiHelper::calculate(dpi, originalSize);
		QSizePolicy sizePolicy = spacerItem->sizePolicy();
		spacerItem->changeSize(newSize.width(), newSize.height(), sizePolicy.horizontalPolicy(), sizePolicy.verticalPolicy());
	}
}

void processLayout(QLayout *layout, double dpi);
void processLayoutItems(QLayout *layout, double dpi);
void processWidget(QWidget *widget, double dpi);

void processLayout(QLayout *layout, double dpi)
{
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
	qInfo() << "processLayout" << layout->metaObject()->className() << layout->objectName();
#endif

	processLayoutItems(layout, dpi);
	setContentsMargins(layout, dpi);
	if (QBoxLayout *bl = dynamic_cast<QBoxLayout *>(layout); bl) {
		setSpacing(bl, dpi);
	} else if (QGridLayout *gl = dynamic_cast<QGridLayout *>(layout); gl) {
		setHorizontalSpacing(gl, dpi);
		setVerticalSpacing(gl, dpi);
		for (int row = 0; row < gl->rowCount(); ++row) {
			setRowMinimumHeight(gl, dpi, row);
		}
		for (int column = 0; column < gl->columnCount(); ++column) {
			setColumnMinimumWidth(gl, dpi, column);
		}
	} else if (QFormLayout *fl = dynamic_cast<QFormLayout *>(layout); fl) {
		setHorizontalSpacing(fl, dpi);
		setVerticalSpacing(fl, dpi);
	}
}
void processLayoutItems(QLayout *layout, double dpi)
{
	for (int i = 0, count = layout->count(); i < count; ++i) {
		QLayoutItem *li = layout->itemAt(i);
		if (QSpacerItem *si = dynamic_cast<QSpacerItem *>(li)) {
			setSpacerItem(layout, si, dpi);
		} else if (QLayout *l = dynamic_cast<QLayout *>(li); l) {
			processLayout(l, dpi);
		}
	}
}
void processWidget(QWidget *widget, double dpi)
{
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
	qInfo() << "processWidget" << widget->metaObject()->className() << widget->objectName();
#endif

	for (QObject *child : widget->children()) {
		if (child->isWidgetType()) {
			if (QWidget *childWidget = dynamic_cast<QWidget *>(child); childWidget && !childWidget->isWindow()) {
				processWidget(childWidget, dpi);
			}
		} else if (QLayout *l = dynamic_cast<QLayout *>(child); l) {
			processLayout(l, dpi);
		}
	}
}

QWidget *getNotNullToplevelWidget(QWidget *widget)
{
	for (QWidget *parent = widget; parent; parent = parent->parentWidget()) {
		widget = parent;
	}
	return widget;
}

inline double getScreenDpi(PLSDpiHelper::Screen *screen)
{
	return getMonitorDpi((HMONITOR)screen);
}

inline double getScreenDpi(PLSWidgetDpiAdapter *dpiAdapter, PLSDpiHelper::Screen *&screen)
{
	screen = (PLSDpiHelper::Screen *)getMonitor(dpiAdapter);
	return getScreenDpi(screen);
}

inline double getWidgetDpi(QWidget *widget, PLSDpiHelper::Screen *&screen)
{
	auto dpiAdapter = getDpiAdapter(widget);
	if (!dpiAdapter) {
		return getScreenDpi(screen = (PLSDpiHelper::Screen *)getMonitor(widget));
	}

	QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(dpiAdapter->selfWidget());
	if (!dockWidget) {
		return getScreenDpi(dpiAdapter, screen);
	} else if (dockWidget->isFloating()) {
		return getScreenDpi(screen = (PLSDpiHelper::Screen *)getMonitor(dockWidget));
	} else {
		return getWidgetDpi(dockWidget->parentWidget(), screen);
	}
}

inline double hdpiCalculate(double value, double dpi)
{
	return value * dpi;
}

inline int hdpiCalculate(int value, double dpi)
{
	return qRound(value * dpi);
}
}

class PLSDpiHelperImpl {
	using WidgetInfo = std::tuple<std::pair<bool, bool>,                                             // updateLayout
				      std::pair<bool, QSize>,                                            // initSize
				      std::pair<std::pair<bool, int>, std::pair<bool, int>>,             // minimumSize
				      std::pair<std::pair<bool, int>, std::pair<bool, int>>,             // maximumSize
				      std::pair<bool, QMargins>,                                         // contentMargins
				      std::pair<bool, QList<PLSCssIndex>>,                               // cssIndexes
				      std::pair<bool, QString>,                                          // styleSheet
				      std::pair<bool, std::function<QString(double, bool)>>,             // dynamicStyleSheet
				      std::pair<bool, QList<std::function<void(double, double, bool)>>>, // dpiChangedCallbacks
				      std::pair<bool, QList<std::function<void(const QRect &)>>>         // screenAvailableGeometryChangedCallbacks
				      >;
	using Widgets = std::map<QWidget *, WidgetInfo>; // widget

public:
	PLSDpiHelperImpl() : widgets() {}
	~PLSDpiHelperImpl()
	{
		for (auto iter = widgets.begin(), endIter = widgets.end(); iter != endIter; ++iter) {
			dpiHelper(iter->first, iter->second);
		}
	}

public:
	void setUpdateLayout(QWidget *widget, bool updateLayout) { std::get<0>(getInfo(widget)) = {true, updateLayout}; }

	QSize getInitSize(QWidget *widget) const { return std::get<1>(getInfo(widget)).second; }
	void setInitSize(QWidget *widget, const QSize &initSize) { std::get<1>(getInfo(widget)) = {true, initSize}; }
	void clearInitSize(QWidget *widget) { std::get<1>(getInfo(widget)) = {false, {}}; }

	void setMinimumWidth(QWidget *widget, int minimumWidth) { std::get<2>(getInfo(widget)).first = {true, minimumWidth}; }
	void setMinimumHeight(QWidget *widget, int minimumHeight) { std::get<2>(getInfo(widget)).second = {true, minimumHeight}; }

	void setMaximumWidth(QWidget *widget, int maximumWidth) { std::get<3>(getInfo(widget)).first = {true, maximumWidth}; }
	void setMaximumHeight(QWidget *widget, int maximumHeight) { std::get<3>(getInfo(widget)).second = {true, maximumHeight}; }

	void setContentsMargins(QWidget *widget, const QMargins &margins) { std::get<4>(getInfo(widget)) = {true, margins}; }

	void setCss(QWidget *widget, const QList<PLSCssIndex> &cssIndexes) { mergeCss(std::get<5>(getInfo(widget)), cssIndexes); }

	void setStyleSheet(QWidget *widget, const QString &styleSheet) { std::get<6>(getInfo(widget)) = {true, styleSheet}; }

	void setDynamicStyleSheet(QWidget *widget, std::function<QString(double, bool)> dynamicStyleSheetCallback) { std::get<7>(getInfo(widget)) = {true, dynamicStyleSheetCallback}; }

	void notifyDpiChanged(QWidget *widget, std::function<void(double, double, bool)> callback)
	{
		auto &item = std::get<8>(getInfo(widget));
		item.first = true;
		item.second.append(callback);
	}

	void notifyScreenAvailableGeometryChanged(QWidget *widget, std::function<void(const QRect &screenAvailableGeometry)> callback)
	{
		auto &item = std::get<9>(getInfo(widget));
		item.first = true;
		item.second.append(callback);
	}

private:
	WidgetInfo &getInfo(QWidget *widget) const
	{
		if (auto iter = widgets.find(widget); iter == widgets.end()) {
			return widgets.insert(Widgets::value_type(widget, WidgetInfo())).first->second;
		} else {
			return iter->second;
		}
	}
	void dpiHelper(QWidget *widget, const WidgetInfo &widgetInfo)
	{
		PLSThemeManager *manager = PLS_THEME_MANAGER;

		if (PLSWidgetDpiAdapter *dpiAdapter = getDpiAdapter(widget); dpiAdapter) {
			PLS_DEBUG(CSTR_DPI, "widget dpi adapter listen. widget class name: %s, toplevel: %s", widget->metaObject()->className(), dpiAdapter->selfWidget()->metaObject()->className());

			auto dpiAdapterInfo = dpiAdapter->getWidgetDpiAdapterInfo(widget);

			const auto &updateLayout = std::get<0>(widgetInfo);
			if (updateLayout.first) {
				dpiAdapterInfo->updateLayout = updateLayout;
			}

			const auto &initSize = std::get<1>(widgetInfo);
			if (initSize.first) {
				dpiAdapterInfo->initSize = initSize;
			}

			const auto &minimumSize = std::get<2>(widgetInfo);
			if (minimumSize.first.first) {
				dpiAdapterInfo->minimumSize.first = minimumSize.first;
			}
			if (minimumSize.second.first) {
				dpiAdapterInfo->minimumSize.second = minimumSize.second;
			}

			const auto &maximumSize = std::get<3>(widgetInfo);
			if (maximumSize.first.first) {
				dpiAdapterInfo->maximumSize.first = maximumSize.first;
			}
			if (maximumSize.second.first) {
				dpiAdapterInfo->maximumSize.second = maximumSize.second;
			}

			const auto &contentMargins = std::get<4>(widgetInfo);
			if (contentMargins.first) {
				dpiAdapterInfo->contentMargins = contentMargins;
			}

			const auto &cssIndexes = std::get<5>(widgetInfo);
			if (cssIndexes.first) {
				mergeCss(dpiAdapterInfo->cssIndexes, cssIndexes.second);
			}

			const auto &styleSheet = std::get<6>(widgetInfo);
			if (styleSheet.first) {
				dpiAdapterInfo->styleSheet = styleSheet.second;
			}

			const auto &dynamicStyleSheet = std::get<7>(widgetInfo);
			if (dynamicStyleSheet.first) {
				dpiAdapterInfo->dynamicStyleSheetCallback = dynamicStyleSheet.second;
			}

			const auto &callbacks = std::get<8>(widgetInfo);
			if (callbacks.first) {
				dpiAdapterInfo->callbacks.append(callbacks.second);
			}

			const auto &sagcCallbacks = std::get<9>(widgetInfo);
			if (sagcCallbacks.first) {
				dpiAdapterInfo->sagcCallbacks.append(sagcCallbacks.second);
			}

			if (QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(dpiAdapter->selfWidget()); dockWidget) {
				if (PLSWidgetDpiAdapter *mainDpiAdapter = getDpiAdapter(dockWidget->parentWidget()); mainDpiAdapter) {
					bool isNew = false;
					auto dockWidgetDpiAdapterInfo = mainDpiAdapter->getWidgetDpiAdapterInfo(dockWidget, isNew);
					if (isNew) {
						QObject::connect(
							dockWidget, &QDockWidget::topLevelChanged, dockWidget, [=]() { dpiAdapter->checkStatusChanged(); }, Qt::QueuedConnection);

						dockWidgetDpiAdapterInfo->updateLayout = {true, false};
						dockWidgetDpiAdapterInfo->callbacks.append([=](double dpi, double /*oldDpi*/, bool firstShow) {
							if (firstShow) {
								dpiAdapter->dockWidgetFirstShow(dpi);
							} else if (!dockWidget->isFloating()) {
								dpiAdapter->dpiChangedEvent(dockWidget, dpi, QRect(), false, true, true);
							}
						});
					}
				}
			}
		} else {
			PLS_DEBUG(CSTR_DPI, "widget can't convert to dpi adapter, listen parent change event. widget class name: %s", widget->metaObject()->className());

			QWidget *notNullToplevelWidget = getNotNullToplevelWidget(widget);
			QObject::connect(widget, &QObject::destroyed, notNullToplevelWidget, [=]() {
				char name[256];
				sprintf_s(name, CSTR_HDPI_ORIGINAL CSTR_EVENT_FILTER_REMOVED "%p", widget);
				setProperty(notNullToplevelWidget, name, true);
			});

			QObject::connect(manager, &PLSThemeManager::parentChange, notNullToplevelWidget, [=](QWidget *changed) {
				char name[256];
				sprintf_s(name, CSTR_HDPI_ORIGINAL CSTR_EVENT_FILTER_REMOVED "%p", widget);
				if (changed == notNullToplevelWidget && !getProperty(notNullToplevelWidget, name, false)) {
					dpiHelper(widget, widgetInfo);
					notNullToplevelWidget->removeEventFilter(manager);
				}
			});

			notNullToplevelWidget->installEventFilter(manager);
		}
	}
	void mergeCss(QList<PLSCssIndex> &dst, const QList<PLSCssIndex> &src)
	{
		for (auto cssIndex : src) {
			if (!dst.contains(cssIndex)) {
				dst.append(cssIndex);
			}
		}
	}
	void mergeCss(std::pair<bool, QList<PLSCssIndex>> &dst, const QList<PLSCssIndex> &src)
	{
		dst.first = true;
		mergeCss(dst.second, src);
	}

private:
	mutable Widgets widgets;
};

PLSDpiHelper::PLSDpiHelper() : impl(new PLSDpiHelperImpl()) {}

PLSDpiHelper::~PLSDpiHelper() {}

PLSDpiHelper::PLSDpiHelper(const PLSDpiHelper &helper) : impl(helper.impl) {}

PLSDpiHelper &PLSDpiHelper::operator=(const PLSDpiHelper &helper)
{
	impl = helper.impl;
	return *this;
}

double PLSDpiHelper::getDpi(Screen *screen)
{
	return ::getScreenDpi(screen);
}

double PLSDpiHelper::getDpi(QWidget *widget)
{
	Screen *screen = nullptr;
	return ::getWidgetDpi(widget, screen);
}

double PLSDpiHelper::getDpi(QWidget *widget, Screen *&screen)
{
	return ::getWidgetDpi(widget, screen);
}

int PLSDpiHelper::calculate(double dpi, int value)
{
	return ::hdpiCalculate(value, dpi);
}

int PLSDpiHelper::calculate(QWidget *widget, int value)
{
	return ::hdpiCalculate(value, getDpi(widget));
}

double PLSDpiHelper::calculate(double dpi, double value)
{
	return ::hdpiCalculate(value, dpi);
}

double PLSDpiHelper::calculate(QWidget *widget, double value)
{
	return ::hdpiCalculate(value, getDpi(widget));
}

QPoint PLSDpiHelper::calculate(double dpi, const QPoint &value)
{
	return QPoint(calculate(dpi, value.x()), calculate(dpi, value.y()));
}

QPoint PLSDpiHelper::calculate(QWidget *widget, const QPoint &value)
{
	return calculate(getDpi(widget), value);
}

QSize PLSDpiHelper::calculate(double dpi, const QSize &value)
{
	return QSize(calculate(dpi, value.width()), calculate(dpi, value.height()));
}

QSize PLSDpiHelper::calculate(QWidget *widget, const QSize &value)
{
	return calculate(getDpi(widget), value);
}

QMargins PLSDpiHelper::calculate(double dpi, const QMargins &value)
{
	return QMargins(calculate(dpi, value.left()), calculate(dpi, value.top()), calculate(dpi, value.right()), calculate(dpi, value.bottom()));
}

QMargins PLSDpiHelper::calculate(QWidget *widget, const QMargins &value)
{
	return calculate(getDpi(widget), value);
}

void PLSDpiHelper::updateLayout(QWidget *widget)
{
	updateLayout(widget, getDpi(widget));
}

void PLSDpiHelper::updateLayout(QWidget *widget, double dpi)
{
	processWidget(widget, dpi);
}

void PLSDpiHelper::updateMinimumSize(QWidget *widget, const QSize &minimumSize, double dpi)
{
	bool canSetWidth = !getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MINIMUM_WIDTH, false);
	bool canSetHeight = !getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MINIMUM_HEIGHT, false);
	if (canSetWidth && canSetHeight) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "minimumSize" << minimumSize;
#endif
		widget->setMinimumSize(calculate(dpi, minimumSize));
	} else if (canSetWidth) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "minimumWidth" << minimumSize.width();
#endif
		widget->setMinimumWidth(calculate(dpi, minimumSize.width()));
	} else if (canSetHeight) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "minimumHeight" << minimumSize.height();
#endif
		widget->setMinimumHeight(calculate(dpi, minimumSize.height()));
	}
}

void PLSDpiHelper::updateMinimumWidth(QWidget *widget, int minimumWidth, double dpi)
{
	if (!getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MINIMUM_WIDTH, false)) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "minimumWidth" << minimumWidth;
#endif
		widget->setMinimumWidth(calculate(dpi, minimumWidth));
	}
}

void PLSDpiHelper::updateMinimumHeight(QWidget *widget, int minimumHeight, double dpi)
{
	if (!getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MINIMUM_HEIGHT, false)) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "minimumHeight" << minimumHeight;
#endif
		widget->setMinimumHeight(calculate(dpi, minimumHeight));
	}
}

void PLSDpiHelper::updateMaximumSize(QWidget *widget, const QSize &maximumSize, double dpi)
{
	bool canSetWidth = !getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MAXIMUM_WIDTH, false);
	bool canSetHeight = !getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MAXIMUM_HEIGHT, false);
	if (canSetWidth && canSetHeight) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "maximumSize" << maximumSize;
#endif
		widget->setMaximumSize(calculate(dpi, maximumSize));
	} else if (canSetWidth) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "maximumWidth" << maximumSize.width();
#endif
		widget->setMaximumWidth(calculate(dpi, maximumSize.width()));
	} else if (canSetHeight) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "maximumHeight" << maximumSize.height();
#endif
		widget->setMaximumHeight(calculate(dpi, maximumSize.height()));
	}
}

void PLSDpiHelper::updateMaximumWidth(QWidget *widget, int maximumWidth, double dpi)
{
	if (!getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MAXIMUM_WIDTH, false)) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "maximumWidth" << maximumWidth;
#endif
		widget->setMaximumWidth(calculate(dpi, maximumWidth));
	}
}

void PLSDpiHelper::updateMaximumHeight(QWidget *widget, int maximumHeight, double dpi)
{
	if (!getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MAXIMUM_HEIGHT, false)) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "maximumHeight" << maximumHeight;
#endif
		widget->setMaximumHeight(calculate(dpi, maximumHeight));
	}
}

void PLSDpiHelper::updateContentMargins(QWidget *widget, const QMargins &contentMargins, double dpi)
{
	if (!getProperty(widget, CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_CONTENTS_MARGINS, false)) {
#if defined(DUMP_LAYOUT_PROCESS_DETAILS)
		qInfo() << widget->metaObject()->className() << widget->objectName() << "contentsMargins" << contentMargins;
#endif
		widget->setContentsMargins(calculate(dpi, contentMargins));
	}
}

void PLSDpiHelper::dpiDynamicUpdate(QWidget *widget, bool updateCss)
{
	if (PLSWidgetDpiAdapter *dpiAdapter = getDpiAdapter(widget); dpiAdapter) {
		dpiAdapter->dpiDynamicUpdate(widget, updateCss);
	}
}

void PLSDpiHelper::checkStatusChanged(QWidget *widget, Screen *screen)
{
	if (PLSWidgetDpiAdapter *dpiAdapter = getDpiAdapter(widget); dpiAdapter) {
		checkStatusChanged(dpiAdapter, screen);
	}
}

void PLSDpiHelper::checkStatusChanged(PLSWidgetDpiAdapter *adapter, Screen *screen)
{
	adapter->checkStatusChanged(screen);
}

void PLSDpiHelper::dpiDynamicUpdateBeforeFirstShow(double dpi, QWidget *widget)
{
	if (PLSWidgetDpiAdapter *dpiAdapter = getDpiAdapter(widget); dpiAdapter) {
		dpiAdapter->dpiDynamicUpdateBeforeFirstShow(dpi, widget);
		pls_flush_style_recursive(widget);
	}
}

void PLSDpiHelper::setUpdateLayout(QWidget *widget, bool updateLayout)
{
	impl->setUpdateLayout(widget, updateLayout);
}

QSize PLSDpiHelper::getInitSize(QWidget *widget) const
{
	return impl->getInitSize(widget);
}

void PLSDpiHelper::setInitSize(QWidget *widget, const QSize &initSize)
{
	impl->setInitSize(widget, initSize);
}

void PLSDpiHelper::clearInitSize(QWidget *widget)
{
	impl->clearInitSize(widget);
}

void PLSDpiHelper::setFixedWidth(QWidget *widget, int fixedWidth)
{
	setMinimumWidth(widget, fixedWidth);
	setMaximumWidth(widget, fixedWidth);
}

void PLSDpiHelper::setFixedHeight(QWidget *widget, int fixedHeight)
{
	setMinimumHeight(widget, fixedHeight);
	setMaximumHeight(widget, fixedHeight);
}

void PLSDpiHelper::setFixedSize(QWidget *widget, const QSize &fixedSize)
{
	setMinimumSize(widget, fixedSize);
	setMaximumSize(widget, fixedSize);
}

void PLSDpiHelper::setMinimumWidth(QWidget *widget, int minimumWidth)
{
	impl->setMinimumWidth(widget, minimumWidth);
}

void PLSDpiHelper::setMinimumHeight(QWidget *widget, int minimumHeight)
{
	impl->setMinimumHeight(widget, minimumHeight);
}

void PLSDpiHelper::setMinimumSize(QWidget *widget, const QSize &minimumSize)
{
	setMinimumWidth(widget, minimumSize.width());
	setMinimumHeight(widget, minimumSize.height());
}

void PLSDpiHelper::setMaximumWidth(QWidget *widget, int maximumWidth)
{
	impl->setMaximumWidth(widget, maximumWidth);
}

void PLSDpiHelper::setMaximumHeight(QWidget *widget, int maximumHeight)
{
	impl->setMaximumHeight(widget, maximumHeight);
}

void PLSDpiHelper::setMaximumSize(QWidget *widget, const QSize &maximumSize)
{
	setMaximumWidth(widget, maximumSize.width());
	setMaximumHeight(widget, maximumSize.height());
}

void PLSDpiHelper::setContentsMargins(QWidget *widget, const QMargins &margins)
{
	impl->setContentsMargins(widget, margins);
}

void PLSDpiHelper::setCss(QWidget *widget, const QList<PLSCssIndex> &cssIndexes)
{
	impl->setCss(widget, cssIndexes);
}

void PLSDpiHelper::setStyleSheet(QWidget *widget, const QString &styleSheet)
{
	impl->setStyleSheet(widget, styleSheet);
}

void PLSDpiHelper::setDynamicStyleSheet(QWidget *widget, std::function<QString(double, bool)> dynamicStyleSheetCallback)
{
	impl->setDynamicStyleSheet(widget, dynamicStyleSheetCallback);
}

void PLSDpiHelper::notifyDpiChanged(QWidget *widget, std::function<void()> callback)
{
	impl->notifyDpiChanged(widget, [=](double, double, bool) { callback(); });
}

void PLSDpiHelper::notifyDpiChanged(QWidget *widget, std::function<void(double dpi)> callback)
{
	impl->notifyDpiChanged(widget, [=](double dpi, double, bool) { callback(dpi); });
}

void PLSDpiHelper::notifyDpiChanged(QWidget *widget, std::function<void(double dpi, double oldDpi)> callback)
{
	impl->notifyDpiChanged(widget, [=](double dpi, double oldDpi, bool) { callback(dpi, oldDpi); });
}

void PLSDpiHelper::notifyDpiChanged(QWidget *widget, std::function<void(double dpi, double oldDpi, bool firstShow)> callback)
{
	impl->notifyDpiChanged(widget, [=](double dpi, double oldDpi, bool firstShow) { callback(dpi, oldDpi, firstShow); });
}

void PLSDpiHelper::notifyScreenAvailableGeometryChanged(QWidget *widget, std::function<void(const QRect &screenAvailableGeometry)> callback)
{
	impl->notifyScreenAvailableGeometryChanged(widget, callback);
}

void PLSDpiHelper::setDynamicMinimumWidth(QWidget *widget, bool isDynamic)
{
	widget->setProperty(CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MINIMUM_WIDTH, isDynamic);
}
void PLSDpiHelper::setDynamicMinimumHeight(QWidget *widget, bool isDynamic)
{
	widget->setProperty(CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MINIMUM_HEIGHT, isDynamic);
}

void PLSDpiHelper::setDynamicMaximumWidth(QWidget *widget, bool isDynamic)
{
	widget->setProperty(CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MAXIMUM_WIDTH, isDynamic);
}

void PLSDpiHelper::setDynamicMaximumHeight(QWidget *widget, bool isDynamic)
{
	widget->setProperty(CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_MAXIMUM_HEIGHT, isDynamic);
}

void PLSDpiHelper::setDynamicContentsMargins(QWidget *widget, bool isDynamic)
{
	widget->setProperty(CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_CONTENTS_MARGINS, isDynamic);
}

void PLSDpiHelper::setDynamicContentsMargins(QLayout *layout, bool isDynamic)
{
	layout->setProperty(CSTR_HDPI_ORIGINAL CSTR_DYNAMIC_CONTENTS_MARGINS, isDynamic);
}

void PLSDpiHelper::setDynamicSpacerItem(QLayout *layout, QSpacerItem *spacerItem, bool isDynamic)
{
	char name[128];
	sprintf_s(name, CSTR_HDPI_ORIGINAL_SPACER_DYNAMIC "%p", spacerItem);
	layout->setProperty(name, isDynamic);
}

void PLSDpiHelper::dynamicDeleteSpacerItem(QLayout *layout, QSpacerItem *spacerItem)
{
	char name[128];
	sprintf_s(name, CSTR_HDPI_ORIGINAL_SPACER "%p", spacerItem);
	layout->setProperty(name, QSize(-1, -1));
	layout->removeItem(spacerItem);
	delete spacerItem;
}
