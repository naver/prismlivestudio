#ifndef PLSDIALOGVIEW_HPP
#define PLSDIALOGVIEW_HPP

#include <functional>
#include <QDialog>
#include <QFrame>

#include "frontend-api-global.h"
#include "PLSToplevelView.hpp"

namespace Ui {
class PLSDialogView;
}

class FRONTEND_API PLSDialogView : public PLSToplevelView<QDialog> {
	Q_OBJECT
	Q_PROPERTY(int captionHeight READ getCaptionHeight WRITE setCaptionHeight)
	Q_PROPERTY(int captionButtonSize READ getCaptionButtonSize WRITE setCaptionButtonSize)
	Q_PROPERTY(int captionButtonMargin READ getCaptionButtonMargin WRITE setCaptionButtonMargin)
	Q_PROPERTY(int textMarginLeft READ getTextMarginLeft WRITE setTextMarginLeft)
	Q_PROPERTY(int closeButtonMarginRight READ getCloseButtonMarginRight WRITE setCloseButtonMarginRight)
	Q_PROPERTY(bool hasCaption READ getHasCaption WRITE setHasCaption)
	Q_PROPERTY(bool hasHLine READ getHasHLine WRITE setHasHLine)
	Q_PROPERTY(bool hasBorder READ getHasBorder WRITE setHasBorder)
	Q_PROPERTY(bool hasMinButton READ getHasMinButton WRITE setHasMinButton)
	Q_PROPERTY(bool hasCloseButton READ getHasCloseButton WRITE setHasCloseButton)
	Q_PROPERTY(bool isMoveInContent READ getIsMoveInContent WRITE setIsMoveInContent)
	Q_PROPERTY(bool maxState READ getMaxState)
	Q_PROPERTY(bool fullScreenState READ getFullScreenState)

public:
	explicit PLSDialogView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSDialogView();

public:
	/**
	* @brief      get content view
	* @return     content view
	*/
	QWidget *content() const;

	/**
	* @brief      get or set content widget
	*/
	QWidget *widget() const;
	void setWidget(QWidget *widget);

	/**
	* @brief      get or set title bar height
	*/
	int getCaptionHeight() const;
	void setCaptionHeight(int captionHeight);

	/**
	* @brief      get or set title bar close/minimized button's width/height
	*/
	int getCaptionButtonSize() const;
	void setCaptionButtonSize(int captionButtonSize);

	/**
	* @brief      get or set title bar close/minimized button's margin
	*/
	int getCaptionButtonMargin() const;
	void setCaptionButtonMargin(int captionButtonMargin);

	/**
	* @brief      get or set title bar text's left margin
	*/
	int getTextMarginLeft() const;
	void setTextMarginLeft(int textMarginLeft);

	/**
	* @brief      get or set title bar close button's right margin
	*/
	int getCloseButtonMarginRight() const;
	void setCloseButtonMarginRight(int closeButtonMarginRight);

	/**
	* @brief      get or set title bar show or not
	*/
	bool getHasCaption() const;
	void setHasCaption(bool hasCaption);

	/**
	* @brief      get or set vline show or not
	*/
	bool getHasHLine() const;
	void setHasHLine(bool hasHLine);

	/**
	* @brief      get or set border show or not
	*/
	bool getHasBorder() const;
	void setHasBorder(bool hasBorder);

	/**
	* @brief      get or set minimized button show or not
	*/
	bool getHasMinButton() const;
	void setHasMinButton(bool hasMinButton);

	/**
	* @brief      get or set maxres button show or not
	*/
	bool getHasMaxResButton() const;
	void setHasMaxResButton(bool hasMaxResButton);

	/**
	* @brief      get or set close button show or not
	*/
	bool getHasCloseButton() const;
	void setHasCloseButton(bool hasCloseButton);

	/**
	* @brief      get or set close event callback
	*/
	void setCloseEventCallback(std::function<bool(QCloseEvent *)> closeEventCallback);
	void callBaseCloseEvent(QCloseEvent *event);

	/**
	* @brief      change dialog size to content
	* @param[inopt] parent      : minimized size
	*/
	void sizeToContent(const QSize &size = QSize());
	void setHeightForFixedWidth();
	/**
	* @brief      move dialog to center of parent
	*/
	void moveToCenter();

	bool getEscapeCloseEnabled() const;
	void setEscapeCloseEnabled(bool enabled);

	// add by zq
	void setHasBackgroundTransparent(bool isTransparent);

public:
	/**
	* @brief      get module name for log
	*/
	virtual const char *getModuleName() const;
	/**
	* @brief      get control name for log
	*/
	virtual std::string getViewName() const;

	void done(int);

	QRect titleBarRect() const;
	bool canMaximized() const;
	bool canFullScreen() const;

public slots:
	int exec() override;

signals:
	void shown();
	void beginResizeSignal();
	void endResizeSignal();
	void visibleSignal(bool visible);

private:
	void flushMaxFullScreenStateStyle();

protected:
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	bool eventFilter(QObject *watcher, QEvent *event);

private:
	Ui::PLSDialogView *ui;
	QWidget *owidget;
	int captionHeight;
	int captionButtonSize;
	int captionButtonMargin;
	int textMarginLeft;
	int closeButtonMarginRight;
	bool hasCaption;
	bool hasHLine;
	bool hasBorder;
	bool hasMinButton;
	bool hasMaxResButton;
	bool hasCloseButton;
	bool isEscapeCloseEnabled;
	std::function<bool(QCloseEvent *)> closeEventCallback;
};

#endif // PLSDIALOGVIEW_HPP
