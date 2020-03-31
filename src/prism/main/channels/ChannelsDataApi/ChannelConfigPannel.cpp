#include "ChannelConfigPannel.h"
#include <QMenu>
#include <QUrl>
#include <QWidgetAction>
#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "LogPredefine.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelsVirualAPI.h"
#include "PLSDpiHelper.h"
#include "alert-view.hpp"
#include "ui_ChannelConfigPannel.h"

using namespace ChannelData;

ChannelConfigPannel::ChannelConfigPannel(QWidget *parent) : QFrame(parent), ui(new Ui::ChannelConfigPannel)
{
	ui->setupUi(this);
	auto btns = this->findChildren<QPushButton *>();
	std::for_each(btns.begin(), btns.end(), [](QPushButton *btn) {
		if (!btn->toolTip().isEmpty()) {
			btn->setAttribute(Qt::WA_AlwaysShowToolTips);
		}
	});
}

ChannelConfigPannel::~ChannelConfigPannel()
{
	delete ui;
}

void ChannelConfigPannel::setChannelID(const QString &channelID)
{
	mChannelID = channelID;
	updateUI();
}

void ChannelConfigPannel::changeEvent(QEvent *e)
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

bool ChannelConfigPannel::eventFilter(QObject *watched, QEvent *event)
{
	switch (event->type()) {
	case QEvent::HoverEnter:

		break;
	case QEvent::HoverLeave:

		break;
	default:
		break;
	}
	return false;
}

void ChannelConfigPannel::on_showInfoBtn_clicked()
{
	PRE_LOG_UI(showInfoBtn, ChannelConfigPannel);
	this->hide();
	showChannelInfo(mChannelID);
}

void ChannelConfigPannel::on_ShareBtn_clicked()
{
	this->hide();
	PRE_LOG_UI(ShareBtn, ChannelConfigPannel);
	showShareView(mChannelID);
}

void ChannelConfigPannel::on_ConfigBtn_clicked()
{
	QSignalBlocker blocker(this);
	auto removeChannel = [&]() {
		int myType = PLSCHANNELS_API->getValueOfChannel(mChannelID, g_data_type, NoType);
		QString typeStr;
		if (myType == ChannelType) {
			typeStr = PLSCHANNELS_API->getValueOfChannel(mChannelID, g_channelName, QString("RTMP"));
		} else {
			typeStr = "RTMP";
		}
		auto ret = PLSAlertView::question(nullptr, CHANNELS_TR(Confirm), CHANNELS_TR(DisconnectWarning).arg(typeStr),
						  {{PLSAlertView::Button::Yes, CHANNELS_TR(Yes)}, {PLSAlertView::Button::Cancel, CHANNELS_TR(Cancel)}}, PLSAlertView::Button::Cancel);
		if (ret != PLSAlertView::Button::Yes) {
			return;
		}
		PRE_LOG_UI(RemoveChannel, ChannelConfigPannel);
		PLSCHANNELS_API->sigTryRemoveChannel(mChannelID, true, true);
	};

	auto gotoYoutube = []() { QDesktopServices::openUrl(QUrl(g_yoububeLivePage)); };

	QMenu menu(this);
	menu.addAction(CHANNELS_TR(Disconnect), this, removeChannel);
	int state = PLSCHANNELS_API->getChannelStatus(mChannelID);
	if (state == UnInitialized) {
		menu.addAction(CHANNELS_TR(GoTo), this, gotoYoutube);
	}

	menu.exec(QCursor::pos());
	this->hide();
}

void ChannelConfigPannel::doChildrenExclusive(bool &retflag)
{
	retflag = true;
	auto myPlatform = PLSCHANNELS_API->getValueOfChannel(mChannelID, g_channelName, QString());
	auto childSelected = PLSCHANNELS_API->getCurrentSelectedPlatformChannels(myPlatform, ChannelType);
	if (childSelected.count() > 0) {
		auto ret = PLSAlertView::question(nullptr, CHANNELS_TR(Confirm), CHANNELS_TR(ChildDisableWaring).arg(myPlatform),
						  {{PLSAlertView::Button::Yes, CHANNELS_TR(Yes)}, {PLSAlertView::Button::Cancel, CHANNELS_TR(Cancel)}}, PLSAlertView::Button::Cancel);
		//do not want to disable other child
		if (ret != PLSAlertView::Button::Yes) {
			QSignalBlocker blocker(ui->EnableSwitch);
			ui->EnableSwitch->setChecked(false);
			PLSCHANNELS_API->setChannelUserStatus(mChannelID, Disabled);
			return;
		}

		childExclusive(mChannelID);
		PLSCHANNELS_API->setChannelUserStatus(mChannelID, Enabled);
		return;
	}
	retflag = false;
}

void ChannelConfigPannel::checkExclusiveChannel(bool &retflag)
{
	retflag = true;
	auto exclusiveID = findExistEnabledExclusiveChannel();
	//checked now exists
	if (!exclusiveID.isEmpty()) {

		QString typeStr = PLSCHANNELS_API->getValueOfChannel(exclusiveID, g_channelName, QString(""));
		int myType = PLSCHANNELS_API->getValueOfChannel(exclusiveID, g_data_type, NoType);
		if (myType == RTMPType && !typeStr.contains("RTMP", Qt::CaseInsensitive)) {
			typeStr = PLSCHANNELS_API->getValueOfChannel(exclusiveID, g_channelName, QString("")) + " RTMP";
		}

		auto ret = PLSAlertView::question(nullptr, CHANNELS_TR(Confirm), CHANNELS_TR(NowDisableWarning).arg(typeStr),
						  {{PLSAlertView::Button::Yes, CHANNELS_TR(Yes)}, {PLSAlertView::Button::Cancel, CHANNELS_TR(Cancel)}}, PLSAlertView::Button::Cancel);
		//do not want to disable now
		if (ret != PLSAlertView::Button::Yes) {
			QSignalBlocker blocker(ui->EnableSwitch);
			ui->EnableSwitch->setChecked(false);
			PLSCHANNELS_API->setChannelUserStatus(mChannelID, Disabled);
			return;
		}
		//disable now
		PLSCHANNELS_API->setChannelUserStatus(exclusiveID, Disabled);
		PLSCHANNELS_API->setChannelUserStatus(mChannelID, Enabled);
		return;
	}
	retflag = false;
}

void ChannelConfigPannel::on_EnableSwitch_toggled(bool checked)
{
	PRE_LOG_UI(SwitchChannel, ChannelConfigPannel);
	int myType = PLSCHANNELS_API->getValueOfChannel(mChannelID, g_data_type, NoType);
	bool isLiving = PLSCHANNELS_API->isLiving();
	//to uncheck and isliving
	if (!checked && isLiving) {
		QString typeStr = PLSCHANNELS_API->getValueOfChannel(mChannelID, g_channelName, QString(""));

		auto ret = PLSAlertView::question(nullptr, CHANNELS_TR(Confirm), CHANNELS_TR(DisableWarnig).arg(typeStr),
						  {{PLSAlertView::Button::Yes, CHANNELS_TR(Yes)}, {PLSAlertView::Button::Cancel, CHANNELS_TR(Cancel)}}, PLSAlertView::Button::Cancel);
		//reject disable
		if (ret != PLSAlertView::Button::Yes) {
			QSignalBlocker blocker(ui->EnableSwitch);
			ui->EnableSwitch->setChecked(true);
			PLSCHANNELS_API->setChannelUserStatus(mChannelID, Enabled);
			return;
		}
		//agree disable
		PLSCHANNELS_API->setChannelUserStatus(mChannelID, Disabled);
		return;
	}

	//to uncheck and not living
	if (!checked && !isLiving) {
		PLSCHANNELS_API->setChannelUserStatus(mChannelID, Disabled);
		return;
	}
	/************************to  checked******************************/
	bool hasExclusive;
	checkExclusiveChannel(hasExclusive);
	if (hasExclusive) {
		return;
	}
	//to checked ,is now channel
	bool isExclusive = isExclusiveChannel(mChannelID);
	if (isExclusive) {
		//to checked not now
		disableAll();
		PLSCHANNELS_API->setChannelUserStatus(mChannelID, Enabled);
		return;
	}

	/* not exclusive channel */
	if (myType == ChannelType) {
		bool retflag;
		//close other if is multi children platform
		doChildrenExclusive(retflag);
		if (retflag) {
			return;
		}
	}

	// checked now is not exist and to checked and max limitted
	if (PLSCHANNELS_API->currentSelectedCount() >= 6) {
		PLSAlertView::warning(nullptr, CHANNELS_TR(warning.title), CHANNELS_TR(info.selectedLimited));
		QSignalBlocker blocker(ui->EnableSwitch);
		ui->EnableSwitch->setChecked(false);
		PLSCHANNELS_API->setChannelUserStatus(mChannelID, Disabled);
		return;
	}

	// checked now is not exist and to checked and in limitted
	PLSCHANNELS_API->setChannelUserStatus(mChannelID, Enabled);
	/*****************to checked *****************************************/
	return;
}

void ChannelConfigPannel::shiftState(const QVariantMap &info)
{
	QSignalBlocker blocker(ui->EnableSwitch);
	int userState = getInfo(info, g_channelUserStatus, NotExist);
	int dataState = getInfo(info, g_channelStatus, Error);
	int dataType = getInfo(info, g_data_type, NoType);
	int currentSelected = 0;
	bool isLiving = PLSCHANNELS_API->isLiving();
	if (isLiving) {
		currentSelected = PLSCHANNELS_API->currentSelectedValidCount();
	} else {
		currentSelected = PLSCHANNELS_API->currentSelectedCount();
	}

	ui->EnableSwitch->setChecked(userState == Enabled);
	ui->EnableSwitch->setToolTip((userState == Enabled ? CHANNELS_TR(EnableTip) : CHANNELS_TR(DisableTip)));

	auto checkIsEnabled = [&]() {
		if (isLiving && currentSelected <= 1) {
			return false;
		}

		if (isLiving && userState != Enabled) {
			return false;
		}

		if (dataState == Expired && userState == Enabled) {
			return true;
		}

		if (dataState == UnAuthorized && userState == Enabled) {
			return true;
		}

		if (dataState != Valid) {
			return false;
		}

		return true;
	};

	bool isEnabeld = checkIsEnabled();

	ui->EnableSwitch->setEnabled(isEnabeld);

	ui->showInfoBtn->setProperty(g_data_type.toStdString().c_str(), dataType);
	refreshStyle(ui->showInfoBtn);
	switch (dataType) {
	case RTMPType: {
		ui->showInfoBtn->setToolTip(CHANNELS_TR(EditRTMPTips));
		ui->showInfoBtn->setEnabled(!isLiving);
		ui->ShareBtn->setEnabled(false);
	} break;
	case ChannelType: {
		ui->showInfoBtn->setToolTip(CHANNELS_TR(InfoTip));
		ui->showInfoBtn->setEnabled(dataState == Valid);
		bool isNeedShare = (info.contains(g_shareUrl) || info.contains(g_shareUrlTemp));
		if (isNeedShare) {
			ui->ShareBtn->setEnabled(dataState == Valid || dataState == Expired);
		} else {
			ui->ShareBtn->setEnabled(false);
		}
	} break;
	default:
		break;
	}

	ui->ConfigBtn->setVisible(!isLiving);
	if (isLiving) {
		ui->horizontalLayout->removeItem(ui->configSpacer);
		ui->horizontalLayout->removeWidget(ui->ConfigBtn);
	} else if (ui->horizontalLayout->indexOf(ui->configSpacer) < 0) {
		ui->horizontalLayout->addItem(ui->configSpacer);
		ui->horizontalLayout->addWidget(ui->ConfigBtn);
	}
}

void ChannelConfigPannel::updateUI()
{
	auto &info = PLSCHANNELS_API->getChanelInfoRef(mChannelID);
	if (info.isEmpty()) {
		return;
	}
	shiftState(info);
	this->setEnabled(!PLSCHANNELS_API->isRehearsaling());

	return;
}
