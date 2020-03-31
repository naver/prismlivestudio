#include "PLSPlatformFacebook.h"
#include <QVariantMap>
#include "PLSLiveInfoFacebook.h"
#include "PLSPlatformApi.h"
#include "../channels/ChannelsDataApi/PLSChannelDataAPI.h"
#include "../PLSLiveInfoDialogs.h"
#include "../../frontend-api/alert-view.hpp"

const QString TimelinePublicId = "{value:'EVERYONE'}";
const QString TimelineFriendId = "{value:'ALL_FRIENDS'}";
const QString TimelineOnlymeId = "{value:'SELF'}";
const QString FacebookShareObjectName_Key = "FacebookShareObjectName_Key";
const QString FacebookPrivacyId_Key = "FacebookPrivacyId_Key";
const QString FacebookPrivacyItemType = "FacebookPrivacyItemType";
const QString FacebookTimelineItemId = "me";
const QString FacebookGroupItemType = "FacebookGroupItemType";
const QString FacebookGroupId_Key = "FacebookGroupId_Key";
const QString FacebookPageItemType = "FacebookPageItemType";
const QString FacebookPageId_Key = "FacebookPageId_Key";
const QString FacebookLiveTitle_Key = "FacebookLiveTitle_Key";
const QString FacebookLiveDescription_Key = "FacebookLiveDescription_Key";
const QString FacebookLiveGameItemType = "FacebookLiveGameItemType";
const QString FacebookGameItemType = "FacebookGameItemType";
const QString FacebookLiveGameName_Key = "FacebookLiveGameName_Key";
const QString FacebookListName_Key = "FacebookListName_Key";
const QString FacebookListId_Key = "FacebookListId_Key";
const QString FacebookListToken_Key = "FacebookListToken_Key";
const QString FacebookListProfile_Key = "FacebookListProfile_Key";
const QString FacebookLiveId_Key = "FacebookLiveId_Key";
const QString FacebookVideoId_Key = "FacebookVideoId_Key";
const QString FacebookLiveUrl_Key = "FacebookLiveUrl_Key";
const QString FacebookShareLink_Key = "FacebookShareLink_Key";
const QString FacebookLiveStatus_Key = "FacebookLiveStatus_Key";

PLSPlatformFacebook::PLSPlatformFacebook() : PLSPlatformBase()
{
	connect(
		PLS_PLATFORM_API, &PLSPlatformApi::liveEndedForUi, this, [=]() { resetLiveInfo(); }, Qt::DirectConnection);
}

void PLSPlatformFacebook::initInfo(const QVariantMap &srcInfo)
{
	m_srcInfo = srcInfo;
	clearInfoData();
}

const QVariantMap &PLSPlatformFacebook::getSrcInfo()
{
	return m_srcInfo;
}

void PLSPlatformFacebook::insertSrcInfo(QString key, QVariant value)
{
	m_srcInfo[key] = value;
	if (key == ChannelData::g_nickName) {
		m_timelineNickname = value.toString();
	} else if (key == ChannelData::g_userIconCachePath) {
		m_timelineProfilePath = value.toString();
	}
}

QString PLSPlatformFacebook::getAccessToken() const
{
	auto channelInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	auto accessToken = channelInfo[ChannelData::g_channelToken].toString();
	return accessToken;
}

QString PLSPlatformFacebook::getGameId(const QString &gameName) const
{
	for (int i = 0; i < m_gameList.size(); i++) {
		QMap<QString, QString> map = m_gameList.at(i);
		if (map.value(FacebookListName_Key) == gameName) {
			return map.value(FacebookListId_Key);
		}
	}
	return QString();
}

const QStringList &PLSPlatformFacebook::getShareObjectList() const
{
	return m_shareObjectList;
}

QList<QString> PLSPlatformFacebook::getItemNameList(const QString &itemType) const
{
	QList<QMap<QString, QString>> list;
	if (itemType == FacebookGameItemType) {
		list = m_gameList;
	} else if (itemType == FacebookGroupItemType) {
		list = m_groupList;
	} else if (itemType == FacebookPageItemType) {
		list = m_pageList;
	} else if (itemType == FacebookPrivacyItemType) {
		list = m_timelinePrivacyList;
	}
	QList<QString> name_list;
	for (int i = 0; i < list.size(); i++) {
		QMap<QString, QString> map = list.at(i);
		name_list.insert(i, map.value(FacebookListName_Key));
	}
	return name_list;
}

QList<QString> PLSPlatformFacebook::getItemIdList(const QString &itemType) const
{
	QList<QMap<QString, QString>> list;
	if (itemType == FacebookGameItemType) {
		list = m_gameList;
	} else if (itemType == FacebookGroupItemType) {
		list = m_groupList;
	} else if (itemType == FacebookPageItemType) {
		list = m_pageList;
	} else if (itemType == FacebookPrivacyItemType) {
		list = m_timelinePrivacyList;
	}
	QList<QString> id_list;
	for (int i = 0; i < list.size(); i++) {
		QMap<QString, QString> map = list.at(i);
		id_list.insert(i, map.value(FacebookListId_Key));
	}
	return id_list;
}

QString PLSPlatformFacebook::getItemName(const QString &itemId, const QString &itemType)
{
	QList<QMap<QString, QString>> list;
	if (itemType == FacebookGameItemType) {
		list = m_gameList;
	} else if (itemType == FacebookGroupItemType) {
		list = m_groupList;
	} else if (itemType == FacebookPageItemType) {
		list = m_pageList;
	} else if (itemType == FacebookPrivacyItemType) {
		list = m_timelinePrivacyList;
	}
	if (itemId == GROUPLIST_DEFAULT_ITEM_ID && itemType == FacebookGroupItemType) {
		return GROUPLIST_DEFAULT_SELECT_TEXT;
	} else if (itemId == PAGELIST_DEFAULT_ITEM_ID && itemType == FacebookPageItemType) {
		return PAGELIST_DEFAULT_SELECT_TEXT;
	}
	for (int i = 0; i < list.size(); i++) {
		QMap<QString, QString> map = list.at(i);
		if (map.value(FacebookListId_Key) == itemId) {
			QString name = map.value(FacebookListName_Key);
			return name;
		}
	}
	return QString();
}

PLSServiceType PLSPlatformFacebook::getServiceType() const
{
	return PLSServiceType::ST_FACEBOOK;
}

void PLSPlatformFacebook::onPrepareLive(bool value)
{
	if (!value) {
		prepareLiveCallback(value);
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ "show liveinfo");
	value = pls_exec_live_Info_facebook(getChannelUUID(), getInitData()) == QDialog::Accepted;
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ "liveinfo closed with:%d", value);
	prepareLiveCallback(value);
}

void PLSPlatformFacebook::onAlLiveStarted(bool value)
{
	if (!value || !isActive()) {
		return;
	}

	//start update statistics
	if (!m_statusTimer) {
		m_statusTimer = new QTimer(this);
		connect(m_statusTimer, &QTimer::timeout, this, &PLSPlatformFacebook::timerAction);
		m_statusTimer->start(5000);
	}

	if (m_liveStartShowPermissionToast == true) {
		m_liveStartShowPermissionToast = false;
		pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QTStr("facebook.living.privacy.inconsistent.permissions"));
	}
}

void PLSPlatformFacebook::onAllPrepareLive(bool value)
{
	if (!value) {
		updateChannelInfo(ChannelData::g_shareUrl, "", true);
	}
	PLSPlatformBase::onAllPrepareLive(value);
}

void PLSPlatformFacebook::onLiveStopped()
{
	if (m_statusTimer && m_statusTimer->isActive()) {
		m_statusTimer->stop();
		m_statusTimer = nullptr;
	}
	auto callBack = [=](PLSAPIFacebookType type) { liveStoppedCallback(); };
	PLSFaceBookRquest->stopFacebookLiving(PLSAPIFacebook::PLSAPIStopFacebookLiving, getLiveInfoValue(FacebookLiveId_Key), callBack);
	if (m_facebookLivingExpired) {
		PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_channelStatus, ChannelData::Expired);
	}
}

void PLSPlatformFacebook::getLongLivedUserAccessToken(MyRequestTypeFunction onFinished)
{
	auto CallBack = [=](PLSAPIFacebookType type) {
		if (type == PLSAPIFacebookType::PLSFacebookInvalidAccessToken) {
			onFinished(type);
			return;
		}
		getUserInfo(onFinished);
	};
	PLSFaceBookRquest->getLongLiveUserAccessToken(PLSAPIFacebook::PLSAPIGetLongLiveUserAccessToken, CallBack);
}

void PLSPlatformFacebook::getUserInfo(MyRequestTypeFunction onFinished)
{
	auto CallBack = [=](PLSAPIFacebookType type) {
		if (type != PLSAPIFacebookType::PLSFacebookSuccess && type != PLSAPIFacebookType::PLSFacebookInvalidAccessToken) {
			insertSrcInfo(ChannelData::g_channelStatus, ChannelData::ChannelStatus::Error);
			if (type == PLSAPIFacebookType::PLSFacebookNetworkError) {
				insertSrcInfo(ChannelData::g_errorType, ChannelData::NetWorkErrorType::NetWorkNoStable);
			} else {
				insertSrcInfo(ChannelData::g_errorType, ChannelData::NetWorkErrorType::UnknownError);
			}
		}
		onFinished(type);
	};
	PLSFaceBookRquest->getUserInfo(PLSAPIFacebook::PLSAPIGetUserInfo, CallBack);
}

void PLSPlatformFacebook::getMyGroupListRequestAndCheckPermission(MyRequestTypeFunction onFinished, QWidget *parent)
{
	PLSFaceBookRquest->getMyGroupListRequestAndCheckPermission(PLSAPIFacebook::PLSAPIGetMyGroupListRequest, m_groupList, onFinished, parent);
}

void PLSPlatformFacebook::getMyPageListRequestAndCheckPermission(MyRequestTypeFunction onFinished, QWidget *parent)
{
	PLSFaceBookRquest->getMyPageListRequestAndCheckPermission(PLSAPIFacebook::PLSAPIGetMyPageListRequest, m_pageList, onFinished, parent);
}

void PLSPlatformFacebook::getGameTagListByKeyword(MyRequestTypeFunction onFinished, const QString &keyword)
{
	PLSFaceBookRquest->searchGameTagListByKeyword(PLSAPIFacebook::PLSAPIGetMyGameListRequest, m_gameList, onFinished, keyword);
}

void PLSPlatformFacebook::startLiving(QMap<QString, QString> info, MyRequestTypeFunction onFinished)
{
	m_liveStartShowPermissionToast = false;
	m_facebookLivingExpired = false;
	QString title = info.value(FacebookLiveTitle_Key);
	__super::setTitle(title.toStdString());
	QString description = info.value(FacebookLiveDescription_Key);
	QString encodePrivacy = info.value(FacebookPrivacyId_Key);
	QString gameName = info.value(FacebookLiveGameName_Key);
	QString gameId = getGameId(gameName);
	QString shareObjectName = info.value(FacebookShareObjectName_Key);
	QString accessToken;
	QString itemId;
	PLSAPIFacebook::PLSAPI apiType;
	if (shareObjectName == TimelineObjectFlags) {
		itemId = FacebookTimelineItemId;
		accessToken = getAccessToken();
		apiType = PLSAPIFacebook::PLSAPIStartTimelineLiving;
	} else if (shareObjectName == GroupObjectFlags) {
		itemId = info.value(FacebookGroupId_Key);
		accessToken = getAccessToken();
		apiType = PLSAPIFacebook::PLSAPIStartGroupLiving;
	} else if (shareObjectName == PageObjectFlags) {
		itemId = info.value(FacebookPageId_Key);
		for (int i = 0; i < m_pageList.size(); i++) {
			QMap<QString, QString> map = m_pageList.at(i);
			if (map.value(FacebookListId_Key) == itemId) {
				accessToken = map.value(FacebookListToken_Key);
			}
		}
		apiType = PLSAPIFacebook::PLSAPIStartPageLiving;
	}
	auto livingFinished = [=](PLSAPIFacebookType type) {
		if (type == PLSAPIFacebookType::PLSFacebookSuccess) {
			setStreamUrlAndStreamKey();
			if (shareObjectName != TimelineObjectFlags) {
				requestItemInfoRequest(info, onFinished);
				return;
			}
			getTimelinePrivacyRequest(info, onFinished);
			return;
		}
		onFinished(type);
	};
	PLSFaceBookRquest->startLiving(apiType, itemId, title, description, encodePrivacy, gameId, accessToken, livingFinished);
}

void PLSPlatformFacebook::updateLiving(QMap<QString, QString> info, MyRequestTypeFunction onFinished)
{
	QString title = info.value(FacebookLiveTitle_Key);
	QString description = info.value(FacebookLiveDescription_Key);
	QString encodePrivacy = info.value(FacebookPrivacyId_Key);
	QString gameName = info.value(FacebookLiveGameName_Key);
	QString gameId = getGameId(gameName);
	QString shareObjectName = info.value(FacebookShareObjectName_Key);
	auto updateFinished = [=](PLSAPIFacebookType type) {
		if (type == PLSAPIFacebookType::PLSFacebookSuccess) {
			if (shareObjectName != TimelineObjectFlags) {
				requestItemInfoRequest(info, onFinished);
				return;
			}
			getTimelinePrivacyRequest(info, onFinished);
			return;
		}
		onFinished(type);
	};
	QString liveVideoId = getLiveInfoValue(FacebookLiveId_Key);
	PLSFaceBookRquest->updateFacebookLiving(PLSAPIFacebook::PLSAPIUpdateFacebookLiving, liveVideoId, title, description, encodePrivacy, gameId, updateFinished);
}

void PLSPlatformFacebook::requestItemInfoRequest(QMap<QString, QString> requestInfo, MyRequestTypeFunction onFinished)
{
	QString shareObjectName = requestInfo.value(FacebookShareObjectName_Key);
	QString itemId;
	if (shareObjectName == TimelineObjectFlags) {
		itemId = FacebookTimelineItemId;
	} else if (shareObjectName == GroupObjectFlags) {
		itemId = requestInfo.value(FacebookGroupId_Key);
	} else if (shareObjectName == PageObjectFlags) {
		itemId = requestInfo.value(FacebookPageId_Key);
	}

	auto itemInfoFinished = [=](PLSAPIFacebookType type, QString nickname, QString profilePath) {
		if (type == PLSAPIFacebookType::PLSFacebookSuccess) {

			//update liveInfo
			QMap<QString, QString>::const_iterator i;
			for (i = requestInfo.constBegin(); i != requestInfo.constEnd(); ++i) {
				insertLiveInfo(i.key(), i.value());
			}

			//update dashboard
			QVariantMap map;
			map.insert(ChannelData::g_nickName, nickname);
			map.insert(ChannelData::g_userIconCachePath, profilePath);
			if (shareObjectName == TimelineObjectFlags) {
				QString privacyId = getLiveInfoValue(FacebookPrivacyId_Key);
				QString privacyName = getItemName(privacyId, FacebookPrivacyItemType);
				map.insert(ChannelData::g_catogry, privacyName);
				m_timelineNickname = nickname;
				m_timelineProfilePath = profilePath;
			} else if (shareObjectName == GroupObjectFlags || shareObjectName == PageObjectFlags) {
				QString gameName = getLiveInfoValue(FacebookLiveGameName_Key);
				QString gameId = getGameId(gameName);
				if (gameId.size() > 0) {
					map.insert(ChannelData::g_catogry, gameName);
				} else {
					map.insert(ChannelData::g_catogry, "");
				}
			}
			updateChannelInfos(map, true);
		}
		onFinished(type);
	};
	PLSFaceBookRquest->getFacebookItemUserInfo(PLSAPIFacebook::PLSAPIGetFacebookItemUserInfo, itemId, itemInfoFinished);
}

void PLSPlatformFacebook::updateChannelInfo(const QString &key, const QVariant &value, bool refresh)
{
	auto latInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	latInfo.insert(key, value);
	PLSCHANNELS_API->setChannelInfos(latInfo, refresh);
}

void PLSPlatformFacebook::updateChannelInfos(const QVariantMap &channelInfos, bool refresh)
{
	auto latInfo = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	QVariantMap::const_iterator i;
	for (i = channelInfos.constBegin(); i != channelInfos.constEnd(); ++i) {
		latInfo.insert(i.key(), i.value());
	}
	PLSCHANNELS_API->setChannelInfos(latInfo, refresh);
}

void PLSPlatformFacebook::insertLiveInfo(const QString &key, const QVariant &value)
{
	m_liveInfo[key] = value;
}

QString PLSPlatformFacebook::getLiveInfoValue(const QString &key)
{
	return m_liveInfo.value(key).toString();
}

void PLSPlatformFacebook::resetLiveInfo()
{
	clearInfoData();
	QVariantMap updateInfoMap;
	updateInfoMap.insert(ChannelData::g_nickName, m_timelineNickname);
	updateInfoMap.insert(ChannelData::g_userIconCachePath, m_timelineProfilePath);
	QString privacyId = getLiveInfoValue(FacebookPrivacyId_Key);
	QString privacyName = getItemName(privacyId, FacebookPrivacyItemType);
	updateInfoMap.insert(ChannelData::g_catogry, privacyName);
	updateInfoMap.insert(ChannelData::g_shareUrl, "");
	updateInfoMap.insert(ChannelData::g_likes, "0");
	updateInfoMap.insert(ChannelData::g_viewers, "0");
	updateInfoMap.insert(ChannelData::g_comments, "0");
	updateChannelInfos(updateInfoMap, false);
}

void PLSPlatformFacebook::setStreamUrlAndStreamKey()
{
	QString pushUrl = getLiveInfoValue(FacebookLiveUrl_Key);
	QStringList list = pushUrl.split("/");
	QString streamKey = list.last();
	list.removeLast();
	QString streamUrl = list.join("/");
	setStreamKey(streamKey.toStdString());
	setStreamServer(streamUrl.toStdString());
}

void PLSPlatformFacebook::getLivingVideoInfo(MyRequestTypeFunction onFinished)
{
	QString liveId = getLiveInfoValue(FacebookLiveId_Key);
	PLSFaceBookRquest->getLiveVideoInfoRequest(PLSAPIFacebook::PLSAPIGetLiveVideoInfoRequest, liveId, "title, description", onFinished);
}

void PLSPlatformFacebook::timerAction()
{
	requestStatisticsInfo();
	requestTimePrivacyInfo();
}

void PLSPlatformFacebook::requestStatisticsInfo()
{
	auto infoFinished = [=](PLSAPIFacebookType type) {
		QString liveStatus = getLiveInfoValue(FacebookLiveStatus_Key);
		bool liveEnd = (liveStatus == "LIVE_STOPPED" || liveStatus == "VOD" || liveStatus == "PROCESSING");
		if (type == PLSAPIFacebookType::PLSFacebookInvalidAccessToken || type == PLSAPIFacebookType::PLSFacebookObjectDontExist || liveEnd) {
			if (m_statusTimer && m_statusTimer->isActive()) {
				m_statusTimer->stop();
				m_statusTimer = nullptr;
			}
			//token expired
			if (type == PLSAPIFacebookType::PLSFacebookInvalidAccessToken) {
				pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QTStr("facebook.living.login.token.expired"));
				PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_channelStatus, ChannelData::Expired);
				m_facebookLivingExpired = true;
				return;
			}
			if (PLS_PLATFORM_API->isPrismLive()) {
				return;
			}
			const auto channelName = getChannelName();
			if (PLS_PLATFORM_API->getActivePlatforms().size() == 1) {
				PLSAlertView::warning(PLSBasic::Get(), QTStr("Live.Check.Alert.Title"), QTStr("MQTT.Aborted.Error").arg(channelName));
				PLSCHANNELS_API->toStopBroadcast();
			} else {
				pls_toast_message(pls_toast_info_type::PLS_TOAST_ERROR, QTStr("MQTT.Aborted.Error").arg(channelName));
				PLSCHANNELS_API->setChannelStatus(getChannelUUID(), ChannelData::Error);
			}
		}
	};
	QString liveId = getLiveInfoValue(FacebookLiveId_Key);
	PLSFaceBookRquest->getLiveVideoInfoRequest(PLSAPIFacebook::PLSAPIGetLiveVideoInfoRequest, liveId, "live_views,comments.limit(0).summary(true),reactions.limit(0).summary(total_count),status",
						   infoFinished);
}

void PLSPlatformFacebook::requestTimePrivacyInfo()
{
	QString shareObjectName = getLiveInfoValue(FacebookShareObjectName_Key);
	if (shareObjectName != TimelineObjectFlags) {
		return;
	}

	auto privacyFinished = [=](PLSAPIFacebookType type, QString privacyId) {
		if (type != PLSAPIFacebookType::PLSFacebookSuccess) {
			return;
		}
		m_timelinePrivacyId = privacyId;
		insertLiveInfo(FacebookPrivacyId_Key, privacyId);
		updatePrivateChat();
	};
	QString videoId = getLiveInfoValue(FacebookVideoId_Key);
	PLSFaceBookRquest->getTimelinePrivacyRequest(PLSAPIFacebook::PLSAPIGetLiveVideoPrivacyRequest, videoId, privacyFinished);
}

void PLSPlatformFacebook::insertParentPointer(PLSLiveInfoFacebook *parent)
{
	m_parent = parent;
}

void PLSPlatformFacebook::insertUserId(const QString &user_id)
{
	m_userId = user_id;
}

const PLSLiveInfoFacebook *PLSPlatformFacebook::getParentPointer()
{
	return m_parent;
}

QJsonObject PLSPlatformFacebook::getWebChatParams()
{
	QJsonObject platform;
	platform["name"] = "FACEBOOK";
	m_privateChat = isPrivateChat();
	platform["isPrivate"] = m_privateChat;
	platform["userId"] = m_userId;
	return platform;
}

QJsonObject PLSPlatformFacebook::getLiveStartParams()
{
	QJsonObject platform;
	bool isPrivate = false;
	QString shareObjectName = getLiveInfoValue(FacebookShareObjectName_Key);
	if (shareObjectName == TimelineObjectFlags) {
		QString privacy = getLiveInfoValue(FacebookPrivacyId_Key);
		if (privacy == TimelinePublicId) {
			platform["privacyStatus"] = "PUBLIC";
		} else if (privacy == TimelineFriendId) {
			platform["privacyStatus"] = "FRIEND";
		} else if (privacy == TimelineOnlymeId) {
			isPrivate = true;
			platform["privacyStatus"] = "PRIVATE";
		}
		platform["target"] = "TIMELINE";
		platform.insert("accessToken", getChannelToken());
	} else if (shareObjectName == GroupObjectFlags) {
		platform["target"] = "GROUP";
		platform.insert("accessToken", getChannelToken());
	} else if (shareObjectName == PageObjectFlags) {
		platform["target"] = "PAGE";
		QString pageId = getLiveInfoValue(FacebookPageId_Key);
		QString pageAccessToken;
		for (int i = 0; i < m_pageList.size(); i++) {
			QMap<QString, QString> map = m_pageList.at(i);
			if (map.value(FacebookListId_Key) == pageId) {
				pageAccessToken = map.value(FacebookListToken_Key);
			}
		}
		platform.insert("accessToken", pageAccessToken);
	}
	platform["isPrivate"] = false;
	platform["broadcastId"] = getLiveInfoValue(FacebookLiveId_Key);
	const auto &channelData = PLSCHANNELS_API->getChanelInfoRef(getChannelUUID());
	platform["simulcastChannel"] = channelData.value(ChannelData::g_nickName, "").toString();
	return platform;
}

void PLSPlatformFacebook::resetLiveInfoGroupId()
{
	insertLiveInfo(FacebookGroupId_Key, GROUPLIST_DEFAULT_ITEM_ID);
}

void PLSPlatformFacebook::resetLiveInfoPageId()
{
	insertLiveInfo(FacebookPageId_Key, PAGELIST_DEFAULT_ITEM_ID);
}

bool PLSPlatformFacebook::isPrivateChat()
{
	bool isPrivate = false;
	QString shareObjectName = getLiveInfoValue(FacebookShareObjectName_Key);
	if (shareObjectName == TimelineObjectFlags) {
		QString privacy = getLiveInfoValue(FacebookPrivacyId_Key);
		if (privacy == TimelineOnlymeId || privacy == TimelineFriendId) {
			isPrivate = true;
		}
	} else if (shareObjectName == GroupObjectFlags) {
		isPrivate = true;
	}
	return isPrivate;
}

void PLSPlatformFacebook::updatePrivateChat()
{
	bool newPrivate = isPrivateChat();
	if (m_privateChat != newPrivate) {

	}
	m_privateChat = newPrivate;
}

void PLSPlatformFacebook::getTimelinePrivacyRequest(QMap<QString, QString> requestInfo, MyRequestTypeFunction onFinished)
{
	auto userInfoFinished = [=](PLSAPIFacebookType type) {
		if (type == PLSAPIFacebookType::PLSFacebookSuccess) {
			insertLiveInfo(FacebookPrivacyId_Key, m_timelinePrivacyId);
		}
		onFinished(type);
	};
	auto privacyFinished = [=](PLSAPIFacebookType type, QString privacyId) {
		if (type == PLSAPIFacebookType::PLSFacebookSuccess) {
			if (privacyId != requestInfo.value(FacebookPrivacyId_Key)) {
				if (PLSCHANNELS_API->isLiving()) {
					pls_toast_message(pls_toast_info_type::PLS_TOAST_NOTICE, QTStr("facebook.living.privacy.inconsistent.permissions"));
				} else {
					m_liveStartShowPermissionToast = true;
				}
			}
			m_timelinePrivacyId = privacyId;
			requestItemInfoRequest(requestInfo, userInfoFinished);
			return;
		}
		onFinished(type);
	};
	QString videoId = getLiveInfoValue(FacebookVideoId_Key);
	PLSFaceBookRquest->getTimelinePrivacyRequest(PLSAPIFacebook::PLSAPIGetLiveVideoPrivacyRequest, videoId, privacyFinished);
}

QString PLSPlatformFacebook::getServiceLiveLink()
{
	auto info = PLSCHANNELS_API->getChannelInfo(getChannelUUID());
	auto shareUrl = info.value(ChannelData::g_shareUrl).toString();
	return shareUrl;
}

void PLSPlatformFacebook::setPrivacyToTimeline()
{
	QVariantMap updateInfoMap;
	insertLiveInfo(FacebookShareObjectName_Key, TimelineObjectFlags);
	insertLiveInfo(FacebookPrivacyId_Key, TimelinePublicId);
	updateInfoMap.insert(ChannelData::g_nickName, m_timelineNickname);
	updateInfoMap.insert(ChannelData::g_userIconCachePath, m_timelineProfilePath);
	QString privacyId = getLiveInfoValue(FacebookPrivacyId_Key);
	QString privacyName = getItemName(privacyId, FacebookPrivacyItemType);
	updateInfoMap.insert(ChannelData::g_catogry, privacyName);
	updateChannelInfos(updateInfoMap, true);
}

void PLSPlatformFacebook::clearInfoData()
{
	m_liveInfo.clear();
	m_groupList.clear();
	m_pageList.clear();
	m_gameList.clear();
	m_shareObjectList.clear();
	m_shareObjectList.append(TimelineObjectFlags);
	m_shareObjectList.append(GroupObjectFlags);
	m_shareObjectList.append(PageObjectFlags);
	m_timelinePrivacyList.clear();
	QList<QString> privacyNames;
	privacyNames.append(TimelinePublicName);
	privacyNames.append(TimelineFriendName);
	privacyNames.append(TimelineOnlymeName);
	QList<QString> privacyIds;
	privacyIds.append(TimelinePublicId);
	privacyIds.append(TimelineFriendId);
	privacyIds.append(TimelineOnlymeId);
	for (int i = 0; i < privacyNames.size(); i++) {
		QString name = privacyNames.at(i);
		QString idStr = privacyIds.at(i);
		QMap<QString, QString> map;
		map.insert(FacebookListName_Key, name);
		map.insert(FacebookListId_Key, idStr);
		m_timelinePrivacyList.insert(i, map);
	}
	insertLiveInfo(FacebookShareObjectName_Key, TimelineObjectFlags);
	insertLiveInfo(FacebookPrivacyId_Key, TimelinePublicId);
	resetLiveInfoGroupId();
	resetLiveInfoPageId();
}
