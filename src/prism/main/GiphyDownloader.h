#ifndef GIPHYDOWNLOADER_H
#define GIPHYDOWNLOADER_H

#include <QDebug>
#include <QThread>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QMutex>
#include <QTimer>
#include <QQueue>
#include "GiphyDefine.h"

class GiphyDownloader : public QObject {
	Q_OBJECT
	using NetworkAccessible = QNetworkAccessManager::NetworkAccessibility;

public:
	static GiphyDownloader *instance();
	void Get(const DownloadTaskData &taskData);
	void Start();
	bool IsRunning();
	static bool IsDownloadFileExsit(const DownloadTaskData &task, QString &fileName);

private slots:
	void excuteTask(const DownloadTaskData &taskData);
	void sslErrors(QList<QSslError>);
	void downloadFinished(QNetworkReply *reply);
	void OnNetworkAccessChanged(NetworkAccessible accessible);

signals:
	void downloadResult(const TaskResponData &result);
	void downloadProgress(const TaskResponData &result, qint64 bytesReceived, qint64 bytesTotal);

private:
	bool saveToDisk(QString &filename, QNetworkReply *reply);
	static QString saveFileName(const QUrl &url, const QString &id, const QString &tail);
	void ClearTask();

private:
	explicit GiphyDownloader(QObject *parent = nullptr);
	~GiphyDownloader() override;

private:
	QNetworkAccessManager *manager{nullptr};
	QMap<QNetworkReply *, DownloadTaskData> taskDownloads;
	QQueue<DownloadTaskData> tasksRetry;
	QThread threadDownload;
	bool running{false};
};
#endif // GIPHYDOWNLOADER_H
