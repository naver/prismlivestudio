/*
* @file		PLSPlatformFacebook.h
* @brief	All facebook relevant api is implemented in this file
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
*/

#pragma once

#include "../PLSPlatformBase.hpp"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "PLSAPIFacebook.h"

#define MODULE_PLATFORM_FACEBOOK "Platform/Facebook"
#define GROUPLIST_DEFAULT_SELECT_TEXT tr("facebook.liveinfo.Select.Group")
#define GROUPLIST_DEFAULT_ITEM_ID "-1001"
#define PAGELIST_DEFAULT_SELECT_TEXT tr("facebook.liveinfo.Select.Page")
#define PAGELIST_DEFAULT_ITEM_ID "-2001"

#define TimelinePublicName tr("facebook.privacy.type.all")
#define TimelineFriendName tr("facebook.privacy.type.all.friends")
#define TimelineOnlymeName tr("facebook.privacy.type.self")
#define TimelineObjectFlags tr("facebook.channel.type.timeline")
#define GroupObjectFlags tr("facebook.channel.type.group")
#define PageObjectFlags tr("facebook.channel.type.page")

extern const QString TimelinePublicId;
extern const QString TimelineFriendId;
extern const QString TimelineOnlymeId;
extern const QString FacebookShareObjectName_Key;
extern const QString FacebookPrivacyId_Key;
extern const QString FacebookPrivacyItemType;
extern const QString FacebookTimelineItemId;
extern const QString FacebookGroupItemType;
extern const QString FacebookGroupId_Key;
extern const QString FacebookPageItemType;
extern const QString FacebookPageId_Key;
extern const QString FacebookLiveTitle_Key;
extern const QString FacebookLiveDescription_Key;
extern const QString FacebookGameItemType;
extern const QString FacebookLiveGameName_Key;
extern const QString FacebookListName_Key;
extern const QString FacebookListId_Key;
extern const QString FacebookListToken_Key;
extern const QString FacebookListProfile_Key;
extern const QString FacebookLiveId_Key;
extern const QString FacebookVideoId_Key;
extern const QString FacebookLiveUrl_Key;
extern const QString FacebookShareLink_Key;
extern const QString FacebookLiveStatus_Key;

using namespace std;

class PLSLiveInfoFacebook;

class PLSPlatformFacebook : public PLSPlatformBase {

	Q_OBJECT
public:
	PLSPlatformFacebook();
	void initInfo(const QVariantMap &srcInfo);
	const QVariantMap &getSrcInfo();
	void insertSrcInfo(QString key, QVariant value);
	QString getAccessToken() const;
	QString getGameId(const QString &gameName) const;
	const QStringList &getShareObjectList() const;
	QList<QString> getItemNameList(const QString &itemType) const;
	QList<QString> getItemIdList(const QString &itemType) const;
	QString getItemName(const QString &itemId, const QString &itemType);
	PLSServiceType getServiceType() const override;
	void onPrepareLive(bool value) override;
	void onAlLiveStarted(bool) override;
	void onAllPrepareLive(bool value);
	void onLiveStopped() override;
	void getLongLivedUserAccessToken(MyRequestTypeFunction onFinished);
	void getUserInfo(MyRequestTypeFunction onFinished);
	void getMyGroupListRequestAndCheckPermission(MyRequestTypeFunction onFinished, QWidget *parent);
	void getMyPageListRequestAndCheckPermission(MyRequestTypeFunction onFinished, QWidget *parent);
	void getGameTagListByKeyword(MyRequestTypeFunction onFinished, const QString &keyword);
	void startLiving(QMap<QString, QString> requestInfo, MyRequestTypeFunction onFinished);
	void updateLiving(QMap<QString, QString> info, MyRequestTypeFunction onFinished);
	void requestItemInfoRequest(QMap<QString, QString> requestInfo, MyRequestTypeFunction onFinished);
	void updateChannelInfo(const QString &key, const QVariant &value, bool refresh);
	void updateChannelInfos(const QVariantMap &channelInfos, bool refresh);
	void insertLiveInfo(const QString &key, const QVariant &value);
	QString getLiveInfoValue(const QString &key);
	void resetLiveInfo();
	void setStreamUrlAndStreamKey();
	void getLivingVideoInfo(MyRequestTypeFunction onFinished);
	void insertParentPointer(PLSLiveInfoFacebook *parent);
	void insertUserId(const QString &user_id);
	const PLSLiveInfoFacebook *getParentPointer();
	QJsonObject getWebChatParams() override;
	QJsonObject getLiveStartParams() override;
	void resetLiveInfoGroupId();
	void resetLiveInfoPageId();
	bool isPrivateChat();
	void updatePrivateChat();
	void getTimelinePrivacyRequest(QMap<QString, QString> requestInfo, MyRequestTypeFunction onFinished);
	QString getServiceLiveLink() override;
	void setPrivacyToTimeline();

signals:
	void privateChatChanged(bool oldPrivate, bool newPrivate);

private:
	void timerAction();
	void requestStatisticsInfo();
	void requestTimePrivacyInfo();
	void clearInfoData();

private:
	QStringList m_shareObjectList;
	QList<QMap<QString, QString>> m_timelinePrivacyList;
	QList<QMap<QString, QString>> m_groupList;
	QList<QMap<QString, QString>> m_pageList;
	QList<QMap<QString, QString>> m_gameList;
	QVariantMap m_srcInfo;
	QVariantMap m_liveInfo;
	QPointer<QTimer> m_statusTimer;
	QPointer<PLSLiveInfoFacebook> m_parent;
	QString m_timelineNickname;
	QString m_timelineProfilePath;
	bool m_privateChat;
	QString m_timelinePrivacyId;
	QString m_userId;
	bool m_liveStartShowPermissionToast;
	bool m_facebookLivingExpired;
};
