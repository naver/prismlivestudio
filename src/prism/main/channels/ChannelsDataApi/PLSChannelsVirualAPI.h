#pragma once
#include <QJsonDocument>
#include <QString>
#include <QVariantMap>
class QNetworkReply;
using ReplyPtrs = QSharedPointer<QNetworkReply>;
class LoadingFrame;
using InfosList = QList<QVariantMap>;

/* macro just for develop tip when any function in progress */
#define API_TIP                                                \
	QMessageBox::information(nullptr, QObject::tr("Tips"), \
				 QObject::tr("this function should be done in file %1 \n line %2 \n function %3").arg(QString(__FILE__), QString::number(__LINE__), QString(__FUNCTION__)));

/* API for any to show live info of give json */
void showLiveInfo(const QString &uuid);

void showChannelInfo(const QString &uuid);

/* API for show share view */
void showShareView(const QString &channelUUID);

int showSummaryView();
void showChatView(bool isRebackLogin = false, bool isOnlyShow = false, bool isOnlyInit = false);

void addLoginChannel(const QJsonObject &retJson);
bool updateChannelInfoFromNet(const QString &uuid);

void sortInfosByNickName(InfosList &infos);

bool handleChannelStatus(const QVariantMap &info);
bool updateChannelTypeFromNet(const QString &uuid);
bool updateRTMPTypeFromNet(const QString &uuid);

QVariantMap createErrorMap(int errorType);
void addErrorForType(int errorType);

void refreshChannel(const QString &uuid);

void resetChannel(const QString &uuid);
void resetExpiredChannel(const QString &uuid, bool toAsk = true);
void reloginChannel(const QString &platformName, bool toAsk = true);

template<typename ReplyType> bool isPrismReplyExpired(ReplyType reply, const QByteArray &data)
{
	int netCode = getReplyStatusCode(reply);
	auto doc = QJsonDocument::fromJson(data);
	int code = 0;
	if (!doc.isNull()) {
		auto obj = doc.object();
		if (!obj.isEmpty()) {
			code = obj.value("code").toInt();
		}
	}
	if (netCode == 401 && code == 3000) {
		return true;
	}
	return false;
}

void reloginPrismExpired();

void resetAllChannels();

void refreshAllChannels();

void handleEmptyChannel(const QString &uuid);

void addRTMP();
void editRTMP(const QString &uuid);
void runCMD(const QString &cmdStr);

bool checkChannelsState();

bool startStreamingCheck();
bool toGoLive();

bool stopStreamingCheck();
bool toStopLive();

bool startRecordCheck();
bool toRecord();

bool stopRecordCheck();
bool toTryStopRecord();

void childExclusive(const QString &channelID);

bool isExclusiveChannel(const QString &uuid);
bool isExclusiveChannel(const QVariantMap &info);
bool isEnabledExclusiveChannelExist();

QString findExistEnabledExclusiveChannel();

void exclusiveChannelCheckAndVerify(const QVariantMap &Newinfo);

void disableAll();

/* API for create and  show Busy view */
LoadingFrame *createBusyFrame(QWidget *parent = nullptr);

void showNetworkErrorAlert();

void showChannelsSetting(int index = 0);

void showChannelsSetting(const QString &platform);
