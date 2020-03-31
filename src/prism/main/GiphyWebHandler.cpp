#include "GiphyWebHandler.h"
#include "log/module_names.h"
#include "liblog.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QEventLoop>

static const int TIME_OUT_MS = 10 * 1000;
static const int SHOW_LOADING_TIME_OUT_MS = 500;

GiphyWebHandler::GiphyWebHandler(QObject *parent) : QObject(parent) {}

GiphyWebHandler::~GiphyWebHandler()
{
	if (nullptr != manager)
		manager->deleteLater();
}

void GiphyWebHandler::GiphyFetch(RequestTaskData task)
{
	if (nullptr == manager) {
		manager = new QNetworkAccessManager;
		connect(manager, &QNetworkAccessManager::networkAccessibleChanged, [=](QNetworkAccessManager::NetworkAccessibility accessible) { emit networkAccssibleChanged(accessible); });
		emit networkAccssibleChanged(manager->networkAccessible());
	}
#ifdef DEBUG
	qDebug() << "GiphyWebHandler: requst from " << task.url;
#endif // DEBUG

	Append(task);
}

bool GiphyWebHandler::isHttpRedirect(QNetworkReply *reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303 || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void GiphyWebHandler::Get(RequestTaskData task)
{
	QMutexLocker locker(&taskMutex);
	if (task.url != currentRequestTask.second.url)
		QMetaObject::invokeMethod(this, "GiphyFetch", Qt::QueuedConnection, Q_ARG(RequestTaskData, task));
}

void GiphyWebHandler::DiscardTask()
{
	QMetaObject::invokeMethod(this, "ClearTask", Qt::QueuedConnection);
}

bool GiphyWebHandler::DealResponData(QNetworkReply *reply, ResponData &responData)
{
	QJsonObject obj;
	QJsonDocument document;
	QJsonParseError error;
	QJsonArray arrayData;
	QByteArray byteArray = reply->readAll();
	document = QJsonDocument::fromJson(byteArray, &error);
	if (QJsonParseError::NoError == error.error) {
		obj = document.object();
		if (obj.contains("meta")) {
			QJsonObject meta = obj.value("meta").toObject();
			if (meta.contains("status")) {
				int status = meta.value("status").toInt();
				responData.metaData.status = status;
				responData.metaData.msg = meta.value("msg").toString();
				responData.metaData.responId = meta.value("response_id").toString();
				if (status != 200) {
					return true;
				}

				if (obj.contains("pagination")) {
					QJsonObject pageObj = obj.value("pagination").toObject();
					responData.pageData.totalCount = pageObj.value("total_count").toInt();
					responData.pageData.count = pageObj.value("count").toInt();
					responData.pageData.offset = pageObj.value("offset").toInt();
				}

				if (obj.contains("data") && obj.value("data").isArray()) {
					arrayData = obj.value("data").toArray();
					auto iter = arrayData.constBegin();
					while (iter != arrayData.constEnd()) {
						GiphyData data;
						QJsonObject objData = (*iter).toObject();
						data.id = objData.value("id").toString();
						data.type = objData.value("type").toString();
						data.title = objData.value("title").toString();
						data.rating = objData.value("rating").toString();
						QJsonObject imageObj = objData.value("images").toObject();
						GetImageInfo(imageObj, data);
						responData.giphyData.push_back(data);
						iter++;
					}
				}
			}

			return true;
		}
	}

	return false;
}

void GiphyWebHandler::GetImageInfo(const QJsonObject &imageObj, GiphyData &data)
{
	// some GIFs don’t have every property available,so we should check which property should be used.
	QJsonObject obj_preview = imageObj.value("fixed_width_downsampled").toObject();
	if (obj_preview.isEmpty())
		obj_preview = imageObj.value("preview_gif").toObject();
	if (obj_preview.isEmpty())
		obj_preview = imageObj.value("fixed_width_still").toObject();

	data.previewUrl = obj_preview.value("url").toString();
	data.sizePreview = QSize(obj_preview.value("width").toString().toInt(), obj_preview.value("height").toString().toInt());

	// Get the original info.
	QJsonObject obj_original = imageObj.value("original").toObject();
	data.originalUrl = obj_original.value("url").toString();
	data.sizeOriginal = QSize(obj_original.value("width").toString().toInt(), obj_original.value("height").toString().toInt());
}

void GiphyWebHandler::Append(const RequestTaskData &task)
{
	if (requestQueue.isEmpty())
		QTimer::singleShot(0, this, SLOT(StartNextRequest()));

	QUrl requestUrl = QUrl::fromEncoded(task.url.toUtf8());

	// refuse consequent same request.
	if (!requestQueue.isEmpty()) {
		if (task.url != requestQueue.head().second.url)
			requestQueue.enqueue(qMakePair<QUrl, RequestTaskData>(requestUrl, task));
	} else
		requestQueue.enqueue(qMakePair<QUrl, RequestTaskData>(requestUrl, task));
}

void GiphyWebHandler::ClearTask()
{
	requestQueue.clear();
}

void GiphyWebHandler::StartNextRequest()
{
	taskMutex.lock();
	currentRequestTask.second.url = "";
	taskMutex.unlock();

	if (requestQueue.isEmpty())
		return;

	auto task = requestQueue.dequeue();
	taskMutex.lock();
	currentRequestTask = task;
	taskMutex.unlock();

	QUrl url = task.first;
	QNetworkRequest request(url);
	QNetworkReply *reply = manager->get(request);

	QTimer timerShowLoading;
	QTimer timer;
	timer.setSingleShot(true);
	timerShowLoading.setSingleShot(true);

	QEventLoop eventLoop;
	connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
	connect(&timerShowLoading, &QTimer::timeout, [=]() { emit LoadingVisible(task.second, true); });
	connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	timer.start(TIME_OUT_MS);
	timerShowLoading.start(SHOW_LOADING_TIME_OUT_MS);
	eventLoop.exec();

	if (timer.isActive()) {
		timer.stop();
		timerShowLoading.stop();
		RequestFinished(reply);
	} else {
		//time out
		disconnect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
		reply->abort();
		reply->deleteLater();

#ifdef DEBUG
		qDebug("GiphyWebHandler: requst to %s, time out %d.", url.toEncoded().constData(), TIME_OUT_MS);
#endif // DEBUG
		emit LoadingVisible(task.second, false);
		PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Request time out %d s.", TIME_OUT_MS);
		RequestErrorInfo errorInfo;
		errorInfo.errorText = QString("Request time out.");
		errorInfo.errorType = RequestErrorType::RequstTimeOut;
		emit FetchError(task.second, errorInfo);
		requestQueue.clear();
		taskMutex.lock();
		currentRequestTask.second.url = "";
		taskMutex.unlock();
	}
}

void GiphyWebHandler::RequestFinished(QNetworkReply *reply)
{
	QUrl url = reply->url();
	taskMutex.lock();
	RequestTaskData task = currentRequestTask.second;
	taskMutex.unlock();
	emit LoadingVisible(task, false);
	if (reply->error()) {
		PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "http response failed! url = %s, error info = %s\n", url.toEncoded().constData(), qPrintable(reply->errorString()));
		RequestErrorInfo errorInfo;
		errorInfo.errorText = reply->errorString();
		errorInfo.networkError = reply->error();
		emit FetchError(task, errorInfo);
	} else {
		if (isHttpRedirect(reply)) {
			PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "Request was redirected.\n");
			RequestErrorInfo errorInfo;
			errorInfo.errorText = QString("Request was redirected.");
			errorInfo.networkError = reply->error();
			emit FetchError(task, errorInfo);
		} else {
			struct ResponData responData;
			responData.task = task;
			if (DealResponData(reply, responData))
				emit FetchResult(responData);
			else {
				PLS_ERROR(MAIN_GIPHY_STICKER_MODULE, "parse sticker list json data error.");
				RequestErrorInfo errorInfo;
				errorInfo.errorText = QString("parse sticker list json data error.");
				errorInfo.networkError = reply->error();
				emit FetchError(task, errorInfo);
			}
		}
	}
	reply->deleteLater();
	StartNextRequest();
}
