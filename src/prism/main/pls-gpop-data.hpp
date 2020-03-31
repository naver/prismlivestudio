#ifndef PLSGPOPDATA_H
#define PLSGPOPDATA_H

#include <QObject>

#include "network-access-manager.hpp"
#include "pls-net-url.hpp"
#include "pls-gpop-data-struct.hpp"

class PLSBasic;

class PLSGpopData : public QObject {
	Q_OBJECT

public:
	static PLSGpopData *instance();
	void getGpopDataRequest();

public:
	Common getCommon();
	QMap<int, RtmpDestination> getRtmpDestination();
	Connection getConnection();
	const QVariantMap &rtmpFPSMap();
	QMap<QString, QString> &getOpenSourceLicenseMap();
	VliveNotice getVliveNotice();

private:
	void initCommon();
	void initSnscallbackUrls();
	void initOpenLicenseUrl();
	void initRtmpDestination();
	void initConnection();
	void initVliveNotice();

private:
	explicit PLSGpopData(QObject *parent = nullptr);
	~PLSGpopData();

private slots:

	void initGpopData();
	/**
     * @brief the reponse of http request
     */
	void onReplyResultData(int statusCode, const QString &url, const QByteArray array);
	/**
     * @brief the error reponse of http request
     */
	void onReplyErrorData(int statusCode, const QString &url, const QString &body, const QString &errorInfo);

signals:
	void finished();
	void categoryFinished();

private:
	QByteArray m_gpopDataArray;
	QString m_libraryUrl;
	Common m_common;
	QMap<QString, SnsCallbackUrl> m_snsCallbackUrls;
	QMap<int, RtmpDestination> m_destinations;
	Connection m_connection;
	QVariantMap m_rtmpFPSMap;
	QMap<QString, QString> m_openSourceLicenseMap;
	QString m_gpopURL;
	VliveNotice m_vliveNotice;
	QString m_appDir;
};

#endif // PLSGPOPDATA_H
