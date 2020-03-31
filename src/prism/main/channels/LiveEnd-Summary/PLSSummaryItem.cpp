#include "PLSSummaryItem.h"
#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "QPainter"
#include "ui_PLSSummaryItem.h"

int const summaryYoutubeDetailNameWidth = 255;
int const summaryTwitchDetailNameWidth = 270;

PLSSummaryItem::PLSSummaryItem(const QMap<QString, QVariant> &source, QWidget *parent) : QFrame(parent), ui(new Ui::PLSSummaryItem), mSourceData(source)
{
	ui->setupUi(this);
	setupData();
}

PLSSummaryItem::~PLSSummaryItem()
{
	delete ui;
}

void PLSSummaryItem::setupData()
{
	QString str("<span style = \" font-size:12px; font-weight: bold; color:#ffffff;\">%1</span>\
		<span style = \" font-size:12px; font-weight: bold; color:#666666;\">·</span>\
		<span style = \" font-size:12px; color:#ffffff;\">%2</span>");

	QString platformName = mSourceData.value(ChannelData::g_channelName, "OOOO").toString();
	QString userNname = mSourceData.value(ChannelData::g_nickName, "OOOO").toString();
	ui->nameLabel->setText(str.arg(platformName, userNname));

	setupDetailLabelData(platformName);
	combineTwoImage();
}

void PLSSummaryItem::setupDetailLabelData(QString platform)
{
	ui->bottomLabel->setHidden(false);

	/*auto dataType =getInfo(mSourceData, ChannelData::g_data_type, ChannelData::ChannelType);*/
	auto dataType = getInfo(mSourceData, ChannelData::g_data_type, 0);

	if (platform == TWITCH && dataType == ChannelData::ChannelType) {
		QString strLeft("<span style = \" font-size:12px; color:#999999;\">%1</span>");
		QString testName = "20181017 라이브 (12:04 7:00 PM)";
		QFontMetrics fontWidth(ui->bottomLabel->font());
		QString elidedText(fontWidth.elidedText(testName, Qt::ElideRight, summaryTwitchDetailNameWidth));

		QString rightStr("%1\
		<span style = \" font-size:12px; color:#666666;\">·</span>\
		<span style = \" font-size:12px; color:#999999;\">%2</span>");

		QString serverName = "This is serverName";

		QString htmlStr = rightStr.arg(strLeft.arg(elidedText), serverName);
		ui->bottomLabel->setText(htmlStr);

	} else if (platform == YOUTUBE && dataType == ChannelData::ChannelType) {
		QString strLeft("<span style = \" font-size:12px; color:#999999;\">%1</span>");
		QString testName = "20181017 라이브 (12:04 7:00 PM)";
		QFontMetrics fontWidth(ui->bottomLabel->font());
		QString elidedText(fontWidth.elidedText(testName, Qt::ElideRight, summaryYoutubeDetailNameWidth));

		QString rightStr("%1\
		<span style = \" font-size:12px; color:#666666;\">·</span>\
		<span style = \" font-size:12px; color:#999999;\">%2</span>\
		<span style = \" font-size:12px; color:#666666;\">·</span>\
		<span style = \" font-size:12px; color:#999999;\">%3</span>");

		QString category = "this category";
		QString privacyStatus = "this privacyStatus";

		QString htmlStr = rightStr.arg(strLeft.arg(elidedText), category, privacyStatus);
		ui->bottomLabel->setText(htmlStr);
	} else {
		ui->bottomLabel->setHidden(true);
	}
}

void PLSSummaryItem::combineTwoImage()
{
	QString platformName = mSourceData.value(ChannelData::g_channelName, QString()).toString();
	QImage img2(getPlatformImageFromName(platformName));

	QRect imgRect1(0, 0, 34, 34);
	QRect imgRect2(imgRect1.width() - 11, imgRect1.height() - 17, 17, 17);

	QPixmap newImage(40, 34);
	QPainter p(&newImage);
	p.fillRect(newImage.rect(), QBrush("#363636"));

	QString ChanelImageStr = mSourceData.value(ChannelData::g_userIconCachePath, ChannelData::g_defaultHeaderIcon).toString();
	QPixmap img1(ChanelImageStr);
	QPixmap fitpixmap_userIcon = img1.scaled(34, 34, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	fitpixmap_userIcon = pixmapToRound(fitpixmap_userIcon, 17);
	p.drawPixmap(imgRect1, fitpixmap_userIcon);

	p.drawImage(imgRect2, img2);

	QPixmap image = newImage.scaled(newImage.width(), newImage.height(), Qt::KeepAspectRatio);
	ui->channelIcon->clear();
	ui->channelIcon->setPixmap(image);
}

QPixmap PLSSummaryItem::pixmapToRound(QPixmap &src, int radius)
{
	if (src.isNull()) {
		return QPixmap();
	}

	QSize size(2 * radius, 2 * radius);
	QBitmap mask(size);
	QPainter painter(&mask);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
	painter.setBrush(QColor(0, 0, 0));
	painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);

	QPixmap image = src.scaled(size);
	image.setMask(mask);
	return image;
}
