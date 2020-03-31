/*
* @file		PLSHttpHelper.h
* @brief	A helper class for http request
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
*/

#pragma once

#include <functional>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "PLSNetworkReplyBuilder.h"
#include "PLSHmacNetworkReplyBuilder.h"

#include "pls-net-url.hpp"

#define MODULE_PLSHttpHelper "PLSHttpHelper"

using namespace std;
using dataFunction = function<void(QNetworkReply *networkReplay, int code, QByteArray data, void *context)>;
using dataErrorFunction = function<void(QNetworkReply *networkReplay, int code, QByteArray data, QNetworkReply::NetworkError error, void *context)>;
using ImageCallback = function<void(bool ok, const QString &imagePath, void *context)>;
using ImagesCallback = function<void(const QList<QPair<bool, QString>> &imageList, void *context)>;

class PLSHttpHelper {
public:
	static PLSHttpHelper *instance();

public:
	QNetworkAccessManager *getNetworkAccessManager() { return &networkAccessManager; }

	static void connectFinished(QNetworkReply *networkReplay, const QObject *receiver = nullptr, dataFunction onSucceed = nullptr, dataErrorFunction onFailed = nullptr,
				    dataErrorFunction onFinished = nullptr, void *const context = nullptr, int iTimeout = PRISM_NET_REQUEST_TIMEOUT, bool bPrintLog = true);

	// download image
	static void downloadImageAsync(QNetworkReply *reply, const QObject *receiver, const QString &saveDir, ImageCallback callback, const QString &saveFileNamePrefix,
				       const QString &saveFileName = QString(), const QString &defaultImagePath = QString(), void *context = nullptr);
	static QPair<bool, QString> downloadImageSync(QNetworkReply *reply, const QObject *receiver, const QString &saveDir, const QString &saveFileNamePrefix, const QString &saveFileName = QString(),
						      const QString &defaultImagePath = QString());
	static void downloadImagesAsync(QList<QNetworkReply *> &replyList, const QObject *receiver, const QString &saveDir, ImagesCallback callback, const QString &saveFileNamePrefix,
					const QList<QString> &saveFileNameList = QList<QString>(), const QString &defaultImagePath = QString(), void *context = nullptr);
	static QList<QPair<bool, QString>> downloadImagesSync(QList<QNetworkReply *> &replyList, const QObject *receiver, const QString &saveDir, const QString &saveFileNamePrefix,
							      const QList<QString> &saveFileNameList = QList<QString>(), const QString &defaultImagePath = QString());

	static QString getLocalIPAddr();
	static QString getUserAgent();

	static QString getFileName(const QString &filePath);
	static QString imageContentTypeToFileName(const QString &contentType, const QString &fileName);
	static QString imageFileNameToContentType(const QString &fileName);

private:
	QNetworkAccessManager networkAccessManager;
};

#define PLS_HTTP_HELPER PLSHttpHelper::instance()
