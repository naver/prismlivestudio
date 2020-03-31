#include "PLSWidgetDpiAdapter.hpp"

#include <Windows.h>

#include <algorithm>

#include <QScreen>
#include <QMetaObject>
#include <QWindow>
#include <QApplication>
#include <QDockWidget>
#include <QWindowStateChangeEvent>

#include <QDesktopWidget>

#include <private/qwidget_p.h>

#include "PLSDpiHelper.h"
#include "log.h"

#define CSTR_DPIADAPTER "dpi-adapter"

extern int getScreenIndex(QWidget *widget);
extern int getPrimaryScreenIndex();
extern QScreen *getScreen(QWidget *widget);
extern QScreen *getScreen(PLSWidgetDpiAdapter *adapter);
extern PLSWidgetDpiAdapter *getDpiAdapter(QWidget *widget);

namespace {
QRect toRect(const LPRECT rc)
{
	return QRect(rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top);
}
LPRECT toRECT(LPRECT rc, const QRect &qrc)
{
	rc->left = qrc.left();
	rc->top = qrc.top();
	rc->right = qrc.right();
	rc->bottom = qrc.bottom();
	return rc;
}
bool isChild(QWidget *parent, QWidget *child)
{
	for (; child; child = child->parentWidget()) {
		if (parent == child) {
			return true;
		}
	}
	return false;
}
void processCss(QWidget *widget, double dpi, const QList<PLSCssIndex> &cssIndexes, const QString &styleSheet, const QString &dynamicStyleSheet)
{
	PLSThemeManager *manager = PLS_THEME_MANAGER;
	QString css = manager->loadCss(dpi, cssIndexes);
	css.append(manager->preprocessCss(dpi, styleSheet));
	css.append(manager->preprocessCss(dpi, dynamicStyleSheet));
	widget->setStyleSheet(css);
}
bool isToplevel(QWidget *widget)
{
	if (!widget) {
		return false;
	} else if (QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(widget); dockWidget) {
		return dockWidget->isFloating();
	} else {
		return widget->isWindow();
	}
}
bool isMinimized(QWidget *widget)
{
	if (!widget) {
		return false;
	} else if (QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(widget); !dockWidget) {
		return widget->isMinimized();
	} else if (PLSWidgetDpiAdapter *dpiAdapter = getDpiAdapter(dockWidget->parentWidget()); dpiAdapter) {
		return dpiAdapter->selfWidget()->isMinimized();
	}

	return false;
}
void checkRestoredGeometry(const QRect &availableGeometry, QRect *restoredGeometry, int frameHeight)
{
	if (!restoredGeometry->intersects(availableGeometry)) {
		restoredGeometry->moveBottom(qMin(restoredGeometry->bottom(), availableGeometry.bottom()));
		restoredGeometry->moveLeft(qMax(restoredGeometry->left(), availableGeometry.left()));
		restoredGeometry->moveRight(qMin(restoredGeometry->right(), availableGeometry.right()));
	}
	restoredGeometry->moveTop(qMax(restoredGeometry->top(), availableGeometry.top() + frameHeight));
}
}

void rectLimitTo(QRect &target, const QRect &limited)
{
	if (target.right() > limited.right()) {
		target.moveRight(limited.right());
	}

	if (target.bottom() > limited.bottom()) {
		target.moveBottom(limited.bottom());
	}

	if (target.left() < limited.left()) {
		target.moveLeft(limited.left());
	}

	if (target.top() < limited.top()) {
		target.moveTop(limited.top());
	}
}

PLSWidgetDpiAdapter::WidgetDpiAdapterInfo::WidgetDpiAdapterInfo(QWidget *widget_) : widget(widget_) {}

PLSWidgetDpiAdapter::WidgetDpiAdapterInfo::~WidgetDpiAdapterInfo()
{
	QObject::disconnect(connection);
}

PLSWidgetDpiAdapter::WidgetDpiAdapterInfo::WidgetDpiAdapterInfo(const WidgetDpiAdapterInfo &widgetDpiAdapterInfo)
	: widget(widgetDpiAdapterInfo.widget),
	  updateLayout(widgetDpiAdapterInfo.updateLayout),
	  initSize(widgetDpiAdapterInfo.initSize),
	  minimumSize(widgetDpiAdapterInfo.minimumSize),
	  maximumSize(widgetDpiAdapterInfo.maximumSize),
	  contentMargins(widgetDpiAdapterInfo.contentMargins),
	  cssIndexes(widgetDpiAdapterInfo.cssIndexes),
	  styleSheet(widgetDpiAdapterInfo.styleSheet),
	  dynamicStyleSheetCallback(widgetDpiAdapterInfo.dynamicStyleSheetCallback),
	  callbacks(widgetDpiAdapterInfo.callbacks),
	  connection(widgetDpiAdapterInfo.connection)
{
}

PLSWidgetDpiAdapter::WidgetDpiAdapterInfo &PLSWidgetDpiAdapter::WidgetDpiAdapterInfo::operator=(const WidgetDpiAdapterInfo &widgetDpiAdapterInfo)
{
	widget = widgetDpiAdapterInfo.widget;
	updateLayout = widgetDpiAdapterInfo.updateLayout;
	initSize = widgetDpiAdapterInfo.initSize;
	minimumSize = widgetDpiAdapterInfo.minimumSize;
	maximumSize = widgetDpiAdapterInfo.maximumSize;
	contentMargins = widgetDpiAdapterInfo.contentMargins;
	cssIndexes = widgetDpiAdapterInfo.cssIndexes;
	styleSheet = widgetDpiAdapterInfo.styleSheet;
	dynamicStyleSheetCallback = widgetDpiAdapterInfo.dynamicStyleSheetCallback;
	callbacks = widgetDpiAdapterInfo.callbacks;
	connection = widgetDpiAdapterInfo.connection;
	return *this;
}

void PLSWidgetDpiAdapter::WidgetDpiAdapterInfo::dpiHelper(PLSWidgetDpiAdapter *adapter, double dpi, bool firstShow, bool updateCss)
{
	extern QRect getScreenAvailableRect(PLSWidgetDpiAdapter * adapter);
	extern void normalGeometry(PLSWidgetDpiAdapter * adapter, const QRect &screenAvailableGeometry, QRect &geometryOfNormal);

	if (updateLayout.first && updateLayout.second) {
		PLSDpiHelper::updateLayout(widget, dpi);
	}

	if (updateCss) {
		QString dynamicStyleSheet;
		if (dynamicStyleSheetCallback) {
			dynamicStyleSheet = dynamicStyleSheetCallback(dpi, firstShow);
		}

		if (!cssIndexes.isEmpty() || !styleSheet.isEmpty() || !dynamicStyleSheet.isEmpty()) {
			::processCss(widget, dpi, cssIndexes, styleSheet, dynamicStyleSheet);
		}
	}

	if (minimumSize.first.first && minimumSize.second.first) {
		PLSDpiHelper::updateMinimumSize(widget, QSize(minimumSize.first.second, minimumSize.second.second), dpi);
	} else if (minimumSize.first.first) {
		PLSDpiHelper::updateMinimumWidth(widget, minimumSize.first.second, dpi);
	} else if (minimumSize.second.first) {
		PLSDpiHelper::updateMinimumHeight(widget, minimumSize.second.second, dpi);
	}

	if (maximumSize.first.first && maximumSize.second.first) {
		PLSDpiHelper::updateMaximumSize(widget, QSize(maximumSize.first.second, maximumSize.second.second), dpi);
	} else if (maximumSize.first.first) {
		PLSDpiHelper::updateMaximumWidth(widget, maximumSize.first.second, dpi);
	} else if (maximumSize.second.first) {
		PLSDpiHelper::updateMaximumHeight(widget, maximumSize.second.second, dpi);
	}

	if (contentMargins.first) {
		PLSDpiHelper::updateContentMargins(widget, contentMargins.second, dpi);
	}

	if (firstShow && initSize.first) {
		QRect geometry(QPoint(0, 0), PLSDpiHelper::calculate(dpi, initSize.second).expandedTo(widget->minimumSize()).boundedTo(widget->maximumSize()));
		normalGeometry(adapter, getScreenAvailableRect(adapter), geometry);
		widget->resize(geometry.size());
	}
}

QRect PLSWidgetDpiAdapter::WidgetDpiAdapterInfo::showAsDefaultSize(double dpi, const QRect &normalGeometry, const QRect &screenAvailableGeometry)
{
	QRect geometry = screenAvailableGeometry.adjusted(1, 0, -1, 0);

	if (normalGeometry.width() < geometry.width()) {
		geometry.setLeft(normalGeometry.left());
		geometry.setWidth(normalGeometry.width());
	}

	if (normalGeometry.height() < geometry.height()) {
		geometry.setTop(normalGeometry.top());
		geometry.setHeight(normalGeometry.height());
	}

	if (minimumSize.first.first) {
		int minimumWidth = PLSDpiHelper::calculate(dpi, minimumSize.first.second);
		if (minimumWidth > geometry.width()) {
			geometry.setWidth(minimumWidth);
		}
	}

	if (minimumSize.second.first) {
		int minimumHeight = PLSDpiHelper::calculate(dpi, minimumSize.second.second);
		if (minimumHeight > geometry.height()) {
			geometry.setHeight(minimumHeight);
		}
	}

	if (maximumSize.first.first) {
		int maximumWidth = PLSDpiHelper::calculate(dpi, maximumSize.first.second);
		if (maximumWidth < geometry.width()) {
			geometry.setWidth(maximumWidth);
		}
	}

	if (maximumSize.second.first) {
		int maximumHeight = PLSDpiHelper::calculate(dpi, maximumSize.second.second);
		if (maximumHeight < geometry.height()) {
			geometry.setHeight(maximumHeight);
		}
	}

	rectLimitTo(geometry, screenAvailableGeometry);

	return geometry;
}

PLSWidgetDpiAdapter::ScreenConnection::ScreenConnection(QScreen *screen_) : screen(screen_) {}

PLSWidgetDpiAdapter::ScreenConnection::~ScreenConnection()
{
	QObject::disconnect(screenAvailableGeometryChanged);
}

PLSWidgetDpiAdapter::ScreenConnection::ScreenConnection(const ScreenConnection &screenConnection)
	: screen(screenConnection.screen), screenAvailableGeometryChanged(screenConnection.screenAvailableGeometryChanged)
{
}

PLSWidgetDpiAdapter::ScreenConnection &PLSWidgetDpiAdapter::ScreenConnection::operator=(const ScreenConnection &screenConnection)
{
	screen = screenConnection.screen;
	screenAvailableGeometryChanged = screenConnection.screenAvailableGeometryChanged;
	return *this;
}

PLSWidgetDpiAdapter::PLSWidgetDpiAdapter() {}

PLSWidgetDpiAdapter::~PLSWidgetDpiAdapter()
{
	QObject::disconnect(screenAddedConnection);
	QObject::disconnect(screenRemovedConnection);
}

std::shared_ptr<PLSWidgetDpiAdapter::WidgetDpiAdapterInfo> PLSWidgetDpiAdapter::getWidgetDpiAdapterInfo(QWidget *widget) const
{
	bool isNew = false;
	return getWidgetDpiAdapterInfo(widget, isNew);
}

std::shared_ptr<PLSWidgetDpiAdapter::WidgetDpiAdapterInfo> PLSWidgetDpiAdapter::getWidgetDpiAdapterInfo(QWidget *widget, bool &isNew) const
{
	if (auto iter = std::find_if(widgetDpiAdapterInfos.begin(), widgetDpiAdapterInfos.end(), [=](const auto &i) { return i->widget == widget; }); iter != widgetDpiAdapterInfos.end()) {
		isNew = false;
		return *iter;
	}

	isNew = true;
	std::shared_ptr<WidgetDpiAdapterInfo> widgetDpiAdapterInfo(new WidgetDpiAdapterInfo(widget));

	auto where = widgetDpiAdapterInfos.end();
	for (auto iter = widgetDpiAdapterInfos.begin(), endIter = widgetDpiAdapterInfos.end(); iter != endIter; ++iter) {
		if (isChild(widget, (*iter)->widget)) {
			where = iter;
			break;
		}
	}

	widgetDpiAdapterInfos.insert(where, widgetDpiAdapterInfo);

	if (QWidget *self = selfWidget(); widget != self) {
		widgetDpiAdapterInfo->connection = QObject::connect(widget, &QObject::destroyed, self, [=]() { widgetDpiAdapterInfos.remove_if([=](const auto &i) { return i->widget == widget; }); });
	}

	return widgetDpiAdapterInfo;
}

std::shared_ptr<PLSWidgetDpiAdapter::ScreenConnection> PLSWidgetDpiAdapter::getScreenConnection(QScreen *screen) const
{
	if (auto iter = std::find_if(screenConnections.begin(), screenConnections.end(), [=](const auto &i) { return i->screen == screen; }); iter != screenConnections.end()) {
		return *iter;
	}

	std::shared_ptr<ScreenConnection> screenConnection(new ScreenConnection(screen));
	screenConnections.push_back(screenConnection);
	return screenConnection;
}

QRect PLSWidgetDpiAdapter::showAsDefaultSize(const QRect &normalGeometry, const QRect &screenAvailableGeometry)
{
	auto iter = std::find_if(widgetDpiAdapterInfos.begin(), widgetDpiAdapterInfos.end(),
				 [=](std::shared_ptr<WidgetDpiAdapterInfo> widgetDpiAdapterInfo) { return widgetDpiAdapterInfo->widget == selfWidget(); });
	if (iter != widgetDpiAdapterInfos.end()) {
		return (*iter)->showAsDefaultSize(dpi, normalGeometry, screenAvailableGeometry);
	}

	QRect geometry = screenAvailableGeometry.adjusted(1, 0, -1, 0);

	if (normalGeometry.width() < geometry.width()) {
		geometry.setLeft(normalGeometry.left());
		geometry.setWidth(normalGeometry.width());
	}

	if (normalGeometry.height() < geometry.height()) {
		geometry.setTop(normalGeometry.top());
		geometry.setHeight(normalGeometry.height());
	}

	rectLimitTo(geometry, screenAvailableGeometry);

	return geometry;
}

bool PLSWidgetDpiAdapter::needQtProcessDpiChangedEvent() const
{
	return true;
}

bool PLSWidgetDpiAdapter::needProcessScreenChangedEvent() const
{
	return true;
}

QRect PLSWidgetDpiAdapter::onDpiChanged(double /*dpi*/, double /*oldDpi*/, const QRect &suggested, bool /*firstShow*/)
{
	return suggested;
}

void PLSWidgetDpiAdapter::onDpiChanged(double /*dpi*/, double /*oldDpi*/, bool /*firstShow*/) {}

void PLSWidgetDpiAdapter::onScreenAvailableGeometryChanged(const QRect & /*screenAvailableGeometry*/) {}

void PLSWidgetDpiAdapter::onScreenRemoved() {}

QRect PLSWidgetDpiAdapter::getSuggestedRect(const QRect &suggested) const
{
	return suggested;
}

void PLSWidgetDpiAdapter::init(QWidget *widget)
{
	for (auto screen : QApplication::screens()) {
		auto screenConnection = getScreenConnection(screen);
		screenConnection->screenAvailableGeometryChanged = QObject::connect(
			screen, &QScreen::availableGeometryChanged, widget,
			[this, widget, screen](const QRect &screenAvailableGeometry) {
				if (::isToplevel(widget) && ::getScreen(this) == screen) {
					if (!::isMinimized(widget)) {
						PLS_DEBUG(CSTR_DPIADAPTER, "screen available geometry change event, dpi = %lf, class name: %s, object name: %s", dpi, widget->metaObject()->className(),
							  widget->objectName().toUtf8().constData());

						QMetaObject::invokeMethod(
							widget, [=]() { screenAvailableGeometryChangedEvent(screenAvailableGeometry); }, Qt::QueuedConnection);
					}
				}
			},
			Qt::QueuedConnection);
	}

	screenAddedConnection = QObject::connect(qApp, &QApplication::screenAdded, [this, widget](QScreen *screen) {
		auto screenConnection = getScreenConnection(screen);
		screenConnection->screenAvailableGeometryChanged = QObject::connect(
			screen, &QScreen::availableGeometryChanged, widget,
			[this, widget, screen](const QRect &screenAvailableGeometry) {
				if (::isToplevel(widget) && ::getScreen(this) == screen) {
					if (!::isMinimized(widget)) {
						PLS_DEBUG(CSTR_DPIADAPTER, "screen available geometry change event, dpi = %lf, class name: %s, object name: %s", dpi, widget->metaObject()->className(),
							  widget->objectName().toUtf8().constData());

						QMetaObject::invokeMethod(
							widget, [=]() { screenAvailableGeometryChangedEvent(screenAvailableGeometry); }, Qt::QueuedConnection);
					}
				}
			},
			Qt::QueuedConnection);
	});

	screenRemovedConnection = QObject::connect(
		qApp, &QApplication::screenRemoved, widget,
		[this](QScreen *screen) {
			screenConnections.remove_if([=](const auto &i) { return i->screen == screen; });
			if (::getScreen(this) == screen) {
				onScreenRemoved();
			}
		},
		Qt::DirectConnection);
}

bool PLSWidgetDpiAdapter::event(QWidget *widget, QEvent *event, std::function<bool(QEvent *)> baseEvent)
{
	switch (event->type()) {
	case QEvent::Show: {
		if (firstShowState == FirstShowState::Waiting) {
			if (QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(widget); !dockWidget || dockWidget->isFloating()) {
				firstShowEvent(widget);

				firstShowState = FirstShowState::InitDpi;
				this->dpi = PLSDpiHelper::getDpi(widget);

				firstShowState = FirstShowState::InProgress;
				dpiChangedEvent(widget, this->dpi, QRect(), true, false, true);

				firstShowState = FirstShowState::Complete;
			}
		}
		break;
	}
	}

	return baseEvent(event);
}

bool PLSWidgetDpiAdapter::nativeEvent(QWidget *widget, const QByteArray &eventType, void *message, long *result, std::function<bool(const QByteArray &, void *, long *)> baseNativeEvent)
{
	MSG *msg = (MSG *)message;
	switch (msg->message) {
	case WM_DPICHANGED: {
		if (firstShowState == FirstShowState::Waiting) {
			toRECT((LPRECT)msg->lParam, getSuggestedRect(toRect((LPRECT)msg->lParam)));
			return needQtProcessDpiChangedEvent() ? baseNativeEvent(eventType, message, result) : true;
		}

		if (!isMinimizedDpiChanged) {
			QRect suggested = dpiChangedEvent(widget, HIWORD(msg->wParam) / 96.0, toRect((LPRECT)msg->lParam), false, false, true);
			toRECT((LPRECT)msg->lParam, suggested);
			return needQtProcessDpiChangedEvent() ? baseNativeEvent(eventType, message, result) : true;
		}

		dpiChangedEvent(widget, HIWORD(msg->wParam) / 96.0, QRect(), false, true, false);
		screenAvailableGeometryChangedEvent(getScreen(this)->availableGeometry());
		isMinimizedDpiChanged = false;
		return true;
	}
	default:
		return baseNativeEvent(eventType, message, result);
	}
}

void PLSWidgetDpiAdapter::notifyFirstShow(std::function<void()> callback)
{
	firstShowCallbacks.push_back(callback);
}

QByteArray PLSWidgetDpiAdapter::saveGeometry(QWidget *widget) const
{
	return saveGeometry(widget, widget->geometry());
}

QByteArray PLSWidgetDpiAdapter::saveGeometry(QWidget *widget, const QRect &geometry) const
{
	QByteArray array;
	QDataStream stream(&array, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_4_0);
	const quint32 magicNumber = 0x1D9D0CB;
	// Version history:
	// - Qt 4.2 - 4.8.6, 5.0 - 5.3    : Version 1.0
	// - Qt 4.8.6 - today, 5.4 - today: Version 2.0, save screen width in addition to check for high DPI scaling.
	// - Qt 5.12 - today              : Version 3.0, save QWidget::geometry()
	quint16 majorVersion = 3;
	quint16 minorVersion = 0;
	int screenNumber = getScreenIndex(widget);
	QScreen *screen = screenNumber >= 0 ? QApplication::screens()[screenNumber] : QApplication::primaryScreen();
	stream << magicNumber << majorVersion << minorVersion          // version
	       << widget->frameGeometry()                              // frameGeometry
	       << widget->normalGeometry()                             // normalGeometry
	       << qint32(screenNumber)                                 // screenNumber
	       << quint8(widget->windowState() & Qt::WindowMaximized)  // MaximizedState
	       << quint8(widget->windowState() & Qt::WindowFullScreen) // FullScreenState
	       << qint32(screen->geometry().width())                   // added in 2.0
	       << geometry;                                            // added in 3.0
	return array;
}

bool PLSWidgetDpiAdapter::restoreGeometry(QWidget *widget, const QByteArray &geometry)
{
	if (geometry.size() < 4)
		return false;
	QDataStream stream(geometry);
	stream.setVersion(QDataStream::Qt_4_0);

	const quint32 magicNumber = 0x1D9D0CB;
	quint32 storedMagicNumber;
	stream >> storedMagicNumber;
	if (storedMagicNumber != magicNumber)
		return false;

	const quint16 currentMajorVersion = 3;
	quint16 majorVersion = 0;
	quint16 minorVersion = 0;

	stream >> majorVersion >> minorVersion;

	if (majorVersion > currentMajorVersion)
		return false;
	// (Allow all minor versions.)

	QRect restoredFrameGeometry;
	QRect restoredGeometry;
	QRect restoredNormalGeometry;
	qint32 restoredScreenNumber;
	quint8 maximized;
	quint8 fullScreen;
	qint32 restoredScreenWidth = 0;

	stream >> restoredFrameGeometry // Only used for sanity checks in version 0
		>> restoredNormalGeometry >> restoredScreenNumber >> maximized >> fullScreen;

	if (majorVersion > 1)
		stream >> restoredScreenWidth;
	if (majorVersion > 2)
		stream >> restoredGeometry;

	// ### Qt 6 - Perhaps it makes sense to dumb down the restoreGeometry() logic, see QTBUG-69104
	QList<QScreen *> screens = QApplication::screens();
	if (restoredScreenNumber < 0 || restoredScreenNumber >= screens.count())
		restoredScreenNumber = getPrimaryScreenIndex();
	QScreen *screen = screens[restoredScreenNumber];
	const qreal screenWidthF = qreal(screen->geometry().width());
	// Sanity check bailing out when large variations of screen sizes occur due to
	// high DPI scaling or different levels of DPI awareness.
	if (restoredScreenWidth) {
		const qreal factor = qreal(restoredScreenWidth) / screenWidthF;
		if (factor < 0.8 || factor > 1.25)
			return false;
	} else {
		// Saved by Qt 5.3 and earlier, try to prevent too large windows
		// unless the size will be adapted by maximized or fullscreen.
		if (!maximized && !fullScreen && qreal(restoredFrameGeometry.width()) / screenWidthF > 1.5)
			return false;
	}

	const int frameHeight = 20;

	if (!restoredNormalGeometry.isValid())
		restoredNormalGeometry = QRect(QPoint(0, frameHeight), widget->sizeHint());
	if (!restoredNormalGeometry.isValid()) {
		// use the widget's adjustedSize if the sizeHint() doesn't help
		restoredNormalGeometry.setSize(restoredNormalGeometry.size().expandedTo(QWidgetPrivate::get(widget)->adjustedSize()));
	}

	const QRect availableGeometry = screen->availableGeometry();

	checkRestoredGeometry(availableGeometry, &restoredGeometry, frameHeight);
	checkRestoredGeometry(availableGeometry, &restoredNormalGeometry, frameHeight);

	if (maximized || fullScreen) {
		// set geometry before setting the window state to make
		// sure the window is maximized to the right screen.
		Qt::WindowStates ws = widget->windowState();
#ifndef Q_OS_WIN
		setGeometry(restoredNormalGeometry);
#else
		if (ws & Qt::WindowFullScreen) {
			// Full screen is not a real window state on Windows.
			widget->move(availableGeometry.topLeft());
		} else if (ws & Qt::WindowMaximized) {
			// Setting a geometry on an already maximized window causes this to be
			// restored into a broken, half-maximized state, non-resizable state (QTBUG-4397).
			// Move the window in normal state if needed.
			if (restoredScreenNumber != getScreenIndex(widget)) {
				widget->setWindowState(Qt::WindowNoState);
				widget->setGeometry(restoredNormalGeometry);
			}
		} else {
			widget->setGeometry(restoredNormalGeometry);
		}
#endif // Q_OS_WIN
		if (maximized)
			ws |= Qt::WindowMaximized;
		if (fullScreen)
			ws |= Qt::WindowFullScreen;
		widget->setWindowState(ws);
		QWidgetPrivate::get(widget)->topData()->normalGeometry = restoredNormalGeometry;
	} else {
		widget->setWindowState(widget->windowState() & ~(Qt::WindowMaximized | Qt::WindowFullScreen));
		if (majorVersion > 2)
			widget->setGeometry(restoredGeometry);
		else
			widget->setGeometry(restoredNormalGeometry);
	}
	return true;
}

void PLSWidgetDpiAdapter::dpiDynamicUpdate(QWidget *widget, bool updateCss)
{
	dpiChangedEvent(widget, this->dpi, QRect(), false, true, updateCss);
}

void PLSWidgetDpiAdapter::dpiDynamicUpdateBeforeFirstShow(double dpi, QWidget *widget)
{
	if (firstShowState == FirstShowState::Waiting) {
		firstShowState = FirstShowState::InProgress;
		dpiChangedEvent(widget, dpi, QRect(), false, true, true);
		firstShowState = FirstShowState::Waiting;
	}
}

void PLSWidgetDpiAdapter::firstShowEvent(QWidget * /*widget*/)
{
	for (auto callback : firstShowCallbacks) {
		callback();
	}
}

QRect PLSWidgetDpiAdapter::dpiChangedEvent(QWidget *widget, double dpi, const QRect &suggested, bool firstShow, bool dynamicUpdate, bool updateCss)
{
	if (firstShow && (firstShowState == FirstShowState::Complete)) {
		PLS_DEBUG(CSTR_DPIADAPTER, "filter dpi change event, repeat process, dpi = %lf, class name: %s, object name: %s", dpi, widget->metaObject()->className(),
			  widget->objectName().toUtf8().constData());
	} else if (isDpiChanging) {
		PLS_DEBUG(CSTR_DPIADAPTER, "dpi change event, dpi is in changing, dpi = %lf, first show: %s, dynamic update: %s, class name: %s, object name: %s", dpi, firstShow ? "true" : "false",
			  dynamicUpdate ? "true" : "false", widget->metaObject()->className(), widget->objectName().toUtf8().constData());
	} else if ((firstShowState == FirstShowState::Waiting) || (!firstShow && !dynamicUpdate && (this->dpi == dpi))) {
		PLS_DEBUG(CSTR_DPIADAPTER, "filter dpi change event, dpi = %lf, first show: %s, dynamic update: %s, class name: %s, object name: %s", dpi, firstShow ? "true" : "false",
			  dynamicUpdate ? "true" : "false", widget->metaObject()->className(), widget->objectName().toUtf8().constData());
	} else {
		PLS_DEBUG(CSTR_DPIADAPTER, "dpi change event, dpi = %lf, first show: %s, dynamic update: %s, class name: %s, object name: %s", dpi, firstShow ? "true" : "false",
			  dynamicUpdate ? "true" : "false", widget->metaObject()->className(), widget->objectName().toUtf8().constData());

		double oldDpi = this->dpi;
		this->dpi = dpi;
		return dpiChangedEventImpl(widget, dpi, oldDpi, suggested, firstShow, updateCss);
	}
	return getSuggestedRect(suggested);
}

QRect PLSWidgetDpiAdapter::dpiChangedEventImpl(QWidget *widget, double dpi, double oldDpi, const QRect &suggested, bool firstShow, bool updateCss)
{
	isDpiChanging = true;

	auto iter = std::find_if(widgetDpiAdapterInfos.begin(), widgetDpiAdapterInfos.end(),
				 [=](std::shared_ptr<WidgetDpiAdapterInfo> widgetDpiAdapterInfo) { return isChild(widget, widgetDpiAdapterInfo->widget); });
	if ((iter == widgetDpiAdapterInfos.end()) || ((*iter)->widget != widget)) {
		PLSDpiHelper::updateLayout(widget, dpi);
	}

	std::for_each(iter, widgetDpiAdapterInfos.end(), [=](std::shared_ptr<WidgetDpiAdapterInfo> widgetDpiAdapterInfo) {
		if (isChild(widget, widgetDpiAdapterInfo->widget)) {
			widgetDpiAdapterInfo->dpiHelper(this, dpi, firstShow, updateCss);
		}
	});

	std::for_each(iter, widgetDpiAdapterInfos.end(), [=](std::shared_ptr<WidgetDpiAdapterInfo> widgetDpiAdapterInfo) {
		if (isChild(widget, widgetDpiAdapterInfo->widget)) {
			for (auto callback : widgetDpiAdapterInfo->callbacks) {
				callback(dpi, oldDpi, firstShow);
			}
		}
	});

	QRect result = suggested;
	if (widget == selfWidget()) {
		result = onDpiChanged(dpi, oldDpi, suggested, firstShow);
		onDpiChanged(dpi, oldDpi, firstShow);
	}

	isDpiChanging = false;
	return result;
}

void PLSWidgetDpiAdapter::screenAvailableGeometryChangedEvent(const QRect &screenAvailableGeometry)
{
	onScreenAvailableGeometryChanged(screenAvailableGeometry);

	std::for_each(widgetDpiAdapterInfos.begin(), widgetDpiAdapterInfos.end(), [=](std::shared_ptr<WidgetDpiAdapterInfo> widgetDpiAdapterInfo) {
		for (auto callback : widgetDpiAdapterInfo->sagcCallbacks) {
			callback(screenAvailableGeometry);
		}
	});
}

void PLSWidgetDpiAdapter::checkStatusChanged(PLSDpiHelper::Screen *screen)
{
	if (firstShowState != FirstShowState::Complete) {
		return;
	}

	QWidget *widget = selfWidget();

	double oldDpi = this->dpi;
	double dpi = screen ? PLSDpiHelper::getDpi(screen) : PLSDpiHelper::getDpi(widget);

	if (this->dpi != dpi) {
		isMinimizedDpiChanged = ::isMinimized(widget);
		PLS_DEBUG(CSTR_DPIADAPTER, "check dpi changed, dpi = %lf, minimized dpi changed: %s, class name: %s, object name: %s", dpi, isMinimizedDpiChanged ? "true" : "false",
			  widget->metaObject()->className(), widget->objectName().toUtf8().constData());
		dpiChangedEvent(widget, dpi, QRect(), false, true, true);
	}
}

void PLSWidgetDpiAdapter::dockWidgetFirstShow(double dpi)
{
	QWidget *widget = selfWidget();
	if (firstShowState == FirstShowState::Waiting) {
		if (QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(widget); dockWidget && !dockWidget->isFloating()) {
			firstShowEvent(widget);

			firstShowState = FirstShowState::InitDpi;
			this->dpi = PLSDpiHelper::getDpi(widget);

			firstShowState = FirstShowState::InProgress;
			dpiChangedEvent(widget, this->dpi, QRect(), true, false, true);

			firstShowState = FirstShowState::Complete;
		}
	}
}
