#include "PLSAPIAfreecaTV.h"
#include <qfileinfo.h>
#include <QNetworkCookieJar>
#include <vector>
#include "../PLSPlatformBase.hpp"
#include "ChannelCommonFunctions.h"
#include "PLSChannelDataAPI.h"
#include "PLSHttpApi/PLSHttpHelper.h"
#include "PLSPlatformAfreecaTV.h"
#include "PLSPlatformApi.h"
#include "frontend-api.h"
#include "pls-common-define.hpp"
#include "pls-gpop-data.hpp"

using namespace std;

PLSAPIAfreecaTV::PLSAPIAfreecaTV(QObject *parent) : QObject(parent) {}

void PLSAPIAfreecaTV::requestUsersInfoAndChannel(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	PLSHmacNetworkReplyBuilder builder(g_plsAfreecaTVChannelInfo, HmacType::HT_NONE);
	addCommonData(builder);
	PLS_HTTP_HELPER->connectFinished(builder.get(), receiver, onSucceed, onFailed);
}

void PLSAPIAfreecaTV::requestUsersNickName(const QString &userID, const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	QString url = g_plsAfreecaTVUserNick.arg(userID);
	PLSHmacNetworkReplyBuilder builder(url, HmacType::HT_NONE);
	addCommonData(builder);
	PLS_HTTP_HELPER->connectFinished(builder.get(), receiver, onSucceed, onFailed);
}

void PLSAPIAfreecaTV::requestDashboradData(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	PLSHmacNetworkReplyBuilder builder(g_plsAfreecaTVDashboard, HmacType::HT_NONE);
	//only in ko can get frmAccessCode value
	addCommonData(builder, true);
	PLS_HTTP_HELPER->connectFinished(builder.get(), receiver, onSucceed, onFailed);
}

void PLSAPIAfreecaTV::requestCategoryList(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed)
{
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	QString url = g_plsAfreecaTVCategories.arg(isEnglishLanguage() ? "en_US" : "ko_KR");
	PLSHmacNetworkReplyBuilder builder(url, HmacType::HT_NONE);
	addCommonData(builder);
	PLS_HTTP_HELPER->connectFinished(builder.get(), receiver, onSucceed, onFailed);
}

void PLSAPIAfreecaTV::requestLiveID(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed)
{
	//id is "CHANNEL"-> "BNO"
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	const auto &channelData = PLSCHANNELS_API->getChanelInfoRef(PLS_PLATFORM_AFREECATV->getChannelUUID());
	auto channelId = channelData.value(ChannelData::g_subChannelId, "").toString();
	QString url = g_plsAfreecaTVLiveID.arg(channelId);
	PLSHmacNetworkReplyBuilder builder(url, HmacType::HT_NONE);
	builder.addField("bid", channelId);
	PLS_HTTP_HELPER->connectFinished(builder.get(), receiver, onSucceed, onFailed);
}

void PLSAPIAfreecaTV::updateLiveInfo(const QObject *receiver, const QString &title, dataFunction onSucceed, dataErrorFunction onFailed)
{
	/*
work		live_update
frmCategory	00020010
is_wait		Y
waiting_time	5
frmTitle	000000000000
frmViewer	777
frmWaterMark	6
frmHashTags	ahahaha,popopopo
frmByeBye	878984998
encode_type	normal
frmStreamKey	abby0816-2146523166
*/
	PLS_INFO(MODULE_PlatformService, __FUNCTION__ " start");
	PLSHmacNetworkReplyBuilder builder(g_plsAfreecaTVUpdate, HmacType::HT_NONE);
	addCommonData(builder);
	QJsonObject object = QJsonObject();

	const auto &data = PLS_PLATFORM_AFREECATV->getSelectData();

	object["work"] = data.work;
	object["frmCategory"] = data.frmCategoryID;
	object["is_wait"] = data.is_wait;
	if (data.b_showFrmWait) {
		object["waiting_time"] = data.frmWaitTime;
		if (PLS_PLATFORM_API->isLiving()) {
			object["frmWait"] = data.frmWait;
			object["frmWaitTime"] = data.frmWaitTime; //not call living
		}
	}
	object["frmTitle"] = title;
	object["frmViewer"] = data.frmViewer;
	object["frmWaterMark"] = data.frmWaterMark;
	object["frmHashTags"] = data.frmHashTags;
	object["frmByeBye"] = data.frmByeBye;
	object["encode_type"] = data.encode_type;
	object["frmStreamKey"] = data.frmStreamKey;
	if (data.b_frmAdult) {
		object["frmAdult"] = data.frmAdult;
	}
	if (data.b_frmHidden) {
		object["frmHidden"] = data.frmHidden;
	}

	if (data.b_frmTuneOut) {
		object["frmTuneOut"] = data.frmTuneOut;
	}
	if (data.b_containFrmAccess) {
		object["frmAccess"] = data.frmAccess;
		object["frmAccessCode"] = data.frmAccessCode;
	}

	builder.setFields(object.toVariantMap());
	PLS_HTTP_HELPER->connectFinished(builder.post(), receiver, onSucceed, onFailed);
}

void PLSAPIAfreecaTV::addCommonData(PLSNetworkReplyBuilder &builder, bool forceKo)
{
	builder.addRawHeader("Cookie", PLS_PLATFORM_AFREECATV->getChannelCookie());
	QString acceptLanguage = "ko;q=0.9,en;q=0.8,zh;q=0.7,ja;q=0.6";

	if (isEnglishLanguage() && !forceKo) {
		acceptLanguage = "en;q=0.9,ko;q=0.8,zh;q=0.7,ja;q=0.6";
	}
	builder.addRawHeader("Accept-Language", acceptLanguage);
}
void PLSAPIAfreecaTV::readDataByRegu(PLSAfreecaTVLiveinfoData &data, const QString &originStr)
{
	QString htmlStr = originStr;
	QString compareS = "";

	QString regTemp("name=\"%1\"([\\s\\S]*)>");
	QString frmWaitTimeReg("<select[ ]*name=\"frmWaitTime\"([\\s\\S]*)</select>");
	QString frmByeByeReg("name=\"frmByeBye\"([\\s\\S]*)</textarea>");
	QString tagsReg("var[ ]*szTags[ ]*=([\\s\\S]*);");
	QString serverUrlReg("((id=\"ServerUrl\"))([\\s\\S]*)(</p>)");

	compareS = getFirstReg(htmlStr, regTemp.arg("work"));
	data.work = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmCategory"));
	data.frmCategoryID = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("is_wait"));
	data.is_wait = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmWait"));
	data.frmWait = getValue(compareS);
	data.b_showFrmWait = data.frmWait == "Y";

	compareS = getFirstReg(htmlStr, regTemp.arg("waiting_time"));
	data.waiting_time = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmTitle"));
	data.frmTitle = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmTuneOut"));
	data.b_frmTuneOut = getIsCheck(htmlStr, compareS);
	data.frmTuneOut = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmAdult"));
	data.b_frmAdult = getIsCheck(htmlStr, compareS);
	data.frmAdult = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmHidden"));
	data.b_frmHidden = getIsCheck(htmlStr, compareS);
	data.frmHidden = getValue(compareS);

	compareS = getFirstReg(htmlStr, frmWaitTimeReg);
	data.frmWaitTime = getValueOfList(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmViewer"));
	data.frmViewer = getValue(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmWaterMark"));
	data.frmWaterMark = getValue(compareS);

	compareS = getFirstReg(htmlStr, tagsReg);
	data.frmHashTags = getValueOfTags(compareS);

	compareS = getFirstReg(htmlStr, frmByeByeReg);
	data.frmByeBye = getValueOfFrmByeBye(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmStreamKey"));
	data.frmStreamKey = getValue(compareS);

	compareS = getFirstReg(htmlStr, serverUrlReg);
	data.frmServerUrl = getValueOfFrmByeBye(compareS);

	compareS = getFirstReg(htmlStr, regTemp.arg("frmAccess"));
	data.b_containFrmAccess = compareS.contains("checked");
	if (data.b_containFrmAccess) {
		data.frmAccess = getValue(compareS);
		compareS = getFirstReg(htmlStr, regTemp.arg("frmAccessCode"));
		data.frmAccessCode = getValue(compareS);
	}
}

void PLSAPIAfreecaTV::parseCategory(const QString &originStr)
{
	QString convertStr = "";
	QString oriStr = originStr;
	oriStr.remove(QRegExp("^var[ ]*szBroadCategory[ ]*=[ ]*"));
	oriStr.remove(QRegExp(";*$"));

	auto doc = QJsonDocument::fromJson(oriStr.toUtf8());
	if (doc.isNull()) {
		return;
	}

	QJsonObject jsonObject = doc.object();
	vector<PLSAfreecaTVCategory> vecCategory;
	auto categories = jsonObject["CHANNEL"].toObject()["BROAD_CATEGORY"].toArray();
	recursiveConvertCategory(categories, vecCategory);
	if (!vecCategory.empty()) {
		PLS_PLATFORM_AFREECATV->setCategories(vecCategory);
	}
}

QString PLSAPIAfreecaTV::getFirstReg(const QString &htmlStr, const QString &regStr)
{
	QString pattern(regStr);
	QRegExp rx(pattern);
	rx.setMinimal(true);
	int pos = htmlStr.indexOf(rx);
	QString compareS = "";
	if (pos >= 0) {
		compareS = rx.cap(0);
	}
	return compareS;
}

QString PLSAPIAfreecaTV::getValue(const QString &originStr, const QString &keyStr)
{
	QString pattern = QString("%1=\"[^\"]+").arg(keyStr);
	QRegExp rx(pattern);
	int pos = originStr.indexOf(rx);
	QString compareS = "";
	if (pos >= 0) {
		compareS = rx.cap(0);
	}
	QString removeStr = QString("%1=\"").arg(keyStr);
	compareS.remove(removeStr);
	return compareS;
}

QString PLSAPIAfreecaTV::getValueOfList(const QString &originStr)
{
	QString selectValue = "";
	QStringList list = originStr.split("/option");
	for (int i = 0; i < list.size(); i++) {
		QString subList = list[i];
		QString subValue = getValue(subList);
		if (subList.contains("selected")) {
			selectValue = subValue;
		}
	}
	return selectValue;
}

QString PLSAPIAfreecaTV::getValueOfTags(const QString &originStr)
{
	QString selectValue = "";
	QStringList list = originStr.split("'");
	if (list.size() > 1) {
		selectValue = list[1];
	}
	return selectValue;
}

QString PLSAPIAfreecaTV::getValueOfFrmByeBye(const QString &originStr)
{
	QString selectValue = "";
	QStringList list = originStr.split(">");
	if (list.size() > 1) {
		selectValue = list[1].split("<")[0];
	}
	return selectValue;
}

QString PLSAPIAfreecaTV::getServerUrlValue(const QString &originStr)
{
	QString selectValue = "";
	QStringList list = originStr.split(">");
	if (list.size() > 1) {
		selectValue = list[1];
	}
	return selectValue;
}

bool PLSAPIAfreecaTV::getIsCheck(const QString &htmlStr, const QString &getIdStr)
{
	auto idValue = getValue(getIdStr, "id");
	QString regTemp("label[ ]*for=\"%1\"([\\s\\S]*)>");
	auto classStr = getFirstReg(htmlStr, regTemp.arg(idValue));
	return classStr.contains("class=\"on\"");
}

bool PLSAPIAfreecaTV::isEnglishLanguage()
{
	return (QString(App()->GetLocale()).contains("en", Qt::CaseInsensitive));
}

void PLSAPIAfreecaTV::recursiveConvertCategory(const QJsonArray &categories, vector<PLSAfreecaTVCategory> &recieveData)
{
	for (int i = 0; i < categories.size(); i++) {
		auto dic = categories[i].toObject();
		PLSAfreecaTVCategory data;
		data.cate_name = dic["cate_name"].toString();
		data.cate_no = dic["cate_no"].toString();
		data.ucc_cate = dic["ucc_cate"].toString();
		data.display = dic["display"].toString();
		auto subItems = dic["child"].toArray();
		if (!subItems.isEmpty()) {
			recursiveConvertCategory(subItems, data.child);
		}
		recieveData.push_back(data);
	}
}

QString PLSAPIAfreecaTV::getSelectCategoryString(const QString &selectID)
{
	const auto &categories = PLS_PLATFORM_AFREECATV->getCategories();
	QStringList allLevel;
	recursiveFindSelectStr(selectID, allLevel, categories);
	auto str = allLevel.join(" > ");
	return str;
}

bool PLSAPIAfreecaTV::recursiveFindSelectStr(const QString &selectID, QStringList &allLevel, const vector<PLSAfreecaTVCategory> &recieveData)
{
	bool isFind = false;
	for (auto data : recieveData) {
		auto tempList = allLevel;
		if (data.cate_no == selectID) {
			tempList.append(data.cate_name);
			allLevel = tempList;
			isFind = true;
			break;
		}

		if (!data.child.empty()) {
			tempList.append(data.cate_name);
			isFind = recursiveFindSelectStr(selectID, tempList, data.child);
			if (isFind) {
				allLevel = tempList;
				break;
			}
		}
	}
	return isFind;
}
