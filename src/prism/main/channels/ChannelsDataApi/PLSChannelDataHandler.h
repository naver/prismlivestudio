#ifndef CHANNELDATAHANDLER_H
#define CHANNELDATAHANDLER_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVariantMap>

using ChannelsMap = QMap<QString, QVariantMap>;

using InfosList = QList<QVariantMap>;
using UpdateCallback = void (*)(const InfosList &);
Q_DECLARE_METATYPE(UpdateCallback)

struct FinishTaskReleaser {
	FinishTaskReleaser(const QString &srcUUID) : mSrcUUID(srcUUID){};
	~FinishTaskReleaser();

private:
	QString mSrcUUID;
};

class ChannelDataBaseHandler {
public:
	virtual ~ChannelDataBaseHandler(){};
	virtual QString getPlatformName() { return ""; }
	virtual bool isMultiChildren() { return false; }

	virtual bool tryToUpdate(const QVariantMap &, UpdateCallback) { return false; };
	virtual int showLiveInfo(const QVariantMap &) { return 0; };
	virtual int refreshToken(const QVariantMap &) { return 0; };
};

/* example */
class TwitchDataHandler : public ChannelDataBaseHandler {

public:
	TwitchDataHandler();

	virtual QString getPlatformName();
	virtual bool tryToUpdate(const QVariantMap &srcInfo, UpdateCallback callback);
	virtual bool downloadHeaderImage();

protected:
	UpdateCallback mCallBack;
	QVariantMap mLastInfo;
	static ChannelsMap mDataMaper;
};

class YoutubeHandler : public TwitchDataHandler {

public:
	YoutubeHandler();
	virtual QString getPlatformName();
	virtual bool tryToUpdate(const QVariantMap &srcInfo, UpdateCallback callback);
	virtual bool refreshToken();

	bool runTasks();

	bool getRealToken();

	bool getBasicInfo();
	bool getheaderImage();

protected:
	void handleError(int statusCode);
	void resetData();

private:
	QMap<int, QVariant> mTaskMap;
};

#endif // ! CHANNELDATAHANDLER_H
