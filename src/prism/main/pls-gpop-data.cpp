#include "pls-gpop-data.hpp"
#include "pls-common-define.hpp"
#include "json-data-handler.hpp"
#include "log/log.h"
#include "log/module_names.h"
#include "vlive/PLSAPIVLive.h"
#include <QApplication>
#include "TextMotionTemplateDataHelper.h"

#define PRISM_GPOP_PATH QString("%1/data/prism-studio/user/gpop.json").arg(m_appDir)


PLSGpopData *PLSGpopData::instance()
{
	static PLSGpopData gpopdata;
	return &gpopdata;
}

void PLSGpopData::getGpopDataRequest()
{
	TextMotionRemoteDataHandler::instance()->getTMRemoteDataRequest("");
}

PLSGpopData::PLSGpopData(QObject *parent) : QObject(parent)
{
	connect(this, &PLSGpopData::finished, this, &PLSGpopData::initGpopData, Qt::QueuedConnection);
	m_appDir = QApplication::applicationDirPath();
}

PLSGpopData::~PLSGpopData() {}

void PLSGpopData::onReplyResultData(int statusCode, const QString &url, const QByteArray array)
{
	if (m_gpopURL == url && statusCode == HTTP_STATUS_CODE_200) {
		m_gpopDataArray = array;
		PLSJsonDataHandler::saveJsonFile(array, pls_get_user_path(PRISM_GPOP_PATH));
		emit finished();
	}
}

void PLSGpopData::onReplyErrorData(int statusCode, const QString &url, const QString &body, const QString &errorInfo)
{
	Q_UNUSED(url);
	Q_UNUSED(errorInfo);
	if (m_gpopURL == url) {
		emit finished();
	}
}

Connection PLSGpopData::getConnection()
{
	if (m_connection.ssl.isEmpty()) {
		PLSJsonDataHandler::getJsonArrayFromFile(m_gpopDataArray, pls_get_user_path(PRISM_GPOP_PATH));
		initConnection();
	}
	return m_connection;
}
Common PLSGpopData::getCommon()
{
	if (m_common.deviceType.isEmpty()) {
		PLSJsonDataHandler::getJsonArrayFromFile(m_gpopDataArray, pls_get_user_path(PRISM_GPOP_PATH));
		initCommon();
	}
	return m_common;
}

void PLSGpopData::initCommon()
{
	QVariantMap map;
	QVariant value;
	PLSJsonDataHandler::getValuesFromByteArray(m_gpopDataArray, "optional", map);
	Common common;
	common.stage = map.value(name2str(common)).toMap().value(name2str(stage)).toString();
	common.regionCode = map.value(name2str(common)).toMap().value(name2str(regionCode)).toString();
	common.deviceType = map.value(name2str(common)).toMap().value(name2str(deviceType)).toString();
	m_common = common;
}

void PLSGpopData::initSnscallbackUrls()
{
	QMap<QString, SnsCallbackUrl> snscallbackUrls;
	QVariantMap map;
	PLSJsonDataHandler::getValuesFromByteArray(m_gpopDataArray, name2str(snsCallbackUrls), map);
	for (auto key : map.keys()) {
		SnsCallbackUrl callbackUrl;
		callbackUrl.callbackUrlType = key;
		callbackUrl.url = map.value(key).toString();
		snscallbackUrls.insert(key, callbackUrl);
	}
	m_snsCallbackUrls = snscallbackUrls;
}

void PLSGpopData::initOpenLicenseUrl()
{
	m_openSourceLicenseMap.clear();
	QVariantMap map;
	PLSJsonDataHandler::getValuesFromByteArray(m_gpopDataArray, name2str(openSourceLicense), map);
	for (auto key : map.keys()) {
		QString url = map.value(key).toString();
		m_openSourceLicenseMap.insert(key, url);
	}
}

void PLSGpopData::initRtmpDestination()
{
	QMap<int, RtmpDestination> rtmpDestinations;
	QVariantMap map;
	QVariantList list;
	PLSJsonDataHandler::getValuesFromByteArray(m_gpopDataArray, name2str(rtmpDestination), list);
	for (auto map : list) {
		RtmpDestination rtmpDestionation;
		rtmpDestionation.streamName = map.toMap().value(name2str(streamName)).toString();
		rtmpDestionation.rtmpUrl = map.toMap().value(name2str(rtmpUrl)).toString();
		rtmpDestionation.exposure = map.toMap().value(name2str(exposure)).toBool();
		rtmpDestionation.order = map.toMap().value(name2str(order)).toInt();
		if (rtmpDestionation.exposure) {
			rtmpDestinations.insert(rtmpDestionation.order, rtmpDestionation);
		}
		bool hasResolutionLimit = map.toMap().value(name2str(hasResolutionLimit)).toBool();
		if (rtmpDestionation.exposure && hasResolutionLimit) {
			QString idName = map.toMap().value(name2str(id)).toString();
			QMap resolutionFps = map.toMap().value(name2str(resolutionFps)).toMap();
			m_rtmpFPSMap.insert(idName, resolutionFps);
		}
	}
	m_destinations = rtmpDestinations;
}

void PLSGpopData::initConnection()
{
	QVariantMap map;
	PLSJsonDataHandler::getValuesFromByteArray(m_gpopDataArray, "apis", map);
	Connection connection;
	connection.ssl = map.value(name2str(ssl)).toString();
	connection.url = map.value(name2str(url)).toString();
	connection.fallback.ssl = map.value(name2str(fallback)).toMap().value(name2str(ssl)).toString();
	connection.fallback.url = map.value(name2str(fallback)).toMap().value(name2str(url)).toString();
	m_connection = connection;
}

void PLSGpopData::initVliveNotice()
{
	QVariantMap map;
	PLSJsonDataHandler::getValuesFromByteArray(m_gpopDataArray, "vliveReorganizationNotice", map);
	VliveNotice liveNotice;
	liveNotice.version = map.value(name2str(version)).toString();
	liveNotice.isNeedNotice = map.value(name2str(needNotice)).toBool();
	liveNotice.pageUrl = map.value(name2str(noticePageUrl)).toString();
	m_vliveNotice = liveNotice;
}


QMap<int, RtmpDestination> PLSGpopData::getRtmpDestination()
{
	if (0 == m_destinations.size()) {
		PLSJsonDataHandler::getJsonArrayFromFile(m_gpopDataArray, PRISM_GPOP_PATH);
		initRtmpDestination();
	}
	return m_destinations;
}
QMap<QString, QString> &PLSGpopData::getOpenSourceLicenseMap()
{
	return m_openSourceLicenseMap;
}

VliveNotice PLSGpopData::getVliveNotice()
{
	if (m_vliveNotice.pageUrl.isEmpty()) {
		PLSJsonDataHandler::getJsonArrayFromFile(m_gpopDataArray, pls_get_user_path(PRISM_GPOP_PATH));
		initVliveNotice();
	}
	return m_vliveNotice;
}

const QVariantMap &PLSGpopData::rtmpFPSMap()
{
	return m_rtmpFPSMap;
}

void PLSGpopData::initGpopData()
{
	PLSJsonDataHandler::getJsonArrayFromFile(m_gpopDataArray, pls_get_user_path(PRISM_GPOP_PATH));
	initCommon();
	initSnscallbackUrls();
	initOpenLicenseUrl();
	initRtmpDestination();
	initConnection();
}
