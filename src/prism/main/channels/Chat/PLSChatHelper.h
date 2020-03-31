#pragma once
#include <QObject>
#include <QVariantMap>

extern const char *s_ChatStatusType;
extern const char *s_ChatStatusNormal;
extern const char *s_ChatStatusOnlyOne;
extern const char *s_ChatStatusSelect;

class PLSChatHelper : public QObject {
	Q_OBJECT

public:
	enum ChatPlatformIndex {
		ChatIndexAll = 0,
		ChatIndexRTMP,
		ChatIndexTwitch,
		ChatIndexYoutube,
		ChatIndexFacebook,
		ChatIndexNaverTV,
		ChatIndexVLive,
		ChatIndexAfreecaTV,
		ChatIndexUnDefine,
	};

	static PLSChatHelper *instance();
	PLSChatHelper::~PLSChatHelper();

public:
	bool isLocalHtmlPage(PLSChatHelper::ChatPlatformIndex index);
	//toLowerSpace: change to lower and remove white space
	const char *getString(PLSChatHelper::ChatPlatformIndex index, bool toLowerSpace = false);
	PLSChatHelper::ChatPlatformIndex getIndexFromInfo(const QVariantMap &info);
	//only for channel select info
	void getSelectInfoFromIndex(PLSChatHelper::ChatPlatformIndex, QVariantMap &getInfo);
	QString getPlatformNameFromIndex(PLSChatHelper::ChatPlatformIndex index);
	QString getRtmpPlaceholderString();
	bool isCefWidgetIndex(PLSChatHelper::ChatPlatformIndex index);
	bool canChatYoutube(const QVariantMap &info, bool checkForKids) const;
	std::string getChatUrlWithIndex(PLSChatHelper::ChatPlatformIndex index, const QVariantMap &info);
	bool showToastWhenStart(QString &showStr);
	void startToNotify();
	const QString getTabButtonCss(const QString &objectName, const QString &platName);
	void sendWebShownEventIfNeeded(PLSChatHelper::ChatPlatformIndex index);
};

#define PLS_CHAT_HELPER PLSChatHelper::instance()
