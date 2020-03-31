/**
* @file		PLSPlatformApi.h
* @brief	A class to manage all platforms
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
**/

#pragma once

#include <vector>
#include <list>
#include <QObject>
#include <QTimer>
#include <QMutex>

#include "obs.hpp"
#include "PLSPlatformBase.hpp"
#include "rtmp/PLSPlatformRtmp.h"
#include "twitch/PLSPlatformTwitch.h"
#include "youtube/PLSPlatformYoutube.h"
#include "facebook/PLSPlatformFacebook.h"
#include "vlive/PLSPlatformVLive.h"
#include "PLSPlatformAfreecaTV.h"

enum class LiveStatus { Normal, PrepareLive, ToStart, LiveStarted, Living, PrepareFinish, ToStop, LiveStoped, LiveEnded };

#define BOOL2STR(x) (x) ? "true" : "false"

using namespace std;

class PLSPlatformApi : public QObject {
	Q_OBJECT

public:
	static PLSPlatformApi *instance();
	PLSPlatformApi();
	~PLSPlatformApi();

	bool initialize();

	void saveStreamSettings(const string &platform, const string &server, const string &key) const;

	//Used for title of live information
	int getTotalSteps() const { return m_iTotalSteps; };

	//Use prism server to live, or use each platform directly
	bool isPrismLive() const { return m_bPrismLive; };

	//Whether in activating a channel
	bool isDuringActivate() const { return m_bDuringActivate; }

	////Whether in deativating a channel
	bool isDuringDeactivate() const { return m_bDuringDeactivate; }

	LiveStatus getLiveStatus() const { return m_liveStatus; }

	//Whether in onPrepareStream status, User click "GoLive" button
	bool isPrepareLive() const { return LiveStatus::PrepareLive == m_liveStatus; };

	//Whether is living, the rtmp output module is started
	bool isLiving() const { return LiveStatus::Living == m_liveStatus; };

	bool isRecording() const { return m_bRecording; }

	//Whether in onPrepareStop status, User click "Finish" button
	bool isPrepareFinish() const { return LiveStatus::PrepareFinish == m_liveStatus; };

	//Long lifecycle, from user click "GoLive" button to live is ended
	bool isGoLive() const { return m_liveStatus > LiveStatus::Normal; };

	bool isGoLiveOrRecording() const { return isGoLive() || isRecording(); }

	//Whether user need to exit app
	bool isStopForExit() const { return m_bStopForExit; };

	//Whether platfrom prepared api is successful after GoLive
	bool isApiPrepared() const { return m_bApiPrepared; };

	//Whether platfrom relevant api is successful after stream is started
	bool isApiStarted() const { return m_bApiStarted; };

	const list<QString> &getUuidOnStarted() const { return uuidOnStarted; }

	const list<PLSPlatformBase *> &getAllPlatforms() const { return platformList; };
	list<PLSPlatformBase *> getActivePlatforms() const;

	bool isPlatformActived(PLSServiceType) const;

	PLSPlatformBase *getPlatformByType(PLSServiceType type, bool froceCreate = true);
	list<PLSPlatformBase *> getPlatformsByType(PLSServiceType type);
	PLSPlatformBase *getPlatformById(const QString &which, const QVariantMap &info, bool bRemove = false);
	PLSPlatformBase *getPlatformByName(const QString &name);
	PLSPlatformBase *getActivePlatformByName(const QString &name);

	PLSPlatformTwitch *getPlatformTwitch();
	PLSPlatformYoutube *getPlatformYoutube(bool froceCreate = true);
	PLSPlatformFacebook *getPlatformFacebook();
	PLSPlatformVLive *getPlatformVLive();
	PLSPlatformAfreecaTV *getPlatformAfreecaTV();

	void activateCallback(PLSPlatformBase *, bool);
	void deactivateCallback(PLSPlatformBase *, bool);

	void prepareLiveCallback(bool value);
	void liveStartedCallback(bool value);
	void prepareFinishCallback();
	void liveStoppedCallback();
	void liveEndedCallback();

	void showEndView(bool isRecord);
signals:

	void channelActive(const QString &uuid, bool value);
	void channelDeactive(const QString &uuid, bool value);
	void channelSelected(const QString &uuid, bool value);
	void channelRemoved(const QVariantMap &info);

	/**
	* Occured when user click "GoLive" button to prepare stream server&key
	* To notify dashboard whether prepare stream server&key is successful
	*/
	void livePrepared(bool value);

	/**
	* Occured all platform api is sended after stream is started
	* To notify dashboard whether api is successful after stream is started
	*/
	void liveStarted(bool value);

	/**
	* Occured all platform api is sended before stream is going to stop
	* To notify dashboard to stop stream
	*/
	void liveToStop();

	/**
	* Occured all platform api is sended after stream is stopped
	* May popup LiveEnd dialog
	* param apiPrepared, whether all the platform prepared api is successful
	* param apiStarted, whether all the platform special start api is successful
	* ***This signal is before liveEndedForUi***
	*/
	void liveEnded(bool apiPrepared, bool apiStarted);

	/**
	* Occured all platform api is sended after stream is stopped
	* To notify dashboard switch to "GoLive" status
	* ***This signal is after liveEnded***
	*/
	void liveEndedForUi();

	void outputStopped();

	/**
	* when click golive button, emit isEnterd=true
	* when start live failed or succeed, emit isEnterd=false
	**/
	void enterLivePrepareState(bool isEnterd);

	void liveEndPageShowComplected(bool isRecordEnd);

public slots:
	void onActive(const QString &which);
	void onInactive(const QString &which);

	void onPrepareLive();
	void onLiveStarted();
	void onPrepareFinish();
	void onLiveStopped();
	void onLiveEnded();

	void onRecordingStarted();
	void onRecordingStoped();

	void ensureStopOutput();

	void onAddChannel(const QString &which);
	void onRemoveChannel(const QString &which);
	void onUpdateChannel(const QString &which);
	void onClearChannel();

	void doChannelInitialized();
private:
	void saveStreamSettings(OBSData settings) const;

	PLSServiceType getServiceType(const QVariantMap &info) const;
	PLSPlatformBase *buildPlatform(PLSServiceType type);
	PLSPlatformBase *getPlatformBySimulcastSeq(int simulcastSeq);

	void setLiveStatus(LiveStatus);

	static bool isContainVliveChannel();
	static bool isValidChannel(const QVariantMap &info);

	static void onFrontendEvent(enum obs_frontend_event event, void *private_data);

	void showStartMessageIfNeeded();

private:
	mutable QMutex platformListMutex;
	list<PLSPlatformBase *> platformList;
	list<QString> uuidOnStarted;

	int m_iTotalSteps = 0;
	bool m_bPrismLive = false;

	bool m_bDuringActivate = false;
	bool m_bDuringDeactivate = false;
	LiveStatus m_liveStatus = LiveStatus::Normal;

	bool m_bApiPrepared = false;
	bool m_bApiStarted = false;

	bool m_bRecording = false;
	bool m_bStopForExit = false;
};

#define PLS_PLATFORM_API PLSPlatformApi::instance()

#define PLS_PLATFORM_ALL PLS_PLATFORM_API->getAllPlatforms()
#define PLS_PLATFORM_ACTIVIED PLS_PLATFORM_API->getActivePlatforms()

#define PLS_PLATFORM_TWITCH PLS_PLATFORM_API->getPlatformTwitch()
#define PLS_PLATFORM_YOUTUBE PLS_PLATFORM_API->getPlatformYoutube()
#define PLS_PLATFORM_FACEBOOK PLS_PLATFORM_API->getPlatformFacebook()
#define PLS_PLATFORM_VLIVE PLS_PLATFORM_API->getPlatformVLive()
#define PLS_PLATFORM_AFREECATV PLS_PLATFORM_API->getPlatformAfreecaTV()
