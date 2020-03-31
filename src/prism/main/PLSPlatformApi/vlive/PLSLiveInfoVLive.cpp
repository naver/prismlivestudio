#include "PLSLiveInfoVLive.h"
#include <frontend-api.h>
#include <QDebug>
#include <QDir>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QWidgetAction>
#include <Vector>
#include "../PLSLiveInfoDialogs.h"
#include "../PLSPlatformApi.h"
#include "ChannelCommonFunctions.h"

#include "PLSAPIVLive.h"
#include "PLSChannelDataAPI.h"
#include "alert-view.hpp"
#include "log/log.h"
#include "ui_PLSLiveInfoVLive.h"

static const char *liveInfoMoudule = "PLSLiveInfoVLive";

PLSLiveInfoVLive::PLSLiveInfoVLive(PLSPlatformBase *pPlatformBase, QWidget *parent, PLSDpiHelper dpiHelper) : PLSLiveInfoBase(pPlatformBase, parent, dpiHelper), ui(new Ui::PLSLiveInfoVLive)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSLiveInfoVLive, PLSCssIndex::PLSFanshipWidget});
	dpiHelper.setFixedSize(this, {720, 550});
	dpiHelper.notifyDpiChanged(this, [this](double, double, bool firstShow) {
		if (firstShow) {
			QMetaObject::invokeMethod(
				this, [this] { pls_flush_style(ui->lineEditTitle); }, Qt::QueuedConnection);
		}
	});

	PLS_INFO(liveInfoMoudule, "VLive liveinfo Will show");
	ui->setupUi(this->content());

	setHasCloseButton(false);
	setHasBorder(true);
	this->setWindowTitle(tr("LiveInfo.Dialog.Title"));

	ui->scheCombox->setFocusPolicy(Qt::NoFocus);
	content()->setFocusPolicy(Qt::StrongFocus);

	PLS_PLATFORM_VLIVE->liveInfoisShowing();
	m_enteredID = PLS_PLATFORM_VLIVE->getSelectData()._id;

	setupFirstUI();

	auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	pPlatformVLive->setAlertParent(this);

	updateStepTitle(ui->okButton);

	if (!PLS_PLATFORM_API->isPrepareLive()) {
		ui->bottomButtonWidget->layout()->addWidget(ui->cancelButton);
		ui->rehearsalButton->setHidden(true);
	}

	updateUIEnabel();
}

PLSLiveInfoVLive::~PLSLiveInfoVLive()
{
	delete ui;
}
void PLSLiveInfoVLive::updateUIEnabel()
{
	doUpdateOkState();
	if (PLS_PLATFORM_API->isLiving()) {
		ui->thumbnailButton->setEnabled(false);
		ui->scheCombox->setButtonEnable(false);
		ui->lineEditTitle->setEnabled(false);
		ui->fanshipWidget->setEnabled(false);
	}
}
void PLSLiveInfoVLive::setupFirstUI()
{
	/*auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	if (nullptr == pPlatformVLive) {
		return; 
	}*/
	auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	const auto &shipDatas = pPlatformVLive->getTempSelectData().fanshipDatas;
	ui->fanshipWidget->setupDatas(shipDatas);

	ui->lineEditTitle->setText(pPlatformVLive->getTempSelectData().title);
	ui->lineEditTitle->setPlaceholderText(pPlatformVLive->getDefaultTitle());

	refreshUI();
	ui->lineEditTitle->setCursorPosition(0);
	connect(ui->thumbnailButton, &PLSSelectImageButton::takeButtonClicked, this, []() { PLS_UI_STEP(liveInfoMoudule, "VLIVE LiveInfo's Thumbnail Take Button", ACTION_CLICK); });
	connect(ui->thumbnailButton, &PLSSelectImageButton::selectButtonClicked, this, []() { PLS_UI_STEP(liveInfoMoudule, "VLIVE LiveInfo's Thumbnail Select Button", ACTION_CLICK); });
	connect(ui->thumbnailButton, &PLSSelectImageButton::imageSelected, this, &PLSLiveInfoVLive::onImageSelected);

	connect(ui->scheCombox, &PLSScheduleCombox::pressed, this, &PLSLiveInfoVLive::scheduleButtonClicked);
	connect(ui->scheCombox, &PLSScheduleCombox::menuItemClicked, this, &PLSLiveInfoVLive::scheduleItemClick);
	connect(ui->scheCombox, &PLSScheduleCombox::menuItemExpired, this, &PLSLiveInfoVLive::scheduleItemExpired);

	connect(ui->fanshipWidget, &PLSFanshipWidget::shipBoxClick, this, &PLSLiveInfoVLive::fanshipButtonClicked);

	connect(ui->lineEditTitle, &QLineEdit::textChanged, this, &PLSLiveInfoVLive::titleEdited);

	connect(ui->rehearsalButton, &QPushButton::clicked, this, &PLSLiveInfoVLive::rehearsalButtonClicked);
	connect(ui->okButton, &QPushButton::clicked, this, &PLSLiveInfoVLive::okButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &PLSLiveInfoVLive::cancelButtonClicked);
	connect(
		pPlatformVLive, &PLSPlatformVLive::closeDialogByExpired, this, [=]() { reject(); }, Qt::DirectConnection);

	connect(
		pPlatformVLive, &PLSPlatformVLive::toShowLoading, this,
		[=](bool isShow) {
			if (isShow) {
				showLoading(this->content());
			} else {

				hideLoading();
			}
		},
		Qt::DirectConnection);
}

void PLSLiveInfoVLive::showEvent(QShowEvent *event)
{
	Q_UNUSED(event)
	showLoading(content());

	__super::showEvent(event);
	refreshFanshipView();

	auto _onNext = [=](bool value) {
		hideLoading();

		if (!value && !PLS_PLATFORM_API->isPrepareLive()) {
			reject();
		}
	};

	_onNext(true);
}

void PLSLiveInfoVLive::refreshUI()
{
	refreshTitleEdit();
	refreshSchedulePopButton();
	refreshFanshipView();
	refreshThumbnailButton();
	doUpdateOkState();

	const auto &data = PLS_PLATFORM_VLIVE->getTempSelectData();
	if (!data.thumRemoteUrl.isEmpty() && data.thumLocalPath.isEmpty()) {
		PLS_PLATFORM_VLIVE->downloadThumImage([=]() { refreshThumbnailButton(); }, ui->thumbnailButton);
	}
}

void PLSLiveInfoVLive::refreshTitleEdit()
{
	auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	auto data = pPlatformVLive->getTempSelectData();

	ui->lineEditTitle->setText(data.title);
	ui->lineEditTitle->setEnabled(data.isNormalLive);
}
void PLSLiveInfoVLive::refreshSchedulePopButton()
{

	auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	auto data = pPlatformVLive->getTempSelectData();

	PLSScheComboxItemData scheData = PLSScheComboxItemData();
	scheData.title = data.isNormalLive ? tr("LiveInfo.Schedule.PopUp.New") : data.title;
	scheData._id = data._id;
	scheData.type = data.isNormalLive ? PLSScheComboxItemType::Ty_NormalLive : PLSScheComboxItemType::Ty_Schedule;
	scheData.time = data.startTimeShort;
	scheData.isVliveUpcoming = data.isUpcoming;
	scheData.timeStamp = data.startTimeStamp;
	scheData.endTimeStamp = data.endTimeStamp;
	scheData.needShowTimeLeftTimer = PLS_PLATFORM_API->isLiving() ? false : true;
	ui->scheCombox->setupButton(scheData);
}

void PLSLiveInfoVLive::refreshThumbnailButton()
{
	ui->thumDetailLabel->setHidden(PLS_PLATFORM_VLIVE->getTempSelectData().isNormalLive);
	auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	auto data = pPlatformVLive->getTempSelectData();
	ui->thumbnailButton->setImagePath(data.thumLocalPath);
	if (!data.thumRemoteUrl.isEmpty() && data.thumLocalPath.isEmpty()) {
		auto info = PLSCHANNELS_API->getChannelInfo(PLS_PLATFORM_VLIVE->getChannelUUID());
		QString localFile = PLSAPIVLive::getLocalImageFile(data.thumRemoteUrl, info.value(ChannelData::g_channelName).toString());
		ui->thumbnailButton->setImagePath(localFile);
		pPlatformVLive->setThumLocalFile(localFile);
	}
	ui->thumbnailButton->setEnabled(data.isNormalLive);
}

void PLSLiveInfoVLive::updateScheduleComboxItems()
{
	if (ui->scheCombox == nullptr || ui->scheCombox->isMenuNULL() || ui->scheCombox->getMenuHide()) {
		return;
	}

	m_vecItemDatas.clear();
	auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	for (auto data : pPlatformVLive->getScheduleDatas()) {
		PLSScheComboxItemData scheData = PLSScheComboxItemData();
		scheData.title = data.title;
		scheData._id = data._id;
		scheData.type = PLSScheComboxItemType::Ty_Schedule;
		scheData.time = data.startTimeUTC;
		scheData.isVliveUpcoming = data.isUpcoming;
		scheData.timeStamp = data.startTimeStamp;
		scheData.endTimeStamp = data.endTimeStamp;
		scheData.needShowTimeLeftTimer = true;

		if (data._id != pPlatformVLive->getTempSelectID()) {
			m_vecItemDatas.push_back(scheData);
		}
	}

	if (pPlatformVLive->getIsTempSchedule()) {
		PLSScheComboxItemData nomarlData = PLSScheComboxItemData();
		nomarlData._id = "";
		nomarlData.title = tr("LiveInfo.Schedule.PopUp.New");
		nomarlData.time = tr("LiveInfo.Schedule.PopUp.New");
		nomarlData.type = PLSScheComboxItemType::Ty_NormalLive;
		m_vecItemDatas.insert(m_vecItemDatas.begin(), nomarlData);
	}

	{
		//add a placehoder item, if item is 0, then will show this.
		PLSScheComboxItemData nomarlData = PLSScheComboxItemData();
		nomarlData._id = "";
		nomarlData.title = tr("LiveInfo.Schedule.PopUp.New");
		nomarlData.time = tr("LiveInfo.Youtube.no.scheduled");
		nomarlData.type = PLSScheComboxItemType::Ty_Placehoder;
		m_vecItemDatas.insert(m_vecItemDatas.begin(), nomarlData);
	}

	ui->scheCombox->showScheduleMenu(m_vecItemDatas);
}

void PLSLiveInfoVLive::refreshFanshipView()
{
	auto pPlatformVLive = PLS_PLATFORM_VLIVE;
	auto &data = pPlatformVLive->getTempSelectData();
	auto shipDatas = data.fanshipDatas;
	ui->fanshipWidget->setupDatas(shipDatas, !data.isNormalLive);
}

void PLSLiveInfoVLive::onImageSelected(const QString &imageFilePath)
{
	Q_UNUSED(imageFilePath)
	doUpdateOkState();
}

void PLSLiveInfoVLive::saveTempNormalDataWhenSwitch()
{
	PLSVLiveLiveinfoData &tempData = PLS_PLATFORM_VLIVE->getTempNormalData();
	tempData.title = ui->lineEditTitle->text();
	tempData.thumLocalPath = ui->thumbnailButton->getImagePath();
	vector<bool> boxChecks;
	ui->fanshipWidget->getChecked(boxChecks);
	for (size_t i = 0; i < tempData.fanshipDatas.size(); i++) {
		tempData.fanshipDatas[i].isChecked = boxChecks[i];
	}
}

void PLSLiveInfoVLive::okButtonClicked()
{
	PLS_PLATFORM_VLIVE->setIsRehearsal(false);
	PLS_UI_STEP(liveInfoMoudule, "VLIVE liveinfo OK Button Click", ACTION_CLICK);
	saveDateWhenClickButton();
}

void PLSLiveInfoVLive::cancelButtonClicked()
{
	PLS_UI_STEP(liveInfoMoudule, "VLIVE liveinfo Cancel Button Click", ACTION_CLICK);
	reject();
}

void PLSLiveInfoVLive::rehearsalButtonClicked()
{

	PLS_UI_STEP(liveInfoMoudule, "VLIVE liveinfo Rehearsal Button Click", ACTION_CLICK);
	PLS_PLATFORM_VLIVE->setIsRehearsal(true);
	saveDateWhenClickButton();
}

void PLSLiveInfoVLive::fanshipButtonClicked(int index)
{
	string log = "VLIVE liveinfo fanship Button Click with index:" + index;
	PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	doUpdateOkState();
}

void PLSLiveInfoVLive::saveDateWhenClickButton()
{
	auto pPlatformVLive = PLS_PLATFORM_VLIVE;

	auto _onNext = [=](bool isSucceed) {
		hideLoading();
		PLS_INFO(liveInfoMoudule, "vlive liveinfo Save %s", (isSucceed ? "succeed" : "failed"));
		if (isSucceed) {
			accept();
		}
	};

	showLoading(content());

	PLSVLiveLiveinfoData uiData = pPlatformVLive->getTempSelectData();

	vector<bool> boxChecks;
	ui->fanshipWidget->getChecked(boxChecks);

	PLS_PLATFORM_VLIVE->isModifiedWithNewData(ui->lineEditTitle->text(), boxChecks, ui->thumbnailButton->getImagePath(), &uiData);
	if (ui->lineEditTitle->text().trimmed().isEmpty()) {
		uiData.title = ui->lineEditTitle->placeholderText();
	}
	pPlatformVLive->saveSettings(_onNext, uiData);
}

void PLSLiveInfoVLive::scheduleButtonClicked()
{
	PLS_UI_STEP(liveInfoMoudule, "VLive liveinfo schedule pop button click", ACTION_CLICK);
	if (!ui->scheCombox->getMenuHide()) {
		return;
	}
	auto pPlatformVLive = PLS_PLATFORM_VLIVE;

	m_vecItemDatas.clear();
	for (int i = 0; i < 1; i++) {
		PLSScheComboxItemData data = PLSScheComboxItemData();
		data.title = "";
		data.time = tr("LiveInfo.Youtube.loading.scheduled");
		data._id = "";
		data.type = PLSScheComboxItemType::Ty_Loading;
		m_vecItemDatas.push_back(data);
	}

	auto _onNext = [=](bool value) {
		Q_UNUSED(value)
		updateScheduleComboxItems();
	};

	if (nullptr != pPlatformVLive) {
		pPlatformVLive->requestSchedule(_onNext, ui->scheCombox);
	}

	ui->scheCombox->showScheduleMenu(m_vecItemDatas);
}

void PLSLiveInfoVLive::scheduleItemClick(const QString selectID)
{

	for (auto data : m_vecItemDatas) {
		PLSScheComboxItemType type = data.type;
		if (type == PLSScheComboxItemType::Ty_Loading || type == PLSScheComboxItemType::Ty_Placehoder) {
			continue;
		}

		auto pPlatformVLive = PLS_PLATFORM_VLIVE;
		if (pPlatformVLive->getTempSelectID() == selectID) {
			//if select same id, ignore
			break;
		}
		if (data._id != selectID) {
			continue;
		}

		bool isSchedule = type == PLSScheComboxItemType::Ty_Schedule;
		if (pPlatformVLive->getTempSelectData().isNormalLive && isSchedule) {
			//normal to schedule, temp saved.
			saveTempNormalDataWhenSwitch();
		}
		pPlatformVLive->setTempSchedule(isSchedule);
		pPlatformVLive->setTempSelectID(selectID);
		refreshUI();
		break;
	}
}

void PLSLiveInfoVLive::scheduleItemExpired(vector<QString> &ids)
{
	PLS_PLATFORM_VLIVE->removeExpiredSchedule(ids);
	updateScheduleComboxItems();
}

void PLSLiveInfoVLive::doUpdateOkState()
{
	bool haveChecked = false;
	vector<bool> boxChecks;
	ui->fanshipWidget->getChecked(boxChecks);

	for (const auto &checked : boxChecks) {
		if (true == checked) {
			haveChecked = true;
			break;
		}
	}

	if ((PLS_PLATFORM_VLIVE->getTempSelectData().isNormalLive && !haveChecked)) {
		ui->okButton->setEnabled(false);
		ui->rehearsalButton->setEnabled(false);
		return;
	}

	if (PLS_PLATFORM_API->isPrepareLive()) {
		ui->okButton->setEnabled(true);
		ui->rehearsalButton->setEnabled(true);
		return;
	}

	if (!isModified()) {
		ui->okButton->setEnabled(false);
		ui->rehearsalButton->setEnabled(false);
		return;
	}

	ui->okButton->setEnabled(true);
	ui->rehearsalButton->setEnabled(true);
}

bool PLSLiveInfoVLive::isModified()
{
	vector<bool> boxChecks;
	ui->fanshipWidget->getChecked(boxChecks);
	bool isModified = PLS_PLATFORM_VLIVE->isModifiedWithNewData(ui->lineEditTitle->text(), boxChecks, ui->thumbnailButton->getImagePath(), nullptr);
	if (!isModified && m_enteredID != PLS_PLATFORM_VLIVE->getTempSelectData()._id) {
		isModified = true;
	}
	return isModified;
}

void PLSLiveInfoVLive::titleEdited()
{
	static const int TitleLengthLimit = 100;
	QString newText = ui->lineEditTitle->text();

	bool isLargeToMax = false;
	if (newText.length() > TitleLengthLimit) {
		isLargeToMax = true;
		newText = newText.left(TitleLengthLimit);
	}

	if (newText.compare(ui->lineEditTitle->text()) != 0) {
		ui->lineEditTitle->setText(newText);
	}
	doUpdateOkState();

	if (isLargeToMax) {
		const auto channelName = PLS_PLATFORM_VLIVE->getInitData().value(ChannelData::g_channelName).toString();
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.Title.Length.Check.arg").arg(TitleLengthLimit).arg("V LIVE"));
	}
}
