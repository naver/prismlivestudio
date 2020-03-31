#pragma once
#include <qobject.h>
#include <vector>
#include "..\PLSPlatformBase.hpp"
#include "PLSChannelDataHandler.h"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "pls-app.hpp"
using namespace std;

struct PLSAfreecaTVLiveinfoData {
	QString work;
	QString frmCategoryID;
	QString frmCategoryStr;
	QString is_wait;
	QString frmWait;
	bool b_showFrmWait = false;
	QString waiting_time;
	QString frmTitle;
	QString frmWaitTime;
	QString frmViewer;
	QString frmWaterMark;
	QString frmHashTags;
	QString frmByeBye;
	QString encode_type = "normal"; //normal || vr
	QString frmStreamKey;
	QString frmServerUrl;
	QString _id;
	bool b_frmAdult = false;
	QString frmAdult;
	bool b_frmHidden = false;
	QString frmHidden;
	bool b_frmTuneOut = false;
	QString frmTuneOut;
	bool b_containFrmAccess = false; //which is only showen in ko
	QString frmAccess;
	QString frmAccessCode;
};
struct PLSAfreecaTVCategory {
	QString cate_name;
	QString cate_no;
	QString ucc_cate;
	QString display = "1";
	vector<PLSAfreecaTVCategory> child;
};

class PLSPlatformAfreecaTV : public PLSPlatformBase {
	Q_OBJECT

public:
	PLSPlatformAfreecaTV();
	~PLSPlatformAfreecaTV();
	PLSServiceType getServiceType() const override;
	void reInitLiveInfo(bool isReset);
	const PLSAfreecaTVLiveinfoData &getSelectData();
	void requestChannelInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall);
	void requestDashborad(function<void(bool)> onNext, QObject *reciever);
	void requestUserNickName(QVariantMap &srcInfo, UpdateCallback finishedCall);
	void saveSettings(function<void(bool)> onNext, const QString &title);
	bool isSendChatToMqtt() const override { return true; }
	const vector<PLSAfreecaTVCategory> &getCategories() { return m_vecCategories; }
	void setCategories(const vector<PLSAfreecaTVCategory> &cates) { m_vecCategories = cates; }
	QJsonObject getLiveStartParams() override;
	QJsonObject getMqttChatParams() override;
	QJsonObject getWebChatParams() override;
	bool getIsChatDisabled();
	void setIsChatDisabled(bool needAuth) { m_IsChatDisabled = needAuth; }
	QString getServiceLiveLink() override;
signals:
	void closeDialogByExpired();

private:
	//finish selected data when click ok;
	PLSAfreecaTVLiveinfoData m_selectData;
	vector<PLSAfreecaTVCategory> m_vecCategories;
	bool m_IsChatDisabled;

private:
	QString getShareUrl(const QString &id, bool isLiveUrl = false);
	void onPrepareLive(bool value) override;
	PLSPlatformApiResult getApiResult(int code, QNetworkReply::NetworkError error, QByteArray data);
	void showApiRefreshError(PLSPlatformApiResult value);
	void showApiUpdateError(PLSPlatformApiResult value);

	void requestCategories(function<void(bool)> onNext, QObject *reciever);
	void updateLiveinfo(function<void(bool)> onNext, QObject *reciever, const QString &title);

	void onLiveStopped() override;
	void onAlLiveStarted(bool) override;

	void setSelectData(PLSAfreecaTVLiveinfoData data);
	void setupApiFailedWithCode(PLSPlatformApiResult result);
};
