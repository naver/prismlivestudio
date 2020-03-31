#include "PLSAPIVLive.h"
#include <qfileinfo.h>
#include <quuid.h>
#include <QNetworkCookieJar>
#include <vector>
#include "../PLSPlatformBase.hpp"
#include "ChannelCommonFunctions.h"
#include "PLSChannelDataAPI.h"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "PLSPlatformApi.h"
#include "PLSPlatformVLive.h"
#include "frontend-api.h"
#include "pls-common-define.hpp"

using namespace std;
const static QString strVersion = "1";
static const QString IMAGE_FILE_NAME_PREFIX = "vlive-";

PLSAPIVLive::PLSAPIVLive(QObject *parent) : QObject(parent) {}

void PLSAPIVLive::vliveRequestGccAndLanguage(const QObject *receiver, dataErrorFunction onFinish)
{
}

void PLSAPIVLive::vliveRequestUsersInfoAndChannel(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed)
{
}

void PLSAPIVLive::vliveRequestSchedule(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, void *const context)
{

}

void PLSAPIVLive::vliveRequestStartLive(const QObject *receiver, dataErrorFunction onFinish)
{
}

void PLSAPIVLive::vliveRequestStatistics(const QObject *receiver, dataErrorFunction onFinish)
{
}

void PLSAPIVLive::vliveRequestStopLive(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed)
{
	
}

void PLSAPIVLive::uploadImage(const QObject *receiver, const QString &imageFilePath, UploadImageCallback callback)
{
	
}

void PLSAPIVLive::downloadImageAsync(const QObject *receiver, const QString &imageUrl, ImageCallback callback, void *const context, const QString &prefix, bool ingoreCache)
{
}

QString PLSAPIVLive::getLocalImageFile(const QString &imageUrl, const QString &prefix)
{
	QString localFile;
	return localFile;
}

QPair<bool, QString> PLSAPIVLive::downloadImageSync(const QObject *receive, const QString &url, const QString &prefix, bool ingoreCache)
{
	PLSNetworkReplyBuilder builder(url);
	return PLSHttpHelper::downloadImageSync(builder.get(), receive, "", "", "");
}

void PLSAPIVLive::addCommenQuery(PLSNetworkReplyBuilder &builder)
{
}

void PLSAPIVLive::addCommenCookie(PLSNetworkReplyBuilder &builder)
{
}

void PLSAPIVLive::addMacAddress(PLSNetworkReplyBuilder &builder)
{

}

void PLSAPIVLive::addUserAgent(PLSNetworkReplyBuilder &builder)
{
}

bool PLSAPIVLive::isVliveFanship()
{
	return false;
}

QString PLSAPIVLive::getVliveHost()
{
	QString ssl = pls_get_gpop_connection().ssl;
	return ssl;
}

QString PLSAPIVLive::getStreamName()
{
	QString streamName = "";
	return streamName;
}
