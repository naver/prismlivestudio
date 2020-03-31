#include "PLSAPIYoutube.h"
#include <assert.h>
#include <qfileinfo.h>
#include <QNetworkCookieJar>
#include <vector>
#include "../PLSPlatformBase.hpp"
#include "ChannelCommonFunctions.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelDataHandlerFunctions.h"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "PLSPlatformApi.h"
#include "PLSPlatformYoutube.h"
#include "frontend-api.h"
#include "pls-common-define.hpp"
#include "pls-gpop-data.hpp"
#include <assert.h>
#include "PLSDateFormate.h"
#include <quuid.h>

using namespace std;

PLSAPIYoutube::PLSAPIYoutube(QObject *parent) : QObject(parent) {}

void PLSAPIYoutube::requestLiveBroadcastStatus(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/liveBroadcasts").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);
		builder.setQuerys({{"id", PLS_PLATFORM_YOUTUBE->getSelectData()._id}, {"part", "id,status"}});
		addCommenCookieAndUserKey(builder);
		return builder.get();
	};
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestVideoStatus(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/videos").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);
		builder.setQuerys({{"part", "statistics, liveStreamingDetails"}, {"id", PLS_PLATFORM_YOUTUBE->getSelectData()._id}});
		PLSAPIYoutube::addCommenCookieAndUserKey(builder);
		return builder.get();
	};
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestStopLive(const QObject *receiver, function<void()> onNext)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");
		if (nullptr != onNext) {
			onNext();
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		Q_UNUSED(error)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");
		if (nullptr != onNext) {
			onNext();
		}
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	PLSNetworkReplyBuilder builder(QString("%1/liveBroadcasts/transition").arg(g_plsYoutubeAPIHost));
	builder.setContentType(HTTP_CONTENT_TYPE_VALUE);
	builder.setQuerys({{"broadcastStatus", "complete"}, {"id", PLS_PLATFORM_YOUTUBE->getSelectData()._id}, {"part", "status"}});
	addCommenCookieAndUserKey(builder);
	PLS_HTTP_HELPER->connectFinished(builder.post(), receiver, _onSucceed, _onFail);
}

void PLSAPIYoutube::requestLiveBroadcastsInsert(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/liveBroadcasts").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);
		builder.addQuery("part", "snippet,status,contentDetails");
		addCommenCookieAndUserKey(builder);
		/*builder.addJsonObject("status", QVariantMap({{"privacyStatus", PLS_PLATFORM_YOUTUBE->getTrySaveDataData().privacyStatus.toLower()},
							     {"selfDeclaredMadeForKids", PLS_PLATFORM_YOUTUBE->getTrySaveDataData().isForKids}}));
		*/
		builder.addJsonObject("status", QVariantMap({{"privacyStatus", PLS_PLATFORM_YOUTUBE->getTrySaveDataData().privacyStatus.toLower()}}));

		auto nowTime = PLSDateFormate::youtubeTimeStampToString(PLSDateFormate::getNowTimeStamp());
		builder.addJsonObject("snippet", QVariantMap({{"title", PLS_PLATFORM_YOUTUBE->getTrySaveDataData().title}, {"scheduledStartTime", nowTime}}));
		builder.addJsonObject("contentDetails", QVariantMap({{"enableAutoStart", true}, {"enableAutoStop", true}}));
		return builder.post();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestLiveStreamsInsert(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/liveStreams").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);
		builder.addQuery("part", "snippet,cdn,contentDetails,status");
		addCommenCookieAndUserKey(builder);
		QJsonObject object = QJsonObject();

		const auto &itemData = PLS_PLATFORM_YOUTUBE->getTrySaveDataData();
		if (itemData.isNormalLive) {
			QJsonObject snippetObject = QJsonObject();
			snippetObject["title"] = QUuid::createUuid().toString();
			snippetObject["description"] = PLS_PLATFORM_YOUTUBE->getTrySaveDataData().description;
			object["snippet"] = snippetObject;
			QJsonObject cdnObject = QJsonObject();
			cdnObject["frameRate"] = "variable";
			cdnObject["ingestionType"] = "rtmp";
			cdnObject["resolution"] = "variable";
			object["cdn"] = cdnObject;
		} else {
			object["snippet"] = itemData.streamAPIData["snippet"].toObject();

			QJsonObject cdnObject = QJsonObject();

			cdnObject["frameRate"] = itemData.streamAPIData["cdn"].toObject()["frameRate"].toString();
			cdnObject["ingestionType"] = itemData.streamAPIData["cdn"].toObject()["ingestionType"].toString();
			cdnObject["resolution"] = itemData.streamAPIData["cdn"].toObject()["resolution"].toString();
			object["cdn"] = cdnObject;

			QJsonObject contentDetails = QJsonObject();
			contentDetails["isReusable"] = itemData.streamAPIData["contentDetails"].toObject()["isReusable"].toBool();
			object["contentDetails"] = contentDetails;
		}

		builder.setJsonObject(object);
		return builder.post();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestLiveBroadcastsBind(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/liveBroadcasts/bind").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);
		builder.setQuerys(
			{{"id", PLS_PLATFORM_YOUTUBE->getTrySaveDataData()._id}, {"streamId", PLS_PLATFORM_YOUTUBE->getTrySaveDataData().boundStreamId}, {"part", "id,contentDetails,status"}});
		addCommenCookieAndUserKey(builder);
		return builder.post();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestLiveBroadcastsUpdate(PLSYoutubeLiveinfoData data, const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/liveBroadcasts").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);
		builder.addQuery("part", "contentDetails,status");
		addCommenCookieAndUserKey(builder);
		auto nowTime = PLSDateFormate::youtubeTimeStampToString(PLSDateFormate::getNowTimeStamp());
		builder.addJsonObject("id", data._id);
		builder.addJsonObject("contentDetails", data.contentDetails);
		builder.addJsonObject("status", QVariantMap({{"privacyStatus", data.privacyStatus.toLower()}}));
		return builder.put();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestCategoryID(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType, bool isAllList, int context)
{
	auto _getNetworkReply = [=] {
		QString liveID = PLS_PLATFORM_YOUTUBE->getSelectData()._id;
		if (isAllList) {
			for (int i = 0; i < PLS_PLATFORM_YOUTUBE->getScheduleDatas().size(); i++) {
				auto sche = PLS_PLATFORM_YOUTUBE->getScheduleDatas()[i];
				liveID = liveID.append("%1").arg(i == 0 ? "" : ",").append(sche._id);
			}
		}
		PLSNetworkReplyBuilder builder(QString("%1/videos").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE).setQuerys({{"id", liveID}, {"part", "snippet"}});
		addCommenCookieAndUserKey(builder);
		return builder.get();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed, nullptr, reinterpret_cast<void *>(context));
}

void PLSAPIYoutube::requestCategoryList(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		const char *lang = static_cast<PLSApp *>(QCoreApplication::instance())->GetLocale();
		QString currentLangure(lang);
		bool isKr = 0 == currentLangure.compare(KO_KR, Qt::CaseInsensitive);
		QString langStr = isKr ? "ko-kr" : "en-us";
		QString regionCode = isKr ? "kr" : "us";
		PLSNetworkReplyBuilder builder(QString("%1/videoCategories").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE).setQuerys({{"hl", langStr}, {"regionCode", regionCode}, {"part", "snippet"}});
		addCommenCookieAndUserKey(builder);
		return builder.get();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestScheduleList(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType, int context)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/liveBroadcasts").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE).setQuerys({{"part", "contentDetails,snippet,status"}, {"broadcastStatus", "upcoming"}, {"maxResults", "50"}, {"pageToken", "@"}});
		addCommenCookieAndUserKey(builder);
		return builder.get();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed, nullptr, reinterpret_cast<void *>(context));
}

void PLSAPIYoutube::requestCurrentSelectData(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/liveBroadcasts").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE).setQuerys({{"part", "contentDetails,snippet,status"}, {"id", PLS_PLATFORM_YOUTUBE->getSelectData()._id}});
		addCommenCookieAndUserKey(builder);
		return builder.get();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed, nullptr);
}

void PLSAPIYoutube::requestLiveStreamKey(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType)
{
	auto _getNetworkReply = [=] {
		QStringList ids;
		for (const auto &data : PLS_PLATFORM_YOUTUBE->getScheduleDatas()) {
			ids.append(data.boundStreamId);
		}
		QString boundStreamID = ids.join(",");
		PLSNetworkReplyBuilder builder(QString("%1/liveStreams").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE).setQuerys({{"part", "snippet,cdn,contentDetails,status"}, {"id", boundStreamID}, {"maxResults", "50"}});
		addCommenCookieAndUserKey(builder);
		return builder.get();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");

	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::requestUpdateVideoData(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType, PLSYoutubeLiveinfoData data)
{
	auto _getNetworkReply = [=] {
		PLSNetworkReplyBuilder builder(QString("%1/videos").arg(g_plsYoutubeAPIHost));
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE).addQuery("part", "snippet,status");
		addCommenCookieAndUserKey(builder);
		builder.addJsonObject("status", QVariantMap({{"privacyStatus", data.privacyStatus.toLower()} /*, {"selfDeclaredMadeForKids", data.isForKids}*/}));
		builder.addJsonObject("id", data._id);
		builder.addJsonObject("snippet", QVariantMap({{"description", data.description}, {"title", data.title}, {"categoryId", data.categoryID}}));
		return builder.put();
	};
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	refreshYoutubeTokenBeforeRequest(refreshType, _getNetworkReply, receiver, onSucceed, onFailed);
}

void PLSAPIYoutube::refreshYoutubeTokenBeforeRequest(RefreshType refreshType, function<QNetworkReply *()> originNetworkReplay, const QObject *originReceiver, dataFunction originOnSucceed,
						     dataErrorFunction originOnFailed, dataErrorFunction originOnFinished, void *const originContext)
{
	const QString &youtubeUUID = PLS_PLATFORM_YOUTUBE->getChannelUUID();
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLS_INFO(MODULE_PlatformService, __FUNCTION__ "succeed");
		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			auto root = doc.object();
			auto access_token = root["access_token"].toString();
			PLSCHANNELS_API->setValueOfChannel(youtubeUUID, ChannelData::g_channelToken, access_token);
			PLSCHANNELS_API->setValueOfChannel(youtubeUUID, ChannelData::g_createTime, QDateTime::currentDateTime());
			PLSCHANNELS_API->channelModified(youtubeUUID);
			if (originNetworkReplay != nullptr) {
				PLS_HTTP_HELPER->connectFinished(originNetworkReplay(), originReceiver, originOnSucceed, originOnFailed, originOnFinished, originContext);
			} else if (nullptr != originOnSucceed) {
				originOnSucceed(networkReplay, code, data, context);
			}
		} else {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ " failed, doc is not object");
			if (nullptr != originOnFailed) {
				originOnFailed(networkReplay, 1999, data, QNetworkReply::InternalServerError, context);
			}
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)
		Q_UNUSED(code)
		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ "failed");
		originOnFailed(networkReplay, code, data, error, context);
	};

	assert(!youtubeUUID.isEmpty());

	if (refreshType == RefreshType::NotRefresh || (refreshType == RefreshType::CheckRefresh && isTokenValid(youtubeUUID))) {
		if (originNetworkReplay != nullptr) {
			PLS_HTTP_HELPER->connectFinished(originNetworkReplay(), originReceiver, originOnSucceed, originOnFailed, originOnFinished, originContext);
		} else if (nullptr != originOnSucceed) {
			originOnSucceed(nullptr, 200, nullptr, originContext);
		}
		return;
	}

	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");

	auto &channelData = PLSCHANNELS_API->getChanelInfoRef(youtubeUUID);
	auto token = channelData.value(ChannelData::g_refreshToken).toString();

	QString bodyStr(QString("refresh_token=%0&client_id=%1&client_secret=%2&grant_type=refresh_token").arg(token).arg(YOUTUBE_CLIENT_ID).arg(YOUTUBE_CLIENT_KEY));

	QByteArray bodyByte = bodyStr.toUtf8();
	PLSNetworkReplyBuilder builder(QString("%1/token").arg(g_plsYoutubeAPIHostV4));
	builder.setContentType(HTTP_CONTENT_TYPE_URL_ENCODED_VALUE);
	builder.setBody(bodyByte);
	PLS_HTTP_HELPER->connectFinished(builder.post(), originReceiver, _onSucceed, _onFail);
}

void PLSAPIYoutube::addCommenCookieAndUserKey(PLSNetworkReplyBuilder &builder)
{
	builder.addQuery("key", YOUTUBE_CLIENT_KEY);
	builder.setRawHeaders({{"Authorization", QString("Bearer %1").arg(PLS_PLATFORM_YOUTUBE->getChannelToken())}});
}
