#include "PLSBandDataHandler.h"

#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>

#include "json-data-handler.hpp"
#include "pls-net-url.hpp"
#include "pls-common-define.hpp"
#include "../PLSPlatformApi.h"

PLSBandDataHandler::PLSBandDataHandler() : m_platformBand(nullptr) {}

PLSBandDataHandler::~PLSBandDataHandler() {}

QString PLSBandDataHandler::getPlatformName()
{
	return BAND;
}

bool PLSBandDataHandler::tryToUpdate(const QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	PLS_INFO(MODULE_PLATFORM_BAND, "Band platform refresh");
	QString channelUUID = srcInfo[ChannelData::g_channelUUID].toString();

	PLSPlatformBand *band = dynamic_cast<PLSPlatformBand *>(PLS_PLATFORM_API->getPlatformById(channelUUID, srcInfo));
	if (!band) {
		PLS_ERROR(MODULE_PLATFORM_BAND, "Band refresh failed, platform not exists");
		QVariantMap info = srcInfo;
		info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Error;
		finishedCall(m_bandInfos);
		return false;
	} else {
		band->clearBandInfos();
		band->setInitData(srcInfo);

		QMetaObject::invokeMethod(band, [=]() {
			band->initLiveInfo(PLSCHANNELS_API->isResetNeed());
			if (getInfo(srcInfo, ChannelData::g_channelToken).isEmpty()) {
				band->getBandTokenInfo(srcInfo, finishedCall);
			} else {
				band->getBandCategoryInfo(srcInfo, finishedCall);
			}
		});
	}

	return true;
}

//void PLSBandDataHandler::getBandUserProfile(UpdateCallback finishedCall)
//{
//	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
//		Q_UNUSED(networkReplay)
//		Q_UNUSED(context)
//		Q_UNUSED(code)
//		QVariant nickName;
//		QVariant profileImg;
//		PLSJsonDataHandler::getValueFromByteArray(data, "name", nickName);
//		PLSJsonDataHandler::getValueFromByteArray(data, "profile_image_url", profileImg);
//
//		m_bandInfos[ChannelData::g_nickName] = nickName;
//		if (auto icon = getChannelEmblemSync(profileImg.toString()); icon.first) {
//			m_bandInfos[ChannelData::g_userIconCachePath] = icon.second;
//		}
//	};
//
//	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
//		Q_UNUSED(networkReplay)
//		Q_UNUSED(context)
//		Q_UNUSED(code)
//
//		PLS_ERROR(MODULE_PLATFORM_BAND, __FUNCTION__ ".error: %d-%d", code, error);
//		m_bandInfos[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Error;
//	};
//
//	QString code = m_bandInfos.value(ChannelData::g_channelCode).toString();
//	QString authUrlStr = QString(CHANNEL_BAND_AUTH).arg(code);
//
//	PLSNetworkReplyBuilder builder(CHANNEL_BAND_USER_PROFILE);
//	builder.setContentType("application/json;charset=UTF-8");
//	QVariantMap fieldMaps;
//	fieldMaps.insert(COOKIE_ACCESS_TOKEN, m_bandInfos.value(ChannelData::g_channelToken));
//	builder.setFields(fieldMaps);
//
//	PLSHttpHelper::connectFinished(builder.get(), m_platformBand, _onSucceed, _onFail);
//}
