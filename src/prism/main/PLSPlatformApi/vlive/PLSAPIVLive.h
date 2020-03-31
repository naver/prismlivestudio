#ifndef PLSAPIVLive_H
#define PLSAPIVLive_H

#include <PLSHttpApi\PLSHttpHelper.h>
#include <QObject>

class PLSAPIVLive : public QObject {
	Q_OBJECT
public:
	explicit PLSAPIVLive(QObject *parent = nullptr);

	using UploadImageCallback = std::function<void(bool ok, const QString &imageUrl)>;

	static void vliveRequestGccAndLanguage(const QObject *receiver, dataErrorFunction onFinish);
	static void vliveRequestUsersInfoAndChannel(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed);
	static void vliveRequestSchedule(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, void *const context = nullptr);
	static void vliveRequestStartLive(const QObject *receiver, dataErrorFunction onFinish);
	static void vliveRequestStatistics(const QObject *receiver, dataErrorFunction onFinish);
	static void vliveRequestStopLive(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed);
	static void uploadImage(const QObject *receiver, const QString &imageFilePath, UploadImageCallback callback);
	static bool isVliveFanship();

	static QPair<bool, QString> downloadImageSync(const QObject *receive, const QString &url, const QString &prefix = "", bool ingoreCache = false);
	static void downloadImageAsync(const QObject *receiver, const QString &imageUrl, ImageCallback callback, void *const context = nullptr, const QString &prefix = "", bool ingoreCache = false);
	static QString getLocalImageFile(const QString &imageUrl, const QString &prefix = "");

private:
	enum class PLSNetMehod {
		PUT = 0,
		POST,
		GET,
	};

	static void addCommenQuery(PLSNetworkReplyBuilder &builder);
	static void addCommenCookie(PLSNetworkReplyBuilder &builder);
	static void addMacAddress(PLSNetworkReplyBuilder &builder);
	static void addUserAgent(PLSNetworkReplyBuilder &builder);
	static QString getVliveHost();
	static QString getStreamName();
};

#endif // PLSAPIVLive_H
