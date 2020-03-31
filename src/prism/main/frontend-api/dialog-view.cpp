#include "dialog-view.hpp"
#include "ui_PLSDialogView.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QResizeEvent>
#include <QPainter>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <qlistwidget.h>
#include <QAbstractSpinBox>
#include <QHBoxLayout>
#include <QTimer>
#include <QPointer>
#include <QWidget>
#include <QContiguousCache>
#include <log.h>
#include <action.h>

#if defined(Q_OS_WINDOWS)
#include <Windows.h>
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")
#endif // Q_OS_WINDOWS

#include "frontend-api.h"

#define TOPLEVELVIEW_MODULE "ToplevelView"

const QString CONTENT = QStringLiteral("content");

extern PLSWidgetDpiAdapter *getDpiAdapter(QWidget *widget);

static QRect toRect(const RECT &rc)
{
	return QRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

FRONTEND_API int getScreenIndex(const QPoint &point)
{
	QList<QScreen *> screens = QApplication::screens();
	for (int i = 0, count = screens.count(); i < count; ++i) {
		if (screens[i]->geometry().contains(point)) {
			return i;
		}
	}
	return -1;
}

FRONTEND_API int getScreenIndex(PLSWidgetDpiAdapter *adapter)
{
	HMONITOR monitor = MonitorFromWindow((HWND)adapter->selfWidget()->winId(), MONITOR_DEFAULTTOPRIMARY);

	MONITORINFOEXW mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);
	GetMonitorInfoW(monitor, &mi);

	QRect rcMonitor = toRect(mi.rcMonitor);
	QList<QScreen *> screens = QApplication::screens();
	for (int i = 0, count = screens.count(); i < count; ++i) {
		QScreen *screen = screens[i];
		if (screen->geometry() == rcMonitor) {
			return i;
		}
	}
	return -1;
}

FRONTEND_API int getScreenIndex(QWidget *widget)
{
	PLSWidgetDpiAdapter *adapter = getDpiAdapter(widget);
	if (adapter) {
		return getScreenIndex(adapter);
	}
	return -1;
}

FRONTEND_API int getPrimaryScreenIndex()
{
	return QApplication::screens().indexOf(QApplication::primaryScreen());
}

FRONTEND_API QScreen *getScreen(const QPoint &point)
{
	int screenNumber = getScreenIndex(point);
	return screenNumber >= 0 ? QApplication::screens()[screenNumber] : QApplication::primaryScreen();
}

FRONTEND_API QScreen *getScreen(PLSWidgetDpiAdapter *adapter)
{
	int screenNumber = getScreenIndex(adapter);
	return screenNumber >= 0 ? QApplication::screens()[screenNumber] : QApplication::primaryScreen();
}

FRONTEND_API QScreen *getScreen(QWidget *widget)
{
	int screenNumber = getScreenIndex(widget);
	return screenNumber >= 0 ? QApplication::screens()[screenNumber] : QApplication::primaryScreen();
}

static BOOL CALLBACK getPrimaryMonitorEnumMonitorCallback(HMONITOR monitor, HDC, LPRECT, LPARAM lParam)
{
	MONITORINFO mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);
	GetMonitorInfoW(monitor, &mi);
	if (mi.dwFlags & MONITORINFOF_PRIMARY) {
		*(HMONITOR *)lParam = monitor;
		return false;
	}
	return true;
}

FRONTEND_API HMONITOR getPrimaryMonitor()
{
	HMONITOR monitor = nullptr;
	EnumDisplayMonitors(nullptr, nullptr, &getPrimaryMonitorEnumMonitorCallback, (LPARAM)&monitor);
	return monitor;
}

FRONTEND_API HMONITOR getMonitor(const QPoint &point)
{
	return MonitorFromPoint({point.x(), point.y()}, MONITOR_DEFAULTTOPRIMARY);
}

FRONTEND_API HMONITOR getMonitor(PLSWidgetDpiAdapter *adapter)
{
	return MonitorFromWindow((HWND)adapter->selfWidget()->winId(), MONITOR_DEFAULTTOPRIMARY);
}

FRONTEND_API HMONITOR getMonitor(QWidget *widget)
{
	PLSWidgetDpiAdapter *adapter = getDpiAdapter(widget);
	if (adapter) {
		return getMonitor(adapter);
	}
	return getPrimaryMonitor();
}

FRONTEND_API double getMonitorDpi(HMONITOR monitor)
{
	UINT dpiX, dpiY;
	GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
	return dpiX / 96.0;
}

FRONTEND_API QRect getMonitorRect(HMONITOR monitor)
{
	MONITORINFO mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);
	GetMonitorInfoW(monitor, &mi);
	return toRect(mi.rcMonitor);
}

FRONTEND_API QRect getMonitorAvailableRect(HMONITOR monitor)
{
	MONITORINFO mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);
	GetMonitorInfoW(monitor, &mi);
	return toRect(mi.rcWork);
}

FRONTEND_API QRect getScreenRect(QWidget *widget)
{
	return getMonitorRect(getMonitor(widget));
}

FRONTEND_API QRect getScreenAvailableRect(QWidget *widget)
{
	return getMonitorAvailableRect(getMonitor(widget));
}

FRONTEND_API QRect getScreenAvailableRect(const QPoint &pt)
{
	return getMonitorAvailableRect(getMonitor(pt));
}

FRONTEND_API QRect getScreenAvailableRect(PLSWidgetDpiAdapter *adapter)
{
	return getMonitorAvailableRect(getMonitor(adapter));
}

FRONTEND_API QRect getScreenRect(PLSWidgetDpiAdapter *adapter)
{
	return getMonitorRect(getMonitor(adapter));
}

FRONTEND_API void normalGeometry(PLSWidgetDpiAdapter *adapter, const QRect &screenAvailableGeometry, QRect &geometryOfNormal)
{
	extern void rectLimitTo(QRect & target, const QRect &limited);

	if (geometryOfNormal.left() < screenAvailableGeometry.left()) {
		geometryOfNormal.moveLeft(screenAvailableGeometry.left());
	}
	if (geometryOfNormal.top() < screenAvailableGeometry.top()) {
		geometryOfNormal.moveTop(screenAvailableGeometry.top());
	}

	if (geometryOfNormal.width() <= screenAvailableGeometry.width() && geometryOfNormal.height() <= screenAvailableGeometry.height()) {
		rectLimitTo(geometryOfNormal, screenAvailableGeometry);
	} else if (QRect geometry = adapter->showAsDefaultSize(geometryOfNormal, screenAvailableGeometry); geometry.isValid()) {
		geometryOfNormal = geometry;
		rectLimitTo(geometryOfNormal, screenAvailableGeometry);
	} else {
		rectLimitTo(geometryOfNormal, screenAvailableGeometry);
	}
}

FRONTEND_API void centerGeometry(const QRect &screenAvailableGeometry, QRect &geometryOfNormal)
{
	extern void rectLimitTo(QRect & target, const QRect &limited);

	geometryOfNormal = QRect(screenAvailableGeometry.x() + (screenAvailableGeometry.width() - geometryOfNormal.width()) / 2,
				 screenAvailableGeometry.y() + (screenAvailableGeometry.height() - geometryOfNormal.height()) / 2, geometryOfNormal.width(), geometryOfNormal.height());
	rectLimitTo(geometryOfNormal, screenAvailableGeometry);
}

FRONTEND_API QRect fullscreenShow(PLSWidgetDpiAdapter *adapter, const QRect &screenRect, QRect &geometryOfNormal)
{
	normalGeometry(adapter, getScreenAvailableRect(adapter), geometryOfNormal);
	adapter->selfWidget()->setGeometry(screenRect);
	QWidget *widget = adapter->selfWidget();
	PLS_DEBUG(TOPLEVELVIEW_MODULE, "fullscreen show, className: %s, objectName: %s, screenRect: (%d, %d, %d, %d), geometryOfNormal: (%d, %d, %d, %d)", widget->metaObject()->className(),
		  widget->objectName().toUtf8().constData(), screenRect.x(), screenRect.y(), screenRect.width(), screenRect.height(), geometryOfNormal.x(), geometryOfNormal.y(),
		  geometryOfNormal.width(), geometryOfNormal.height());
	return screenRect;
}

FRONTEND_API QRect fullscreenShow(PLSWidgetDpiAdapter *adapter, QRect &geometryOfNormal)
{
	return fullscreenShow(adapter, getScreenRect(adapter), geometryOfNormal);
}

FRONTEND_API QRect fullscreenShowForce(PLSWidgetDpiAdapter *adapter, QRect &geometryOfNormal)
{
	QRect screenRect = getScreenRect(adapter);
	normalGeometry(adapter, getScreenAvailableRect(adapter), geometryOfNormal);
	QWidget *widget = adapter->selfWidget();
	widget->setGeometry(screenRect.adjusted(1, 1, -1, -1));
	widget->setGeometry(screenRect);
	PLS_DEBUG(TOPLEVELVIEW_MODULE, "force fullscreen show, className: %s, objectName: %s, screenRect: (%d, %d, %d, %d), geometryOfNormal: (%d, %d, %d, %d)", widget->metaObject()->className(),
		  widget->objectName().toUtf8().constData(), screenRect.x(), screenRect.y(), screenRect.width(), screenRect.height(), geometryOfNormal.x(), geometryOfNormal.y(),
		  geometryOfNormal.width(), geometryOfNormal.height());
	return screenRect;
}

FRONTEND_API QRect maximizeShow(PLSWidgetDpiAdapter *adapter, const QRect &screenAvailableRect, QRect &geometryOfNormal)
{
	normalGeometry(adapter, screenAvailableRect, geometryOfNormal);
	QWidget *widget = adapter->selfWidget();
	widget->setGeometry(screenAvailableRect);
	PLS_DEBUG(TOPLEVELVIEW_MODULE, "maximize show, className: %s, objectName: %s, screenAvailableRect: (%d, %d, %d, %d), geometryOfNormal: (%d, %d, %d, %d)", widget->metaObject()->className(),
		  widget->objectName().toUtf8().constData(), screenAvailableRect.x(), screenAvailableRect.y(), screenAvailableRect.width(), screenAvailableRect.height(), geometryOfNormal.x(),
		  geometryOfNormal.y(), geometryOfNormal.width(), geometryOfNormal.height());
	return screenAvailableRect;
}

FRONTEND_API QRect maximizeShow(PLSWidgetDpiAdapter *adapter, QRect &geometryOfNormal)
{
	return maximizeShow(adapter, getScreenAvailableRect(adapter), geometryOfNormal);
}

FRONTEND_API QRect normalShow(PLSWidgetDpiAdapter *adapter, const QRect &screenAvailableGeometry, QRect &geometryOfNormal)
{
	QWidget *widget = adapter->selfWidget();
	normalGeometry(adapter, screenAvailableGeometry, geometryOfNormal);
	widget->setGeometry(geometryOfNormal);
	PLS_DEBUG(TOPLEVELVIEW_MODULE, "normal show, className: %s, objectName: %s, screenAvailableGeometry: (%d, %d, %d, %d), geometryOfNormal: (%d, %d, %d, %d)", widget->metaObject()->className(),
		  widget->objectName().toUtf8().constData(), screenAvailableGeometry.x(), screenAvailableGeometry.y(), screenAvailableGeometry.width(), screenAvailableGeometry.height(),
		  geometryOfNormal.x(), geometryOfNormal.y(), geometryOfNormal.width(), geometryOfNormal.height());
	return geometryOfNormal;
}

FRONTEND_API QRect normalShow(PLSWidgetDpiAdapter *adapter, QRect &geometryOfNormal)
{
	return normalShow(adapter, getScreenAvailableRect(adapter), geometryOfNormal);
}

FRONTEND_API void setGeometrySys(PLSWidgetDpiAdapter *adapter, const QRect &geometry)
{
	QWidget *widget = adapter->selfWidget();
	widget->setGeometry(geometry);
	MoveWindow((HWND)widget->winId(), geometry.x(), geometry.y(), geometry.width(), geometry.height(), TRUE);
}

FRONTEND_API void setGeometrySys(QWidget *widget, const QRect &geometry)
{
	if (PLSWidgetDpiAdapter *adapter = getDpiAdapter(widget); adapter) {
		setGeometrySys(adapter, geometry);
	} else {
		widget->setGeometry(geometry);
	}
}

FRONTEND_API QRect centerShow(PLSWidgetDpiAdapter *adapter, const QRect &screenAvailableGeometry, QRect &geometryOfNormal)
{
	QWidget *widget = adapter->selfWidget();
	normalGeometry(adapter, screenAvailableGeometry, geometryOfNormal);
	centerGeometry(screenAvailableGeometry, geometryOfNormal);
	widget->setGeometry(geometryOfNormal);
	PLS_DEBUG(TOPLEVELVIEW_MODULE, "center show, className: %s, objectName: %s, screenAvailableGeometry: (%d, %d, %d, %d), geometryOfNormal: (%d, %d, %d, %d)", widget->metaObject()->className(),
		  widget->objectName().toUtf8().constData(), screenAvailableGeometry.x(), screenAvailableGeometry.y(), screenAvailableGeometry.width(), screenAvailableGeometry.height(),
		  geometryOfNormal.x(), geometryOfNormal.y(), geometryOfNormal.width(), geometryOfNormal.height());
	return geometryOfNormal;
}

FRONTEND_API QRect centerShow(PLSWidgetDpiAdapter *adapter, QRect &geometryOfNormal)
{
	return centerShow(adapter, getScreenAvailableRect(adapter), geometryOfNormal);
}

FRONTEND_API bool toplevelViewNativeEvent(PLSWidgetDpiAdapter *adapter, const QByteArray &eventType, void *message, long *result,
					  std::function<bool(const QByteArray &, void *, long *)> baseNativeEvent, bool isMaxState, bool isFullScreenState)
{
	QWidget *widget = adapter->selfWidget();

	bool retval = baseNativeEvent(eventType, message, result);

	MSG *msg = (MSG *)message;
	if (msg->message == WM_SIZE) {
		if (isFullScreenState) {
			QRect screenRect = getScreenRect(adapter);
			screenRect.setSize(screenRect.size().expandedTo(widget->minimumSize()).boundedTo(widget->maximumSize()));
			QSize currentSize = QSize(LOWORD(msg->lParam), HIWORD(msg->lParam));
			if (currentSize != screenRect.size()) {
				setGeometrySys(adapter, screenRect);
			}
		} else if (isMaxState) {
			QRect screenAvailableRect = getScreenAvailableRect(adapter);
			screenAvailableRect.setSize(screenAvailableRect.size().expandedTo(widget->minimumSize()).boundedTo(widget->maximumSize()));
			QSize currentSize = QSize(LOWORD(msg->lParam), HIWORD(msg->lParam));
			if (currentSize != screenAvailableRect.size()) {
				setGeometrySys(adapter, screenAvailableRect);
			}
		}
	}
	return retval;
}

PLSDialogView::PLSDialogView(QWidget *parent, PLSDpiHelper dpiHelper) : ToplevelView(parent, Qt::Dialog | Qt::FramelessWindowHint), ui(new Ui::PLSDialogView)
{
	dpiHelper.setCss(this, {PLSCssIndex::Common, PLSCssIndex::QCheckBox, PLSCssIndex::QLineEdit, PLSCssIndex::QMenu, PLSCssIndex::QPlainTextEdit, PLSCssIndex::QPushButton,
				PLSCssIndex::QRadioButton, PLSCssIndex::QScrollBar, PLSCssIndex::QSlider, PLSCssIndex::QSpinBox, PLSCssIndex::QTableView, PLSCssIndex::QTabWidget,
				PLSCssIndex::QTextEdit, PLSCssIndex::QToolButton, PLSCssIndex::QToolTip, PLSCssIndex::QComboBox, PLSCssIndex::PLSDialogView});

	ui->setupUi(this);
	setMouseTracking(true);
	ui->titleBar->setMouseTracking(true);
	ui->contentBorder->setMouseTracking(true);
	ui->content->setMouseTracking(true);
	ui->titleLabel->setMouseTracking(this);
	ui->titleBar->installEventFilter(this);
	ui->contentBorder->installEventFilter(this);
	ui->content->installEventFilter(this);
	ui->titleLabel->installEventFilter(this);

	owidget = nullptr;
	captionHeight = captionButtonSize = GetSystemMetrics(SM_CYCAPTION);
	captionButtonMargin = 0;
	textMarginLeft = 8;
	closeButtonMarginRight = 0;
	hasCaption = true;
	hasHLine = true;
	hasBorder = true;
	hasMinButton = false;
	hasMaxResButton = false;
	hasCloseButton = true;
	isMoveInContent = false;
	closeEventCallback = [this](QCloseEvent *e) {
		callBaseCloseEvent(e);
		return true;
	};
	isEscapeCloseEnabled = false;

	this->ui->min->setVisible(hasMinButton);
	this->ui->maxres->setVisible(hasMaxResButton);

	connect(ui->close, &QToolButton::clicked, [this]() {
		PLS_UI_STEP(getModuleName(), (getViewName() + "'s Close Button").c_str(), ACTION_CLICK);
		reject();
	});
	QObject::connect(ui->maxres, &QToolButton::clicked, [this]() {
		PLS_UI_STEP(getModuleName(), (getViewName() + "'s Maximized Button").c_str(), ACTION_CLICK);
		if (!isMaxState && !isFullScreenState) {
			showMaximized();
		} else {
			showNormal();
		}
	});
	connect(ui->min, &QToolButton::clicked, [this]() {
		PLS_UI_STEP(getModuleName(), (getViewName() + "'s Minimized Button").c_str(), ACTION_CLICK);
		showMinimized();
	});

	connect(this, &PLSDialogView::windowTitleChanged, ui->titleLabel, &QLabel::setText);
	connect(ui->content, &QWidget::windowTitleChanged, this, &PLSDialogView::setWindowTitle);

	ui->content->setObjectName(QString());
	connect(ui->content, &QObject::objectNameChanged, [this](const QString &objectName) {
		if (objectName != CONTENT) {
			setObjectName(objectName);
			ui->content->setObjectName(CONTENT);
		}
	});
}

PLSDialogView::~PLSDialogView()
{
	delete ui;
}

using widgetPtr = QPointer<QWidget>;
static QContiguousCache<widgetPtr> widgetStack(20);

int PLSDialogView::exec()
{
	widgetPtr lastAvailable = nullptr;
	QWidget *lastParent = this->parentWidget();
	if (lastParent == nullptr && !this->objectName().contains("PLSLoginView")) {
		lastAvailable = pls_get_main_view();
	}

	if (!widgetStack.isEmpty()) {
		auto last = widgetStack.last();
		if (last != lastAvailable) {
			lastAvailable = last;
		}
	}

	auto lastFlags = this->windowFlags();
	if (!lastAvailable.isNull()) {
		this->setParent(lastAvailable, lastFlags);
	}

	widgetStack.append(widgetPtr(this));
	int ret = ToplevelView::exec();
	widgetStack.removeLast();
	return ret;
}

QWidget *PLSDialogView::content() const
{
	return ui->content;
}

QWidget *PLSDialogView::widget() const
{
	return owidget;
}

void PLSDialogView::setWidget(QWidget *widget)
{
	this->owidget = widget;
	widget->setParent(ui->content);
	connect(widget, &QWidget::windowTitleChanged, this, &PLSDialogView::setWindowTitle);
	setWindowTitle(widget->windowTitle());

	QHBoxLayout *l = new QHBoxLayout(ui->content);
	l->setMargin(0);
	l->setSpacing(0);
	l->addWidget(widget);
}

int PLSDialogView::getCaptionHeight() const
{
	return captionHeight;
}

void PLSDialogView::setCaptionHeight(int captionHeight)
{
	int borderWidth = hasBorder ? BORDER_WIDTH() : 0;
	this->captionHeight = captionHeight - borderWidth;
	ui->titleBar->setFixedHeight(this->captionHeight);
}

int PLSDialogView::getCaptionButtonSize() const
{
	return captionButtonSize;
}

void PLSDialogView::setCaptionButtonSize(int captionButtonSize)
{
	this->captionButtonSize = captionButtonSize;
	ui->min->setFixedSize(captionButtonSize, captionButtonSize);
	ui->maxres->setFixedSize(captionButtonSize, captionButtonSize);
	ui->close->setFixedSize(captionButtonSize, captionButtonSize);
}

int PLSDialogView::getCaptionButtonMargin() const
{
	return captionButtonMargin;
}

void PLSDialogView::setCaptionButtonMargin(int captionButtonMargin)
{
	this->captionButtonMargin = captionButtonMargin;
	ui->titleButtonLayout->setSpacing(captionButtonMargin);
}

int PLSDialogView::getTextMarginLeft() const
{
	return textMarginLeft;
}

void PLSDialogView::setTextMarginLeft(int textMarginLeft)
{
	this->textMarginLeft = textMarginLeft;
	QMargins margins = ui->titleBarLayout->contentsMargins();
	margins.setLeft(textMarginLeft);
	ui->titleBarLayout->setContentsMargins(margins);
	PLSDpiHelper::setDynamicContentsMargins(ui->titleBarLayout, true);
}

int PLSDialogView::getCloseButtonMarginRight() const
{
	return closeButtonMarginRight;
}

void PLSDialogView::setCloseButtonMarginRight(int closeButtonMarginRight)
{
	this->closeButtonMarginRight = closeButtonMarginRight;
	QMargins margins = ui->titleBarLayout->contentsMargins();
	margins.setRight(closeButtonMarginRight);
	ui->titleBarLayout->setContentsMargins(margins);
	PLSDpiHelper::setDynamicContentsMargins(ui->titleBarLayout, true);
}

bool PLSDialogView::getHasCaption() const
{
	return hasCaption;
}

void PLSDialogView::setHasCaption(bool hasCaption)
{
	this->hasCaption = hasCaption;
	ui->close->setVisible(hasCaption);
	ui->min->setVisible(hasCaption && hasMinButton);
	ui->titleBar->setVisible(hasCaption);
}

bool PLSDialogView::getHasHLine() const
{
	return hasHLine;
}

void PLSDialogView::setHasHLine(bool hasHLine)
{
	this->hasHLine = hasHLine;
	ui->hline->setVisible(hasHLine);
}

bool PLSDialogView::getHasBorder() const
{
	return hasBorder;
}

void PLSDialogView::setHasBorder(bool hasBorder)
{
	this->hasBorder = hasBorder;
	update();
}

bool PLSDialogView::getHasMinButton() const
{
	return hasMinButton;
}

bool PLSDialogView::getHasMaxResButton() const
{
	return hasMaxResButton;
}

void PLSDialogView::setHasMaxResButton(bool hasMaxResButton)
{
	this->hasMaxResButton = hasMaxResButton;
	ui->maxres->setVisible(hasMaxResButton);
}

void PLSDialogView::setHasMinButton(bool hasMinButton)
{
	this->hasMinButton = hasMinButton;
	ui->min->setVisible(hasMinButton);
}

bool PLSDialogView::getHasCloseButton() const
{
	return hasCloseButton;
}

void PLSDialogView::setHasCloseButton(bool hasCloseButton)
{
	this->hasCloseButton = hasCloseButton;
	ui->close->setVisible(hasCloseButton);
}

bool PLSDialogView::getEscapeCloseEnabled() const
{
	return isEscapeCloseEnabled;
}

void PLSDialogView::setEscapeCloseEnabled(bool enabled)
{
	isEscapeCloseEnabled = enabled;
}

void PLSDialogView::setHasBackgroundTransparent(bool isTransparent)
{
	if (isTransparent) {
		setAttribute(Qt::WA_TranslucentBackground, true);
		ui->content->setAttribute(Qt::WA_TranslucentBackground, true);
		ui->contentBorder->setAttribute(Qt::WA_TranslucentBackground, true);
	} else {
		setAttribute(Qt::WA_TranslucentBackground, false);
		ui->content->setAttribute(Qt::WA_TranslucentBackground, false);
		ui->contentBorder->setAttribute(Qt::WA_TranslucentBackground, false);
	}
}

void PLSDialogView::setCloseEventCallback(std::function<bool(QCloseEvent *)> closeEventCallback)
{
	this->closeEventCallback = closeEventCallback;
}

void PLSDialogView::callBaseCloseEvent(QCloseEvent *event)
{
	if (event) {
		ToplevelView::closeEvent(event);
	}
}

void PLSDialogView::sizeToContent(const QSize &size)
{
	QSize newSize = ui->content->layout()->minimumSize();
	if (size.isValid()) {
		newSize.setWidth(qMax(size.width(), newSize.width()));
		newSize.setHeight(qMax(size.height(), newSize.height()));
	}

	if (hasCaption || hasBorder) {
		int BORDER_WIDTH = this->BORDER_WIDTH();

		if (hasCaption) {
			newSize.setHeight(newSize.height() + captionHeight + (hasHLine && hasBorder ? BORDER_WIDTH : 0));
		}

		if (hasBorder) {
			newSize.setWidth(newSize.width() + BORDER_WIDTH * 2);
			newSize.setHeight(newSize.height() + BORDER_WIDTH * 2);
		}
	}

	resize(newSize);
}

void PLSDialogView::setHeightForFixedWidth()
{
	int fixedWidth = minimumWidth();
	int height = layout()->minimumHeightForWidth(fixedWidth);
	setFixedHeight(height);
}

static inline QPoint operator+(const QPoint &pt, const QSize &sz)
{
	return QPoint(pt.x() + sz.width(), pt.y() + sz.height());
}

void PLSDialogView::moveToCenter()
{
	QWidget *toplevel = pls_get_toplevel_view(parentWidget());
	if (toplevel)
		move(toplevel->mapToGlobal(QPoint(0, 0)) + (toplevel->size() - this->size()) / 2);
}

const char *PLSDialogView::getModuleName() const
{
	return MODULE_FRONTEND_API;
}

std::string PLSDialogView::getViewName() const
{
	return metaObject()->className();
}

void PLSDialogView::done(int result)
{
	if (closeEventCallback(nullptr)) {
		ToplevelView::done(result);
	}
}

QRect PLSDialogView::titleBarRect() const
{
	return hasCaption ? ui->titleBar->geometry() : QRect();
}

bool PLSDialogView::canMaximized() const
{
	return hasCaption && hasMaxResButton;
}

bool PLSDialogView::canFullScreen() const
{
	return false;
}

void PLSDialogView::flushMaxFullScreenStateStyle()
{
	pls_flush_style(ui->maxres);
}

void PLSDialogView::closeEvent(QCloseEvent *event)
{
	closeEventCallback(event);
}

void PLSDialogView::showEvent(QShowEvent *event)
{
	emit shown();
	ToplevelView::showEvent(event);
}

void PLSDialogView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() != Qt::Key_Escape) {
		ToplevelView::keyPressEvent(event);
	} else if (isEscapeCloseEnabled) {
		event->accept();
		close();
	} else {
		event->ignore();
	}
}

void PLSDialogView::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() != Qt::Key_Escape) {
		ToplevelView::keyReleaseEvent(event);
	} else {
		event->ignore();
	}
}

void PLSDialogView::mousePressEvent(QMouseEvent *event)
{
	mousePress(event);
}

void PLSDialogView::mouseReleaseEvent(QMouseEvent *event)
{
	mouseRelease(event);
}

void PLSDialogView::mouseDoubleClickEvent(QMouseEvent *event)
{
	mouseDbClick(event);
}

void PLSDialogView::mouseMoveEvent(QMouseEvent *event)
{
	mouseMove(event);
}

bool PLSDialogView::eventFilter(QObject *watcher, QEvent *event)
{
	if (watcher == ui->content) {
		switch (event->type()) {
		case QEvent::ChildAdded:
			if (QWidget *widget = dynamic_cast<QWidget *>(static_cast<QChildEvent *>(event)->child()); widget) {
				widget->setCursor(Qt::ArrowCursor);
			}
			break;
		}
	}

	if (watcher == this || watcher == ui->titleBar || watcher == ui->content || watcher == ui->contentBorder) {
		switch (event->type()) {
		case QEvent::MouseButtonPress:
			mousePress(static_cast<QMouseEvent *>(event));
			break;
		case QEvent::MouseButtonRelease:
			mouseRelease(static_cast<QMouseEvent *>(event));
			break;
		case QEvent::MouseButtonDblClick:
			mouseDbClick(static_cast<QMouseEvent *>(event));
			break;
		case QEvent::MouseMove:
			mouseMove(static_cast<QMouseEvent *>(event));
			break;
		}
	}

	if (watcher == ui->titleLabel) {
		switch (event->type()) {
		case QEvent::Resize:
			ui->titleLabel->setText(QFontMetrics(ui->titleLabel->font()).elidedText(windowTitle(), Qt::ElideRight, static_cast<QResizeEvent *>(event)->size().width()));
			break;
		}
	}

	return ToplevelView::eventFilter(watcher, event);
}
