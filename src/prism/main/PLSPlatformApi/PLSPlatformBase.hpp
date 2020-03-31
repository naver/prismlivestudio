/*
* @file		PLSPlatformBase.hpp
* @brief	A base class for all platforms
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
*/

#pragma once

#include <string>
#include <functional>

#include <QWidget>
#include <QPointer>
#include <QVariant>
#include <QMap>
#include <QJsonObject>
#include <QNetworkReply>

#include "window-basic-main.hpp"
#include "channels/ChannelsDataApi/ChannelConst.h"

using namespace std;

#define MODULE_PlatformService "PlatformService"

#define PLATFORM_SIZE 8
enum class PLSServiceType { ST_RTMP, ST_TWITCH, ST_YOUTUBE, ST_FACEBOOK, ST_VLIVE, ST_NAVERTV, ST_BAND, ST_AFREECATV };
enum class PLSTokenRequestStatus { PLS_GOOD, PLS_ING, PLS_BAD };

extern const char *const NamesForChannelType[PLATFORM_SIZE];
extern const char *const NamesForSettingId[PLATFORM_SIZE];
extern const char *const NamesForLiveStart[PLATFORM_SIZE];

extern const char *const KeyConfigLiveInfo;
extern const char *const KeyTwitchServer;

enum class PLSPlatformApiResult {
	PAR_SUCCEED,
	PAR_NETWORK_ERROR,
	PAR_API_FAILED,
	PAR_TOKEN_EXPIRED,
	YOUTUBE_API_ERROR_NO_CHANNEL,
	YOUTUBE_API_ERROR_REDUNDANT_TRANSITION,
	YOUTUBE_API_ERROR_INVALID_TRANSITION,
	PAR_API_ERROR_LIVE_BROADCAST_NOT_FOUND,
	PAR__API_ERROR_FORBIDDEN,
	VLIVE_API_ERROR_NO_PERMISSION,
	PAR_API_ERROR_Live_Invalid,
	PAR_API_ERROR_Scheduled_Time,
	BAND_API_ERROR_NO_PERMISSION,
	PAR_API_ERROR_Upload_Image,
	PAR_API_ERROR_StartLive_Other,
	PAR_API_ERROR_StartLive_User_Blocked
};

class PLSPlatformBase : public QObject {
	Q_OBJECT
public:
	virtual PLSServiceType getServiceType() const = 0;

	//Corresponding UI's channel name
	const char *getNameForChannelType() const { return NamesForChannelType[static_cast<int>(getServiceType())]; }

	//The key name in service.json
	const char *getNameForSettingId() const { return NamesForSettingId[static_cast<int>(getServiceType())]; }

	//The value of "platform" in "live/start" api
	const char *getNameForLiveStart() const { return NamesForLiveStart[static_cast<int>(getServiceType())]; }

	//relevant data from dashboard
	const QString getChannelUUID() const;
	const QString getChannelToken() const;
	const QString getChannelRefreshToken() const;
	const ChannelData::ChannelDataType getChannelType() const;
	const QString getChannelName() const;
	const int getChannelOrder() const;
	const QString getChannelCookie() const;

	PLSPlatformBase &setInitData(const QVariantMap &value)
	{
		m_mapInitData = value;
		onInitDataChanged();
		return *this;
	}
	const QVariantMap &getInitData();

	PLSPlatformBase &setActive(bool value)
	{
		m_bActive = value;
		return *this;
	}
	bool isActive() const { return m_bActive; }

	PLSPlatformBase &setSingleChannel(bool value)
	{
		m_bSingleChannel = value;
		return *this;
	}
	bool isSingleChannel() const { return m_bSingleChannel; }

	virtual bool isSendChatToMqtt() const { return false; }

	PLSPlatformBase &setChannelId(const string &value)
	{
		m_strChannelId = value;
		return *this;
	}
	const string &getChannelId() const { return m_strChannelId; }

	PLSPlatformBase &setUserName(const string &value)
	{
		m_strUserName = value;
		return *this;
	}
	const string &getUserName() const { return m_strUserName; }

	PLSPlatformBase &setDisplayName(const string &value)
	{
		m_strDisplayName = value;
		return *this;
	}
	const string &getDisplayName() const { return m_strDisplayName; }

	virtual PLSPlatformBase &setTitle(const string &value)
	{
		m_strTitle = value;
		return *this;
	}
	const string &getTitle() const { return m_strTitle; }

	virtual PLSPlatformBase &setCategory(const string &value)
	{
		m_strCategory = value;
		return *this;
	}
	const string &getCategory() const { return m_strCategory; }

	PLSPlatformBase &setStreamServer(const string &value)
	{
		m_strStreamServer = value;
		return *this;
	}
	const string &getStreamServer() const { return m_strStreamServer; };

	PLSPlatformBase &setStreamKey(const string &value)
	{
		m_strStreamKey = value;
		return *this;
	}
	const string &getStreamKey() const { return m_strStreamKey; }

	PLSPlatformBase &setDescription(const string &value)
	{
		m_strDescription = value;
		return *this;
	}
	const string &getDescription() const { return m_strDescription; }

	QString getStreamUrl() const { return QString::fromStdString(m_strStreamServer + "/" + m_strStreamKey); }

	PLSPlatformBase &setCurrStep(int value)
	{
		m_iCurrStep = value;
		return *this;
	}
	int getCurrentStep() const { return m_iCurrStep; }

	bool isApiPrepared() const { return m_bApiPrepared; };

	bool isApiStarted() const { return m_bApiStarted; };
	bool getIsSubChannelStartApiCall() const { return m_bSubChannelStartApiCall; };
	void setIsSubChannelStartApiCall(bool isCalled) { m_bSubChannelStartApiCall = isCalled; };

	PLSPlatformBase &setAlertParent(QWidget *value)
	{
		m_pAlertParent = value;
		return *this;
	}
	QWidget *getAlertParent()
	{
		if (m_pAlertParent.isNull()) {
			return PLSBasic::Get();
		}

		return m_pAlertParent;
	}

	PLSPlatformBase &setChannelLiveSeq(int value)
	{
		m_channelLiveSeq = value;
		return *this;
	}
	int getChannelLiveSeq() const { return m_channelLiveSeq; }

	PLSPlatformBase &setTokenRequestStatus(PLSTokenRequestStatus value)
	{
		m_tokenRequestStatus = value;
		return *this;
	}
	PLSTokenRequestStatus getTokenRequestStatus() const { return m_tokenRequestStatus; }

	/**
	* Occured when user activate a platform, may send some api to get necessary data
	* param: callback, notify result, true for no error, false for otherwise
	**/
	virtual void onActive()
	{
		setActive(true);
		activateCallback(true);
	};
	/**
	* Occured when user deactivate a platform
	* if in multicast, should stop single stream
	* param: callback, notify result, true for no error, false for otherwise
	**/
	virtual void onInactive()
	{
		setActive(false);
		deactivateCallback(true);
	};

	/**
	* Occured when ready to stream, typically, should get rtmp server&key
	*
	* param: value, the previous result of platform
	**/
	virtual void onPrepareLive(bool value) { prepareLiveCallback(value); }

	/**
	* Occured after all the platforms is prepared
	*
	* param: value, the result of all platforms
	*/
	virtual void onAllPrepareLive(bool) {}

	/**
	* Occured when output module succeed to stream
	* typically, may send some api after stream is started
	*
	* param: value, the previous result of platform
	**/
	virtual void onLiveStarted(bool value) { liveStartedCallback(value); }

	/**
	* Occured after all the platforms is started
	*
	* param: value, the result of all platforms
	*/
	virtual void onAlLiveStarted(bool) {}

	/**
	* Occured when ready to stop stream, may send some api before stop stream
	**/
	virtual void onPrepareFinish() { prepareFinishCallback(); }

	/**
	* Occured when stream is stopped, may send some api after stream is stopped
	**/
	virtual void onLiveStopped() { liveStoppedCallback(); }

	/**
	* Each onPrepareLive always have an onLiveEnded call,
	* whatever onPrepareLive is succeeded or failed.
	* if onPrepareLive is failed, there is no onLiveStarted & onLiveStopped.
	**/
	virtual void onLiveEnded() { liveEndedCallback(); }

	virtual QString getShareUrl() { return QString(); }
	virtual QString getServiceLiveLink() { return QString(); }

	virtual QJsonObject getWebChatParams();
	virtual QJsonObject getMqttChatParams();
	virtual QJsonObject getLiveStartParams();
	virtual QJsonObject getLiveEndParams() { return QJsonObject(); }
	virtual QJsonObject getDirectStartParams() { return QJsonObject(); }
	virtual QMap<QString, QString> getDirectEndParams() { return {}; }

	virtual void onInitDataChanged() {}

	virtual bool isMqttChatCanShow(const QJsonObject &);

protected:
	void activateCallback(bool);
	void deactivateCallback(bool);

	void prepareLiveCallback(bool value);
	void liveStartedCallback(bool value);
	void prepareFinishCallback();
	void liveStoppedCallback();
	void liveEndedCallback();

protected:
	bool m_bActive = false;
	bool m_bSingleChannel = false;

	QVariantMap m_mapInitData;

	string m_strChannelId;
	string m_strUserName;
	string m_strDisplayName;
	string m_strTitle;
	string m_strCategory;
	string m_strStreamServer;
	string m_strStreamKey;
	string m_strDescription;

	//The step index when click "Go Live" button
	int m_iCurrStep = 0;

	bool m_bApiPrepared = false;
	bool m_bApiStarted = false;

	//The parent to show alter dialog
	QPointer<QWidget> m_pAlertParent;

	//the prism live, each platform will have one sepecial seq
	int m_channelLiveSeq = 0;

	//When live stoped will clear the status;
	PLSTokenRequestStatus m_tokenRequestStatus;

	bool m_bSubChannelStartApiCall = false;
};
