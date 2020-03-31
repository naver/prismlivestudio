#include "PLSChannelsVirualAPI.h"
#include <qobject.h>
#include <QDialog>
#include <QJsonObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QUUid>
#include <QThread>
#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "ChannelsSettingsWidget.h"
#include "LoadingFrame.h"
#include "LogPredefine.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelDataHandler.h"
#include "PLSChannelDataHandlerFunctions.h"
#include "PLSChannelsEntrance.h"
#include "PLSChatDialog.h"
#include "PLSLiveEndDialog.h"
#include "PLSPlatformApi.h"
#include "PLSPlatformApi/PLSLiveInfoDialogs.h"
#include "PLSRtmpChannelView.h"
#include "PLSShareSourceItem.h"
#include "alert-view.hpp"
#include "frontend-api.h"
#include "main-view.hpp"
#include "window-basic-main.hpp"

using namespace ChannelData;

#define BOOL_To_STR(x) (x) ? "true" : "false"

struct HotKeyLocker {
	HotKeyLocker() { App()->DisableHotkeys(); }
	~HotKeyLocker() { App()->UpdateHotkeyFocusSetting(true); }
};

void showLiveInfo(const QString &uuid)
{
	auto info = PLSCHANNELS_API->getChannelInfo(uuid);
	if (info.isEmpty()) {
		return;
	}
	pls_exec_live_Info(info);
	auto newInfo = PLSCHANNELS_API->getChannelInfo(uuid);
	if (newInfo != info) {
		PLSCHANNELS_API->channelModified(uuid);
	}
}

void showChannelInfo(const QString &uuid)
{
	auto info = PLSCHANNELS_API->getChannelInfo(uuid);
	if (info.isEmpty()) {
		return;
	}
	int infoType = getInfo(info, g_data_type, NoType);

	switch (infoType) {
	case ChannelType: {
		showLiveInfo(uuid);
	} break;
	case RTMPType:
		editRTMP(uuid);
		break;
	default:
		break;
	}
}

bool updateChannelInfoFromNet(const QString &uuid)
{
	Q_ASSERT(!uuid.isEmpty());
	auto info = PLSCHANNELS_API->getChannelInfo(uuid);
	if (info.isEmpty()) {
		return false;
	}
	int type = getInfo(info, g_data_type, ChannelType);
	switch (type) {
	case ChannelType:
		return updateChannelTypeFromNet(uuid);
		break;
	case RTMPType:
		return updateRTMPTypeFromNet(uuid);
		break;
	default:
		break;
	}
	return true;
}

void sortInfosByNickName(InfosList &infos)
{
	auto isLessThan = [](const QVariantMap &left, const QVariantMap &right) { return QString::localeAwareCompare(getInfo(left, g_nickName), getInfo(right, g_nickName)) < 0; };
	std::sort(infos.begin(), infos.end(), isLessThan);
}

void setShowSettingTransation(const QString &platformName)
{
	QVariantList tasks;

	QVariantMap task;
	QVariant parameter = platformName;
	TaskFun func = [](const QVariant &para) {
		showChannelsSetting(para.toString());
		return;
	};
	task.insert(ChannelTransactionsKeys::g_functions, QVariant::fromValue(func));
	task.insert(ChannelTransactionsKeys::g_taskParameters, parameter);
	task.insert(ChannelTransactionsKeys::g_context, QVariant::fromValue(getMainWindow()));

	tasks.append(task);
	PLSCHANNELS_API->addTransaction(ChannelTransactionsKeys::g_taskQueue, tasks);
}

void checkIfSettingNeedShow(int newInfoCount, int currentTransactionType, const QString &platformName)
{
	if (newInfoCount <= 1 || PLSCHANNELS_API->currentTaskQueueCount() > 0) {
		return;
	}

	if (currentTransactionType == ChannelTransactionsKeys::AddChannelCMD) {
		setShowSettingTransation(platformName);
		return;
	}
	setShowSettingTransation("");
	return;
}

void removeNotExist(const InfosList &oldInfos, const InfosList &taskInfos)
{
	for (const auto &oldInfo : oldInfos) {
		auto oldSubId = getInfo(oldInfo, g_subChannelId);
		auto isChannelEqual = [&](const QVariantMap &info) { return getInfo(info, g_subChannelId) == oldSubId; };
		auto findIte = std::find_if(taskInfos.constBegin(), taskInfos.constEnd(), isChannelEqual);
		if (findIte == taskInfos.constEnd()) {
			bool isNotify = getInfo(oldInfo, g_isUpdated, false);
			PLSCHANNELS_API->removeChannelInfo(getInfo(oldInfo, g_channelUUID), isNotify, false);
		}
	}
}

void handleUpdateRet(const InfosList &infos)
{
	if (infos.isEmpty()) {
		endRefresh();
		return;
	}
	auto sortedInfos = infos;
	sortInfosByNickName(sortedInfos);

	InfosList taskInfos;
	auto platformName = getInfo(sortedInfos.first(), g_channelName);
	bool isMulti = PLSCHANNELS_API->isPlatformMultiChildren(platformName);
	auto oldInfos = PLSCHANNELS_API->getChanelInfosByPlatformName(platformName, ChannelType);
	int currentTransactionType = PLSCHANNELS_API->currentTransactionCMDType();
	int newInfoCount = sortedInfos.count();
	for (int i = 0; i < newInfoCount; ++i) {
		auto &newInfo = sortedInfos[i];
		newInfo[g_isLeader] = (i == 0 ? true : false);
		auto nickname = getInfo(newInfo, g_nickName);
		if (nickname.isEmpty()) {
			PRE_LOG_MSG(QString("empty nick name:" + platformName).toStdString().c_str(), ERROR);
			newInfo.insert(g_channelStatus, Error);
		}
		int status = getInfo(newInfo, g_channelStatus, Error);
		if (status == Error) {
			taskInfos.append(newInfo);
			goto finish;
		}
		QVariantMap baseInfo;
		auto doUpdateChannelInfo = [&](const QVariantMap &source) {
			PLSCHANNELS_API->setChannelInfos(source, false);
			taskInfos.append(source);
		};
		if (!isMulti) {
			doUpdateChannelInfo(newInfo);
			goto finish;
		}

		newInfo.remove(g_displayState);
		auto newSubId = getInfo(newInfo, g_subChannelId);
		auto isChannelEqual = [&](const QVariantMap &info) { return getInfo(info, g_subChannelId) == newSubId; };
		auto findIte = std::find_if(oldInfos.constBegin(), oldInfos.constEnd(), isChannelEqual);
		if (findIte == oldInfos.constEnd()) {
			baseInfo = createDefaultChannelInfoMap(platformName, ChannelType);
			newInfo[g_isUpdated] = false;

			if (i == 0 && currentTransactionType == ChannelTransactionsKeys::AddChannelCMD) {
				newInfo[g_displayState] = true;
			} else {
				newInfo[g_displayState] = false;
			}

			addToMap(baseInfo, newInfo);
			PLSCHANNELS_API->addChannelInfo(baseInfo, false);
			taskInfos.append(baseInfo);
			checkIfSettingNeedShow(newInfoCount, currentTransactionType, platformName);
			continue;
		}
		//update old
		{
			baseInfo = *findIte;
			addToMap(baseInfo, newInfo);
			doUpdateChannelInfo(baseInfo);
			continue;
		}
	}

	//to do delete work
	if (isMulti) {
		removeNotExist(oldInfos, taskInfos);
	}

finish:
	//refresh
	PLSCHANNELS_API->sortAllChannels();
	for (const auto &info : taskInfos) {
		if (!handleChannelStatus(info)) {
			break;
		}
	}

	endRefresh();
	PLSCHANNELS_API->sigAllChannelRefreshDone();
}

bool handleChannelStatus(const QVariantMap &info)
{
	bool isContinue = false;
	int status = getInfo(info, g_channelStatus, Error);
	auto uuid = getInfo(info, g_channelUUID);

	switch (status) {

	case UnInitialized: {
		PLSCHANNELS_API->setValueOfChannel(uuid, g_channelUserStatus, int(Disabled));
		PLSCHANNELS_API->setValueOfChannel(uuid, g_errorString, CHANNELS_TR(emptyChannelError));
		PLSCHANNELS_API->channelGoToInitialize(uuid);
		FinishTaskReleaser releaser(uuid);
	} break;
	case UnAuthorized:
	case Expired: {
		PLSCHANNELS_API->channelExpired(uuid);
	} break;
	case WaitingActive: {
		PLSCHANNELS_API->setValueOfChannel(uuid, g_channelUserStatus, Disabled);
		PLSCHANNELS_API->setValueOfChannel(uuid, g_errorString, CHANNELS_TR(initialChannelError));
		FinishTaskReleaser releaser(uuid);
	} break;
	case EmptyChannel: {
		PLSCHANNELS_API->setValueOfChannel(uuid, g_channelUserStatus, Disabled);
		PLSCHANNELS_API->setValueOfChannel(uuid, g_errorString, CHANNELS_TR(emptyChannelError));
		PLSCHANNELS_API->channelGoToInitialize(uuid);
		FinishTaskReleaser releaser(uuid);
	} break;
	case LoginError: {
		QVariantMap errormap;
		errormap.insert(g_errorTitle, CHANNELS_TR(Check.Alert.Title));
		errormap.insert(g_errorString, CHANNELS_TR(Check.Login.Error));
		PLSCHANNELS_API->addError(errormap);
		bool isUpdated = getInfo(info, g_isUpdated, false);
		PLSCHANNELS_API->sigTryRemoveChannel(uuid, isUpdated, false);

	} break;
	case Error: {
		int errorType = getInfo(info, g_errorType, NetWorkErrorType::NetWorkNoStable);
		addErrorForType(errorType);

		bool isUpdated = getInfo(info, g_isUpdated, false);
		if (!isUpdated) {
			PLSCHANNELS_API->sigTryRemoveChannel(uuid, false, false);
		}
	} break;
	case Valid: {
		auto displayname = getInfo(info, g_nickName);
		if (displayname.isEmpty()) {
			auto platform = getInfo(info, g_channelName);
			PRE_LOG_MSG(QString("empty nick name:" + platform).toStdString().c_str(), ERROR);
			addErrorForType(NetWorkErrorType::UnknownError);
			break;
		}
		{
			isContinue = true;
			FinishTaskReleaser releaser(uuid);
		}
	} break;
	default: {
		isContinue = false;
	} break;
	} //end switch

	return isContinue;
}

bool updateChannelTypeFromNet(const QString &uuid)
{
	auto info = PLSCHANNELS_API->getChannelInfo(uuid);
	if (info.isEmpty()) {
		return false;
	}

	bool isLeader = getInfo(info, g_isLeader, true);
	if (!isLeader) {
		return true;
	}

	QString name = getInfo(info, g_channelName, QString());

	if (!PLSCHANNELS_API->isPlatformEnabled(name)) {
		PLSCHANNELS_API->removeChannelsByPlatformName(name, ChannelType, true, false);
		PLSCHANNELS_API->addingHold(false);
		return true;
	}

	auto handler = PLSCHANNELS_API->getPlatformHandler(name);
	if (handler) {
		PLSCHANNELS_API->release();
		if (!handler->tryToUpdate(info, handleUpdateRet)) {
			PRE_LOG_MSG(QString("Error on update platform:" + name).toStdString().c_str(), ERROR);
			endRefresh();
			return false;
		}
		return true;
	}

	PLSCHANNELS_API->removeChannelInfo(uuid, false, false);
	PLSCHANNELS_API->addingHold(false);
	return false;
}

bool updateRTMPTypeFromNet(const QString &uuid)
{
	if (!PLSCHANNELS_API->isInitilized()) {
		return false;
	}
	auto &info = PLSCHANNELS_API->getChanelInfoRef(uuid);
	if (info.isEmpty()) {
		return false;
	}
	QString seq = getInfo(info, g_rtmpSeq, QString());
	if (seq.isEmpty()) {
		return RTMPAddToPrism(uuid);
	} else {
		return RTMPUpdateToPrism(uuid);
	}
}

QVariantMap createErrorMap(int errorType)
{

	QVariantMap errormap;
	switch (errorType) {

	case NetWorkErrorType::NetWorkNoStable: {
		errormap.insert(g_errorTitle, CHANNELS_TR(Check.Alert.Title));
		errormap.insert(g_errorString, CHANNELS_TR(Check.Network.Error));
	} break;
	default: {
		errormap.insert(g_errorTitle, CHANNELS_TR(Check.Alert.Title));
		errormap.insert(g_errorString, CHANNELS_TR(Check.Network.UnknownError));
	} break;
	}

	return errormap;
}

void addErrorForType(int errorType)
{
	auto errorMap = createErrorMap(errorType);
	PLSCHANNELS_API->addError(errorMap);
}

void refreshChannel(const QString &uuid)
{
	auto info = PLSCHANNELS_API->getChannelInfo(uuid);
	if (info.isEmpty()) {
		return;
	}
	int type = getInfo(info, g_data_type, ChannelType);
	switch (type) {
	case ChannelType:
		updateChannelTypeFromNet(uuid);
		break;
	case RTMPType:
		resetChannel(uuid);
		break;
	default:
		break;
	}
}

void resetChannel(const QString &uuid)
{
	auto &info = PLSCHANNELS_API->getChanelInfoRef(uuid);
	int state = getInfo(info, g_channelStatus, Error);

	switch (state) {
	case Error:
		info[g_channelStatus] = Valid;
		PLSCHANNELS_API->channelModified(uuid);
		break;
	case Expired:
	case UnAuthorized:
		PLSCHANNELS_API->sigTryRemoveChannel(uuid, true, false);
		break;
	default:
		PLSCHANNELS_API->channelModified(uuid);
		break;
	}
}

void resetExpiredChannel(const QString &uuid, bool toAsk)
{
	if (!PLSCHANNELS_API->isInitilized()) {
		auto info = PLSCHANNELS_API->getChannelInfo(uuid);
		PLSCHANNELS_API->recordExpiredInfo(info);
		PLSCHANNELS_API->tryRemoveChannel(uuid, true, false);
		return;
	}

	if (PLSCHANNELS_API->isLiving()) {
		PLSCHANNELS_API->setChannelStatus(uuid, Expired);
		return;
	}

	auto info = PLSCHANNELS_API->getChannelInfo(uuid);
	if (info.isEmpty()) {
		return;
	}
	PLSCHANNELS_API->tryRemoveChannel(uuid, true, false);

	auto name = getInfo(info, g_channelName);
	reloginChannel(name, toAsk);
}

void reloginChannel(const QString &platformName, bool toAsk)
{
	HotKeyLocker locker;
	if (toAsk) {
		auto ret = PLSAlertView::warning(getMainWindow(), CHANNELS_TR(Confirm), CHANNELS_TR(expiredQuestion).arg(platformName), {{PLSAlertView::Button::Yes, CHANNELS_TR(Yes)}});
		if (ret != PLSAlertView::Button::Yes) {
			return;
		}
	}

	QTimer::singleShot(500, [=]() { runCMD(platformName); });
}

void reloginPrismExpired()
{
	return;
}

void resetAllChannels()
{
	auto channles = PLSCHANNELS_API->getAllChannelInfo();
	auto ite = channles.begin();
	while (ite != channles.end()) {
		int type = getInfo(*ite, g_data_type, NoType);
		switch (type) {
		case RTMPType:
		case ChannelType:
			resetChannel(ite.key());
			break;
		default:
			break;
		}
		++ite;
	}
}

void refreshAllChannels()
{
	HolderReleaser releaser(&PLSChannelDataAPI::holdOnChannelArea);
	auto channels = PLSCHANNELS_API->getAllChannelInfo();
	if (!channels.isEmpty()) {
		SemaphoreHolder holder(PLSCHANNELS_API->getSourceSemaphore());
		PLSCHANNELS_API->addTransaction(ChannelTransactionsKeys::g_CMDType, ChannelTransactionsKeys::RefreshChannelsCMD);
		auto ite = channels.begin();
		while (ite != channels.end()) {

			const auto &info = ite.value();
			const auto &uuid = ite.key();
			++ite;
			bool isLeader = getInfo(info, g_isLeader, true);
			if (!isLeader) {
				continue;
			}
			refreshChannel(uuid);
		}
	}
	PLSCHANNELS_API->sigUpdateAllRtmp();
}

void handleEmptyChannel(const QString &uuid)
{
	if (!PLSCHANNELS_API->isInitilized()) {
		return;
	}
	auto &info = PLSCHANNELS_API->getChanelInfoRef(uuid);
	bool isAsked = getInfo(info, g_isUserAsked, false);
	if (isAsked) {
		return;
	}
	auto platformName = getInfo(info, g_channelName);
	if (platformName == YOUTUBE) {
		auto ret = PLSAlertView::question(nullptr, CHANNELS_TR(Confirm), CHANNELS_TR(GotoYoutubetoSet),
						  {{PLSAlertView::Button::Yes, CHANNELS_TR(GotoYoutubeOk)}, {PLSAlertView::Button::Cancel, CHANNELS_TR(Cancel)}}, PLSAlertView::Button::Cancel);

		info[g_isUserAsked] = true;
		if (ret != PLSAlertView::Button::Yes) {
			return;
		}
		QDesktopServices::openUrl(QUrl(g_yoububeLivePage));
		return;
	}

	info[g_isUserAsked] = true;
	PLSAlertView::warning(getMainWindow(), CHANNELS_TR(Alert), CHANNELS_TR(EmptyChannelMessage));
	return;
}

void addRTMP()
{
	HotKeyLocker locker;
	auto rtmpInfo = createDefaultChannelInfoMap(SELECT, RTMPType);
	PLSRtmpChannelView *rtmpView = new PLSRtmpChannelView(rtmpInfo, getMainWindow());
	if (rtmpView->exec() == QDialog::Accepted) {
		rtmpInfo = rtmpView->SaveResult();
		rtmpInfo[g_channelStatus] = QVariant::fromValue(Valid);
		PLSCHANNELS_API->addChannelInfo(rtmpInfo);
	}
	rtmpView->deleteLater();
}

void editRTMP(const QString &uuid)
{
	HotKeyLocker locker;
	auto channelInfo = PLSCHANNELS_API->getChannelInfo(uuid);
	PLSRtmpChannelView *rtmpView = new PLSRtmpChannelView(channelInfo, getMainWindow());
	if (rtmpView->exec() == QDialog::Accepted) {
		channelInfo = rtmpView->SaveResult();
		PLSCHANNELS_API->backupInfo(uuid);
		PLSCHANNELS_API->setChannelInfos(channelInfo, false);
		PLSCHANNELS_API->sigTryToUpdateChannel(uuid);
	}
	rtmpView->deleteLater();
}

void runCMD(const QString &cmdStr)
{
	static bool isBegin = false;
	if (isBegin) {
		PRE_LOG_MSG(QString("run add channel twice......" + cmdStr).toStdString().c_str(), INFO);
		return;
	}

	isBegin = true;
	PRE_LOG_MSG(QString("run add channel:" + cmdStr).toStdString().c_str(), INFO);
	if (PLSCHANNELS_API->count() >= 100) {
		HotKeyLocker locker;
		PLSAlertView::warning(getMainWindow(), CHANNELS_TR(Alert), CHANNELS_TR(MaxChannels));
		isBegin = false;
		return;
	}

	if (cmdStr == CUSTOM_RTMP) {
		addRTMP();
		isBegin = false;
		return;
	}

	if (gDefaultPlatform.contains(cmdStr, Qt::CaseInsensitive)) {
		PLSCHANNELS_API->addTransaction(ChannelTransactionsKeys::g_CMDType, ChannelTransactionsKeys::AddChannelCMD);
		QVariantMap channelInfo = createDefaultChannelInfoMap(cmdStr);
		QJsonObject retObj = QJsonObject::fromVariantMap(channelInfo);
		if (pls_channel_login(retObj, cmdStr, pls_get_main_view())) {
			addLoginChannel(retObj);
		}
	}

	isBegin = false;
}

/****************streaming ***************************/
Qt::ConnectionType getInvokeType()
{
	//qDebug() << " current thread " << QThread::currentThread() << " channels thread " << PLSCHANNELS_API->thread();
	return (QThread::currentThread() != getMainWindow()->thread() ? Qt::BlockingQueuedConnection : Qt::AutoConnection);
}

bool checkChannelsState()
{
	if (PLSCHANNELS_API->currentSelectedCount() == 0) {
		auto showWarning = []() { PLSAlertView::warning(getMainWindow(), CHANNELS_TR(SelectOneErrorTitle), CHANNELS_TR(ErrorEmptyChannelMessage)); };
		QMetaObject::invokeMethod(getMainWindow(), showWarning, getInvokeType());
		return false;
	}
	return true;
}

bool startStreamingCheck()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool retVal = false;
	bool isok = QMetaObject::invokeMethod(mainw, "startStreamingCheck", getInvokeType(), Q_RETURN_ARG(bool, retVal));
	if (!isok) {
		PRE_LOG("error :startStreamingCheck", ERROR);
	}
	return retVal && isok;
}

bool toGoLive()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool retVal = false;
	bool isok = QMetaObject::invokeMethod(mainw, "StartStreaming", getInvokeType(), Q_RETURN_ARG(bool, retVal));
	if (!isok || !retVal) {
		PRE_LOG("error :StartStreaming", ERROR);
	}
	return isok && retVal;
}

bool stopStreamingCheck()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool retVal = false;
	bool isok = QMetaObject::invokeMethod(mainw, "stopStreamingCheck", getInvokeType(), Q_RETURN_ARG(bool, retVal));
	if (!isok) {
		PRE_LOG("error :startStreamingCheck", ERROR);
	}
	return isok && retVal;
}

bool toStopLive()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool isok = QMetaObject::invokeMethod(mainw, "StopStreaming", getInvokeType());
	if (!isok) {
		PRE_LOG("error :StopStreaming", ERROR);
	}
	return isok;
}
/****************streaming ** end *************************/

/**************** recording  ***************************/
bool startRecordCheck()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool retVal = false;
	bool isok = QMetaObject::invokeMethod(mainw, "startRecordCheck", getInvokeType(), Q_RETURN_ARG(bool, retVal));
	if (!isok) {
		PRE_LOG("error :startRecordCheck ", ERROR);
	}
	return retVal && isok;
}

bool toRecord()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool retVal = false;
	bool isok = QMetaObject::invokeMethod(mainw, "StartRecording", getInvokeType(), Q_RETURN_ARG(bool, retVal));
	if (!isok) {
		PRE_LOG("error :StartRecording ", ERROR);
	}
	return isok && retVal;
}

bool stopRecordCheck()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool retVal = false;
	bool isok = QMetaObject::invokeMethod(mainw, "stopRecordCheck", getInvokeType(), Q_RETURN_ARG(bool, retVal));
	if (!isok) {
		PRE_LOG("error :stopRecordCheck ", ERROR);
	}
	return retVal && isok;
}

bool toTryStopRecord()
{
	auto mainw = getMainWindow();
	Q_ASSERT(mainw);
	bool retVal = false;
	bool isok = QMetaObject::invokeMethod(mainw, "StopRecording", getInvokeType(), Q_RETURN_ARG(bool, retVal));
	if (!isok || !retVal) {
		PRE_LOG("error :StopRecording ", ERROR);
	}
	return isok && retVal;
}

/**************** recording  end ***************************/

void childExclusive(const QString &channelID)
{
	auto platformName = PLSCHANNELS_API->getValueOfChannel(channelID, g_channelName, QString());
	if (PLSCHANNELS_API->isPlatformMultiChildren(platformName)) {
		int myType = PLSCHANNELS_API->getValueOfChannel(channelID, g_data_type, NoType);
		auto allChildren = PLSCHANNELS_API->getChanelInfosByPlatformName(platformName, myType);
		for (const auto &child : allChildren) {
			auto uuid = getInfo(child, g_channelUUID);
			int userStatus = getInfo(child, g_channelUserStatus, Disabled);
			if (userStatus == Enabled && uuid != channelID) {
				PLSCHANNELS_API->setChannelUserStatus(uuid, Disabled);
			}
		}
	}
}

bool isExclusiveChannel(const QString &uuid)
{
	auto &info = PLSCHANNELS_API->getChanelInfoRef(uuid);
	return isExclusiveChannel(info);
}

bool isExclusiveChannel(const QVariantMap &info)
{
	auto platName = getInfo(info, g_channelName);
	auto rtmpUrl = getInfo(info, g_channelRtmpUrl);
	return platName.contains(NOW) || platName.contains(VLIVE) || rtmpUrl.contains(g_exclusiveRtmpUrl, Qt::CaseInsensitive);
}

bool isEnabledExclusiveChannelExist()
{
	return !findExistEnabledExclusiveChannel().isEmpty();
}

QString findExistEnabledExclusiveChannel()
{
	QString ret;
	const auto &channels = PLSCHANNELS_API->getAllChannelInfoReference();
	auto isEnabledNow = [](const QVariantMap &info) { return isExclusiveChannel(info) && getInfo(info, g_channelUserStatus, Disabled) == Enabled; };
	auto retIte = std::find_if(channels.cbegin(), channels.cend(), isEnabledNow);
	if (retIte != channels.cend()) {
		ret = retIte.key();
	}
	return ret;
}

void exclusiveChannelCheckAndVerify(const QVariantMap &Newinfo)
{
	if (isExclusiveChannel(Newinfo)) {
		disableAll();
	} else {
		auto exclusiveID = findExistEnabledExclusiveChannel();
		if (!exclusiveID.isEmpty()) {
			PLSCHANNELS_API->setChannelUserStatus(exclusiveID, Disabled);
		}
	}
}

void disableAll()
{
	const auto &channels = PLSCHANNELS_API->getAllChannelInfoReference();
	auto ite = channels.cbegin();
	while (ite != channels.cend()) {
		PLSCHANNELS_API->setChannelUserStatus(ite.key(), Disabled);
		++ite;
	}
}

void addLoginChannel(const QJsonObject &retJson)
{
	auto newInfo = retJson.toVariantMap();
	PLSCHANNELS_API->addChannelInfo(newInfo);
}

void showShareView(const QString &channelUUID)
{
	auto &info = PLSCHANNELS_API->getChanelInfoRef(channelUUID);
	if (info.isEmpty()) {
		return;
	}

	if (getInfo(info, g_shareUrl).isEmpty() && getInfo(info, g_shareUrlTemp).isEmpty()) {
		PLSAlertView::warning(getMainWindow(), CHANNELS_TR(Alert), CHANNELS_TR(EmptyShareUrl).arg(getInfo(info, g_channelName)));
		return;
	}

	auto *dialog = new PLSShareSourceItem(App()->getMainView());
	dialog->setAttribute(Qt::WA_DeleteOnClose, true);
	dialog->setWindowFlag(Qt::Popup);
	dialog->show();
	dialog->initInfo(info);
}

int showSummaryView()
{
	return 0;
}

PLSChatDialog *g_dialogChat = nullptr;

void showChatView(bool isRebackLogin, bool isOnlyShow, bool isOnlyInit)
{
}

LoadingFrame *createBusyFrame(QWidget *parent)
{
	LoadingFrame *loadView = new LoadingFrame(parent);
	loadView->initialize(":/images/loading-1.svg", 500);
	loadView->show();
	QPoint position;
	if (parent) {
		position = parent->mapToGlobal(QPoint(parent->rect().center())) - loadView->rect().center();
	} else {
		position = QCursor::pos();
	}
	loadView->move(position);
	return loadView;
}

void showNetworkErrorAlert()
{
	if (!PLSCHANNELS_API->isInitilized()) {
		return;
	}

	if (PLSCHANNELS_API->hasError() && PLSCHANNELS_API->isEmptyToAcquire()) {
		auto errorMap = PLSCHANNELS_API->takeFirstError();
		SemaphoreHolder holder(PLSCHANNELS_API->getSourceSemaphore());
		PLSAlertView::warning(getMainWindow(), getInfo(errorMap, g_errorTitle), getInfo(errorMap, g_errorString));
		PLSCHANNELS_API->clearAllErrors();
	}
	PLSCHANNELS_API->holdOnChannelArea(false);
}

void showChannelsSetting(int index)
{
	HotKeyLocker locker;
	auto dialog = new ChannelsSettingsWidget(getMainWindow());
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setChannelsData(PLSCHANNELS_API->getAllChannelInfo(), index);
	dialog->exec();
}

void showChannelsSetting(const QString &platform)
{
	HotKeyLocker locker;
	auto dialog = new ChannelsSettingsWidget(getMainWindow());
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setChannelsData(PLSCHANNELS_API->getAllChannelInfo(), platform);
	dialog->exec();
}
