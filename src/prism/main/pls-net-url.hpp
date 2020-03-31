/*
 * @fine      NetBaseInfo
 * @brief     A brief introduction to class functionality
 * @date      2019-10-10
 * @author    Bing Cheng
 * @attention

 * @version   v1.0
 * @modify
 */

#ifndef PLS_NET_URL_HPP
#define PLS_NET_URL_HPP

#include <string>
#include <QString>
#include "frontend-api-global.h"

extern const QString PLS_FACEBOOK_LOGIN_URL_DEV;
extern const QString PLS_GOOGLE_LOGIN_URL_DEV;
extern const QString PLS_LINE_LOGIN_URL_DEV;
extern const QString PLS_NAVER_LOGIN_URL_DEV;
extern const QString PLS_TWITTER_LOGIN_URL_DEV;
extern const QString PLS_TWITCH_LOGIN_URL_DEV;
extern const QString PLS_WHALESPACE_LOGIN_URL_DEV;
extern const QString PLS_SNS_LOGIN_SIGNUP_URL_DEV;
extern const QString PLS_EMAIL_LOGIN_URL_DEV;
extern const QString PLS_EMAIL_SIGNUP_URL_DEV;
extern const QString PLS_EMAIL_FOGETTON_URL_DEV;
extern const QString PLS_LOGOUT_URL_DEV;
extern const QString PLS_TERM_OF_USE_URL_DEV;
extern const QString PLS_PRIVACY_URL_DEV;
extern const QString PLS_HMAC_KEY_DEV;

extern const QString PLS_TOKEN_SESSION_URL_DEV;
extern const QString PLS_NOTICE_URL_DEV;
extern const QString PLS_SIGNOUT_URL_DEV;
extern const QString PLS_CHANGE_PASSWORD_DEV;

// Twitch
extern const QString TWITCH_CLIENT_ID_DEV;
extern const QString TWITCH_REDIRECT_URI_DEV;

// Youtube
extern const QString YOUTUBE_CLIENT_ID_DEV;
extern const QString YOUTUBE_CLIENT_KEY_DEV;
extern const QString YOUTUBE_CLIENT_URL_DEV;

extern const QString PLS_CATEGORY_DEV;
// gpop
extern const QString PLS_GPOP_DEV;
extern const QString PLS_GPOP_CATEGORY_DEV;

// rtmp
extern const QString PLS_RTMP_ADD_DEV;
extern const QString PLS_RTMP_MODIFY_DEV;
extern const QString PLS_RTMP_DELETE_DEV;
extern const QString PLS_RTMP_LIST_DEV;

extern const QString g_streamKeyPrismHelperEn_DEV;
extern const QString g_streamKeyPrismHelperKr_DEV;

extern const QString LIBRARY_POLICY_PC_ID_DEV;
extern const QString LIBRARY_SENSETIME_PC_ID_DEV;

extern const QString MQTT_SERVER_DEV;
extern const QString MQTT_SERVER_WEB_DEV;
extern const QString MQTT_SERVER_PW_DEV;

extern const QString UPDATE_URL_DEV;
extern const QString LASTEST_UPDATE_URL_DEV;
extern const QString CONTACT_SEND_EMAIL_URL_DEV;

extern const QString PRISM_API_BASE_DEV;
extern const QString PRISM_AUTH_API_BASE_DEV;
extern const QString PRISM_API_ACTION_DEV;
extern const QString PRISM_API_STATUS_DEV;

//NaverTV
extern const QString CHANNEL_NAVERTV_LOGIN_DEV;
extern const QString CHANNEL_NAVERTV_GET_AUTH_DEV;
extern const QString CHANNEL_NAVERTV_GET_LIVES_DEV;
extern const QString CHANNEL_NAVERTV_GET_STREAM_INFO_DEV;
extern const QString CHANNEL_NAVERTV_QUICK_START_DEV;
extern const QString CHANNEL_NAVERTV_OPEN_DEV;
extern const QString CHANNEL_NAVERTV_CLOSE_DEV;
extern const QString CHANNEL_NAVERTV_MODIFY_DEV;
extern const QString CHANNEL_NAVERTV_COMMENT_OPTIONS_DEV;
extern const QString CHANNEL_NAVERTV_UPLOAD_IMAGE_DEV;
extern const QString CHANNEL_NAVERTV_STATUS_DEV;
extern const QString CHANNEL_NAVERTV_COMMENT_DEV;
extern const QString CHANNEL_NAVERTV_AUTHORIZE_DEV;
extern const QString CHANNEL_NAVERTV_AUTHORIZE_REDIRECT_DEV;
extern const QString CHANNEL_NAVERTV_TOKEN_DEV;

//Vlive
extern const QString CHANNEL_VLIVE_LOGIN_DEV;
extern const QString CHANNEL_VLIVE_LOGIN_JUMP_DEV;
extern const QString CHANNEL_VLIVE_LOGIN_JUMP_1_DEV;
extern const QString CHANNEL_VLIVE_SHARE_DEV;

//Band
extern const QString CHANNEL_BAND_LOGIN_DEV;
extern const QString CHANNEL_BAND_ID_DEV;
extern const QString CHANNEL_BAND_REDIRECTURL_DEV;
extern const QString CHANNEL_BAND_SECRET_DEV;
extern const QString CHANNEL_BAND_AUTH_DEV;
extern const QString CHANNEL_BAND_REFRESH_TOKEN_DEV;
extern const QString CHANNEL_BAND_USER_PROFILE_DEV;
extern const QString CHANNEL_BAND_CATEGORY_DEV;
extern const QString CHANNEL_BAND_LIVE_CREATE_DEV;
extern const QString CHANNEL_BAND_LIVE_OFF_DEV;

//Facebook
extern const QString CHANNEL_FACEBOOK_CLIENT_ID_DEV;
extern const QString CHANNEL_FACEBOOK_SECRET_DEV;

//Chat Widget
extern const QString CHAT_SOURCE_URL_DEV;

FRONTEND_API extern QString PLS_FACEBOOK_LOGIN_URL;
FRONTEND_API extern QString PLS_GOOGLE_LOGIN_URL;
FRONTEND_API extern QString PLS_LINE_LOGIN_URL;
FRONTEND_API extern QString PLS_NAVER_LOGIN_URL;
FRONTEND_API extern QString PLS_TWITTER_LOGIN_URL;
FRONTEND_API extern QString PLS_TWITCH_LOGIN_URL;
FRONTEND_API extern QString PLS_WHALESPACE_LOGIN_URL;
FRONTEND_API extern QString PLS_SNS_LOGIN_SIGNUP_URL;
FRONTEND_API extern QString PLS_EMAIL_LOGIN_URL;
FRONTEND_API extern QString PLS_EMAIL_SIGNUP_URL;
FRONTEND_API extern QString PLS_EMAIL_FOGETTON_URL;
FRONTEND_API extern QString PLS_TERM_OF_USE_URL;
FRONTEND_API extern QString PLS_PRIVACY_URL;
FRONTEND_API extern QString PLS_HMAC_KEY;
FRONTEND_API extern QString PLS_VLIVE_HMAC_KEY;
FRONTEND_API extern QString PLS_LOGOUT_URL;
FRONTEND_API extern QString PLS_TOKEN_SESSION_URL;
FRONTEND_API extern QString PLS_NOTICE_URL;
FRONTEND_API extern QString PLS_SIGNOUT_URL;
FRONTEND_API extern QString PLS_CHANGE_PASSWORD;

// Twitch
FRONTEND_API extern QString TWITCH_CLIENT_ID;
FRONTEND_API extern QString TWITCH_REDIRECT_URI;

// Youtube
FRONTEND_API extern QString YOUTUBE_CLIENT_ID;
FRONTEND_API extern QString YOUTUBE_CLIENT_KEY;
FRONTEND_API extern QString YOUTUBE_CLIENT_URL;

FRONTEND_API extern QString PLS_CATEGORY;
// gpop
FRONTEND_API extern QString PLS_GPOP;
FRONTEND_API extern QString PLS_GPOP_CATEGORY;

// rtmp
FRONTEND_API extern QString PLS_RTMP_ADD;
FRONTEND_API extern QString PLS_RTMP_MODIFY;
FRONTEND_API extern QString PLS_RTMP_DELETE;
FRONTEND_API extern QString PLS_RTMP_LIST;

FRONTEND_API extern QString g_streamKeyPrismHelperEn;
FRONTEND_API extern QString g_streamKeyPrismHelperKr;

FRONTEND_API extern QString LIBRARY_POLICY_PC_ID;
FRONTEND_API extern QString LIBRARY_SENSETIME_PC_ID;

FRONTEND_API extern QString MQTT_SERVER;
FRONTEND_API extern QString MQTT_SERVER_PW;
FRONTEND_API extern QString MQTT_SERVER_WEB;

FRONTEND_API extern QString UPDATE_URL;
FRONTEND_API extern QString LASTEST_UPDATE_URL;
FRONTEND_API extern QString CONTACT_SEND_EMAIL_URL;

FRONTEND_API extern QString PRISM_API_BASE;
FRONTEND_API extern QString PRISM_AUTH_API_BASE;
FRONTEND_API extern QString PRISM_API_ACTION;
FRONTEND_API extern QString PRISM_API_STATUS;

FRONTEND_API extern int PRISM_NET_REQUEST_TIMEOUT;

//NaverTV
FRONTEND_API extern QString CHANNEL_NAVERTV_LOGIN;
FRONTEND_API extern QString CHANNEL_NAVERTV_GET_AUTH;
FRONTEND_API extern QString CHANNEL_NAVERTV_GET_USERINFO;
FRONTEND_API extern QString CHANNEL_NAVERTV_DEFAULT_HEAD_IMAGE_URL;
FRONTEND_API extern QString CHANNEL_NAVERTV_GET_LIVES;
FRONTEND_API extern QString CHANNEL_NAVERTV_GET_STREAM_INFO;
FRONTEND_API extern QString CHANNEL_NAVERTV_QUICK_START;
FRONTEND_API extern QString CHANNEL_NAVERTV_OPEN;
FRONTEND_API extern QString CHANNEL_NAVERTV_CLOSE;
FRONTEND_API extern QString CHANNEL_NAVERTV_MODIFY;
FRONTEND_API extern QString CHANNEL_NAVERTV_COMMENT_OPTIONS;
FRONTEND_API extern QString CHANNEL_NAVERTV_UPLOAD_IMAGE;
FRONTEND_API extern QString CHANNEL_NAVERTV_STATUS;
FRONTEND_API extern QString CHANNEL_NAVERTV_COMMENT;
FRONTEND_API extern QString CHANNEL_NAVERTV_AUTHORIZE;
FRONTEND_API extern QString CHANNEL_NAVERTV_AUTHORIZE_REDIRECT;
FRONTEND_API extern QString CHANNEL_NAVERTV_TOKEN;

//Vlive
FRONTEND_API extern QString CHANNEL_VLIVE_LOGIN;
FRONTEND_API extern QString CHANNEL_VLIVE_LOGIN_JUMP;
FRONTEND_API extern QString CHANNEL_VLIVE_LOGIN_JUMP_1;
FRONTEND_API extern QString CHANNEL_VLIVE_SHARE;

//Band
FRONTEND_API extern QString CHANNEL_BAND_LOGIN;
FRONTEND_API extern QString CHANNEL_BAND_ID;
FRONTEND_API extern QString CHANNEL_BAND_REDIRECTURL;
FRONTEND_API extern QString CHANNEL_BAND_SECRET;
FRONTEND_API extern QString CHANNEL_BAND_AUTH;
FRONTEND_API extern QString CHANNEL_BAND_REFRESH_TOKEN;
FRONTEND_API extern QString CHANNEL_BAND_USER_PROFILE;
FRONTEND_API extern QString CHANNEL_BAND_CATEGORY;
FRONTEND_API extern QString CHANNEL_BAND_LIVE_CREATE;
FRONTEND_API extern QString CHANNEL_BAND_LIVE_OFF;

//FACEBOOK
FRONTEND_API extern QString CHANNEL_FACEBOOK_CLIENT_ID;
FRONTEND_API extern QString CHANNEL_FACEBOOK_REDIRECTURL;
FRONTEND_API extern QString CHANNEL_FACEBOOK_SECRET;

//Chat Widget
FRONTEND_API extern QString CHAT_SOURCE_URL;

const QString NEO_SESKEY = "NEO_SESKEY";

// channel rtmp default url
const QString RTMP_CHANNEL_NAVERTV_DEFAULT_URL = "rtmp://beta-rtmp.nova.navercorp.com:11935/live/";
const QString RTMP_CHANNEL_YOUTUBE_DEFAULT_URL = "rtmp://a.rtmp.youtube.com/live2";
const QString RTMP_CHANNEL_FACEBOOK_DEFAULT_URL = "rtmps://live-api-s.facebook.com:443/rtmp/";
const QString RTMP_CHANNEL_TWITCH_DEFAULT_URL = "rtmp://live-sel.twitch.tv/app/";
const QString RTMP_CHANNEL_AFREECATV_DEFAULT_URL = "rtmp://rtmpmanager-freecat.afreeca.tv/app/";

const QString PLS_AUTH_INIT_URL = "https://dev.apis.naver.com/prism/auth-api/init";
const QString CHANNEL_TWITCH_LOGIN_URL = "https://api.twitch.tv/kraken/oauth2/authorize";
const QString CHANNEL_TWITCH_URL = "https://api.twitch.tv/kraken/channel";

// Youtube
const QString CHANNEL_YOUTUBE_LOGIN_URL = "https://accounts.google.com/o/oauth2/v2/auth";
const QString YOUTUBE_REDIRECT_URI = "http://localhost";
const QString YOUTUBE_API_TOKEN = "https://www.googleapis.com/oauth2/v4/token";
const QString YOUTUBE_API_INFO = "https://www.googleapis.com/youtube/v3/channels";
//facebook
const QString CHANNEL_FACEBOOK_LOGIN_URL = "https://www.facebook.com/v7.0/dialog/oauth";
#define FACEBOOK_GRAPHA_DOMAIN QStringLiteral("https://graph.facebook.com/v7.0/")
#define FACEBOOK_STREAMING_GRAPH_DOMAIN QStringLiteral("https://streaming-graph.facebook.com/")

//twitter
const QString CHANNEL_TWITTER_COOKIE_URL = "https://www.twitter.com";
// NaverTv
const QString NAVERTV_REFERER = "http://tv.naver.com";

#define APP_ORGANIZATION "https://www.naver.com/"

#define TWITCH_URL_BASE "https://www.twitch.tv"
#define TWITCH_API_BASE "https://api.twitch.tv"
#define TWITCH_API_INGESTS "https://ingest.twitch.tv/ingests"

// youtube url
FRONTEND_API extern QString const g_plsGoogleApiHost;
FRONTEND_API extern QString const g_plsYoutubeShareHost;
FRONTEND_API extern QString const g_plsYoutubeAPIHost;
FRONTEND_API extern QString const g_plsYoutubeChatUrl;
FRONTEND_API extern QString const g_plsYoutubeShareUrl;
FRONTEND_API extern QString const g_plsYoutubeAPIHostV4;
FRONTEND_API extern QString const g_plsYoutubeStudioManagerUrl;

//twitch url
FRONTEND_API extern QString const g_plsTwitchApiHostUrl;
FRONTEND_API extern QString const g_plsTwitchChatUrl;

//vlive url
FRONTEND_API extern QString const g_plsVliveSchedulePathUrl;
FRONTEND_API extern QString const g_plsVliveObjectUrl;

//streamkey url
const QString g_facebookStreamKeyUrl = "https://www.facebook.com";
const QString g_twitterStreamKeyUrl = "https://twitter.com";
const QString g_twitchStreamkeyUrlWithAccount = "https://dashboard.twitch.tv/u/%1/settings/channel";
const QString g_twitchStreamkeyUrl = "https://dashboard.twitch.tv/";
const QString g_youtubeStreamkeyUrl = "https://www.youtube.com/live_dashboard?nv=1";

//share url
const QString g_facebookShareUrl = "http://www.facebook.com/sharer/sharer.php?u=%1";
const QString g_twitterShareUrl = "https://twitter.com/intent/tweet?text=%1&url=%2";

const QString g_youtubeUrl = "https://www.youtube.com";
const QString g_youtubeV3 = "https://www.googleapis.com/youtube/v3";
const QString g_youtubeBroadcast = "https://www.googleapis.com/youtube/v3/liveBroadcasts";
const QString g_youtubeChannels = "https://www.googleapis.com/youtube/v3/channels";
const QString g_yoububeLivePage = "https://www.youtube.com/live_dashboard";

FRONTEND_API extern const QString g_exclusiveRtmpUrl;

//AfreecaTV
FRONTEND_API extern const QString CHANNEL_AFREECA_REDIRECTURL;
FRONTEND_API extern const QString CHANNEL_AFREECA_LOGIN;
FRONTEND_API extern const QString g_plsAfreecaTVShareUrl_beforeLive;
FRONTEND_API extern const QString g_plsAfreecaTVShareUrl_living;
FRONTEND_API extern const QString g_plsAfreecaTVChannelInfo;
FRONTEND_API extern const QString g_plsAfreecaTVUserNick;
FRONTEND_API extern const QString g_plsAfreecaTVDashboard;
FRONTEND_API extern const QString g_plsAfreecaTVCategories;
FRONTEND_API extern const QString g_plsAfreecaTVUpdate;
FRONTEND_API extern const QString g_plsAfreecaTVLiveID;

#endif // PLS_NET_URL_HPP
