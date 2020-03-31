#include "PLSChatDialog.h"
#include <assert.h>
#include <frontend-api.h>
#include <qdebug.h>
#include <qlabel.h>
#include <qstyle.h>
#include <qtabbar.h>
#include <QDesktopWidget>
#include <QFile>
#include <QSettings>
#include "../PLSPlatformApi/PLSPlatformApi.h"
#include "ChannelCommonFunctions.h"
#include "PLSChannelDataAPI.h"
#include "log/log.h"
#include "main-view.hpp"
#include "pls-app.hpp"
#include "pls-common-define.hpp"
#include "ui_PLSChatDialog.h"

extern QCef *cef;

static const char *s_chatModuleName = "PLSChat";
static const char *s_geometrySection = "BasicWindow";
static const char *s_geometryName = "geometryChat";
static const char *s_maxSection = "ChatDialog";
static const char *s_maxName = "MaximizedState";
static const char *s_hiddenSection = "Basic";
static const char *s_hiddenName = "chatIsHidden";

namespace {
//PRISM/Zhangdewen/20200921/#/add chat source button
class ChatSourceButton : public QFrame {
	bool hovered = false;
	bool pressed = false;
	std::function<void()> clicked;

public:
	ChatSourceButton(const QString &buttonText, QWidget *parent, std::function<void()> clicked_) : QFrame(parent), clicked(std::move(clicked_))
	{
		setObjectName("chatSourceButton");
		setProperty("lang", pls_get_curreng_language());
		setMouseTracking(true);

		QLabel *icon = new QLabel(this);
		icon->setObjectName("chatSourceButtonIcon");
		icon->setMouseTracking(true);

		QLabel *text = new QLabel(this);
		text->setObjectName("chatSourceButtonText");
		text->setMouseTracking(true);
		text->setText(buttonText);

		QHBoxLayout *layout = new QHBoxLayout(this);
		layout->setContentsMargins(10, 0, 10, 0);
		layout->setSpacing(5);
		layout->addWidget(icon);
		layout->addWidget(text);
	}
	virtual ~ChatSourceButton() {}

private:
	void setState(const char *name, bool &state, bool value)
	{
		if (state != value) {
			pls_flush_style_recursive(this, name, state = value);
		}
	}

protected:
	virtual bool event(QEvent *event) override
	{
		switch (event->type()) {
		case QEvent::Enter:
			setState("hovered", hovered, true);
			break;
		case QEvent::Leave:
			setState("hovered", hovered, false);
			break;
		case QEvent::MouseButtonPress:
			setState("pressed", pressed, true);
			break;
		case QEvent::MouseButtonRelease:
			setState("pressed", pressed, false);
			if (rect().contains(dynamic_cast<QMouseEvent *>(event)->pos())) {
				clicked();
			}
			break;
		case QEvent::MouseMove: {
			setState("hovered", hovered, rect().contains(dynamic_cast<QMouseEvent *>(event)->pos()));
			break;
		}
		}
		return QFrame::event(event);
	}
};
}

void onPrismUserLogout(pls_frontend_event event, const QVariantList &params, void *context)
{
	Q_UNUSED(event);
	Q_UNUSED(params);
	PLSChatDialog *view = static_cast<PLSChatDialog *>(context);
	view->logoutToReInitUI();
}

void onPrismAppQuit(enum obs_frontend_event event, void *context)
{
	if (event == OBS_FRONTEND_EVENT_EXIT) {
		PLSChatDialog *view = static_cast<PLSChatDialog *>(context);
		if (!view->getMaxState()) {
			view->onSaveNormalGeometry();
		}
		view->deleteLater();
	}
}

PLSChatDialog::PLSChatDialog(QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper), ui(new Ui::PLSChatDialog), m_timerToastHide(this), m_pLabelToast(nullptr), m_p_ButtonToastClose(nullptr), chat_panel_cookies(nullptr)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSChatDialog});
	dpiHelper.notifyDpiChanged(this, [this](double dpi, double, bool firstShow) {
		extern QRect normalShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (firstShow) {
			setInitSize(dpi, false);
			if (!isMaxState && !isFullScreenState) {
				normalShow(this, geometryOfNormal);
			}
		}
	});

	ui->setupUi(this->content());
	setHasMaxResButton(true);
	setInitSize(PLSDpiHelper::getDpi(this), true);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	setupFirstUI(dpiHelper);

	connect(
		PLS_PLATFORM_API, &PLSPlatformApi::channelRemoved, this, [=]() { refreshUI(); }, Qt::QueuedConnection);

	connect(
		PLSCHANNELS_API, &PLSChannelDataAPI::channelActiveChanged, this, [=]() { refreshUI(); }, Qt::QueuedConnection);
	connect(
		PLSCHANNELS_API, &PLSChannelDataAPI::sigAllChannelRefreshDone, this, [=]() { refreshUI(); }, Qt::QueuedConnection);
	connect(
		PLS_PLATFORM_YOUTUBE, &PLSPlatformYoutube::selectIDChanged, this, [=]() { refreshUI(); }, Qt::QueuedConnection);
	connect(
		PLS_PLATFORM_YOUTUBE, &PLSPlatformYoutube::privacyChangedWhenliving, this, [=]() { updateYoutubeUrlIfNeeded(); }, Qt::QueuedConnection);
	connect(
		PLS_PLATFORM_API, &PLSPlatformApi::liveStarted, this,
		[=](bool isSucceed) {
			updateRtmpPlaceText();
			if (isSucceed) {
				showToastIfNeeded();
				updateYoutubeUrlIfNeeded();
			}
		},
		Qt::QueuedConnection);
	connect(
		PLS_PLATFORM_API, &PLSPlatformApi::liveEnded, this, [=]() { refreshUI(); }, Qt::QueuedConnection);
	connect(PLS_PLATFORM_FACEBOOK, &PLSPlatformFacebook::privateChatChanged, this, &PLSChatDialog::facebookPrivateChatChanged, Qt::QueuedConnection);
	pls_frontend_add_event_callback(pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT, onPrismUserLogout, this);
	obs_frontend_add_event_callback(onPrismAppQuit, this);

	m_pLabelToast = new QLabel(this);
	m_pLabelToast->setObjectName("labelToast");
	dpiHelper.setFixedHeight(m_pLabelToast, 53);
	m_pLabelToast->setAlignment(Qt::AlignCenter);
	m_pLabelToast->setWordWrap(true);
	m_pLabelToast->hide();

	m_p_ButtonToastClose = new QPushButton(m_pLabelToast);
	m_p_ButtonToastClose->setObjectName("pushButtonClear");

	m_timerToastHide.setInterval(5000);
	m_timerToastHide.setSingleShot(true);
	connect(&m_timerToastHide, &QTimer::timeout, m_pLabelToast, &QWidget::hide);
	connect(m_p_ButtonToastClose, &QPushButton::clicked, m_pLabelToast, &QWidget::hide);

	ui->stackedWidget->installEventFilter(this);
}

PLSChatDialog::~PLSChatDialog()
{
	PLS_INFO(s_chatModuleName, __FUNCTION__);
	pls_frontend_remove_event_callback(pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT, onPrismUserLogout, this);
	obs_frontend_remove_event_callback(onPrismAppQuit, this);
	m_vecChatDatas.clear();
	delete ui;
}

void PLSChatDialog::setInitSize(double dpi, bool inConstructor)
{
	extern void setGeometrySys(PLSWidgetDpiAdapter * adapter, const QRect &geometry);

	const char *geometry = config_get_string(App()->GlobalConfig(), s_geometrySection, s_geometryName);
	if (!geometry || !geometry[0]) {
		const int defaultWidth = 300;
		const int mainRightOffest = 5;
		PLSMainView *mainView = App()->getMainView();
		QPoint mainTopRight = App()->getMainView()->mapToGlobal(QPoint(mainView->frameGeometry().width(), 0));

		auto chatHeight = PLSDpiHelper::calculate(dpi, mainView->frameGeometry().height() / PLSDpiHelper::getDpi(mainView));
		geometryOfNormal = QRect(mainTopRight.x() + PLSDpiHelper::calculate(dpi, mainRightOffest), mainTopRight.y(), PLSDpiHelper::calculate(dpi, defaultWidth), chatHeight);
		setGeometrySys(this, geometryOfNormal);
	} else if (inConstructor) {
		QByteArray byteArray = QByteArray::fromBase64(QByteArray(geometry));
		this->restoreGeometry(byteArray);
		if (config_get_bool(App()->GlobalConfig(), s_maxSection, s_maxName)) {
			showMaximized();
		}
	}
}

void PLSChatDialog::logoutToReInitUI()
{
	PLS_INFO(s_chatModuleName, "PLSChat Logout to reinit ui");

	ui->titleWidget->setHidden(true);

	for (int i = 0; i < m_vecChatDatas.size(); i++) {
		channelRemoveToDeleteCef((PLSChatHelper::ChatPlatformIndex)i);
	}
	changedSelectIndex(PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP);
	for (int i = 0; i < m_vecChatDatas.size(); i++) {
		if (m_vecChatDatas[i].widget == nullptr) {
			continue;
		}
		m_vecChatDatas[i].button->setHidden(m_selectIndex != i);
	}
}

void PLSChatDialog::setupFirstUI(PLSDpiHelper dpiHelper)
{
	this->setWindowTitle(tr("chat.title"));
	ui->scrollArea->verticalScrollBar()->setDisabled(true);
	ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	ui->titleWidget->setHidden(true);
	ui->titleWidget->setCursor(Qt::ArrowCursor); //PRISM/Zhangdewen/20200921/#/fix cursor show problem
	ui->scrollArea->setCursor(Qt::CursorShape::ArrowCursor);

	//PRISM/Zhangdewen/20200921/#/add chat source button
	m_chatSourceButtonOnePlatform = createChatSourceButton(ui->titleWidget, false);
	ui->horizontalLayoutTitleWidget->addWidget(m_chatSourceButtonOnePlatform);

	m_vecChatDatas.clear();
	QString addStyle = "";
	for (int i = 0; i <= PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine; i++) {
		QWidget *widget = nullptr;
		switch (i) {
		case PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP: {
			QWidget *rtmpWidget = new QWidget();
			rtmpWidget->setObjectName("rtmpWidget");
			rtmpWidget->setCursor(Qt::ArrowCursor); //PRISM/Zhangdewen/20200921/#/fix cursor show problem
			setupFirstRtmpUI(rtmpWidget);
			widget = rtmpWidget;
		} break;
		default:
			break;
		}

		QPushButton *tapButton = new QPushButton("", ui->scrollAreaWidgetContents);
		auto smallName = PLS_CHAT_HELPER->getString((PLSChatHelper::ChatPlatformIndex)i, true);
		auto objectName = QString(smallName).append("btn");
		tapButton->setObjectName(objectName);
		connect(tapButton, &QPushButton::clicked, [=]() { changedSelectIndex((PLSChatHelper::ChatPlatformIndex)i); });
		ui->scrollAreaWidgetContents->layout()->addWidget(tapButton);
		tapButton->setHidden(true);
		string showUrl = PLS_CHAT_HELPER->getChatUrlWithIndex((PLSChatHelper::ChatPlatformIndex)i, QVariantMap());
		m_vecChatDatas.push_back({widget, tapButton, showUrl});

		if (i != PLSChatHelper::ChatPlatformIndex::ChatIndexAll && i != PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP && i != PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine) {
			tapButton->setStyleSheet(PLS_CHAT_HELPER->getTabButtonCss(objectName, smallName));
		} else {
			tapButton->setText(PLS_CHAT_HELPER->getString((PLSChatHelper::ChatPlatformIndex)i));
		}
	}
	m_rightHorizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	ui->scrollAreaWidgetContents->layout()->addItem(m_rightHorizontalSpacer);

	setSelectIndex(PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine);

	QMetaObject::invokeMethod(
		this, [=]() { refreshUI(); }, Qt::QueuedConnection);
}

void PLSChatDialog::refreshUI()
{
	if (qobject_cast<PLSMainView *>(pls_get_main_view())->isClosing() || m_vecChatDatas.empty()) {
		PLS_INFO(s_chatModuleName, "refreshUI stoped, because mainview is closing");
		return;
	}

	updateRtmpPlaceText();
	if (!m_isForceRefresh && PLS_PLATFORM_API->isLiving()) {
		return;
	}
	m_isForceRefresh = false;

	refreshTabButtonCount();
	hideOrShowTabButton();
	updateTabPolicy();

	for (int i = 0; i < m_vecChatDatas.size(); i++) {
		ChatDatas &data = m_vecChatDatas[i];
		if (data.isWebLoaded && data.button->isHidden()) {
			channelRemoveToDeleteCef((PLSChatHelper::ChatPlatformIndex)i);
		}
	}
}

void PLSChatDialog::refreshTabButtonCount()
{
	for (auto data : m_vecChatDatas) {
		data.button->setHidden(true);
	}

	for (const auto &info : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		if (info.empty()) {
			continue;
		}

		PLSChatHelper::ChatPlatformIndex index = PLS_CHAT_HELPER->getIndexFromInfo(info);
		if (PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine == index || PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP == index) {
			continue;
		}

		auto channelUserStatus = info.value(ChannelData::g_channelUserStatus, ChannelData::NotExist).toInt();
		updateNewUrlByIndex(index, info);
		bool willHidden = channelUserStatus != ChannelData::Enabled;
		m_vecChatDatas[index].button->setHidden(willHidden);
	}
}

void PLSChatDialog::hideOrShowTabButton()
{

	int plaltformCount = 0;
	for (auto &info : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		if (info.empty()) {
			continue;
		}

		PLSChatHelper::ChatPlatformIndex index = PLS_CHAT_HELPER->getIndexFromInfo(info);
		if (PLSChatHelper::ChatPlatformIndex::ChatIndexUnDefine == index || PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP == index) {
			continue;
		}

		plaltformCount++;
	}

	//PRISM/Zhangdewen/20200921/#/add chat source button
	m_chatSourceButtonOnePlatform->setVisible(plaltformCount == 1);

	bool isOnlyRtmp = plaltformCount <= 0;
	//PRISM/Zhangdewen/20201021/#5310/switch delay
	//move code

	bool allWillShow = plaltformCount > 1;
	m_vecChatDatas[PLSChatHelper::ChatPlatformIndex::ChatIndexAll].button->setHidden(!allWillShow);

	if (isOnlyRtmp) {
		//if only rtmp or not channel selected, change to rtmp;
		changedSelectIndex(PLSChatHelper::ChatPlatformIndex::ChatIndexRTMP);
	} else if (allWillShow && !m_vecChatDatas[PLSChatHelper::ChatPlatformIndex::ChatIndexAll].isWebLoaded) {
		//alltab first to show, change to all
		changedSelectIndex(PLSChatHelper::ChatPlatformIndex::ChatIndexAll);
	} else if (m_vecChatDatas[m_selectIndex].button->isHidden()) {
		changedSelectIndex(foundFirstShowedButton());
	} else {
		changedSelectIndex(m_selectIndex);
	}

	//PRISM/Zhangdewen/20201021/#5310/switch delay
	ui->titleWidget->setHidden(isOnlyRtmp);
}

PLSChatHelper::ChatPlatformIndex PLSChatDialog::foundFirstShowedButton()
{
	PLSChatHelper::ChatPlatformIndex index = PLSChatHelper::ChatPlatformIndex::ChatIndexAll;
	for (int i = 0; i < m_vecChatDatas.size(); i++) {
		if (!m_vecChatDatas[i].button->isHidden()) {
			index = (PLSChatHelper::ChatPlatformIndex)i;
			break;
		}
	}
	return index;
}

void PLSChatDialog::changedSelectIndex(PLSChatHelper::ChatPlatformIndex index)
{
	if (m_selectIndex == index) {
		return;
	}
	setSelectIndex(index);
	int showButtonCount = 0;
	int count = (int)m_vecChatDatas.size();
	for (int i = 0; i < count; i++) {
		if (!m_vecChatDatas[i].button->isHidden()) {
			showButtonCount++;
		}
	}

	for (int i = 0; i < count; i++) {
		QPushButton *button = m_vecChatDatas[i].button;
		if (showButtonCount <= 1) {
			button->setProperty(s_ChatStatusType, s_ChatStatusOnlyOne);
		} else if (m_selectIndex == i) {
			button->setProperty(s_ChatStatusType, s_ChatStatusSelect);
		} else {
			button->setProperty(s_ChatStatusType, s_ChatStatusNormal);
		}

		button->style()->unpolish(button);
		button->style()->polish(button);
	}
	PLS_UI_STEP(s_chatModuleName, QString("PLSChat Dialog Tab To Index %1").arg(PLS_CHAT_HELPER->getString(index)).toUtf8().constData(), ACTION_CLICK);

	//PRISM/Zhangdewen/20201021/#5310/switch delay
	for (QObject *child : ui->stackedWidget->children()) {
		if (!child->isWidgetType()) {
			continue;
		}
		if (QWidget *childWidget = dynamic_cast<QWidget *>(child); childWidget && !childWidget->isWindow()) {
			ui->stackedWidget->removeWidget(childWidget);
			childWidget->hide();
		}
	}

	PLSChatHelper::ChatPlatformIndex _selIndex = (PLSChatHelper::ChatPlatformIndex)m_selectIndex;
	QMetaObject::invokeMethod(
		this,
		[this, _selIndex]() {
			if (this->m_vecChatDatas.empty()) {
				return;
			}
			auto &data = this->m_vecChatDatas[_selIndex];
			auto ce = data.widget;
			if (ce == nullptr && PLS_CHAT_HELPER->isCefWidgetIndex(_selIndex)) {
				if (!data.isWebLoaded) {
					data.isWebLoaded = true;
					data.widget = createANewCefWidget(data.url, _selIndex);
					ce = data.widget;
				}
			}
			if (ce == nullptr) {
				return;
			}
			switchStackWidget(_selIndex);
		},
		Qt::QueuedConnection);
}

void PLSChatDialog::setupFirstRtmpUI(QWidget *parent)
{
	QSpacerItem *rtmpSpacerTop = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
	QLabel *chatPlaceImageLabel = new QLabel();
	chatPlaceImageLabel->setObjectName("chatPlaceImageLabel");
	chatPlaceImageLabel->setAlignment(Qt::AlignCenter);

	m_rtmpPlaceTextLabel = new QLabel();
	m_rtmpPlaceTextLabel->setObjectName("rtmpPlaceTextLabel");
	m_rtmpPlaceTextLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	m_rtmpPlaceTextLabel->setWordWrap(true);

	//PRISM/Zhangdewen/20200921/#/add chat source button
	//no channel select chat source button
	m_chatSourceButtonNoPlatform = createChatSourceButton(parent, true);

	updateRtmpPlaceText();

	QSpacerItem *rtmpSpacerBottom = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

	QVBoxLayout *rtmpVLayout = new QVBoxLayout(parent);
	rtmpVLayout->setSpacing(0);
	rtmpVLayout->setContentsMargins(0, 0, 0, 0);
	rtmpVLayout->addItem(rtmpSpacerTop);
	rtmpVLayout->addWidget(chatPlaceImageLabel);
	rtmpVLayout->addWidget(m_rtmpPlaceTextLabel);
	rtmpVLayout->addWidget(m_chatSourceButtonNoPlatform, 0, Qt::AlignCenter);
	rtmpVLayout->addItem(rtmpSpacerBottom);
}

void PLSChatDialog::setupNewUrl(PLSChatHelper::ChatPlatformIndex index, string url)
{
	if (m_vecChatDatas.empty() || index < 0 || index > m_vecChatDatas.size()) {
		return;
	}

	if (url == m_vecChatDatas[index].url) {
		return;
	}
	QString oldUrl = QString::fromStdString(m_vecChatDatas[index].url);
	m_vecChatDatas[index].url = url;
	QCefWidget *ce = dynamic_cast<QCefWidget *>(m_vecChatDatas[index].widget.data());
	if (ce == nullptr) {
		return;
	}
	ce->setURL(url);
	//set url again, sometimes set new url, the cef will not refresh, so set it again.
	//the cef init will cause some time, when init not complect, set url will failed.
	QTimer::singleShot(100, ce, [=] { ce->setURL(url); });
	PLS_INFO(s_chatModuleName, "PLSChat Dialog %s set new url:%s", PLS_CHAT_HELPER->getString(index), url.c_str(), ACTION_CLICK);
}

int PLSChatDialog::rtmpChannelCount()
{
	int count = 0;
	for (auto &info : PLSCHANNELS_API->getCurrentSelectedChannels()) {
		if (info.empty()) {
			continue;
		}
		auto dataType = info.value(ChannelData::g_data_type, ChannelData::RTMPType).toInt();
		if (dataType == ChannelData::RTMPType) {
			count++;
		}
	}
	return count;
}

QCefWidget *PLSChatDialog::createANewCefWidget(const string &url, PLSChatHelper::ChatPlatformIndex index)
{
	if (!PLS_CHAT_HELPER->isCefWidgetIndex(index)) {
		return nullptr;
	}
	PLS_INFO(s_chatModuleName, "PLSChat Dialog %s create cef with new url:%s", PLS_CHAT_HELPER->getString(index), url.c_str());
	std::map<std::string, std::string> mapData;
	QCefWidget *cefWidget = nullptr;

	auto name = PLS_CHAT_HELPER->getPlatformNameFromIndex(index);
	if (index == PLSChatHelper::ChatPlatformIndex::ChatIndexAll) {
		name = NAVER_TV;
	}
	chat_panel_cookies = PLSBasic::getBrowserPannelCookieMgr(name);
	cefWidget = cef->create_widget(nullptr, url, (PLS_CHAT_HELPER->isLocalHtmlPage(index) ? "sendToPrism('{\"type\":\"onReady\", \"data\": {}}')" : pls_get_offline_javaScript()),
				       chat_panel_cookies, mapData);

	if (PLS_CHAT_HELPER->isLocalHtmlPage(index)) {
		cefWidget->installEventFilter(this);
	}

	return cefWidget;
}

void PLSChatDialog::channelRemoveToDeleteCef(PLSChatHelper::ChatPlatformIndex index)
{
	ChatDatas &data = m_vecChatDatas[index];
	data.url = PLS_CHAT_HELPER->getChatUrlWithIndex(index, QVariantMap());
	data.isWebLoaded = false;
	QCefWidget *cefWidget = dynamic_cast<QCefWidget *>(data.widget.data());
	if (cefWidget == nullptr) {
		return;
	}
	data.widget = nullptr;
	ui->stackedWidget->removeWidget(cefWidget);
	delete cefWidget;
	PLS_INFO(s_chatModuleName, "PLSChat Dialog" __FUNCTION__ " index:%s", PLS_CHAT_HELPER->getString(index));
}

void PLSChatDialog::showEvent(QShowEvent *event)
{
	PLS_INFO(s_chatModuleName, "PLSChat Dialog" __FUNCTION__);
	PLSDialogView::showEvent(event);
	updateTabPolicy();

	pls_window_right_margin_fit(this);
	config_set_bool(App()->GlobalConfig(), s_hiddenSection, s_hiddenName, false);
	config_save(App()->GlobalConfig());
	emit chatShowOrHide(true);

	if (m_vecChatDatas.size() > m_selectIndex) {
		QCefWidget *ce = dynamic_cast<QCefWidget *>(m_vecChatDatas[m_selectIndex].widget.data());
		if (ce == nullptr || ce->isHidden()) {
			return;
		}
		QSize originSize = this->frameGeometry().size();
		QSize newSize = QSize(this->frameGeometry().width(), originSize.height() + 1);
		resize(newSize);
		resize(originSize);
	}
}

void PLSChatDialog::hideEvent(QHideEvent *event)
{
	PLS_INFO(s_chatModuleName, "PLSChat Dialog" __FUNCTION__);
	config_set_bool(App()->GlobalConfig(), s_hiddenSection, s_hiddenName, true);
	if (!getMaxState()) {
		onSaveNormalGeometry();
	}
	config_save(App()->GlobalConfig());
	PLSDialogView::hideEvent(event);
	emit chatShowOrHide(false);
}

void PLSChatDialog::closeEvent(QCloseEvent *event)
{
	PLS_INFO(s_chatModuleName, "PLSChat Dialog" __FUNCTION__);
	//can't emit close event, otherwise the cef brower will all deleted.
	hide();
	event->ignore();
}

void PLSChatDialog::updateRtmpPlaceText()
{
	m_rtmpPlaceTextLabel->setText(PLS_CHAT_HELPER->getRtmpPlaceholderString());
	//PRISM/Zhangdewen/20200921/#/add chat source button
	//m_chatSourceButtonNoPlatform->setVisible(PLSCHANNELS_API->getCurrentSelectedChannels().isEmpty());
}

void PLSChatDialog::updateNewUrlByIndex(PLSChatHelper::ChatPlatformIndex index, const QVariantMap &info)
{
	switch (index) {
	case PLSChatHelper::ChatPlatformIndex::ChatIndexTwitch:
	case PLSChatHelper::ChatPlatformIndex::ChatIndexYoutube: {
		setupNewUrl(index, PLS_CHAT_HELPER->getChatUrlWithIndex(index, info));
	} break;
	default:
		break;
	}
}

void PLSChatDialog::onMaxFullScreenStateChanged()
{
	config_set_bool(App()->GlobalConfig(), s_maxSection, s_maxName, isMaxState);
	config_save(App()->GlobalConfig());
}

void PLSChatDialog::onSaveNormalGeometry()
{
	config_set_string(App()->GlobalConfig(), s_geometrySection, s_geometryName, saveGeometry().toBase64().constData());
	config_save(App()->GlobalConfig());
}

bool PLSChatDialog::eventFilter(QObject *watched, QEvent *event)
{
	if (ui->stackedWidget == watched) {
		if (event->type() == QEvent::Resize) {
			adjustToastSize();
		}
	}
	return false;
}

void PLSChatDialog::wheelEvent(QWheelEvent *event)
{
	QPoint numDegrees = event->angleDelta() / 8;
	bool forwartStep = numDegrees.y() > 0;
	auto bar = ui->scrollArea->horizontalScrollBar();
	bar->setValue(bar->value() + (forwartStep ? -1 : 1) * 50);
	event->accept();
}

void PLSChatDialog::adjustToastSize()
{
	int spaceWidth = PLSDpiHelper::calculate(this, 10);
	int closeTop = PLSDpiHelper::calculate(this, 5);
	int closeLeft = PLSDpiHelper::calculate(this, 21);
	m_pLabelToast->setFixedWidth(ui->stackedWidget->width() - 2 * spaceWidth);
	m_pLabelToast->move(ui->stackedWidget->mapTo(this, {spaceWidth, spaceWidth}));
	m_p_ButtonToastClose->move(m_pLabelToast->width() - closeLeft, closeTop);
}

void PLSChatDialog::updateTabPolicy()
{
	int showButtonCount = 0;
	for (int i = 0; i < m_vecChatDatas.size(); i++) {
		const ChatDatas &data = m_vecChatDatas[i];
		if (data.button->isVisible()) {
			showButtonCount++;
		}
	}

	m_rightHorizontalSpacer->changeSize(0, 20, showButtonCount <= 1 ? QSizePolicy ::Expanding : QSizePolicy::Fixed);
	ui->leftPlaceholder->setHidden(showButtonCount > 1);
	ui->scrollAreaWidgetContents->style()->unpolish(ui->scrollAreaWidgetContents);
	ui->scrollAreaWidgetContents->style()->polish(ui->scrollAreaWidgetContents);
}

void PLSChatDialog::showToastIfNeeded()
{
	QString showStr("");
	if (!PLS_CHAT_HELPER->showToastWhenStart(showStr)) {
		return;
	}
	m_pLabelToast->setText(showStr);
	adjustToastSize();
	m_pLabelToast->show();
	m_timerToastHide.start();
}

void PLSChatDialog::updateYoutubeUrlIfNeeded()
{
	auto index = PLSChatHelper::ChatPlatformIndex::ChatIndexYoutube;
	QVariantMap &info = QVariantMap();
	PLS_CHAT_HELPER->getSelectInfoFromIndex(index, info);
	if (!info.isEmpty()) {
		updateNewUrlByIndex(index, info);
	}
}

void PLSChatDialog::setSelectIndex(PLSChatHelper::ChatPlatformIndex index)
{
	m_selectIndex = index;
	if (!m_vecChatDatas[index].isWebLoaded) {
		return;
	}
	PLS_CHAT_HELPER->sendWebShownEventIfNeeded(m_selectIndex);
}

//PRISM/Zhangdewen/20200921/#/add chat source button
QWidget *PLSChatDialog::createChatSourceButton(QWidget *parent, bool noPlatform)
{
	if (noPlatform) {
		return new ChatSourceButton(tr("Chat.SourceButton.NoPlatform"), parent, [this]() { chatSourceButtonClicked(); });
	}

	QWidget *widget = new QWidget(parent);
	QWidget *button = new ChatSourceButton(tr("Chat.SourceButton.OnePlatform"), widget, [this]() { chatSourceButtonClicked(); });
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setContentsMargins(0, 0, 12, 0);
	layout->setSpacing(0);
	layout->addWidget(button);
	return widget;
}

void PLSChatDialog::switchStackWidget(PLSChatHelper::ChatPlatformIndex index)
{
	//when switch to the cefwidget, which is the first loaded, in some computer maybe will delay a short time to show the cef page.
	//so when switch to other page, first to remove all sub view from superview.
	for (QObject *child : ui->stackedWidget->children()) {
		if (!child->isWidgetType()) {
			continue;
		}
		if (QWidget *childWidget = dynamic_cast<QWidget *>(child); childWidget && !childWidget->isWindow()) {
			ui->stackedWidget->removeWidget(childWidget);
		}
	}
	auto widget = m_vecChatDatas[index].widget;
	if (widget == nullptr) {
		return;
	}
	ui->stackedWidget->addWidget(widget);
	ui->stackedWidget->setCurrentWidget(widget);
	//PRISM/Zhangdewen/20200921/#/add chat source button, dynamic flush css
	PLSDpiHelper::dpiDynamicUpdate(widget);
}

void PLSChatDialog::facebookPrivateChatChanged(bool oldPrivate, bool newPrivate)
{
	if (oldPrivate == false && newPrivate == true) {
		m_pLabelToast->setText(tr("facebook.living.chat.Private"));
		adjustToastSize();
		m_pLabelToast->show();
		m_timerToastHide.start();
	} else {
		m_pLabelToast->hide();
	}
}

//PRISM/Zhangdewen/20200921/#/add chat source button
void PLSChatDialog::chatSourceButtonClicked()
{
	PLSBasic::Get()->AddSource(PRISM_CHAT_SOURCE_ID);
}
