#include "ChannelCommonFunctions.h"
#include <QCoreApplication>
#include <QDatastream>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QSvgRenderer>
#include <QUUid>
#include <QUrl>
#include "ChannelConst.h"

#include "PLSChannelDataAPI.h"
#include "PLSChannelsVirualAPI.h"
#include "frontend-api.h"

#define QRCPATH ":/images"
using namespace ChannelData;

QString findFileInResources(const QString &dirPath, const QString &key)
{
	QDir dir(dirPath);
	QRegularExpression rex(key, QRegularExpression::CaseInsensitiveOption);
	auto filesList = dir.entryInfoList();
	for (const auto &file : filesList) {
		QString filename = file.fileName();
		auto match = rex.match(filename);
		if (match.hasMatch()) {
			return file.absoluteFilePath();
		}
		if (file.isDir()) {
			auto retStr = findFileInResources(file.absoluteFilePath(), key);
			if (!retStr.isEmpty()) {
				return retStr;
			}
		}
	}

	return QString();
}

void deleteItem(QListWidgetItem *item)
{
	auto listWid = item->listWidget();
	auto widget = listWid->itemWidget(item);
	if (widget) {
		widget->deleteLater();
	}
	if (listWid) {
		listWid->removeItemWidget(item);
		listWid->takeItem(listWid->row(item));
	}

	delete item;
}

QVariantMap createDefaultChannelInfoMap(const QString &channelName, int defaultType)
{
	QVariantMap channelInfo;
	QString uuid = createUUID();
	channelInfo.insert(g_channelUUID, uuid);
	channelInfo.insert(g_channelName, channelName);
	channelInfo.insert(g_nickName, channelName);
	channelInfo.insert(g_userName, channelName);
	channelInfo.insert(g_data_type, defaultType);
	channelInfo.insert(g_channelStatus, InValid);
	channelInfo.insert(g_channelUserStatus, Disabled);
	channelInfo.insert(g_createTime, QDateTime::currentDateTime());
	channelInfo.insert(g_isUpdated, false);
	channelInfo.insert(g_displayState, true);
	channelInfo.insert(g_isLeader, true);

	int index = -1;
	if (defaultType == ChannelType) {
		auto defaultPlatforms = getDefaultPlatforms();
		index = defaultPlatforms.indexOf(channelName);
	} else if (defaultType == RTMPType) {
		channelInfo.insert(g_rtmpUserID, "");
		channelInfo.insert(g_password, "");
		channelInfo.insert(g_channelName, RTMPT_DEFAULT_TYPE);
	}
	channelInfo.insert(g_displayOrder, index);
	return channelInfo;
}

const QString createUUID()
{
	return QUuid::createUuid().toString();
}
const QString getPlatformImageFromName(const QString &channelName, const QString &prefix, const QString &surfix)
{
	auto searchKey = prefix + channelName + surfix;
	searchKey.remove(" ");
	return findFileInResources(QRCPATH, searchKey);
}

QString getYoutubeShareUrl(const QString &broadCastID)
{
	QString url(g_youtubeUrl);
	url += "/watch?v=";
	url += broadCastID;
	return url;
}

const QString getHostMacAddress()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
	QString strMacAddr = "";
	for (int i = 0; i < nets.count(); i++) {
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack)) {
			strMacAddr = nets[i].hardwareAddress();
			break;
		}
	}
	return strMacAddr;
}

bool writeFile(const QByteArray &array, const QString &path)
{
	QFile file(path);
	if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
		return false;
	}

	file.write(array);
	file.close();
	return true;
}

QPixmap paintSvg(QSvgRenderer &renderer, const QSize &pixSize)
{
	QPixmap pixmap(pixSize);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	renderer.render(&painter);
	return pixmap;
}

QPixmap paintSvg(const QString &pixmapPath, const QSize &pixSize)
{
	QSvgRenderer renderer(pixmapPath);
	return paintSvg(renderer, pixSize);
}

void loadPixmap(QPixmap &pix, const QString &pixmapPath, const QSize &pixSize)
{
	if (pixmapPath.isEmpty()) {
		pix = QPixmap();
		return;
	}

	if (pixmapPath.toLower().endsWith(".svg")) {
		QSvgRenderer renderer(pixmapPath);
		pix = paintSvg(renderer, pixSize);
	} else {
		if (!pix.load(pixmapPath)) {
			pix.load(pixmapPath, "PNG");
			PRE_LOG_MSG(QString("error when load image: " + pixmapPath + " , may be the image suffix is not right").toStdString().c_str(), INFO)
		}
	}
}

QPixmap &getCubePix(QPixmap &mBigPix)
{
	int width = mBigPix.width();
	int height = mBigPix.height();
	int cube = qMin(width, height);
	QRect cubeRec((width - cube) / 2, (height - cube) / 2, cube, cube);
	mBigPix = mBigPix.copy(cubeRec);
	return mBigPix;
}

void getComplexImageOfChannel(const QString &uuid, QString &userIcon, QString &platformIcon, const QString &prefix, const QString &surfix)
{
	const auto &srcData = PLSCHANNELS_API->getChanelInfoRef(uuid);

	QString platformName = getInfo(srcData, g_channelName, QString());
	if (srcData.isEmpty()) {
		return;
	}
	int type = getInfo(srcData, g_data_type, NoType);
	if (type == RTMPType) {
		userIcon = getPlatformImageFromName(platformName);
		if (userIcon.isEmpty()) {
			userIcon = g_defualtPlatformIcon;
		}
		platformIcon = g_defualtPlatformSmallIcon;
	} else {
		userIcon = getInfo(srcData, g_userIconCachePath, g_defaultHeaderIcon);
		platformIcon = getPlatformImageFromName(platformName, prefix, surfix);
	}
}

void formatJson(QByteArray &array)
{
	auto document = QJsonDocument::fromJson(array);
	array = document.toJson(QJsonDocument::Indented);
}

void requestStartLog(const QString &url, const QString &requestType)
{
	PLS_INFO("Channels", "%s", QString("http request start:%1, url = " + url + ".").arg(requestType).toStdString().c_str());
}

const QString getChannelCacheFilePath()
{
	return getChannelCacheDir() + QDir::separator() + g_channelCacheFile;
}

const QString getChannelCacheDir()
{
	QString ret = pls_get_user_path(QCoreApplication::applicationName() + QDir::separator() + "Cache");
	QDir dir(ret);
	if (!dir.exists(ret)) {
		dir.mkpath(ret);
	}
	return dir.toNativeSeparators(ret);
}

const QString getChannelSettingsFilePath()
{
	return getChannelCacheDir() + QDir::separator() + g_channelSettingsFile;
}

const QStringList getDefaultPlatforms()
{
	return gDefaultPlatform;
}

const QString guessPlatformFromRTMP(const QString &rtmpUrl)
{
	auto rtmpInfos = PLSCHANNELS_API->getRTMPInfos();
	auto isUrlMatched = [&](const QString &url) { return rtmpUrl.compare(url, Qt::CaseInsensitive) == 0; };
	auto retIte = std::find_if(rtmpInfos.constBegin(), rtmpInfos.constEnd(), isUrlMatched);
	if (retIte != rtmpInfos.constEnd()) {
		return retIte.key();
	}
	return QString(CUSTOM_RTMP);
}

QPropertyAnimation *createShowAnimation(QWidget *wid, int msSec)
{
	auto animation = new QPropertyAnimation(wid, "pos");

	animation->setDuration(msSec);
	animation->setStartValue(QPoint(wid->width(), 0));
	animation->setEndValue(QPoint(0, 0));
	return animation;
}

void displayWidgetWithAnimation(QWidget *wid, int msSec, bool show)
{

	if (show) {
		wid->show();
		auto animation = createShowAnimation(wid, msSec);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		auto animation = createHideAnimation(wid);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
		QWidget::connect(animation, &QPropertyAnimation::finished, wid, &QWidget::hide);
	}
}

QPropertyAnimation *createHideAnimation(QWidget *wid, int msSec)
{
	auto animation = new QPropertyAnimation(wid, "pos");

	animation->setDuration(msSec);
	animation->setStartValue(QPoint(0, 0));
	animation->setEndValue(QPoint(wid->width(), 0));
	return animation;
}

void moveWidgetToParentCenter(QWidget *wid)
{
	auto parentGeo = wid->parentWidget()->contentsRect();
	auto parentCenter = wid->parentWidget()->mapToGlobal(parentGeo.center() - QPoint(wid->frameGeometry().width() / 2, wid->frameGeometry().height() / 2) - parentGeo.topLeft());
	auto realCenter = parentCenter;
	//qDebug() << " geo " << wid->parentWidget()->frameGeometry();
	if (wid->windowFlags() | Qt::Widget) {
		wid->move(wid->mapFromGlobal(realCenter));
	} else {
		wid->move(realCenter);
	}
}

void refreshStyle(QWidget *widget)
{
	assert(widget != nullptr);
	widget->style()->unpolish(widget);
	widget->style()->polish(widget);
}

QString getElidedText(QWidget *widget, const QString &srcTxt, double minWidth, Qt::TextElideMode mode, int flag)
{
	if (srcTxt.isEmpty()) {
		return srcTxt;
	}
	return widget->fontMetrics().elidedText(srcTxt, mode, minWidth, flag);
}

void setStyleSheetFromFile(const QString &fileStr, QWidget *wid)
{
	QFile file(fileStr);
	if (file.open(QIODevice::ReadOnly)) {
		wid->setStyleSheet(file.readAll());
	}
}
