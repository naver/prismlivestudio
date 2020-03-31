#include "PLSChatHelper.h"
#include <qobject.h>
#include "../PLSPlatformApi/PLSPlatformApi.h"
#include "ChannelConst.h"
#include "PLSChannelDataAPI.h"
#include "pls-common-define.hpp"

using namespace std;

static const string defaultUrl = " "; //because cef can't use empty url, so use this temp error url.
static const QString youtubeLocalUrlPath = "data/prism-studio/chat/youtube.html";

const char *s_ChatStatusType = "status";
const char *s_ChatStatusNormal = "nomarl";
const char *s_ChatStatusOnlyOne = "onlyOne";
const char *s_ChatStatusSelect = "select";

PLSChatHelper *PLSChatHelper::instance()
{
	static PLSChatHelper _instance;
	return &_instance;
}

void onPrismUserLogout1(pls_frontend_event event, const QVariantList &params, void *context)
{
	Q_UNUSED(event);
	Q_UNUSED(params);
	Q_UNUSED(context);

	config_set_string(App()->GlobalConfig(), "BasicWindow", "geometryChat", NULL);
	config_set_bool(App()->GlobalConfig(), "Basic", "chatIsHidden", false);
	config_set_uint(App()->GlobalConfig(), KeyConfigLiveInfo, KeyTwitchServer, 0);
	config_save(App()->GlobalConfig());
}

PLSChatHelper::~PLSChatHelper()
{
	pls_frontend_remove_event_callback(pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT, onPrismUserLogout1, this);
}

void PLSChatHelper::startToNotify()
{
	pls_frontend_add_event_callback(pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT, onPrismUserLogout1, this);
}

const QString PLSChatHelper::getTabButtonCss(const QString &objectName, const QString &platName)
{
	auto shDisable = QString("PLSChatDialog #%1:disable {image:url(\":/images/chat/btn-tab-%2-off-disable.svg\");}").arg(objectName).arg(platName);
	auto shSelect = QString("PLSChatDialog #%1[%2=%3] {image:url(\":/images/chat/btn-tab-%4-on-normal.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusSelect).arg(platName);
	auto shSelectHover =
		QString("PLSChatDialog #%1[%2=%3]:hover {image:url(\":/images/chat/btn-tab-%4-on-over.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusSelect).arg(platName);
	auto shSelectPressed =
		QString("PLSChatDialog #%1[%2=%3]:pressed {image:url(\":/images/chat/btn-tab-%4-on-click.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusSelect).arg(platName);
	auto shNormal = QString("PLSChatDialog #%1 {image:url(\":/images/chat/btn-tab-%2-off-normal.svg\");}").arg(objectName).arg(platName);
	auto shNormalHover =
		QString("PLSChatDialog #%1[%2=%3]:hover {image:url(\":/images/chat/btn-tab-%4-off-over.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusNormal).arg(platName);
	auto shNormalPressed =
		QString("PLSChatDialog #%1[%2=%3]:pressed {image:url(\":/images/chat/btn-tab-%4-off-click.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusNormal).arg(platName);
	auto shOnlyOne = QString("PLSChatDialog #%1[%2=%3] {image:url(\":/images/chat/btn-tab-%4-on-normal.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusOnlyOne).arg(platName);
	auto shOnlyOneHover =
		QString("PLSChatDialog #%1[%2=%3]:hover {image:url(\":/images/chat/btn-tab-%4-on-normal.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusOnlyOne).arg(platName);
	auto shOnlyOnePressed =
		QString("PLSChatDialog #%1[%2=%3]:pressed {image:url(\":/images/chat/btn-tab-%4-on-normal.svg\");}").arg(objectName).arg(s_ChatStatusType).arg(s_ChatStatusOnlyOne).arg(platName);

	QString styleSheet = shDisable.append(shSelectHover)
				     .append(shSelect)
				     .append(shSelectPressed)
				     .append(shNormalHover)
				     .append(shNormal)
				     .append(shNormalPressed)
				     .append(shOnlyOne)
				     .append(shOnlyOneHover)
				     .append(shOnlyOnePressed);
	return styleSheet;
}

void PLSChatHelper::sendWebShownEventIfNeeded(PLSChatHelper::ChatPlatformIndex index)
{
	if (!isLocalHtmlPage(index)) {
		return;
	}

	auto name = PLS_CHAT_HELPER->getPlatformNameFromIndex(index);
	if (name.isEmpty() && index != ChatPlatformIndex::ChatIndexAll) {
		return;
	}
}

bool PLSChatHelper::isLocalHtmlPage(PLSChatHelper::ChatPlatformIndex index)
{
	return (index == PLSChatHelper::ChatPlatformIndex::ChatIndexAll || index == PLSChatHelper::ChatPlatformIndex::ChatIndexNaverTV || index == PLSChatHelper::ChatPlatformIndex::ChatIndexVLive ||
		index == PLSChatHelper::ChatPlatformIndex::ChatIndexAfreecaTV || index == PLSChatHelper::ChatPlatformIndex::ChatIndexFacebook ||
		index == PLSChatHelper::ChatPlatformIndex::ChatIndexYoutube);
}
const char *PLSChatHelper::getString(PLSChatHelper::ChatPlatformIndex index, bool toLowerSpace)
{
	QString name = "";
	switch (index) {
	case PLSChatHelper::ChatPlatformIndex::ChatIndexAll:
		name = "All";
		break;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP:
		name = "Rtmp";
		break;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexTwitch:
		name = "Twitch";
		break;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexYoutube:
		name = "YouTube";
		break;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexNaverTV:
		name = "NAVER TV";
		break;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexVLive:
		name = "V LIVE";
		break;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexFacebook:
		name = "Facebook";
		break;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexAfreecaTV:
		name = "afreecaTV";
		break;
	default:
		name = "Other";
		break;
	}
	if (toLowerSpace) {
		name = name.toLower();
		name = name.replace(" ", "");
	}
	static std::string str;
	str = name.toStdString();
	return str.c_str();
}

PLSChatHelper::ChatPlatformIndex PLSChatHelper::getIndexFromInfo(const QVariantMap &info)
{
	PLSChatHelper::ChatPlatformIndex index = PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine;

	if (info.empty()) {
		return index;
	}
	QString platformName = info.value(ChannelData::g_channelName, "").toString();
	auto dataType = info.value(ChannelData::g_data_type, ChannelData::RTMPType).toInt();

	if (dataType == ChannelData::RTMPType) {
		index = PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP;
	} else if (dataType == ChannelData::ChannelType) {
		if (platformName == TWITCH) {
			index = PLSChatHelper::ChatPlatformIndex::ChatIndexTwitch;
		} else if (platformName == YOUTUBE) {
			index = PLSChatHelper::ChatPlatformIndex::ChatIndexYoutube;
		} else if (platformName == NAVER_TV) {
			index = PLSChatHelper::ChatPlatformIndex::ChatIndexNaverTV;
		} else if (platformName == VLIVE) {
			index = PLSChatHelper::ChatPlatformIndex::ChatIndexVLive;
		} else if (platformName == FACEBOOK) {
			index = PLSChatHelper::ChatPlatformIndex::ChatIndexFacebook;
		} else if (platformName == AFREECATV) {
			index = PLSChatHelper::ChatPlatformIndex::ChatIndexAfreecaTV;
		}
	}
	return index;
}

void PLSChatHelper::getSelectInfoFromIndex(PLSChatHelper::ChatPlatformIndex index, QVariantMap &getInfo)
{
	QVariantMap &selectInfo = QVariantMap();
	for (const auto &info : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		if (info.empty()) {
			continue;
		}
		auto dataType = info.value(ChannelData::g_data_type, ChannelData::RTMPType).toInt();
		if (dataType != ChannelData::ChannelType) {
			continue;
		}
		auto &name = getPlatformNameFromIndex(index);
		QString platformName = info.value(ChannelData::g_channelName, "").toString();
		if (!name.isEmpty() && name == platformName) {
			selectInfo = info;
			break;
		}
	}
	getInfo = selectInfo;
}

QString PLSChatHelper::getPlatformNameFromIndex(PLSChatHelper::ChatPlatformIndex index)
{
	switch (index) {
	case PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP:
	case PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine:
		return QString();
	case PLSChatHelper::ChatPlatformIndex::ChatIndexTwitch:
		return TWITCH;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexYoutube:
		return YOUTUBE;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexNaverTV:
		return NAVER_TV;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexVLive:
		return VLIVE;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexFacebook:
		return FACEBOOK;
	case PLSChatHelper::ChatPlatformIndex::ChatIndexAfreecaTV:
		return AFREECATV;
	default:
		return QString();
	}
}

QString PLSChatHelper::getRtmpPlaceholderString()
{
	QString showStr;
	if (PLS_PLATFORM_API->isLiving() && !PLS_PLATFORM_API->isPlatformActived(PLSServiceType::ST_BAND)) {
		showStr = QObject::tr("chat.rtmp.placehoder.living");
	} else if (PLSCHANNELS_API->getCurrentSelectedChannels().size() == 0) {
		showStr = QObject::tr("chat.rtmp.placehoder.nochannel");
	} else {
		if (PLS_PLATFORM_API->isPlatformActived(PLSServiceType::ST_BAND)) {
			showStr = QObject::tr("Chat.Band.Not.Supported");
		} else {
			showStr = QObject::tr("chat.rtmp.placehoder.onlyrtmp");
		}
	}
	return showStr;
}

bool PLSChatHelper::isCefWidgetIndex(PLSChatHelper::ChatPlatformIndex index)
{
	return (PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP != index && PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine != index);
}

bool PLSChatHelper::canChatYoutube(const QVariantMap &info, bool checkForKids) const
{
	QString category_en = info.value(ChannelData::g_catogryTemp, "").toString();
	if (category_en.isEmpty()) {
		category_en = info.value(ChannelData::g_catogry, "").toString();
	}

	bool isPrivate = tr("youtube.privacy.private.en") == category_en;
	if (isPrivate || checkForKids && PLS_PLATFORM_YOUTUBE->getSelectData().isForKids) {
		return false;
	}
	return true;
}

std::string PLSChatHelper::getChatUrlWithIndex(PLSChatHelper::ChatPlatformIndex index, const QVariantMap &info)
{
	return "";
}

bool PLSChatHelper::showToastWhenStart(QString &showStr)
{
	if (!PLS_PLATFORM_API->isPrismLive()) {
		return false;
	}
	auto hasRtmp = false;
	auto hasBand = false;
	auto hasPlatforms = false;
	auto platformActived = PLS_PLATFORM_ACTIVIED;
	for (auto item : platformActived) {
		if (PLSServiceType::ST_RTMP == item->getServiceType()) {
			hasRtmp = true;
			continue;
		}
		if (PLSServiceType::ST_BAND == item->getServiceType()) {
			hasBand = true;
			continue;
		}
		hasPlatforms = true;
	}

	if (!hasPlatforms) {
		return false;
	}
	if (hasRtmp && hasBand) {
		showStr = QTStr("Chat.Band.And.RTMP.Not.Supported");
	} else if (hasRtmp) {
		showStr = QTStr("Chat.Toast.RTMP.Not.Supported");
	} else if (hasBand) {
		showStr = QTStr("Chat.Band.Not.Supported");
	}
	return !showStr.isEmpty();
}
