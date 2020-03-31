#ifndef PLSAPIFacebook_H
#define PLSAPIFacebook_H

#include <QObject>
#include <PLSHttpApi\PLSHttpHelper.h>

enum class PLSAPIFacebookType {
	PLSFacebookSuccess,
	PLSFacebookFailed,
	PLSFacebookNotObject,
	PLSFacebookDeclined,
	PLSFacebookGranted,
	PLSFacebookInvalidAccessToken,
	PLSFacebookObjectDontExist,
	PLSRequestPermissionReject,
	PLSLivingPermissionReject,
	PLSUpdateLiveInfoFailed,
	PLSFacebookNetworkError,
};

using MyRequestTypeFunction = function<void(PLSAPIFacebookType type)>;
using MyRequestSuccessFunction = function<void(QJsonObject root)>;
using ItemInfoRequestFunction = function<void(PLSAPIFacebookType type, QString username, QString profilePath)>;
using TimelinePrivacyFunction = function<void(PLSAPIFacebookType type, QString privacyId)>;

#define timeline_living_permission "publish_video"
#define group_living_permission "publish_to_groups"
#define pages_manage_posts_permission "pages_manage_posts"
#define pages_read_engagement_permission "pages_read_engagement"
#define pages_read_user_content_permission "pages_read_user_content"

#define PLSFaceBookRquest PLSAPIFacebook::instance()

class PLSAPIFacebook : public QObject {
	Q_OBJECT

public:
	enum PLSAPI {
		PLSAPIGetLongLiveUserAccessToken,
		PLSAPIGetUserInfo,
		PLSAPIGetMyGroupListRequest,
		PLSAPIGetMyPageListRequest,
		PLSAPIGetMyGameListRequest,
		PLSAPICheckMyGroupListPermission,
		PLSAPICheckMyPageListPermission,
		PLSAPICheckTimelineLivingPermission,
		PLSAPICheckGroupLivingPermission,
		PLSAPICheckPageLivingPermission,
		PLSAPIStartTimelineLiving,
		PLSAPIStartGroupLiving,
		PLSAPIStartPageLiving,
		PLSAPIGetFacebookItemUserInfo,
		PLSAPIGetLiveVideoInfoRequest,
		PLSAPIGetLiveVideoPrivacyRequest,
		PLSAPIUpdateFacebookLiving,
		PLSAPIStopFacebookLiving,
	};
	Q_ENUM(PLSAPI)

public:
	static PLSAPIFacebook *instance();
	void getLongLiveUserAccessToken(PLSAPI requestType, MyRequestTypeFunction onFinished);
	void getUserInfo(PLSAPI requestType, MyRequestTypeFunction onFinished);
	void checkPermission(PLSAPI requestType, QStringList permission, MyRequestTypeFunction onFinished, QWidget *parent);
	void getMyGroupListRequestAndCheckPermission(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished, QWidget *parent);
	void getMyGroupListRequest(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished);
	void getMyPageListRequestAndCheckPermission(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished, QWidget *parent);
	void getMyPageListRequest(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished);
	void searchGameTagListByKeyword(PLSAPI requestType, QList<QMap<QString, QString>> &list, MyRequestTypeFunction onFinished, const QString &keyword);
	void startLiving(PLSAPI requestType, const QString &itemId, const QString &title, const QString &description, const QString &privacy, const QString &game_id, const QString &accessToken,
			 MyRequestTypeFunction onFinished);
	void getFacebookItemUserInfo(PLSAPI requestType, const QString &itemId, ItemInfoRequestFunction onFinished);
	void getLiveVideoInfoRequest(PLSAPI requestType, const QString &liveVideoId, const QString &field, MyRequestTypeFunction onFinished);
	void getTimelinePrivacyRequest(PLSAPI requestType, const QString &videoId, TimelinePrivacyFunction onFinished);
	void updateFacebookLiving(PLSAPI requestType, const QString &liveVideoId, const QString &title, const QString &description, const QString &privacy, const QString &game_id,
				  MyRequestTypeFunction onFinished);
	void stopFacebookLiving(PLSAPI requestType, const QString &liveVideoId, MyRequestTypeFunction onFinished);
	void downloadSyncImage(const QString &url, QString &imagePath);

private:
	static QString getFaceboolURL(const QString &endpoint);
	QUrl getPermissionRequestUrl(QString &permission);
	bool goFacebookRequestPermission(QString &permission, QWidget *parent);
	void startRequestApi(PLSAPI requestType, QNetworkReply *requestReply, MyRequestSuccessFunction successFunction, MyRequestTypeFunction failedFunction);
	PLSAPIFacebookType handleApiErrorCode(PLSAPI requestType, int statusCode, QByteArray data, QNetworkReply::NetworkError error);
	const char *getApiName(PLSAPI requestType);

private:
	QMap<PLSAPI, QPointer<QNetworkReply>> m_reply;
};

#endif // PLSAPIFacebook_H
