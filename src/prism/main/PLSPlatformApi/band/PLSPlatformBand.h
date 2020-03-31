#ifndef PLSPLATFORMBAND_H
#define PLSPLATFORMBAND_H

#include <functional>
#include <QJsonDocument>

#include "../PLSPlatformBase.hpp"
#include "../channels/ChannelsDataApi/PLSChannelDataHandler.h"

#define MODULE_PLATFORM_BAND "Platform/Band"

using refreshTokenCallback = function<void(bool isRefreshok)>;

class PLSPlatformBand : public PLSPlatformBase {
public:
	PLSPlatformBand();
	virtual ~PLSPlatformBand();

public:
	void initLiveInfo(bool isUpdate);
	void clearBandInfos();
	void getBandRefreshTokenInfo(refreshTokenCallback callback = nullptr, bool isFromServer = false);
	void getBandTokenInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall);
	void getBandCategoryInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall);
	QPair<bool, QString> getChannelEmblemSync(const QString &url);

	virtual PLSServiceType getServiceType() const;
	virtual void onPrepareLive(bool value);
	virtual QString getShareUrl();
	QString getLiveId() const;
	int getMaxLiveTime() const;
	void requestLiveStreamKey(std::function<void(int value)>);
	QJsonObject getLiveStartParams() override;

private:
	void onAlLiveStarted(bool) override;
	void onAllPrepareLive(bool isOk) override;
	void onLiveStopped() override;

	void requesetLiveStop(std::function<void()>);
	void setMaxLiveTime(const int &min);
	void setLiveId(const QString &liveId);
	PLSPlatformApiResult getApiResult(int code, QNetworkReply::NetworkError error);
	void showApiRefreshError(PLSPlatformApiResult value);
	void showChannelInfoError(PLSPlatformApiResult value);

private:
	int m_maxLiveTime;
	QString m_liveId;
	QList<QVariantMap> m_bandInfos;
	QVariantMap m_bandLoginInfo;
	bool m_isRequestStart;
};

#endif // PLSBANDDATAHANDLER_H
