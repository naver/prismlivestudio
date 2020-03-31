#include "ChannelsSettingsWidget.h"
#include <QCloseEvent>
#include "ChannelCommonFunctions.h"
#include "ChannelItem.h"
#include "LogPredefine.h"
#include "PLSChannelsVirualAPI.h"
#include "PLSDpiHelper.h"
#include "ui_ChannelsSettingsWidget.h"

ChannelsSettingsWidget::ChannelsSettingsWidget(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::ChannelsSettingsWidget)
{
	dpiHelper.setCss(this, {PLSCssIndex::ChannelsSettingsWidget});
	setResizeEnabled(false);
	ui->setupUi(this->content());
	this->initializePlatforms(getDefaultPlatforms());
	this->setHasCaption(false);
	QMetaObject::connectSlotsByName(this);
	PRE_LOG_UI(Channels settings, ChannelsSettingsWidget);

	ui->CenterStack->setFocus();

	dpiHelper.notifyDpiChanged(this, [this](double dpi) {
		if (ui->ChannelsListCombox->currentIndex() >= 1) {
			auto platform = ui->ChannelsListCombox->currentText();
			setGuidePageInfo(platform);
		}
	});
}

ChannelsSettingsWidget::~ChannelsSettingsWidget()
{
	delete ui;
}

void ChannelsSettingsWidget::setChannelsData(const ChannelsMap &datas, const QString &platform)
{

	int index = getDefaultPlatforms().indexOf(platform) + 1;
	this->setChannelsData(datas, index);
}

void ChannelsSettingsWidget::setChannelsData(const ChannelsMap &datas, int index)
{
	mOriginalInfo = datas;
	mLastChannelsInfo = datas;
	auto ite = datas.begin();
	for (; ite != datas.end(); ++ite) {
		const auto &var = ite.value();
		auto item = new ChannelItem(this);
		item->setData(var);
		int order = getInfo(var, ChannelData::g_displayOrder, int(0));
		QString orderStr = QString::number(order).rightJustified(10, '0');
		auto listItem = new QListWidgetItem(orderStr, ui->listWidget);
		listItem->setData(ChannelItemData, var);
		ui->listWidget->addItem(listItem);
		ui->listWidget->setItemWidget(listItem, item);
		connect(item, &ChannelItem::sigSelectionChanged, this, &ChannelsSettingsWidget::onSelectionChanged);
	}
	ui->listWidget->setItemDelegate(new GeometryDelegate(this));
	ui->listWidget->sortItems();
	ui->ChannelsListCombox->setCurrentIndex(index);
}

void ChannelsSettingsWidget::initializePlatforms(const QStringList &platforms)
{
	QStringList tmp = platforms;
	tmp.prepend(CHANNELS_TR(ALL));
	ui->ChannelsListCombox->addItems(tmp);
	ui->ChannelsListCombox->setMaxVisibleItems(tmp.size());

	auto pushButton = new QPushButton(ui->ChannelsListCombox);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(pushButton);
	layout->setMargin(0);
	ui->ChannelsListCombox->setLayout(layout);
	pushButton->setText(ui->ChannelsListCombox->currentText());
	pushButton->setCheckable(true);
	connect(pushButton, &QPushButton::clicked, ui->ChannelsListCombox, &PLSComboBox::showPopup);
	connect(ui->ChannelsListCombox, &QComboBox::currentTextChanged, pushButton, &QPushButton::setText);
	connect(ui->ChannelsListCombox, &PLSComboBox::popupShown, pushButton, &QPushButton::setChecked);
}

void ChannelsSettingsWidget::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void ChannelsSettingsWidget::closeEvent(QCloseEvent *event)
{
	if (!mLastChannelsInfo.isEmpty() && hasSelected()) {
		event->ignore();
		return;
	}
	applyChanges();
	event->accept();
}

bool ChannelsSettingsWidget::hasSelected()
{
	auto isSelected = [](const QVariantMap &info) { return getInfo(info, ChannelData::g_displayState, true); };
	auto ite = std::find_if(mLastChannelsInfo.cbegin(), mLastChannelsInfo.cend(), isSelected);
	if (ite != mLastChannelsInfo.cend()) {
		return true;
	}
	return false;
}

void ChannelsSettingsWidget::applyChanges()
{
	HolderReleaser releaser(&PLSChannelDataAPI::holdOnChannelArea, PLSCHANNELS_API);
	auto lstIte = mLastChannelsInfo.cbegin();
	for (; lstIte != mLastChannelsInfo.cend(); ++lstIte) {
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		const auto &varMap = lstIte.value();
		auto orginalInfo = mOriginalInfo.value(lstIte.key());
		if (getInfo(varMap, ChannelData::g_displayState, true) == getInfo(orginalInfo, ChannelData::g_displayState, true)) {
			continue;
		}
		PLSCHANNELS_API->setChannelInfos(varMap, false);
	}
}

void ChannelsSettingsWidget::on_ChannelsListCombox_currentIndexChanged(const QString &arg1)
{
	PRE_LOG_UI_MSG(QString("Channels Filter changed to " + arg1).toStdString().c_str(), ChannelsSettingsWidget);
	int rowCount = ui->listWidget->count();
	if (rowCount == 0) {
		return;
	}
	int visibleCount = 0;
	int type = ChannelData::ChannelType;
	if (ui->ChannelsListCombox->currentIndex() == (ui->ChannelsListCombox->count() - 1)) {
		type = (ChannelData::RTMPType);
	} else if (ui->ChannelsListCombox->currentIndex() == 0) {
		type = (ChannelData::NoType);
	}
	// to check visibility of item
	auto transformItems = [&](std::function<bool(const QListWidgetItem *)> fun) {
		for (int i = 0; i < rowCount; ++i) {
			auto item = ui->listWidget->item(i);
			bool toHide = fun(item);
			item->setHidden(toHide);
			if (!toHide) {
				++visibleCount;
			}
		}
	};

	if (type == ChannelData::RTMPType) {

		auto isNotRTMP = [](const QListWidgetItem *item) {
			auto varMap = item->data(ChannelItemData).toMap();
			int tmpType = getInfo(varMap, ChannelData::g_data_type, ChannelData::NoType);
			return (tmpType == ChannelData::ChannelType);
		};
		transformItems(isNotRTMP);
	}

	if (type == ChannelData::ChannelType) {
		auto isNotMatchPlatform = [&](const QListWidgetItem *item) {
			auto varMap = item->data(ChannelItemData).toMap();
			int tmpType = getInfo(varMap, ChannelData::g_data_type, ChannelData::NoType);
			QString platformName = getInfo(varMap, ChannelData::g_channelName);
			return (!platformName.contains(arg1, Qt::CaseInsensitive) || tmpType != ChannelData::ChannelType);
		};
		transformItems(isNotMatchPlatform);
	}

	if (type == ChannelData::NoType) {
		auto allNotHide = [](const QListWidgetItem *) { return false; };
		transformItems(allNotHide);
	}

	if (visibleCount == 0) {
		ui->CenterStack->setCurrentIndex(1);
		setGuidePageInfo(arg1);
	} else {
		ui->CenterStack->setCurrentIndex(0);
	}
}

void ChannelsSettingsWidget::onSelectionChanged(const QString &uuid, bool isSelected)
{
	auto ite = mLastChannelsInfo.find(uuid);
	if (ite != mLastChannelsInfo.end()) {
		auto &varMap = ite.value();
		varMap.insert(ChannelData::g_displayState, isSelected);
		if (!isSelected) {
			varMap.insert(ChannelData::g_channelUserStatus, ChannelData::Disabled);
		}
	}
}

void ChannelsSettingsWidget::on_ApplySettingsBtn_clicked()
{
	if (!hasSelected()) {
		PLSAlertView::warning(this, CHANNELS_TR(Alert), CHANNELS_TR(NoChannelSelected));
		return;
	}
	PRE_LOG_UI_MSG("try apply changes  ", ChannelsSettingsWidget);
	applyChanges();
	this->accept();
}

void ChannelsSettingsWidget::on_Cancel_clicked()
{
	PRE_LOG_UI_MSG("cancel   ", ChannelsSettingsWidget);
	this->reject();
}

void ChannelsSettingsWidget::on_GotoLoginBtn_clicked()
{
	if (!hasSelected()) {
		PLSAlertView::warning(this, CHANNELS_TR(Alert), CHANNELS_TR(AtLeastOneSelected));
		return;
	}
	PRE_LOG_UI_MSG("try to go to login page   ", ChannelsSettingsWidget);
	applyChanges();
	this->accept();
	QString cmd = ui->ChannelsListCombox->currentText();
	runCMD(cmd);
}

void ChannelsSettingsWidget::setGuidePageInfo(const QString &platfom)
{
	QString guidInfo;
	QString guidBtnText;
	if (ui->ChannelsListCombox->currentIndex() == ui->ChannelsListCombox->count() - 1) {
		guidInfo = CHANNELS_TR(GuideRTM);
		guidBtnText = CHANNELS_TR(GotoLoginRTMP);
	} else {
		guidInfo = CHANNELS_TR(GuideInfos);
		guidBtnText = CHANNELS_TR(GotoLogin);
	}
	ui->GuideLabel->setText(guidInfo);
	ui->GotoLoginBtn->setText(guidBtnText);

	auto pixPath = getPlatformImageFromName(platfom, "addch-", "-large");
	QPixmap pix;
	loadPixmap(pix, pixPath, ui->LogoLabel->size());
	ui->LogoLabel->setPixmap(pix);
}
