#pragma once

#include <PLSHttpApi\PLSHttpHelper.h>
#include <QObject>

struct PLSAfreecaTVLiveinfoData;
struct PLSAfreecaTVCategory;

class PLSAPIAfreecaTV : public QObject {
	Q_OBJECT

public:
	explicit PLSAPIAfreecaTV(QObject *parent = nullptr);

	static void requestDashboradData(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed);
	static void requestUsersInfoAndChannel(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed);
	static void requestUsersNickName(const QString &userID, const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed);
	static void requestCategoryList(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed);
	static void updateLiveInfo(const QObject *receiver, const QString &title, dataFunction onSucceed, dataErrorFunction onFailed);
	static void PLSAPIAfreecaTV::requestLiveID(const QObject *receiver, dataFunction onSucceed, dataErrorFunction onFailed);
	static void readDataByRegu(PLSAfreecaTVLiveinfoData &data, const QString &originStr = "");
	static void parseCategory(const QString &originStr);
	static QString getSelectCategoryString(const QString &selectID);

private:
	static void addCommonData(PLSNetworkReplyBuilder &builder, bool forceKo = false);

	static QString getFirstReg(const QString &htmlStr, const QString &regStr);
	static QString getValue(const QString &originStr, const QString &keyStr = "value");
	static QString getValueOfList(const QString &originStr);
	static QString getValueOfTags(const QString &originStr);
	static QString getValueOfFrmByeBye(const QString &originStr);
	static QString getServerUrlValue(const QString &originStr);
	static bool getIsCheck(const QString &htmlStr, const QString &getIdStr);
	static bool isEnglishLanguage();

	static void recursiveConvertCategory(const QJsonArray &categories, vector<PLSAfreecaTVCategory> &recieveData);
	static bool recursiveFindSelectStr(const QString &selectID, QStringList &allLevel, const vector<PLSAfreecaTVCategory> &recieveData);
};
