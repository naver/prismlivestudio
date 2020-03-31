#include "PLSPlatformVLive.h"
#include <QObject>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <PLSChannelDataHandler.h>
#include <QDebug>
#include <QTimer>
#include <cassert>
#include "../PLSLiveInfoDialogs.h"
#include "../PLSPlatformApi.h"
#include "ChannelCommonFunctions.h"
#include "PLSAPIVLive.h"
#include "PLSChannelDataAPI.h"
#include "PLSDateFormate.h"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "PLSScheLiveNotice.h"
#include "alert-view.hpp"
#include "frontend-api.h"
#include "log.h"
#include "main-view.hpp"
#include "pls-app.hpp"
#include "pls-net-url.hpp"
#include "window-basic-main.hpp"

const static QString vliveBasic = "BASIC";
const static QString vlivePreminm = "PREMIUM";

PLSPlatformVLive::PLSPlatformVLive() : m_bTempSchedule(false)
{
	setSingleChannel(true);
	m_statusTimer = new QTimer(this);
	connect(m_statusTimer, &QTimer::timeout, this, &PLSPlatformVLive::requestStatisticsInfo);

	connect(PLS_PLATFORM_API, &PLSPlatformApi::channelRemoved, this, [=](const QVariantMap &info) {
		QString platformName = info.value(ChannelData::g_channelName, "").toString();
		auto dataType = info.value(ChannelData::g_data_type, ChannelData::RTMPType).toInt();

		if (dataType == ChannelData::ChannelType && platformName == VLIVE) {
			reInitLiveInfo(true);
		}
	});
	connect(PLS_PLATFORM_API, &PLSPlatformApi::liveEndPageShowComplected, this, [=](bool isRecord) {
		if (!isRecord && !getIsRehearsal() && isActive()) {
			reInitLiveInfo(true);
		}
	});
}

PLSPlatformVLive::~PLSPlatformVLive() {}

PLSServiceType PLSPlatformVLive::getServiceType() const
{
	return PLSServiceType::ST_VLIVE;
}

const QString PLSPlatformVLive::getSelectID()
{
	return m_selectData._id;
}

const vector<PLSVLiveLiveinfoData> &PLSPlatformVLive::getScheduleDatas()
{
	return m_vecSchedules;
}

const PLSVLiveLiveinfoData PLSPlatformVLive::getNomalLiveData()
{
	return m_noramlData;
}

const PLSVLiveLiveinfoData &PLSPlatformVLive::getTempSelectData()
{
	const PLSVLiveLiveinfoData &data = getTempSelectDataRef();
	return data;
}

const PLSVLiveLiveinfoData PLSPlatformVLive::getSelectData()
{
	return m_selectData;
}

void PLSPlatformVLive::setSelectDataByID(const QString &scheID)
{
	for (auto &data : m_vecSchedules) {
		if (data._id == scheID) {
			setSelectData(data);
		}
	}
}

void PLSPlatformVLive::removeExpiredSchedule(const vector<QString> &ids)
{
	QString tempID = getTempSelectData()._id;
	QString selectID = getSelectData()._id;
	for (auto &id : ids) {
		for (int i = 0; i < m_vecSchedules.size(); i++) {
			auto &item = m_vecSchedules[i];
			if (item._id == id && tempID != id && selectID != id) {
				m_vecSchedules.erase(m_vecSchedules.begin() + i);
				break;
			}
		}
	}
}

void PLSPlatformVLive::setSelectData(PLSVLiveLiveinfoData data)
{
	m_bTempSchedule = !data.isNormalLive;
	m_selectData = data;
	setTempSelectID(data._id);
	if (!data.isNormalLive) {
		for (auto &scheduleData : m_vecSchedules) {
			if (scheduleData._id.compare(data._id) == 0) {
				scheduleData = data;
			}
		}
	}
	__super::setTitle(m_selectData.title.toStdString());

	QStringList list;
	QString showChannelName;
	bool isNormalChannelSelect = false;
	for (auto bage : m_selectData.fanshipDatas) {
		if (!bage.isChecked) {
			continue;
		}
		if (showChannelName.isEmpty()) {
			showChannelName = bage.channelName;
		}

		if (bage.isNormalSeq) {

			isNormalChannelSelect = true;
			continue;
		}
		list.append(bage.badgeName);
	}
	if (isNormalChannelSelect) {
		list.clear();
	}
	if (showChannelName.isEmpty()) {
		auto info = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
		showChannelName = info.value(ChannelData::g_vliveNormalChannelName).toString();
	}
	QString bages;
	if (!list.isEmpty()) {
		QString src = QString::fromStdWString({0x00B7}) + QString::fromStdWString({0x00B7}) + QString::fromStdWString({0x00B7});
		bages = list.join(src);
	}

	auto latInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	latInfo.insert(ChannelData::g_catogryTemp, bages);
	latInfo.insert(ChannelData::g_nickName, showChannelName);
	PLSCHANNELS_API->setChannelInfos(latInfo, true);
}

bool PLSPlatformVLive::isModifiedWithNewData(QString title, vector<bool> boxChecks, const QString &localThumFile, PLSVLiveLiveinfoData *uiData)
{
	bool isModified = false;
	PLSVLiveLiveinfoData sData = getTempSelectData();
	if (sData.isNormalLive) {
		sData = getNomalLiveData();
	}

	if (title.compare(sData.title) != 0) {
		isModified = true;
	} else if (sData.isNormalLive) {
		if (localThumFile.compare(sData.thumLocalPath) != 0) {
			isModified = true;
		} else {
			for (int i = 0; i < sData.fanshipDatas.size(); i++) {
				if (boxChecks.size() <= i) {
					break;
				}
				if (sData.fanshipDatas[i].isChecked != boxChecks[i]) {
					isModified = true;
					break;
				}
			}
		}
	}

	if (uiData != NULL) {
		//*uiData = sData;
		uiData->title = title;
		if (uiData->isNormalLive) {
			for (int i = 0; i < boxChecks.size(); i++) {
				uiData->fanshipDatas[i].isChecked = boxChecks[i];
			}
		}

		if (uiData->thumLocalPath.compare(localThumFile, Qt::CaseInsensitive) != 0) {
			uiData->thumLocalPath = localThumFile;
			uiData->thumRemoteUrl = "";
		}
	}

	return isModified;
}

void PLSPlatformVLive::saveSettings(function<void(bool)> onNext, PLSVLiveLiveinfoData uiData)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	auto _onNext = [=](bool isSucceed, const QString &iamgeUrl) {
		if (!isSucceed) {
			onNext(isSucceed);
			return;
		}

		PLSVLiveLiveinfoData savedData = uiData;
		savedData.thumRemoteUrl = iamgeUrl;
		if (!getIsTempSchedule()) {
			m_noramlData = savedData;
		}

		setSelectData(savedData);

		if (!PLS_PLATFORM_API->isPrepareLive()) {
			onNext(isSucceed);
			return;
		}

		auto _onAPINext = [=](bool value) {
			const QString &uuid = getChannelUUID();
			PLSCHANNELS_API->setValueOfChannel(uuid, ChannelData::g_shareUrl, getShareUrl());
			PLSCHANNELS_API->setValueOfChannel(uuid, ChannelData::g_shareUrlTemp, getShareUrl());
			onNext(value);
		};
		requestStartLive(_onAPINext);
	};

	if (!uiData.isNormalLive) {
		requestSchedule([=](bool isOk) { _onNext(isOk, uiData.thumRemoteUrl); }, this, true, uiData._id);
		return;
	}

	if (uiData.thumLocalPath.isEmpty()) {
		_onNext(true, uiData.thumRemoteUrl);
		return;
	}

	if (uiData.thumLocalPath.compare(getTempSelectData().thumLocalPath, Qt::CaseInsensitive) == 0) {
		_onNext(true, uiData.thumRemoteUrl);
		return;
	}
	requestUploadImage(uiData.thumLocalPath, _onNext);
}

void PLSPlatformVLive::setThumLocalFile(const QString &localFile)
{
	getTempSelectDataRef().thumLocalPath = localFile;
}

void PLSPlatformVLive::setThumRemoteUrl(const QString &remoteUrl)
{
	getTempSelectDataRef().thumRemoteUrl = remoteUrl;
}

const QString PLSPlatformVLive::getDefaultTitle()
{
	auto info = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
	auto channelName = info.value(ChannelData::g_vliveNormalChannelName, "").toString();
	return channelName.append(tr("LiveInfo.live.title.suffix"));
}

void PLSPlatformVLive::onShowScheLiveNoticeIfNeeded()
{
	if (m_isHaveShownScheduleNotice) {
		return;
	}
	auto _onNext = [=](bool value) {
		if (!value || getScheduleDatas().size() == 0) {
			return;
		}
		const PLSVLiveLiveinfoData *liveInfo = nullptr;
		long old10Min = PLSDateFormate::getNowTimeStamp() + 60 * 10;
		long nowTime = PLSDateFormate::getNowTimeStamp();
		auto tempDatas = getScheduleDatas();
		auto descendOrder = [&](const PLSVLiveLiveinfoData &left, const PLSVLiveLiveinfoData &right) { return left.startTimeStamp > right.startTimeStamp; };
		sort(tempDatas.begin(), tempDatas.end(), descendOrder);
		for (auto &sli : tempDatas) {
			if (sli.isUpcoming == false) {
				continue;
			}
			if (old10Min > sli.startTimeStamp && sli.startTimeStamp > nowTime) {
				liveInfo = &sli;
				break;
			}
		}

		if (liveInfo) {
			showScheLiveNotice(*liveInfo);
		}
	};
	requestSchedule(_onNext, this, false);
}

QString PLSPlatformVLive::getShareUrl()
{
	return QString(CHANNEL_VLIVE_SHARE).arg(m_selectData.startVideoSeq);
}

void PLSPlatformVLive::liveInfoisShowing()
{
	if (getSelectData().isNormalLive && !getSelectData().title.isEmpty()) {
		m_noramlData = getSelectData();
	} else {
		if (m_noramlData.title.isEmpty()) {
			m_noramlData = PLSVLiveLiveinfoData();
			auto info = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
			auto channelName = info.value(ChannelData::g_vliveNormalChannelName, "").toString();
			m_noramlData.title = channelName.append(tr("LiveInfo.live.title.suffix"));
			m_noramlData.channelSeq = info.value(ChannelData::g_subChannelId, "").toString();
			setFanshipDatas(m_noramlData);
		}
	}
	m_tempNoramlData = m_noramlData;
	setTempSelectID(getSelectData()._id);
	setTempSchedule(!getSelectData().isNormalLive);
}

void PLSPlatformVLive::reInitLiveInfo(bool isReset)
{
	if (!isReset) {
		return;
	}
	isStopedByRemote = false;
	m_isRehearsal = false;
	m_bTempSelectID = "";
	m_bTempSchedule = false;
	m_selectData = PLSVLiveLiveinfoData();
	m_noramlData = PLSVLiveLiveinfoData();
	m_tempNoramlData = PLSVLiveLiveinfoData();
	setSelectData(m_selectData);
};

void PLSPlatformVLive::onPrepareLive(bool value)
{
	if (!value) {
		prepareLiveCallback(value);
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ "show liveinfo");
	value = pls_exec_live_Info_vlive(getChannelUUID(), getInitData()) == QDialog::Accepted;
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ "liveinfo closed with:%d", value);
	if (!value) {
		prepareLiveCallback(value);
		return;
	}

	auto latInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	if (getIsRehearsal()) {
		latInfo.remove(ChannelData::g_viewers);
		latInfo.remove(ChannelData::g_likes);
	} else {
		if (PLSAPIVLive::isVliveFanship()) {
			latInfo.remove(ChannelData::g_viewers);
			latInfo.insert(ChannelData::g_likes, "0");
		} else {
			latInfo.insert(ChannelData::g_viewers, "0");
			latInfo.insert(ChannelData::g_likes, "0");
		}
	}
	PLSCHANNELS_API->setChannelInfos(latInfo, true);

	if (getIsRehearsal()) {
		PLSCHANNELS_API->rehearsalBegin();
	}
	prepareLiveCallback(value);
}

void PLSPlatformVLive::initVliveGcc(function<void()> onNext)
{
	auto _finish = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		getGccData().isLoaded = true;

		PLSPlatformApiResult apiResult = PLSPlatformApiResult::PAR_SUCCEED;
		int responseCode = 0;
		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			responseCode = doc["code"].toInt();
		}
		if (responseCode > 0 && responseCode != 1000) {
			apiResult = PLSPlatformApiResult::PAR_API_FAILED;
		} else if (error != QNetworkReply::NetworkError::NoError) {
			apiResult = PLSPlatformApiResult::PAR_API_FAILED;
		} else if (!doc.isObject()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, doc is not object");
			apiResult = PLSPlatformApiResult::PAR_API_FAILED;
		}

		if (apiResult != PLSPlatformApiResult::PAR_SUCCEED) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed");
			onNext();
			return;
		}

		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");

		auto root = doc.object();

		auto result = root["result"].toObject();
		QString gcc = result["gcc"].toString();

		if (gcc.isEmpty()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, gcc is empty");
			onNext();
			return;
		}
		QString lang = "";
		auto localLists = result["localeLabelList"].toArray();
		for (int i = 0; i < localLists.size(); i++) {
			auto data = localLists[i].toObject();
			if (data["country"].toString().compare(gcc, Qt::CaseInsensitive) == 0) {
				lang = data["language"].toString();
				break;
			}
		}
		if (!gcc.isEmpty() && !lang.isEmpty()) {
			getGccData().gcc = gcc;
			getGccData().lang = lang;
		}
		onNext();
	};

	PLSAPIVLive::vliveRequestGccAndLanguage(this, _finish);
}

void PLSPlatformVLive::requestChannelInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");
		auto doc = QJsonDocument::fromJson(data);

		if (!doc.isObject()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, doc is not object");

			QVariantMap info = srcInfo;
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			finishedCall(QList<QVariantMap>{info});

			return;
		}
		auto root = doc.object();
		if (root["code"].toInt() == 3001) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, code == 3001");
			QVariantMap info = srcInfo;
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			finishedCall(QList<QVariantMap>{info});
			return;
		}

		auto result = root["result"].toObject();
		QString userID = result["id"].toString();

		if (userID.isEmpty()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, userID.isEmpty");
			QVariantMap info = srcInfo;
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			finishedCall(QList<QVariantMap>{info});
			return;
		}

		//TODO nickName is used to user, not channel.
		//auto nickName = result["nickname"].toString();

		QVariantMap info = srcInfo;

		info[ChannelData::g_userID] = QString::number(result["userID"].toInt());        //b7abf7c0-f050-11e7-987d-000000000e4d
		info[ChannelData::g_userVliveSeq] = QString::number(result["userSeq"].toInt()); //3763
		info[ChannelData::g_userVliveCode] = result["userCode"].toString();             //V7C64
		info[ChannelData::g_userName] = result["nickname"].toString();
		info[ChannelData::g_userProfileImg] = result["profileImg"].toString();
		info[ChannelData::g_isFanship] = false;

		auto inAuthChannels = result["inAuthChannel"].toArray();
		for (int i = 0; i < inAuthChannels.size(); i++) {
			auto item = inAuthChannels[i].toObject();
			auto channelPlusType = item["channelPlusType"].toString();
			if (channelPlusType.compare(vliveBasic) == 0) {
				info[ChannelData::g_subChannelId] = QString::number(item["channelSeq"].toInt()); //189
				info[ChannelData::g_nickName] = item["name"].toString();                         //display in channel screa
				info[ChannelData::g_vliveNormalChannelName] = item["name"].toString();
				if (auto icon = PLSAPIVLive::downloadImageSync(this, item["profileImg"].toString(), info[ChannelData::g_channelName].toString()); icon.first) {
					info[ChannelData::g_userIconCachePath] = icon.second;
				}
			} else if (channelPlusType.compare(vlivePreminm) == 0) {
				info[ChannelData::g_isFanship] = true;
				info[ChannelData::g_vliveFanshipModel] = item.toVariantMap();
			}
		}
		info[ChannelData::g_channelStatus] = inAuthChannels.size() == 0 ? ChannelData::ChannelStatus::EmptyChannel : ChannelData::ChannelStatus::Valid;

		info[ChannelData::g_viewers] = "0";
		info[ChannelData::g_viewersPix] = ChannelData::g_naverTvViewersIcon;
		info[ChannelData::g_likes] = "0";
		info[ChannelData::g_likesPix] = ChannelData::g_naverTvLikeIcon;
		info[ChannelData::g_catogryTemp] = QString();
		info[ChannelData::g_shareUrl] = QString();
		info[ChannelData::g_shareUrlTemp] = QString();
		finishedCall(QList<QVariantMap>{info});
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");
		PLSPlatformApiResult result = getApiResult(code, error, data);
		QVariantMap info = srcInfo;

		switch (result) {
		case PLSPlatformApiResult::PAR_NETWORK_ERROR:
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Error;
			info[ChannelData::g_errorType] = ChannelData::NetWorkErrorType::NetWorkNoStable;
			break;
		case PLSPlatformApiResult::PAR_TOKEN_EXPIRED:
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Expired;
			break;
		default:
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Error;
			info[ChannelData::g_errorType] = ChannelData::NetWorkErrorType::UnknownError;
			break;
		}
		finishedCall(QList<QVariantMap>{info});
	};

	PLSAPIVLive::vliveRequestUsersInfoAndChannel(this, _onSucceed, _onFail);
}

void PLSPlatformVLive::requestSchedule(function<void(bool)> onNext, QObject *widget, bool isNeedShowErrAlert, const QString &checkID)
{
	static int m_iContext = 0;
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		if (context != reinterpret_cast<void *>(m_iContext)) {
			return;
		}

		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");
		auto doc = QJsonDocument::fromJson(data);

		if (!doc.isObject()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, doc is not object");
			if (isNeedShowErrAlert) {
				setupApiFailedWithCode(PLSPlatformApiResult::PAR_API_FAILED);
			}
			if (nullptr != onNext) {
				onNext(false);
			}
			return;
		}
		auto root = doc.object();
		if (root["code"].toInt() != 1000) {
			if (isNeedShowErrAlert) {
				setupApiFailedWithCode(PLSPlatformApiResult::PAR_API_FAILED);
			}
			if (nullptr != onNext) {
				onNext(false);
			}
			return;
		}

		auto items = root["result"].toObject()["liveList"].toArray();
		PLSVLiveLiveinfoData selectData = getTempSelectData();
		bool isContainSelectData = false;
		bool isContainCheckedItem = false;
		m_vecSchedules.clear();

		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed with itemCount:%i", items.count());
		for (int i = 0; i < items.size(); i++) {
			auto data = items[i].toObject();
			bool upcomingYn = data["upcomingYn"].toBool();
			auto status = data["status"].toString();
			if ((upcomingYn && status.compare("EXPOSE") != 0)) {
				continue;
			}
			PLSVLiveLiveinfoData scheduleDta = PLSVLiveLiveinfoData(data);
			if (scheduleDta.isUpcoming) {
				long ago10Min = PLSDateFormate::getNowTimeStamp() - 10 * 60;
				long early3Day = PLSDateFormate::getNowTimeStamp() + 3 * 60 * 60 * 24;
				if (scheduleDta.startTimeStamp < ago10Min || early3Day < scheduleDta.startTimeStamp) {
					continue;
				}
			}
			auto scheShips = data["fanshipBadges"].toArray();
			if (m_noramlData.fanshipDatas.size() == 0) {
				setFanshipDatas(m_noramlData);
			}
			if (scheShips.isEmpty()) {
				for (auto ship : m_noramlData.fanshipDatas) {
					ship.isChecked = true;
					ship.uiEnabled = false;
					scheduleDta.fanshipDatas.push_back(ship);
				}
			} else {
				for (auto ship : m_noramlData.fanshipDatas) {
					ship.isChecked = false;
					ship.uiEnabled = false;
					for (size_t i = 0; i < scheShips.size(); i++) {
						auto scheShip = scheShips[i].toString();
						if (scheShip == ship.badgeName) {
							ship.isChecked = true;
							break;
						}
					}
					scheduleDta.fanshipDatas.push_back(ship);
				}
			}

			if (scheduleDta._id == selectData._id) {
				isContainSelectData = true;
			}
			if (scheduleDta._id == checkID) {
				isContainCheckedItem = true;
			}
			m_vecSchedules.push_back(scheduleDta);
		}

		if (checkID.isEmpty()) {
			isContainCheckedItem = true;
		}
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed with itemCount:%i", items.count());
		if (isContainSelectData == false && !selectData.isNormalLive) {
			//if the remote not found selected schedule, so add it.
			m_vecSchedules.push_back(selectData);
		}

		if (!isContainCheckedItem) {
			setupApiFailedWithCode(PLSPlatformApiResult::PAR_API_ERROR_LIVE_BROADCAST_NOT_FOUND);
		}
		if (nullptr != onNext) {
			onNext(!isContainCheckedItem ? false : true);
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");
		if (isNeedShowErrAlert) {
			setupApiFailedWithCode(getApiResult(code, error, data));
		}

		if (nullptr != onNext) {
			onNext(false);
		}
	};
	PLSAPIVLive::vliveRequestSchedule(widget, _onSucceed, _onFail, reinterpret_cast<void *>(++m_iContext));
}

void PLSPlatformVLive::downloadThumImage(function<void()> onNext, QWidget *reciver)
{
	static int m_iContext = 0;
	auto _callBack = [=](bool ok, const QString &imagePath, void *const context) {
		if (context != reinterpret_cast<void *>(m_iContext)) {
			return;
		}
		if (ok) {
			setThumLocalFile(imagePath);
			onNext();
			//emit onThumImageDownloaded();
		}
	};
	PLSAPIVLive::downloadImageAsync(reciver, getTempSelectData().thumRemoteUrl, _callBack, reinterpret_cast<void *>(++m_iContext));
}

void PLSPlatformVLive::requestUploadImage(const QString &localFile, function<void(bool, const QString &)> onNext)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	PLSAPIVLive::uploadImage(this, localFile, [=](bool ok, const QString &imageUrl) {
		//if (ok) {
		//setThumRemoteUrl(imageUrl);
		/*	remoteUrl = QString(imageUrl);
			remoteUrl = "fdfdf";*/
		//}
		if (!ok) {
			showApiRefreshError(PLSPlatformApiResult::PAR_API_ERROR_Upload_Image);
		}
		if (nullptr != onNext) {
			onNext(ok, imageUrl);
		}
	});
}

void PLSPlatformVLive::requestStatisticsInfo()
{
	auto _finish = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		int responseCode = 0;

		auto doc = QJsonDocument::fromJson(data);
		bool isLiveEnd = false;
		QString errorAlert;

		if (doc.isObject()) {
			responseCode = doc["code"].toInt();
			isLiveEnd = doc.object()["result"].toObject()["status"].toString().compare("END") == 0;
		}
		if (isLiveEnd || responseCode == 9100 || responseCode == 9102 || responseCode == 9115) {
			errorAlert = tr("LiveInfo.live.error.stoped.byRemote").arg(getNameForChannelType());
		}

		if (!errorAlert.isEmpty()) {
			if (isStopedByRemote) {
				return;
			}
			isStopedByRemote = true;
			PLS_INFO(MODULE_PlatformService, __FUNCTION__ " stop by remote errorCode, errorCode:%d, responseCode:%d", error, responseCode);

			if (LiveStatus::LiveStarted <= PLS_PLATFORM_API->getLiveStatus() && PLS_PLATFORM_API->getLiveStatus() < LiveStatus::LiveStoped) {
				PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": toStopBroadcast");
				PLSCHANNELS_API->toStopBroadcast();
			}
			PLSAlertView::warning(getAlertParent(), QTStr("Live.Check.Alert.Title"), errorAlert);
			return;
		}

		if (errorAlert.isEmpty() && (error != QNetworkReply::NetworkError::NoError || responseCode != 1000)) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, errorCode:%d, responseCode:%d", error, responseCode);
			return;
		}

		auto root = doc.object();
		auto likeCount = QString::number(root["result"].toObject()["likeCount"].toInt());
		auto watchCount = QString::number(root["result"].toObject()["watchingCount"].toInt());
		auto commentCount = QString::number(root["result"].toObject()["commentCount"].toInt());

		auto mSourceData = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
		if (mSourceData.contains(ChannelData::g_viewers)) {
			PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_viewers, watchCount);
		}
		if (mSourceData.contains(ChannelData::g_likes)) {
			PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_likes, likeCount);
		}
		PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_comments, commentCount);
	};
	PLSAPIVLive::vliveRequestStatistics(this, _finish);
}

void PLSPlatformVLive::requestStartLive(function<void(bool)> onNext)
{
	auto _finish = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLSPlatformApiResult apiResult = PLSPlatformApiResult::PAR_SUCCEED;
		int responseCode = 0;

		/*
		9105	방송 시작 권한 관련 오류. 지정한 채널로 방송 시작할수 있는 권한이 없는 사용자가 방송 요청   启动与广播权限相关的错误。作为非特权用户开始将请求广播广播的指定通道
         9106	방송 시작 시간 오류. 예약 되지 않은 시간에 방송 시작 요청   错误的广播开始时间。请求时启动广播
         9109	유효하지 않은 장비에서 방송 시작 요청. 폰이 아닌 별도 장비로 방송하겠다고 했는데, 폰에서 방송 요청이 들어온 경우 启动无效的广播设备请求。在一个单独的广播设备是一个典当的要求, 如果你带来的丰
         9115 invaild viedoSeq
         */
		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			responseCode = doc["code"].toInt();
		}
		if (responseCode == 9105) {
			apiResult = PLSPlatformApiResult::VLIVE_API_ERROR_NO_PERMISSION;
		} else if (responseCode == 9115) {
			apiResult = PLSPlatformApiResult::PAR_API_ERROR_Live_Invalid;
		} else if (responseCode == 9106) {
			apiResult = PLSPlatformApiResult::PAR_API_ERROR_Scheduled_Time;
		} else if (error != QNetworkReply::NetworkError::NoError) {
			apiResult = getApiResult(code, error, data);
			apiResult = apiResult == PLSPlatformApiResult::PAR_TOKEN_EXPIRED ? PLSPlatformApiResult::PAR_TOKEN_EXPIRED : PLSPlatformApiResult::PAR_API_ERROR_StartLive_Other;
		} else if (responseCode > 0 && responseCode != 1000) {
			apiResult = PLSPlatformApiResult::PAR_API_ERROR_StartLive_Other;
		}

		QString pushUrl;
		if (apiResult == PLSPlatformApiResult::PAR_SUCCEED && doc.isObject()) {
			pushUrl = doc["result"].toObject()["publishUrl"].toString();
			if (pushUrl.isEmpty()) {
				PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, pushUrl is empty");
				apiResult = PLSPlatformApiResult::PAR_API_ERROR_StartLive_Other;
			}
		}

		if (apiResult != PLSPlatformApiResult::PAR_SUCCEED) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, responseCode:%d", responseCode);
			showApiRefreshError(apiResult);
			if (nullptr != onNext) {
				onNext(false);
				return;
			}
		}

		auto result = doc["result"].toObject();

		QString videoSeq;
		if (getIsRehearsal()) {
			videoSeq = QString::number(result["rehearsalSeq"].toInt());
		} else {
			videoSeq = QString::number(result["videoSeq"].toInt());
		}

		m_selectData.startVideoSeq = videoSeq;

		QStringList list = pushUrl.split("/");
		QString streamKey = list.last();
		list.removeLast();
		QString streamUrl = list.join("/");

		m_selectData.streamKey = streamKey;
		m_selectData.streamUrl = streamUrl;

		setStreamKey(m_selectData.streamKey.toStdString());
		setStreamServer(m_selectData.streamUrl.toStdString());
		setIsSubChannelStartApiCall(true);
		if (nullptr != onNext) {
			onNext(true);
		}
	};

	PLSAPIVLive::vliveRequestStartLive(this, _finish);
}

void PLSPlatformVLive::requestStopLive(function<void(void)> onNext)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		if (nullptr != onNext) {
			onNext();
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		Q_UNUSED(error)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");
		if (nullptr != onNext) {
			onNext();
		}
	};
	setIsSubChannelStartApiCall(false);
	PLSAPIVLive::vliveRequestStopLive(this, _onSucceed, _onFail);
}

PLSPlatformApiResult PLSPlatformVLive::getApiResult(int code, QNetworkReply::NetworkError error, QByteArray data)
{
	auto result = PLSPlatformApiResult::PAR_SUCCEED;

	if (QNetworkReply::NoError == error) {

	} else if (QNetworkReply::UnknownNetworkError >= error) {
		result = PLSPlatformApiResult::PAR_NETWORK_ERROR;
	} else {
		switch (code) {
		case 401:
			result = PLSPlatformApiResult::PAR_TOKEN_EXPIRED;
			break;
		case 403:
			result = PLSPlatformApiResult::PAR_API_FAILED;
			break;
		default:
			result = PLSPlatformApiResult::PAR_API_FAILED;
			break;
		}
	}
	return result;
}

void PLSPlatformVLive::setupApiFailedWithCode(PLSPlatformApiResult result)
{
	showApiRefreshError(result);
}

void PLSPlatformVLive::showApiRefreshError(PLSPlatformApiResult value)
{

	auto alertParent = getAlertParent();

	switch (value) {
	case PLSPlatformApiResult::PAR_NETWORK_ERROR:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Network.Error"));
		break;
	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED: {
		emit toShowLoading(true);
		auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
		auto channelName = info.value(ChannelData::g_channelName).toString();
		PLSAlertView::Button button = PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Expired").arg(channelName));
		emit closeDialogByExpired();
		if (button == PLSAlertView::Button::Ok) {
			PLSCHANNELS_API->channelExpired(getChannelUUID(), false);
		}
		emit toShowLoading(false);
	} break;
	case PLSPlatformApiResult::VLIVE_API_ERROR_NO_PERMISSION:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.scheduled.vlive.9105"));

		break;
	case PLSPlatformApiResult::PAR_API_ERROR_Live_Invalid:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.scheduled.invalid"));
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_Scheduled_Time:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.scheduled.time.invaild"));
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_Upload_Image:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.set_photo_error"));
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_StartLive_Other:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.start.other").arg(VLIVE));
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_LIVE_BROADCAST_NOT_FOUND:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Broadcast.Error.Delete"));
		break;
	default:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Failed"));
		break;
	}
}

void PLSPlatformVLive::showApiUpdateError(PLSPlatformApiResult value)
{
	auto alertParent = getAlertParent();
	auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	auto channelName = info.value(ChannelData::g_channelName).toString();

	switch (value) {
	case PLSPlatformApiResult::PAR_NETWORK_ERROR:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Update.Network.Error"));
		break;
	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED: {
		emit toShowLoading(true);
		PLSAlertView::Button button = PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Expired").arg(channelName));
		emit closeDialogByExpired();
		if (button == PLSAlertView::Button::Ok) {
			PLSCHANNELS_API->channelExpired(getChannelUUID(), false);
		}
		emit toShowLoading(false);
	} break;
	default:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Update.Error.Failed").arg(channelName));
		break;
	}
}

void PLSPlatformVLive::onActive()
{
	PLSPlatformBase::onActive();
	QMetaObject::invokeMethod(this, &PLSPlatformVLive::onShowScheLiveNoticeIfNeeded, Qt::QueuedConnection);
}

void PLSPlatformVLive::onLiveStopped()
{
	if (m_statusTimer && m_statusTimer->isActive()) {
		m_statusTimer->stop();
	}

	if (PLS_PLATFORM_API->isPrismLive() || !isActive()) {
		liveStoppedCallback();
		return;
	}

	auto _onNext = [=]() {
		PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_shareUrl, QString());
		PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_shareUrlTemp, QString());
		liveStoppedCallback();
	};

	requestStopLive(_onNext);
}

void PLSPlatformVLive::onAlLiveStarted(bool value)
{
	isStopedByRemote = false;
	if (!value || PLS_PLATFORM_API->isPrismLive() || !isActive()) {
		return;
	}
	if (getIsRehearsal()) {
		return;
	}

	if (m_statusTimer && !m_statusTimer->isActive()) {
		m_statusTimer->start(5000);
	}
}

QJsonObject PLSPlatformVLive::getWebChatParams()
{
	const static QString s_strTicket = "globalv";
	const static QString s_templateId = "default";
	const static QString s_pool = "cbox4";
	const static QString s_strNEOIDConsumerKey = "EvbbxIFhoa3Z2SkNwe0K";

	auto info = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
	QString allToken = PLS_PLATFORM_VLIVE->getChannelToken();

	QJsonObject platform;
	platform["name"] = "VLIVE";
	platform["objectId"] = getSelectData().startVideoSeq;
	platform["gcc"] = getGccData().gcc;
	platform["lang"] = getGccData().lang;
	platform["ticket"] = s_strTicket;
	platform["templateId"] = s_templateId;
	platform["objectUrl"] = g_plsVliveObjectUrl;
	auto snsCdList = allToken.split(";snsCd=");
	if (snsCdList.size() > 1) {
		platform["snsCode"] = snsCdList[1];
	}
	platform["pool"] = s_pool;
	platform["username"] = info.value(ChannelData::g_userName, "").toString();
	platform["userProfileImage"] = info.value(ChannelData::g_userProfileImg, "").toString();
	platform["extension"] = QJsonObject({{"no", info.value(ChannelData::g_userVliveSeq, "").toString()}, {"cno", getSelectData().channelSeq.toInt()}});
	platform["isVliveFanship"] = !PLSAPIVLive::isVliveFanship(); //when show chat fanship is true, not show fanshiop icon is false.
	platform["version"] = 1;
	platform["GLOBAL_URI"] = pls_get_gpop_connection().ssl;
	platform["HMAC_SALT"] = PLS_VLIVE_HMAC_KEY;
	platform["consumerKey"] = s_strNEOIDConsumerKey;
	platform["isRehearsal"] = getIsRehearsal();
	auto neoAll = allToken.split(";snsToken=").first();
	auto neoStringList = neoAll.split("NEO_SES=");
	if (neoStringList.size() > 1) {
		platform["Authorization"] = QString("Bearer %1").arg(neoStringList[1]).remove("\"");
	}
	return platform;
}

PLSVLiveLiveinfoData &PLSPlatformVLive::getTempSelectDataRef()
{
	if (!m_bTempSchedule) {
		return m_tempNoramlData;
	}
	for (auto &scheuleData : m_vecSchedules) {
		if (0 == getTempSelectID().compare(scheuleData._id)) {
			return scheuleData;
			break;
		}
	}
	return m_tempNoramlData;
}

void PLSPlatformVLive::showScheLiveNotice(const PLSVLiveLiveinfoData &liveInfo)
{
	if (m_isHaveShownScheduleNotice) {
		return;
	}
	m_isHaveShownScheduleNotice = true;
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	PLSScheLiveNotice scheLiveNotice(this, liveInfo.title, liveInfo.startTimeUTC, App()->getMainView());

	if (scheLiveNotice.exec() != PLSScheLiveNotice::Accepted) {
		return;
	}
}

PLSVLiveLiveinfoData::PLSVLiveLiveinfoData() {}

PLSVLiveLiveinfoData::PLSVLiveLiveinfoData(const QJsonObject &data)
{

	this->isPlus = data["channelPlusPublicYn"].toBool();
	this->_id = QString::number(data["videoSeq"].toInt());
	this->title = data["title"].toString();
	this->channelSeq = QString::number(data["channelSeq"].toInt());
	this->thumRemoteUrl = data["thumb"].toString();
	this->isUpcoming = data["upcomingYn"].toBool();
	this->isNormalLive = false;
	this->startTimeOrigin = data["onAirAt"].toString();
	this->endTimeOrigin = data["willEndAt"].toString();

	if (this->startTimeOrigin.isEmpty()) {
		this->startTimeOrigin = PLSDateFormate::vliveTimeStampToString(PLSDateFormate::getNowTimeStamp());
	}
	if (this->endTimeOrigin.isEmpty()) {
		this->startTimeOrigin = PLSDateFormate::vliveTimeStampToString(PLSDateFormate::getNowTimeStamp());
	}

	this->startTimeStamp = PLSDateFormate::vliveTimeStringToStamp(startTimeOrigin);
	this->startTimeShort = PLSDateFormate::timeStampToShortString(this->startTimeStamp);
	this->startTimeUTC = PLSDateFormate::timeStampToUTCString(this->startTimeStamp);
	this->endTimeStamp = PLSDateFormate::vliveTimeStringToStamp(endTimeOrigin);
}

bool sortAscending(const PLSVLiveFanshipData &v1, const PLSVLiveFanshipData &v2)
{
	return v1.shipSeq.toInt() < v2.shipSeq.toInt();
}

void PLSPlatformVLive::setFanshipDatas(PLSVLiveLiveinfoData &infoData)
{
	auto info = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());

	auto fanshipMap = info.value(ChannelData::g_vliveFanshipModel).toMap();
	auto fanship = QJsonObject(QJsonDocument::fromJson(QJsonDocument::fromVariant(QVariant(fanshipMap)).toJson()).object());
	auto shipLists = fanship["fanshipPackageList"].toArray();

	infoData.fanshipDatas.clear();

	PLSVLiveFanshipData basicData;
	basicData.isNormalSeq = true;
	basicData.badgeName = "";
	basicData.channelSeq = info.value(ChannelData::g_subChannelId).toString();
	basicData.channelName = info.value(ChannelData::g_vliveNormalChannelName).toString();
	basicData.shipSeq = "";
	basicData.isChecked = shipLists.size() == 0;
	infoData.fanshipDatas.push_back(basicData);

	if (shipLists.size() == 0) {
		return;
	}

	for (int i = 0; i < shipLists.size(); i++) {
		auto item = shipLists[i].toObject();
		PLSVLiveFanshipData shipData;
		shipData.isNormalSeq = false;
		shipData.badgeName = item["badge"].toString();
		shipData.channelSeq = QString::number(fanship["channelSeq"].toInt());
		shipData.channelName = fanship["name"].toString();
		shipData.shipSeq = QString::number(item["fanshipBundleSeq"].toInt());
		infoData.fanshipDatas.push_back(shipData);
	}
	sort(infoData.fanshipDatas.begin(), infoData.fanshipDatas.end(), sortAscending);
}
QString PLSPlatformVLive::getServiceLiveLink()
{
	return getShareUrl();
}
