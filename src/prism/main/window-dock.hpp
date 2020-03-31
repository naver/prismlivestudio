#pragma once

#include <QDockWidget>
#include <QMargins>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QAction>
#include <QTimer>

#include "PLSDpiHelper.h"
#include "PLSToplevelView.hpp"

class QMenu;
class QWidgetResizeHandler;
class PLSPopupMenu;
class PLSDockTitlePopupMenu;
class PLSDock;

class PLSDockTitle : public QFrame {
	Q_OBJECT
	Q_PROPERTY(int captionHeight READ getCaptionHeight WRITE setCaptionHeight)
	Q_PROPERTY(int marginLeft READ getMarginLeft WRITE setMarginLeft)
	Q_PROPERTY(int marginRight READ getMarginRight WRITE setMarginRight)
	Q_PROPERTY(int contentSpacing READ getContentSpacing WRITE setContentSpacing)

public:
	explicit PLSDockTitle(PLSDock *parent = nullptr);
	~PLSDockTitle();

public:
	int getCaptionHeight() const;
	void setCaptionHeight(int captionHeight);

	int getMarginLeft() const;
	void setMarginLeft(int marginLeft);

	int getMarginRight() const;
	void setMarginRight(int marginRight);

	int getContentSpacing() const;
	void setContentSpacing(int contentSpacing);

	QList<QToolButton *> getButtons() const;
	QToolButton *getAdvButton() const;

	void setButtonActions(QList<QAction *> actions);

	void setAdvButtonMenu(QMenu *menu);
	void setAdvButtonMenu(PLSPopupMenu *menu);
	void setAdvButtonActions(QList<QAction *> actions);

private:
	void setButtonPropertiesFromAction(QToolButton *button, QAction *action);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	bool eventFilter(QObject *watched, QEvent *event) override;

	QSize sizeHint() const override { return minimumSizeHint(); }
	QSize minimumSizeHint() const override;

private:
	PLSDock *dock;
	int captionHeight;
	int marginLeft;
	int marginRight;
	int contentSpacing;
	QLabel *titleLabel;
	QList<QToolButton *> buttons;
	QList<QAction *> buttonActions;
	QToolButton *advButton;
	PLSDockTitlePopupMenu *advButtonMenu;
	QHBoxLayout *buttonsLayout;
};

class PLSWidgetResizeHandler : public QObject {
	Q_OBJECT
public:
	explicit PLSWidgetResizeHandler(PLSDock *dock, QWidgetResizeHandler *resizeHandler);
	~PLSWidgetResizeHandler();

protected:
	virtual bool eventFilter(QObject *watched, QEvent *event);

private:
	PLSDock *dock;
	QWidgetResizeHandler *resizeHandler;
	bool isForResize;
	bool isForMove;
	bool isResizing;
	QSize dockSizeMousePress;
	QPoint globalPosMouseMoveForResize;
	QTimer resizePausedCheckTimer;

	friend class PLSDock;
};

class PLSDock : public PLSWidgetDpiAdapterHelper<QDockWidget> {
	Q_OBJECT
	Q_PROPERTY(bool moving READ isMoving WRITE setMoving)
	Q_PROPERTY(bool movingAndFloating READ isMovingAndFloating)
	Q_PROPERTY(int contentMarginLeft READ getContentMarginLeft WRITE setContentMarginLeft)
	Q_PROPERTY(int contentMarginTop READ getContentMarginTop WRITE setContentMarginTop)
	Q_PROPERTY(int contentMarginRight READ getContentMarginRight WRITE setContentMarginRight)
	Q_PROPERTY(int contentMarginBottom READ getContentMarginBottom WRITE setContentMarginBottom)

public:
	explicit PLSDock(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());

public:
	bool isMoving() const;
	void setMoving(bool moving);

	bool isMovingAndFloating() const;

	int getContentMarginLeft() const;
	void setContentMarginLeft(int contentMarginLeft);

	int getContentMarginTop() const;
	void setContentMarginTop(int contentMarginTop);

	int getContentMarginRight() const;
	void setContentMarginRight(int contentMarginRight);

	int getContentMarginBottom() const;
	void setContentMarginBottom(int contentMarginBottom);

	PLSDockTitle *titleWidget() const;

	QWidget *widget() const;
	void setWidget(QWidget *widget);

	QWidget *getContent() const;

signals:
	void beginResizeSignal();
	void endResizeSignal();
	void initResizeHandlerProxySignal(QObject *child);
	void visibleSignal(bool visible);

private slots:
	void beginResizeSlot();
	void endResizeSlot();

protected:
	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);
	bool event(QEvent *event);
	QRect onDpiChanged(double dpi, double oldDpi, const QRect &suggested, bool firstShow);
	void onScreenAvailableGeometryChanged(const QRect &screenAvailableGeometry);

private:
	PLSDockTitle *dockTitle;
	bool moving;
	int contentMarginLeft;
	int contentMarginTop;
	int contentMarginRight;
	int contentMarginBottom;
	QFrame *content;
	QHBoxLayout *contentLayout;
	QWidget *owidget;
	PLSWidgetResizeHandler *resizeHandlerProxy;
	QRect geometryOfNormal;

	friend class PLSDockTitle;
	friend class PLSWidgetResizeHandler;
};
