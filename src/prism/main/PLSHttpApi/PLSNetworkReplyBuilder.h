/*
* @file		PLSNetworkReplyBuilder.h
* @brief	A help class to build QNetworkRequest
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
*/

#pragma once

#include <functional>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;

class PLSNetworkReplyBuilder {
public:
	PLSNetworkReplyBuilder();
	PLSNetworkReplyBuilder(const QString &value);
	PLSNetworkReplyBuilder(const QUrl &value);

	PLSNetworkReplyBuilder &setUrl(const QString &value);
	PLSNetworkReplyBuilder &setUrl(const QUrl &value);
	const QUrl &getUrl() { return url; };

	PLSNetworkReplyBuilder &addKnownHeader(const QNetworkRequest::KnownHeaders &key, const QVariant &value);
	PLSNetworkReplyBuilder &setKnownHeaders(const QMap<QNetworkRequest::KnownHeaders, QVariant> &headers);

	PLSNetworkReplyBuilder &setCookie(const QVariant &value);
	PLSNetworkReplyBuilder &setContentType(const QString &value);

	PLSNetworkReplyBuilder &addRawHeader(const QString &key, const QVariant &value);
	PLSNetworkReplyBuilder &setRawHeaders(const QVariantMap &headers);

	PLSNetworkReplyBuilder &addQuery(const QString &key, const QVariant &value);
	PLSNetworkReplyBuilder &setQuerys(const QVariantMap &querys);

	PLSNetworkReplyBuilder &setBody(const QByteArray &value);

	PLSNetworkReplyBuilder &addField(const QString &key, const QVariant &value);
	PLSNetworkReplyBuilder &setFields(const QVariantMap &fields);

	PLSNetworkReplyBuilder &addJsonObject(const QString &key, const QVariant &value);
	PLSNetworkReplyBuilder &setJsonObject(const QVariantMap &jsons);
	PLSNetworkReplyBuilder &setJsonObject(const QJsonObject &jsons);

	PLSNetworkReplyBuilder &addJsonArray(const QVariant &value);
	PLSNetworkReplyBuilder &setJsonArray(const QVariantList &jsons);
	PLSNetworkReplyBuilder &setJsonArray(const QJsonArray &jsons);

	QNetworkReply *get(QNetworkAccessManager *networkAccessManager = nullptr);
	QNetworkReply *post(QNetworkAccessManager *networkAccessManager = nullptr);
	QNetworkReply *put(QNetworkAccessManager *networkAccessManager = nullptr);
	QNetworkReply *del(QNetworkAccessManager *networkAccessManager = nullptr);

protected:
	virtual QUrl buildUrl(const QUrl &url);
	virtual QNetworkRequest *buildHeader(QNetworkRequest *request);
	virtual QUrlQuery buildQuery(const QVariantMap &value);
	virtual QByteArray buildBody();
	virtual QNetworkRequest buildRequest();

protected:
	QUrl url;

	QMap<QNetworkRequest::KnownHeaders, QVariant> knownheaders;
	QVariantMap rawHeaders;
	QVariantMap querys;
	QByteArray body;
	QVariantMap fields;
	QJsonObject jsonObject;
	QJsonArray jsonArray;
};
