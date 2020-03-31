#include "PLSVLiveDataHandler.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>

#include <QMetaMethod>
#include "../PLSPlatformApi.h"
#include "PLSAPIVLive.h"
#include "PLSChannelDataAPI.h"
#include "PLSPlatformVLive.h"
#include "pls-common-define.hpp"
#include "pls-net-url.hpp"
//#include "PLSPlatformVLive.h"

PLSVLiveDataHandler::PLSVLiveDataHandler() {}
PLSVLiveDataHandler::~PLSVLiveDataHandler() {}

QString PLSVLiveDataHandler::getPlatformName()
{
	return VLIVE;
}

bool PLSVLiveDataHandler::tryToUpdate(const QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	PLSPlatformVLive *vlive = PLS_PLATFORM_VLIVE;

	if (!vlive) {
		PLS_ERROR(MODULE_PlatformService, "VLive refresh failed, platform not exists");
		QVariantMap info = srcInfo;
		info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Error;
		finishedCall(QList<QVariantMap>{info});
		return false;
	}

	vlive->setInitData(srcInfo);
	QMetaObject::invokeMethod(vlive, [=]() {
		vlive->reInitLiveInfo(PLSCHANNELS_API->isResetNeed());
		auto _onNext = [=]() { vlive->requestChannelInfo(srcInfo, finishedCall); };
		if (!vlive->getGccData().isLoaded) {
			vlive->initVliveGcc(_onNext);
		} else {
			_onNext();
		}
	});
	return true;
}
