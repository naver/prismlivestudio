#ifndef PLSMAINVIEW_H
#define PLSMAINVIEW_H

#include <functional>
#include <QFrame>
#include <QMenu>
#include <QListWidgetItem>
#include "PLSLivingMsgView.hpp"
#include "frontend-api.h"
#include "PLSToplevelView.hpp"

namespace Ui {
class PLSMainView;
}

class QPushButton;
class PLSBasicStatusBar;
class PLSToastView;
class CustomHelpMenuItem;
class QLabel;
class PLSToastMsgPopup;
class PLSToastButton;
class PLSBasic;

class PLSMainView : public PLSToplevelView<QFrame> {
	Q_OBJECT
	Q_PROPERTY(int captionHeight READ getCaptionHeight WRITE setCaptionHeight)
	Q_PROPERTY(int captionButtonSize READ getCaptionButtonSize WRITE setCaptionButtonSize)
	Q_PROPERTY(int captionButtonMargin READ getCaptionButtonMargin WRITE setCaptionButtonMargin)
	Q_PROPERTY(int logoMarginLeft READ getLogoMarginLeft WRITE setLogoMarginLeft)
	Q_PROPERTY(int menuButtonMarginLeft READ getMenuButtonMarginLeft WRITE setMenuButtonMarginLeft)
	Q_PROPERTY(QSize menuButtonSize READ getMenuButtonSize WRITE setMenuButtonSize)
	Q_PROPERTY(int closeButtonMarginRight READ getCloseButtonMarginRight WRITE setCloseButtonMarginRight)
	Q_PROPERTY(int rightAreaWidth READ getRightAreaWidth WRITE setRightAreaWidth)
	Q_PROPERTY(int bottomAreaHeight READ getBottomAreaHeight WRITE setBottomAreaHeight)
	Q_PROPERTY(int channelsAreaHeight READ getChannelsAreaHeight WRITE setChannelsAreaHeight)
	Q_PROPERTY(bool maxState READ getMaxState)
	Q_PROPERTY(bool fullScreenState READ getFullScreenState)

public:
	explicit PLSMainView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSMainView();

public:
	QWidget *content() const;
	QWidget *channelsArea() const;

	QPushButton *menuButton() const;
	PLSBasicStatusBar *statusBar() const;

	int getCaptionHeight() const;
	void setCaptionHeight(int captionHeight);

	int getCaptionButtonSize() const;
	void setCaptionButtonSize(int captionButtonSize);

	int getCaptionButtonMargin() const;
	void setCaptionButtonMargin(int captionButtonMargin);

	int getLogoMarginLeft() const;
	void setLogoMarginLeft(int logoMarginLeft);

	int getMenuButtonMarginLeft() const;
	void setMenuButtonMarginLeft(int menuButtonMarginLeft);

	QSize getMenuButtonSize() const;
	void setMenuButtonSize(const QSize &menuButtonSize);

	int getCloseButtonMarginRight() const;
	void setCloseButtonMarginRight(int closeButtonMarginRight);

	int getRightAreaWidth() const;
	void setRightAreaWidth(int rightAreaWidth);

	int getBottomAreaHeight() const;
	void setBottomAreaHeight(int bottomAreaHeight);

	int getChannelsAreaHeight() const;
	void setChannelsAreaHeight(int channelsAreaHeight);

	void setCloseEventCallback(std::function<void(QCloseEvent *)> closeEventCallback);
	void callBaseCloseEvent(QCloseEvent *event);

	void setStudioMode(bool studioMode);

	void toastMessage(pls_toast_info_type type, const QString &message, int autoClose);
	void toastMessage(pls_toast_info_type type, const QString &message, const QString &url, const QString &replaceStr, int autoClose);

	void toastClear();

	void setUserButtonIcon(const QIcon &icon);

	QRect &getGeometryOfNormal() { return geometryOfNormal; }

	void initToastMsgView(bool isInitShow = true);
	void InitBeautyView();
	void CreateBeautyView();
	void OnBeautyViewVisibleChanged(bool visible);
	void RefreshBeautyButtonStyle(bool state);
	void RefreshStickersButtonStyle(bool state);

	// bgm
	void InitBgmView();
	void OnBgmViewVisibleChanged(bool visible);

	PLSToastButton *getToastButton();

	void showMainFloatView();
	void hideMainFloatView();

	QRect titleBarRect() const;
	bool canMaximized() const;
	bool canFullScreen() const;
	void flushMaxFullScreenStateStyle();
	void onMaxFullScreenStateChanged();
	void onSaveNormalGeometry();

	bool isClosing() const;
	void close();
	void OnBeautySourceDownloadFinished();

public slots:
	void onUpdateChatSytle(bool isShow);

private:
	static void resetToastViewPostion(pls_frontend_event event, const QVariantList &params, void *context);
	void helpMenuAboutToHide();

signals:
	void popupSettingView(const QString &tab, const QString &group);
	void studioModeChanged();
	void isshowSignal(bool isShow);
	void beginResizeSignal();
	void endResizeSignal();
	void beautyClicked();
	void bgmClicked();
	void stickersClicked();
	void setBeautyViewVisible(bool state);
	void setBgmViewVisible(bool state);
	void maxFullScreenStateChanged(bool isMaxState, bool isFullScreenState);
	void CreateBeautyInstance();
	void BeautySourceDownloadFinished();
	void visibleSignal(bool visible);

private slots:
	void on_menu_clicked();
	void on_user_clicked();
	void on_studioMode_clicked();
	void on_chat_clicked();
	void on_alert_clicked();
	void on_help_clicked();
	void on_settings_clicked();
	void on_listWidget_itemClicked(QListWidgetItem *item);
	void on_beauty_clicked();
	void on_bgmBtn_clicked();
	void on_stickers_clicked();

protected:
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	bool eventFilter(QObject *watcher, QEvent *event);

private:
	Ui::PLSMainView *ui;
	int captionHeight;
	int captionButtonSize;
	int captionButtonMargin;
	int logoMarginLeft;
	int menuButtonMarginLeft;
	QSize menuButtonSize;
	int closeButtonMarginRight;
	int rightAreaWidth;
	int bottomAreaHeight;
	int channelsAreaHeight;
	CustomHelpMenuItem *checkMenuItem;
	QListWidgetItem *checkListWidgetItem;
	std::function<void(QCloseEvent *)> closeEventCallback;
	QMap<qint64, QString> toastMessages;
	QMenu *helpMenu;
	PLSToastView *toast;
	bool m_isFirstLogin = true;
	PLSLivingMsgView m_livingMsgView;
	PLSToastMsgPopup *m_toastMsg;
	bool m_livingToastViewShow;
	bool m_IsChatShowWhenRebackLogin = false;
	bool closing = false;

	friend class PLSBasic;
};

#endif // PLSMAINVIEW_H
