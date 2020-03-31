#include "GiphyDownloader.h"
#include <QThreadPool>
#include "GiphyWebHandler.h"
#include <pls-common-define.hpp>
#include <frontend-api\frontend-api.h>
#include "log/module_names.h"
#include "liblog.h"

static const char *CONTENT_TYPE = "image/gif";

GiphyDownloader::GiphyDownloader(QObject *parent) : QObject(parent) {}

GiphyDownloader::~GiphyDownloader()
{
	threadDownload.quit();
	threadDownload.wait();

	if (nullptr != manager)
		manager->deleteLater();
	ClearTask();
}

GiphyDownloader * ::GiphyDownloader::instance()
{
	static GiphyDownloader downloader;
	return &downloader;
}

void GiphyDownloader::Get(const DownloadTaskData &taskData)
{
	QMetaObject::invokeMethod(this, "excuteTask", Qt::QueuedConnection, Q_ARG(const DownloadTaskData &, taskData));
}

void GiphyDownloader::Start()
{
	if (running)
		return;
	running = true;
	this->moveToThread(&threadDownload);
	threadDownload.start();
}

bool GiphyDownloader::IsRunning()
{
	return running;
}

bool GiphyDownloader::IsDownloadFileExsit(const DownloadTaskData &taskData, QString &outputFileName)
{
	QUrl url = QUrl::fromEncoded(taskData.url.toLocal8Bit());
	QString tail;
	switch (taskData.type) {
	case StickerDownloadType::THUMBNAIL:
		tail = "thumbnail";
		break;
	case StickerDownloadType::ORIGINAL:
		tail = "original";
		break;
	}
	QString fileName = saveFileName(url, taskData.uniqueId, tail);
	outputFileName = pls_get_user_path(GIPHY_STICKERS_CACHE_PATH) + fileName;
	return QFile::exists(outputFileName);
}

void GiphyDownloader::excuteTask(const DownloadTaskData &taskData)
{
	if (!manager) {
		manager = new QNetworkAccessManager;
		connect(manager, &QNetworkAccessManager::finished, this, &GiphyDownloader::downloadFinished);
		connect(manager, &QNetworkAccessManager::networkAccessibleChanged, this, &GiphyDownloader::OnNetworkAccessChanged);
	}
	QUrl url = QUrl::fromEncoded(taskData.url.toLocal8Bit());
	QString userFileName;
	if (IsDownloadFileExsit(taskData, userFileName)) {
		TaskResponData responData;
		responData.taskData = taskData;
		responData.fileName = userFileName;
		emit downloadResult(responData);
		return;
	}

	if (QNetworkAccessManager::Accessible != manager->networkAccessible()) {
		if (taskData.needRetry) {
			qDebug("Network is not accessible,add task to retry list, current retry task size is:%d", tasksRetry.size());
			tasksRetry.enqueue(taskData);
		}
		QString errorString("Network is not accessible");
		TaskResponData responData;
		responData.taskData = taskData;
		responData.errorString = errorString;
		responData.resultType = ResultStatus::ERROR_OCCUR;
		emit downloadResult(responData);
		return;
	}

	QNetworkRequest request(url);
	QNetworkReply *reply = manager->get(request);
	connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
		TaskResponData responData;
		responData.taskData = taskData;
		responData.fileName = userFileName;
		emit downloadProgress(responData, bytesReceived, bytesTotal);
	});

	taskDownloads.insert(reply, taskData);
}

void GiphyDownloader::sslErrors(QList<QSslError>)
{
	qDebug("ssl error occured");
}

void GiphyDownloader::downloadFinished(QNetworkReply *reply)
{
	if (!reply)
		return;
	QUrl url = reply->url();
	TaskResponData responData;
	responData.taskData = taskDownloads[reply];
	if (QNetworkReply::NoError != reply->error()) {
		if (responData.taskData.needRetry)
			tasksRetry.enqueue(responData.taskData);
		PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Download of %s failed: %s\n", url.toEncoded().constData(), qPrintable(reply->errorString()));
		QString errorString;
		errorString.sprintf("Download of %s failed: %s", url.toEncoded().constData(), reply->errorString().constData());
		responData.errorString = errorString;
		responData.resultType = ResultStatus::ERROR_OCCUR;
		emit downloadResult(responData);
	} else {
		if (GiphyWebHandler::isHttpRedirect(reply)) {
			PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Request was redirected.url=%s", url.toEncoded().constData());
			responData.errorString = QString("Request was redirected.");
			responData.resultType = ResultStatus::ERROR_OCCUR;
			emit downloadResult(responData);
		} else {
			QString tail;
			switch (responData.taskData.type) {
			case StickerDownloadType::THUMBNAIL:
				tail = "thumbnail";
				break;
			case StickerDownloadType::ORIGINAL:
				tail = "original";
				break;
			}
			QString filename = saveFileName(url, responData.taskData.uniqueId, tail);
			if (saveToDisk(filename, reply)) {
				responData.fileName = filename;
				emit downloadResult(responData);

			} else {
				PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Save file to disk failed.File name=%s", filename.toUtf8().constData());
				responData.errorString = QString("Save file to disk failed");
				responData.resultType = ResultStatus::ERROR_OCCUR;
				emit downloadResult(responData);
			}
		}
	}
	taskDownloads.remove(reply);
	reply->deleteLater();
}

void GiphyDownloader::OnNetworkAccessChanged(NetworkAccessible accessible)
{
	if (QNetworkAccessManager::Accessible == accessible) {
		if (tasksRetry.isEmpty())
			return;
		qDebug("retry task size=%d,retry download...", tasksRetry.size());
		int sizeTask = tasksRetry.size();
		for (int i = 0; i < sizeTask; ++i) {
			DownloadTaskData task = tasksRetry.dequeue();
			excuteTask(task);
			QThread::msleep(50);
		}
	} else {
		auto iter = taskDownloads.begin();
		while (iter != taskDownloads.end()) {
			if (iter->needRetry)
				tasksRetry.enqueue(iter.value());
			iter.key()->deleteLater();
			iter = taskDownloads.erase(iter);
		}
	}
}

bool GiphyDownloader::saveToDisk(QString &filename, QNetworkReply *reply)
{
	QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
	if (0 == contentType.compare(CONTENT_TYPE, Qt::CaseInsensitive)) {
		QString path(pls_get_user_path(GIPHY_STICKERS_CACHE_PATH));
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		filename = path + "/" + filename;
		QFile file(filename);
		if (!file.open(QIODevice::WriteOnly)) {
			PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Could not open %s for writing: %s\n", qPrintable(filename), qPrintable(file.errorString()));
			return false;
		}

		file.write(reply->readAll());
		file.close();
		return true;
	}

	return false;
}

QString GiphyDownloader::saveFileName(const QUrl &url, const QString &id, const QString &tail)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();

	if (!id.isEmpty())
		basename = id + "_" + tail + "." + QFileInfo(path).suffix();
	return basename;
}

void GiphyDownloader::ClearTask()
{
	auto iter = taskDownloads.begin();
	while (iter != taskDownloads.end()) {
		iter.key()->deleteLater();
		iter = taskDownloads.erase(iter);
	}
}
