#include "PLSPlatformTwitch.h"

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "pls-app.hpp"
#include "log.h"
#include "frontend-api.h"
#include "alert-view.hpp"
#include "window-basic-main.hpp"
#include "pls-common-define.hpp"
#include "PLSHttpApi\PLSHttpHelper.h"
#include "../PLSPlatformApi.h"
#include "../PLSLiveInfoDialogs.h"
#include "PLSChannelDataAPI.h"
#include "QTimer"

#define TWTICH_CHAT "TwitchChat"
#define TWTICH_SERVER "TwitchServer"

PLSPlatformTwitch::PLSPlatformTwitch() : m_idxServer(0)
{
	setSingleChannel(true);
	connect(&m_statusTimer, &QTimer::timeout, this, &PLSPlatformTwitch::requestStatisticsInfo);
}

PLSServiceType PLSPlatformTwitch::getServiceType() const
{
	return PLSServiceType::ST_TWITCH;
}

int PLSPlatformTwitch::getServerIndex() const
{
	return m_idxServer;
}

void PLSPlatformTwitch::setServerIndex(int idxServer)
{
	m_idxServer = idxServer;
}

vector<string> PLSPlatformTwitch::getServerNames() const
{
	vector<string> vecServerNames;
	vecServerNames.reserve(m_vecTwitchServers.size());

	for_each(m_vecTwitchServers.begin(), m_vecTwitchServers.end(), [&](const TwitchServer &value) { vecServerNames.push_back(value.name); });

	return vecServerNames;
}

void PLSPlatformTwitch::saveSettings(const string &title, const string &category, int idxServer)
{
	if (getTitle() != title || getCategory() != category) {
		requestUpdateChannel(title, category);
	} else {
		emit onUpdateChannel(PLSPlatformApiResult::PAR_SUCCEED);
	}

	m_idxServer = idxServer;
	if (m_idxServer != -1 && m_idxServer < m_vecTwitchServers.size()) {
		config_set_uint(App()->GlobalConfig(), KeyConfigLiveInfo, KeyTwitchServer, m_vecTwitchServers[m_idxServer]._id);
	}
	saveStreamServer();
}

void PLSPlatformTwitch::onPrepareLive(bool value)
{
	if (!value) {
		prepareLiveCallback(value);
		return;
	}

	value = pls_exec_live_Info_twitch(getChannelUUID(), getInitData()) == QDialog::Accepted;

	prepareLiveCallback(value);
}

void PLSPlatformTwitch::onAlLiveStarted(bool value)
{
	if (value && !PLS_PLATFORM_API->isPrismLive()) {
		m_statusTimer.start(5000);
	}
}

void PLSPlatformTwitch::onLiveStopped()
{
	if (m_statusTimer.isActive()) {
		m_statusTimer.stop();
	}

	if (PLS_PLATFORM_API->isPrismLive()) {
		liveStoppedCallback();
	} else {
		requestVideos();
	}
}

void PLSPlatformTwitch::saveStreamServer()
{
	if (-1 != m_idxServer && m_idxServer < m_vecTwitchServers.size()) {
		const auto &value = m_vecTwitchServers[m_idxServer].url_template;
		setStreamServer(value.substr(0, value.size() - 13));
	}
}

PLSPlatformApiResult PLSPlatformTwitch::getApiResult(int code, QNetworkReply::NetworkError error)
{
	auto result = PLSPlatformApiResult::PAR_SUCCEED;

	if (QNetworkReply::NoError == error) {

	} else if (QNetworkReply::UnknownNetworkError >= error) {
		result = PLSPlatformApiResult::PAR_NETWORK_ERROR;
	} else {
		switch (code) {
		case 401:
			result = PLSPlatformApiResult::PAR_TOKEN_EXPIRED;
			break;
		case 403:
			result = PLSPlatformApiResult::PAR__API_ERROR_FORBIDDEN;
			break;
		default:
			result = PLSPlatformApiResult::PAR_API_FAILED;
			break;
		}
	}

	return result;
}

void PLSPlatformTwitch::showApiRefreshError(PLSPlatformApiResult value)
{
	auto alertParent = getAlertParent();

	switch (value) {
	case PLSPlatformApiResult::PAR_NETWORK_ERROR:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Network.Error"));
		break;
	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED:
	case PLSPlatformApiResult::PAR__API_ERROR_FORBIDDEN: {
		auto dialogResult = PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Twitch.Expired"));
		if (PLS_PLATFORM_API->isPrepareLive()) {
			emit closeDialogByExpired();
		}
		if (QDialogButtonBox::Ok == dialogResult) {
			PLSCHANNELS_API->channelExpired(getChannelUUID(), false);
		}
	} break;
	default:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Twitch.Failed"));
		break;
	}
}

void PLSPlatformTwitch::showApiUpdateError(PLSPlatformApiResult value)
{
	auto alertParent = getAlertParent();

	switch (value) {
	case PLSPlatformApiResult::PAR_NETWORK_ERROR:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Update.Network.Error"));
		break;
	case PLSPlatformApiResult::PAR_TOKEN_EXPIRED:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Update.Twitch.Expired"));
		break;
	default:
		PLSAlertView::warning(alertParent, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Update.Twitch.Failed"));
		break;
	}
}

void PLSPlatformTwitch::requestChannel(bool showAlert)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			auto root = doc.object();
			setChannelId(root["_id"].toString().toStdString())
				.setUserName(root["name"].toString().toStdString())
				.setDisplayName(root["display_name"].toString().toStdString())
				.setTitle(root["status"].toString().toStdString())
				.setCategory(root["game"].toString().toStdString())
				.setStreamKey(root["stream_key"].toString().toStdString());

			m_strOriginalTitle = getTitle();

			emit onGetChannel(PLSPlatformApiResult::PAR_SUCCEED);

			requestServer(showAlert);
		} else {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%s", code, QString(data).toStdString().c_str());

			if (showAlert) {
				PLSAlertView::warning(getAlertParent(), QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Twitch.Failed"));
			}
			emit onGetChannel(PLSPlatformApiResult::PAR_API_FAILED);
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%d", code, error);

		auto result = getApiResult(code, error);
		if (showAlert) {
			showApiRefreshError(result);
		}
		emit onGetChannel(result);
	};

	PLSNetworkReplyBuilder builder(TWITCH_API_BASE "/kraken/channel");
	builder.setContentType(HTTP_CONTENT_TYPE_URL_ENCODED_VALUE).setRawHeaders({{"Client-ID", TWITCH_CLIENT_ID}, {"Authorization", "OAuth " + getChannelToken()}, {"Accept", HTTP_ACCEPT_TWITCH}});
	PLS_HTTP_HELPER->connectFinished(builder.get(), this, _onSucceed, _onFail);
}

void PLSPlatformTwitch::requestServer(bool showAlert)
{
	if (!m_vecTwitchServers.empty()) {
		emit onGetServer(PLSPlatformApiResult::PAR_SUCCEED);
		return;
	}

	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			auto root = doc.object();
			auto ingests = root["ingests"].toArray();
			auto _idConfig = config_get_uint(App()->GlobalConfig(), KeyConfigLiveInfo, KeyTwitchServer);

			m_vecTwitchServers.clear();
			for (int i = 0; i < ingests.count() - 1; ++i) {
				auto server = ingests[i].toObject();
				auto name = server["name"].toString().toStdString();
				if (0 == i) {
					name = QTStr("LiveInfo.Twitch.Recommend").toStdString() + name;
				}
				auto _id = server["_id"].toInt();
				if (_idConfig == _id) {
					m_idxServer = i;
				}
				m_vecTwitchServers.push_back({_id, name, server["url_template"].toString().toStdString(), server["default"].toBool()});
			}

			saveStreamServer();

			emit onGetServer(PLSPlatformApiResult::PAR_SUCCEED);
		} else {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%s", code, QString(data).toStdString().c_str());

			if (showAlert) {
				PLSAlertView::warning(getAlertParent(), QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Twitch.Failed"));
				emit onGetServer(PLSPlatformApiResult::PAR_API_FAILED);
			}
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%d", code, error);

		auto result = getApiResult(code, error);
		if (showAlert) {
			showApiRefreshError(result);
			emit onGetServer(result);
		}
	};

	//PLSNetworkReplyBuilder builder(TWITCH_API_BASE "/kraken/ingests/");
	//builder.setContentType(HTTP_CONTENT_TYPE_URL_ENCODED_VALUE).setRawHeaders({{"Client-ID", TWITCH_CLIENT_ID}, {"Accept", HTTP_ACCEPT_TWITCH}});
	PLSNetworkReplyBuilder builder(TWITCH_API_INGESTS);
	PLS_HTTP_HELPER->connectFinished(builder.get(), this, _onSucceed, _onFail);
}

void PLSPlatformTwitch::requestUpdateChannel(const string &title, const string &category)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			auto root = doc.object();

			setTitle(title);
			setCategory(category);

			emit onUpdateChannel(PLSPlatformApiResult::PAR_SUCCEED);
		} else {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%s", code, QString(data).toStdString().c_str());

			PLSAlertView::warning(getAlertParent(), QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Update.Twitch.Failed"));
			emit onUpdateChannel(PLSPlatformApiResult::PAR_API_FAILED);
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%d", code, error);

		auto result = getApiResult(code, error);
		showApiUpdateError(result);
		emit onUpdateChannel(result);
	};

	auto url = QString(TWITCH_API_BASE "/kraken/channels/%1").arg(QString::fromStdString(getChannelId()));
	PLSNetworkReplyBuilder builder(url);
	builder.setContentType(HTTP_CONTENT_TYPE_URL_ENCODED_VALUE)
		.setRawHeaders({{"Client-ID", TWITCH_CLIENT_ID}, {"Authorization", "OAuth " + getChannelToken()}, {"Accept", HTTP_ACCEPT_TWITCH}})
		.addField("channel[status]", title.c_str())
		.addField("channel[game]", category.c_str());
	PLS_HTTP_HELPER->connectFinished(builder.put(), this, _onSucceed, _onFail);
}

void PLSPlatformTwitch::requestStatisticsInfo()
{
	if (PLS_PLATFORM_API->isPrismLive() || !isActive()) {
		return;
	}

	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			auto data = doc.object();
			auto viewers = data["stream"].toObject()["viewers"].toInt();
			PLSCHANNELS_API->setValueOfChannel(getChannelUUID(), ChannelData::g_viewers, QString::number(viewers));
		} else {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%s", code, QString(data).toStdString().c_str());
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%d", code, error);

		auto result = getApiResult(code, error);
	};

	auto url = QString(TWITCH_API_BASE "/kraken/streams/%1").arg(QString::fromStdString(getChannelId()));
	PLSNetworkReplyBuilder builder(url);
	builder.setContentType(HTTP_CONTENT_TYPE_URL_ENCODED_VALUE).setRawHeaders({{"Client-ID", TWITCH_CLIENT_ID}, {"Authorization", "OAuth " + getChannelToken()}, {"Accept", HTTP_ACCEPT_TWITCH}});
	PLS_HTTP_HELPER->connectFinished(builder.get(), this, _onSucceed, _onFail);
}

void PLSPlatformTwitch::requestCategory(const QString &query)
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			auto root = doc.object();

			emit onGetCategory(root, query);
		} else {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%s", code, QString(data).toStdString().c_str());

			emit onGetCategory(QJsonObject(), query);
		}
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%d", code, error);

		emit onGetCategory(QJsonObject(), query);
	};

	auto url = QString(TWITCH_API_BASE "/kraken/search/games?query=%1").arg(query);
	PLSNetworkReplyBuilder builder(url);
	builder.setContentType(HTTP_CONTENT_TYPE_URL_ENCODED_VALUE).setRawHeaders({{"Client-ID", TWITCH_CLIENT_ID}, {"Accept", HTTP_ACCEPT_TWITCH}});
	PLS_HTTP_HELPER->connectFinished(builder.get(), this, _onSucceed, _onFail);
}

void PLSPlatformTwitch::requestVideos()
{
	auto _onSucceed = [=](QNetworkReply *networkReplay, int code, QByteArray data, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		auto doc = QJsonDocument::fromJson(data);
		if (doc.isObject()) {
			auto root = doc.object()["data"].toArray();
			if (!root.isEmpty()) {
				m_strEndUrl = root[0].toObject()["url"].toString();
			}
		} else {
			PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%s", code, QString(data).toStdString().c_str());
		}

		liveStoppedCallback();
	};

	auto _onFail = [=](QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context) {
		Q_UNUSED(networkReplay)
		Q_UNUSED(context)

		PLS_ERROR(MODULE_PlatformService, __FUNCTION__ ".error: %d-%d", code, error);

		liveStoppedCallback();
	};

	PLSNetworkReplyBuilder builder("https://api.twitch.tv/helix/videos");
	builder.setRawHeaders({{"Client-ID", TWITCH_CLIENT_ID}, {"Authorization", "Bearer " + getChannelToken()}}).addQuery("user_id", QString::fromStdString(getChannelId()));
	PLS_HTTP_HELPER->connectFinished(builder.get(), this, _onSucceed, _onFail);
}

QJsonObject PLSPlatformTwitch::getLiveStartParams()
{
	QJsonObject platform(__super::getLiveStartParams());

	platform["simulcastChannel"] = QString::fromStdString(getDisplayName());

	return platform;
}

QJsonObject PLSPlatformTwitch::getWebChatParams()
{
	QJsonObject platform(__super::getWebChatParams());

	platform["clientId"] = TWITCH_CLIENT_ID;

	return platform;
}

QString PLSPlatformTwitch::getServiceLiveLink()
{
	return m_strEndUrl;
}
