#include "PLSRegionCapture.h"
#include "PLSDpiHelper.h"
#include "pls-app.hpp"
#include "log/module_names.h"
#include "liblog.h"
#include "action.h"
#include <QScreen>
#include <QPainter>
#include <QGuiApplication>
#include <QWindow>
#include <QDebug>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <set>
#include <QMutex>
#include <windows.h>
#include <strsafe.h>

const static int TIP_MARGIN = 5;
const static int TIP_PADDING_H = 40;
const static int TIP_PADDING_V = 5;
const static int MENU_FRAME_WIDTH = 80;
const static int MENU_FRAME_HEIGHT = 40;
const static int MENU_BUTTON_WIDTH = 22;
const static int MENU_BUTTON_HEIGHT = 22;
const static int TIP_FONT_SIZE = 12;
const static QColor &tipRectColor = QColor("#effc35");
const static QColor &errorTextColor = QColor("#e02020");
const static QColor &normalTextColor = Qt::black;
const static QColor &selectRectBgColor = QColor(255, 255, 255, 51);
const static QColor &selectRectBorderColor = QColor("#effc35");
const static QColor &backgroundColor = QColor(0, 0, 0, 1);

HHOOK hhook = NULL;
using PARAM = void *;
std::vector<PARAM> hookedWidget;

QMutex mute;

/**************************************************************** 
  WH_KEYBOARD hook procedure 
 ****************************************************************/

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) // do not process message
		return CallNextHookEx(hhook, nCode, wParam, lParam);
	if (wParam == WM_KEYDOWN) {
		auto hookStruct = *((KBDLLHOOKSTRUCT *)lParam);
		if (hookStruct.vkCode == VK_ESCAPE) {
			QMutexLocker locker(&mute);
			auto size = hookedWidget.size();
			if (size > 0) {
				auto param = hookedWidget.at(size - 1);
				PLSRegionCapture *w = reinterpret_cast<PLSRegionCapture *>(param);
				QMetaObject::invokeMethod(w, "close", Qt::QueuedConnection);
				PLS_UI_STEP(MAIN_AREA_CAPTURE, "Esc button", ACTION_CLICK);
			}
		}
	}

	return CallNextHookEx(hhook, nCode, wParam, lParam);
}

PLSRegionCapture::PLSRegionCapture(QWidget *parent) : QWidget(parent), labelRect(nullptr)
{
	this->setMouseTracking(true);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAttribute(Qt::WA_DeleteOnClose);

	penLine.setWidth(1);
	penLine.setColor(QColor(30, 255, 255));

	penBorder.setWidth(2);
	penBorder.setColor(selectRectBorderColor);

	connect(qApp, &QGuiApplication::screenAdded, this, [=](QScreen *screen) {
		qInfo("screen Added, recalculate frame rect");
		rectWhole = CalculateFrameSize();
		this->resize(rectWhole.size());
		this->move(rectWhole.x(), rectWhole.y());
	});
	connect(qApp, &QGuiApplication::screenRemoved, this, [=](QScreen *screen) {
		auto gemetry = screen->geometry();
		qInfo("screen removed, recalculate frame rect");
		rectWhole = CalculateFrameSize();
		this->resize(rectWhole.size());
		this->move(rectWhole.x(), rectWhole.y());
		auto selectRect = QRect(mapToGlobal(rectSelected.topLeft()), rectSelected.size());
		if (gemetry.intersects(selectRect)) {
			Init();
		}
	});

	labelRect = new QLabel(this);
	labelRect->setObjectName("labelRect");
	labelRect->setStyleSheet("#labelRect{border:2px solid #effc35;background-color:rgba(255, 255, 255, 0.2);}");
	labelRect->hide();

	menuFrame = new QFrame(this);
	menuFrame->setAttribute(Qt::WA_StyledBackground);
	menuFrame->installEventFilter(this);
	menuFrame->setObjectName("frameMenu");

	labelTip = new QLabel(this);
	labelTip->setAttribute(Qt::WA_TransparentForMouseEvents);
	labelTip->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	labelTip->setObjectName("CaptureTipLabel");

	auto btn_ok = new QPushButton(menuFrame);
	btn_ok->setFocusPolicy(Qt::NoFocus);
	btn_ok->setObjectName("confirmBtn");
	connect(btn_ok, &QPushButton::clicked, this, [=]() {
		PLS_UI_STEP(MAIN_AREA_CAPTURE, "ok button", ACTION_CLICK);
		drawing = false;
		repaint();
		QPoint pos(this->mapToGlobal(rectSelected.topLeft()));
		qInfo() << "selected region point=" << pos << "size=" << rectSelected.size();
		emit selectedRegion({pos, rectSelected.size()});
		close();
	});
	auto btn_cancel = new QPushButton(menuFrame);
	btn_cancel->setFocusPolicy(Qt::NoFocus);
	btn_cancel->setObjectName("cancelBtn");
	connect(btn_cancel, &QPushButton::clicked, this, [=]() {
		PLS_UI_STEP(MAIN_AREA_CAPTURE, "Cancel button", ACTION_CLICK);
		this->Init();
	});

	QLabel *label_space = new QLabel(menuFrame);
	label_space->setObjectName("label_space");

	QHBoxLayout *layout = new QHBoxLayout(menuFrame);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addStretch();
	layout->addWidget(btn_cancel);
	layout->addStretch();
	layout->addWidget(label_space);
	layout->addStretch();
	layout->addWidget(btn_ok);
	layout->addStretch();
	menuFrame->hide();
}

PLSRegionCapture::~PLSRegionCapture()
{
	ReleaseHook();
}

QRect PLSRegionCapture::GetSelectedRect() const
{
	QPoint pos(this->mapToGlobal(rectSelected.topLeft()));
	qInfo() << "selected region point=" << pos << "size=" << rectSelected.size();
	return {pos, rectSelected.size()};
}

void PLSRegionCapture::StartCapture(uint64_t maxWidth, uint64_t maxHeight)
{
	SetHook();
	maxRegionWidth = maxWidth;
	maxRegionHeight = maxHeight;
	this->setCursor(Qt::CrossCursor);
	rectWhole = CalculateFrameSize();
	qInfo() << "whole rect" << rectWhole;
	this->resize(rectWhole.size());
	this->move(rectWhole.x(), rectWhole.y());
	pointMoved = mapFromGlobal(QCursor::pos());
	SetCurrentPosDpi(mapToGlobal(pointMoved));
	SetCoordinateValue();
	this->raise();
	this->show();
}

void PLSRegionCapture::paintEvent(QPaintEvent *event)
{
	QPainter dc(this);
	drawBackground(&dc, event->rect());
	QWidget::paintEvent(event);
}

void PLSRegionCapture::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		labelRect->hide();
		rectSelected = QRect();
		pressed = true;
		hideMuenus();
		pointPressed = event->pos();
	}
	QWidget::mousePressEvent(event);
}

void PLSRegionCapture::mouseMoveEvent(QMouseEvent *event)
{
	if (pressed)
		UpdateSelectedRect();
	if (rectSelected.isValid()) {
		labelRect->setGeometry(rectSelected);
		labelRect->show();
	}
	pointMoved = event->pos();
	SetCurrentPosDpi(mapToGlobal(pointMoved));
	SetCoordinateValue();
	QWidget::mouseMoveEvent(event);
}

void PLSRegionCapture::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		pressed = false;
		pointReleased = event->pos();
		if (rectSelected.isValid()) {
			labelTip->hide();
			showMenus();
		} else
			labelRect->hide();
	}
	QWidget::mouseReleaseEvent(event);
}

void PLSRegionCapture::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
}

void PLSRegionCapture::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		this->close();
	}
	QWidget::keyPressEvent(event);
}

bool PLSRegionCapture::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == menuFrame) {
		switch (event->type()) {
		case QEvent::MouseButtonPress:
		case QEvent::MouseButtonRelease:
		case QEvent::MouseButtonDblClick:
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}

void PLSRegionCapture::closeEvent(QCloseEvent *event)
{
	ReleaseHook();
	QWidget::closeEvent(event);
}

void PLSRegionCapture::UpdateSelectedRect()
{
	auto width = qAbs(pointMoved.x() - pointPressed.x());
	auto height = qAbs(pointMoved.y() - pointPressed.y());

	if (maxRegionWidth <= 0 || maxRegionHeight <= 0) {
		overWidth = false;
		overHeight = false;
	} else {
		overWidth = (width > maxRegionWidth);
		overHeight = (height > maxRegionHeight);
	}

	int startX = 0, startY = 0;
	//top of pressed point
	if (pointMoved.y() < pointPressed.y()) {
		// left
		if (pointMoved.x() < pointPressed.x()) {
			startX = overWidth ? (pointPressed.x() - maxRegionWidth) : pointMoved.x();
			startY = overHeight ? (pointPressed.y() - maxRegionHeight) : pointMoved.y();
		}
		//right
		if (pointMoved.x() > pointPressed.x()) {
			startX = pointPressed.x();
			startY = overHeight ? (pointPressed.y() - maxRegionHeight) : pointMoved.y();
		}
	}

	//bottom  of pressed point
	if (pointMoved.y() > pointPressed.y()) {
		// left
		if (pointMoved.x() < pointPressed.x()) {
			startX = overWidth ? (pointPressed.x() - maxRegionWidth) : pointMoved.x();
			startY = pointPressed.y();
		}
		//right
		if (pointMoved.x() > pointPressed.x()) {
			startX = pointPressed.x();
			startY = pointPressed.y();
		}
	}

	rectSelected.setRect(startX, startY, overWidth ? maxRegionWidth : width, overHeight ? maxRegionHeight : height);
}

void PLSRegionCapture::drawBackground(QPainter *painter, const QRect &rect)
{
	painter->save();
	painter->setCompositionMode(QPainter::CompositionMode_Source);
	painter->fillRect(rect, drawing ? backgroundColor : Qt::transparent);
	painter->restore();
}

void PLSRegionCapture::drawCenterLines(QPainter *painter)
{
	if (rectSelected.isValid())
		return;
	painter->save();
	painter->setPen(penLine);
	painter->drawLine(QPointF(pointMoved.x(), 0.0), QPointF(pointMoved.x(), this->height()));
	painter->drawLine(QPointF(0.0, pointMoved.y()), QPointF(this->width(), pointMoved.y()));
	painter->restore();
}

void PLSRegionCapture::showMenus()
{
	this->setCursor(Qt::ArrowCursor);
	if (menuFrame) {
		SetMenuCoordinate();
		menuFrame->show();
	}
}

void PLSRegionCapture::hideMuenus()
{
	this->setCursor(Qt::CrossCursor);
	if (menuFrame) {
		menuFrame->hide();
	}
}

void PLSRegionCapture::Init()
{
	pointMoved = mapFromGlobal(QCursor::pos());
	SetCurrentPosDpi(mapToGlobal(pointMoved));
	pointPressed = QPoint();
	pointReleased = QPoint();
	rectSelected = QRect();
	pressed = false;
	if (labelRect) {
		labelRect->setGeometry(0, 0, 0, 0);
		labelRect->hide();
	}
	hideMuenus();
	SetCoordinateValue();
	this->repaint();
}

QRect PLSRegionCapture::CalculateFrameSize()
{
	screenShots.clear();
	QList<QScreen *> listScreen = QGuiApplication::screens();
	int index = 0;
	int left = 0, top = 0, right = 0, bottom = 0;
	for (auto screen : listScreen) {
		qDebug() << "screen" << index << "geometry=" << screen->geometry() << "\nDpi=" << screen->logicalDotsPerInch();
		QRect rect = screen->geometry();
		++index;
		if (rect.left() < left) {
			left = rect.left();
		}

		if (rect.right() > right) {
			right = rect.right();
		}

		if (rect.top() < top) {
			top = rect.top();
		}

		if (rect.bottom() > bottom) {
			bottom = rect.bottom();
		}

		ScreenShotData screenData;
		screenData.pixmap = screen->grabWindow(0);
		screenData.geometry = rect;
		screenShots << screenData;
	}
	return {QPoint(left, top), QPoint(right, bottom)};
}

void PLSRegionCapture::SetTipCoordinate(int tipTextWidth, int tipTextHeight)
{
	auto pos = mapToGlobal(pointMoved);
	auto screen = QGuiApplication::screenAt(pos);
	QRect gloabalSelctedRect(mapToGlobal(rectSelected.topLeft()), rectSelected.size());
	if (screen) {
		QList<QRect> possibleRect;
		possibleRect << QRect(pos.x() + PLSDpiHelper::calculate(screenDpi, 5), pos.y() + PLSDpiHelper::calculate(screenDpi, 5), tipTextWidth, tipTextHeight);
		possibleRect << QRect(pos.x() - tipTextWidth - PLSDpiHelper::calculate(screenDpi, 5), pos.y() + PLSDpiHelper::calculate(screenDpi, 5), tipTextWidth, tipTextHeight);
		possibleRect << QRect(pos.x() + PLSDpiHelper::calculate(screenDpi, 5), pos.y() - tipTextHeight - PLSDpiHelper::calculate(screenDpi, 5), tipTextWidth, tipTextHeight);
		possibleRect << QRect(pos.x() - tipTextWidth - PLSDpiHelper::calculate(screenDpi, 5), pos.y() - PLSDpiHelper::calculate(screenDpi, 5) - tipTextHeight, tipTextWidth, tipTextHeight);

		for (int i = 0; i < possibleRect.size(); ++i) {
			if (screen->geometry().contains(possibleRect.at(i), true)) {
				auto targetPos = mapFromGlobal(possibleRect.at(i).topLeft());
				tipPositon.setX(targetPos.x());
				tipPositon.setY(targetPos.y());
				return;
			}
		}
	}

	tipPositon.setX(pos.x() - tipTextWidth - PLSDpiHelper::calculate(screenDpi, 5));
	tipPositon.setY(pos.y() - PLSDpiHelper::calculate(screenDpi, 5) - tipTextHeight);
}

void PLSRegionCapture::SetMenuCoordinate()
{
	auto pos = mapToGlobal(rectSelected.bottomRight());
	auto screen = QGuiApplication::screenAt(pos);
	bool bottomRight = true;
	if (!screen) {
		bottomRight = false;
		pos = mapToGlobal(pointReleased);
		screen = QGuiApplication::screenAt(pos);
	}
	if (!screen)
		return;
	auto dpi = screen->logicalDotsPerInch() / 100.0;
	int w = PLSDpiHelper::calculate(dpi, MENU_FRAME_WIDTH);
	int h = PLSDpiHelper::calculate(dpi, MENU_FRAME_HEIGHT);
	QRect gloabalSelctedRect(mapToGlobal(rectSelected.topLeft()), rectSelected.size());
	QList<QRect> rectPossible;
	if (bottomRight) {
		rectPossible << QRect(pos.x() - w, pos.y() + PLSDpiHelper::calculate(dpi, 5), w, h);
		rectPossible << QRect(pos.x() + PLSDpiHelper::calculate(dpi, 5), pos.y() - h, w, h);
		rectPossible << QRect(pos.x() - w, pos.y() - rectSelected.height() - h - PLSDpiHelper::calculate(dpi, 5), w, h);
		rectPossible << QRect(pos.x() - rectSelected.width() - w - PLSDpiHelper::calculate(dpi, 5), pos.y() - h, w, h);
	} else {
		rectPossible << QRect(pos.x(), pos.y() + PLSDpiHelper::calculate(dpi, 5), w, h);
		rectPossible << QRect(pos.x(), pos.y() - PLSDpiHelper::calculate(dpi, 5) - h, w, h);
		rectPossible << QRect(pos.x() - w - PLSDpiHelper::calculate(dpi, 5), pos.y() - h, w, h);
		rectPossible << QRect(pos.x() + PLSDpiHelper::calculate(dpi, 5), pos.y() - h, w, h);
	}

	auto setFrameGeometry = [=](int x, int y) {
		menuGeometry.setX(x);
		menuGeometry.setY(y);
		menuGeometry.setWidth(w);
		menuGeometry.setHeight(h);
		menuFrame->setGeometry(menuGeometry);
		menuFrame->findChild<QPushButton *>("confirmBtn")->setFixedSize(PLSDpiHelper::calculate(dpi, MENU_BUTTON_WIDTH), PLSDpiHelper::calculate(dpi, MENU_BUTTON_HEIGHT));
		menuFrame->findChild<QPushButton *>("cancelBtn")->setFixedSize(PLSDpiHelper::calculate(dpi, MENU_BUTTON_WIDTH), PLSDpiHelper::calculate(dpi, MENU_BUTTON_HEIGHT));
		menuFrame->findChild<QLabel *>("label_space")->setFixedWidth(PLSDpiHelper::calculate(dpi, 1));
	};

	for (int i = 0; i < rectPossible.size(); ++i) {
		if (screen->geometry().contains(rectPossible.at(i), true) && !gloabalSelctedRect.contains(rectPossible.at(i))) {
			auto targetPos = mapFromGlobal(rectPossible.at(i).topLeft());
			setFrameGeometry(targetPos.x(), targetPos.y());
			return;
		}
	}
	auto targetPos = mapFromGlobal(pos);
	setFrameGeometry(targetPos.x() - w, targetPos.y() - h);
}

void PLSRegionCapture::SetCurrentPosDpi(const QPoint &posGlobal)
{
	auto screen = QGuiApplication::screenAt(posGlobal);
	auto newDpi = screen->logicalDotsPerInch() / 100.0;
	if (newDpi != screenDpi) {
		screenDpi = newDpi;
		labelTip->setStyleSheet(
			QString::asprintf("#CaptureTipLabel{color:black;background-color: #effc35;font-weight:bold;font-size:%dpx;}", PLSDpiHelper::calculate(screenDpi, TIP_FONT_SIZE)));
		menuFrame->setStyleSheet(QString::asprintf(
			"#frameMenu{background:rgba(39, 39, 39, 0.953);border:%dpx solid rgba(255, 255, 255, 0.098);}#frameMenu > #label_space{background:rgba(255, 255, 255, 0.071);min-width:%dpx;max-width: %dpx;border:none;}",
			PLSDpiHelper::calculate(screenDpi, 1), PLSDpiHelper::calculate(screenDpi, 1), PLSDpiHelper::calculate(screenDpi, 1)));
		pls_flush_style(labelTip);
		pls_flush_style(menuFrame);
	}
}

bool isRegisted(void *param)
{
	auto iter = hookedWidget.cbegin();
	while (iter != hookedWidget.cend()) {
		if (*(iter) == param)
			return true;
		++iter;
	}
	return false;
}

void unregisterWidget(void *param)
{
	auto iter = hookedWidget.cbegin();
	while (iter != hookedWidget.cend()) {
		if (*(iter) == param) {
			hookedWidget.erase(iter);
			break;
		}
		++iter;
	}
}

void PLSRegionCapture::SetHook()
{
	QMutexLocker locker(&mute);
	if (isRegisted(this)) {
		qWarning("PLSRegionCapture: this window has registerd keybord hook");
		return;
	}
	if (hhook) {
		hookedWidget.emplace_back(this);
		return;
	}
	hhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, (HINSTANCE)NULL, 0);
	if (!hhook) {
		qWarning("PLSRegionCapture: hook WH_KEYBOARD_LL failed");
		PLS_ERROR(MAIN_AREA_CAPTURE, "PLSRegionCapture: hook WH_KEYBOARD_LL failed");
		return;
	}
	hookedWidget.emplace_back(this);
}

void PLSRegionCapture::ReleaseHook()
{
	QMutexLocker locker(&mute);
	if (hookedWidget.size() == 1) {
		UnhookWindowsHookEx(hhook);
		hhook = NULL;
		hookedWidget.clear();
		return;
	}
	unregisterWidget(this);
}

void PLSRegionCapture::SetCoordinateValue()
{
	QString strTip;
	if (rectSelected.isValid()) {
		if (!pressed) {
			labelTip->hide();
			return;
		}
		strTip.sprintf("%d X %d", rectSelected.width(), rectSelected.height());
	} else {
		if (!pressed)
			strTip.sprintf("%d,%d", pointMoved.x(), pointMoved.y());
		else
			strTip.sprintf("0 X 0");
	}
	int w = labelTip->fontMetrics().width(strTip) + PLSDpiHelper::calculate(screenDpi, TIP_PADDING_H);
	int h = labelTip->fontMetrics().height() + PLSDpiHelper::calculate(screenDpi, TIP_PADDING_V);
	SetTipCoordinate(w, h);
	labelTip->resize(w, h);
	labelTip->move(tipPositon);
	if (rectSelected.isValid()) {
		strTip = overWidth ? QString("<span style='color:red;'>%1</span>").arg(rectSelected.width()) : QString::number(rectSelected.width());
		strTip.append(" X ");
		strTip.append((overHeight ? QString("<span style='color:red;'>%1</span>").arg(rectSelected.height()) : QString::number(rectSelected.height())));
	}
	labelTip->setText(strTip);
	labelTip->show();
}
