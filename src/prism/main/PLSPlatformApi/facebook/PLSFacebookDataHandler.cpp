#include "PLSFacebookDataHandler.h"
#include "PLSChannelDataAPI.h"
#include "../PLSPlatformApi.h"

PLSFacebookDataHandler::PLSFacebookDataHandler() {}

PLSFacebookDataHandler::~PLSFacebookDataHandler() {}

QString PLSFacebookDataHandler::getPlatformName()
{
	return FACEBOOK;
}

bool PLSFacebookDataHandler::tryToUpdate(const QVariantMap &srcInfo, UpdateCallback finishedCall)
{
	PLS_ERROR(MODULE_PLATFORM_FACEBOOK, "Facebook refresh new");
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	if (!facebook) {
		QVariantMap info = srcInfo;
		PLS_ERROR(MODULE_PLATFORM_FACEBOOK, "Facebook refresh failed, platform not exists");
		info[ChannelData::g_channelStatus] = ChannelData::ChannelStatus::Error;
		finishedCall(QList<QVariantMap>{info});
		return false;
	}

	facebook->setInitData(srcInfo);
	QMetaObject::invokeMethod(facebook, [=]() {
		facebook->initInfo(srcInfo);
		auto userInfoFinished = [=](PLSAPIFacebookType type) {
			QVariantMap info = facebook->getSrcInfo();
			finishedCall(QList<QVariantMap>{info});
			return true;
		};
		facebook->getLongLivedUserAccessToken(userInfoFinished);
	});
	return true;
}
