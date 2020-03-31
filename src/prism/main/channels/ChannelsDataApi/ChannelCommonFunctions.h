#ifndef CHANELCOMMONFUNCTION_H
#define CHANELCOMMONFUNCTION_H

#include <QBitmap>
#include <QDesktopServices>
#include <QDir>
#include <QJsonDocument>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMap>
#include <QNetworkInterface>
#include <QPainter>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QSemaphore>
#include <QString>
#include <QVariantMap>
#include "ChannelConst.h"
#include "ChannelDefines.h"
#include "LogPredefine.h"

#include "PLSChannelDataAPI.h"
#include "PLSChannelDataHandler.h"
#include "alert-view.hpp"
#include "frontend-api.h"

#define ADD_CHANNELS_H(str) "Channels." #str

#ifdef QT_DEBUG
#define REGISTERSTR(str) PLSCHANNELS_API->RegisterStr(ADD_CHANNELS_H(str))
#else
#define REGISTERSTR(str) 0
#endif

#define CHANNELS_TR(str) (REGISTERSTR(str), QObject::tr(ADD_CHANNELS_H(str)))

#ifdef QT_DEBUG

/*for debug view json or map data */
#define ViewJsonByteArray(arrayData)                               \
	{                                                          \
		QString path = QDir::homePath() + "/tmpTest.json"; \
		writeFile(arrayData, path);                        \
		QDesktopServices::openUrl(path);                   \
	}

#define ViewMapData(MapData)                                        \
	{                                                           \
		auto jsonDoc = QJsonDocument::fromVariant(MapData); \
		auto arrayData = jsonDoc.toJson();                  \
		ViewJsonByteArray(arrayData);                       \
	}

#define ViewJsonDoc(JsonDoc)                       \
	{                                          \
		auto arrayData = JsonDoc.toJson(); \
		ViewJsonByteArray(arrayData);      \
	}

#else

#define ViewMapData(MapData)
#define ViewJsonDoc(JsonDoc)

#endif // DEBUG

/* function for get and Type value of any QvariantMap */
template<typename RetType = QString> inline auto getInfo(const QVariantMap &source, const QString &key, const RetType &defaultData = RetType()) -> RetType
{
	return source.value(key, QVariant::fromValue(defaultData)).value<RetType>();
}

/* function for get and Type value of any QvariantMap */
template<typename RetType = QString> inline auto takeInfo(QVariantMap &source, const QString &key, const RetType &defaultData = RetType()) -> RetType
{
	auto ite = source.find(key);
	if (ite == source.end()) {
		return defaultData;
	}
	auto retValue = ite->value<RetType>();
	source.erase(ite);
	return retValue;
}

template<typename RetType> inline RetType getInfoOfObject(const QObject *obj, const char *key, const RetType &defaultData = RetType())
{
	auto variantV = obj->property(key);
	if (!variantV.isValid()) {
		return defaultData;
	}
	return variantV.value<RetType>();
}

/* to find matched key file in dir path recursive */
QString findFileInResources(const QString &dirPath, const QString &key);

/* delete widget function for using with smart pointer */
template<typename WidgetType = QWidget> void deleteChannelWidget(WidgetType *channelWid)
{
	channelWid->deleteLater();
	return;
}

/* delete list item function for using with smart pointer */
void deleteItem(QListWidgetItem *item);

/* convert pointer to qobeject */
template<typename SRCType> inline QObject *convertToObejct(SRCType *srcPt)
{
	return dynamic_cast<QObject *>(srcPt);
}

QVariantMap createDefaultChannelInfoMap(const QString &channelName, int defaultType = ChannelData::ChannelType);
const QString createUUID();
const QString getPlatformImageFromName(const QString &channelName, const QString &prefix = "", const QString &surfix = ".*profile");
void getComplexImageOfChannel(const QString &uuid, QString &userIcon, QString &platformIcon, const QString &prefix = "stremsetting-", const QString &surfix = "");
QString getYoutubeShareUrl(const QString &broadCastID);

/*get local machine info */
const QString getHostMacAddress();

/*write json array to file */
bool writeFile(const QByteArray &array, const QString &path);

void loadPixmap(QPixmap &pix, const QString &pixmapPath, const QSize &pixSize);

/* to create a circle mask for user header */
template<typename SourceType> QBitmap createImageCircleMask(const SourceType &source)
{

	auto size = source.size();
	double radius = (qMin(size.height(), size.width())) / 2.0;
	if (radius < 10) {
		radius = 10;
		size.setHeight(radius);
		size.setWidth(radius);
	}

	QBitmap mask(size);
	mask.fill(Qt::white);
	QPainter painter;
	painter.begin(&mask);
	QPen tmpPen(Qt::black, 0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	QBrush brush(Qt::black, Qt::SolidPattern);
	painter.setPen(tmpPen);
	painter.setBrush(brush);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	QPointF center(size.width() / 2.0, size.height() / 2.0);
	painter.translate(center);
	painter.drawEllipse(QPointF(0, 0), radius, radius);

	painter.end();
	return mask;
}

QPixmap &getCubePix(QPixmap &src);

template<typename SourceType> SourceType &circleMaskImage(SourceType &source)
{
	auto mask = createImageCircleMask(source);
	if (mask.size() == source.size()) {
		source.setMask(mask);
	}
	return source;
}
template<typename SourceType> QBitmap createImageRoundRectMask(const SourceType &source, const SourceType &sourceInner)
{
	auto size = source.size();
	QBitmap mask(size);
	mask.fill(Qt::white);
	QPainter painter;
	painter.begin(&mask);
	QPen tmpPen(Qt::black, 0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	QBrush brush(Qt::black, Qt::SolidPattern);
	painter.setPen(tmpPen);
	painter.setBrush(brush);
	painter.setClipping(true);
	painter.setRenderHint(QPainter::Antialiasing);
	auto actRect = sourceInner.adjusted(2, 2, -5, -4);
	painter.drawRoundedRect(actRect, actRect.height() / 2.0 + 0.5, actRect.height() / 2.0 + 3.5);
	painter.end();
	return mask;
}

/* format json array and write to file */
template<typename DataType> bool writeJsonFile(const DataType &array, const QString &path)
{
	QJsonDocument document(array);
	return writeFile(document.toJson(QJsonDocument::Indented), path);
}

/* format json array */
void formatJson(QByteArray &array);

/* add srcmap to destmap */
template<typename MapType> auto addToMap(MapType &destMap, const MapType &srcMap) -> MapType
{
	auto srcIte = srcMap.cbegin();
	for (; srcIte != srcMap.cend(); ++srcIte) {
		destMap.insert(srcIte.key(), srcIte.value());
	}
	return destMap;
}

/* add srcmap to destmap */
template<typename MapType> auto addToMap(MapType &destMap, const MapType &srcMap, const MapType &mapper) -> MapType
{
	auto srcIte = srcMap.cbegin();
	for (; srcIte != srcMap.cend(); ++srcIte) {
		if (mapper.contains(srcIte.key())) {
			const auto &tagetKey = mapper[srcIte.key()].toString();
			destMap.insert(tagetKey, srcIte.value());
		}
	}
	return destMap;
}

template<typename EnumType> QDataStream &operator<<(QDataStream &out, const EnumType &src)
{
	out << int(src);
	return out;
}

template<typename EnumType> QDataStream &operator>>(QDataStream &in, EnumType &target)
{
	int value;
	in >> value;
	target = EnumType(value);
	return in;
}

#define RegisterEnum(x)                                                       \
	template QDataStream &operator<<<x>(QDataStream &out, const x &type); \
	template QDataStream &operator>><x>(QDataStream &in, x &type);

RegisterEnum(ChannelData::ChannelStatus);
RegisterEnum(ChannelData::ChannelUserStatus);
RegisterEnum(ChannelData::ChannelDataType);
RegisterEnum(ChannelData::LiveState);
RegisterEnum(ChannelData::RecordState);

template<typename DataType> bool saveDataXToFile(const DataType &srcData, const QString &path)
{
	QFile file(path);
	if (file.open(QIODevice::WriteOnly)) {
		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_5_12);
		out << srcData;
		return true;
	}
	return false;
}

template<typename DataType> bool loadDataFromFile(DataType &destData, const QString &path)
{
	QFile file(path);
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_5_12);
		in >> destData;
		return true;
	}
	return false;
}

template<typename FunctionType, typename ClassPtr = PLSChannelDataAPI *> struct HolderReleaser {
	HolderReleaser(FunctionType function, ClassPtr objPtr = PLSCHANNELS_API) : mpf(function), mObjPtr(objPtr) { (*mObjPtr.*mpf)(true); }
	~HolderReleaser() { (*mObjPtr.*mpf)(false); }
	FunctionType mpf;
	ClassPtr mObjPtr;
};

struct SemaphoreHolder {
	SemaphoreHolder(QSemaphore &src) : mSem(src) { mSem.release(1); }
	~SemaphoreHolder() { mSem.acquire(1); }

private:
	QSemaphore &mSem;
};

template<typename ReplyPt> int getReplyStatusCode(ReplyPt reply)
{
	return reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

void requestStartLog(const QString &url, const QString &requestType);

template<typename ReplyType> void formatNetworkLogs(ReplyType reply, const QByteArray &data)
{
	int code = 0;
	auto doc = QJsonDocument::fromJson(data);
	if (!doc.isNull()) {
		auto obj = doc.object();
		if (!obj.isEmpty()) {
			code = obj.value("code").toInt();
		}
	}
	QString msg = reply->url().toString() + " statusCode = " + QString::number(getReplyStatusCode(reply)) + " code = " + QString::number(code) + ".";
	if (code != QNetworkReply::NoError) {
		msg.prepend("http request error! url = ");
		msg += "\n error string :" + reply->errorString();
		PLS_ERROR("Channels", "%s", msg.toStdString().c_str());
		PRE_LOG_MSG(("reply content :" + reply->readAll()).constData(), INFO);
	} else {
		msg.prepend("http request successfull! url = ");
		PLS_INFO("Channels", "%s", msg.toStdString().c_str());
	}
}

template<typename ReplyType> void ChannelsNetWorkPretestWithAlerts(ReplyType reply, const QByteArray &data, bool notify = true)
{
	formatNetworkLogs(reply, data);
	if (!notify) {
		return;
	}
	QVariantMap errormap;
	auto errorValue = reply->error();
	if (errorValue <= QNetworkReply::UnknownNetworkError || errorValue == QNetworkReply::UnknownServerError) {
		errormap.insert(g_errorTitle, CHANNELS_TR(Check.Alert.Title));
		errormap.insert(g_errorString, CHANNELS_TR(Check.Network.Error));

	} else {
		return;
	}
	PLSCHANNELS_API->addError(errormap);
	PLSCHANNELS_API->networkInvalidOcurred();
}

const QString getChannelCacheFilePath();
const QString getChannelCacheDir();
const QString getChannelSettingsFilePath();
const QStringList getDefaultPlatforms();
const QString guessPlatformFromRTMP(const QString &rtmpUrl);

QPropertyAnimation *createShowAnimation(QWidget *wid, int msSec = 250);
void displayWidgetWithAnimation(QWidget *wid, int msSec = 250, bool show = true);
QPropertyAnimation *createHideAnimation(QWidget *wid, int msSec = 250);

void moveWidgetToParentCenter(QWidget *wid);

/* repolish any widget when you change some property of widget*/
void refreshStyle(QWidget *widget);

/*get elide text of widget*/
QString getElidedText(QWidget *widget, const QString &srcTxt, double minWidth, Qt::TextElideMode mode = Qt::ElideRight, int flag = 0);

/*set a single css of widget */
void setStyleSheetFromFile(const QString &fileStr, QWidget *wid);

/* set  css of multi file for wid */
template<typename... Args> void setStyleSheetFromFile(QWidget *wid, Args... args)
{
	QStringList files{args...};
	if (!files.isEmpty()) {
		QString styleSteets;
		for (const QString &fileStr : files) {
			QFile file(fileStr);
			if (file.open(QIODevice::ReadOnly)) {
				styleSteets.append(file.readAll() + "\n");
			}
		}
		wid->setStyleSheet(styleSteets);
	}
}

template<typename ParentType = QWidget *, typename ChildType = QWidget *> auto findParent(ChildType child) -> ParentType
{
	auto tmpP = child->parent();
	if (tmpP == nullptr) {
		return nullptr;
	}
	auto retP = dynamic_cast<ParentType>(tmpP);
	if (retP == nullptr) {
		return findParent<ParentType>(tmpP);
	}
	return retP;
}

template<typename DestType, typename FunctionType> auto getMemberPointer(FunctionType func) -> DestType
{
	return *static_cast<DestType *>((reinterpret_cast<void *>(&func)));
}

#endif //CHANELCOMMONFUNCTION_H
