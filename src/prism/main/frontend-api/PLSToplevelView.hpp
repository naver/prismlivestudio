#ifndef PLSTOPLEVELVIEW_HPP
#define PLSTOPLEVELVIEW_HPP

#include <functional>
#include <list>
#include <memory>

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QApplication>
#include <QMouseEvent>
#include <QComboBox>
#include <QLineEdit>
#include <QAbstractButton>
#include <QTextEdit>
#include <QListWidget>
#include <QAbstractSpinBox>
#include <QDebug>
#include <QScreen>
#include <QDataStream>

#include "frontend-api.h"
#include "PLSDpiHelper.h"
#include "PLSWidgetDpiAdapter.hpp"

template<typename ParentWidget> class PLSToplevelView : public PLSWidgetDpiAdapterHelper<ParentWidget> {
public:
	using ToplevelView = PLSToplevelView<ParentWidget>;
	using WidgetDpiAdapter = PLSWidgetDpiAdapterHelper<ParentWidget>;

protected:
	PLSToplevelView(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) : WidgetDpiAdapter(parent, f)
	{
		resizePausedCheckTimer.setSingleShot(true);
		resizePausedCheckTimer.setInterval(200);

		connect(&resizePausedCheckTimer, &QTimer::timeout, this, [this]() {
			if (isResizing) {
				isResizing = false;
				resizePausedCheckTimer.stop();
				emit endResizeSignal();
			}
		});
	}
	virtual ~PLSToplevelView() {}

public:
	enum class CursorPosition { None, Top, Bottom, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight, TitleBar, Content };
	static const int ORIGINAL_BORDER_WIDTH = 1;
	static const int ORIGINAL_RESIZE_BORDER_WIDTH = 5;
	static const int ORIGINAL_CORNER_WIDTH = 5;

	class InActiveHelper {
		ToplevelView *tlv;

	public:
		InActiveHelper(ToplevelView *tlv_) : tlv(tlv_) { tlv->isActive = false; }
		~InActiveHelper() { tlv->isActive = true; }
	};

public:
	int BORDER_WIDTH() { return BORDER_WIDTH(this); }
	int RESIZE_BORDER_WIDTH() { return RESIZE_BORDER_WIDTH(this); }
	int CORNER_WIDTH() { return CORNER_WIDTH(this); }

public:
	static int BORDER_WIDTH(QWidget *widget) { return PLSDpiHelper::calculate(widget, ORIGINAL_BORDER_WIDTH); }
	static int RESIZE_BORDER_WIDTH(QWidget *widget) { return PLSDpiHelper::calculate(widget, ORIGINAL_RESIZE_BORDER_WIDTH); }
	static int CORNER_WIDTH(QWidget *widget) { return PLSDpiHelper::calculate(widget, ORIGINAL_CORNER_WIDTH); }

public:
	void checkCursorPosition(const QPoint &globalPos)
	{
		if (!isActive) {
			cursorPosition = CursorPosition::None;
			return;
		}

		QPoint pos = mapFromGlobal(globalPos);
		if (!isResizeEnabled || !isWidthResizeEnabled && !isHeightResizeEnabled || isMaxState || isFullScreenState) {
			if (titleBarRect().contains(pos)) {
				cursorPosition = CursorPosition::TitleBar;
			} else if (rect().contains(pos)) {
				cursorPosition = CursorPosition::Content;
			} else {
				cursorPosition = CursorPosition::None;
			}
			return;
		}

		int RESIZE_BORDER_WIDTH = this->RESIZE_BORDER_WIDTH();
		QRect centralRect(RESIZE_BORDER_WIDTH, RESIZE_BORDER_WIDTH, width() - RESIZE_BORDER_WIDTH * 2, height() - RESIZE_BORDER_WIDTH * 2);
		if (pos.x() <= centralRect.left() && pos.y() <= centralRect.top()) {
			cursorPosition = CursorPosition::TopLeft;
		} else if (pos.x() >= centralRect.right() && pos.y() >= centralRect.bottom()) {
			cursorPosition = CursorPosition::BottomRight;
		} else if (pos.x() >= centralRect.right() && pos.y() <= centralRect.top()) {
			cursorPosition = CursorPosition::TopRight;
		} else if (pos.x() <= centralRect.left() && pos.y() >= centralRect.bottom()) {
			cursorPosition = CursorPosition::BottomLeft;
		} else if (pos.x() <= centralRect.left()) {
			cursorPosition = isWidthResizeEnabled ? CursorPosition::Left : CursorPosition::None;
		} else if (pos.x() >= centralRect.right()) {
			cursorPosition = isWidthResizeEnabled ? CursorPosition::Right : CursorPosition::None;
		} else if (pos.y() <= centralRect.top()) {
			cursorPosition = isHeightResizeEnabled ? CursorPosition::Top : CursorPosition::None;
		} else if (pos.y() >= centralRect.bottom()) {
			cursorPosition = isHeightResizeEnabled ? CursorPosition::Bottom : CursorPosition::None;
		} else if (titleBarRect().contains(pos)) {
			cursorPosition = CursorPosition::TitleBar;
		} else if (centralRect.contains(pos)) {
			cursorPosition = CursorPosition::Content;
		} else {
			cursorPosition = CursorPosition::None;
		}
	}
	void updateCursor(void)
	{
		switch (cursorPosition) {
		case CursorPosition::Top:
		case CursorPosition::Bottom:
			setCursor(Qt::SizeVerCursor);
			break;
		case CursorPosition::Left:
		case CursorPosition::Right:
			setCursor(Qt::SizeHorCursor);
			break;
		case CursorPosition::TopLeft:
		case CursorPosition::BottomRight:
			setCursor(Qt::SizeFDiagCursor);
			break;
		case CursorPosition::TopRight:
		case CursorPosition::BottomLeft:
			setCursor(Qt::SizeBDiagCursor);
			break;
		default:
			setCursor(Qt::ArrowCursor);
			break;
		}
	}
	void mousePress(QMouseEvent *event)
	{
		globalPos = event->globalPos();
		QPoint pos = mapFromGlobal(globalPos);
		if (isActive && rect().contains(pos) && (event->button() == Qt::LeftButton) && !isMouseButtonDown) {
			checkCursorPosition(globalPos);
			updateCursor();

			if (cursorPosition == CursorPosition::None) {
				return;
			} else if (!isMoveInContent && cursorPosition == CursorPosition::Content) {
				return;
			} else if (cursorPosition == CursorPosition::Content && isInControl(childAt(pos))) {
				return;
			}

			isMouseButtonDown = true;
			isResizing = false;

			globalPosMousePress = globalPos;
			relativePosMousePress = mapFromGlobal(globalPos);
			geometryOfMousePress = geometry();
			globalPosMouseMoveForResize = globalPos;
		}
	}
	void mouseRelease(QMouseEvent *event)
	{
		if (isActive && (event->button() == Qt::LeftButton) && isMouseButtonDown) {
			if (isResizing) {
				isResizing = false;
				resizePausedCheckTimer.stop();
				emit endResizeSignal();
			}

			isMouseButtonDown = false;

			cursorPosition = CursorPosition::None;
			updateCursor();

			onSaveNormalGeometry();
		}
	}
	void mouseDbClick(QMouseEvent *event)
	{
		if (!isActive || event->button() != Qt::LeftButton) {
			return;
		} else if (!canMaximized()) {
			return;
		} else if ((QDateTime::currentMSecsSinceEpoch() - lastDbClickTime) <= qint64(100)) {
			return;
		}

		lastDbClickTime = QDateTime::currentMSecsSinceEpoch();

		checkCursorPosition(event->globalPos());
		updateCursor();

		if (cursorPosition != CursorPosition::TitleBar) {
			return;
		} else if (!isMaxState && !isFullScreenState) {
			showMaximized();
		} else {
			showNormal();
		}
	}
	void mouseMove(QMouseEvent *event)
	{
		globalPos = event->globalPos();

		isMouseButtonDown = isMouseButtonDown && (event->buttons() & Qt::LeftButton);

		if (isMouseButtonDown && globalPosMousePress == globalPos) {
			return;
		}

		if (isActive && !isMouseButtonDown) {
			checkCursorPosition(globalPos);
			updateCursor();
		}

		if (isForMove()) {
			if (isMaxState || isFullScreenState) {
				isMaxState = false;
				isFullScreenState = false;
				flushMaxFullScreenStateStyle();

				QRect g = geometry();

				QRect ng = geometryOfNormal;
				QPoint pos = mapFromGlobal(globalPos);
				relativePosMousePress = QPoint(pos.x() * ng.width() / g.width(), pos.y());
				ng.moveTo(globalPos - relativePosMousePress);
				setGeometry(ng);

				globalPosMousePress = globalPos;
				geometryOfMousePress = ng;

				onMaxFullScreenStateChanged();
			} else {
				QRect g(globalPos - relativePosMousePress, geometryOfMousePress.size());
				if (g != geometry()) {
					geometryOfNormal = g;
					setGeometry(g);
				}
			}
		} else if (isForResize()) {
			QSize maxSize = maximumSize(), minSize = minimumSize();
			int mpw = geometryOfMousePress.width(), mph = geometryOfMousePress.height(), ox = globalPos.x() - globalPosMousePress.x(), oy = globalPos.y() - globalPosMousePress.y();
			QRect g = geometry();
			switch (cursorPosition) {
			case CursorPosition::Top:
				g = fromBottomLeftSize(geometryOfMousePress.bottomLeft(), QSize(mpw, mph - oy).expandedTo(minSize).boundedTo(maxSize));
				break;
			case CursorPosition::Bottom:
				g = fromTopLeftSize(geometryOfMousePress.topLeft(), QSize(mpw, mph + oy).expandedTo(minSize).boundedTo(maxSize));
				break;
			case CursorPosition::Left:
				g = fromTopRightSize(geometryOfMousePress.topRight(), QSize(mpw - ox, mph).expandedTo(minSize).boundedTo(maxSize));
				break;
			case CursorPosition::Right:
				g = fromTopLeftSize(geometryOfMousePress.topLeft(), QSize(mpw + ox, mph).expandedTo(minSize).boundedTo(maxSize));
				break;
			case CursorPosition::TopLeft:
				g = fromBottomRightSize(geometryOfMousePress.bottomRight(), QSize(mpw - ox, mph - oy).expandedTo(minSize).boundedTo(maxSize));
				break;
			case CursorPosition::TopRight:
				g = fromBottomLeftSize(geometryOfMousePress.bottomLeft(), QSize(mpw + ox, mph - oy).expandedTo(minSize).boundedTo(maxSize));
				break;
			case CursorPosition::BottomLeft:
				g = fromTopRightSize(geometryOfMousePress.topRight(), QSize(mpw - ox, mph + oy).expandedTo(minSize).boundedTo(maxSize));
				break;
			case CursorPosition::BottomRight:
				g = fromTopLeftSize(geometryOfMousePress.topLeft(), QSize(mpw + ox, mph + oy).expandedTo(minSize).boundedTo(maxSize));
				break;
			default:
				break;
			}

			if (g != geometry()) {
				if (globalPosMouseMoveForResize != globalPos) {
					globalPosMouseMoveForResize = globalPos;

					if (!isResizing) {
						isResizing = true;
						resizePausedCheckTimer.start();
						emit beginResizeSignal();
					} else {
						resizePausedCheckTimer.stop();
						resizePausedCheckTimer.start();
					}
				}

				geometryOfNormal = g;
				setGeometry(g);
			}
		}
	}

public:
	bool getIsMoveInContent() const { return isMoveInContent; }
	void setIsMoveInContent(bool isMoveInContent) { this->isMoveInContent = isMoveInContent; }

	bool getResizeEnabled() const { return isResizeEnabled; }
	void setResizeEnabled(bool isResizeEnabled) { this->isResizeEnabled = isResizeEnabled; }

	bool getWidthResizeEnabled() const { return isWidthResizeEnabled; }
	void setWidthResizeEnabled(bool isResizeEnabled) { this->isWidthResizeEnabled = isResizeEnabled; }

	bool getHeightResizeEnabled() const { return isHeightResizeEnabled; }
	void setHeightResizeEnabled(bool isResizeEnabled) { this->isHeightResizeEnabled = isResizeEnabled; }

	bool getMaxState() const { return isMaxState; }
	bool getFullScreenState() const { return isFullScreenState; }

protected:
	bool isCursorPosition(CursorPosition cursorPosition) const { return this->cursorPosition == cursorPosition; }
	bool isForMove() const
	{
		if (!isActive || !isMouseButtonDown) {
			return false;
		} else if (isCursorPosition(CursorPosition::TitleBar)) {
			return true;
		} else if (!isMoveInContent) {
			return false;
		} else if (isCursorPosition(CursorPosition::Content)) {
			return true;
		}
		return false;
	}
	bool isForResize() const
	{
		if (!isActive || !isMouseButtonDown) {
			return false;
		} else if (isCursorPosition(CursorPosition::Left)) {
			return true;
		} else if (isCursorPosition(CursorPosition::Top)) {
			return true;
		} else if (isCursorPosition(CursorPosition::Right)) {
			return true;
		} else if (isCursorPosition(CursorPosition::Bottom)) {
			return true;
		} else if (isCursorPosition(CursorPosition::TopLeft)) {
			return true;
		} else if (isCursorPosition(CursorPosition::TopRight)) {
			return true;
		} else if (isCursorPosition(CursorPosition::BottomLeft)) {
			return true;
		} else if (isCursorPosition(CursorPosition::BottomRight)) {
			return true;
		}
		return false;
	}
	template<typename Control> bool isControl(QWidget *child)
	{
		if (this == child) {
			return false;
		} else if (dynamic_cast<Control *>(child)) {
			return true;
		} else {
			return isControl<Control>(child->parentWidget());
		}
	}
	bool isInControl(QWidget *child)
	{
		if (!child) {
			return false;
		} else if (this == child) {
			return false;
		} else if (isControl<QAbstractButton>(child)) {
			return true;
		} else if (isControl<QComboBox>(child)) {
			return true;
		} else if (isControl<QLineEdit>(child)) {
			return true;
		} else if (isControl<QTextEdit>(child)) {
			return true;
		} else if (isControl<QAbstractSpinBox>(child)) {
			return true;
		} else if (isControl<QListWidget>(child)) {
			return true;
		} else if (isInCustomControl(child)) {
			return true;
		}
		return false;
	}
	QRect fromTopLeftSize(const QPoint &topLeft, const QSize &size) { return QRect(topLeft.x(), topLeft.y(), size.width(), size.height()); }
	QRect fromTopRightSize(const QPoint &topRight, const QSize &size) { return QRect(topRight.x() - size.width(), topRight.y(), size.width(), size.height()); }
	QRect fromBottomLeftSize(const QPoint &bottomLeft, const QSize &size) { return QRect(bottomLeft.x(), bottomLeft.y() - size.height(), size.width(), size.height()); }
	QRect fromBottomRightSize(const QPoint &bottomRight, const QSize &size) { return QRect(bottomRight.x() - size.width(), bottomRight.y() - size.height(), size.width(), size.height()); }
	QRect onDpiChanged(double dpi, double oldDpi, const QRect &suggested, bool firstShow) override
	{
		extern QScreen *getScreen(QWidget * widget);
		extern QRect fullscreenShowForce(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);
		extern QRect maximizeShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);
		extern QRect normalShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (firstShow || isForResize() || isManualSetPos || (oldDpi == 0.0)) {
			geometryOfNormal = suggested.isValid() ? suggested : geometryOfNormal;
			return suggested;
		}

		QRect retval = geometryOfNormal;
		if (isForMove()) {
			relativePosMousePress = (QPointF(relativePosMousePress) * dpi / oldDpi).toPoint();
			geometryOfMousePress.setSize((QSizeF(geometryOfMousePress.size()) * dpi / oldDpi).toSize().expandedTo(minimumSize()).boundedTo(maximumSize()));

			QRect g(globalPos - relativePosMousePress, geometryOfMousePress.size());
			if (g != geometry()) {
				geometryOfNormal = g;
				setGeometry(g);
				retval = geometryOfNormal;
			}
		} else if (suggested.isValid()) {
			if (isMaxState || isFullScreenState) {
				InActiveHelper helper(this);

				flushMaxFullScreenStateStyle();
				onMaxFullScreenStateChanged();

				geometryOfNormal.setSize((QSizeF(geometryOfNormal.size()) * dpi / oldDpi).toSize().expandedTo(minimumSize()).boundedTo(maximumSize()));
				if (isFullScreenState) {
					retval = fullscreenShowForce(this, geometryOfNormal);
				} else {
					retval = maximizeShow(this, geometryOfNormal);
				}
			} else {
				QRect g(geometryOfNormal.isValid() ? geometryOfNormal.topLeft() : suggested.topLeft(), suggested.size().expandedTo(minimumSize()).boundedTo(maximumSize()));
				geometryOfNormal = g;
				retval = normalShow(this, geometryOfNormal);
			}

			onSaveNormalGeometry();
		} else /*if (isMinimizedDpiChanged)*/ {
			if (isMaxState || isFullScreenState) {
				InActiveHelper helper(this);

				flushMaxFullScreenStateStyle();
				onMaxFullScreenStateChanged();

				geometryOfNormal.setSize((QSizeF(geometryOfNormal.size()) * dpi / oldDpi).toSize().expandedTo(minimumSize()).boundedTo(maximumSize()));
				if (isFullScreenState) {
					retval = fullscreenShowForce(this, geometryOfNormal);
				} else {
					retval = maximizeShow(this, geometryOfNormal);
				}
			} else {
				QRect g(geometryOfNormal.topLeft(), (QSizeF(geometryOfNormal.size()) * dpi / oldDpi).toSize().expandedTo(minimumSize()).boundedTo(maximumSize()));
				geometryOfNormal = g;
				retval = normalShow(this, geometryOfNormal);
			}

			onSaveNormalGeometry();
		}
		return retval;
	}
	void onScreenAvailableGeometryChanged(const QRect &screenAvailableGeometry) override
	{
		extern QRect fullscreenShowForce(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);
		extern QRect maximizeShow(PLSWidgetDpiAdapter * adapter, const QRect &screenAvailableRect, QRect &geometryOfNormal);
		extern QRect normalShow(PLSWidgetDpiAdapter * adapter, const QRect &screenAvailableGeometry, QRect &geometryOfNormal);

		if (isFullScreenState) {
			fullscreenShowForce(this, geometryOfNormal);
		} else if (isMaxState) {
			maximizeShow(this, screenAvailableGeometry, geometryOfNormal);
		} else {
			normalShow(this, screenAvailableGeometry, geometryOfNormal);
		}

		onSaveNormalGeometry();
	}
	QRect getSuggestedRect(const QRect &suggested) const override { return geometryOfNormal; }
	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override
	{
		extern bool toplevelViewNativeEvent(PLSWidgetDpiAdapter * adapter, const QByteArray &eventType, void *message, long *result,
						    std::function<bool(const QByteArray &, void *, long *)> baseNativeEvent, bool isMaxState, bool isFullScreenState);

		return toplevelViewNativeEvent(
			this, eventType, message, result, [this](const QByteArray &eventType, void *message, long *result) { return WidgetDpiAdapter::nativeEvent(eventType, message, result); },
			isMaxState, isFullScreenState);
	}
	bool event(QEvent *event) override
	{
		if (event->type() == QEvent::Resize) {
			QResizeEvent *resizeEvent = reinterpret_cast<QResizeEvent *>(event);
			if (!isMaxState && !isFullScreenState && geometryOfNormal.size() != resizeEvent->size()) {
				geometryOfNormal = geometry();
			}
		}

		return WidgetDpiAdapter::event(event);
	}

public:
	void showMaximized()
	{
		extern QRect maximizeShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (!isMaxState) {
			InActiveHelper helper(this);

			isMaxState = true;
			flushMaxFullScreenStateStyle();
			onMaxFullScreenStateChanged();

			geometryOfNormal = geometry();
			maximizeShow(this, geometryOfNormal);
			onSaveNormalGeometry();
		}
	}
	void showFullScreen()
	{
		extern QRect fullscreenShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (!isFullScreenState) {
			InActiveHelper helper(this);

			isFullScreenState = true;
			flushMaxFullScreenStateStyle();
			onMaxFullScreenStateChanged();

			if (!isMaxState) {
				geometryOfNormal = geometry();
			}

			fullscreenShow(this, geometryOfNormal);
			onSaveNormalGeometry();
		}
	}
	void showNormal()
	{
		extern QRect maximizeShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (isMaxState && isFullScreenState) {
			InActiveHelper helper(this);

			isFullScreenState = false;
			flushMaxFullScreenStateStyle();
			onMaxFullScreenStateChanged();

			maximizeShow(this, geometryOfNormal);
			onSaveNormalGeometry();
		} else if (isMaxState || isFullScreenState) {
			InActiveHelper helper(this);

			isMaxState = false;
			isFullScreenState = false;
			onMaxFullScreenStateChanged();
			flushMaxFullScreenStateStyle();

			setGeometry(geometryOfNormal);
			activateWindow();
		}
	}
	void setPos(int x, int y) { setPos(QPoint(x, y)); }
	void setPos(const QPoint &pos)
	{
		isManualSetPos = true;
		geometryOfNormal.moveTopLeft(pos);
		move(pos.x(), pos.y());
		isManualSetPos = false;
	}
	QByteArray saveGeometry() const { return PLSWidgetDpiAdapter::saveGeometry(selfWidget(), geometryOfNormal); }
	bool restoreGeometry(const QByteArray &geometry)
	{
		extern void setGeometrySys(QWidget * widget, const QRect &geometry);

		bool result = PLSWidgetDpiAdapter::restoreGeometry(this, geometry);
		if (result) {
			geometryOfNormal = this->geometry();
			setGeometrySys(this, geometryOfNormal);
		}
		return result;
	}
	void resize(const QSize &size) { resize(size.width(), size.height()); }
	void resize(int width, int height)
	{
		geometryOfNormal.setSize(QSize(width, height));
		WidgetDpiAdapter::resize(width, height);
	}

protected:
	virtual QRect titleBarRect() const = 0;
	virtual bool canMaximized() const = 0;
	virtual bool canFullScreen() const = 0;
	virtual bool isInCustomControl(QWidget * /*child*/) const { return false; }

	virtual void beginResizeSignal() = 0;
	virtual void endResizeSignal() = 0;
	virtual void flushMaxFullScreenStateStyle() = 0;
	virtual void onMaxFullScreenStateChanged() {}
	virtual void onSaveNormalGeometry() {}

protected:
	bool isActive = true;
	bool isMoveInContent = false;
	bool isResizeEnabled = true;
	bool isWidthResizeEnabled = true;
	bool isHeightResizeEnabled = true;
	bool isMaxState = false;
	bool isFullScreenState = false;
	bool isMouseButtonDown = false;
	bool isResizing = false;
	bool isManualSetPos = false;
	CursorPosition cursorPosition = CursorPosition::None;
	qint64 lastDbClickTime = 0;
	QPoint globalPos;
	QPoint globalPosMousePress;
	QPoint relativePosMousePress;
	QRect geometryOfMousePress;
	QPoint globalPosMouseMoveForResize;
	QTimer resizePausedCheckTimer;
	QRect geometryOfNormal;
};

#endif // PLSTOPLEVELVIEW_HPP
