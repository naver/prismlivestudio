/*
 * @file      CommonDefine.h
 * @brief     Definition of all strings used internally by the system.
 * @date      2019/09/25
 * @author    liuying
 * @attention null
 * @version   2.0.1
 * @modify    liuying create 2019/09/25
 */

#ifndef COMMON_COMMONDEFINE_H
#define COMMON_COMMONDEFINE_H

namespace common {
/*************** prism ****************/
#define PLS_PRISM "Prism"
#define PLS_LIVE_STUDIO "PRISM Live Studio"
#define PLS_ARCHITECTURE " Architecture "
#define PLS_BUILD " Build "
#define PLS_LANGUAGE " Language "

/*************** enter ****************/
#define NEW_LINE "\n"
#define ENTER "\r"
#define ENTER_WITH_BACKSLASH "\\r"
#define NEW_LINE_WITH_BACKSLASH "\\n"
#define SLASH "/"

/*************** dpi ****************/
#define HIGH_DPI_CAN_UPDATE_SIZE "_hdpi_canUpdateSize"
#define HIGH_DPI_ORIGINAL_SIZE "_hdpi_originalSize"
#define HIGH_DPI_TAG_COMMENT_BEGIN "/*"
#define HIGH_DPI_TAG_COMMENT_END "*/"
#define HIGH_DPI_COMMENT_INT "/*hdpi:int*/"

/*************** status ****************/
#define STATUS "status"
#define STATUS_ENABLE "enable"
#define STATUS_DISABLE "disable"
#define STATUS_SELECTED "selected"
#define STATUS_UNSELECTED "unselected"
#define STATUS_DISABLE_SELECTED "disableSelected"
#define STATUS_NORMAL "normal"
#define STATUS_HOVER "hover"
#define STATUS_PRESSED "pressed"
#define STATUS_ON "on"
#define STATUS_OFF "off"
#define STATUS_CLICKED "clicked"
#define STATUS_UNCLICKED "unclicked"
#define STATUS_TRUE "true"
#define STATUS_FALSE "false"
#define STATUS_ERROR "error"
#define STATUS_ENTER "enter"
#define STATUS_ACTION "action"
#define STATUS_HANDLE "handle"
#define STATUS_VISIBLE "visible"
#define STATUS_INVISIBLE "invisible"
#define STATUS_OPEN "open"
#define STATUS_PLAY "play"
#define STATUS_PAUSE "pause"
#define STATUS_STATE "state"

/*************** position ****************/
#define POSITION "position"

/*************** mode ****************/
#define MODE "mode"
#define PASSWORD_MODE "password"
#define NORMAL_MODE "normal"
#define LIVE_MODE "liveMode"
#define RECORD_MODE "recordMode"

/*************** version ****************/
#define DEFAULT_PLS_VERSION "2.0.1.001"

/*************** state ****************/
#define READY "ready"
#define GOLIVE "golive"
#define RECORD "record"
#define FINISH "finish"
#define LIVE "live"
#define FINISH "finish"
#define PRESSED "pressed"

#define CONTROLLER_TYPE "controllerType"
#define PASSWORD "password"

/*****************language******************/
#define EN_US "en-US"
#define KO_KR "ko-KR"
#define ZH_CN "zh-CN"
#define LANGUAGE_ENGLISH "English"

// Because of the Korean encoding format is not utf-8, it is expressed in octal code
#define LANGUAGE_KOREAN "\355\225\234\352\265\255\354\226\264"
#define LANGUAGE_ENGLISH_TRANSLATE_PATH "en-us/strings.xml"
#define LANGUAGE_KOREAN_TRANSLATE_PATH "ko-kr/strings.xml"

#define LANGUAGE_ENGLISH_TRANSLATE_PATH "en-us/strings.xml"
#define LANGUAGE_KOREAN_TRANSLATE_PATH "ko-kr/strings.xml"

#define LANGUAGE_SETTING_ENGLISH "en-US,en;q=0.9"
#define LANGUAGE_SETTING_KOREAN "ko-KR,ko;q=0.9"

#define ENCODING_SETTINGS_GUIDE_EN_US_URL ""
#define ENCODING_SETTINGS_GUIDE_KO_KR_URL ""

/*****************resources******************/
#define RESOURCES_PATH "resources/"
#define RESOURCES_STYLES_PATH "resources/styles/"
#define RESOURCES_LANGUAGES_PATH "resources/languages/"

/***************popup objname****************/
#define TITLEBAR_VIEW "TitleBarView"
#define CONTENT_VIEW "ContentView"
#define DIALOG_VIEW "DialogView"
#define MAINWINDOW_VIEW "MainWindowView"
#define SETTINGS_VIEW "SettingsView"
#define MAINWINDOW_VIEW "MainWindowView"
#define CONNECT_LIVEPLATFORM_VIEW "ConnectLivePlatformView"
#define ADD_RTMP_CHANNEL_VIEW "AddRtmpChannelView"
#define LOGIN_BACKGROUND_FRAME "LoginBackgroundFrame"
#define TERMS_OF_AGREE_VIEW "TermOfUserFrame"
#define LOGIN_WITH_EMAIL_VIEW "LoginWithEmailView"
#define SIGNUP_WITH_EMAIL_VIEW "SignupWithEmailView"
#define LOGIN_BACKGROUND_VIEW "LoginBackgroundView"
#define CHANNEL_SETTINGS_VIEW "ChannelSettingsView"
#define CHANNEL_SETTINGS_BLANK_VIEW "ChannelSettingsBlankView"
#define CHANNEL_SETTINGS_ITEM_VIEW "ChannelSettingsItemView"
#define SETTING_COMMON_LOADING_VIEW "SettingCommonLoadingView"
#define LOGIN_SNS_VIEW "SelectLoginPlatformView"

/***************struct*********************/
#define SNS_CHANNEL_DATA "SnsChannelData"
#define RTMP_CHANNEL_DATA "RtmpChannelData"
#define RTMP_CHANNEL_Type "RtmpChannelType"
#define CHANNEL_DATA "ChannelData"
#define CHANNEL_TYPE "ChannelType"
#define SNS_ALL_CHANNEL_DATA "SnsAllChannelData"
#define RTMP_ALL_CHANNEL_DATA "RtmpAllChannelData"
#define VIDEO_ENCODER_MAP "VideoEncoderMap"
#define SNS_AUTH_MAP "SnsAuthMap"
#define RtmpChannelDataVec "RtmpChannelDataVec"

/***************configs*********************/
#define CONFIGS_GROUP_LOGIN "login"
#define CONFIGS_GROUP_COOKIE "prism_cookie"
#define CONFIGS_LOGIN_TOKEN "login/token"
#define CONFIGS_FILE_PATH " /configs/prismconfig.ini"
#define CONFIGS_CHANNEL_LIST_PATH "channellist.txt"
#define CONFIGS_CUSTOM_ENCODER_PATH "customencoder.json"
#define CONFIGS_LIBRARY_ENCODER_POLICY_ZIP_PATH "library_Encoder_Policy_PC.zip"
#define CONFIGS_GCC_PATH "PRISMLiveStudio/user/gcc.json"
#define CONFIGS_USER_CONFIG_PATH "PRISMLiveStudio/user/config.ini"
#define CONFIGS_USER_THUMBNAIL_PATH "PRISMLiveStudio/user/%1"
#define CONFIGS_USER_TEXTMOTION_PATH "PRISMLiveStudio/textmotion/%1"
#define CONFIGS_CATEGORYS_PATH "PRISMLiveStudio/user/categorys.json"
#define CONFIGS_CATEGORYS_LIBRARY_PATH "PRISMLiveStudio/user/library.json"
#define CONFIGS_GPOP_PATH "PRISMLiveStudio/user/gpop.json"
#define CONFIGS_BEATURY_USER_PATH "PRISMLiveStudio/beauty/"
#define CONFIGS_BEATURY_PRESET_IMAGE_PATH "/thumb x3/"
#define CONFIGS_BEATURY_CUSTOM_IMAGE_PATH "beauty_"
#define CONFIGS_BEATURY_DEFAULT_IMAGE_PATH "PRISMLiveStudio/beauty/image/"
#define CONFIGS_BEATURY_SENSETIME_FILE "PRISMLiveStudio/beauty/license_online.lic"
#define BEAUTY_CONFIG "BeautyConfig"
#define GIPHY_STICKERS_CONFIG "GiphyStickersConfig"
#define GIPHY_STICKERS_USER_PATH "PRISMLiveStudio/sticker/"
#define GIPHY_STICKERS_CACHE_PATH "PRISMLiveStudio/sticker/cache/"
#define GIPHY_STICKERS_JSON_FILE "PRISMLiveStudio/sticker/sticker.json"

#define SENSETIME_UNZIP_PATH "PRISMLiveStudio/beauty"
#define SENSETIME_ZIP "PRISMLiveStudio/beauty/library_SenseTime_PC.zip"
#define SENSETIME_FILE_PATH "PRISMLiveStudio/beauty/library_SenseTime_PC/"
#define SENSETIME_OLD_FILE_NAME "sensetime_license.lic"
#define SENSETIME_NEW_FILE_NAME "license_online.lic"

#define BGM_CONFIG "BgmConfig"
#define CONFIGS_MUSIC_USER_PATH "PRISMLiveStudio/music/"
#define MUSIC_JSON_FILE "music.json"
#define BGM_MUSIC_PLAYING_GIF ":/images/bgm/BGM_equalizer.gif"
#define CONFIG_MUSIC_PATH "data/prism-studio/music/"

/* **the keys of source settings need to be same with bgm source plugins ** */
#define RANDOM_PLAY "random play"
#define PLAY_IN_ORDER "play in order"
#define BGM_GROUP "group"
#define PLAY_LIST "play list"
#define BGM_TITLE "title"
#define BGM_PRODUCER "producer"
#define BGM_URL "music"
#define BGM_DURATION "duration"
#define BGM_DURATION_TYPE "duration_type"
#define IS_LOOP "is_loop"
#define IS_SHOW "is_show"
#define BGM_IS_CURRENT "is_current"
#define BGM_IS_LOCAL_FILE "is_local_file"
#define BGM_IS_DISABLE "is_disable"
#define BGM_HAVE_COVER "has_cover"
#define BGM_COVER_PATH "cover_path"
#define BGM_URLS "urls"

/* **the keys of source settings need to be same with bgm source plugins ** */

/**************prism login******/
#define NET_REQUEST_TIME_OUT 10000
#define COOKIES "cookies"
#define JUMPURL "redrectUrl"
#define LOGIN_LOADING_TIME 800
#define SNS_LOGIN_URL "snsLoginUrl"
#define LOGIN_USERINFO_EMAIL "email"
#define LOGIN_USERINFO_PASSWORD "password"
#define LOGIN_USERINFO_NICKNAME "nickname"
#define LOGIN_USERINFO_AGREEMENT "agreement"
#define LOGIN_USERINFO_USERCODE "userCode"
#define LOGIN_USERINFO_PROFILEURL "profileThumbnailUrl"
#define LOGIN_USERINFO_AUTHTYPE "authType"
#define LOGIN_USERINFO_TOKEN "token"
#define LOGIN_USERINFO_AUThSTATUS_CODE "authStatusCode"
#define LOGIN_USERINFO_USER_CODE "userCode"
#define LOGIN_CODE "code"
#define LOGIN_AUTHTYPE_AFREECATV "afreecaTv"
#define LOGIN_AUTHTYPE_YOUTUBE "YouTube"
#define LOGIN_AUTHTYPE_WAV "wav"
#define LOGIN_AUTHTYPE_VLIVE "vlive"
#define LOGIN_AUTHTYPE_GOOGLE "google"
#define LOGIN_AUTHTYPE_FACEBOOK "facebook"
#define LOGIN_AUTHTYPE_TWITCH "twitch"
#define LOGIN_AUTHTYPE_TWITTER "twitter"
#define LOGIN_AUTHTYPE_LINE "line"
#define LOGIN_AUTHTYPE_NAVER "naver"
#define LOGIN_AUTHTYPE_EMAIL "email"
#define LOGIN_AUTHTYPE_CUSTOMRTMP "CustomRTMP"
#define HTTP_CONTENT_TYPE_VALUE "application/json;charset=UTF-8"

/**************channel login************************/
#define HTTP_CONTENT_TYPE_URL_ENCODED_VALUE "application/x-www-form-urlencoded;charset=UTF-8"
#define HTTP_GCC_KR "KR"

/**************channel cookie************************/
#define COOKIE_NEO_SES "NEO_SES"
#define COOKIE_SNS_TOKEN "snsToken"
#define COOKIE_SNS_CD "snsCd"
#define COOKIE_OAUTH_TOKEN "oauth_token"
#define COOKIE_NID_SES "NID_SES"
#define COOKIE_NID_INF "nid_inf"
#define COOKIE_NID_AUT "NID_AUT"
#define COOKIE_ACCESS_TOKEN "access_token"
#define COOKIE_EXPIRES_IN "expires_in"
#define COOKIE_TOKEN_TYPE "token_type"
#define COOKIE_REFRESH_TOKEN "refresh_token"

#define CHANNEL_NICKNAME "nickname"
#define CHANNEL_NAME "channelName"
#define CHANNEL_COOKIE_VLIVE "VLive_root"
#define CHANNEL_COOKIE_TWITCH "Twitch_root"
#define CHANNEL_COOKIE_WAV "Wav_root"
#define CHANNEL_COOKIE_YOUTUBE "Youtube_root"
#define CHANNEL_COOKIE_NAVERTV "NaverTv_root"
#define CHANNEL_RTMP_LIST "Rtmp_List"
#define HTTP_CODE "code"
#define HTTP_CLIENT_SECRET "client_secret"
#define HTTP_CLIENT_ID "client_id"
#define HTTP_GRANT_TYPE "grant_type"
#define HTTP_REDIRECT_URI "redirect_uri"
#define HTTP_ACCEPT "Accept"
#define HTTP_ACCEPT_TWITCH "application/vnd.twitchtv.v5+json"
#define HTTP_DISPLAY_NAME "display_name"
#define HTTP_NAME "name"
#define HTTP_IN_AUTH_CHANNEL "inAuthChannel"
#define HTTP_AUTHORIZATION_CODE "authorization_code"
#define HTTP_AUTHORIZATION "Authorization"
#define HTTP_PART "part"
#define HTTP_PART_SNPIIPT "snippet"
#define HTTP_MINE "mine"
#define HTTP_MINE_TRUE "true"
#define HTTP_MINE "mine"
#define HTTP_REFERER "Referer"
#define HTTP_FAILURE "Failure"
#define HTTP_SUCCESS "success"
#define HTTP_RTN_MSG "rtn_msg"
#define HTTP_NICK_NAME "nick_name"
#define HTTP_ITEM_ID "itemId"
#define HTTP_ITEMS "items"
#define HTTP_LIBRARY_1743 "LIBRARY_1743"
#define HTTP_RESOURCE_URL "resourceUrl"
#define CATEGORY_ID "categoryId"
#define COLOR_THUMBNAILURL "thumbnailUrl"
#define COLOR "color"
#define BEAUTY "beauty"
#define MUSIC "music"
#define LIBRARY "library"
#define HTTP_MESSAGE "message"
#define RTMP_SEQ "rtmpSeq"
#define STREAM_NAME "streamName"
#define RTMP_URL "rtmpUrl"
#define DESCRIPTION "description"
#define STREAM_KEY "streamKey"
#define ABP_FLAG "abpFlag"
#define CONFIG_RESOLUTION "resolution"
#define BITRATE "bitrate"
#define FRAMERATE "framerate"
#define INTERVAL "interval"
#define USERNAME "username"
#define META "meta"
#define ENCODER_POLICIES "encoderPolicies"
#define VIDEO_POLICIES "videoPolicies"

#define COLOR_FILTER_COLOR "color"
#define COLOR_FILTER_IMAGE_FORMAT_PNG ".png"
#define COLOR_FILTER_THUMBNAIL "_thumbnail"
#define COLOR_FILTER_JSON_FILE "color_filter.json"
#define COLOR_FILTER_TMP_PATH "tmp/"
#define CONFIG_COLOR_FILTER_PATH "data/prism-studio/color_filter/"
#define CONFIG_BEAUTY_PATH "data/prism-studio/beauty/"
#define CONFIG_BEAUTY_TMP_PATH "tmp/"

#define COLOR_FILTER_ORDER_NUMBER 10

/************Settings************************/

#define SETTINGS_NONE "None"
#define BFRAME_ZERO "0"
#define BASE_LINE "Baseline"
#define MAIN "Main"
#define HIGH "High"
#define QUALITY "Quality"
#define SPEED "Speed"
#define BALANCED "Balanced"
#define VERY_FAST "Very Fast"
#define FASTER "Faster"
#define MEDIUM "Medium"
#define SLOW "Slow"
#define VERY_SLOW "Very Slow"
#define PLACEBO "Placebo"
#define INTERVAL_ONE "1s"
#define INTERVAL_TWO "2s"
#define INTERVAL_THEREE "3s"
#define INTERVAL_FOUR "4s"
#define INTERVAL_FIVE "5s"
#define RATE_CONTROL_CBR "CBR"
#define RATE_CONTROL_VBR "VBR"
#define RATE_CONTROL_VCM "VCM"
#define RATE_CONTROL_CQP "CQP"
#define RATE_CONTROL_AVBR "AVBR"
#define RATE_CONTROL_ICQ "ICQ"
#define RATE_CONTROL_LA "LA"
#define RATE_CONTROL_LA_ICQ "LA_ICQ"
#define FILM "Film"
#define ANIMATION "Animation"
#define GRAIN "Grain"
#define STILL_IMAGE "Still Image"
#define PSNR "Psnr"
#define SSIM "Ssim"
#define FAST_DECODE "Fast Decode"
#define ZERO_LATENCY "Zero Latency"
#define FPS_SIXTY "60fps"
#define FPS_THIRTY "30fps"
#define ENCODER_X264 "x.264"
#define ENCODER_H264 "QuickSync H.264"
#define BITRATE_4000 "4000"
#define BITRATE_6000 "6000"
#define BITRATE_96K "96k"
#define BITRATE_128K "128k"
#define BITRATE_160K "160k"
#define BITRATE_192K "192k"
#define RECORD_FORMAT_MP4 "mp4"
#define RECORD_FORMAT_F4V "f4v"
#define RECORD_FORMAT_FLV "flv"
#define RECORD_FORMAT_MOV "mov"
#define VIDEO_SIZE_2048 "2048"
#define AUDIO_ENCODER_FDK_AAC "FDK-AAC"
#define AUDIO_ENCODER_FOUNDATION_AAC "Media Foundation AAC"
#define AUDIO_CODEC_AACLC "AAC-LC"
#define AUDIO_SAMPLE_44_1KHZ "44.1kHz"
#define AUDIO_SAMPLE_48KHZ "48kHz"
#define AUDIO_DEFAULT_SPEAKERS "Default Speakers"
#define ENCODER_1080P_WITH_60FPS "1080p60"
#define ENCODER_1080P_WITH_30FPS "1080p30"
#define ENCODER_720P_WITH_60FPS "720p60"
#define ENCODER_720P_WITH_30FPS "720p30"
#define VIDEO_POLICY_TYPE_COMMON "common"
#define VIDEO_POLICY_TYPE_RECORD "record"
#define VIDEO_POLICY_TYPE_SIMULCAST "simulcast"
#define VIDEO_POLICY_TYPE_VLIVE "vlive"
#define VIDEO_POLICY_TYPE_WAV "wav"
#define VIDEO_POLICY_TYPE_NAVERTV "navertv"
#define VIDEO_POLICY_TYPE_YOUTUBE "youtube"
#define VIDEO_POLICY_TYPE_TWITCH "twitch"
#define VIDEO_POLICY_TYPE_FACEBOOK "facebook"
#define VIDEO_POLICY_TYPE_AFREECATV "afreecatv"
#define VIDEO_POLICY_TYPE_PERISCOPE "periscope"

#define QUICK_SYNC "quickSync"
#define X264 "x264"
#define RATE_CONTROL_METHOD "rateControlMethod"
#define BITRATE "bitrate"
#define RECOMMENDED "Recommended"
#define RATE_CONTROL "Rate Control"
#define KEYFRAME_INTERVAL "keyframe Interval"
#define KEY_FRAME_INTERVAL_SEC "keyframeIntervalSec"
#define PRESET "preset"
#define PROFILE "profile"
#define TUNE "tune"
#define ENCODER "Encoder"
#define RESOLUTION "Resolution"
#define FRAME_RATE "Frame Rate"
#define VIDEO_QUALITY "Video Quality"
#define VIDEO_SAVE_IN "Video Save In"
#define VIDEO_SIZE "Video Size"
#define FILE_NAME "File Name"
#define RECORD_FROMAT "Record Format"
#define AUDIO_ENCODER "Audio Encoder"
#define AUDIO_CODEC "Audio Codec"
#define AUDIO_SAMPLE "Audio Sample"
#define AUDIO_BITRATE "Audio Bitrate"
#define MONITORING_DEVICE "Monitoring Device"
#define STREAM_ROOT "stream_root"
#define RECORD_ROOT "record_root"
#define AUDIO_ROOT "audio_root"

/************RegExp************************/
#define EMAIL_REGEXP "^[\\w!#$\%&'*+/=?`{|}~^-]+(?:\\.[\\w!#$\%&'*+/=?`{|}~^-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,6}$"

#define PASSWORD_REGEXP "^(?![A-Za-z0-9]{6,20}$)(?![0-9\\W]{6,20}$)(?![a-zA-Z\\W]{6,20}$)(?![\\_\\W]{6,20}$)(?![0-9\\_\\W]{6,20}$)(?![A-Za-z\\_\\W]{6,20}$)[a-zA-Z0-9\\_\\W]{6,20}"
#define NICK_REGEXP "[^.]{1,20}"
#define SNS_LOG_URL_REGEXP "https://([\\w-]+\\.)+apis.naver.com+(/[\\w-]*)+\\/callback\\?\\w.*"
#define TWITTER_URL_FILTER "#_="
#define GOOGLE_URL_FILTER "&scope="
#define REGEXP_NUMBER "[0-9]{1,}"

/***************httpMgr******************/
#define HTTP_HEAD_OS "X-prism-os"
#define HTTP_HEAD_APP_VERSION "X-prism-appversion"
#define HTTP_HMAC_STR "apis.naver.com/prism"
#define HTTP_COOKIE "Cookie"
#define HTTP_HEAD_CONTENT_TYPE "Content-Type"
#define HTTP_HEAD_CC_TYPE "X-prism-cc"
#define HTTP_USER_AGENT "User-Agent"
#define HTTP_DEVICE_ID "deviceId"
#define HTTP_VERSION "version"
#define HTTP_GCC "gcc"
#define HTTP_WITH_ACTIVITY_INFO "withActivityInfo"
#define NETWORK_CHECK_URL "apis.naver.com"

/************login response code *********/
#define HTTP_NOT_EXIST_USER (1110)
#define HTTP_RESTRICT_USER (1120)
#define HTTP_PW_FAIL (1210)
#define HTTP_PW_RETYR_FAIL (1220)
#define HTTP_PW_EXPIRED (1230)
#define HTTP_IP_BLOCKED (12310)
#define INTERNAL_ERROR (-1)
#define SIGNUP_NO_AGREE (10000)
#define JOIN_EXIST_ID (2120)
#define JOIN_SESSION_EXPIRED (2320)
#define HTTP_STATUS_CODE_202 (202)
#define HTTP_STATUS_CODE_400 (400)
#define HTTP_STATUS_CODE_500 (500)
#define HTTP_STATUS_CODE_501 (501)
#define HTTP_STATUS_CODE_406 (406)
#define HTTP_STATUS_CODE_401 (401)
#define HTTP_TOKEN_INVAILD_CODE_3000 (3000)

/************http response code *********/
#define HTTP_STATUS_CODE_200 (200)

/***********reset password email********/
#define NO_EXIST_EMAIL (9010)

/****************  combobox *************/
#define COMBOBOX_MAX_VISIBLE_NUMBER 5

/**************scene module****************/
#define SCENE "scene"
#define SCENE_DRAG_MIME_TYPE "sceneItem"
#define FILTER_DRAG_MIME_TYPE "filterItem"
#define BUTTON_CHECK "check"
#define SCENE_MOVE_STEP 6
#define SCENE_SCROLLCONTENT_ITEM_LEFT 10
#define SCENE_SCROLLCONTENT_SPACEING 10
#define SCENE_SCROLLCONTENT_OFFSET 5
#define SCENE_SCROLLCONTENT_ITEM_HEIGHT 68
#define SCENE_SCROLLCONTENT_ITEM_WIDTH 81
#define SCENE_SCROLLCONTENT_LINE_COLOR "#effc35"
#define SCENE_SCROLLCONTENT_DEFAULT_COLOR "#272727"
#define SCENE_SCROLLCONTENT_COLUMN 2
#define SCENE_TRANSITION_DEFAULT_DURATION_VALUE 300
/**************source module****************/
#define SOURCE_ITME_WIDTH 45
#define SOURCE_MAXINTERVAL 5
#define SOURCE_ROW_INTERVAL (-2)
#define DRAG_MIME_TYPE "sceneItem"
#define SOURCE_DRAG_MIME_TYPE "CustomListView/text-icon-icon_hover"
#define SOURCE_VISABLE "visable"
#define SOURCE_MOVE "move"
#define SOURCE_WEB "url"
#define SOURCE_GAME "game"
#define SOURCE_TEXT "text"
#define SOURCE_AUDIO "audio"
#define SOURCE_IMAGE "image"
#define SOURCE_MEDIA "media"
#define SOURCE_CAMERA "camera"
#define SOURCE_DISPLAY "display"
#define SOURCE_SCREENSHOT "screenshot"
#define ONE_ZERO_EIGHT_ZERO_P_THREE_ZERO_FPS "1080P.30fps"
#define SOURCE_ICON_PATH "skin/"
#define SOURCE_ITEM_NORMAL_ICON "icon-source-%1.png"
#define SOURCE_ITEM_DISABLE_ICON "icon-source-%1-disable.png"
#define SOURCE_ITEM_SELECT_ICON "icon-source-%1-select.png"
#define SOURCE_ITEM_SELECT_DISABLE_ICON "icon-source-%1-select-disable.png"
#define SOURCE_ITEM_NORMAL_ERRORICON "erroricon-source-%1.png"
#define SOURCE_ITEM_DISABLE_ERRORICON "erroricon-source-%1-disabled.png"
#define SOURCE_ITEM_SELECT_ERRORINCON "erroricon-source-%1-select.png"
#define SOURCE_ITEM_SELECT_DISABLE_ERRORICON "erroricon-source-%1-select-disable.png"
#define SOURCE_ITEM_HOVER_NORMAL_ICON "icon-source-move.png"
#define SOURCE_ITEM_HOVER_SELECT_ICON "icon-source-move-select.png"
#define SOURCE_ITEM_HOVER_SELECT_DISABLE_ICON "icon-source-move-select-disable.png"
#define SOURCE_MENUSHOW_OFFSET 10
#define SOURCE_POLYGON 4
#define SOURCE_WIDTH 1

/************* menu manager module *****/
#define MENU_ICON_SIZE 30
#define MENU_SOURCEADD_DISPLAY "skin/icon-source-display.png"
#define MENU_SOURCEADD_WORDS "skin/icon-source-text.png"
#define MENU_SOURCEADD_WINDOW "skin/icon-source-window.png"
#define MENU_SOURCEADD_SCREENSOT "skin/icon-source-screenshot.png"
#define ACTION_SOURCEADD_GAME "skin/icon-source-game.png"
#define ACTION_SOURCEADD_VIDEOCAPTURE "skin/icon-source-camera.png"
#define ACTION_SOURCEADD_AUDIOACPTURE "skin/icon-source-audio.png"
#define ACTION_SOURCEADD_VIDEOFILES "skin/icon-source-media.png"
#define ACTION_SOURCEADD_IMAGE "skin/icon-source-image.png"
#define ACTION_SOURCEADD_BROSWER "skin/icon-source-url.png"
#define ACTION_SHORTCUT_OPEN "Ctrl+O"
#define ACTION_SHORTCUT_SAVE "Ctrl+S"
#define ACTON_SHORTCUT_SAVEAS "Shift+Ctrl+S"
#define ACTION_SHORTCUT_EXIT "Ctrl +Q"
#define ACTION_SHORTCUT_READY "Ctrl+T"
#define ACTION_SHORTCUT_STARTBROADCAST "Ctrl+N"
#define ACTION_SHORTCUT_ENDBROADCAST "Ctrl+E"
#define ACTION_SHORTCUT_BROADCASETSET "Ctrl+B"
#define ACTION_SHORTCUT_STARTECORD "Ctrl+I"
#define ACTION_SHORTCUT_ENDRECORD "Ctrl+F"
#define ACTION_SHORTCUT_UNDO "Ctrl+Z"
#define ACTION_SHORTCUT_REDO "Ctrl+Y"
#define ACTION_SHORTCUT_CUT "Ctrl+X"
#define ACTION_SHORTCUT_COPY "Ctrl+C"
#define ACTION_SHORTCUT_PASTE "Ctrl+V"
#define ACTION_SHORTCUT_CROPMODE "Alt"
#define ACTION_SHORTCUT_PROPERTIES "Ctrl+R"
#define ACTION_SHORTCUT_REMOVE "Del"
#define ACTION_SHORTCUT_FLIPHORZ "Alt+H"
#define ACTION_SHORTCUT_FLIPVERT "Alt+V"
#define ACTION_SHORTCUT_ADDSCENE "Shift+Ctrl+N"
#define ACTION_SHORTCUT_DUPSCENE "Shift+Ctrl+C"
#define ACTION_SHORTCUT_RENAMESCENE "Shift+Ctrl+D"
#define ACTION_SHORTCUT_FULLSCREEN "F11"
#define ACTION_SHORTCUT_ALWAYSONTOP "Shift+Ctrl+T"
#define ACTION_SHORTCUT_NOEFFECT "Alt+E"

/****************msg box**************/
#define MESSAGEBOX_BUTTON_OBJECTNAME "msgButton"
#define MESSAGEBOX_YES "Yes"
#define MESSAGEBOX_NO "No"
#define MESSAGEBOX_LAYOUT_SPACE 10
#define MESSAGEBOX_LAYOUT_MARGIN 0

/*************** font ****************/
#define FILE_VERSION_PATH "version_win_dev_qt.txt"
#define FONT_NANUM_GOTHIC_PATH "resources/font/NanumGothic.ttf"
#define FONT_NANUM_GOTHIC_BOLD_PATH "resources/font/NanumGothicBold.ttf"
#define DEFAULT_FONT_FAMILY "Arial"

/*************** time format ****************/
#define TIME_FORMAT "00:00:00"
#define TIME_ZERO "0"

/*************** param ****************/
#define PERCENT "%"
#define KBPS "kbps"
#define FPS "fps"
#define ONE_POINT "."
#define PRECISION_G 'g'
#define PRECISION_FOUR 4
#define EMPTY_STRING ""
#define ONE_SPACE " "
#define COLON ":"
#define PIXMAP "pixMap"
#define TYPE "type"
#define TYPE_PICTURE "picture"
#define TYPE_TEXT "text"
#define IMAGE_URL "imageUrl"
#define PNG "PNG"
#define OBJECT "object"
#define SELECT "Select"
#define INDEX "index"
#define CQSTR_STRING_BUNDLE "stringbundle"
#define CQSTR_STRING "string"
#define CQSTR_ID "id"
#define VIEW "view"
#define RESULT "result"
#define TITLE "title"
#define SHORT_TITLE "shortTitle"

#define VIDEO_SAVE_DEFAULT_PATH "C:\\Users\\admin"
/*********prism notice*************/
#define NOTICE_NOTICE_SEQ "noticeSeq"
#define NOTICE_COUNTRY_CODE "countryCode"
#define NOTICE_OS "os"
#define NOTICE_OS_VERSION "osVersion"
#define NOTICE_APP_VERSION "appVersion"
#define NOTICE_TITLE "title"
#define NOTICE_CONTENE "content"
#define NOTICE_DETAIL_LINK "detailLink"
#define NOTICE_CONTENT_TYPE "contentType"
#define NOTICE_CREATED_AT "createdAt"
#define NOTICE_VAILD_UNTIL_AT "validUntilAt"
/*************** const int variable ****************/
#define LOADING_PICTURE_MAX_NUMBER 8
#define LOADING_TIMER_TIMEROUT 100 // ms
#define TIMING_TIMEOUT 1000 // ms
#define CPU_TIMER_TIMEOUT 1000 // ms
#define FEED_UI_MAX_TIME 100 // ms
#define MAINWINDOW_MIN_WIDTH 625
#define ONE_HOUR_MINUTES 60
#define ONE_HOUR_SECONDS 3600
#define NUMBER_TEN 10
#define RIGHT_MARGIN 25
#define LEFT_MARGIN 15
#define MAX_CHANNEL_ITEM_NUMBER 100
#define ChannelSettingsOrder 1
#define DPI_VALUE_NINTY_SIX 96
#define DPI_VALUE_ONE 1

#define SCENE_LEFT_SPACING 10
#define SCENE_ITEM_VSPACING 0
#define SCENE_ITEM_HSPACING 15
#define SCENE_COLUMNS 2
#define SCENE_ITEM_FIX_HEIGHT 155
#define SCENE_SCROLL_AREA_SPACING_HEIGHT 10
#define SCENE_ITEM_FIX_WIDTH 112
#define SCENE_RENDER_NUMBER 10

/*************** const for encodingsettingview ****************/
#define ENCODING_SETTING_VIEW_WIDTH 310
#define ENCODING_SETTING_VIEW_HEIGHT_LIVE 183
#define ENCODING_SETTING_VIEW_HEIGHT_RECORD 152

/***************    id of source plugin from obs    ****************/
#define SCENE_SOURCE_ID "scene"
#define GROUP_SOURCE_ID "group"
#define DSHOW_SOURCE_ID "dshow_input"
#define AUDIO_INPUT_SOURCE_ID "wasapi_input_capture"
#define AUDIO_OUTPUT_SOURCE_ID "wasapi_output_capture"
#define GDIP_TEXT_SOURCE_ID "text_gdiplus"
#define GAME_SOURCE_ID "game_capture"
#define WINDOW_SOURCE_ID "window_capture"
#define PRISM_MONITOR_REGION_MENU "prism_monitor_region_menu"
#define PRISM_MONITOR_SOURCE_ID "prism_monitor_capture"
#define PRISM_REGION_SOURCE_ID "prism_region_source"
#define BROWSER_SOURCE_ID "browser_source"
#define MEDIA_SOURCE_ID "ffmpeg_source"
#define IMAGE_SOURCE_ID "image_source"
#define SLIDESHOW_SOURCE_ID "slideshow"
#define COLOR_SOURCE_ID "color_source"
#define BGM_SOURCE_ID "prism_bgm_source"
#define PRISM_STICKER_SOURCE_ID "prism_sticker_source"
#define PRISM_CHAT_SOURCE_ID "prism_chat_source"
#define PRISM_TEXT_MOTION_ID "prism_text_motion_source"
#define PRISM_NDI_SOURCE_ID "ndi_source"

/***************    filter id     ****************/
#define FILTER_TYPE_ID_APPLYLUT "clut_filter"
#define FILTER_TYPE_ID_CHROMAKEY "chroma_key_filter"
#define FILTER_TYPE_ID_COLOR_FILTER "color_filter"
#define FILTER_TYPE_ID_COLOR_KEY_FILTER "color_key_filter"
#define FILTER_TYPE_ID_COMPRESSOR "compressor_filter"
#define FILTER_TYPE_ID_CROP_PAD "crop_filter"
#define FILTER_TYPE_ID_EXPANDER "expander_filter"
#define FILTER_TYPE_ID_GAIN "gain_filter"
#define FILTER_TYPE_ID_IMAGEMASK_BLEND "mask_filter"
#define FILTER_TYPE_ID_INVERT_POLARITY "invert_polarity_filter"
#define FILTER_TYPE_ID_LIMITER "limiter_filter"
#define FILTER_TYPE_ID_LUMAKEY "luma_key_filter"
#define FILTER_TYPE_ID_NOISEGATE "noise_gate_filter"
#define FILTER_TYPE_ID_NOISE_SUPPRESSION "noise_suppress_filter"
#define FILTER_TYPE_ID_RENDER_DELAY "gpu_delay"
#define FILTER_TYPE_ID_SCALING_ASPECTRATIO "scale_filter"
#define FILTER_TYPE_ID_SCROLL "scroll_filter"
#define FILTER_TYPE_ID_SHARPEN "sharpness_filter"
#define FILTER_TYPE_ID_VSTPLUGIN "vst_filter"
#define FILTER_TYPE_ID_VIDEODELAY_ASYNC "async_delay_filter"
#define FILTER_TYPE_ID_PREMULTIPLIED_ALPHA_FILTER "premultiplied_alpha_filter"

/*************** properties view ****************/
#define PROPERTIES_VIEW_VERTICAL_SPACING_MIN 10
#define PROPERTIES_VIEW_VERTICAL_SPACING_MAX 15

#define DOCK_DEATTACH_MIN_SIZE 20
#define DISPLAY_VIEW_DEFAULT_WIDTH 690
#define DISPLAY_VIEW_DEFAULT_HEIGHT 210
#define DISPLAY_LABEL_DEFAULT_HEIGHT 339
#define DISPLAY_VIEW_MIN_HEIGHT 150
#define DISPLAY_VIEW_MAX_HEIGHT 368
#define FILTERS_VIEW_DEFAULT_WIDTH 910
#define FILTERS_VIEW_DEFAULT_HEIGHT 700
#define FILTERS_ITEM_VIEW_FIXED_HEIGHT 40
#define FILTERS_DISPLAY_VIEW_MIN_HEIGHT 150
#define FILTERS_DISPLAY_VIEW_MAX_HEIGHT 319
#define FILTERS_DISPLAY_VIEW_MIN_WIDTH 448
#define FILTERS_DISPLAY_VIEW_MAX_WIDTH 690
#define FILTERS_TRANSITION_VIEW_FIXED_HEIGHT 40
#define FILTERS_TRANSITION_VIEW_FIXED_WIDTH 128
#define FILTERS_PROPERTIES_VIEW_MAX_HEIGHT 244
#define COLOR_FILTERS_IMAGE_FIXED_WIDTH 60
#define COLOR_FILTERS_IMAGE_FIXED_HEIGHT 60
#define COLOR_FILTERS_VIEW_LEFT_PADDING 15
#define COLOR_FILTERS_VIEW_RIGHT_PADDING 15

/*************** object name ****************/
#define OBJECT_NMAE_PREVIEW_BUTTON "previewBtn"
#define OBJECT_NMAE_DELETE_BUTTON "deleteBtn"
#define OBJECT_NMAE_ADD_BUTTON "addBtn"
#define OBJECT_NMAE_ADD_SOURCE_BUTTON "addSourceBtn"
#define OBJECT_NMAE_SWITCH_EFFECT_BUTTON "switchEffectBtn"
#define OBJECT_NMAE_SEPERATE_BUTTON "seperateBtn"
#define OBJECT_NMAE_SEPERATE_SOURCE_BUTTON "seperateSourceBtn"
#define OBJECT_NAME_BUTTON "button"
#define OBJECT_NAME_BUTTON_BOX "buttonBox"
#define OBJECT_NAME_WIDGET "widget"
#define OBJECT_NAME_CHECKBOX "checkbox"
#define OBJECT_NAME_RADIOBUTTON "radiobutton"
#define OBJECT_NAME_PLAINTEXTEDIT "plainTextEdit"
#define OBJECT_NAME_LINEEDIT "lineedit"
#define OBJECT_NAME_BROWSE "browse"
#define OBJECT_NAME_SPINBOX "spinBox"
#define OBJECT_NAME_SLIDER "slider"
#define OBJECT_NAME_EDITABLELIST "editableList"
#define OBJECT_NAME_COMBOBOX "combobox"
#define OBJECT_NAME_FONTLABEL "fontLabel"
#define OBJECT_NAME_FORMLABEL "formLabel"
#define OBJECT_NAME_SPACELABEL "spaceLabel"
#define OBJECT_NAME_SEPERATOR_LABEL "seperatorLabel"
#define OBJECT_NAME_DISPLAYTEXT "displayText"
#define OBJECT_NAME_PROPERTYVIEW "propertyPreview"
#define OBJECT_NAME_PROPERTY_SPLITTER "propertyWindowSplitter"
#define OBJECT_NAME_PROPERTY_VIEW_CONTAINER "propertyViewContainer"
#define OBJECT_NAME_RIGHT_SCROLL_BTN "rightScrollBtn"
#define OBJECT_NAME_LEFT_SCROLL_BTN "leftScrollBtn"
#define OBJECT_NAME_COLOR_FILTER_LABEL "colorFilterLabel"
#define OBJECT_NAME_SOURCE_RENAME_EDIT "sourceRenameEditor"

#define OBJECT_NAME_FILTER_ITEM_MENU "filterItemMenu"
#define OBJECT_NAME_BASIC_FILTER_MENU "basicFilterMenu"
#define OBJECT_NAME_ADD_STICKER_BUTTON "addStickerBtn"

/*************** property name ****************/
#define PROPERTY_NAME_SHOW_IMAGE "showImage"
#define PROPERTY_NAME_ID "id"
#define PROPERTY_NAME_TRANSITION "transition"
#define PROPERTY_NAME_ICON_TYPE "type"

#define INVALID_SOURCE_ERROR_TEXT_BUTTON "displaySourceInvalid"
#define DISPLAY_RESIZE_SCREEN "displayResizeScreen"
#define DISPLAY_RESIZE_CENTER "displayResizeCenter"

#define NO_SOURCE_TEXT_LABEL "noSourceTipsLabel"

#define TRANSITION_APPLY_BUTTON "transitionApplyButton"

#define PROPERTY_NAME_SOURCE_SELECT "selected"

#define PROPERTY_NAME_STATUS "status"

#define PROPERTY_NAME_MOUSE_STATUS "status"
#define PROPERTY_VALUE_MOUSE_STATUS_NORMAL "normal"
#define PROPERTY_VALUE_MOUSE_STATUS_HOVER "hover"
#define PROPERTY_VALUE_MOUSE_STATUS_PRESSED "pressed"

} // namespace common

#endif // COMMON_COMMONDEFINE_H
