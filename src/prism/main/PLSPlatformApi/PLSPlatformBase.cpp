#pragma once

#include "PLSPlatformBase.hpp"
#include "PLSPlatformApi.h"
#include "PLSChannelDataAPI.h"

const char *const NamesForChannelType[] = {CUSTOM_RTMP, TWITCH, YOUTUBE, FACEBOOK, VLIVE, NAVER_TV, BAND, AFREECATV};
const char *const NamesForSettingId[] = {"rtmp_custom", "Twitch", "YouTube / YouTube Gaming", "Facebook Live", "Vlive", "NaverTv", "BAND", "AFREECATV"};
const char *const NamesForLiveStart[] = {"CUSTOM", "TWITCH", "YOUTUBE", "FACEBOOK", "VLIVE", "NAVERTV", "BAND", "AFREECATV"};

const char *const KeyConfigLiveInfo = "LiveInfo";
const char *const KeyTwitchServer = "TwitchServer";

const QString PLSPlatformBase::getChannelUUID() const
{
	return m_mapInitData[ChannelData::g_channelUUID].toString();
}

const QString PLSPlatformBase::getChannelToken() const
{
	return m_mapInitData[ChannelData::g_channelToken].toString();
}

const QString PLSPlatformBase::getChannelRefreshToken() const
{
	return m_mapInitData[ChannelData::g_refreshToken].toString();
}

const ChannelData::ChannelDataType PLSPlatformBase::getChannelType() const
{
	return static_cast<ChannelData::ChannelDataType>(m_mapInitData[ChannelData::g_data_type].toInt());
}

const QString PLSPlatformBase::getChannelName() const
{
	return m_mapInitData[ChannelData::g_channelName].toString();
}

const int PLSPlatformBase::getChannelOrder() const
{
	return m_mapInitData[ChannelData::g_displayOrder].toInt();
}

const QString PLSPlatformBase::getChannelCookie() const
{
	return m_mapInitData[ChannelData::g_channelCookie].toString();
}

const QVariantMap &PLSPlatformBase::getInitData()
{
	auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	for (auto iter = info.cbegin(); iter != info.cend(); ++iter) {
		m_mapInitData[iter.key()] = iter.value();
	}

	return m_mapInitData;
}

void PLSPlatformBase::activateCallback(bool value)
{
	if (!PLS_PLATFORM_API->isDuringActivate()) {
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ".not: %p %d type=%d, name=%s, uuid=%s", this, value, getChannelType(), getChannelName().toStdString().c_str(),
			 getChannelUUID().toStdString().c_str());
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %p %d type=%d, name=%s, uuid=%s", this, value, getChannelType(), getChannelName().toStdString().c_str(),
		 getChannelUUID().toStdString().c_str());

	PLS_PLATFORM_API->activateCallback(this, value);
}
void PLSPlatformBase::deactivateCallback(bool value)
{
	if (!PLS_PLATFORM_API->isDuringDeactivate()) {
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ".not: %p %d type=%d, name=%s, uuid=%s", this, value, getChannelType(), getChannelName().toStdString().c_str(),
			 getChannelUUID().toStdString().c_str());
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %p %d type=%d, name=%s, uuid=%s", this, value, getChannelType(), getChannelName().toStdString().c_str(),
		 getChannelUUID().toStdString().c_str());

	PLS_PLATFORM_API->deactivateCallback(this, value);
}

void PLSPlatformBase::prepareLiveCallback(bool value)
{
	if (LiveStatus::PrepareLive != PLS_PLATFORM_API->getLiveStatus()) {
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %d type=%d, name=%s, uuid=%s", value, getChannelType(), getChannelName().toStdString().c_str(), getChannelUUID().toStdString().c_str());

	m_bApiPrepared = value;
	if (value) {
		PLS_PLATFORM_API->saveStreamSettings(getNameForSettingId(), getStreamServer(), getStreamKey());
	}

	auto platforms = PLS_PLATFORM_API->getActivePlatforms();
	auto iter = find(platforms.begin(), platforms.end(), this);

	if (iter != platforms.end() && ++iter != platforms.end()) {
		(*iter)->onPrepareLive(value);
	} else {
		PLS_PLATFORM_API->prepareLiveCallback(value);
	}
}

void PLSPlatformBase::liveStartedCallback(bool value)
{
	if (LiveStatus::LiveStarted != PLS_PLATFORM_API->getLiveStatus()) {
		return;
	}

	m_bApiStarted = true;

	auto platforms = PLS_PLATFORM_API->getActivePlatforms();
	auto iter = find(platforms.begin(), platforms.end(), this);

	if (iter != platforms.end() && ++iter != platforms.end()) {
		(*iter)->onLiveStarted(value);
	} else {
		PLS_PLATFORM_API->liveStartedCallback(value);
	}
}

void PLSPlatformBase::prepareFinishCallback()
{
	if (LiveStatus::PrepareFinish != PLS_PLATFORM_API->getLiveStatus()) {
		return;
	}

	auto platforms = PLS_PLATFORM_API->getActivePlatforms();
	auto iter = find(platforms.begin(), platforms.end(), this);

	if (iter != platforms.end() && ++iter != platforms.end()) {
		(*iter)->onPrepareFinish();
	} else {
		PLS_PLATFORM_API->prepareFinishCallback();
	}
}

void PLSPlatformBase::liveStoppedCallback()
{
	if (LiveStatus::LiveStoped != PLS_PLATFORM_API->getLiveStatus()) {
		onLiveEnded();
		return;
	}

	auto platforms = PLS_PLATFORM_API->getActivePlatforms();
	auto iter = find(platforms.begin(), platforms.end(), this);

	if (iter != platforms.end() && ++iter != platforms.end()) {
		(*iter)->onLiveStopped();
	} else {
		PLS_PLATFORM_API->liveStoppedCallback();
	}
}

void PLSPlatformBase::liveEndedCallback()
{
	if (LiveStatus::LiveEnded != PLS_PLATFORM_API->getLiveStatus()) {
		return;
	}

	auto platforms = PLS_PLATFORM_API->getActivePlatforms();
	auto iter = find(platforms.begin(), platforms.end(), this);

	if (iter != platforms.end() && ++iter != platforms.end()) {
		(*iter)->onLiveEnded();
	} else {
		PLS_PLATFORM_API->liveEndedCallback();
	}
}

QJsonObject PLSPlatformBase::getWebChatParams()
{
	QJsonObject platform;

	platform.insert("name", QString::fromStdString(getNameForLiveStart()));
	platform.insert("accessToken", getChannelToken());

	return platform;
}

QJsonObject PLSPlatformBase::getMqttChatParams()
{
	QJsonObject platform;

	platform.insert("accessToken", getChannelToken());

	return platform;
}

QJsonObject PLSPlatformBase::getLiveStartParams()
{
	QJsonObject platform;

	platform.insert("accessToken", getChannelToken());

	return platform;
}

bool PLSPlatformBase::isMqttChatCanShow(const QJsonObject &)
{
	return true;
}
