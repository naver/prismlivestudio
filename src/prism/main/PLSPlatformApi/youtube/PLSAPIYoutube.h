#pragma once

#include <PLSHttpApi\PLSHttpHelper.h>
#include <QObject>
#include "PLSPlatformYoutube.h"

class PLSAPIYoutube : public QObject {
	Q_OBJECT
public:
	enum class RefreshType {
		NotRefresh = 0,
		CheckRefresh,
		ForceRefresh,
	};

	explicit PLSAPIYoutube(QObject *parent = nullptr);

	static void addCommenCookieAndUserKey(PLSNetworkReplyBuilder &builder);
	//get the live is stoped by remote.
	static void requestLiveBroadcastStatus(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);
	//get the viwer count by this live.
	static void requestVideoStatus(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);

	//below three api is to creat a new shcedule.
	static void requestLiveBroadcastsInsert(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);
	static void requestLiveStreamsInsert(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);
	static void requestLiveBroadcastsBind(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);

	//to change schedule live to start and stop live by auto.
	static void requestLiveBroadcastsUpdate(PLSYoutubeLiveinfoData data, const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);

	static void requestCurrentSelectData(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);

	static void requestStopLive(const QObject *receiver, function<void()> onNext);
	static void requestCategoryID(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType, bool isSchedule, int context);
	static void requestCategoryList(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);
	static void requestScheduleList(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType, int context);
	static void requestLiveStreamKey(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType);
	static void requestUpdateVideoData(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed, RefreshType refreshType, PLSYoutubeLiveinfoData data);

	static void refreshYoutubeTokenBeforeRequest(RefreshType refreshType, function<QNetworkReply *()> originNetworkReplay, const QObject *originReceiver, dataFunction originOnSucceed = nullptr,
						     dataErrorFunction originOnFailed = nullptr, dataErrorFunction originOnFinished = nullptr, void *const originContext = nullptr);
};
