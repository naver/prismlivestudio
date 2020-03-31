#include "PLSLiveInfoYoutube.h"
#include <frontend-api.h>
#include <QDebug>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QWidgetAction>
#include <qradiobutton.h>
#include <Vector>
#include "../PLSLiveInfoDialogs.h"
#include "../PLSPlatformApi.h"
#include "ChannelCommonFunctions.h"

#include "alert-view.hpp"
#include "log/log.h"
#include "ui_PLSLiveInfoYoutube.h"

static const char *liveInfoMoudule = "PLSLiveInfoYoutube";

PLSLiveInfoYoutube::PLSLiveInfoYoutube(PLSPlatformBase *pPlatformBase, QWidget *parent, PLSDpiHelper dpiHelper) : PLSLiveInfoBase(pPlatformBase, parent, dpiHelper), ui(new Ui::PLSLiveInfoYoutube)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSLiveInfoYoutube});
	dpiHelper.setFixedSize(this, {720, 550});

	PLS_INFO(liveInfoMoudule, "Youtube liveinfo Will show");
	ui->setupUi(this->content());

	setHasCloseButton(false);
	setHasBorder(true);

	ui->sPushButton->setFocusPolicy(Qt::NoFocus);
	content()->setFocusPolicy(Qt::StrongFocus);

	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;
	pPlatformYoutube->liveInfoisShowing();
	m_enteredID = PLS_PLATFORM_YOUTUBE->getSelectData()._id;
	setupFirstUI();

	connect(ui->okButton, &QPushButton::clicked, this, &PLSLiveInfoYoutube::okButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &PLSLiveInfoYoutube::cancelButtonClicked);

	pPlatformYoutube->setAlertParent(this);

	updateStepTitle(ui->okButton);

	if (PLS_PLATFORM_API->isPrepareLive()) {
		ui->bottomButtonWidget->layout()->addWidget(ui->okButton);
	}

	ui->sPushButton->setButtonEnable(!PLS_PLATFORM_API->isLiving());

	doUpdateOkState();
}

PLSLiveInfoYoutube::~PLSLiveInfoYoutube()
{
	delete ui;
}

void PLSLiveInfoYoutube::setupFirstUI()
{
	ui->kidsLabel->setHidden(true);
	ui->kidsRadioButton->setHidden(true);
	ui->notKidsRadioButton->setHidden(true);
	ui->hiddenRadioButton->setHidden(true);

	ui->lineEditTitle->setText("");
	ui->textEditDescribe->setText("");

	ui->textEditDescribe->setPlaceholderText(tr("LiveInfo.youtube.description.guide"));
	ui->comboBoxPublic->clear();
	ui->comboBoxCategory->clear();

	refreshUI();
	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;
	if (nullptr != pPlatformYoutube) {
		connect(pPlatformYoutube, &PLSPlatformYoutube::onGetTitleDescription, this, &PLSLiveInfoYoutube::refreshTitleDescri);
		connect(pPlatformYoutube, &PLSPlatformYoutube::onGetTitleDescription, this, &PLSLiveInfoYoutube::refreshPrivacy);
		connect(pPlatformYoutube, &PLSPlatformYoutube::onGetTitleDescription, this, &PLSLiveInfoYoutube::refreshSchedulePopButton);
		connect(pPlatformYoutube, &PLSPlatformYoutube::onGetCategorys, this, &PLSLiveInfoYoutube::refreshCategory);
	}

	connect(ui->sPushButton, &PLSScheduleCombox::pressed, this, &PLSLiveInfoYoutube::scheduleButtonClicked);
	connect(ui->sPushButton, &PLSScheduleCombox::menuItemClicked, this, &PLSLiveInfoYoutube::scheduleItemClick);
	connect(ui->kidsRadioButton, &QRadioButton::clicked, this, &PLSLiveInfoYoutube::setKidsRadioButtonClick);
	connect(ui->notKidsRadioButton, &QRadioButton::clicked, this, &PLSLiveInfoYoutube::setNotKidsRadioButtonClick);
	connect(ui->lineEditTitle, &QLineEdit::textChanged, this, &PLSLiveInfoYoutube::titleEdited);
	connect(ui->textEditDescribe, &QTextEdit::textChanged, this, &PLSLiveInfoYoutube::descriptionEdited);
	connect(ui->comboBoxPublic, SIGNAL(currentIndexChanged(int)), this, SLOT(doUpdateOkState()));
	connect(ui->comboBoxCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(doUpdateOkState()));
	connect(
		pPlatformYoutube, &PLSPlatformYoutube::closeDialogByExpired, this,
		[=]() {
			hideLoading();
			reject();
		},
		Qt::DirectConnection);
	connect(
		pPlatformYoutube, &PLSPlatformYoutube::toShowLoading, this,
		[=](bool isShow) {
			if (isShow) {
				showLoading(this->content());
			} else {

				hideLoading();
			}
		},
		Qt::DirectConnection);
}

void PLSLiveInfoYoutube::showEvent(QShowEvent *event)
{
	Q_UNUSED(event)
	showLoading(content());

	auto _onNextVideo = [=](bool value) {
		refreshUI();
		hideLoading();

		if (!value && !PLS_PLATFORM_API->isPrepareLive()) {
			reject();
		}
	};
	PLS_PLATFORM_YOUTUBE->requestCategoryList(
		[=](bool value) {
			if (!value) {
				_onNextVideo(false);
				return;
			}
			if (PLS_PLATFORM_YOUTUBE->getSelectData()._id.isEmpty()) {
				_onNextVideo(true);
				return;
			}
			PLS_PLATFORM_YOUTUBE->requestCurrentSelectData(_onNextVideo, this);
		},
		this);
	PLSLiveInfoBase::showEvent(event);
}

void PLSLiveInfoYoutube::refreshUI()
{
	refreshRadios();
	refreshTitleDescri();
	refreshPrivacy();
	refreshSchedulePopButton();
	refreshCategory();
}

void PLSLiveInfoYoutube::refreshTitleDescri()
{
	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;
	auto &data = pPlatformYoutube->getTempSelectData();

	ui->lineEditTitle->setText(data.title);
	ui->lineEditTitle->setCursorPosition(0);
	QString des = data.description;
	if (PLS_PLATFORM_API->isPrepareLive()) {
		QString add_str = tr("LiveInfo.Youtube.description.default.add");
		if (!des.contains(add_str)) {
			if (!des.isEmpty()) {
				des.append("\n");
			}
			des.append(add_str);
		}
	}
	ui->textEditDescribe->setText(des);
}

void PLSLiveInfoYoutube::refreshPrivacy()
{
	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;
	auto data = pPlatformYoutube->getTempSelectData();
	ui->comboBoxPublic->clear();
	for (int i = 0; i < pPlatformYoutube->getPrivacyDatas().size(); i++) {
		auto lcoalString = pPlatformYoutube->getPrivacyDatas()[i];
		ui->comboBoxPublic->addItem(lcoalString);

		QString enString = pPlatformYoutube->getPrivacyEnglishDatas()[i];
		if (0 == enString.compare(data.privacyStatus, Qt::CaseInsensitive)) {
			ui->comboBoxPublic->setCurrentIndex(i);
		}
	}
}

void PLSLiveInfoYoutube::refreshCategory()
{
	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;
	auto data = pPlatformYoutube->getTempSelectData();

	ui->comboBoxCategory->clear();
	for (int i = 0; i < pPlatformYoutube->getCategoryDatas().size(); i++) {
		auto categoryData = pPlatformYoutube->getCategoryDatas()[i];
		ui->comboBoxCategory->addItem(categoryData.title);
		if (0 == categoryData._id.compare(data.categoryID, Qt::CaseInsensitive)) {
			ui->comboBoxCategory->setCurrentIndex(i);
		}
	}
	if (ui->comboBoxCategory->currentIndex() < 0 && pPlatformYoutube->getCategoryDatas().size() > 0) {
		ui->comboBoxCategory->setCurrentIndex(0);
	}
}

void PLSLiveInfoYoutube::refreshSchedulePopButton()
{

	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;
	auto data = pPlatformYoutube->getTempSelectData();

	QString title(data.title);
	if (data.isNormalLive) {
		title = tr("LiveInfo.Schedule.PopUp.New");
	}
	ui->sPushButton->setupButton(title, data.startTimeShort);
}

void PLSLiveInfoYoutube::saveTempNormalDataWhenSwitch()
{
	PLSYoutubeLiveinfoData &tempData = PLS_PLATFORM_YOUTUBE->getTempNormalData();
	tempData.title = ui->lineEditTitle->text();
	tempData.description = ui->textEditDescribe->toPlainText();
	tempData.categoryID = PLS_PLATFORM_YOUTUBE->getCategoryDatas()[ui->comboBoxCategory->currentIndex()]._id;
	tempData.privacyStatus = PLS_PLATFORM_YOUTUBE->getPrivacyEnglishDatas()[ui->comboBoxPublic->currentIndex()];
	//tempData.iskidsUserSelect = ui->kidsRadioButton->isChecked() || ui->notKidsRadioButton->isChecked();
	//tempData.isForKids = ui->kidsRadioButton->isChecked();
}

void PLSLiveInfoYoutube::okButtonClicked()
{
	PLS_UI_STEP(liveInfoMoudule, "Youtube liveinfo OK Button Click", ACTION_CLICK);
	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;

	auto _onNext = [=](bool isSucceed) {
		hideLoading();
		PLS_INFO(liveInfoMoudule, "Youtube liveinfo Save %s", (isSucceed ? "succeed" : "failed"));
		if (isSucceed) {
			accept();
		}
	};

	showLoading(content());

	PLSYoutubeLiveinfoData uiData = pPlatformYoutube->getTempSelectData();
	bool isNeedUpdate = PLS_PLATFORM_YOUTUBE->isModifiedWithNewData(ui->lineEditTitle->text(), ui->textEditDescribe->toPlainText(), ui->comboBoxCategory->currentIndex(),
									ui->comboBoxPublic->currentIndex(), ui->kidsRadioButton->isChecked(), ui->notKidsRadioButton->isChecked(), &uiData);

	if (PLS_PLATFORM_API->isPrepareLive()) {
		QString add_str = tr("LiveInfo.Youtube.description.default.add");
		if (!uiData.description.contains(add_str)) {
			if (!uiData.description.isEmpty()) {
				uiData.description.append("\n");
			}
			uiData.description.append(add_str);
			isNeedUpdate = true;
		}
	}
	pPlatformYoutube->saveSettings(_onNext, isNeedUpdate, uiData);
}

void PLSLiveInfoYoutube::cancelButtonClicked()
{
	PLS_UI_STEP(liveInfoMoudule, "Youtube liveinfo Cancel Button Click", ACTION_CLICK);
	reject();
}

void PLSLiveInfoYoutube::scheduleButtonClicked()
{
	PLS_UI_STEP(liveInfoMoudule, "Youtube liveinfo schedule pop button click", ACTION_CLICK);
	if (!ui->sPushButton->getMenuHide()) {
		return;
	}
	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;

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
		if (ui->sPushButton == nullptr || ui->sPushButton->isMenuNULL() || ui->sPushButton->getMenuHide()) {
			return;
		}

		m_vecItemDatas.clear();

		for (auto data : pPlatformYoutube->getScheduleDatas()) {
			PLSScheComboxItemData scheData = PLSScheComboxItemData();
			scheData.title = data.title;
			scheData._id = data._id;
			scheData.type = PLSScheComboxItemType::Ty_Schedule;
			scheData.time = data.startTimeUTC;
			if (data._id != pPlatformYoutube->getTempSelectID()) {
				m_vecItemDatas.push_back(scheData);
			}
		}

		if (!pPlatformYoutube->getTempSelectData().isNormalLive) {
			PLSScheComboxItemData nomarlData = PLSScheComboxItemData();
			nomarlData._id = "";
			nomarlData.title = tr("LiveInfo.Schedule.PopUp.New");
			nomarlData.time = tr("LiveInfo.Schedule.PopUp.New");
			nomarlData.type = PLSScheComboxItemType::Ty_NormalLive;
			m_vecItemDatas.insert(m_vecItemDatas.begin(), nomarlData);
		}

		if (m_vecItemDatas.size() == 0) {
			PLSScheComboxItemData nomarlData = PLSScheComboxItemData();
			nomarlData._id = "";
			nomarlData.title = tr("LiveInfo.Schedule.PopUp.New");
			nomarlData.time = tr("LiveInfo.Youtube.no.scheduled");
			nomarlData.type = PLSScheComboxItemType::Ty_Placehoder;
			m_vecItemDatas.insert(m_vecItemDatas.begin(), nomarlData);
		}

		ui->sPushButton->showScheduleMenu(m_vecItemDatas);
	};

	if (nullptr != pPlatformYoutube) {
		pPlatformYoutube->requestScheduleList(_onNext, ui->sPushButton);
	}

	ui->sPushButton->showScheduleMenu(m_vecItemDatas);
}

void PLSLiveInfoYoutube::scheduleItemClick(const QString selectID)
{

	for (auto data : m_vecItemDatas) {
		if ((0 != data._id.compare(selectID))) {
			continue;
		}

		PLSScheComboxItemType type = data.type;
		if (type != PLSScheComboxItemType::Ty_Schedule && type != PLSScheComboxItemType::Ty_NormalLive) {
			break;
		}

		auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;

		if (pPlatformYoutube->getTempSelectID().compare(selectID) == 0) {
			break;
		}

		if (data._id != selectID) {
			continue;
		}

		if (pPlatformYoutube->getTempSelectData().isNormalLive && type == PLSScheComboxItemType::Ty_Schedule) {
			//normal to schedule, temp saved.
			saveTempNormalDataWhenSwitch();
		}
		pPlatformYoutube->setTempSelectID(selectID);
		refreshUI();
		break;
	}
}

void PLSLiveInfoYoutube::doUpdateOkState()
{
	if (ui->lineEditTitle->text().trimmed().isEmpty()) {
		ui->okButton->setEnabled(false);
		return;
	}
	/*if (!ui->kidsRadioButton->isChecked() && !ui->notKidsRadioButton->isChecked()) {
		ui->okButton->setEnabled(false);
		return;
	}*/
	ui->okButton->setEnabled(PLS_PLATFORM_API->isPrepareLive() || isModified());
}

void PLSLiveInfoYoutube::refreshRadios()
{
	return;
	auto data = PLS_PLATFORM_YOUTUBE->getTempSelectData();

	if (!data.iskidsUserSelect) {
		ui->hiddenRadioButton->setChecked(true);
	} else {
		if (data.isForKids) {
			ui->kidsRadioButton->setChecked(true);
		} else {
			ui->notKidsRadioButton->setChecked(true);
		}
	}
}

void PLSLiveInfoYoutube::setKidsRadioButtonClick(bool checked)
{
	PLS_UI_STEP(liveInfoMoudule, __FUNCTION__, BOOL2STR(checked), ACTION_CLICK);
	doUpdateOkState();
}

void PLSLiveInfoYoutube::setNotKidsRadioButtonClick(bool checked)
{
	PLS_UI_STEP(liveInfoMoudule, __FUNCTION__, BOOL2STR(checked), ACTION_CLICK);
	doUpdateOkState();
}

bool PLSLiveInfoYoutube::isModified()
{
	bool isModified = PLS_PLATFORM_YOUTUBE->isModifiedWithNewData(ui->lineEditTitle->text(), ui->textEditDescribe->toPlainText(), ui->comboBoxCategory->currentIndex(),
								      ui->comboBoxPublic->currentIndex(), ui->kidsRadioButton->isChecked(), ui->notKidsRadioButton->isChecked(), nullptr);
	if (!isModified) {
		if (m_enteredID.isEmpty() && PLS_PLATFORM_YOUTUBE->getTempSelectData().isNormalLive) {
			isModified = false;
		} else if (m_enteredID.compare(PLS_PLATFORM_YOUTUBE->getTempSelectData()._id, Qt::CaseInsensitive) != 0) {
			isModified = true;
		}
	}
	return isModified;
}

void PLSLiveInfoYoutube::titleEdited()
{
	static const int YoutubeTitleLengthLimit = 100;
	QString newText = ui->lineEditTitle->text();

	bool isLargeToMax = false;
	if (newText.length() > YoutubeTitleLengthLimit) {
		isLargeToMax = true;
		newText = newText.left(YoutubeTitleLengthLimit);
	}

	bool isContainSpecial = false;
	//this must use origin text to check special string
	if (ui->lineEditTitle->text().contains("<") || ui->lineEditTitle->text().contains(">")) {
		isContainSpecial = true;
		newText = newText.replace("<", "");
		newText = newText.replace(">", "");
	}
	if (newText.compare(ui->lineEditTitle->text()) != 0) {
		ui->lineEditTitle->setText(newText);
	}
	doUpdateOkState();

	auto pPlatformYoutube = PLS_PLATFORM_YOUTUBE;
	const auto channelName = pPlatformYoutube->getInitData().value(ChannelData::g_channelName).toString();

	if (isLargeToMax) {
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.Title.Length.Check.arg").arg(YoutubeTitleLengthLimit).arg(channelName));
	}

	if (isContainSpecial) {
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.Youtube.Title.Contain.Special.Text"));
	}
}

void PLSLiveInfoYoutube::descriptionEdited()
{
	static const int YoutubeDescribeLengthLimit = 5000;

	if (ui->textEditDescribe->toPlainText().length() > YoutubeDescribeLengthLimit) {
		ui->textEditDescribe->setText(ui->textEditDescribe->toPlainText().left(YoutubeDescribeLengthLimit));
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("LiveInfo.Youtube.Description.Length.Check.arg"));
	}

	doUpdateOkState();
}
