#include "PLSChannelDataHandlerFunctions.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QRunnable>
#include <QThreadPool>
#include <QUrl>

#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "LogPredefine.h"

#include "PLSChannelDataAPI.h"
#include "frontend-api.h"

#include "PLSChannelDataHandler.h"
#include "PLSChannelsVirualAPI.h"
#include "PLSPlatformApi.h"
#include "pls-common-define.hpp"

#include "../../PLSPlatformApi/band/PLSBandDataHandler.h"
#include "../../PLSPlatformApi/facebook/PLSFacebookDataHandler.h"
#include "../../PLSPlatformApi/vlive/PLSVLiveDataHandler.h"
#include "PLSAfreecaTVDataHandler.h"

using namespace ChannelData;

QVariantMap loadMapFromJsonFile(const QString &fileName)
{
	QVariantMap tmpMaper;
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		auto data = file.readAll();
		auto jsonDoc = QJsonDocument::fromJson(data);
		tmpMaper = jsonDoc.toVariant().toMap();
	}

	return tmpMaper;
}

void registerAllPlatforms()
{
	PLSCHANNELS_API->registerPlatformHandler(new PLSAfreecaTVDataHandler);
	PLSCHANNELS_API->registerPlatformHandler(new TwitchDataHandler);
	PLSCHANNELS_API->registerPlatformHandler(new YoutubeHandler);
	PLSCHANNELS_API->registerPlatformHandler(new PLSVLiveDataHandler);
	PLSCHANNELS_API->registerPlatformHandler(new PLSBandDataHandler);
	PLSCHANNELS_API->registerPlatformHandler(new PLSFacebookDataHandler);
}

QString getYoutubeImageUrl(const QVariantMap &src)
{
	auto list = src.value("items").toList();
	if (list.isEmpty()) {
		return "";
	}

	return list[0].toMap()["snippet"].toMap()["thumbnails"].toMap()["high"].toMap()["url"].toString();
}

QString getYoutubeName(const QVariantMap &src)
{
	auto list = src.value("items").toList();
	if (list.isEmpty()) {
		return "";
	}

	return list[0].toMap()["snippet"].toMap()["title"].toString();
}

QString getYoutubeFirstID(const QVariantMap &src)
{
	auto list = src.value("items").toList();
	if (list.isEmpty()) {
		return "";
	}
	return list[0].toMap()["id"].toString();
}

bool isItemsExists(const QVariantMap &src)
{
	return src.contains("items");
}

bool isChannelItemEmpty(const QVariantMap &src)
{
	auto list = src.value("items").toList();
	if (list.isEmpty()) {
		return true;
	}

	return list[0].toMap().isEmpty();
}

QString getYoutubePriacyStatus(const QVariantMap &src)
{
	auto list = src.value("items").toList();
	if (list.isEmpty()) {
		return "";
	}
	auto mapS = list[0].toMap()["status"].toMap();
	return mapS["privacyStatus"].toString();
}

bool isInvalidGrant(const QVariantMap &src)
{
	if (src.contains("error")) {
		return src.value("error").toString().contains("invalid_grant", Qt::CaseInsensitive);
	}

	return false;
}

bool isTokenValid(const QString &mSrcUUID)
{
	auto &channelInfo = PLSCHANNELS_API->getChanelInfoRef(mSrcUUID);
	auto lastTokenCreateTime = getInfo(channelInfo, g_createTime, QDateTime());
	int expiresSeconds = getInfo(channelInfo, g_expires_in, g_defaultExpiresSeconds);
	if (lastTokenCreateTime.isValid()) {
		int differ = lastTokenCreateTime.secsTo(QDateTime::currentDateTime());
		return (differ < expiresSeconds);
	}
	return false;
}

bool isTokenValid(const QVariantMap &channelInfo)
{
	auto lastTokenCreateTime = getInfo(channelInfo, g_createTime, QDateTime());
	int expiresSeconds = getInfo(channelInfo, g_expires_in, g_defaultExpiresSeconds);
	if (lastTokenCreateTime.isValid()) {
		int differ = lastTokenCreateTime.secsTo(QDateTime::currentDateTime());
		return (differ < expiresSeconds);
	}
	return false;
}

bool isReplyContainExpired(const QByteArray &body, const QStringList &keys)
{
	int matchCount = 0;
	for (const QString &key : keys) {
		if (body.contains(key.toUtf8())) {
			++matchCount;
		}
	}

	return matchCount = keys.count();
}

QJsonObject createJsonArrayFromInfo(const QString &uuid)
{
	QJsonObject obj;
	auto &info = PLSCHANNELS_API->getChanelInfoRef(uuid);
	if (!info.isEmpty()) {
		auto platformName = getInfo(info, g_channelName);

		platformName = platformName.remove("RTMP").remove(" ").toUpper();
		auto userID = getInfo(info, g_rtmpUserID);
		auto disName = getInfo(info, g_nickName);
		auto rtmpUrl = getInfo(info, g_channelRtmpUrl);
		auto description = getInfo(info, g_otherInfo);
		auto streamKey = getInfo(info, g_streamKey);

		obj.insert("streamName", disName);
		obj.insert("rtmpUrl", rtmpUrl);
		obj.insert("streamKey", streamKey);
		obj.insert("username", userID);
		obj.insert(g_publishService, platformName);
		obj.insert("description", description);
		obj.insert("resolution", "720");
		obj.insert("bitrate", "2000");
		obj.insert("framerate", "30");
		obj.insert("interval", 2);
	}

	return obj;
};

QVariantMap createPrismHeader()
{
	QVariantMap headerMap;
	headerMap[HTTP_HEAD_CONTENT_TYPE] = HTTP_CONTENT_TYPE_VALUE;
	pls_http_request_head(headerMap, true);
	return headerMap;
}

bool RTMPAddToPrism(const QString &uuid)
{
	PRE_LOG(add rtmp begin, INFO);
	HolderReleaser releaser(&PLSChannelDataAPI::addingHold);

	if (PLSCHANNELS_API->isChannelInfoExists(uuid)) {
		FinishTaskReleaser finishAdd(uuid);
		return true;
		PLSCHANNELS_API->release();

		auto headerMap = createPrismHeader();
		if (headerMap.isEmpty()) {
			PLSCHANNELS_API->acquire();
			return false;
		}
		PLSHmacNetworkReplyBuilder builder;
		builder.setRawHeaders(headerMap);

		auto obj = createJsonArrayFromInfo(uuid);
		builder.setJsonObject(obj);
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);

		builder.setCookie(QVariant::fromValue(createPrismCookie().toRawForm()));
		builder.setUrl(PLS_RTMP_ADD);
		builder.setHmacType(HmacType::HT_PRISM);

		auto handleSuccess = [=](QNetworkReply *networkReplay, int statusCode, QByteArray data, void *context) {
			PLSCHANNELS_API->acquire();
			auto &lastInfo = PLSCHANNELS_API->getChanelInfoRef(uuid);
			auto jsonDoc = QJsonDocument::fromJson(data);
			auto jsonMap = jsonDoc.toVariant().toMap();
			addToMap(lastInfo, jsonMap);
			FinishTaskReleaser finishAdd(uuid);
		};

		auto handleFail = [=](QNetworkReply *networkReplay, int statusCode, QByteArray Arraydatas, QNetworkReply::NetworkError error, void *context) {
			PLSCHANNELS_API->acquire();
			PLSCHANNELS_API->removeChannelInfo(uuid, false, false);
			FinishTaskReleaser finishUpdate(uuid);
			if (isPrismReplyExpired(networkReplay, Arraydatas)) {
				PLSCHANNELS_API->prismTokenExpired();
				return;
			}
			ChannelsNetWorkPretestWithAlerts(networkReplay, Arraydatas);
		};

		auto network = PLSCHANNELS_API->getNetWorkAPI();
		PLS_HTTP_HELPER->connectFinished(builder.post(network), PLSCHANNELS_API, handleSuccess, handleFail);
	}
	return true;
}

bool RTMPUpdateToPrism(const QString &uuid)
{
	PRE_LOG(update rtmp begin, INFO);
	HolderReleaser releaser(&PLSChannelDataAPI::holdOnChannelArea);

	if (PLSCHANNELS_API->isChannelInfoExists(uuid)) {
		FinishTaskReleaser finishUpdate(uuid);
		return true;
		PLSCHANNELS_API->release();
		auto headerMap = createPrismHeader();
		if (headerMap.isEmpty()) {
			PLSCHANNELS_API->acquire();
			PLSCHANNELS_API->recoverInfo(uuid);
			return false;
		}

		PLSHmacNetworkReplyBuilder builder;
		builder.setRawHeaders(headerMap);

		auto obj = createJsonArrayFromInfo(uuid);
		builder.setJsonObject(obj);
		builder.setContentType(HTTP_CONTENT_TYPE_VALUE);

		builder.setCookie(QVariant::fromValue(createPrismCookie().toRawForm()));
		auto sqNo = PLSCHANNELS_API->getValueOfChannel(uuid, g_rtmpSeq, QString());
		QString url = QString(PLS_RTMP_MODIFY).arg(sqNo);
		builder.setUrl(url);
		builder.setHmacType(HmacType::HT_PRISM);

		auto handleSuccess = [=](QNetworkReply *networkReplay, int statusCode, QByteArray data, void *context) {
			PLSCHANNELS_API->acquire();
			auto &lastInfo = PLSCHANNELS_API->getChanelInfoRef(uuid);
			auto jsonDoc = QJsonDocument::fromJson(data);
			auto jsonMap = jsonDoc.toVariant().toMap();
			addToMap(lastInfo, jsonMap);
			PLSCHANNELS_API->clearBackup(uuid);
			FinishTaskReleaser finishUpdate(uuid);
		};

		auto handleFail = [=](QNetworkReply *networkReplay, int statusCode, QByteArray Arraydatas, QNetworkReply::NetworkError error, void *context) {
			PLSCHANNELS_API->acquire();
			PLSCHANNELS_API->recoverInfo(uuid);
			FinishTaskReleaser finishUpdate(uuid);
			if (isPrismReplyExpired(networkReplay, Arraydatas)) {
				PLSCHANNELS_API->prismTokenExpired();
				return;
			}
			ChannelsNetWorkPretestWithAlerts(networkReplay, Arraydatas);
		};

		auto network = PLSCHANNELS_API->getNetWorkAPI();
		PLS_HTTP_HELPER->connectFinished(builder.put(network), PLSCHANNELS_API, handleSuccess, handleFail);
	}
	return true;
}

bool RTMPDeleteToPrism(const QString &uuid)
{
	PRE_LOG(delete rtmp begin, INFO);
	if (PLSCHANNELS_API->isChannelInfoExists(uuid)) {
		PLSCHANNELS_API->removeChannelInfo(uuid, true, false);
		FinishTaskReleaser finishUpdate(uuid);
		return true;
		PLSCHANNELS_API->release();
		auto headerMap = createPrismHeader();
		if (headerMap.isEmpty()) {
			PLSCHANNELS_API->acquire();
			return false;
		}
		PLSHmacNetworkReplyBuilder builder;
		builder.setRawHeaders(headerMap);
		auto seq = PLSCHANNELS_API->getValueOfChannel(uuid, g_rtmpSeq, QString());

		builder.setCookie(QVariant::fromValue(createPrismCookie().toRawForm()));
		QString url = QString(PLS_RTMP_DELETE).arg(seq);
		builder.setUrl(url);
		builder.setHmacType(HmacType::HT_PRISM);

		auto handleSuccess = [=](QNetworkReply *networkReplay, int statusCode, QByteArray data, void *context) {
			PLSCHANNELS_API->acquire();
			PLSCHANNELS_API->removeChannelInfo(uuid, true, false);
			FinishTaskReleaser finishUpdate(uuid);
		};

		auto handleFail = [=](QNetworkReply *networkReplay, int statusCode, QByteArray Arraydatas, QNetworkReply::NetworkError error, void *context) {
			PLSCHANNELS_API->acquire();
			FinishTaskReleaser finishUpdate(uuid);
			if (isPrismReplyExpired(networkReplay, Arraydatas)) {
				PLSCHANNELS_API->prismTokenExpired();
				return;
			}
			ChannelsNetWorkPretestWithAlerts(networkReplay, Arraydatas);
		};

		auto network = PLSCHANNELS_API->getNetWorkAPI();
		PLS_HTTP_HELPER->connectFinished(builder.del(network), PLSCHANNELS_API, handleSuccess, handleFail);
	}
	return true;
}

QNetworkCookie createPrismCookie()
{
	QNetworkCookie cookie;
	cookie.setName(COOKIE_NEO_SES);
	cookie.setValue(pls_get_prism_token().toUtf8());
	return cookie;
}

void updateAllRtmps()
{
	PRE_LOG(update all RTMPs begin, INFO);
	HolderReleaser releaser(&PLSChannelDataAPI::holdOnChannelArea);
	PLSCHANNELS_API->release();

	auto headerMap = createPrismHeader();
	if (headerMap.isEmpty()) {
		PRE_LOG(update all RTMPs error when prism token is not right, INFO);
		PLSCHANNELS_API->acquire();
		if (!PLSCHANNELS_API->isInitilized()) {
			PLSCHANNELS_API->resetInitializeState(true);
		}
		return;
	}
	PLSHmacNetworkReplyBuilder builder;
	builder.setRawHeaders(headerMap);
	builder.setCookie(QVariant::fromValue(createPrismCookie().toRawForm()));

	builder.setUrl(PLS_RTMP_LIST);
	builder.setHmacType(HmacType::HT_PRISM);

	auto handleSuccess = [&](QNetworkReply *networkReplay, int statusCode, QByteArray data, void *context) { updateRTMPCallback(data); };

	auto handleFail = [&](QNetworkReply *networkReplay, int statusCode, QByteArray Arraydatas, QNetworkReply::NetworkError error, void *context) {
		if (isPrismReplyExpired(networkReplay, Arraydatas)) {
			endRefresh();
			PLSCHANNELS_API->prismTokenExpired();
			return;
		}
		ChannelsNetWorkPretestWithAlerts(networkReplay, Arraydatas);
		endRefresh();
	};

	auto network = PLSCHANNELS_API->getNetWorkAPI();
	PLS_HTTP_HELPER->connectFinished(builder.get(network), PLSCHANNELS_API, handleSuccess, handleFail);
}

QString &localizePlatName(QString &srcName)
{
	auto tmp = srcName;
	tmp = tmp.remove(" ").toUpper();
	const auto &rmtpNames = PLSCHANNELS_API->getRTMPsName();
	bool isFind = false;
	for (int i = 0; i < rmtpNames.size(); ++i) {
		auto name = rmtpNames[i];
		name = name.remove(" ").toUpper();
		if (name == tmp) {
			srcName = rmtpNames[i];
			isFind = true;
			break;
		}
	}
	if (!isFind) {
		srcName = CUSTOM_RTMP;
	}
	return srcName;
}

void endRefresh()
{
	//qDebug() << "end rtmp update "<<QTime::currentTime();
	PRE_LOG(End Task..., INFO);
	PLSCHANNELS_API->acquire();
	if (PLSCHANNELS_API->isEmptyToAcquire() && !PLSCHANNELS_API->isInitilized()) {
		PLSCHANNELS_API->resetInitializeState(true);
	}
	PLSCHANNELS_API->holdOnChannelArea(false);
	if (PLSCHANNELS_API->hasError()) {
		PLSCHANNELS_API->networkInvalidOcurred();
	}

	if (PLSCHANNELS_API->isEmptyToAcquire()) {
		PLSCHANNELS_API->endTransactions();
	}
}

struct RtmpRun : public QRunnable {
	using QRunnable::QRunnable;
	~RtmpRun()
	{
		PRE_LOG(End RTMP update..., INFO);
		endRefresh();
	}
	void run()
	{
		auto jsonDoc = QJsonDocument::fromJson(taskData);
		auto jsArray = jsonDoc.array().toVariantList();
		auto mapper = loadMapFromJsonFile(":/configs/configs/RTMPJsonMapper.json");

		auto allChannels = PLSCHANNELS_API->getAllChannelInfo();
		QSet<QString> matched;
		//match
		auto checkMatched = [&](const QVariant &obj) {
			auto tmpMap = obj.toMap();
			QVariantMap objMap;
			addToMap(objMap, tmpMap, mapper);
			auto platformName = getInfo(objMap, g_channelName);
			auto guessPlaform = guessPlatformFromRTMP(getInfo(objMap, g_channelRtmpUrl));
			if (platformName.isEmpty()) {
				platformName = CUSTOM_RTMP;
			} else {
				platformName = localizePlatName(platformName);
			}

			if (guessPlaform != platformName) {
				if (guessPlaform != BAND && guessPlaform != NOW) {
					platformName = guessPlaform;
				}
			}

			objMap[g_channelName] = platformName;
			objMap[g_isUpdated] = true;
			auto seq = getInfo(objMap, g_rtmpSeq);
			auto isSeqMatched = [&](const QVariantMap &src) {
				int type = getInfo(src, g_data_type, ChannelType);
				auto srcSeq = getInfo(src, g_rtmpSeq);
				if (type == RTMPType && srcSeq == seq) {
					return true;
				}
				return false;
			};
			auto retIte = std::find_if(allChannels.begin(), allChannels.end(), isSeqMatched);
			if (retIte == allChannels.end()) {
				auto defaultMap = createDefaultChannelInfoMap(platformName, RTMPType);
				addToMap(defaultMap, objMap);
				defaultMap[g_channelStatus] = Valid;
				auto uuid = getInfo(defaultMap, g_channelUUID);
				matched.insert(uuid);
				PLSCHANNELS_API->addChannelInfo(defaultMap, false);
				PLSCHANNELS_API->channelAdded(uuid);
			} else {
				auto &lastMap = retIte.value();
				addToMap(lastMap, objMap);
				lastMap[g_channelStatus] = Valid;
				matched.insert(getInfo(lastMap, g_channelUUID));
				PLSCHANNELS_API->setChannelInfos(lastMap);
			}
		};

		std::for_each(jsArray.rbegin(), jsArray.rend(), checkMatched);

		// remove
		auto removeIte = allChannels.begin();
		for (; removeIte != allChannels.end(); ++removeIte) {
			auto info = removeIte.value();
			if (getInfo(info, g_data_type, RTMPType) == RTMPType && !matched.contains(removeIte.key())) {
				PLSCHANNELS_API->removeChannelInfo(removeIte.key(), true, false);
			}
		}
	}

public:
	QByteArray taskData;
};

void updateRTMPCallback(const QByteArray &retData)
{
	auto run = new RtmpRun();
	run->taskData = retData;
	QThreadPool::globalInstance()->start(run);
}
