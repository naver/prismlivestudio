/*
 * @fine      PLSNetworkAccessManager
 * @brief     https request and response control manager;
                Create different requests based on request type ,
 * @date      2019-09-27
 * @author    Bing Cheng
 * @attention

 * @version   v1.0
 * @modify
 */

#ifndef PLSNETWORKACCESSMANAGER_H
#define PLSNETWORKACCESSMANAGER_H

#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVariantMap>

class PLSNetworkAccessManager : public QNetworkAccessManager {
	Q_OBJECT
public:
	PLSNetworkAccessManager(QObject *parent = nullptr);
	~PLSNetworkAccessManager();

	static PLSNetworkAccessManager *getInstance();
	/**
     * @brief createHttpRequest
     * @param op:request type get;put; post etc;
     * @param url:request url
     * @param isEncode:is or not hamc
     * @param headData: head params
     * @param sendData: data params
     */
	QNetworkReply *createHttpRequest(Operation op, const QString &url, bool isEncode = false, const QVariantMap &headData = QVariantMap(), const QVariantMap &sendData = QVariantMap(),
					 bool isGcc = true);
	/**
     * @brief getCookieForUrl: get cookies with url
     * @param url
     * @return :all cookies
     */
	QList<QNetworkCookie> getCookieForUrl(const QString &url);

	/**
     * @brief set single Cookie in http header
     * @param key
     * @param value
     */
	void setCookieToHttpHeader(const QString &key, const QString &value, QVariantMap &headerMap);

	/**
     * @brief set multiple cookies in http header
     * @param cookiesMap
     */
	void setCookiesToHttpHeader(const QMap<QString, QString> cookiesMap, QVariantMap &headerMap);

	/**
     * @brief set post request multipart object
     * @param multi_part_ multi form object
     */
	void setPostMultiPart(QHttpMultiPart *multi_part_);
signals:
	/**
     * @brief replyResultData notify response data
     * @param statusCode response status code
     * @param url source url
     * @param array response data
     */
	void replyResultData(int statusCode, const QString &url, const QByteArray array);
	/**
     * @brief replyErrorData notify response error info
     * @param url source url
     * @param error error content
     */
	void replyErrorData(const QString &url, const QString error);
	void replyErrorDataWithSatusCode(int statusCode, const QString &url, const QString &body, const QString &errorInfo);

	void sessionExpired();

private:
	/**
     * @brief setQueryData
     * @param query httpRequest query object
     * @param data query param
     */
	void setQueryData(QUrlQuery &query, const QVariantMap &data);
	/**
     * @brief requestUrlHeadHandler set httprequest head params
     * @param isEncode is or not hmac
     * @param headData head params
     * @param httpRequest
     * @param url
     */
	void requestUrlHeadHandler(const QVariantMap &headData, QNetworkRequest &httpRequest, bool isGcc);
	/**
     * @brief setHmacUrlHandler
     * @param isEncode
     * @param url
     */
	void setHmacUrlHandler(bool isEncode, QUrl &url);
	/**
     * @brief httpGetRequest: get request handler
     * @param sendData
     * @param isEncode
     * @param httpRequest
     * @param url
     * @return
     */

	QNetworkReply *httpGetRequest(const QVariantMap &sendData, bool isEncode, QNetworkRequest &httpRequest, QUrl &url);

	/**
     * @brief httpDeleteRequest: delete request handler
     * @param sendData
     * @param isEncode
     * @param httpRequest
     * @param url
     * @return
     */
	QNetworkReply *httpDeleteRequest(const QVariantMap &sendData, bool isEncode, QNetworkRequest &httpRequest, QUrl &httpUrl);

	/**
     * @brief httpPostRequest: post request handler
     * @param sendData
     * @param httpRequest
     * @param httpUrl
     * @return
     */
	QNetworkReply *httpPostRequest(const QVariantMap &sendData, bool isEncode, QNetworkRequest &httpRequest, QUrl &httpUrl);

	/**
     * @brief httpPostRequest: post request handler
     * @param sendData
     * @param httpRequest
     * @param httpUrl
     * @return
     */
	QNetworkReply *httpPutRequest(const QVariantMap &sendData, bool isEncode, QNetworkRequest &httpRequest, QUrl &httpUrl);

	/**
     * @brief httpResponseHandler http response handler
     * @param reply
     * @param requestHttpUrl
     */
	void httpResponseHandler(QNetworkReply *reply, const QString &requestHttpUrl);
	/**
     * @brief getBodyfromMap get body value
     * @param bodyMap
     * @return
     */
	QByteArray getBodyfromMap(const QVariantMap &bodyMap);

	void setCookieToList(const QString &key, const QString &value, QList<QNetworkCookie> &cookieList);
	QMap<QString, QList<QNetworkCookie>> m_cookies;
	QHttpMultiPart *m_multiPart;
	QString httpHead;
	QString httpData;
};

#endif // PLSNETWORKACCESSMANAGER_H
