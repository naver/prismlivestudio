/*
* @file		PlatformYoutube.h
* @brief	All youtube relevant api is implemented in this file
* @author	ren.jinbo@navercorp.com
* @date		2020-xx-xx
*/

#pragma once

#include <qobject.h>
#include <QDateTime>
#include <functional>
#include <vector>
#include "..\PLSPlatformBase.hpp"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "pls-app.hpp"

using namespace std;

struct PLSYoutubeCategory {
	QString _id;
	QString title;
};

struct PLSYoutubeLiveinfoData {
public:
	PLSYoutubeLiveinfoData();
	PLSYoutubeLiveinfoData(const QJsonObject &data);

	QString boundStreamId;
	QString _id;
	QString title = "";
	QString description;
	QString startTimeOrigin;
	QString startTimeUTC;
	QString startTimeShort; //show in popup button right label
	QString liveChatId;
	QString privacyStatus;
	QString categoryID;
	QString channelID;
	QString streamKey;
	QString streamUrl;
	bool enableAutoStart = true;
	bool enableAutoStop = true;
	bool isNormalLive = true; //true is live now , false is scheduled live.
	bool isForKids = false;
	bool iskidsUserSelect = false;

	//schedule parameter
	QJsonObject contentDetails;
	QJsonObject streamAPIData;
};

class PLSPlatformYoutube : public PLSPlatformBase {
	Q_OBJECT

public:
	PLSPlatformYoutube();

	PLSServiceType getServiceType() const override;

	static void showAutoStartFalseAlertIfNeeded();

	void liveInfoisShowing();
	void reInitLiveInfo();
	const QString getSelectID();

	const vector<QString> getPrivacyDatas();
	const vector<QString> getPrivacyEnglishDatas();
	const vector<PLSYoutubeCategory> &getCategoryDatas();
	const vector<PLSYoutubeLiveinfoData> &getScheduleDatas();
	const PLSYoutubeLiveinfoData &getNomalLiveData();

	const PLSYoutubeLiveinfoData &getTempSelectData();
	const PLSYoutubeLiveinfoData &getSelectData();

	bool isModifiedWithNewData(QString title, QString description, int categotyIndex, int privacyIndex, bool isKidSelect, bool isNotKidSelect, PLSYoutubeLiveinfoData *uiData);

	void saveSettings(function<void(bool)> onNext, bool isNeedUpdate, PLSYoutubeLiveinfoData uiData);
	void requestCurrentSelectData(function<void(bool)> onNext, QWidget *widget);
	void requestCategoryID(function<void(bool)> onNext, bool isAllList, QWidget *widget);
	void requestCategoryList(function<void(bool)> onNext, QWidget *widget);
	void requestScheduleList(function<void(bool)> onNext, QWidget *widget);

	void requestStartToInsertLiveBroadcasts(function<void(bool)> onNext);
	void requestStartToInsertLiveStreams(function<void(bool)> onNext);
	void requestStartToBindTwo(function<void(bool)> onNext);

	PLSPlatformYoutube &setTempSelectID(const QString value)
	{
		m_bTempSelectID = value;
		return *this;
	}
	const QString getTempSelectID() const { return m_bTempSelectID; }

	PLSYoutubeLiveinfoData &getTempNormalData() { return m_tempNoramlData; };
	PLSYoutubeLiveinfoData &getTrySaveDataData() { return m_trySaveData; };

	bool isSendChatToMqtt() const override { return true; }

	QJsonObject getLiveStartParams() override;
	QString getServiceLiveLink() override;
	QJsonObject getWebChatParams() override;

	bool isPrivateStatus();

signals:
	void onGetTitleDescription();
	void onGetCategorys();
	void selectIDChanged();
	void privacyChangedWhenliving();
	void closeDialogByExpired();
	void toShowLoading(bool isShowLoading);

private:
	int m_idxCategory = 0;
	int m_idxPublic = 0;
	vector<QString> m_vecLocalPrivacy;
	vector<QString> m_vecEnglishPrivacy;
	vector<PLSYoutubeCategory> m_vecCategorys;
	vector<PLSYoutubeLiveinfoData> m_vecSchedules;

	//normal live data
	PLSYoutubeLiveinfoData m_noramlData;

	PLSYoutubeLiveinfoData m_tempNoramlData;
	//finish selected data when click ok;
	PLSYoutubeLiveinfoData m_selectData;

	//temp data
	QString m_bTempSelectID;

	PLSYoutubeLiveinfoData m_trySaveData;

	QTimer *m_statusTimer;

	int m_iContext = 0;

private:
	QString getShareUrl(bool isLiving);
	void onPrepareLive(bool value) override;
	void onAllPrepareLive(bool value) override;
	void onLiveStopped() override;
	void onAlLiveStarted(bool) override;

	void createNewNormalData();

	void requestLiveStreamKey(function<void(bool)> onNext);
	void requestUpdateVideoData(function<void(bool)> onNext, PLSYoutubeLiveinfoData data);
	void requestStatisticsInfo();
	void forceToRefreshToken(function<void(bool)> onNext);
	PLSPlatformApiResult getApiResult(int code, QNetworkReply::NetworkError error, QByteArray data);
	void showApiRefreshError(PLSPlatformApiResult value);
	void showApiUpdateError(PLSPlatformApiResult value);
	void showApiCreateError(PLSPlatformApiResult value);

	void setSelectData(PLSYoutubeLiveinfoData data);
	void setupApiFailedWithCode(PLSPlatformApiResult result);
	void requestStopLive(function<void()> onNext);
	void requestLiveBroadcastStatus();
	void checkDuplicateStreamKey(function<void(bool)> onNext);
	void saveTheScheduleSetting(function<void(bool)> onNext, bool isNeedUpdate);
};
