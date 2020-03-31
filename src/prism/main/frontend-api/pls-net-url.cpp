#include "../pls-net-url.hpp"

// Dev
// API host please refer :

const QString PLS_TERM_OF_USE_URL_DEV = "";
const QString PLS_PRIVACY_URL_DEV = "";

const QString PLS_HMAC_KEY_DEV = "";

const QString g_streamKeyPrismHelperEn_DEV = "";
const QString g_streamKeyPrismHelperKr_DEV = "";

const QString LIBRARY_POLICY_PC_ID_DEV = "";
const QString LIBRARY_SENSETIME_PC_ID_DEV = "";

// MQTT
const QString MQTT_SERVER_DEV = "125.209.241.16";
const QString MQTT_SERVER_PW_DEV = "client";
const QString MQTT_SERVER_WEB_DEV = "dev";

// Prism auth api
// 
const QString PRISM_AUTH_API_BASE_DEV = "";

const QString PLS_FACEBOOK_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/facebook";
const QString PLS_GOOGLE_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/google";
const QString PLS_LINE_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/line";
const QString PLS_NAVER_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/naver";
const QString PLS_TWITTER_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/twitter";
const QString PLS_TWITCH_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/twitch";
const QString PLS_WHALESPACE_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/v2/whalespace";
const QString PLS_SNS_LOGIN_SIGNUP_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/sns/joinLogin";

const QString PLS_EMAIL_LOGIN_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/email/v2/simple/login";
const QString PLS_EMAIL_SIGNUP_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/email/v2/simple/join";
const QString PLS_EMAIL_FOGETTON_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/email/reset/pw";
const QString PLS_CHANGE_PASSWORD_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/email/v2/change/pw";

const QString PLS_LOGOUT_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/logout";
const QString PLS_SIGNOUT_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/signout";
const QString PLS_TOKEN_SESSION_URL_DEV = PRISM_AUTH_API_BASE_DEV + "/auth/session";

// rtmp
const QString PLS_RTMP_ADD_DEV = PRISM_AUTH_API_BASE_DEV + "/publisher/rtmp";
const QString PLS_RTMP_MODIFY_DEV = PRISM_AUTH_API_BASE_DEV + "/publisher/rtmp/%1";
const QString PLS_RTMP_DELETE_DEV = PRISM_AUTH_API_BASE_DEV + "/publisher/rtmp/%1";
const QString PLS_RTMP_LIST_DEV = PRISM_AUTH_API_BASE_DEV + "/publisher/v2/rtmps";

// prism sidekick api
// 
const QString PLS_Sidekick_API_GATEWAY_DEV = "";
const QString PLS_NOTICE_URL_DEV = PLS_Sidekick_API_GATEWAY_DEV + "/notice";
const QString CONTACT_SEND_EMAIL_URL_DEV = PLS_Sidekick_API_GATEWAY_DEV + "/email/send";

// prism sync api
// 
const QString PLS_SYNC_API_GATEWAY_DEV = "";
const QString PLS_CATEGORY_DEV = PLS_SYNC_API_GATEWAY_DEV + "/resource/v3/latest/categories";
const QString PLS_GPOP_CATEGORY_DEV = PLS_SYNC_API_GATEWAY_DEV + "/resource/latest/categorys";
const QString UPDATE_URL_DEV = PLS_SYNC_API_GATEWAY_DEV + "/appInit";
const QString LASTEST_UPDATE_URL_DEV = PLS_SYNC_API_GATEWAY_DEV + "/latest/appversion";

// prism action log
//
const QString PRISM_API_ACTION_DEV = "";

// prism status log
const QString PRISM_API_STATUS_DEV = "";

// prism api
// 
const QString PRISM_API_BASE_DEV = "";

// Twitch
const QString TWITCH_CLIENT_ID_DEV = "";
const QString TWITCH_REDIRECT_URI_DEV = "";

// Youtube
const QString YOUTUBE_CLIENT_ID_DEV = "";
const QString YOUTUBE_CLIENT_KEY_DEV = "";
const QString YOUTUBE_CLIENT_URL_DEV = "";
	// gpop
const QString PLS_GPOP_DEV = "";

//NaverTv
const QString CHANNEL_NAVERTV_LOGIN_DEV = "";
const QString CHANNEL_NAVERTV_GET_AUTH_DEV = "";
const QString CHANNEL_NAVERTV_GET_LIVES_DEV = "";
const QString CHANNEL_NAVERTV_GET_STREAM_INFO_DEV = "";
const QString CHANNEL_NAVERTV_QUICK_START_DEV = "";
const QString CHANNEL_NAVERTV_OPEN_DEV = "";
const QString CHANNEL_NAVERTV_CLOSE_DEV = "";
const QString CHANNEL_NAVERTV_MODIFY_DEV = "";
const QString CHANNEL_NAVERTV_COMMENT_OPTIONS_DEV = "";
const QString CHANNEL_NAVERTV_UPLOAD_IMAGE_DEV = "";
const QString CHANNEL_NAVERTV_STATUS_DEV = "";
const QString CHANNEL_NAVERTV_COMMENT_DEV = "";
const QString CHANNEL_NAVERTV_AUTHORIZE_DEV = "";
const QString CHANNEL_NAVERTV_AUTHORIZE_REDIRECT_DEV = "";
const QString CHANNEL_NAVERTV_TOKEN_DEV = "";

//Vlive
const QString CHANNEL_VLIVE_LOGIN_DEV = "";
const QString CHANNEL_VLIVE_LOGIN_JUMP_DEV = "";
const QString CHANNEL_VLIVE_LOGIN_JUMP_1_DEV = "";
const QString CHANNEL_VLIVE_SHARE_DEV = "";

//Band
const QString CHANNEL_BAND_LOGIN_DEV = "";
const QString CHANNEL_BAND_ID_DEV = "";
const QString CHANNEL_BAND_REDIRECTURL_DEV = "";
const QString CHANNEL_BAND_SECRET_DEV = "";
const QString CHANNEL_BAND_AUTH_DEV = "";
const QString CHANNEL_BAND_REFRESH_TOKEN_DEV = "";
const QString CHANNEL_BAND_USER_PROFILE_DEV = "";
const QString CHANNEL_BAND_CATEGORY_DEV = "";
const QString CHANNEL_BAND_LIVE_CREATE_DEV = "";
const QString CHANNEL_BAND_LIVE_OFF_DEV = "";

//Facebook
//"1056056317812617"
const QString CHANNEL_FACEBOOK_CLIENT_ID_DEV = "";
//"ba1f9403e8be73d1fca8f9f5762b7e86"
const QString CHANNEL_FACEBOOK_SECRET_DEV = "";

//Chat Widget
const QString CHAT_SOURCE_URL_DEV = "";

//=============================================================================================================
//=============================================================================================================
// Real

QString PLS_TERM_OF_USE_URL = "";
QString PLS_PRIVACY_URL = "";

QString PLS_HMAC_KEY = "";
QString PLS_VLIVE_HMAC_KEY = "";

QString g_streamKeyPrismHelperEn = "";
QString g_streamKeyPrismHelperKr = "";

QString LIBRARY_POLICY_PC_ID = "";
QString LIBRARY_SENSETIME_PC_ID = "";

// MQTT
QString MQTT_SERVER = "msg-kr.prismlive.com";
QString MQTT_SERVER_PW = "vmflwmazmffk2018";
QString MQTT_SERVER_WEB = "real";

// Prism auth api
// 
QString PRISM_AUTH_API_BASE = "";

QString PLS_FACEBOOK_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/sns/facebook";
QString PLS_GOOGLE_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/sns/google";
QString PLS_LINE_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/sns/line";
QString PLS_NAVER_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/sns/naver";
QString PLS_TWITTER_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/sns/twitter";
QString PLS_TWITCH_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/sns/twitch";
QString PLS_WHALESPACE_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/sns/v2/whalespace";
QString PLS_SNS_LOGIN_SIGNUP_URL = PRISM_AUTH_API_BASE + "/auth/sns/joinLogin";

QString PLS_EMAIL_LOGIN_URL = PRISM_AUTH_API_BASE + "/auth/email/v2/simple/login";
QString PLS_EMAIL_SIGNUP_URL = PRISM_AUTH_API_BASE + "/auth/email/v2/simple/join";
QString PLS_EMAIL_FOGETTON_URL = PRISM_AUTH_API_BASE + "/auth/email/reset/pw";
QString PLS_CHANGE_PASSWORD = PRISM_AUTH_API_BASE + "/auth/email/v2/change/pw";

QString PLS_LOGOUT_URL = PRISM_AUTH_API_BASE + "/auth/logout";
QString PLS_SIGNOUT_URL = PRISM_AUTH_API_BASE + "/auth/signout";
QString PLS_TOKEN_SESSION_URL = PRISM_AUTH_API_BASE + "/auth/session";

// rtmp
QString PLS_RTMP_ADD = PRISM_AUTH_API_BASE + "/publisher/rtmp";
QString PLS_RTMP_MODIFY = PRISM_AUTH_API_BASE + "/publisher/rtmp/%1";
QString PLS_RTMP_DELETE = PRISM_AUTH_API_BASE + "/publisher/rtmp/%1";
QString PLS_RTMP_LIST = PRISM_AUTH_API_BASE + "/publisher/v2/rtmps";

// prism sidekick api
//
QString PLS_Sidekick_API_GATEWAY = "";
QString PLS_NOTICE_URL = PLS_Sidekick_API_GATEWAY + "/notice";
QString CONTACT_SEND_EMAIL_URL = PLS_Sidekick_API_GATEWAY + "/email/send";

// prism sync api
// 
QString PLS_SYNC_API_GATEWAY = "";
QString PLS_CATEGORY = PLS_SYNC_API_GATEWAY + "/resource/v3/latest/categories";
QString PLS_GPOP_CATEGORY = PLS_SYNC_API_GATEWAY + "/resource/latest/categorys";
QString UPDATE_URL = PLS_SYNC_API_GATEWAY + "/appInit";
QString LASTEST_UPDATE_URL = PLS_SYNC_API_GATEWAY + "/latest/appversion";

// prism action log
QString PRISM_API_ACTION = "";

// prism status log
QString PRISM_API_STATUS = "";

// prism api
QString PRISM_API_BASE = "";

// Twitch
QString TWITCH_CLIENT_ID = "";
QString TWITCH_REDIRECT_URI = "";

// Youtube
QString YOUTUBE_CLIENT_ID = "";
QString YOUTUBE_CLIENT_KEY = "";
QString YOUTUBE_CLIENT_URL = "";
// gpop
QString PLS_GPOP = "";

//NaverTV
QString CHANNEL_NAVERTV_LOGIN = "";
QString CHANNEL_NAVERTV_GET_AUTH = "";
QString CHANNEL_NAVERTV_GET_USERINFO = "";
QString CHANNEL_NAVERTV_DEFAULT_HEAD_IMAGE_URL = "";
QString CHANNEL_NAVERTV_GET_LIVES = "";
QString CHANNEL_NAVERTV_GET_STREAM_INFO = "";
QString CHANNEL_NAVERTV_QUICK_START = "";
QString CHANNEL_NAVERTV_OPEN = "";
QString CHANNEL_NAVERTV_CLOSE = "";
QString CHANNEL_NAVERTV_MODIFY = "";
QString CHANNEL_NAVERTV_COMMENT_OPTIONS = "";
QString CHANNEL_NAVERTV_UPLOAD_IMAGE = "";
QString CHANNEL_NAVERTV_STATUS = "";
QString CHANNEL_NAVERTV_COMMENT = "";
QString CHANNEL_NAVERTV_AUTHORIZE = "";
QString CHANNEL_NAVERTV_AUTHORIZE_REDIRECT = "";
QString CHANNEL_NAVERTV_TOKEN = "";

//VLive
QString CHANNEL_VLIVE_LOGIN = "";
QString CHANNEL_VLIVE_LOGIN_JUMP = "";
QString CHANNEL_VLIVE_LOGIN_JUMP_1 = "";
QString CHANNEL_VLIVE_SHARE = "";

//Band
QString CHANNEL_BAND_LOGIN = "";
QString CHANNEL_BAND_ID = "";
QString CHANNEL_BAND_REDIRECTURL = "";
QString CHANNEL_BAND_SECRET = "";
QString CHANNEL_BAND_AUTH = "";
QString CHANNEL_BAND_REFRESH_TOKEN = "";

QString CHANNEL_BAND_USER_PROFILE = "";
QString CHANNEL_BAND_CATEGORY = "";
QString CHANNEL_BAND_LIVE_CREATE = "";
QString CHANNEL_BAND_LIVE_OFF = "";

//Facebook
QString CHANNEL_FACEBOOK_CLIENT_ID = "";
QString CHANNEL_FACEBOOK_REDIRECTURL = "";
QString CHANNEL_FACEBOOK_SECRET = "";

//Chat Widget
QString CHAT_SOURCE_URL = "";

int PRISM_NET_REQUEST_TIMEOUT = 15000;

// youtube
const QString g_plsGoogleApiHost = "";
const QString g_plsYoutubeShareHost = "";
const QString g_plsYoutubeAPIHost = QString("%1/youtube/v3").arg(g_plsGoogleApiHost);
const QString g_plsYoutubeShareUrl = QString("%1/watch?v=%2").arg(g_plsYoutubeShareHost);
const QString g_plsYoutubeAPIHostV4 = QString("%1/oauth2/v4").arg(g_plsGoogleApiHost);
const QString g_plsYoutubeChatUrl = "";
const QString g_plsYoutubeStudioManagerUrl = "";

//twitch
const QString g_plsTwitchApiHostUrl = "";
const QString g_plsTwitchChatUrl = "";

//vlive
QString const g_plsVliveSchedulePathUrl = "";
QString const g_plsVliveObjectUrl = "";

const QString g_exclusiveRtmpUrl = "";

//AfreecaTV
const QString CHANNEL_AFREECA_REDIRECTURL = "";
const QString CHANNEL_AFREECA_LOGIN = "";
const QString g_plsAfreecaTVShareUrl_beforeLive = "";
const QString g_plsAfreecaTVShareUrl_living = "";
const QString g_plsAfreecaTVChannelInfo = "";
const QString g_plsAfreecaTVUserNick = "";
const QString g_plsAfreecaTVDashboard = "";
const QString g_plsAfreecaTVCategories = "";
const QString g_plsAfreecaTVUpdate = "";
const QString g_plsAfreecaTVLiveID = "";
