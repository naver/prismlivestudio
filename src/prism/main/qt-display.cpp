#include "qt-display.hpp"
#include "qt-wrappers.hpp"
#include "display-helpers.hpp"
#include "pls-app.hpp"
#include "pls-common-define.hpp"
#include <QWindow>
#include <QScreen>
#include <QResizeEvent>
#include <QShowEvent>
#include <QDebug>
#include "pls-common-define.hpp"
#include "source-tree.hpp"
#include "window-dock.hpp"
#include "action.h"
#include "liblog.h"
#include "log/module_names.h"
#include <algorithm>

static inline long long color_to_int(const QColor &color)
{
	auto shift = [&](unsigned val, int shift) { return ((val & 0xff) << shift); };

	return shift(color.red(), 0) | shift(color.green(), 8) | shift(color.blue(), 16) | shift(color.alpha(), 24);
}

static inline QColor rgba_to_color(uint32_t rgba)
{
	return QColor::fromRgb(rgba & 0xFF, (rgba >> 8) & 0xFF, (rgba >> 16) & 0xFF, (rgba >> 24) & 0xFF);
}

static QWidget *getDock(PLSQTDisplay *display)
{
	for (QWidget *dock = display; dock; dock = dock->parentWidget()) {
		if (dynamic_cast<PLSDock *>(dock)) {
			return dock;
		}
	}
	return nullptr;
}

void PLSQTDisplay::OnSourceCaptureState(void *data, calldata_t *calldata)
{
	QMetaObject::invokeMethod(static_cast<PLSQTDisplay *>(data), "OnSourceStateChanged");
}

PLSQTDisplay::PLSQTDisplay(QWidget *parent, Qt::WindowFlags flags, PLSDpiHelper dpiHelper) : QWidget(parent, flags)
{
	PLS_INFO(DISPLAY_MODULE, "[%p] display is created", this);

	dpiHelper.setCss(this, {PLSCssIndex::PLSQTDisplay});

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_StaticContents);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	setAttribute(Qt::WA_NativeWindow);

	auto windowVisible = [this](bool visible) {
		if (!visible)
			return;

		if (!display) {
			CreateDisplay();
		} else {
			resizeManual();
		}
	};

	auto sizeChanged = [this](QScreen *) {
		CreateDisplay();

		resizeManual();
	};

	connect(windowHandle(), &QWindow::visibleChanged, windowVisible);
	connect(windowHandle(), &QWindow::screenChanged, sizeChanged);

	displayText = new QLabel(this);
	displayText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	displayText->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	displayText->setObjectName(INVALID_SOURCE_ERROR_TEXT_BUTTON);
	displayText->setWordWrap(true);
	displayText->hide();

	resizeScreen = new QLabel(this);
	resizeScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	resizeScreen->setObjectName(DISPLAY_RESIZE_SCREEN);
	resizeScreen->hide();

	resizeCenter = new QLabel(resizeScreen);
	resizeCenter->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	resizeCenter->setObjectName(DISPLAY_RESIZE_CENTER);
	resizeCenter->setGeometry(0, 0, 0, 0);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(displayText);
	layout->addWidget(resizeScreen);

	if (QWidget *dock = getDock(this)) {
		connect(dock, SIGNAL(beginResizeSignal()), this, SLOT(beginResizeSlot()));
		connect(dock, SIGNAL(endResizeSignal()), this, SLOT(endResizeSlot()));
		connect(dock, SIGNAL(visibleSignal(bool)), this, SLOT(visibleSlot(bool)), Qt::QueuedConnection);
	} else {
		QWidget *toplevelView = pls_get_toplevel_view(this);
		connect(toplevelView, SIGNAL(beginResizeSignal()), this, SLOT(beginResizeSlot()));
		connect(toplevelView, SIGNAL(endResizeSignal()), this, SLOT(endResizeSlot()));
		connect(toplevelView, SIGNAL(visibleSignal(bool)), this, SLOT(visibleSlot(bool)), Qt::QueuedConnection);
	}

	resizeScreen->installEventFilter(this);
}

PLSQTDisplay::~PLSQTDisplay()
{
	PLS_INFO(DISPLAY_MODULE, "[%p] display is deleted", this);
}

QColor PLSQTDisplay::GetDisplayBackgroundColor() const
{
	return rgba_to_color(backgroundColor);
}

void PLSQTDisplay::SetDisplayBackgroundColor(const QColor &color)
{
	uint32_t newBackgroundColor = (uint32_t)color_to_int(color);

	if (newBackgroundColor != backgroundColor) {
		backgroundColor = newBackgroundColor;
		UpdateDisplayBackgroundColor();
	}
}

void PLSQTDisplay::UpdateDisplayBackgroundColor()
{
	obs_display_set_background_color(display, backgroundColor);
}

void PLSQTDisplay::CreateDisplay()
{
	if (display || !windowHandle()->isExposed())
		return;

	QSize size = GetPixelSize(this);

	gs_init_data info = {};
	info.cx = size.width();
	info.cy = size.height();
	info.format = GS_BGRA;
	info.zsformat = GS_ZS_NONE;

	QTToGSWindow(winId(), info.window);

	display = obs_display_create(&info, backgroundColor);

	emit DisplayCreated(this);
}

void PLSQTDisplay::AdjustResizeUI()
{
	bool handled = false;
	emit AdjustResizeView(resizeScreen, resizeCenter, handled);
	if (handled)
		return;

	QSize size = GetPixelSize(resizeScreen);
	if (!source) {
		resizeCenter->setGeometry(0, 0, size.width(), size.height());
		return;
	}

	uint32_t sourceCX = std::max(obs_source_get_width(source), 1u);
	uint32_t sourceCY = std::max(obs_source_get_height(source), 1u);
	int x(0), y(0);
	int newCX(0), newCY(0);
	float scale(1.f);
	GetScaleAndCenterPos(sourceCX, sourceCY, size.width(), size.height(), x, y, scale);
	newCX = static_cast<int>(scale * static_cast<float>(sourceCX));
	newCY = static_cast<int>(scale * static_cast<float>(sourceCY));
	resizeCenter->setGeometry(x, y, newCX, newCY);
}

bool PLSQTDisplay::eventFilter(QObject *object, QEvent *event)
{
	if (object == resizeScreen && resizeCenter) {
		if (event->type() == QEvent::Resize && resizeScreen->isVisible())
			AdjustResizeUI();

		if (event->type() == QEvent::Show)
			AdjustResizeUI();
	}

	return __super::eventFilter(object, event);
}

void PLSQTDisplay::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	CreateDisplay();

	if (isVisible() && display) {
		resizeManual();
	}

	emit DisplayResized();
}

void PLSQTDisplay::paintEvent(QPaintEvent *event)
{
	CreateDisplay();

	QWidget::paintEvent(event);
}

void PLSQTDisplay::resizeManual()
{
	if (!display) {
		return;
	}
	QSize size = GetPixelSize(this);
	obs_display_resize(display, size.width(), size.height());
}

QPaintEngine *PLSQTDisplay::paintEngine() const
{
	return nullptr;
}

void PLSQTDisplay::OnSourceStateChanged()
{
	if (source)
		UpdateSourceState(source);
}

void PLSQTDisplay::UpdateSourceState(obs_source_t *source)
{
	const char *id = obs_source_get_id(source);
	if (!id) {
		setDisplayTextVisible(false);
		return;
	}

	enum obs_source_error error;
	if (obs_source_get_capture_valid(source, &error)) {
		setDisplayTextVisible(false);
		return;
	}

	if (!displayTextAsGuide)
		displayText->setText(SourceTreeItem::GetErrorTips(id, error));
	if (!isResizing)
		setDisplayTextVisible(true);
}

void PLSQTDisplay::beginResizeSlot()
{
	if (isResizing) {
		PLS_ERROR(DISPLAY_MODULE, "[%p] repeated start resize", this);
		return;
	}

	isResizing = true;
	isDisplayActive = obs_display_enabled(display);

	obs_display_set_enabled(display, false);

	enum obs_source_error error;
	if (obs_source_get_capture_valid(source, &error))
		setDisplayTextVisible(false);
	if (!displayText->isVisible())
		resizeScreen->show();
}

void PLSQTDisplay::endResizeSlot()
{
	if (!isResizing) {
		PLS_ERROR(DISPLAY_MODULE, "[%p] unknown end resize", this);
		return;
	}

	isResizing = false;
	obs_display_set_enabled(display, isDisplayActive);

	resizeScreen->hide();
	if (source) {
		enum obs_source_error error;
		if (!obs_source_get_capture_valid(source, &error))
			setDisplayTextVisible(true);
	}
}

void PLSQTDisplay::showGuideText(const QString &guideText)
{
	displayTextAsGuide = true;
	displayText->setText(guideText);
	displayText->show();
}

void PLSQTDisplay::setDisplayTextVisible(bool visible)
{
	if (!displayTextAsGuide) {
		displayText->setVisible(visible);
	}
}

void PLSQTDisplay::visibleSlot(bool visible)
{
	if (property("forceHidden").toBool()) {
		setVisible(false);
	} else {
		setVisible(visible);
	}
}
