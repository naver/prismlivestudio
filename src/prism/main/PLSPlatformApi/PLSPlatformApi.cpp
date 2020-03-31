#include "PLSPlatformApi.h"

#include <string>
#include <algorithm>
#include <functional>
#include <iterator>
#include <QApplication>
#include <QString>

#include "log.h"
#include "frontend-api.h"
#include "alert-view.hpp"
#include "ChannelConst.h"
#include "window-basic-main.hpp"
#include "PLSLiveInfoDialogs.h"
#include "twitch/PLSLiveInfoTwitch.h"
#include "PLSHttpApi\PLSHttpHelper.h"
#include "channels/ChannelsDataApi/ChannelConst.h"
#include "channels/ChannelsDataApi/PLSChannelDataAPI.h"
#include "pls-net-url.hpp"
#include "band/PLSPlatformBand.h"
#include "PLSLiveEndDialog.h"
#include "qwidget.h"
#include "main-view.hpp"
#include "youtube/PLSAPIYoutube.h"
#include "window-basic-main.hpp"

#define PRISM_MAX_OUT_Y 1080

using namespace std;

PLSPlatformApi *PLSPlatformApi::instance()
{
	static PLSPlatformApi *_instance = nullptr;

	if (nullptr == _instance) {
		_instance = new PLSPlatformApi();
		_instance->moveToThread(qApp->thread()); //zhangdewen force move to main thread
		prism_frontend_web_invoked = [](const char *) { return ""; };
		QObject::connect(qApp, &QCoreApplication::aboutToQuit, [] {
			delete _instance;
			_instance = nullptr;
		});
	}

	return _instance;
}

PLSPlatformApi::PLSPlatformApi()
{
}

PLSPlatformApi::~PLSPlatformApi()
{
	for (auto item : platformList) {
		delete item;
	}
	platformList.clear();
}

bool PLSPlatformApi::initialize()
{
	obs_frontend_add_event_callback(onFrontendEvent, this);

	/*for (auto &item : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		auto uuid = item.value(ChannelData::g_channelUUID).toString();

		if (isValidChannel(item)) {
			onActive(uuid);
		}
	}*/

	connect(PLSCHANNELS_API, &PLSChannelDataAPI::channelActiveChanged, this, [=](const QString &channelUUID, bool enable) {
		if (enable) {
			onActive(channelUUID);
		} else {
			onInactive(channelUUID);
		}
	});

	connect(PLSCHANNELS_API, &PLSChannelDataAPI::channelAdded, this, &PLSPlatformApi::onAddChannel);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::channelModified, this, &PLSPlatformApi::onUpdateChannel);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::channelRemoved, this, &PLSPlatformApi::onRemoveChannel);

	connect(PLSCHANNELS_API, &PLSChannelDataAPI::sigAllClear, this, &PLSPlatformApi::onClearChannel);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::broadcastGo, this, [=]() {
		emit enterLivePrepareState(true);
		onPrepareLive();
	});
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::stopBroadcastGo, this, &PLSPlatformApi::onPrepareFinish);
	connect(this, &PLSPlatformApi::livePrepared, this, [=](bool value) {
		if (value == false) {
			emit enterLivePrepareState(false);
		}
	});
	connect(this, &PLSPlatformApi::liveStarted, this, [=]() { emit enterLivePrepareState(false); });

	return true;
}

void PLSPlatformApi::saveStreamSettings(OBSData settings) const
{
	const char *service_id = "rtmp_common";

	obs_service_t *oldService = obs_frontend_get_streaming_service();
	OBSData hotkeyData = obs_hotkeys_save_service(oldService);
	obs_data_release(hotkeyData);

	OBSService newService = obs_service_create(service_id, "default_service", settings, hotkeyData);
	obs_service_release(newService);

	if (!newService)
		return;

	obs_frontend_set_streaming_service(newService);
	obs_frontend_save_streaming_service();
}

void PLSPlatformApi::saveStreamSettings(const string &platform, const string &server, const string &key) const
{
	if (isLiving()) {
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": Platform=%s, StreamUrl=%s, StreamKey=%s", platform.c_str(), server.c_str(), key.c_str());

	OBSData settings = obs_data_create();
	obs_data_release(settings);

	obs_data_set_string(settings, "service", platform.data());
	obs_data_set_string(settings, "server", server.data());
	obs_data_set_string(settings, "key", key.data());

	saveStreamSettings(settings);
}

list<PLSPlatformBase *> PLSPlatformApi::getActivePlatforms() const
{
	list<PLSPlatformBase *> lst;

	QMutexLocker locker(&platformListMutex);
	copy_if(platformList.begin(), platformList.end(), back_inserter(lst), [](const auto v) { return v->isActive(); });

	return lst;
}

bool PLSPlatformApi::isPlatformActived(PLSServiceType serviceType) const
{
	QMutexLocker locker(&platformListMutex);
	return any_of(platformList.begin(), platformList.end(), [=](auto item) { return item->isActive() && item->getServiceType() == serviceType; });
}

PLSPlatformTwitch *PLSPlatformApi::getPlatformTwitch()
{
	return dynamic_cast<PLSPlatformTwitch *>(getPlatformByType(PLSServiceType::ST_TWITCH));
}

PLSPlatformYoutube *PLSPlatformApi::getPlatformYoutube(bool froceCreate)
{
	return dynamic_cast<PLSPlatformYoutube *>(getPlatformByType(PLSServiceType::ST_YOUTUBE, froceCreate));
}

PLSPlatformFacebook *PLSPlatformApi::getPlatformFacebook()
{
	return dynamic_cast<PLSPlatformFacebook *>(getPlatformByType(PLSServiceType::ST_FACEBOOK));
}

PLSPlatformVLive *PLSPlatformApi::getPlatformVLive()
{
	return dynamic_cast<PLSPlatformVLive *>(getPlatformByType(PLSServiceType::ST_VLIVE));
}

PLSPlatformAfreecaTV *PLSPlatformApi::getPlatformAfreecaTV()
{
	return dynamic_cast<PLSPlatformAfreecaTV *>(getPlatformByType(PLSServiceType::ST_AFREECATV));
}

void PLSPlatformApi::onActive(const QString &which)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %s", which.toStdString().c_str());
	m_bDuringActivate = true;

	QVariantMap info = PLSCHANNELS_API->getChannelInfo(which);
	if (info.empty()) {
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".empty: %s", which.toStdString().c_str());
		return;
	}

	const auto channelType = info.value(ChannelData::g_data_type).toInt();
	const auto channelName = info.value(ChannelData::g_channelName).toString();

	if (!isValidChannel(info)) {
		PLS_WARN(MODULE_PlatformService, __FUNCTION__ ".InvalidChannel: %d %s %s", channelType, channelName.toStdString().c_str(), which.toStdString().c_str());
		return;
	}

	auto platform = getPlatformById(which, info);
	if (nullptr != platform) {
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %p %s", platform, which.toStdString().c_str());
		if (!platform->isActive()) {
			platform->onActive();
		}
	}
}

void PLSPlatformApi::onInactive(const QString &which)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %s", which.toStdString().c_str());
	m_bDuringDeactivate = true;

	QVariantMap info = PLSCHANNELS_API->getChannelInfo(which);
	if (info.empty()) {
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".empty: %s", which.toStdString().c_str());
		return;
	}

	auto platform = getPlatformById(which, info);
	if (nullptr != platform) {
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %p %s", platform, which.toStdString().c_str());
		if (platform->isActive()) {
			platform->onInactive();
		}
	}
}

void PLSPlatformApi::setLiveStatus(LiveStatus value)
{
	m_liveStatus = value;

	if (LiveStatus::PrepareLive == value) {
		PLSBasic::Get()->getApi()->on_event(pls_frontend_event::PLS_FRONTEND_EVENT_LIVE_OR_RECORD_START);
	} else if (LiveStatus::Normal == value) {
		if (!isRecording()) {
			PLSBasic::Get()->getApi()->on_event(pls_frontend_event::PLS_FRONTEND_EVENT_LIVE_OR_RECORD_END);
		}
	}
}

void PLSPlatformApi::onPrepareLive()
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	setLiveStatus(LiveStatus::PrepareLive);

	m_bApiPrepared = false;
	m_bApiStarted = false;

	m_bPrismLive = true;
	uint32_t out_cy = config_get_uint(PLSBasic::Get()->Config(), "Video", "OutputCY");
	if (out_cy > PRISM_MAX_OUT_Y || PLSPlatformApi::isContainVliveChannel()) {
		m_bPrismLive = false;
	}

	QSet<QString> setChannelsFromDashboard;
	for (auto &info : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		const auto channelUUID = info.value(ChannelData::g_channelUUID).toString();
		const auto channelType = info.value(ChannelData::g_data_type).toInt();
		const auto channelName = info.value(ChannelData::g_channelName).toString();
		const auto displayOrder = info.value(ChannelData::g_displayOrder).toInt();

		if (isValidChannel(info)) {
			setChannelsFromDashboard.insert(channelUUID);

			auto platform = getPlatformById(channelUUID, info);
			if (nullptr != platform && !platform->isActive()) {
				onActive(channelUUID);
			}

			PLS_INFO(MODULE_PlatformService, __FUNCTION__ ".SelectedChannels: %p type=%d, name=%s, order=%d, uuid=%s", platform, channelType, channelName.toStdString().c_str(),
				 displayOrder, channelUUID.toStdString().c_str());
		} else {
			PLS_INFO(MODULE_PlatformService, __FUNCTION__ ".InvalidChannels: type=%d, name=%s, uuid=%s", channelType, channelName.toStdString().c_str(), channelUUID.toStdString().c_str());
		}
	}

	{
		QMutexLocker locker(&platformListMutex);
		platformList.sort([](PLSPlatformBase *lValue, PLSPlatformBase *rValue) { return lValue->getChannelOrder() < rValue->getChannelOrder(); });
	}

	uuidOnStarted.clear();
	auto platformActived = PLS_PLATFORM_ACTIVIED;
	for (auto &info : platformActived) {
		if (!setChannelsFromDashboard.contains(info->getChannelUUID())) {
			onInactive(info->getChannelUUID());
		}
	}
	platformActived = PLS_PLATFORM_ACTIVIED;
	for (auto &item : platformActived) {
		uuidOnStarted.push_back(item->getChannelUUID());
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ".ActivedPlatform: %p %s %s", item, item->getChannelUUID().toStdString().c_str(), item->getNameForChannelType());
	}

	if (platformActived.empty()) {
		prepareLiveCallback(false);
		return;
	}

	if (!m_bPrismLive) {
		if (PLS_PLATFORM_ACTIVIED.size() > 1) {
			auto alertResult = QDialogButtonBox::Cancel;
			if (isRecording()) {
				PLSAlertView::warning(PLSBasic::Get(), QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.Internal.Greater.1080.Recording"));
			} else {
				alertResult = PLSAlertView::warning(PLSBasic::Get(), QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.Multicast.Greater.1080"));
			}
			prepareLiveCallback(false);
			if (QDialogButtonBox::Ok == alertResult) {
				PLSBasic::Get()->showSettingVideo();
			}
			return;
		}
	}

	if (!pls_network_environment_reachable()) {
		PLSAlertView::warning(PLSBasic::Get(), QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.Network.Error"));
		prepareLiveCallback(false);
		return;
	}

	m_iTotalSteps = 0;
	for (auto item : platformActived) {
		if (PLSServiceType::ST_RTMP != item->getServiceType()) {
			item->setCurrStep(++m_iTotalSteps);
		}

		item->setChannelLiveSeq(0);
		item->setTokenRequestStatus(PLSTokenRequestStatus::PLS_GOOD);
	}

	platformActived.front()->onPrepareLive(true);
}

void PLSPlatformApi::onLiveStarted()
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	setLiveStatus(LiveStatus::LiveStarted);

	auto platformActived = PLS_PLATFORM_ACTIVIED;
	if (platformActived.empty()) {
		liveStartedCallback(false);
		return;
	}

	platformActived.front()->onLiveStarted(true);
}

void PLSPlatformApi::onPrepareFinish()
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	setLiveStatus(LiveStatus::PrepareFinish);

	auto platformActived = PLS_PLATFORM_ACTIVIED;
	if (platformActived.empty()) {
		prepareFinishCallback();
		return;
	}

	platformActived.front()->onPrepareFinish();
}

void PLSPlatformApi::onLiveStopped()
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	if (m_liveStatus < LiveStatus::ToStart || m_liveStatus >= LiveStatus::LiveStoped) {
		PLS_WARN(MODULE_PlatformService, __FUNCTION__, ".Unexpected status, %d", m_liveStatus);
		return;
	}

	setLiveStatus(LiveStatus::LiveStoped);

	if (nullptr != prism_frontend_dispatch_js_event) {
		const char *pszTypeEnd = "{\"type\": \"end\"}";
		prism_frontend_dispatch_js_event("prism_events", pszTypeEnd);
	}

	auto platformActived = PLS_PLATFORM_ACTIVIED;
	if (platformActived.empty()) {
		liveStoppedCallback();
		return;
	}

	platformActived.front()->onLiveStopped();
}

void PLSPlatformApi::onLiveEnded()
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	setLiveStatus(LiveStatus::LiveEnded);

	auto platformActived = PLS_PLATFORM_ACTIVIED;
	if (platformActived.empty()) {
		liveEndedCallback();
		return;
	}

	platformActived.front()->onLiveEnded();
}

void PLSPlatformApi::onRecordingStarted()
{
	m_bRecording = true;

	PLSBasic::Get()->getApi()->on_event(pls_frontend_event::PLS_FRONTEND_EVENT_LIVE_OR_RECORD_START);
}

void PLSPlatformApi::onRecordingStoped()
{
	m_bRecording = false;

	if (LiveStatus::Normal == m_liveStatus) {
		PLSBasic::Get()->getApi()->on_event(pls_frontend_event::PLS_FRONTEND_EVENT_LIVE_OR_RECORD_END);
		emit outputStopped();
	}
}

void PLSPlatformApi::activateCallback(PLSPlatformBase *platform, bool value)
{
	if (!m_bDuringActivate) {
		return;
	}

	m_bDuringActivate = false;
	emit channelActive(platform->getChannelUUID(), value);
}

void PLSPlatformApi::deactivateCallback(PLSPlatformBase *platform, bool value)
{
	if (!m_bDuringDeactivate) {
		return;
	}

	m_bDuringDeactivate = false;
	emit channelDeactive(platform->getChannelUUID(), value);
}

void PLSPlatformApi::prepareLiveCallback(bool value)
{
	if (LiveStatus::PrepareLive != m_liveStatus) {
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": Result=%s", BOOL2STR(value));

	m_bApiPrepared = value;
	if (value) {
		setLiveStatus(LiveStatus::ToStart);
	} else {
		m_bApiStarted = false;
		setLiveStatus(LiveStatus::Normal);
	}

	auto platformActived = PLS_PLATFORM_ACTIVIED;
	for (auto info : platformActived) {
		info->onAllPrepareLive(value);
	}

	if (!value) {
		emit liveEnded(false, false);
	}

	emit livePrepared(value);
}

void PLSPlatformApi::liveStartedCallback(bool value)
{
	if (LiveStatus::LiveStarted != m_liveStatus) {
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": Result=%s", BOOL2STR(value));

	m_bApiStarted = value;
	setLiveStatus(LiveStatus::Living);

	bool bInteractChannel = false;
	auto platformActived = PLS_PLATFORM_ACTIVIED;
	for (auto info : platformActived) {
		if (PLSServiceType::ST_RTMP != info->getServiceType()) {
			bInteractChannel = true;
		}
		info->onAlLiveStarted(value);
	}

	for (auto uuid : getUuidOnStarted()) {
		auto mSourceData = PLSCHANNELS_API->getChanelInfoRef(uuid);
		if (mSourceData.contains(ChannelData::g_viewers)) {
			PLSCHANNELS_API->setValueOfChannel(uuid, ChannelData::g_viewers, QString("0"));
		}
		if (mSourceData.contains(ChannelData::g_likes)) {
			PLSCHANNELS_API->setValueOfChannel(uuid, ChannelData::g_likes, QString("0"));
		}
		if (mSourceData.contains(ChannelData::g_comments)) {
			PLSCHANNELS_API->setValueOfChannel(uuid, ChannelData::g_comments, QString("0"));
		}
		if (mSourceData.contains(ChannelData::g_totalViewers)) {
			PLSCHANNELS_API->setValueOfChannel(uuid, ChannelData::g_totalViewers, QString("0"));
		}
	}

	if (value) {
		QMetaObject::invokeMethod(this, &PLSPlatformApi::showStartMessageIfNeeded, Qt::QueuedConnection);
	}
	emit liveStarted(value);
}

void PLSPlatformApi::prepareFinishCallback()
{
	if (LiveStatus::PrepareFinish != m_liveStatus) {
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	setLiveStatus(LiveStatus::ToStop);

	emit liveToStop();
}

void PLSPlatformApi::liveStoppedCallback()
{
	if (LiveStatus::LiveStoped != m_liveStatus) {
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	onLiveEnded();
}

void PLSPlatformApi::liveEndedCallback()
{
	if (LiveStatus::LiveEnded != m_liveStatus) {
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__);

	emit enterLivePrepareState(false);
	emit liveEnded(isApiPrepared(), isApiStarted());
	emit liveEndedForUi();

	setLiveStatus(LiveStatus::Normal);

	if (!m_bRecording) {
		emit outputStopped();
	}
}

void PLSPlatformApi::onAddChannel(const QString &which)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %s", which.toStdString().c_str());

	QVariantMap info = PLSCHANNELS_API->getChannelInfo(which);
	if (info.empty()) {
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".empty: %s", which.toStdString().c_str());
		return;
	}

	const auto channelType = info.value(ChannelData::g_data_type).toInt();
	const auto channelName = info.value(ChannelData::g_channelName).toString();

	if (!isValidChannel(info)) {
		PLS_WARN(MODULE_PlatformService, __FUNCTION__ ".InvalidChannel: %d %s %s", channelType, channelName.toStdString().c_str(), which.toStdString().c_str());
		return;
	}

	auto platform = getPlatformById(which, info);
	if (nullptr != platform) {
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %d %s %s", channelType, channelName.toStdString().c_str(), which.toStdString().c_str());
}

void PLSPlatformApi::onUpdateChannel(const QString &which)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %s", which.toStdString().c_str());

	QVariantMap info = PLSCHANNELS_API->getChannelInfo(which);
	if (info.empty()) {
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".empty: %s", which.toStdString().c_str());
		return;
	}

	const auto channelType = info.value(ChannelData::g_data_type).toInt();
	const auto channelName = info.value(ChannelData::g_channelName).toString();

	if (!isValidChannel(info)) {
		PLS_WARN(MODULE_PlatformService, __FUNCTION__ ".InvalidChannel: %d %s %s", channelType, channelName.toStdString().c_str(), which.toStdString().c_str());
		return;
	}

	auto platform = getPlatformById(which, info);
	if (nullptr != platform) {
		platform->setInitData(info);
	}
}

void PLSPlatformApi::onRemoveChannel(const QString &which)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %s", which.toStdString().c_str());

	QVariantMap info = PLSCHANNELS_API->getChannelInfo(which);
	if (!info.isEmpty()) {
		const auto channelType = info.value(ChannelData::g_data_type).toInt();
		const auto channelName = info.value(ChannelData::g_channelName).toString();

		if (!isValidChannel(info)) {
			PLS_WARN(MODULE_PlatformService, __FUNCTION__ ".InvalidChannel: %d %s %s", channelType, channelName.toStdString().c_str(), which.toStdString().c_str());
			return;
		}
	}

	auto platform = getPlatformById(which, {}, true);
	if (nullptr != platform) {
		platform->setActive(false);
		emit channelRemoved(platform->getInitData());
		if (!platform->isSingleChannel()) {
			QMutexLocker locker(&platformListMutex);
			platformList.remove(platform);
			delete platform;
		}
	}
}

void PLSPlatformApi::onClearChannel()
{
	QMutexLocker locker(&platformListMutex);
	for (auto iter = platformList.begin(); iter != platformList.end();) {
		auto platform = *iter;
		platform->setActive(false);

		if (!platform->isSingleChannel()) {
			iter = platformList.erase(iter);
			delete platform;
		} else {
			++iter;
		}
	}
}

PLSServiceType PLSPlatformApi::getServiceType(const QVariantMap &info) const
{
	if (info.isEmpty()) {
		auto text = QString(__FUNCTION__ " info is empty");
		PLS_ERROR(MODULE_PlatformService, text.toStdString().c_str());

		assert(false);
		return PLSServiceType::ST_RTMP;
	}

	const auto type = info.value(ChannelData::g_data_type).toInt();
	const auto name = info.value(ChannelData::g_channelName).toString();

	switch (type) {
	case ChannelData::RTMPType:
		return PLSServiceType::ST_RTMP;
	case ChannelData::ChannelType:
		for (int i = 1; i < PLATFORM_SIZE; ++i) {
			if (name == NamesForChannelType[i]) {
				return static_cast<PLSServiceType>(i);
			}
		}
	}

	auto text = QString(__FUNCTION__ " unmatched channel type: %1, %2").arg(type).arg(name);
	PLS_ERROR(MODULE_PlatformService, text.toStdString().c_str());

	return PLSServiceType::ST_RTMP;
}

PLSPlatformBase *PLSPlatformApi::buildPlatform(PLSServiceType type)
{
	PLSPlatformBase *platform = nullptr;

	switch (type) {
	case PLSServiceType::ST_RTMP:
		platform = new PLSPlatformRtmp();
		break;
	case PLSServiceType::ST_TWITCH:
		platform = new PLSPlatformTwitch();
		break;
	case PLSServiceType::ST_YOUTUBE:
		platform = new PLSPlatformYoutube();
		break;
	case PLSServiceType::ST_FACEBOOK:
		platform = new PLSPlatformFacebook();
		break;
	case PLSServiceType::ST_NAVERTV:
		throw;
		break;
	case PLSServiceType::ST_VLIVE:
		platform = new PLSPlatformVLive();
		break;
	case PLSServiceType::ST_BAND:
		platform = new PLSPlatformBand();
		break;
	case PLSServiceType::ST_AFREECATV:
		platform = new PLSPlatformAfreecaTV();
		break;
	default:
		assert(false);
		break;
	}

	if (nullptr != platform) {
		platform->moveToThread(this->thread());

		QMutexLocker locker(&platformListMutex);
		platformList.push_back(platform);
	} else {
		assert(false);
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".null %d", type);
	}

	return platform;
}

PLSPlatformBase *PLSPlatformApi::getPlatformByType(PLSServiceType type, bool froceCreate)
{
	PLSPlatformBase *platform = nullptr;

	{
		QMutexLocker locker(&platformListMutex);
		for (auto item : platformList) {
			if (type == item->getServiceType()) {
				platform = item;
				break;
			}
		}
	}

	if (nullptr == platform && froceCreate) {
		platform = buildPlatform(type);
	}

	return platform;
}

list<PLSPlatformBase *> PLSPlatformApi::getPlatformsByType(PLSServiceType type)
{
	list<PLSPlatformBase *> lst;

	QMutexLocker locker(&platformListMutex);
	for (auto platform : platformList) {
		if (platform->getServiceType() == type) {
			lst.push_back(platform);
		}
	}

	return lst;
}

PLSPlatformBase *PLSPlatformApi::getPlatformById(const QString &which, const QVariantMap &info, bool bRemove)
{
	const auto channelUUID = info.value(ChannelData::g_channelUUID).toString().toUtf8();
	const auto channelType = info.value(ChannelData::g_data_type).toInt();
	const auto channelName = info.value(ChannelData::g_channelName).toString().toUtf8();
	const auto displayOrder = info.value(ChannelData::g_displayOrder).toInt();

	PLSPlatformBase *platform = nullptr;
	auto serviceType = PLSServiceType::ST_RTMP;
	if (!info.isEmpty()) {
		serviceType = getServiceType(info);
	}

	{
		QMutexLocker locker(&platformListMutex);
		for (auto item : platformList) {
			if (which == item->getChannelUUID() || (item->isSingleChannel() && item->getServiceType() == serviceType)) {
				platform = item;
				break;
			}
		}
	}

	if (nullptr == platform && !info.isEmpty() && !bRemove) {
		platform = buildPlatform(serviceType);
		/*if (PLSServiceType::ST_RTMP == serviceType) {
			platform = buildPlatform(serviceType);
		} else {
			platform = getPlatformByType(getServiceType(info));
		}*/
	}

	if (nullptr != platform && !info.isEmpty()) {
		platform->setInitData(info);
	}

	if (nullptr == platform && !bRemove) {
		assert(false);
		PLS_WARN(MODULE_PlatformService, __FUNCTION__ ".null: %s", which.toStdString().c_str());
	}
	if (!bRemove) {
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %p type=%d, name=%s, order=%d, uuid=%s", platform, channelType, channelName.constData(), displayOrder, channelUUID.constData());
	}

	return platform;
}

PLSPlatformBase *PLSPlatformApi::getPlatformByName(const QString &name)
{

	PLSPlatformBase *platform = nullptr;

	QMutexLocker locker(&platformListMutex);
	for (auto item : platformList) {
		if (name == item->getNameForLiveStart()) {
			platform = item;
			break;
		}
	}

	return platform;
}

PLSPlatformBase *PLSPlatformApi::getActivePlatformByName(const QString &name)
{

	PLSPlatformBase *platform = nullptr;

	QMutexLocker locker(&platformListMutex);
	for (auto item : platformList) {
		if (item->isActive() && name == item->getNameForLiveStart()) {
			platform = item;
			break;
		}
	}

	return platform;
}

PLSPlatformBase *PLSPlatformApi::getPlatformBySimulcastSeq(int simulcastSeq)
{
	PLSPlatformBase *platform = nullptr;

	QMutexLocker locker(&platformListMutex);
	for (auto item : platformList) {
		if (item->getChannelLiveSeq() == simulcastSeq) {
			platform = item;
			break;
		}
	}

	return platform;
}

bool PLSPlatformApi::isContainVliveChannel()
{
	bool conatinVlive = false;
	for (auto &info : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		const auto channelType = info.value(ChannelData::g_data_type).toInt();
		const auto channelName = info.value(ChannelData::g_channelName).toString();

		if (channelType == ChannelData::ChannelType && channelName == VLIVE) {
			conatinVlive = true;
			break;
		}
	}
	return conatinVlive;
}

bool PLSPlatformApi::isValidChannel(const QVariantMap &info)
{
	if (info.isEmpty()) {
		throw false;
	}

	const auto channelType = info.value(ChannelData::g_data_type).toInt();
	const auto channelName = info.value(ChannelData::g_channelName).toString();

	switch (channelType) {
	case ChannelData::RTMPType:
	case ChannelData::ChannelType:
		return true;
	default:
		break;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": false %d %s", channelType, channelName.toStdString().c_str());

	return false;
}

void PLSPlatformApi::ensureStopOutput()
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": %d %d", m_bRecording, m_liveStatus);
	m_bStopForExit = true;

	if (m_bRecording || (LiveStatus::LiveStarted <= m_liveStatus && m_liveStatus < LiveStatus::LiveStoped)) {
		QEventLoop loop;
		connect(this, &PLSPlatformApi::outputStopped, &loop, &QEventLoop::quit);

		if (m_bRecording) {
			PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": toStopRecord");
			PLSCHANNELS_API->toStopRecord();
		}
		if (LiveStatus::LiveStarted <= m_liveStatus && m_liveStatus < LiveStatus::LiveStoped) {
			PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": toStopBroadcast");
			PLSCHANNELS_API->toStopBroadcast();
		}

		QTimer::singleShot(10000, &loop, &QEventLoop::quit);

		loop.exec();
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ".end: %d %d", m_bRecording, m_liveStatus);
	}
}

void PLSPlatformApi::onFrontendEvent(enum obs_frontend_event event, void *private_data)
{
	PLSPlatformApi *self = reinterpret_cast<PLSPlatformApi *>(private_data);

	switch (event) {
	case OBS_FRONTEND_EVENT_STREAMING_STARTING:
		break;
	case OBS_FRONTEND_EVENT_STREAMING_STARTED:
		self->onLiveStarted();
		break;
	case OBS_FRONTEND_EVENT_STREAMING_STOPPING:
		break;
	case OBS_FRONTEND_EVENT_STREAMING_STOPPED:
		self->onLiveStopped();
		break;
	case OBS_FRONTEND_EVENT_RECORDING_STARTED:
		self->onRecordingStarted();
		break;
	case OBS_FRONTEND_EVENT_RECORDING_STOPPED:
		self->onRecordingStoped();
		break;
	case OBS_FRONTEND_EVENT_EXIT:
		self->ensureStopOutput();
		break;
	default:
		break;
	}
}

void PLSPlatformApi::showEndView(bool isRecord)
{
	bool isClickToStopRecord = PLSCHANNELS_API->getIsClickToStopRecord();
	bool isLivingAndRecording = false;
	//when stoping live sometime later but not stop complected, then start record, the keepRecordingWhenStreamStops is true.
	bool isRecordingButNotToStop = PLSCHANNELS_API->currentReocrdState() == ChannelData::RecordStarted || PLSCHANNELS_API->currentReocrdState() == ChannelData::RecordStarting;
	if (isRecord && PLS_PLATFORM_API->isLiving()) {
		isLivingAndRecording = true;
	} else if (!isRecord && PLS_PLATFORM_API->isRecording()) {
		isLivingAndRecording = true;
	}

	bool recordWhenStreaming = config_get_bool(GetGlobalConfig(), "BasicWindow", "RecordWhenStreaming");
	bool keepRecordingWhenStreamStops = config_get_bool(GetGlobalConfig(), "BasicWindow", "KeepRecordingWhenStreamStops");
	//when stop streaming will stop record sametime.
	bool isStreamingRecordStopAuto = recordWhenStreaming && !keepRecordingWhenStreamStops;
	bool isRehearsal = PLSCHANNELS_API->isRehearsaling();
	//when live and record stop sametime, will ignore record end page show and toast.
	static bool isIgnoreNextRecordShow = false;
	if (isStreamingRecordStopAuto && !isClickToStopRecord) {
		if (isRecord) {
			isIgnoreNextRecordShow = true;
		} else if (!isRecord && isLivingAndRecording && !isRecordingButNotToStop) {
			isIgnoreNextRecordShow = true;
		}
	}

	PLS_INFO(
		END_MODULE,
		"Show end with parameter \n\tisLivingAndRecording:%s, \n\tisShowRecordEnd:%s, \n\tisClickToStopRecord:%s, \n\tisStreamingRecordStopAuto:%s, \n\tisIgnoreNextRecordShow:%s, \n\tisRehaersaling:%s, \n\isRecordingButNotToStop:%s",
		BOOL2STR(isLivingAndRecording), BOOL2STR(isRecord), BOOL2STR(isClickToStopRecord), BOOL2STR(isStreamingRecordStopAuto), BOOL2STR(isIgnoreNextRecordShow), BOOL2STR(isRehearsal),
		BOOL2STR(isRecordingButNotToStop));

	if (isRecord) {
		if (isIgnoreNextRecordShow) {
			//ignore this end page;
			PLS_INFO(END_MODULE, "Show end with parameter ignore this record end page");
			isIgnoreNextRecordShow = false;
		} else if (isLivingAndRecording) {
			//record stoped, but live still streaming
			if (!isRehearsal) {
				// not rehaersal mode
				pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QObject::tr("main.message.error.recordwhenbroadcasting"));
			} else {
				// rehaersal mode
				pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QObject::tr("Live.Toast.Rehaersal.Record.Ended"));
			}
		} else {
			pls_toast_clear();
			//only record and live all stoped, to clear toast.
			if (PLSBasic::Get()->getMainView() != nullptr && PLSBasic::Get()->getMainView()->isVisible()) {
				auto mainwindw = PLSBasic::Get();
				PLSLiveEndDialog *dialog = new PLSLiveEndDialog(isRecord, mainwindw);
				dialog->exec();
				delete dialog;
			}
		}
		PLSCHANNELS_API->setIsClickToStopRecord(false);
		emit liveEndPageShowComplected(isRecord);
		return;
	}

	if (isLivingAndRecording && !isStreamingRecordStopAuto) {
		//live ended, but still recording
		if (isRehearsal) {
			pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QObject::tr("broadcast.end.rehearsal"));
		} else {
			pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QObject::tr("broadcast.end.live"));
		}
	} else {
		pls_toast_clear();
		//only record and live all stoped, to clear toast.
		if (!isRehearsal) {
			if (PLSBasic::Get()->getMainView() != nullptr && PLSBasic::Get()->getMainView()->isVisible()) {
				auto mainwindw = PLSBasic::Get();
				PLSLiveEndDialog *dialog = new PLSLiveEndDialog(isRecord, mainwindw);
				dialog->exec();
				delete dialog;
			}
		}
	}
	emit liveEndPageShowComplected(isRecord);
}

void PLSPlatformApi::doChannelInitialized()
{
	for (auto &info : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		const auto channelUUID = info.value(ChannelData::g_channelUUID).toString();
		const auto channelType = info.value(ChannelData::g_data_type).toInt();
		const auto channelName = info.value(ChannelData::g_channelName).toString();
		const auto displayOrder = info.value(ChannelData::g_displayOrder).toInt();

		PLS_INFO(MODULE_PlatformService, __FUNCTION__ ": type=%d, name=%s, order=%d, uuid=%s", channelType, channelName.toStdString().c_str(), displayOrder, channelUUID.toStdString().c_str());

		if (isValidChannel(info)) {
			onActive(channelUUID);
		} else {
			PLS_INFO(MODULE_PlatformService, __FUNCTION__ ".InvalidChannels: type=%d, name=%s, uuid=%s", channelType, channelName.toStdString().c_str(), channelUUID.toStdString().c_str());
		}
	}
}

void PLSPlatformApi::showStartMessageIfNeeded()
{
	PLSPlatformYoutube::showAutoStartFalseAlertIfNeeded();
}
