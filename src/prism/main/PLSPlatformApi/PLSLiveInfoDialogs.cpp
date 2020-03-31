#include "PLSLiveInfoDialogs.h"

#include "PLSPlatformApi.h"
#include "twitch/PLSLiveInfoTwitch.h"
#include "youtube/PLSLiveInfoYoutube.h"
#include "vlive/PLSLiveInfoVLive.h"
#include "band/PLSLiveInfoBand.h"
#include "facebook/PLSLiveInfoFacebook.h"
#include "ChannelConst.h"
#include "PLSLiveInfoAfreecaTV.h"

int pls_exec_live_Info(const QVariantMap &info, QWidget *parent)
{
	auto channelName = info.value(ChannelData::g_channelName).toString();
	auto channelUUID = info.value(ChannelData::g_channelUUID).toString();

	if (TWITCH == channelName) {
		return pls_exec_live_Info_twitch(channelUUID, info, parent);
	} else if (YOUTUBE == channelName) {
		return pls_exec_live_Info_youtube(channelUUID, info, parent);
	} else if (VLIVE == channelName) {
		return pls_exec_live_Info_vlive(channelUUID, info, parent);
	} else if (BAND == channelName) {
		return pls_exec_live_Info_band(channelUUID, info, parent);
	} else if (WAV == channelName) {
		//TODO
	} else if (FACEBOOK == channelName) {
		return pls_exec_live_Info_facebook(channelUUID, info, parent);
	} else if (AFREECATV == channelName) {
		return pls_exec_live_Info_afreecatv(channelUUID, info, parent);
	}

	return QDialog::Rejected;
}

int pls_exec_live_Info_twitch(const QString &which, const QVariantMap &info, QWidget *parent)
{
	auto pPlatform = PLS_PLATFORM_API->getPlatformById(which, info);

	if (nullptr == pPlatform) {
		return QDialog::Rejected;
	}

	PLSLiveInfoTwitch liveInfo(pPlatform, parent);
	return liveInfo.exec();
}

int pls_exec_live_Info_youtube(const QString &which, const QVariantMap &info, QWidget *parent)
{
	auto pPlatform = PLS_PLATFORM_API->getPlatformById(which, info);

	if (nullptr == pPlatform) {
		return QDialog::Rejected;
	}

	PLSLiveInfoYoutube liveInfo(pPlatform, parent);
	return liveInfo.exec();
}

int pls_exec_live_Info_vlive(const QString &which, const QVariantMap &info, QWidget *parent)
{
	auto pPlatform = PLS_PLATFORM_API->getPlatformById(which, info);

	if (nullptr == pPlatform) {
		return QDialog::Rejected;
	}

	PLSLiveInfoVLive liveInfo(pPlatform, parent);
	return liveInfo.exec();
}

int pls_exec_live_Info_band(const QString &which, const QVariantMap &info, QWidget *parent)
{
	auto pPlatform = PLS_PLATFORM_API->getPlatformById(which, info);

	if (nullptr == pPlatform) {
		return QDialog::Rejected;
	}

	PLSLiveInfoBand liveInfo(pPlatform, parent);
	return liveInfo.exec();
}

int pls_exec_live_Info_afreecatv(const QString &which, const QVariantMap &info, QWidget *parent)
{
	auto pPlatform = PLS_PLATFORM_API->getPlatformById(which, info);

	if (nullptr == pPlatform) {
		return QDialog::Rejected;
	}
	PLSLiveInfoAfreecaTV liveInfo(pPlatform, parent);
	return liveInfo.exec();
}

int pls_exec_live_Info_facebook(const QString &which, const QVariantMap &info, QWidget *parent)
{
	auto pPlatform = PLS_PLATFORM_API->getPlatformById(which, info);

	if (nullptr == pPlatform) {
		return QDialog::Rejected;
	}

	PLSLiveInfoFacebook liveInfo(pPlatform, parent);
	return liveInfo.exec();
}
