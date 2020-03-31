#include "PLSPlatformAfreecaTV.h"
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
#include "../vlive/PLSAPIVLive.h"
#include "ChannelCommonFunctions.h"
#include "PLSAPIAfreecaTV.h"
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

PLSPlatformAfreecaTV::PLSPlatformAfreecaTV()
{
	setSingleChannel(false);

	connect(PLS_PLATFORM_API, &PLSPlatformApi::channelRemoved, this, [=](const QVariantMap &info) {
		QString platformName = info.value(ChannelData::g_channelName, "").toString();
		auto dataType = info.value(ChannelData::g_data_type, ChannelData::RTMPType).toInt();

		if (dataType == ChannelData::ChannelType && platformName == AFREECATV) {
			reInitLiveInfo(true);
		}
	});
}

PLSPlatformAfreecaTV::~PLSPlatformAfreecaTV() {}

PLSServiceType PLSPlatformAfreecaTV::getServiceType() const
{
	return PLSServiceType::ST_AFREECATV;
}

const PLSAfreecaTVLiveinfoData &PLSPlatformAfreecaTV::getSelectData()
{
	return m_selectData;
}

void PLSPlatformAfreecaTV::setSelectData(PLSAfreecaTVLiveinfoData data)
{
	m_selectData = data;
}

void PLSPlatformAfreecaTV::saveSettings(function<void(bool)> onNext, const QString &title)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	auto _onNext = [=](bool isSucceed) {
		if (!isSucceed) {
			onNext(isSucceed);
			return;
		}
		m_selectData.frmTitle = title;
		onNext(isSucceed);
	};
	if (PLS_PLATFORM_API->isPrepareLive()) {
		setIsChatDisabled(false);
	}
	if (title != m_selectData.frmTitle) {
		updateLiveinfo(_onNext, this, title);
	} else {
		_onNext(true);
	}
}

QString PLSPlatformAfreecaTV::getShareUrl(const QString &id, bool isLiveUrl)
{
	if (isLiveUrl) {
		return QString(g_plsAfreecaTVShareUrl_living).arg(id);
	}

	return QString(g_plsAfreecaTVShareUrl_beforeLive).arg(id);
}

void PLSPlatformAfreecaTV::reInitLiveInfo(bool isReset)
{
	if (!isReset) {
		return;
	}
	setSelectData(m_selectData);
};

void PLSPlatformAfreecaTV::onPrepareLive(bool value)
{
	if (!value) {
		prepareLiveCallback(value);
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ "show liveinfo");
	value = pls_exec_live_Info_afreecatv(getChannelUUID(), getInitData()) == QDialog::Accepted;
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ "liveinfo closed with:%d", value);

	setStreamKey(m_selectData.frmStreamKey.toStdString());
	setStreamServer(m_selectData.frmServerUrl.toStdString());
	__super::setTitle(m_selectData.frmTitle.toStdString());
	prepareLiveCallback(value);
}

void PLSPlatformAfreecaTV::requestChannelInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");
		QVariantMap info = srcInfo;
		auto doc = QJsonDocument::fromJson(data);
		if (!doc.isObject()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, doc is not object");
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			finishedCall(QList<QVariantMap>{info});
			return;
		}
		auto root = doc.object();
		QString userID = root["user_id"].toString();
		if (userID.isEmpty()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, userID.isEmpty");
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			finishedCall(QList<QVariantMap>{info});
			return;
		}

		info[ChannelData::g_subChannelId] = userID;
		info[ChannelData::g_userName] = root["nickname"].toString();
		info[ChannelData::g_nickName] = root["nickname"].toString();

		QString iconUrl = root["profile_image"].toString();
		if (!iconUrl.startsWith("http")) {
			iconUrl = "http:" + iconUrl;
		}
		if (iconUrl.contains("?dummy")) {
			iconUrl = iconUrl.split("?dummy").first();
		}
		info[ChannelData::g_userProfileImg] = iconUrl;
		if (auto icon = PLSAPIVLive::downloadImageSync(this, iconUrl, info[ChannelData::g_channelName].toString(), true); icon.first) {
			info[ChannelData::g_userIconCachePath] = icon.second;
		}
		info[ChannelData::g_shareUrl] = getShareUrl(userID, false);
		info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Valid;
		info[ChannelData::g_viewers] = "0";
		info[ChannelData::g_totalViewers] = "0";
		info[ChannelData::g_viewersPix] = ChannelData::g_naverTvViewersIcon;
		//the requestChannelInfo nick name is maybe uncorrect when nickname changed in web page, so call requestUserNickName this method to get nickname again.
		requestUserNickName(info, finishedCall);
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");
		PLSPlatformApiResult result = getApiResult(code, error, data);
		QVariantMap info = srcInfo;
		info[ChannelData::g_channelStatus] = result == PLSPlatformApiResult::PAR_TOKEN_EXPIRED ? ChannelData::ChannelStatus::Expired : ChannelData::ChannelStatus::Error;
		finishedCall(QList<QVariantMap>{info});
	};

	PLSAPIAfreecaTV::requestUsersInfoAndChannel(this, _onSucceed, _onFail);
}

void PLSPlatformAfreecaTV::requestUserNickName(QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");
		QVariantMap info = srcInfo;
		auto doc = QJsonDocument::fromJson(data);
		if (!doc.isObject()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, doc is not object");
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			finishedCall(QList<QVariantMap>{info});
			return;
		}
		auto root = doc.object();
		QString userNickName = root["data"].toObject()["user_nick"].toString();
		if (userNickName.isEmpty()) {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, userNickName isEmpty");
			info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			finishedCall(QList<QVariantMap>{info});
			return;
		}
		info[ChannelData::g_userName] = userNickName;
		info[ChannelData::g_nickName] = userNickName;
		info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Valid;
		finishedCall(QList<QVariantMap>{info});
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");
		PLSPlatformApiResult result = getApiResult(code, error, data);
		QVariantMap info = srcInfo;
		info[ChannelData::g_channelStatus] = result == PLSPlatformApiResult::PAR_TOKEN_EXPIRED ? ChannelData::ChannelStatus::Expired : ChannelData::ChannelStatus::Error;
		finishedCall(QList<QVariantMap>{info});
	};

	PLSAPIAfreecaTV::requestUsersNickName(srcInfo[ChannelData::g_subChannelId].toString(), this, _onSucceed, _onFail);
}

void PLSPlatformAfreecaTV::requestDashborad(function<void(bool)> onNext, QObject *reciever)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");

		PLSAfreecaTVLiveinfoData livinfoData;
		if (data.isEmpty()) {
			setupApiFailedWithCode(PLSPlatformApiResult::PAR_API_FAILED);
			if (nullptr != onNext) {
				onNext(false);
			}
			return;
		}

		PLSAPIAfreecaTV::readDataByRegu(livinfoData, data);

		if (livinfoData.frmTitle.isEmpty()) {
			if (data.startsWith("<script>location.href='https://login.afreecatv.com/afreeca/login.php")) {
				setupApiFailedWithCode(PLSPlatformApiResult::PAR_TOKEN_EXPIRED);
			} else {
				setupApiFailedWithCode(PLSPlatformApiResult::PAR_API_FAILED);
			}

			if (nullptr != onNext) {
				onNext(false);
			}
			return;
		}
		m_selectData = livinfoData;
		requestCategories(onNext, reciever);
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");

		setupApiFailedWithCode(getApiResult(code, error, data));
		if (nullptr != onNext) {
			onNext(false);
		}
	};

	PLSAPIAfreecaTV::requestDashboradData(reciever, _onSucceed, _onFail);
}

void PLSPlatformAfreecaTV::requestCategories(function<void(bool)> onNext, QObject *reciever)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");

		if (data.isEmpty()) {
			setupApiFailedWithCode(PLSPlatformApiResult::PAR_API_FAILED);
			if (nullptr != onNext) {
				onNext(false);
			}
			return;
		}

		PLSAPIAfreecaTV::parseCategory(data);
		m_selectData.frmCategoryStr = PLSAPIAfreecaTV::getSelectCategoryString(m_selectData.frmCategoryID);
		if (nullptr != onNext) {
			onNext(true);
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");

		setupApiFailedWithCode(getApiResult(code, error, data));
		if (nullptr != onNext) {
			onNext(false);
		}
	};

	if (getCategories().empty()) {
		PLSAPIAfreecaTV::requestCategoryList(reciever, _onSucceed, _onFail);
	} else {
		m_selectData.frmCategoryStr = PLSAPIAfreecaTV::getSelectCategoryString(m_selectData.frmCategoryID);
		if (nullptr != onNext) {
			onNext(true);
		}
	}
}

void PLSPlatformAfreecaTV::updateLiveinfo(function<void(bool)> onNext, QObject *reciever, const QString &title)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");

		if (data.isEmpty() || !QJsonDocument::fromJson(data).isObject()) {
			showApiUpdateError(PLSPlatformApiResult::PAR_API_FAILED);
			if (nullptr != onNext) {
				onNext(false);
			}
			return;
		}
		auto doc = QJsonDocument::fromJson(data);
		auto channel = doc["channel"].toObject();
		if (channel["result"].toInt() == -1) {
			if (channel["remsg"].toString() == "You must log in.(406)") {
				showApiUpdateError(PLSPlatformApiResult::PAR_TOKEN_EXPIRED);
			} else {
				showApiUpdateError(PLSPlatformApiResult::PAR_API_FAILED);
			}
			if (nullptr != onNext) {
				onNext(false);
			}
			return;
		}

		if (nullptr != onNext) {
			onNext(true);
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");

		showApiUpdateError(getApiResult(code, error, data));
		if (nullptr != onNext) {
			onNext(false);
		}
	};

	PLSAPIAfreecaTV::updateLiveInfo(reciever, title, _onSucceed, _onFail);
}

PLSPlatformApiResult PLSPlatformAfreecaTV::getApiResult(int code, QNetworkReply::NetworkError error, QByteArray data)
{
	auto result = PLSPlatformApiResult::PAR_SUCCEED;

	if (QNetworkReply::NoError == error) {

	} else if (QNetworkReply::UnknownNetworkError >= error) {
		result = PLSPlatformApiResult::PAR_NETWORK_ERROR;
	} else {
		switch (code) {
		case 401:
		case 515:
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

void PLSPlatformAfreecaTV::setupApiFailedWithCode(PLSPlatformApiResult result)
{
	showApiRefreshError(result);
}

void PLSPlatformAfreecaTV::showApiRefreshError(PLSPlatformApiResult value)
{
	auto alertParent = getAlertParent();

	switch (value) {
	case PLSPlatformApiResult::PAR_NETWORK_ERROR:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Network.Error"));
		break;
	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED: {
		auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
		auto channelName = info.value(ChannelData::g_channelName).toString();
		PLSAlertView::Button button = PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Expired").arg(channelName));
		if (button == PLSAlertView::Button::Ok) {
			emit closeDialogByExpired();
			PLSCHANNELS_API->channelExpired(getChannelUUID(), false);
		}
	} break;
	case PLSPlatformApiResult::PAR_API_ERROR_Live_Invalid:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.scheduled.invalid"));
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_Scheduled_Time:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.scheduled.time.invaild"));
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_StartLive_Other:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.start.other").arg(AFREECATV));
	default:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Failed"));
		break;
	}
}

void PLSPlatformAfreecaTV::showApiUpdateError(PLSPlatformApiResult value)
{
	auto alertParent = getAlertParent();

	switch (value) {
	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED: {
		auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
		auto channelName = info.value(ChannelData::g_channelName).toString();
		PLSAlertView::Button button = PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Expired").arg(channelName));
		if (button == PLSAlertView::Button::Ok) {
			emit closeDialogByExpired();
			PLSCHANNELS_API->channelExpired(getChannelUUID(), false);
		}
	} break;
	default:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.live.error.update.failed"));
		break;
	}
}

void PLSPlatformAfreecaTV::onLiveStopped()
{
	auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	auto user_id = info.value(ChannelData::g_subChannelId).toString();
	PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_shareUrl, getShareUrl(user_id, false));
	liveStoppedCallback();
}

void PLSPlatformAfreecaTV::onAlLiveStarted(bool value)
{
	if (!value || !isActive()) {
		return;
	}

	auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	auto user_id = info.value(ChannelData::g_subChannelId).toString();
	PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_shareUrl, getShareUrl(user_id, true));
}

QJsonObject PLSPlatformAfreecaTV::getLiveStartParams()
{
	QJsonObject platform;
	platform["cookie"] = getChannelCookie();
	const auto &channelData = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
	platform["channelId"] = channelData.value(ChannelData::g_subChannelId, "").toString();
	platform["simulcastChannel"] = channelData.value(ChannelData::g_nickName, "").toString();
	//platform["liveId"] = ""; //[play.afreecatv.com/ian8539/215696088] -> 215696088, but got after pushed
	return platform;
}

QJsonObject PLSPlatformAfreecaTV::getMqttChatParams()
{
	QJsonObject platform;
	platform.insert("cookie", getChannelCookie());
	return platform;
}

QJsonObject PLSPlatformAfreecaTV::getWebChatParams()
{
	auto info = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
	QJsonObject platform;
	platform["name"] = "AFREECATV";
	platform["userId"] = info.value(ChannelData::g_subChannelId, "").toString();
	platform["isPrivate"] = getIsChatDisabled();
	return platform;
}
bool PLSPlatformAfreecaTV::getIsChatDisabled()
{
	QString logStr = QString("%1 m_IsChatDisabled:%2 b_containFrmAccess:%3").arg(__FUNCTION__).arg(BOOL2STR(m_IsChatDisabled)).arg(BOOL2STR(m_selectData.b_containFrmAccess));
	PLS_INFO(MODULE_PlatformService, logStr.toStdString().c_str());
	return m_IsChatDisabled || m_selectData.b_containFrmAccess;
}

QString PLSPlatformAfreecaTV::getServiceLiveLink()
{
	auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	auto shareUrl = info.value(ChannelData::g_shareUrl).toString();
	return shareUrl;
}
