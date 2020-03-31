#pragma once
#include <qobject.h>
#include <vector>
#include "..\PLSPlatformBase.hpp"
#include "PLSChannelDataHandler.h"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "pls-app.hpp"
using namespace std;

struct PLSVLiveFanshipData {
	QString channelSeq;
	bool isNormalSeq = true;
	QString channelName;
	QString badgeName;
	QString shipSeq;
	bool isChecked = false;
	bool uiEnabled = true;
};

struct PLSVLiveLiveinfoData {
public:
	PLSVLiveLiveinfoData();
	PLSVLiveLiveinfoData(const QJsonObject &data);

	QString boundStreamId;
	QString _id;
	QString startVideoSeq;
	QString title;
	QString startTimeOrigin;
	long startTimeStamp = 0;
	QString startTimeUTC;
	QString startTimeShort; //show in popup button right label

	QString endTimeOrigin;
	long endTimeStamp = 0;
	QString streamKey;
	QString streamUrl;

	QString thumLocalPath;
	QString thumRemoteUrl;

	QString channelSeq;
	QString pitureUrl;
	bool isNormalLive = true; //true is live now , false is scheduled live.
	bool isPlus = false;
	bool isUpcoming = true; //vlive YES:a new schedule    NO：which is broadcasted
	vector<PLSVLiveFanshipData> fanshipDatas;
};

class PLSPlatformVLive : public PLSPlatformBase {
	Q_OBJECT

	struct VliveGcc {
		QString gcc = "KR";
		QString lang = "ko";
		bool isLoaded = false;
	};

public:
	PLSPlatformVLive();
	~PLSPlatformVLive();
	PLSServiceType getServiceType() const override;

	void liveInfoisShowing();
	void reInitLiveInfo(bool isReset);

	const QString getSelectID();
	const vector<PLSVLiveLiveinfoData> &getScheduleDatas();
	const PLSVLiveLiveinfoData getNomalLiveData();

	const PLSVLiveLiveinfoData &getTempSelectData();
	const PLSVLiveLiveinfoData getSelectData();

	PLSVLiveLiveinfoData &getTempNormalData() { return m_tempNoramlData; };

	void setSelectDataByID(const QString &scheID);

	void removeExpiredSchedule(const vector<QString> &ids);

	void initVliveGcc(function<void()> onNext);
	void requestChannelInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall);

	void requestSchedule(function<void(bool)> onNext, QObject *widget, bool isNeedShowErrAlert = true, const QString &checkID = QString());
	void downloadThumImage(function<void()> onNext, QWidget *reciver);

	bool isModifiedWithNewData(QString title, vector<bool> boxChecks, const QString &localThumFile, PLSVLiveLiveinfoData *uiData);

	void saveSettings(function<void(bool)> onNext, PLSVLiveLiveinfoData uiData);

	void setThumLocalFile(const QString &localFile);
	void setThumRemoteUrl(const QString &remoteUrl);

	PLSPlatformVLive &setTempSchedule(bool value)
	{
		m_bTempSchedule = value;
		return *this;
	}
	const bool getIsTempSchedule() const { return m_bTempSchedule; }
	const bool getIsRehearsal() const { return m_isRehearsal; }
	void setIsRehearsal(bool value) { m_isRehearsal = value; }

	PLSPlatformVLive &setTempSelectID(const QString value)
	{
		m_bTempSelectID = value;
		return *this;
	}
	const QString getTempSelectID() const { return m_bTempSelectID; }
	const QString getDefaultTitle();

	bool isSendChatToMqtt() const override { return true; }
	QString getServiceLiveLink() override;
	VliveGcc &getGccData() { return m_gccData; };
signals:
	void closeDialogByExpired();
	void toShowLoading(bool isShowLoading);

private:
	vector<PLSVLiveLiveinfoData> m_vecSchedules;

	//normal live data
	PLSVLiveLiveinfoData m_noramlData;
	PLSVLiveLiveinfoData m_tempNoramlData;

	//finish selected data when click ok;
	PLSVLiveLiveinfoData m_selectData;

	//temp data
	bool m_bTempSchedule = false;
	QString m_bTempSelectID;

	QTimer *m_statusTimer;
	bool m_isHaveShownScheduleNotice = false;
	bool m_isRehearsal = false;
	bool isStopedByRemote = false;
	VliveGcc m_gccData;

private:
	QString getShareUrl() override;
	void setFanshipDatas(PLSVLiveLiveinfoData &infoData);
	void onPrepareLive(bool value) override;
	void requestUploadImage(const QString &localFile, function<void(bool, const QString &)> onNext);
	void requestStatisticsInfo();
	void requestStartLive(function<void(bool)> onNext);
	void requestStopLive(function<void(void)> onNext);

	PLSPlatformApiResult getApiResult(int code, QNetworkReply::NetworkError error, QByteArray data);
	void showApiRefreshError(PLSPlatformApiResult value);
	void showApiUpdateError(PLSPlatformApiResult value);

	void onActive() override;
	void onLiveStopped() override;
	void onAlLiveStarted(bool) override;
	QJsonObject getWebChatParams() override;

	void setSelectData(PLSVLiveLiveinfoData data);
	void setupApiFailedWithCode(PLSPlatformApiResult result);

	PLSVLiveLiveinfoData &getTempSelectDataRef();

	void showScheLiveNotice(const PLSVLiveLiveinfoData &liveInfo);
	void onShowScheLiveNoticeIfNeeded();
};
