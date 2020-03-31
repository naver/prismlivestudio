#include "PLSPlatformBand.h"

#include <QUrl>

#include "PLSChannelDataHandlerFunctions.h"
#include "json-data-handler.hpp"
#include "pls-common-define.hpp"
#include "../PLSLiveInfoDialogs.h"
#include "../PLSHttpApi/PLSHmacNetworkReplyBuilder.h"
#include "../PLSHttpApi/PLSHttpHelper.h"
#include "../channels/ChannelsDataApi/PLSChannelDataAPI.h"
#include "../channels/ChannelsDataApi/ChannelCommonFunctions.h"
#include "../channels/ChannelsDataApi/ChannelConst.h"
#include "../PLSPlatformApi/PLSPlatformApi.h"
#include "PLSLiveInfoBand.h"
#include "PLSBandDataHandler.h"

#define EXPIRE_OFFSET(X) (((X) - (30 * 1000 * 60)) / 1000) //half hour(s)

PLSPlatformBand::PLSPlatformBand()
{
	m_isRequestStart = false;
}

PLSPlatformBand::~PLSPlatformBand() {}

void PLSPlatformBand::initLiveInfo(bool isUpdate)
{
	if (isUpdate) {
		for (auto bandPlatform : PLS_PLATFORM_API->getPlatformsByType(PLSServiceType::ST_BAND)) {
			bandPlatform->setTitle("");
		}
	}
}

void PLSPlatformBand::clearBandInfos()
{
	m_bandInfos.clear();
}
void PLSPlatformBand::getBandRefreshTokenInfo(refreshTokenCallback callbackfunc, bool isFromServer)
{

	auto channelInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());

	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		QVariant token;
		QVariant refreshToken;
		QVariant expire_in;
		PLSJsonDataHandler::getValueFromByteArray(data, COOKIE_ACCESS_TOKEN, token);
		PLSJsonDataHandler::getValueFromByteArray(data, COOKIE_REFRESH_TOKEN, refreshToken);
		PLSJsonDataHandler::getValueFromByteArray(data, COOKIE_EXPIRES_IN, expire_in);

		if (token.isValid() && code == HTTP_STATUS_CODE_200) {
			auto bandInfos = PLSCHANNELS_API->getChanelInfosByPlatformName(BAND, ChannelData::ChannelType);

			m_bandLoginInfo.insert(ChannelData::g_channelToken, token);
			m_bandLoginInfo.insert(ChannelData::g_refreshToken, refreshToken);
			m_bandLoginInfo.insert(ChannelData::g_expires_in, EXPIRE_OFFSET(expire_in.toLongLong()));
			m_bandLoginInfo.insert(ChannelData::g_createTime, QDateTime::currentDateTime());

			for (auto band : bandInfos) {
				QString bandUUID = band.value(ChannelData::g_channelUUID).toString();
				PLSCHANNELS_API->setValueOfChannel(bandUUID, ChannelData::g_channelToken, token);
				PLSCHANNELS_API->setValueOfChannel(bandUUID, ChannelData::g_refreshToken, refreshToken);
				PLSCHANNELS_API->setValueOfChannel(bandUUID, ChannelData::g_createTime, QDateTime::currentDateTime());
				PLSCHANNELS_API->setValueOfChannel(bandUUID, ChannelData::g_expires_in, EXPIRE_OFFSET(expire_in.toLongLong()));
			}
		}
		if (callbackfunc) {
			callbackfunc(true);
		}
		PLS_INFO(MODULE_PLATFORM_BAND, __FUNCTION__ "refresh band token success");
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__ ".refresh band token failed error: %d-%d", code, error);
		if (callbackfunc) {
			callbackfunc(false);
		}
	};

	if (!isTokenValid(channelInfo) || isFromServer) {
		QString refreshToken = channelInfo.value(ChannelData::g_refreshToken).toString();
		QString authUrlStr = QString(CHANNEL_BAND_REFRESH_TOKEN).arg(refreshToken);

		QVariantMap headMap;
		QString codeBase64 = QString("%1:%2").arg(CHANNEL_BAND_ID).arg(CHANNEL_BAND_SECRET).toUtf8().toBase64();
		QString authHeadValue = QString("Basic %1").arg(codeBase64);
		headMap.insert("Authorization", authHeadValue);

		PLSNetworkReplyBuilder builder(authUrlStr);
		builder.setRawHeaders(headMap).setContentType("application/json;charset=UTF-8");

		PLSHttpHelper::connectFinished(builder.get(), this, _onSucceed, _onFail);
	} else {
		if (callbackfunc) {
			callbackfunc(false);
		}
	}
}
void PLSPlatformBand::getBandTokenInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	m_bandLoginInfo = srcInfo;

	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		QVariant token;
		QVariant refreshToken;
		QVariant expire_in;
		PLSJsonDataHandler::getValueFromByteArray(data, COOKIE_ACCESS_TOKEN, token);
		PLSJsonDataHandler::getValueFromByteArray(data, COOKIE_REFRESH_TOKEN, refreshToken);
		PLSJsonDataHandler::getValueFromByteArray(data, COOKIE_EXPIRES_IN, expire_in);

		if (token.isValid() && code == HTTP_STATUS_CODE_200) {
			m_bandLoginInfo.insert(ChannelData::g_channelToken, token);
			m_bandLoginInfo.insert(ChannelData::g_refreshToken, refreshToken);
			m_bandLoginInfo.insert(ChannelData::g_expires_in, EXPIRE_OFFSET(expire_in.toLongLong()));
			m_bandLoginInfo.insert(ChannelData::g_createTime, QDateTime::currentDateTime());
			m_bandLoginInfo[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Valid;
			getBandCategoryInfo(m_bandLoginInfo, finishedCall);

		} else {
			m_bandLoginInfo[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::UnInitialized;
			m_bandInfos.append(m_bandLoginInfo);
			finishedCall(m_bandInfos);
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__ ".error: %d-%d", code, error);
		showChannelInfoError(getApiResult(code, error));

		m_bandInfos.append(m_bandLoginInfo);
		finishedCall(m_bandInfos);
	};

	QString code = m_bandLoginInfo.value(ChannelData::g_channelCode).toString();
	QString authUrlStr = QString(CHANNEL_BAND_AUTH).arg(code);

	QVariantMap headMap;
	QString codeBase64 = QString("%1:%2").arg(CHANNEL_BAND_ID).arg(CHANNEL_BAND_SECRET).toUtf8().toBase64();
	QString authHeadValue = QString("Basic %1").arg(codeBase64);
	headMap.insert("Authorization", authHeadValue);

	PLSNetworkReplyBuilder builder(authUrlStr);
	builder.setRawHeaders(headMap).setContentType("application/json;charset=UTF-8");

	PLSHttpHelper::connectFinished(builder.get(), this, _onSucceed, _onFail);
}

void PLSPlatformBand::getBandCategoryInfo(const QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	m_bandLoginInfo = srcInfo;

	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		QVariantList bands;
		PLSJsonDataHandler::getValuesFromByteArray(data, "bands", bands);
		if (!bands.isEmpty()) {
			for (auto band : bands) {
				QVariantMap tmpband;
				tmpband[ChannelData::g_channelToken] = m_bandLoginInfo[ChannelData::g_channelToken];
				tmpband[ChannelData::g_channelCode] = m_bandLoginInfo[ChannelData::g_channelCode];
				tmpband[ChannelData::g_channelName] = m_bandLoginInfo[ChannelData::g_channelName];
				tmpband[ChannelData::g_expires_in] = m_bandLoginInfo[ChannelData::g_expires_in];
				tmpband[ChannelData::g_createTime] = m_bandLoginInfo[ChannelData::g_createTime];
				tmpband[ChannelData::g_refreshToken] = m_bandLoginInfo[ChannelData::g_refreshToken];

				tmpband[ChannelData::g_nickName] = band.toMap()["name"].toString();
				tmpband[ChannelData::g_subChannelId] = band.toMap()["band_key"].toString();
				if (auto icon = getChannelEmblemSync(band.toMap()["cover"].toString()); icon.first) {
					tmpband[ChannelData::g_userIconCachePath] = icon.second;
				}
				tmpband[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Valid;

				m_bandInfos.append(tmpband);
			}

		} else {
			m_bandLoginInfo[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::EmptyChannel;
			m_bandInfos.append(m_bandLoginInfo);
		}
		finishedCall(m_bandInfos);
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__ ".error: %d-%d", code, error);
		QVariant resultCode;
		PLSJsonDataHandler::getValueFromByteArray(data, "result_code", resultCode);
		showChannelInfoError(getApiResult(code, error));

		m_bandInfos.append(m_bandLoginInfo);
		finishedCall(m_bandInfos);
	};

	auto callbackFun = [this, _onSucceed, _onFail](bool) {
		QString code = m_bandLoginInfo.value(ChannelData::g_channelCode).toString();
		QString authUrlStr = QString(CHANNEL_BAND_AUTH).arg(code);

		PLSNetworkReplyBuilder builder(CHANNEL_BAND_CATEGORY);
		builder.setContentType("application/json;charset=UTF-8");
		QVariantMap fieldMaps;
		builder.addQuery("filter", "create_live").addQuery(COOKIE_ACCESS_TOKEN, m_bandLoginInfo[ChannelData::g_channelToken]);

		PLSHttpHelper::connectFinished(builder.get(), this, _onSucceed, _onFail);
	};

	//refresh token
	getBandRefreshTokenInfo(callbackFun);
}

QPair<bool, QString> PLSPlatformBand::getChannelEmblemSync(const QString &url)
{
	PLSNetworkReplyBuilder builder(url);
	return PLSHttpHelper::downloadImageSync(builder.get(), this, getChannelCacheDir(), BAND);
}

PLSServiceType PLSPlatformBand::getServiceType() const
{
	return PLSServiceType::ST_BAND;
}

void PLSPlatformBand::onPrepareLive(bool value)
{
	if (!value) {
		PLSPlatformBase::onPrepareLive(value);
		return;
	}
	value = pls_exec_live_Info_band(getChannelUUID(), getInitData()) == QDialog::Accepted;

	PLSPlatformBase::onPrepareLive(value);
}

QString PLSPlatformBand::getShareUrl()
{
	return QString();
}

void PLSPlatformBand::onAlLiveStarted(bool value)
{

	PLSPlatformBase::onAlLiveStarted(value);
}

void PLSPlatformBand::onAllPrepareLive(bool isOk)
{
	if (!isOk && m_isRequestStart) {
		requesetLiveStop([this] { PLSPlatformBase::onAllPrepareLive(false); });
		return;
	}
	PLSPlatformBase::onAllPrepareLive(isOk);
}

void PLSPlatformBand::onLiveStopped()
{
	m_isRequestStart = false;

	requesetLiveStop([this] { liveStoppedCallback(); });
	setTitle(std::string());
}

PLSPlatformApiResult PLSPlatformBand::getApiResult(int code, QNetworkReply::NetworkError error)
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
			result = PLSPlatformApiResult::PAR__API_ERROR_FORBIDDEN;
			break;
		case 60106:
			result = PLSPlatformApiResult::BAND_API_ERROR_NO_PERMISSION;
			break;
		default:
			result = PLSPlatformApiResult::PAR_API_FAILED;
			break;
		}
	}

	return result;
}

void PLSPlatformBand::showApiRefreshError(PLSPlatformApiResult value)
{
	auto alertParent = getAlertParent();

	switch (value) {
	case PLSPlatformApiResult::PAR_NETWORK_ERROR:
		PLSAlertView::warning(nullptr, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Network.Error"));
		break;
	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED:
	case PLSPlatformApiResult::PAR__API_ERROR_FORBIDDEN:
		//PLSAlertView::warning(nullptr, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Band.Expired"));
		break;
	case PLSPlatformApiResult::BAND_API_ERROR_NO_PERMISSION:
		break;
	default:
		PLSAlertView::warning(nullptr, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Band.Failed"));
		break;
	}
}
void PLSPlatformBand::showChannelInfoError(PLSPlatformApiResult value)
{
	switch (value) {
	case PLSPlatformApiResult::PAR_SUCCEED:
		break;
	case PLSPlatformApiResult::PAR_NETWORK_ERROR:
		//network error
		m_bandLoginInfo[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Error;
		break;

	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED:
	case PLSPlatformApiResult::PAR__API_ERROR_FORBIDDEN:
		m_bandLoginInfo[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Expired;
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_Live_Invalid:
		break;
	case PLSPlatformApiResult::PAR_API_ERROR_Scheduled_Time:
		break;
	default:
		break;
	}
}
void PLSPlatformBand::requestLiveStreamKey(std::function<void(int value)> callback)
{
	auto alertParent = getAlertParent();
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLSPlatformApiResult result = PLSPlatformApiResult::PAR_API_FAILED;
		QVariant resultCode;
		QVariant publishUrl;
		QVariant streamKey;
		QVariant publishUrlNotKey;
		QVariant liveId;
		QVariant description;
		QVariant maxLiveTime;
		if (HTTP_STATUS_CODE_200 == code) {

			PLS_INFO(MODULE_PLATFORM_BAND, __FUNCTION__ ".success: %d", code);
			PLSJsonDataHandler::getValueFromByteArray(data, "result_code", resultCode);

			PLSJsonDataHandler::getValueFromByteArray(data, "publish_url", publishUrl);
			PLSJsonDataHandler::getValueFromByteArray(data, "stream_key", streamKey);
			PLSJsonDataHandler::getValueFromByteArray(data, "publish_url_without_stream_key", publishUrlNotKey);
			PLSJsonDataHandler::getValueFromByteArray(data, "live_id", liveId);
			PLSJsonDataHandler::getValueFromByteArray(data, "description", description);
			PLSJsonDataHandler::getValueFromByteArray(data, "max_running_time_mins", maxLiveTime);

			setStreamKey(streamKey.toString().toUtf8().data());
			setStreamServer(publishUrlNotKey.toString().toUtf8().data());
			setMaxLiveTime(maxLiveTime.toInt());
			setLiveId(liveId.toString());
			//TODO handler multi and single stream
			if (resultCode == 1) {
				PLS_INFO(MODULE_PLATFORM_BAND, __FUNCTION__, "band perpare ok");
				m_isRequestStart = true;
				result = PLSPlatformApiResult::PAR_SUCCEED;
			} else if (resultCode == 60903) {
				//TODO:alert
				PLS_WARN(MODULE_PLATFORM_BAND, __FUNCTION__, "band aleardy boardcast");
				PLSAlertView::warning(nullptr, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.Band.Have.Broardcast"));
				result = PLSPlatformApiResult::PAR_API_FAILED;
			} else if (resultCode == 60106) {
				PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__, "band have no perssion");
				PLSAlertView::warning(nullptr, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Band.Have.No.Perssion"));
				result = PLSPlatformApiResult::PAR_API_FAILED;
			} else {
				PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__, "band unkonow error");
				PLSAlertView::warning(nullptr, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.Band.Other.Error"));
				result = PLSPlatformApiResult::PAR_API_FAILED;
			}
		}

		callback(static_cast<int>(result));
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__ ".error: %d-%d", code, error);
		QVariant resultCode;
		PLSJsonDataHandler::getValueFromByteArray(data, "result_code", resultCode);
		PLSPlatformApiResult result = getApiResult(code, error);
		showApiRefreshError(result);
		callback(static_cast<int>(result));
		//loop.setProperty("finished", static_cast<int>(result));
		//loop.quit();
	};

	auto callbackFun = [this, _onSucceed, _onFail](bool) {
		auto channelInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
		auto bandKey = channelInfo[ChannelData::g_subChannelId];
		auto accessToken = channelInfo[ChannelData::g_channelToken];
		PLSNetworkReplyBuilder builder(CHANNEL_BAND_LIVE_CREATE);
		builder.setContentType("application/json;charset=UTF-8");
		QVariantMap fieldMaps;
		fieldMaps.insert("band_key", bandKey);
		fieldMaps.insert("description", getDescription().c_str());
		fieldMaps.insert(COOKIE_ACCESS_TOKEN, accessToken);
		builder.setQuerys(fieldMaps);

		PLSHttpHelper::connectFinished(builder.post(), this, _onSucceed, _onFail);
	};

	//refresh token
	getBandRefreshTokenInfo(callbackFun);
}

void PLSPlatformBand::requesetLiveStop(std::function<void()> callback)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		if (HTTP_STATUS_CODE_200 == code) {
			PLS_INFO(MODULE_PLATFORM_BAND, __FUNCTION__ ".success: %d", code);
		}

		if (nullptr != callback) {
			callback();
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)

		PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__ ".error: %d-%d", code, error);

		if (nullptr != callback) {
			callback();
		}
	};

	auto callbackFun = [this, _onSucceed, _onFail](bool) {
		auto channelInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
		auto bandKey = channelInfo[ChannelData::g_subChannelId];
		auto accessToken = channelInfo[ChannelData::g_channelToken];
		PLSNetworkReplyBuilder builder(CHANNEL_BAND_LIVE_OFF);
		builder.setContentType("application/json;charset=UTF-8");
		QVariantMap fieldMaps;
		fieldMaps.insert("band_key", bandKey);
		fieldMaps.insert("live_id", getLiveId());
		fieldMaps.insert(COOKIE_ACCESS_TOKEN, accessToken);

		builder.setQuerys(fieldMaps);

		PLSHttpHelper::connectFinished(builder.post(), this, _onSucceed, _onFail);
	};

	//refresh token
	getBandRefreshTokenInfo(callbackFun);
}

void PLSPlatformBand::setMaxLiveTime(const int &min)
{
	m_maxLiveTime = min;
}

int PLSPlatformBand::getMaxLiveTime() const
{
	return m_maxLiveTime;
}

void PLSPlatformBand::setLiveId(const QString &liveId)
{
	m_liveId = liveId;
}

QString PLSPlatformBand::getLiveId() const
{
	return m_liveId;
}

QJsonObject PLSPlatformBand::getLiveStartParams()
{

	QJsonObject platform(__super::getLiveStartParams());

	platform["liveId"] = getLiveId();
	platform["bandKey"] = PLSCHANNELS_API->getChannelInfo(getChannelUUID()).value(ChannelData::g_subChannelId).toString();
	platform["simulcastChannel"] = PLSCHANNELS_API->getChannelInfo(getChannelUUID()).value(ChannelData::g_nickName).toString();

	return platform;
}
