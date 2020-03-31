#ifndef PLSCHATDIALOG_H
#define PLSCHATDIALOG_H

#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QUrl>
#include <browser-panel.hpp>
#include <dialog-view.hpp>
#include <vector>
#include "PLSChatHelper.h"

namespace Ui {
class PLSChatDialog;
}

using namespace std;

class PLSChatDialog : public PLSDialogView {
	Q_OBJECT
	Q_PROPERTY(bool btnSelected READ getHasCloseButton WRITE setHasCloseButton)

public:
	explicit PLSChatDialog(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSChatDialog();
	void logoutToReInitUI();
	void onMaxFullScreenStateChanged() override;
	void onSaveNormalGeometry() override;

private:
	struct ChatDatas {
		QPointer<QWidget> widget;
		QPushButton *button;
		string url;
		bool isWebLoaded = false;
	};

	Ui::PLSChatDialog *ui;
	void setupFirstUI(PLSDpiHelper dpiHelper);
	void setInitSize(double dpi, bool inConstructor);
	void refreshTabButtonCount();
	void hideOrShowTabButton();
	void setupFirstRtmpUI(QWidget *parent);
	void refreshUI();
	int rtmpChannelCount();
	QCefWidget *createANewCefWidget(const string &url, PLSChatHelper::ChatPlatformIndex index);

	void setupNewUrl(PLSChatHelper::ChatPlatformIndex index, string url);
	PLSChatHelper::ChatPlatformIndex foundFirstShowedButton();

	void updateRtmpPlaceText();
	void updateNewUrlByIndex(PLSChatHelper::ChatPlatformIndex index, const QVariantMap &info);

	void showToastIfNeeded();
	void switchStackWidget(PLSChatHelper::ChatPlatformIndex index);

	void updateYoutubeUrlIfNeeded();

	void setSelectIndex(PLSChatHelper::ChatPlatformIndex index);

	//PRISM/Zhangdewen/20200921/#/add chat source button
	QWidget *createChatSourceButton(QWidget *parent, bool noPlatform);

signals:
	void chatShowOrHide(bool show);

private slots:
	void changedSelectIndex(PLSChatHelper::ChatPlatformIndex index);
	void channelRemoveToDeleteCef(PLSChatHelper::ChatPlatformIndex index);
	void facebookPrivateChatChanged(bool oldPrivate, bool newPrivate);
	//PRISM/Zhangdewen/20200921/#/add chat source button
	void chatSourceButtonClicked();

protected:
	void showEvent(QShowEvent *event) override;
	void hideEvent(QHideEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

	bool eventFilter(QObject *, QEvent *) override;
	void wheelEvent(QWheelEvent *event) override;

	void adjustToastSize();
	void updateTabPolicy();

private:
	QTimer m_timerToastHide;
	QLabel *m_pLabelToast;
	QPushButton *m_p_ButtonToastClose;
	QCefCookieManager *chat_panel_cookies;

	QLabel *m_rtmpPlaceTextLabel;

	//PRISM/Zhangdewen/20200921/#/add chat source button
	QWidget *m_chatSourceButtonOnePlatform = nullptr;
	QWidget *m_chatSourceButtonNoPlatform = nullptr;

	vector<ChatDatas> m_vecChatDatas;
	bool m_isForceRefresh = true;
	PLSChatHelper::ChatPlatformIndex m_selectIndex = PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine;
	QSpacerItem *m_rightHorizontalSpacer;
};

#endif // PLSCHATDIALOG_H
