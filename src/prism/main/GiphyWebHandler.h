#ifndef GIPHYWEBHANDLER_H
#define GIPHYWEBHANDLER_H

#include <QObject>
#include <QQueue>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <GiphyDefine.h>
#include <QMutex>
#include <QThread>
#include <QTimer>

class GiphyWebHandler : public QObject {
	Q_OBJECT

public:
	explicit GiphyWebHandler(QObject *parent = nullptr);
	~GiphyWebHandler();

	static bool isHttpRedirect(QNetworkReply *reply);
	void Get(RequestTaskData task);
	void DiscardTask();

private:
	bool DealResponData(QNetworkReply *reply, ResponData &data);
	void GetImageInfo(const QJsonObject &imgObj, GiphyData &data);
	void Append(const RequestTaskData &task);

public slots:
	void GiphyFetch(RequestTaskData task);
	void ClearTask();

private slots:
	void StartNextRequest();
	void RequestFinished(QNetworkReply *reply);

signals:
	void FetchResult(const ResponData &data);
	void FetchError(const RequestTaskData &task, const RequestErrorInfo &errorInfo);
	void networkAccssibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
	void LoadingVisible(const RequestTaskData &task, bool visible);

private:
	QNetworkAccessManager *manager{nullptr};
	QQueue<QPair<QUrl, RequestTaskData>> requestQueue;
	QPair<QUrl, RequestTaskData> currentRequestTask;
	QMutex taskMutex;
};

#endif // GIPHYWEBHANDLER_H
