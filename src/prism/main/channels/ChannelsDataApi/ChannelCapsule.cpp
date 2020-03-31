#include "ChannelCapsule.h"
#include <QHelpEvent>
#include <QLabel>
#include <QTimer>
#include <QToolTip>
#include "ChannelCommonFunctions.h"
#include "ChannelConfigPannel.h"
#include "ChannelConst.h"
#include "LogPredefine.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelsVirualAPI.h"
#include "frontend-api.h"
#include "pls-app.hpp"
#include "ui_ChannelCapsule.h"

using namespace ChannelData;
constexpr int MaxNameLabelWidth = 86;
constexpr int MaxChannelNameLabelWidth = 130;
const char CheckedStr[] = "Checked";

ChannelCapsule::ChannelCapsule(QWidget *parent) : QFrame(parent), ui(new Ui::ChannelCapsule)
{
	ui->setupUi(this);
	this->setAttribute(Qt::WA_Hover, true);
	this->installEventFilter(this);

	initializeConfigPannel();

	connect(&mUpdateTimer, &QTimer::timeout, this, &ChannelCapsule::updateStatisticInfo);
	connect(qApp, &QCoreApplication::aboutToQuit, &mUpdateTimer, &QTimer::stop);

	PLSDpiHelper dpiHelper;
	dpiHelper.notifyDpiChanged(this, [this]() { delayUpdateText(); });
	PRE_LOG(new channel capsule, INFO);
}

void ChannelCapsule::normalState(const QVariantMap &info)
{

	int dataType = getInfo(info, g_data_type, NoType);
	switch (dataType) {
	case ChannelType:
		channelTypeState(info);
		break;
	case RTMPType:
		RTMPTypeState(info);
		break;
	default:
		break;
	}
}

void ChannelCapsule::shiftState(const QVariantMap &srcData)
{
	int dataState = getInfo(srcData, g_channelStatus, Error);

	switch (dataState) {
	case Error:
	case UnAuthorized:
		errorState(srcData);
		break;
	case UnInitialized:
		unInitializeState(srcData);
		break;
	case EmptyChannel:
	case WaitingActive:
		waitingState(srcData);
		break;
	case Valid:
	case Expired:
		normalState(srcData);
		break;
	default:
		errorState(srcData);
		break;
	}
}

void ChannelCapsule::RTMPTypeState(const QVariantMap &info)
{
	bool isLiving = PLSCHANNELS_API->isLiving();
	int userState = getInfo(info, g_channelUserStatus, NotExist);

	bool isEnabled = (userState == Enabled);
	bool isAct = (isLiving && isEnabled);

	ui->HeaderPart->setEnabled(isEnabled);
	ui->InfoPart->setEnabled(isEnabled);

	ui->UserIcon->setActive(isAct);
	switchUpdateStatisticInfo(false);
	ui->ChannelInfo->hide();

	ui->TextFrame->setVisible(true);
	ui->StateActive->setVisible(!isLiving);
	if (!isLiving) {
		setOnLine(isEnabled);
	}

	ui->CatogeryFrame->hide();

	ui->ErrorFrame->hide();
	this->setEnabled(true);
}

void ChannelCapsule::channelTypeState(const QVariantMap &info)
{

	bool isLiving = PLSCHANNELS_API->isLiving();
	int userState = getInfo(info, g_channelUserStatus, NotExist);

	bool isEnabled = userState == Enabled;
	bool isAct = (isLiving && isEnabled);

	ui->HeaderPart->setEnabled(isEnabled);
	ui->InfoPart->setEnabled(isEnabled);

	ui->UserIcon->setActive(isAct);
	bool isInfoShow = switchUpdateStatisticInfo(isAct);
	ui->ChannelInfo->setVisible(isLiving && isInfoShow);

	ui->TextFrame->setVisible(true);
	ui->StateActive->setVisible(!isLiving);
	if (!isLiving) {
		setOnLine(isEnabled);
	}

	bool hasText = !getInfo(info, g_catogryTemp).isEmpty() || !getInfo(info, g_catogry).isEmpty();
	bool isCatogeryVisible = !isLiving && hasText;
	ui->CatogeryFrame->setVisible(isCatogeryVisible);

	ui->ErrorFrame->hide();
}

void ChannelCapsule::errorState(const QVariantMap &info)
{
	ui->HeaderPart->setEnabled(false);
	ui->InfoPart->setEnabled(false);

	int dataType = getInfo(info, g_data_type, NoType);
	bool isLiving = PLSCHANNELS_API->isLiving();
	int userState = getInfo(info, g_channelUserStatus, NotExist);

	ui->UserIcon->setActive(false);
	bool isInfoOk = switchUpdateStatisticInfo(false);
	ui->TextFrame->setVisible(true);

	ui->StateActive->setVisible(false);
	if (!isLiving) {
		setOnLine(userState == Enabled);
	}

	if (dataType == RTMPType) {
		ui->CatogeryFrame->setVisible(false);
		ui->ChannelInfo->setVisible(false);
	} else {
		ui->CatogeryFrame->setVisible(!isLiving);
		ui->ChannelInfo->setVisible(isLiving && isInfoOk);
	}
	ui->ErrorFrame->setVisible(false);
}

void ChannelCapsule::unInitializeState(const QVariantMap &info)
{
	waitingState(info);
}

void ChannelCapsule::waitingState(const QVariantMap &info)
{
	ui->HeaderPart->setEnabled(false);
	ui->InfoPart->setEnabled(false);

	ui->UserIcon->setVisible(true);
	ui->UserIcon->setActive(false);

	updateErrorLabel(info);
	ui->ErrorFrame->setVisible(true);
	ui->TextFrame->setVisible(false);

	switchUpdateStatisticInfo(false);
	ui->ChannelInfo->setVisible(false);
	setOnLine(false);
}

void ChannelCapsule::initializeConfigPannel()
{
	mConfigPannel = new ChannelConfigPannel(this);
	mConfigPannel->setAttribute(Qt::WA_Hover, true);
	mConfigPannel->setWindowFlags(Qt::SubWindow | Qt::NoDropShadowWindowHint | Qt::FramelessWindowHint);
	mConfigPannel->installEventFilter(this);
	mConfigPannel->hide();
}

bool ChannelCapsule::isPannelOutOfView()
{
	auto parentWid = this->parentWidget()->parentWidget()->parentWidget()->parentWidget();
	auto parentGeometry = parentWid->contentsRect();
	auto center = this->contentsRect().center();
	center = this->mapTo(parentWid, center);

	auto right = center + QPoint(this->width() / 2 - 20, 0);
	auto left = center - QPoint(this->width() / 2 - 20, 0);
	return (!parentGeometry.contains(center) || !parentGeometry.contains(left) || !parentGeometry.contains(right));
}

ChannelCapsule::~ChannelCapsule()
{
	mConfigPannel->deleteLater();
	delete ui;
}

void ChannelCapsule::setChannelID(const QString &uuid)
{
	mInfoID = uuid;
	mConfigPannel->setChannelID(mInfoID);
}

QString &translateForYoutube(QString &src)
{
	if (src.isEmpty()) {
		return src;
	}
	QStringList trans{QObject::tr("youtube.privacy.public"), QObject::tr("youtube.privacy.unlisted"), QObject::tr("youtube.privacy.private")};
	QStringList indexs{("youtube.privacy.public"), ("youtube.privacy.unlisted"), ("youtube.privacy.private")};
	QRegularExpression rule(".+" + src);
	rule.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	int index = indexs.indexOf(rule);
	if (index != -1) {
		src = trans[index];
	}
	return src;
}

void ChannelCapsule::updateUi()
{
	const auto srcData = PLSCHANNELS_API->getChannelInfo(mInfoID);
	if (srcData.isEmpty()) {
		return;
	}

	mLastMap = srcData;
	updateIcons(mLastMap);
	shiftState(mLastMap);
	updateTextFrames(mLastMap);
	delayUpdateText();
	mConfigPannel->updateUI();
}
void ChannelCapsule::updateIcons(const QVariantMap &srcData)
{
	int state = getInfo(srcData, g_channelStatus, Error);
	bool needSharp = false;

	QString userIcon;
	QString platformIcon;
	if (state == Valid || state == Error || state == Expired || state == UnAuthorized) {
		int type = getInfo(srcData, g_data_type, NoType);
		if (type == ChannelType) {
			needSharp = true;
		}
		getComplexImageOfChannel(mInfoID, userIcon, platformIcon);
	} else {
		QString channelName = getInfo(srcData, g_channelName);
		userIcon = getPlatformImageFromName(channelName);
	}

	ui->UserIcon->setPixmap(userIcon, QSize(34, 34), needSharp);
	ui->UserIcon->setPlatformPixmap(platformIcon, QSize(18, 18));

	PLSDpiHelper::dpiDynamicUpdate(ui->UserIcon);
}
void ChannelCapsule::updateErrorLabel(const QVariantMap &info)
{
	QString errorStr = getInfo(info, g_errorString);
	ui->ErrorLabel->setText(errorStr);
	ui->ErrorLabel->setToolTip(errorStr);
}

void ChannelCapsule::updateTextFrames(const QVariantMap &srcData)
{
	auto TextFrameSize = ui->TextFrame->width() - (ui->StateActive->isVisible() ? ui->StateActive->width() : 0);
	ui->UserName->resize(TextFrameSize, ui->UserName->height());
	ui->CatogeryName->resize(ui->TextFrame->width(), ui->CatogeryName->height());

	QString userName = getInfo(srcData, g_nickName, QString("No Name"));
	userName = getElidedText(ui->UserName, userName, TextFrameSize);
	ui->UserName->setText(userName);

	QString catogry;
	QString catogryTemp = getInfo(srcData, g_catogryTemp, QString());
	if (!catogryTemp.isEmpty()) {
		catogry = catogryTemp;
	} else {
		catogry = getInfo(srcData, g_catogry, QString());
	}

	QString platformName = getInfo(srcData, g_channelName, QString());
	if (platformName.contains(YOUTUBE) && !catogry.isEmpty()) {
		translateForYoutube(catogry);
	}

	int widthElid = ui->CatogeryName->width();
	if (platformName == VLIVE && !catogry.isEmpty()) {
		QString split(3, 0x00B7);
		auto values = catogry.split(split);
		if (values.size() == 2) {
			int leftWidth = widthElid * 0.45;
			auto leftString = getElidedText(ui->CatogeryName, values[0], leftWidth);
			int rightWidth = widthElid * 0.45;
			auto rightString = getElidedText(ui->CatogeryName, values[1], rightWidth);
			catogry = leftString + " " + QChar(0x00b7) + " " + rightString;
		} else {
			catogry = getElidedText(ui->CatogeryName, catogry, widthElid);
		}
	} else {
		catogry = getElidedText(ui->CatogeryName, catogry, widthElid);
	}

	//catogry = QString("platfo... ") + QString::fromStdWString(std::wstring{0x25CF})+QString(" chann...");
	//catogry = QString("platfo... ") + QString::fromStdWString(std::wstring{0x00b7}) + QString(" chann...");
	ui->CatogeryName->setText(catogry);
}

void ChannelCapsule::delayUpdateText()
{
	if (mLastMap.isEmpty()) {
		return;
	}
	QTimer::singleShot(100, this, [=]() { updateTextFrames(mLastMap); });
}

QString &formatNumber(QString &number, bool isEng = true)
{
	number = number.simplified();
	if (number.isEmpty()) {
		number = "0";
		return number;
	}
	int N = number.count();
	if (N <= 3) {
		return number;
	}
	if (N <= 6) {
		return number.insert(N - 3, ",");
	}

	QString head;
	QString suffix;

	switch (N) {
	case 7: {
		if (isEng) {
			head = number.chopped(5);
			head.insert(N - 6, '.');
			suffix = CHANNELS_TR(Mill);
		} else {
			head = number.chopped(4);
			suffix = CHANNELS_TR(Wan);
		}
	} break;
	case 8: {
		if (isEng) {
			head = number.chopped(5);
			head.insert(N - 6, '.');
			suffix = CHANNELS_TR(Mill);
		} else {
			head = number.chopped(4);
			head.insert(1, ',');
			suffix = CHANNELS_TR(Wan);
		}

	} break;
	case 9: {
		if (isEng) {
			head = number.chopped(5);
			head.insert(N - 6, '.');
			suffix = CHANNELS_TR(Mill);
		} else {
			head = number.chopped(7);
			head.insert(1, '.');
			suffix = CHANNELS_TR(Yi);
		}

	} break;
	case 10: {
		if (isEng) {
			head = number.chopped(8);
			head.insert(N - 9, '.');
			suffix = CHANNELS_TR(Bill);
		} else {
			head = number.chopped(8);
			suffix = CHANNELS_TR(Yi);
		}

	} break;

	default: {
		if (isEng) {
			head = number.chopped(8);
			head.insert(N - 9, '.');
			suffix = CHANNELS_TR(Bill);
		} else {
			head = number.chopped(8);
			suffix = CHANNELS_TR(Yi);
		}

	} break;
	}
	if (head.endsWith(".0")) {
		head.remove(".0");
	}
	number = head + suffix;
	return number;
}

bool ChannelCapsule::updateStatisticInfo()
{
	bool isEng = false;
	auto lang = QString(App()->GetLocale());
	if (lang.contains("en", Qt::CaseInsensitive)) {
		isEng = true;
	}
	auto info = PLSCHANNELS_API->getChannelInfo(mInfoID);
	QString viewers;

	if (info.contains(g_viewers)) {
		viewers = getInfo(info, g_viewers, QString("0"));
		formatNumber(viewers, isEng);
		ui->ViewerNumberLabel->setText(viewers);
		ui->ViewerPicLabel->setStyleSheet(QString("image: url(%1);").arg(getInfo(info, g_viewersPix, g_defaultViewerIcon)));
		ui->ViewerFrame->setVisible(true);
	} else {
		ui->ViewerFrame->setVisible(false);
	}
	QString likes;
	if (info.contains(g_likes)) {
		likes = getInfo(info, g_likes, QString("0"));
		formatNumber(likes, isEng);
		ui->LikeNumberLabel->setText(likes);
		ui->LikePicLabel->setStyleSheet(QString("image: url(%1);").arg(getInfo(info, g_likesPix, g_defaultLikeIcon)));
		ui->LikeInfo->setVisible(true);
	} else {
		ui->LikeInfo->setVisible(false);
	}

	return !(viewers.isEmpty() && likes.isEmpty());
}
void ChannelCapsule::changeEvent(QEvent *e)
{
	QFrame::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

bool ChannelCapsule::eventFilter(QObject *watched, QEvent *event)
{
	switch (event->type()) {
	case QEvent::HoverMove: {
		//PRE_LOG("hover enter");
		if (!PLSCHANNELS_API->isEmptyToAcquire() || watched == mConfigPannel || isPannelOutOfView() || mConfigPannel->isVisible() || PLSCHANNELS_API->isShifting()) {
			return false;
		}
		showConfigPannel();
		return true;

	} break;
	case QEvent::HoverLeave: {
		if (mConfigPannel == watched) {
			hideConfigPannel();
			return true;
		}
		auto hLeav = dynamic_cast<QHoverEvent *>(event);
		if (hLeav) {
			auto pos = hLeav->pos();
			auto contentRec = this->rect();
			if (!contentRec.contains(pos)) {
				hideConfigPannel();
				return true;
			}
		}
	} break;
	case QEvent::Wheel: {
		if (mConfigPannel == watched) {
			hideConfigPannel();
		}
	} break;
	} //end switch

	return false;
}

void ChannelCapsule::showConfigPannel()
{
	mConfigPannel->show();
}

void ChannelCapsule::hideConfigPannel()
{
	mConfigPannel->hide();
}

bool ChannelCapsule::switchUpdateStatisticInfo(bool on)
{
	bool ret = false;
	if (on) {
		if (updateStatisticInfo()) {
			mUpdateTimer.start(5000);
			ret = true;
		}
	} else {
		mUpdateTimer.stop();
		ret = updateStatisticInfo();
	}
	return ret;
}

bool ChannelCapsule::isOnLine()
{
	return ui->StateActive->property(CheckedStr).toBool();
}

void ChannelCapsule::setOnLine(bool isActive)
{
	ui->StateActive->setProperty(CheckedStr, isActive);
	ui->StateActive->setDisabled(true);
	refreshStyle(ui->StateActive);
}
bool ChannelCapsule::isSelectedDisplay()
{
	return PLSCHANNELS_API->isChannelSelectedDisplay(mInfoID);
}
