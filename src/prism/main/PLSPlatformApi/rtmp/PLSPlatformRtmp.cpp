#include "PLSPlatformRtmp.h"

#include "../PLSPlatformApi.h"

PLSPlatformRtmp::PLSPlatformRtmp() {}

PLSServiceType PLSPlatformRtmp::getServiceType() const
{
	return PLSServiceType::ST_RTMP;
}

void PLSPlatformRtmp::onPrepareLive(bool value)
{
	if (!value) {
		prepareLiveCallback(value);
		return;
	}

	setStreamServer(getInitData().value(ChannelData::g_channelRtmpUrl).toString().toStdString());
	setStreamKey(getInitData().value(ChannelData::g_streamKey).toString().toStdString());

	prepareLiveCallback(true);
}

void PLSPlatformRtmp::onAlLiveStarted(bool value)
{
	if (value && getChannelName() == FACEBOOK) {
		pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QTStr("MQTT.Facebook.OnBroadcast"));
	} else if (value && getChannelName() == BAND) {
		pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QTStr("Live.Check.Band.Rtmp.Living.Notice"));
	}
}

QJsonObject PLSPlatformRtmp::getLiveStartParams()
{
	QJsonObject platform;

	platform["id"] = m_mapInitData[ChannelData::g_rtmpUserID].toString();
	platform["password"] = m_mapInitData[ChannelData::g_password].toString();
	platform["simulcastChannel"] = m_mapInitData[ChannelData::g_nickName].toString();

	return platform;
}
