#include "PLSAPIFacebook.h"
#include <QUrl>
#include <QMap>
#include <QUrlQuery>
#include <QPair>
#include <QMetaEnum>
#include "PLSLiveInfoFacebook.h"
#include "../PLSPlatformBase.hpp"
#include "../ChannelsDataApi/ChannelConst.h"
#include "../PLSPlatformApi.h"
#include "../vlive/PLSAPIVLive.h"
#include "../../pls-common-define.hpp"
#include "../../frontend-api/frontend-api.h"
#include "json-data-handler.hpp"

#define page_list_permission "pages_show_list"
#define granted_status "granted"
#define declined_status "declined"
#define granted_scopes "granted_scopes"

PLSAPIFacebook *PLSAPIFacebook::instance()
{
	static PLSAPIFacebook syncServerManager;
	return &syncServerManager;
}

void PLSAPIFacebook::getLongLiveUserAccessToken(PLSAPI requestType, MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	PLSNetworkReplyBuilder builder(getFaceboolURL("oauth/access_token"));
	builder.addQuery(HTTP_CLIENT_ID, CHANNEL_FACEBOOK_CLIENT_ID);
	builder.addQuery(HTTP_CLIENT_SECRET, CHANNEL_FACEBOOK_SECRET);
	builder.addQuery("grant_type", "fb_exchange_token");
	builder.addQuery("fb_exchange_token", facebook->getAccessToken());
	auto SuccessCallBack = [=](QJsonObject root) {
		QString accessToken = root.value("access_token").toString();
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success: %s", getApiName(requestType), accessToken.toUtf8().constData());
		if (accessToken.length() > 0) {
			facebook->insertSrcInfo(ChannelData::g_channelToken, accessToken);
			onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
			return;
		}
		onFinished(PLSAPIFacebookType::PLSFacebookFailed);
	};
	startRequestApi(requestType, builder.get(), SuccessCallBack, onFinished);
}

void PLSAPIFacebook::getUserInfo(PLSAPI requestType, MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL("me");
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	PLSNetworkReplyBuilder builder(url);
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken());
	builder.addQuery("fields", "name,picture.type(large)");
	auto SuccessCallBack = [=](QJsonObject root) {
		QString username = root.value(name2str(name)).toString();
		facebook->insertSrcInfo(ChannelData::g_nickName, username);
		QVariant url;
		PLSJsonDataHandler::getValue(root, name2str(url), url);
		facebook->insertSrcInfo(ChannelData::g_channelStatus, ChannelData::ChannelStatus::Valid);
		QString imagePath;
		downloadSyncImage(url.toString(), imagePath);
		if (imagePath.length() > 0) {
			facebook->insertSrcInfo(ChannelData::g_userIconCachePath, imagePath);
		}
		QString privacyId = facebook->getLiveInfoValue(FacebookPrivacyId_Key);
		QString privacyName = facebook->getItemName(privacyId, FacebookPrivacyItemType);
		QString userId = root.value(name2str(id)).toString();
		facebook->insertUserId(userId);
		facebook->insertSrcInfo(ChannelData::g_catogry, privacyName);
		facebook->insertSrcInfo(ChannelData::g_shareUrl, "");
		facebook->insertSrcInfo(ChannelData::g_likes, "0");
		facebook->insertSrcInfo(ChannelData::g_viewers, "0");
		facebook->insertSrcInfo(ChannelData::g_comments, "0");
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success: username=%s,imagePath=%s,url=%s", getApiName(requestType), username.toUtf8().constData(), imagePath.toUtf8().constData(),
			 url.toString().toUtf8().constData());
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.get(), SuccessCallBack, onFinished);
}

void PLSAPIFacebook::checkPermission(PLSAPI requestType, QStringList permissionList, MyRequestTypeFunction onFinished, QWidget *parent)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL("me/permissions");
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken());
	auto CallBack = [=](QJsonObject root) {
		bool hasPermission = false;
		QJsonArray json_array = root.value(name2str(data)).toArray();
		QList<QString> grantedList;
		for (int i = 0; i < json_array.size(); i++) {
			QJsonObject json = json_array.at(i).toObject();
			QString userPermission = json.value(name2str(permission)).toString();
			QString status = json.value(name2str(status)).toString();
			if (status == granted_status) {
				grantedList.append(userPermission);
			}
		}
		QSet<QString> intersection = grantedList.toSet().intersect(permissionList.toSet());
		if (intersection.size() == permissionList.size()) {
			PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook checkPermissionSuccess: PLSFacebookGranted__1");
			onFinished(PLSAPIFacebookType::PLSFacebookGranted);
		} else {
			bool granted = goFacebookRequestPermission(permissionList.join(","), parent);
			if (granted) {
				PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook checkPermissionSuccess: PLSFacebookGranted__2");
				onFinished(PLSAPIFacebookType::PLSFacebookGranted);
			} else {
				PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook checkPermissionSuccess: PLSFacebookDeclined");
				onFinished(PLSAPIFacebookType::PLSFacebookDeclined);
			}
		}
	};
	startRequestApi(requestType, builder.get(), CallBack, onFinished);
}

void PLSAPIFacebook::getMyGroupListRequestAndCheckPermission(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished, QWidget *parent)
{
	auto finished = [=, &list](PLSAPIFacebookType type) {
		if (type == PLSAPIFacebookType::PLSFacebookGranted) {
			getMyGroupListRequest(requestType, list, onFinished);
			return;
		}
		onFinished(type);
	};
	QStringList permissionList;
	permissionList << group_living_permission;
	checkPermission(PLSAPICheckMyGroupListPermission, permissionList, finished, parent);
}

void PLSAPIFacebook::getMyGroupListRequest(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL("me/groups");
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken());
	builder.addQuery("fields", "name,picture");
	auto CallBack = [=, &list](QJsonObject root) {
		QJsonArray json_array = root.value(name2str(data)).toArray();
		list.clear();
		for (int i = 0; i < json_array.size(); i++) {
			QJsonObject json = json_array.at(i).toObject();
			QString name = json.value(name2str(name)).toString();
			QString idStr = json.value(name2str(id)).toString();
			QVariant url;
			PLSJsonDataHandler::getValue(json, name2str(url), url);
			QMap<QString, QString> map;
			map.insert(FacebookListName_Key, name);
			map.insert(FacebookListId_Key, idStr);
			map.insert(FacebookListProfile_Key, url.toString());
			list.insert(i, map);
		}
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success: %d", getApiName(requestType), list.size());
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.get(), CallBack, onFinished);
}

void PLSAPIFacebook::getMyPageListRequestAndCheckPermission(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished, QWidget *parent)
{
	auto finished = [=, &list](PLSAPIFacebookType type) {
		if (type == PLSAPIFacebookType::PLSFacebookGranted) {
			getMyPageListRequest(requestType, list, onFinished);
		} else {
			onFinished(type);
		}
	};
	QStringList permissionList;
	permissionList << page_list_permission;
	checkPermission(PLSAPICheckMyPageListPermission, permissionList, finished, parent);
}

void PLSAPIFacebook::getMyPageListRequest(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL("me/accounts");
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken());
	auto CallBack = [=, &list](QJsonObject root) {
		QJsonArray json_array = root.value(name2str(data)).toArray();
		list.clear();
		for (int i = 0; i < json_array.size(); i++) {
			QJsonObject json = json_array.at(i).toObject();
			QString name = json.value(name2str(name)).toString();
			QString idStr = json.value(name2str(id)).toString();
			QString accessToken = json.value(name2str(access_token)).toString();
			QMap<QString, QString> map;
			map.insert(FacebookListName_Key, name);
			map.insert(FacebookListId_Key, idStr);
			map.insert(FacebookListToken_Key, accessToken);
			list.insert(i, map);
		}
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success: %d", getApiName(requestType), list.size());
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.get(), CallBack, onFinished);
}

void PLSAPIFacebook::searchGameTagListByKeyword(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished, const QString &keyword)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL("search");
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken()).addQuery("type", "game").addQuery("q", keyword);
	auto CallBack = [=, &list](QJsonObject root) {
		QJsonArray json_array = root.value(name2str(data)).toArray();
		list.clear();
		for (int i = 0; i < json_array.size(); i++) {
			QJsonObject json = json_array.at(i).toObject();
			QString name = json.value(name2str(name)).toString();
			QString idStr = json.value(name2str(id)).toString();
			QMap<QString, QString> map;
			map.insert(FacebookListName_Key, name);
			map.insert(FacebookListId_Key, idStr);
			list.insert(i, map);
		}
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success: %d", getApiName(requestType), list.size());
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.get(), CallBack, onFinished);
}

void PLSAPIFacebook::startLiving(PLSAPI requestType, const QString &itemId, const QString &title, const QString &description, const QString &privacy, const QString &game_id,
				 const QString &accessToken, MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL(itemId + "/live_videos");
	PLSNetworkReplyBuilder builder(url);
	builder.addField("status", "LIVE_NOW")
		.addField("title", title)
		.addField("description", description)
		.addField("privacy", privacy)
		.addField(COOKIE_ACCESS_TOKEN, accessToken)
		.addField("fields", "embed_html,secure_stream_url,video")
		.addField("game_id", game_id);
	auto successFunction = [=](QJsonObject root) {
		QString url = root.value(name2str(secure_stream_url)).toString();
		QString liveId = root.value(name2str(id)).toString();
		QString videoId = root.value(name2str(video)).toObject().value(name2str(id)).toString();
		PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
		facebook->insertLiveInfo(FacebookLiveId_Key, liveId);
		facebook->insertLiveInfo(FacebookLiveUrl_Key, url);
		facebook->insertLiveInfo(FacebookVideoId_Key, videoId);
		QString embedHtml = root.value(name2str(embed_html)).toString();
		QStringList urlList = embedHtml.split(" ");
		if (urlList.size() >= 2) {
			QString queryURL = urlList.at(1);
			QByteArray queryURLArray = queryURL.toUtf8();
			queryURL = QUrl::fromPercentEncoding(queryURLArray);
			queryURL = queryURL.replace("src=", "");
			QUrl qurl(queryURL);
			QUrlQuery query(qurl.query());
			QList<QPair<QString, QString>> list = query.queryItems();
			for (auto pair : list) {
				if (pair.first.compare("href", Qt::CaseInsensitive) == 0) {
					facebook->updateChannelInfo(ChannelData::g_shareUrl, pair.second, true);
					break;
				}
			}
		}
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success: %s", getApiName(requestType), url.toUtf8().constData());
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.post(), successFunction, onFinished);
}

void PLSAPIFacebook::getFacebookItemUserInfo(PLSAPI requestType, const QString &itemId, ItemInfoRequestFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL(itemId);
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken());
	builder.addQuery("fields", "name,picture.type(large)");

	auto successFunction = [=](QJsonObject root) {
		QString name = root.value(name2str(name)).toString();
		QVariant url;
		PLSJsonDataHandler::getValue(root, name2str(url), url);
		QString imagePath;
		downloadSyncImage(url.toString(), imagePath);
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success", getApiName(requestType));
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess, name, imagePath);
	};

	auto failedFunction = [=](PLSAPIFacebookType type) { onFinished(type, "", ""); };
	startRequestApi(requestType, builder.get(), successFunction, failedFunction);
}

void PLSAPIFacebook::getLiveVideoInfoRequest(PLSAPI requestType, const QString &liveVideoId, const QString &field, MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL(liveVideoId);
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken());
	builder.addQuery("fields", field);
	auto successFunction = [=](QJsonObject root) {
		QString titleKey = "title";
		PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
		if (root.contains(titleKey)) {
			QString name = root.value(titleKey).toString();
			facebook->insertLiveInfo(FacebookLiveTitle_Key, name);
		}
		QString descriptionKey = "description";
		if (root.contains(descriptionKey)) {
			QString des = root.value(descriptionKey).toString();
			facebook->insertLiveInfo(FacebookLiveDescription_Key, des);
		}
		QVariantMap updateInfoMap;
		QString reactionsKey = "reactions";
		if (root.contains(reactionsKey)) {
			QJsonObject reactionsObject = root.value(reactionsKey).toObject();
			QVariant reactionsCount;
			PLSJsonDataHandler::getValue(reactionsObject, name2str(total_count), reactionsCount);
			updateInfoMap.insert(ChannelData::g_likes, reactionsCount);
		}
		QString watchCountKey = "live_views";
		if (root.contains(watchCountKey)) {
			QJsonValue watchCount = root.value(watchCountKey);
			int value = watchCount.toInt();
			QString watch = QString("%1").arg(value);
			updateInfoMap.insert(ChannelData::g_viewers, watch);
		}
		QString commentCountKey = "comments";
		if (root.contains(commentCountKey)) {
			QJsonObject commentObject = root.value(commentCountKey).toObject();
			QVariant commentCount;
			PLSJsonDataHandler::getValue(commentObject, name2str(total_count), commentCount);
			updateInfoMap.insert(ChannelData::g_comments, commentCount);
		}
		QString statusKey = "status";
		if (root.contains(statusKey)) {
			QJsonValue status = root.value(statusKey);
			facebook->insertLiveInfo(FacebookLiveStatus_Key, status.toString());
		}
		facebook->updateChannelInfos(updateInfoMap, false);
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success", getApiName(requestType));
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.get(), successFunction, onFinished);
}

void PLSAPIFacebook::getTimelinePrivacyRequest(PLSAPI requestType, const QString &videoId, TimelinePrivacyFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL(videoId);
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken());
	builder.addQuery("fields", "privacy");
	auto successFunction = [=](QJsonObject root) {
		QString privacyId = TimelineOnlymeId;
		QString privacyKey = "privacy";
		if (root.contains(privacyKey)) {
			QJsonObject privacyObject = root.value(privacyKey).toObject();
			QString privacyValue = privacyObject.value(name2str(value)).toString();
			if (privacyValue == "ALL_FRIENDS") {
				privacyId = TimelineFriendId;
			} else if (privacyValue == "EVERYONE") {
				privacyId = TimelinePublicId;
			}
		}
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success", getApiName(requestType));
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess, privacyId);
	};
	auto failedFunction = [=](PLSAPIFacebookType type) { onFinished(type, ""); };
	startRequestApi(requestType, builder.get(), successFunction, failedFunction);
}

void PLSAPIFacebook::updateFacebookLiving(PLSAPI requestType, const QString &liveVideoId, const QString &title, const QString &description, const QString &privacy, const QString &game_id,
					  MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL(liveVideoId);
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addField("title", title).addField("description", description).addField("privacy", privacy).addField(COOKIE_ACCESS_TOKEN, facebook->getAccessToken()).addField("game_id", game_id);
	auto successFunction = [=](QJsonObject root) {
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success", getApiName(requestType));
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.post(), successFunction, onFinished);
}

void PLSAPIFacebook::stopFacebookLiving(PLSAPI requestType, const QString &liveVideoId, MyRequestTypeFunction onFinished)
{
	PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s start", getApiName(requestType));
	QString url = getFaceboolURL(liveVideoId);
	PLSNetworkReplyBuilder builder(url);
	PLSPlatformFacebook *facebook = PLS_PLATFORM_FACEBOOK;
	builder.addQuery(COOKIE_ACCESS_TOKEN, facebook->getAccessToken()).addQuery("end_live_video", "true");
	auto successFunction = [=](QJsonObject root) {
		PLS_INFO(MODULE_PlatformService, "PLSAPIFacebook %s success", getApiName(requestType));
		onFinished(PLSAPIFacebookType::PLSFacebookSuccess);
	};
	startRequestApi(requestType, builder.post(), successFunction, onFinished);
}

void PLSAPIFacebook::downloadSyncImage(const QString &url, QString &imagePath)
{
	if (auto icon = PLSAPIVLive::downloadImageSync(this, url); icon.first) {
		imagePath = icon.second;
		return;
	}
	imagePath = QString();
}

QString PLSAPIFacebook::getFaceboolURL(const QString &endpoint)
{
	return (FACEBOOK_GRAPHA_DOMAIN + endpoint);
	//return FACEBOOK_GRAPHA_DOMAIN.append(endpoint);
}

QUrl PLSAPIFacebook::getPermissionRequestUrl(QString &permission)
{
	QUrl url(CHANNEL_FACEBOOK_LOGIN_URL);
	QUrlQuery query;
	query.addQueryItem(HTTP_CLIENT_ID, CHANNEL_FACEBOOK_CLIENT_ID);
	query.addQueryItem(HTTP_REDIRECT_URI, CHANNEL_FACEBOOK_REDIRECTURL);
	query.addQueryItem("auth_type", "rerequest");
	query.addQueryItem("response_type", "granted_scopes");
	query.addQueryItem("scope", permission);
	url.setQuery(query);
	return url;
}

bool PLSAPIFacebook::goFacebookRequestPermission(QString &permission, QWidget *parent)
{
	static QString _permission;
	_permission = permission;
	auto check = [](QJsonObject &result, const QString &url, const QMap<QString, QString> &cookies) -> PLSResultCheckingResult {
		if (!url.startsWith(CHANNEL_FACEBOOK_REDIRECTURL)) {
			return PLSResultCheckingResult::Continue;
		}
		QUrl qurl(url);
		bool containsPermission = false;
		QStringList list = qurl.query(QUrl::FullyDecoded).split("&");
		QString grantedString;
		for (auto str : list) {
			if (str.startsWith(granted_scopes, Qt::CaseInsensitive)) {
				grantedString = str;
				break;
			}
		}
		QStringList requestPermissionList = _permission.split(",");
		for (auto requestPermission : requestPermissionList) {
			if (!grantedString.contains(requestPermission, Qt::CaseInsensitive)) {
				containsPermission = false;
				break;
			}
			containsPermission = true;
		}
		if (containsPermission) {
			return PLSResultCheckingResult::Ok;
		}
		return PLSResultCheckingResult::Close;
	};
	std::map<std::string, std::string> header;
	QJsonObject result;
	QUrl url = getPermissionRequestUrl(permission);
	bool bResult = pls_browser_view(result, url, header, FACEBOOK, check, parent);
	return bResult;
}

void PLSAPIFacebook::startRequestApi(PLSAPI requestType, QNetworkReply *requestReply, MyRequestSuccessFunction successFunction, MyRequestTypeFunction failedFunction)
{
	if (m_reply.value(requestType) != nullptr) {
		m_reply.value(requestType)->finished();
	}
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		auto doc = QJsonDocument::fromJson(data);
		if (!doc.isObject()) {
			PLS_ERROR(MODULE_PlatformService, "PLSAPIFacebook %s is not object", getApiName(requestType));
			failedFunction(PLSAPIFacebookType::PLSFacebookNotObject);
			return;
		}
		auto root = doc.object();
		successFunction(root);
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		PLSAPIFacebookType errorType = handleApiErrorCode(requestType, code, data, error);
		failedFunction(errorType);
	};
	m_reply.insert(requestType, requestReply);
	if (PLS_PLATFORM_FACEBOOK->getParentPointer() != nullptr) {
		PLS_HTTP_HELPER->connectFinished(requestReply, PLS_PLATFORM_FACEBOOK->getParentPointer(), _onSucceed, _onFail);
	} else {
		PLS_HTTP_HELPER->connectFinished(requestReply, this, _onSucceed, _onFail);
	}
}

PLSAPIFacebookType PLSAPIFacebook::handleApiErrorCode(PLSAPI requestType, int statusCode, QByteArray data, QNetworkReply::NetworkError error)
{
	auto doc = QJsonDocument::fromJson(data);
	QJsonObject root = doc.object();
	QVariant codeVariant;
	PLSJsonDataHandler::getValue(root, name2str(code), codeVariant);
	QVariant messageVariant;
	PLSJsonDataHandler::getValue(root, name2str(message), messageVariant);
	QString message = messageVariant.toString();
	int code = codeVariant.toInt();
	if (message.length() > 0) {
		qDebug() << "message: " << message << ", code " << code;
		PLS_ERROR(MODULE_PlatformService, "PLSAPIFacebook %s Failed code = %d, message = %s", getApiName(requestType), code, message.toUtf8().constData());
	} else {
		if (error == QNetworkReply::OperationCanceledError) {
			PLS_ERROR(MODULE_PlatformService, "PLSAPIFacebook %s Failed code = CancelNetwork", getApiName(requestType));
		} else {
			PLS_ERROR(MODULE_PlatformService, "PLSAPIFacebook %s Failed code = OtherNetworkError: %d", getApiName(requestType), error);
		}
	}
	//group manager permission and install app permission
	if (code == 200) {
		return PLSAPIFacebookType::PLSFacebookDeclined;
	} else if (code == 190) {
		if (requestType == PLSAPI::PLSAPIGetLongLiveUserAccessToken || requestType == PLSAPI::PLSAPIGetUserInfo) {
			PLS_PLATFORM_FACEBOOK->insertSrcInfo(ChannelData::g_channelStatus, ChannelData::ChannelStatus::Expired);
		}
		//page living accesstoken invalid and page delete
		if (requestType == PLSAPI::PLSAPIStartPageLiving) {
			return PLSAPIFacebookType::PLSFacebookDeclined;
		}
		return PLSAPIFacebookType::PLSFacebookInvalidAccessToken;
	} else if (code == 100) {
		//group delete
		/*if (requestType == PLSAPI::PLSAPIStartGroupLiving) {
			return PLSAPIFacebookType::PLSFacebookDeclined;
		}*/
		return PLSAPIFacebookType::PLSFacebookObjectDontExist;
	}

	if (statusCode == 0 && error > QNetworkReply::NoError && error <= QNetworkReply::UnknownNetworkError) {
		return PLSAPIFacebookType::PLSFacebookNetworkError;
	}
	return PLSAPIFacebookType::PLSFacebookFailed;
}

const char *PLSAPIFacebook::getApiName(PLSAPI requestType)
{
	QMetaEnum metaRequestType = QMetaEnum::fromType<PLSAPIFacebook::PLSAPI>();
	return metaRequestType.valueToKey(requestType);
}
