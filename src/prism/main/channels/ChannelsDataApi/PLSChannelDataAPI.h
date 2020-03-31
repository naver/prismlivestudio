#pragma once

#include <QDateTime>
#include <QMutex>
#include <QObject>
#include <QReadLocker>
#include <QReadWriteLock>
#include <QSemaphore>
#include <QStack>
#include <QTimer>
#include <QWriteLocker>
#include "PLSChannelPublicAPI.h"

class QNetworkAccessManager;
class ChannelDataBaseHandler;
using PlatformHandlerPtrs = QSharedPointer<ChannelDataBaseHandler>;
using InfosList = QList<QVariantMap>;

class PLSChannelDataAPI final : public QObject, public PLSChannelPublicAPI {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "PLSChannelPublicAPI.plugin")
	Q_INTERFACES(PLSChannelPublicAPI)
public:
	explicit PLSChannelDataAPI(QObject *parent = nullptr);
	~PLSChannelDataAPI();

	static PLSChannelDataAPI *getInstance();

	bool isOnlyRecordStop() { return mIsOnlyStopRecording; };
	void setIsOnlyStopRecord(bool isOnly) { mIsOnlyStopRecording = isOnly; };

	void beginTransactions(const QVariantMap &varMap = QVariantMap());
	void addTransaction(const QString &key, const QVariant &value);
	void addTask(const QVariant &function, const QVariant &parameters);
	int currentTransactionCMDType();
	int currentTaskQueueCount();
	void endTransactions();

	QSemaphore &getSourceSemaphore() { return mSemap; }
	void release();
	void acquire();
	bool tryAcquire() { return mSemap.tryAcquire(1); }
	bool isEmptyToAcquire();

	QReadWriteLock &getLock() { return mInfosLocker; }

	/*replace  only one channel info */
	void setChannelInfos(const QVariantMap &Infos, bool notify = true);

	/*add one channel*/
	void addChannelInfo(const QVariantMap &Infos, bool notify = true) override;

	//for rollback data when failed
	void backupInfo(const QString &uuid);
	void recoverInfo(const QString &uuid);
	void clearBackup(const QString &uuid);

	//for hold expired channel and resolve later
	void recordExpiredInfo(const QVariantMap &info);

	/* check if channel exists */
	bool isChannelInfoExists(const QString &);

	void addError(const QVariantMap &errorMap);
	const QVariantMap takeFirstError();
	bool hasError() { return !mErrorStack.isEmpty(); }
	void clearAllErrors() { mErrorStack.clear(); }
	/* check if no channels */
	bool isEmpty() const;

	/*count of channels */
	int count() const;

	/*current selected channels */
	const ChannelsMap getCurrentSelectedChannels();

	/*get current selected channels number*/
	int currentSelectedCount();
	//get current selected channel which can broadcast
	int currentSelectedValidCount();

	const ChannelsMap getCurrentSelectedPlatformChannels(const QString &platform, int Type = 0);

	/*get the channel of given name */
	const QVariantMap getChannelInfo(const QString &channelUUID);

	/* get the channel ref of given name for no copy */
	QVariantMap &getChanelInfoRef(const QString &channelUUID);

	QVariantMap &getChanelInfoRefByPlatformName(const QString &channelName, int type = 0);

	InfosList getChanelInfosByPlatformName(const QString &channelName, int type = 0);

	bool isChannelSelectedDisplay(const QString &uuid);

	//to check if the platform is enbaled
	bool isPlatformEnabled(const QString &platformName);
	void updatePlatformsStates();

	void updateRtmpGpopInfos();

	/*to check the status of channel */
	int getChannelStatus(const QString &channelUUID);
	void setChannelStatus(const QString &uuid, int state);

	/*to get channel user state if enabled */
	void setChannelUserStatus(const QString &channelUuid, int state, bool notify = true);
	int getChannelUserStatus(const QString &channelUUID);

	//to set all kind values of channel
	template<typename ValueType> inline bool setValueOfChannel(const QString &channelUUID, const QString &key, const ValueType &value)
	{
		QWriteLocker lokcer(&mInfosLocker);
		auto ite = mChannelInfos.find(channelUUID);
		if (ite != mChannelInfos.end()) {
			(*ite)[key] = QVariant::fromValue(value);
			return true;
		}
		return false;
	}

	//to get all kind values of channel
	template<typename ValueType> inline auto getValueOfChannel(const QString &channelUUID, const QString &key, const ValueType &defaultValue = ValueType()) -> ValueType
	{
		QReadLocker lokcer(&mInfosLocker);
		auto ite = mChannelInfos.find(channelUUID);
		if (ite != mChannelInfos.end()) {
			return getInfo(*ite, key, defaultValue);
		}
		return defaultValue;
	}

	/*get all channels info for copy */
	const ChannelsMap getAllChannelInfo() override;

	/* get all channels info for reference */
	ChannelsMap &getAllChannelInfoReference();

	//remove channel routered by platform
	void tryRemoveChannel(const QString &channelUUID, bool notify = true, bool notifyServer = true);
	/* delete channel info of name ,the widget will be deleted later  it maybe RTMP */
	void removeChannelInfo(const QString &channelUUID, bool notify = true, bool notifyServer = true);
	//delte channel by platform ,it may be channel logined
	void removeChannelsByPlatformName(const QString &platformName, int type = 0, bool notify = true, bool notifyServer = true);

	/*get the sorted order of channels */
	QStringList getCurrentSortedChannelsUUID();

	InfosList sortAllChannels();

	/*delete all channels info and widgets will be deleted later */
	void clearAll();

	bool isExit() { return mIsExit; }
	void setExitState(bool isExi = true) { mIsExit = isExi; }

	/* load channels data of uer */
	void reloadData();

	//click stop record button to stop record;
	uint getIsClickToStopRecord() { return m_isClickToStopRecord; };
	void setIsClickToStopRecord(bool value) { m_isClickToStopRecord = value; };

	/*get channel active status*/
	bool isLivingOrRecording();
	bool isLiving();
	bool isShifting();
	bool isRecording();

	bool isInWholeBroadCasting();
	bool isInWholeRecording();

	/* check if recording */
	void setRecordState(int state);
	int currentReocrdState() const;

	void setRehearsal(bool isRehearsal = true) { mIsRehearsal = isRehearsal; }
	bool isRehearsaling() { return mIsRehearsal; }

	/*set and get current broadcast */
	void setBroadcastState(int state);
	int currentBroadcastState() const;

	void resetInitializeState(bool);
	bool isInitilized() { return mInitilized; }

	bool isResetNeed() { return mIsResetNeed; }
	void setResetNeed(bool isNeed = false) { mIsResetNeed = isNeed; }

	/*get network*/
	QNetworkAccessManager *getNetWorkAPI() { return mNetWorkAPI; }

	void registerPlatformHandler(ChannelDataBaseHandler *handler);
	PlatformHandlerPtrs getPlatformHandler(const QString &platformName);

	bool isPlatformMultiChildren(const QString &platformName);
	bool isChannelMultiChildren(const QString &uuid);

	const QStringList &getRTMPsName() { return mRtmpPlatforms; }

	const QMap<QString, QString> &getRTMPInfos() { return mRtmps; }

#ifdef DEBUG
	/*for develop use for get what to translate when debug*/
	void RegisterStr(const QString &);

	void saveTranslations();

#endif // DEBUG

public slots:

	void stopAll();
	void exitApi();
	void finishAdding(const QString &uuid);
	void moveToNewThread(QThread *newThread);
	/* save channels data*/
	void saveData();
	void delaySave();

signals:

	void channelAdded(const QString &channelUUID);
	void channelRemoved(const QString &channelUUID);
	void channelModified(const QString &channelUUID);
	void channelExpired(const QString &channelUUID, bool toAsk = true);
	void channelGoToInitialize(const QString &channelUUID);

	/*recording sigs*/
	void toStartRecord();
	void toStopRecord();
	void recordingChanged(int);

	void sigTrySetRecordState(int);
	void recordReady();
	void canRecord();
	void recordStarting();
	void recordStarted();
	void recordStopGo();
	void recordStopping();
	void recordStopped();
	/*recording end*/

	/*broadcast sigs*/
	void toStartBroadcast();
	void toStopBroadcast();
	void rehearsalBegin();

	void sigTrySetBroadcastState(int state);
	void inReady();
	void broadcastGo(); //
	void canBroadcast();
	void toBeBroadcasting();
	void broadcasting();
	void stopBroadcastGo();
	void stopping();
	void broadcastStopped();
	void broadCastEnded();
	void liveStateChanged(int state);
	/*broadcast sigs end*/

	void channelActiveChanged(const QString &channelUUID, bool enable);

	void holdOnChannelArea(bool hold = true);

	void lockerReleased();

	void addingHold(bool hold = true);

	void holdOnGolive(bool);

	void sigAllClear();

	void toDoinitialize();

	void sigChannelAreaInialized();

	void networkInvalidOcurred();

	void prismTokenExpired();

	void sigSendRequest(const QString &uuid);

	void sigTryToAddChannel(const QString &uuid);

	void sigTryToUpdateChannel(const QString &uuid);

	void sigTryRemoveChannel(const QString &uuid, bool nofify = true, bool nofityServer = true);

	void sigUpdateAllRtmp();

	void sigRefreshAllChannels();

	void sigRefreshToken(const QString &uuid, bool isForce = false);

	void tokenRefreshed(const QString &uuid, int returnCode);

	void sigAllChannelRefreshDone();

private:
	void registerEnumsForStream();
	void connectSignals();
	void reCheckExpiredChannels();

	//delete function to copy
	PLSChannelDataAPI(const PLSChannelDataAPI &) = delete;
	PLSChannelDataAPI &operator=(const PLSChannelDataAPI &) = delete;

private:
	ChannelsMap mChannelInfos; // all curretn channels info
	ChannelsMap mBackupInfos;
	ChannelsMap mExpiredChannels;

	QStack<QVariantMap> mErrorStack;

	QNetworkAccessManager *mNetWorkAPI;
	//flags to handle

	bool mIsOnlyStopRecording;          //when record is stopped by click button,the flag is set
	bool m_isClickToStopRecord = false; //click stop record button to stop record;
	bool mInitilized;                   //to set data is initialized
	bool mIsExit;                       //when app to be quit ,the flag is set
	bool mIsResetNeed;                  //when refresh,the flag is set
	bool mIsRehearsal;                  //when channel is rehearsal,the flag is set

	QSemaphore mSemap; //semaphore for hold task count
	QReadWriteLock mInfosLocker;
	QReadWriteLock mFileLocker;

	int mBroadcastState;
	int mRecordState;

	//update fro gpop
	QMap<QString, QString> mRtmps;
	QStringList mRtmpPlatforms;
	QMap<QString, bool> mPlatformStates;

	//use for update platform
	QMap<QString, PlatformHandlerPtrs> mPlatformHandler;

	QTimer *mSaveDelayTimer;
	QVariantMap mTransactions;

	static PLSChannelDataAPI *mInstance;
};
#define PLSCHANNELS_API PLSChannelDataAPI::getInstance()

/***************************************************************/
/*
channels data store ChannelsMap map like json below

all keys is defined in file ChannelConst.h

ChannelsMap {

	twitch:{
	"uuid:"......
	"name":"twitch";
	"url":"www.twitch.......";
	........
	},

	facebook:{
	"name":"facebook";
	"url":"www.facebook.......";
	........
	},
	......

	twitter:{
	"name":"twitter";
	"url":"www.twitter.......";
	........
	}
}

*/
/*****************************************************************************/
