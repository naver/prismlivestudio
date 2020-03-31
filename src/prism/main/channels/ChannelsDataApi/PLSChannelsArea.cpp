#include "PLSChannelsArea.h"
#include <QGuiApplication>
#include <QPushButton>
#include <QScrollBar>
#include <QTime>
#include <QToolButton>
#include <QWheelEvent>
#include <QWindow>
#include "ChannelCapsule.h"
#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "ChannelDefines.h"
#include "ChannelsAddWin.h"
#include "DefaultPlatformsAddList.h"
#include "GoLivePannel.h"
#include "LogPredefine.h"
#include "PLSAddingFrame.h"
#include "PLSChannelDataAPI.h"
#include "PLSChannelsVirualAPI.h"
#include "ui_ChannelsArea.h"
#include "window-basic-main.hpp"

using namespace ChannelData;

PLSChannelsArea::PLSChannelsArea(QWidget *parent) : QFrame(parent), ui(new Ui::ChannelsArea), isHolding(false), isUiInitialized(false)
{
	ui->setupUi(this);
	initializeMychannels();

	auto defaultAddWid = new DefaultPlatformsAddList;
	ui->AddFrame->layout()->addWidget(defaultAddWid);

	goliveWid = new GoLivePannel;
	ui->TailFrame->layout()->addWidget(goliveWid);

	initScollButtons();

	ui->MidFrame->setAttribute(Qt::WA_Hover);
	ui->MidFrame->installEventFilter(this);

	ui->AddFrameInvisible->setVisible(false);
	connect(ui->GotoAddWinButton, &QAbstractButton::clicked, this, &PLSChannelsArea::showChannelsAdd, Qt::QueuedConnection);

	mbusyFrame = new PLSAddingFrame(ui->MidFrame);
	mbusyFrame->setObjectName("LoadingFrame");
	mbusyFrame->setContent(CHANNELS_TR(Loading));
	mbusyFrame->setSourceFirstFile(g_loadingPixPath);
	mbusyFrame->hide();
	mbusyFrame->installEventFilter(this);

	connect(PLSCHANNELS_API, &PLSChannelDataAPI::channelAdded, this, QOverload<const QString &>::of(&PLSChannelsArea::addChannel), Qt::QueuedConnection);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::channelRemoved, this, &PLSChannelsArea::removeChannel, Qt::QueuedConnection);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::channelModified, this, &PLSChannelsArea::updateChannelUi, Qt::QueuedConnection);

	connect(
		PLSCHANNELS_API, &PLSChannelDataAPI::holdOnChannelArea, this,
		[=](bool isHold) {
			if (isHold) {
				mbusyFrame->setContent("");
				mbusyFrame->resize(PLSDpiHelper::calculate(this, QSize(54, 54)));
			}

			holdOnChannelArea(isHold);
		},
		Qt::QueuedConnection);
	connect(
		PLSCHANNELS_API, &PLSChannelDataAPI::addingHold, this,
		[=](bool isHold) {
			if (isHold) {
				mbusyFrame->setContent(CHANNELS_TR(Adding));
				mbusyFrame->resize(PLSDpiHelper::calculate(this, QSize(200, 54)));
			}

			holdOnChannelArea(isHold);
		},
		Qt::QueuedConnection);

	connect(PLSCHANNELS_API, &PLSChannelDataAPI::holdOnGolive, goliveWid, &GoLivePannel::holdOnAll, Qt::QueuedConnection);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::sigAllClear, this, &PLSChannelsArea::clearAll, Qt::QueuedConnection);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::liveStateChanged, this, &PLSChannelsArea::delayUpdateUi, Qt::QueuedConnection);
	connect(PLSCHANNELS_API, &PLSChannelDataAPI::toDoinitialize, this, &PLSChannelsArea::beginInitChannels, Qt::QueuedConnection);
	connect(this, &PLSChannelsArea::sigNextInitialize, this, &PLSChannelsArea::initializeNextStep, Qt::QueuedConnection);

	connect(
		PLSCHANNELS_API, &PLSChannelDataAPI::networkInvalidOcurred, this, []() { showNetworkErrorAlert(); }, Qt::QueuedConnection);

	PLSDpiHelper dpiHelper;
	dpiHelper.notifyDpiChanged(this, [=](double dpi, double oldDpi) { mbusyFrame->resize(mbusyFrame->width() * (dpi / oldDpi), mbusyFrame->height() * (dpi / oldDpi)); });
}

PLSChannelsArea::~PLSChannelsArea()
{
	mChannelsWidget.clear();
	delete ui;
}

void PLSChannelsArea::beginInitChannels()
{
	//qDebug() << " time initialize :" << QTime::currentTime();
	PRE_LOG(InitChannels, INFO);
	isUiInitialized = false;
	checkIsEmptyUi();
	this->holdOnChannelArea(true);
	mInitializeInfos = PLSCHANNELS_API->sortAllChannels();
	PLSCHANNELS_API->release();
	emit sigNextInitialize();
}
void PLSChannelsArea::endInitialize()
{
	//qDebug() << " time initialize :" << QTime::currentTime();
	PLSCHANNELS_API->sigChannelAreaInialized();
	if (PLSCHANNELS_API->hasError()) {
		PLSCHANNELS_API->networkInvalidOcurred();
	}
	this->holdOnChannelArea(false);
	isUiInitialized = true;
}
void PLSChannelsArea::initializeNextStep()
{
	if (PLSCHANNELS_API->isExit()) {
		PLSCHANNELS_API->acquire();
		return;
	}

	if (!mInitializeInfos.isEmpty()) {
		auto channelInfo = mInitializeInfos.takeFirst();
		addChannel(channelInfo);
	}

	if (!mInitializeInfos.isEmpty()) {
		emit sigNextInitialize();
		return;
	}

	if (mInitializeInfos.isEmpty()) {
		PLSCHANNELS_API->acquire();
		endInitialize();
		return;
	}
}

void PLSChannelsArea::holdOnChannelArea(bool holdOn)
{
	//hold
	isHolding = holdOn;
	if (holdOn && mbusyFrame->isVisible()) {
		return;
	}
	if (holdOn) {
		auto posParentC = ui->MidFrame->contentsRect().center();
		auto posMC = mbusyFrame->contentsRect().center();
		auto posDiffer = posParentC - posMC;
		mbusyFrame->move(posDiffer);
		mbusyFrame->start(200);
		mbusyFrame->setVisible(true);

		updateUi();

		return;
	}

	// unhold
	if (PLSCHANNELS_API->isEmptyToAcquire()) {
		delayUpdateUi();
		delayTask(&PLSChannelsArea::hideLoading, 300);
	}
}

void PLSChannelsArea::refreshOrder()
{
	PLSCHANNELS_API->sortAllChannels();
}

void PLSChannelsArea::initScollButtons()
{

	mLeftButon = new QPushButton(ui->MidFrame);
	mLeftButon->setObjectName("LeftButton");
	mLeftButon->setAutoRepeat(true);
	mLeftButon->setAutoRepeatDelay(500);
	mLeftButon->setAutoRepeatInterval(500);
	ui->MidFrameLayout->insertWidget(0, mLeftButon);
	connect(mLeftButon, &QPushButton::clicked, [=]() { scrollNext(true); });
	mLeftButon->setVisible(false);

	mRightButton = new QPushButton(ui->MidFrame);
	mRightButton->setObjectName("RightButton");
	mRightButton->setAutoRepeat(true);
	mRightButton->setAutoRepeatDelay(500);
	mRightButton->setAutoRepeatInterval(500);
	ui->MidFrameLayout->addWidget(mRightButton);
	connect(mRightButton, &QPushButton::clicked, [=]() { scrollNext(false); });
	mRightButton->setVisible(false);

	ui->ChannelScrollArea->verticalScrollBar()->setDisabled(true);
	ui->NormalDisFrameLayout->addStretch(10);
}

void PLSChannelsArea::checkScrollButtonsState(ScrollDirection direction)
{
	bool isNeeded = isScrollButtonsNeeded();
	enabledScrollButtons(isNeeded);
	if (isNeeded) {
		switch (direction) {
		case PLSChannelsArea::NOScroll:
			break;
		case PLSChannelsArea::ForwardScroll:
			ensureCornerChannelVisible(true);
			break;
		case PLSChannelsArea::BackScroll:
			ensureCornerChannelVisible(false);
			break;
		default:
			break;
		}

		buttonLimitCheck();
	}
}

void PLSChannelsArea::insertChannelCapsule(QWidget *wid, int index)
{
	auto scrollLay = dynamic_cast<QHBoxLayout *>(ui->CapusuleLayout);
	scrollLay->insertWidget(index, wid);
}

void PLSChannelsArea::scrollNext(bool forwartStep)
{
	ensureCornerChannelVisible(forwartStep);
	auto bar = ui->ChannelScrollArea->horizontalScrollBar();
	int width = PLSDpiHelper::calculate(this, 200);
	int currentV = bar->value();
	int lastV = currentV + (forwartStep ? -1 : 1) * width;
	bar->setValue(lastV);

	buttonLimitCheck();
}

void PLSChannelsArea::ensureCornerChannelVisible(bool forwartStep)
{
	auto recView = ui->ChannelScrollArea->contentsRect();
	auto targetPos = forwartStep ? (recView.topLeft() + QPoint(10, 10)) : (recView.topRight() + QPoint(-10, 10));
	auto child = ui->ChannelScrollArea->childAt(targetPos);
	//qDebug() << " child " << child;
	if (child) {
		//qDebug() << " child info " << child->metaObject()->className();
		auto capusle = findParent<ChannelCapsule *>(child);
		if (capusle) {
			ui->ChannelScrollArea->ensureWidgetVisible(capusle, 1, 1);
		}
	}
}

void PLSChannelsArea::buttonLimitCheck()
{
	auto bar = ui->ChannelScrollArea->horizontalScrollBar();
	mRightButton->setDisabled(bar->value() == bar->maximum());
	mLeftButon->setDisabled(bar->value() == bar->minimum());
}

void PLSChannelsArea::displayScrollButtons(bool isShow)
{
	mLeftButon->setVisible(isShow);
	mRightButton->setVisible(isShow);
}

void PLSChannelsArea::enabledScrollButtons(bool isEnabled)
{
	mLeftButon->setEnabled(isEnabled);
	mRightButton->setEnabled(isEnabled);
}

void PLSChannelsArea::addChannel(const QString &channelUUID)
{
	auto channelInfo = PLSCHANNELS_API->getChannelInfo(channelUUID);
	if (channelInfo.isEmpty()) {
		return;
	}
	auto channelWid = addChannel(channelInfo);
	if (!isUiInitialized || !channelWid->isVisible()) {
		return;
	}

	bool isLeader = getInfo(channelInfo, g_isLeader, true);
	if (isLeader) {
		checkScrollButtonsState(BackScroll);
		ui->ChannelScrollArea->ensureWidgetVisible(channelWid.data(), PLSDpiHelper::calculate(this, 10), PLSDpiHelper::calculate(this, 10));
	}
}

ChannelCapsulePtr PLSChannelsArea::addChannel(const QVariantMap &channelInfo)
{
	ChannelCapsulePtr channelWid(new ChannelCapsule(), deleteChannelWidget<ChannelCapsule>);
	auto uuid = getInfo(channelInfo, g_channelUUID);
	channelWid->setChannelID(uuid);
	int order = getInfo(channelInfo, g_displayOrder, -1);
	this->insertChannelCapsule(channelWid.data(), order);
	mChannelsWidget.insert(uuid, channelWid);
	bool isToShow = getInfo(channelInfo, g_displayState, true);
	if (isToShow) {
		channelWid->updateUi();
	}
	channelWid->setVisible(isToShow);
	return channelWid;
}

bool PLSChannelsArea::checkIsEmptyUi()
{
	//empty channels
	if (PLSCHANNELS_API->isEmpty()) {
		ui->AddFrameInvisible->setVisible(false);
		ui->NormalDisFrame->hide();
		displayScrollButtons(false);
		ui->AddFrame->show();
		return true;
	}

	// no display channels
	int visibleWidgets = visibleCount();
	if (isUiInitialized && visibleWidgets < 1) {
		ui->NormalDisFrame->setVisible(false);
		displayScrollButtons(false);
		ui->AddFrame->hide();
		ui->AddFrameInvisible->setVisible(true);
		return false;
	}

	//normal
	ui->AddFrame->hide();
	ui->AddFrameInvisible->setVisible(false);
	ui->NormalDisFrame->show();
	displayScrollButtons(true);

	return false;
}

void PLSChannelsArea::updateUi()
{
	int state = PLSCHANNELS_API->currentBroadcastState();

	switch (state) {
	case ReadyState: {
		//hold
		if (isHolding) {
			myChannelsIconBtn->setEnabled(false);
			ui->MidFrame->setEnabled(false);
			ui->TailFrame->setEnabled(false);
			App()->DisableHotkeys();
			break;
		}
		//unhold
		{
			delayUpdateAllChannelsUi();
			ui->MidFrame->setEnabled(true);
			myChannelsIconBtn->setEnabled(true);
			ui->TailFrame->setEnabled(true);
			App()->UpdateHotkeyFocusSetting(true);
		}

	} break;

	case BroadcastGo: {
		myChannelsIconBtn->setEnabled(false);
		ui->MidFrame->setEnabled(false);
	} break;
	case StreamStarted: {
		delayUpdateAllChannelsUi();
		ui->MidFrame->setEnabled(true);
	} break;
	case StopBroadcastGo: {
		ui->MidFrame->setEnabled(false);
	} break;
	default:
		break;
	}
}

void PLSChannelsArea::delayUpdateUi()
{
	delayTask(&PLSChannelsArea::updateUi);
}

void PLSChannelsArea::updateAllChannelsUi()
{
	bool isLiving = PLSCHANNELS_API->isLiving();
	auto check = [&](ChannelCapsulePtr wid) {
		if ((!wid->isSelectedDisplay()) || (isLiving && !wid->isOnLine())) {
			wid->hide();
		} else {
			wid->show();
			wid->updateUi();
		}
	};
	std::for_each(mChannelsWidget.begin(), mChannelsWidget.end(), check);
}

void PLSChannelsArea::delayUpdateAllChannelsUi()
{
	static QTimer *delayTimer = nullptr;
	if (delayTimer == nullptr) {
		delayTimer = new QTimer(this);
		delayTimer->setSingleShot(true);
		connect(delayTimer, &QTimer::timeout, this, [=]() {
			updateAllChannelsUi();
			checkIsEmptyUi();
			checkScrollButtonsState(isUiInitialized ? NOScroll : ForwardScroll);
		});
	}
	delayTimer->start(200);
}

void PLSChannelsArea::hideLoading()
{
	mbusyFrame->setVisible(false);
	mbusyFrame->stop();
}

void PLSChannelsArea::removeChannel(const QString &channelUUID)
{
	auto ite = mChannelsWidget.find(channelUUID);
	if (ite != mChannelsWidget.end()) {
		auto wid = ite.value();
		wid->hide();
		mChannelsWidget.erase(ite);
		delayTask(&PLSChannelsArea::refreshOrder);
	}
}

void PLSChannelsArea::updateChannelUi(const QString &channelUUID)
{
	if (PLSCHANNELS_API->isLiving()) {
		delayUpdateAllChannelsUi();
		return;
	}

	auto channelWid = mChannelsWidget.value(channelUUID);
	if (channelWid == nullptr) {
		return;
	}
	if (channelWid->isSelectedDisplay()) {
		channelWid->setVisible(true);
		channelWid->updateUi();
		return;
	}

	channelWid->setVisible(false);
}

void PLSChannelsArea::refreshChannels()
{
	PRE_LOG_UI(My channels Clicked, PLSChannelsArea);

	auto matchedPlaftorms = PLSCHANNELS_API->getAllChannelInfo();

	auto isMatched = [&](const QVariantMap &info) {
		auto platform = getInfo(info, g_channelName);
		return g_platformsToClearData.contains(platform, Qt::CaseInsensitive) && getInfo(info, g_data_type, NoType) == ChannelType;
	};
	auto ret = std::find_if(matchedPlaftorms.constBegin(), matchedPlaftorms.constEnd(), isMatched);

	if (ret != matchedPlaftorms.constEnd()) {

		auto ret = PLSAlertView::question(this, tr("Live.Check.Alert.Title"), tr("RefreshChannel.DeleteLiveInfo.Alert.Message"),
						  {{PLSAlertView::Button::Yes, tr("RefreshChannel.DeleteLiveInfo.Alert.Refresh")}, {PLSAlertView::Button::Cancel, CHANNELS_TR(Cancel)}},
						  PLSAlertView::Button::Cancel);
		if (ret != PLSAlertView::Button::Yes) {
			return;
		}
	}
	PLSCHANNELS_API->setResetNeed(true);
	PLSCHANNELS_API->sigRefreshAllChannels();
}

void PLSChannelsArea::showChannelsAdd()
{
	auto mainW = pls_get_main_view();
	PRE_LOG_UI(show add channels, PLSChannelsArea);
	auto channelsAdd = new ChannelsAddWin(this);
	channelsAdd->setWindowFlags(Qt::Popup | Qt::NoDropShadowWindowHint);
	channelsAdd->setAttribute(Qt::WA_DeleteOnClose);
	channelsAdd->move(mainW->pos());
	channelsAdd->show();

	auto center = mainW->rect().center();
	auto selfCenter = QPoint(channelsAdd->width() / 2, channelsAdd->height() / 2);
	auto pos = center;
	channelsAdd->move(mainW->mapToGlobal(pos) - selfCenter);
}

void PLSChannelsArea::clearAll()
{
	mChannelsWidget.clear();
}

void PLSChannelsArea::clearAllRTMP()
{
	auto ite = mChannelsWidget.begin();
	while (ite != mChannelsWidget.end()) {
		if (!PLSCHANNELS_API->isChannelInfoExists(ite.key())) {
			ite = mChannelsWidget.erase(ite);
		} else {
			++ite;
		}
	}
}

void PLSChannelsArea::changeEvent(QEvent *e)
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

void PLSChannelsArea::wheelEvent(QWheelEvent *event)
{
	QPoint numDegrees = event->angleDelta() / 8;
	scrollNext(numDegrees.y() > 0);
	event->accept();
}

bool PLSChannelsArea::eventFilter(QObject *watched, QEvent *event)
{
	switch (event->type()) {
	case QEvent::Resize: {
		if (watched == ui->MidFrame || watched == mbusyFrame) {
			auto sizeToPoint = [](const QSize &size) { return QPoint(size.width(), size.height()); };
			mbusyFrame->move(sizeToPoint((ui->MidFrame->size() - mbusyFrame->size()) / 2));
			if (watched == ui->MidFrame && isUiInitialized) {
				checkScrollButtonsState(ForwardScroll);
			}
			return true;
		}
	} break;
	default:

		break;
	}
	return false;
}

bool PLSChannelsArea::isScrollButtonsNeeded()
{
	ui->scrollAreaWidgetContents->adjustSize();
	auto recView = ui->ChannelScrollArea->contentsRect();
	auto scrollGeo = ui->scrollAreaWidgetContents->contentsRect();
	return recView.width() < scrollGeo.width();
}

void PLSChannelsArea::initializeMychannels()
{
	myChannelsIconBtn = new QToolButton();
	myChannelsIconBtn->setObjectName("MyChannelsIconBtn");
	myChannelsTxtBtn = new QPushButton(CHANNELS_TR(MyChannel));
	myChannelsTxtBtn->setObjectName("MyChannelsTxtBtn");
	ui->HeaderLayout->addWidget(myChannelsTxtBtn, 5);
	ui->HeaderLayout->addWidget(myChannelsIconBtn, 1);
	ui->HeaderLayout->addStretch(4);

	auto menu = new QMenu(myChannelsIconBtn);
	menu->setObjectName("MyChannelsMenu");
	auto settingAction = menu->addAction(CHANNELS_TR(SettingChannels), this, [=]() { showChannelsSetting(); });
	menu->addAction(CHANNELS_TR(AddChannels), this, &PLSChannelsArea::showChannelsAdd);
	menu->addAction(CHANNELS_TR(RefreshChannels), this, &PLSChannelsArea::refreshChannels);

	connect(
		myChannelsIconBtn, &QToolButton::clicked, this,
		[=]() {
			settingAction->setDisabled(PLSCHANNELS_API->isEmpty());
			auto pos = QPoint(myChannelsIconBtn->width() / 2, myChannelsIconBtn->height());
			menu->exec(myChannelsIconBtn->mapToGlobal(pos));
		},
		Qt::QueuedConnection);
}

int PLSChannelsArea::visibleCount()
{
	auto isWidVisible = [](ChannelCapsulePtr wid) { return wid->isSelectedDisplay(); };
	return std::count_if(mChannelsWidget.begin(), mChannelsWidget.end(), isWidVisible);
}
