#include "main-view.hpp"
#include "ui_PLSMainView.h"

#include <QResizeEvent>
#include <QPainter>
#include <QStyle>
#include <QWidgetAction>
#include <QLabel>
#include <QKeySequence>
#include <QLabel>
#include <qdatetime.h>
#include <qdesktopwidget.h>

#include "log.h"
#include "action.h"
#include "log/module_names.h"
#include "pls-app.hpp"
#include "window-basic-main.hpp"
#include "toast-view.hpp"
#include "PLSChannelsVirualAPI.h"
#include "custom-help-menu-item.hpp"
#include "frontend-api.h"
#include "PLSLivingMsgView.hpp"
#include "PLSToastMsgPopup.hpp"
#include "login-common-helper.hpp"
#include "pls-common-define.hpp"
#include "PLSToplevelView.hpp"
#include "PLSBlockDump.h"

#if defined(Q_OS_WINDOWS)
#include <Windows.h>
#endif // Q_OS_WINDOWS

#include "PLSPlatformApi.h"
#include <QTimer>

#define LIVINGMSGVIEW "LivingMsgView"
#define MAINVIEWMODE "mainViewMode"

#define EMAIL_TYPE QStringLiteral("email")

enum HelpWidgetItemTag { PrismFAQ = 100, PrismWebsite, ContactUs, CheckForUpdate };

extern QRect getScreenRect(QWidget *widget);
extern QRect getScreenAvailableRect(QWidget *widget);

void showChatView(bool isRebackLogin, bool isOnlyShow, bool isOnlyInit);

static inline QPoint operator-(const QPoint &pt, const QSize &size)
{
	return QPoint(pt.x() - size.width(), pt.y() - size.height());
}
template<typename Type> static void limitResize(int min, int max, Type &begin, Type &end)
{
	int distance = end - begin;
	if ((min > 0) && (min < QWIDGETSIZE_MAX)) {
		if (std::abs(distance) < min) {
			end = distance > 0 ? (begin + min) : (begin - min);
		}
	}

	if ((max > 0) && (max < QWIDGETSIZE_MAX)) {
		if (std::abs(distance) > max) {
			end = distance > 0 ? (begin + max) : (begin - max);
		}
	}
}

PLSMainView::PLSMainView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSToplevelView<QFrame>(parent, Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint), ui(new Ui::PLSMainView)
{
	dpiHelper.setCss(this, {PLSCssIndex::QCheckBox,
				PLSCssIndex::QLineEdit,
				PLSCssIndex::QMenu,
				PLSCssIndex::QPlainTextEdit,
				PLSCssIndex::QPushButton,
				PLSCssIndex::QRadioButton,
				PLSCssIndex::QScrollBar,
				PLSCssIndex::QSlider,
				PLSCssIndex::QSpinBox,
				PLSCssIndex::QTableView,
				PLSCssIndex::QTabWidget,
				PLSCssIndex::QTextEdit,
				PLSCssIndex::QToolButton,
				PLSCssIndex::QToolTip,
				PLSCssIndex::QComboBox,
				PLSCssIndex::PLSDock,
				PLSCssIndex::PLSMainView,
				PLSCssIndex::ChannelCapsule,
				PLSCssIndex::PLSChannelsArea,
				PLSCssIndex::ChannelConfigPannel,
				PLSCssIndex::DefaultPlatformAddList,
				PLSCssIndex::GoLivePannel,
				PLSCssIndex::PLSRTMPConfig});
	dpiHelper.setMinimumSize(this, {707, 633});
	dpiHelper.setInitSize(this, {1310, 802});

	ui->setupUi(this);
	setMouseTracking(true);
	ui->titleBar->setMouseTracking(true);
	ui->body->setMouseTracking(true);
	ui->content->setMouseTracking(true);
	ui->rightArea->setMouseTracking(true);
	ui->bottomArea->setMouseTracking(true);
	ui->channelsArea->setMouseTracking(true);
	ui->channelsArea->installEventFilter(this);
	ui->content->installEventFilter(this);
	ui->beauty->setToolTip(QTStr("main.beauty.title"));
	ui->bgmBtn->setToolTip(QTStr("Bgm.Title"));
	ui->stickers->setToolTip(QTStr("main.giphy.title"));
	ui->verticalLayout->setAlignment(ui->alert, Qt::AlignHCenter);
	ui->verticalLayout->setAlignment(ui->stickers, Qt::AlignHCenter);
	ui->verticalLayout->setAlignment(ui->label_separate, Qt::AlignHCenter);
	ui->alert->setToolTip(QTStr("main.rightbar.alert.tooltip"));

	captionHeight = captionButtonSize = GetSystemMetrics(SM_CYCAPTION);
	captionButtonMargin = 0;
	logoMarginLeft = 8;
	menuButtonMarginLeft = 16;
	menuButtonSize = QSize(34, 26);
	closeButtonMarginRight = 8;
	rightAreaWidth = 30;
	bottomAreaHeight = 30;
	channelsAreaHeight = 70;
	closeEventCallback = [this](QCloseEvent *e) { callBaseCloseEvent(e); };
	m_livingToastViewShow = false;

	helpMenu = new QMenu(this);
	helpMenu->setObjectName("helpMenu");
	QListWidget *listWidget = new QListWidget(this);
	QObject::connect(listWidget, &QListWidget::itemClicked, this, &PLSMainView::on_listWidget_itemClicked);
	listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	listWidget->setObjectName("helpListWidget");
	listWidget->setSelectionMode(QAbstractItemView::NoSelection);
	listWidget->setFocusPolicy(Qt::NoFocus);
	QList<QString> titleList;
	titleList << QTStr("MainFrame.SideBar.Help.PrismFAQ") << QTStr("MainFrame.SideBar.Help.PrismWebsite");
	for (int i = 0; i < titleList.size(); i++) {
		QListWidgetItem *item = new QListWidgetItem();
		item->setData(Qt::UserRole, PrismFAQ + i);
		CustomHelpMenuItem *widget = new CustomHelpMenuItem(titleList[i]);
		if (i == titleList.size() - 1) {
			checkMenuItem = widget;
			checkListWidgetItem = item;
		}
		listWidget->addItem(item);
		listWidget->setItemWidget(item, widget);
	}
	QHBoxLayout *layout = new QHBoxLayout(listWidget);
	listWidget->setLayout(layout);
	QWidgetAction *action = new QWidgetAction(helpMenu);
	action->setDefaultWidget(listWidget);
	helpMenu->addAction(action);
	QObject::connect(helpMenu, &QMenu::aboutToHide, this, &PLSMainView::helpMenuAboutToHide);

	toast = new PLSToastView(this);
	m_toastMsg = new PLSToastMsgPopup(this);
	m_toastMsg->hide();
	//m_livingMsgView.hide();
	QObject::connect(ui->min, &QToolButton::clicked, this, [this]() {
		PLS_UI_STEP(MAINFRAME_MODULE, "Main Frame Title Bar's Minimized Button", ACTION_CLICK);
		showMinimized();
	});
	QObject::connect(ui->maxres, &QToolButton::clicked, this, [this]() {
		PLS_UI_STEP(MAINFRAME_MODULE, "Main Frame Title Bar's Maximized Button", ACTION_CLICK);
		if (!isMaxState && !isFullScreenState) {
			showMaximized();
		} else {
			showNormal();
		}
	});
	QObject::connect(ui->close, &QToolButton::clicked, this, [this]() {
		PLS_UI_STEP(MAINFRAME_MODULE, "Main Frame Title Bar's Close Button", ACTION_CLICK);
		close();
	});
	QObject::connect(ui->alert, &PLSToastButton::clickButton, [=]() { on_alert_clicked(); });
	QObject::connect(&m_livingMsgView, &PLSLivingMsgView::hideSignal, [=]() { ui->alert->setShowAlert(false); });

	QObject::connect(PLS_PLATFORM_API, &PLSPlatformApi::enterLivePrepareState, this, [=](bool disable) { ui->settings->setEnabled(!disable); });

	//default the tooltip only show the active window, so need to change tooltip default action, modify the widget window attribute
	this->setAttribute(Qt::WA_AlwaysShowToolTips, true);
}

PLSMainView::~PLSMainView()
{
	pls_frontend_remove_event_callback(pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT, PLSMainView::resetToastViewPostion, this);
	delete ui;
}

QWidget *PLSMainView::content() const
{
	return ui->content;
}

QWidget *PLSMainView::channelsArea() const
{
	return ui->channelsArea;
}

QPushButton *PLSMainView::menuButton() const
{
	return ui->menu;
}

PLSBasicStatusBar *PLSMainView::statusBar() const
{
	return ui->bottomArea;
}

int PLSMainView::getCaptionHeight() const
{
	return captionHeight;
}

void PLSMainView::setCaptionHeight(int captionHeight)
{
	this->captionHeight = captionHeight - BORDER_WIDTH();
	ui->titleBar->setFixedHeight(captionHeight);
}

int PLSMainView::getCaptionButtonSize() const
{
	return captionButtonSize;
}

void PLSMainView::setCaptionButtonSize(int captionButtonSize)
{
	this->captionButtonSize = captionButtonSize;
	ui->min->setFixedSize(captionButtonSize, captionButtonSize);
	ui->maxres->setFixedSize(captionButtonSize, captionButtonSize);
	ui->close->setFixedSize(captionButtonSize, captionButtonSize);
}

int PLSMainView::getCaptionButtonMargin() const
{
	return captionButtonMargin;
}

void PLSMainView::setCaptionButtonMargin(int captionButtonMargin)
{
	this->captionButtonMargin = captionButtonMargin;
	ui->titleButtonLayout->setSpacing(captionButtonMargin);
}

int PLSMainView::getLogoMarginLeft() const
{
	return logoMarginLeft;
}

void PLSMainView::setLogoMarginLeft(int logoMarginLeft)
{
	this->logoMarginLeft = logoMarginLeft;
	//updateLayout(this->size());
}

int PLSMainView::getMenuButtonMarginLeft() const
{
	return menuButtonMarginLeft;
}

void PLSMainView::setMenuButtonMarginLeft(int menuButtonMarginLeft)
{
	this->menuButtonMarginLeft = menuButtonMarginLeft;
	QMargins margins = ui->titleBarLayout->contentsMargins();
	margins.setLeft(menuButtonMarginLeft);
	ui->titleBarLayout->setContentsMargins(margins);
	PLSDpiHelper::setDynamicContentsMargins(ui->titleBarLayout, true);
}

QSize PLSMainView::getMenuButtonSize() const
{
	return menuButtonSize;
}

void PLSMainView::setMenuButtonSize(const QSize &menuButtonSize)
{
	this->menuButtonSize = menuButtonSize;
	ui->menu->setFixedSize(menuButtonSize);
}

int PLSMainView::getCloseButtonMarginRight() const
{
	return closeButtonMarginRight;
}

void PLSMainView::setCloseButtonMarginRight(int closeButtonMarginRight)
{
	this->closeButtonMarginRight = closeButtonMarginRight;
	QMargins margins = ui->titleBarLayout->contentsMargins();
	margins.setRight(closeButtonMarginRight);
	ui->titleBarLayout->setContentsMargins(margins);
	PLSDpiHelper::setDynamicContentsMargins(ui->titleBarLayout, true);
}

int PLSMainView::getRightAreaWidth() const
{
	return rightAreaWidth;
}

void PLSMainView::setRightAreaWidth(int rightAreaWidth)
{
	this->rightAreaWidth = rightAreaWidth;
	ui->rightArea->setFixedWidth(this->rightAreaWidth);
}

int PLSMainView::getBottomAreaHeight() const
{
	return bottomAreaHeight;
}

void PLSMainView::setBottomAreaHeight(int bottomAreaHeight)
{
	this->bottomAreaHeight = bottomAreaHeight;
	ui->bottomArea->setFixedHeight(this->bottomAreaHeight);
}

int PLSMainView::getChannelsAreaHeight() const
{
	return channelsAreaHeight;
}

void PLSMainView::setChannelsAreaHeight(int channelsAreaHeight)
{
	this->channelsAreaHeight = channelsAreaHeight;
	ui->channelsArea->setFixedHeight(this->channelsAreaHeight);
}

void PLSMainView::setCloseEventCallback(std::function<void(QCloseEvent *)> closeEventCallback)
{
	this->closeEventCallback = closeEventCallback;
}

void PLSMainView::callBaseCloseEvent(QCloseEvent *event)
{
	ToplevelView::closeEvent(event);
}

void PLSMainView::setStudioMode(bool studioMode)
{
	ui->studioMode->setChecked(studioMode);
}

void PLSMainView::toastMessage(pls_toast_info_type type, const QString &message, int autoClose)
{
	PLS_INFO(NOTICE_MODULE, QString("a new toast message:%1").arg(message).toUtf8().data());

	qint64 currtime = QDateTime::currentMSecsSinceEpoch();
	toastMessages.insert(currtime, message);
	ui->alert->setNum(toastMessages.size());

	m_toastMsg->showMsg(message, static_cast<pls_toast_info_type>(type));
	int width = ui->content->width();
	QPoint pt = ui->content->mapToGlobal(QPoint(width, 0));
	pt = mapFromGlobal(pt);
	m_toastMsg->show();
	m_toastMsg->move(pt.x() - m_toastMsg->width() - PLSDpiHelper::calculate(this, 10), pt.y() + PLSDpiHelper::calculate(this, 10));

	//add toast msg view
	m_livingMsgView.addMsgItem(message, currtime, type);
}

void PLSMainView::toastMessage(pls_toast_info_type type, const QString &message, const QString &url, const QString &replaceStr, int autoClose)
{
	QString msg(message);
	toastMessage(type, m_livingMsgView.getInfoWithUrl(message, url, replaceStr), autoClose);
	m_toastMsg->showMsg(msg.replace(url, replaceStr), static_cast<pls_toast_info_type>(type));
}

void PLSMainView::toastClear()
{
	toastMessages.clear();
	ui->alert->setNum(toastMessages.size());
	m_toastMsg->hide();
	m_livingMsgView.clearMsgView();
}

void PLSMainView::setUserButtonIcon(const QIcon &icon)
{
	ui->user->setIcon(icon);
}

void PLSMainView::initToastMsgView(bool isInitShow)
{
	static bool registerOne = true;
	if (registerOne) {
		pls_frontend_add_event_callback(pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT, PLSMainView::resetToastViewPostion, this);
		registerOne = false;
	}
	if (!m_isFirstLogin) {
		return;
	}

	m_livingMsgView.initMsgGeometry();

	bool isVisible = config_get_bool(App()->GlobalConfig(), LIVINGMSGVIEW, "showMode");
	ui->alert->setShowAlert(isVisible && isInitShow);
	m_livingMsgView.setShow(isVisible && isInitShow);
	m_isFirstLogin = false;
}

void PLSMainView::InitBeautyView()
{
	bool show = config_get_bool(App()->GlobalConfig(), BEAUTY_CONFIG, "showMode");
	RefreshBeautyButtonStyle(show);

	emit setBeautyViewVisible(show);
}

void PLSMainView::CreateBeautyView()
{
	emit CreateBeautyInstance();
}

void PLSMainView::OnBeautyViewVisibleChanged(bool visible)
{
	RefreshBeautyButtonStyle(visible);
}

void PLSMainView::RefreshBeautyButtonStyle(bool state)
{
	ui->beauty->setProperty("showMode", state);
	pls_flush_style(ui->beauty);
}

void PLSMainView::InitBgmView()
{
	bool show = config_get_bool(App()->GlobalConfig(), BGM_CONFIG, "showMode");
	pls_flush_style(ui->bgmBtn, "showMode", show);

	emit setBgmViewVisible(show);
}

void PLSMainView::OnBgmViewVisibleChanged(bool visible)
{
	pls_flush_style(ui->bgmBtn, "showMode", visible);
}

void PLSMainView::RefreshStickersButtonStyle(bool state)
{
	ui->stickers->setProperty("showMode", state);
	pls_flush_style(ui->stickers);
}

PLSToastButton *PLSMainView::getToastButton()
{
	return ui->alert;
}
void PLSMainView::showMainFloatView()
{
	//show toast view ;chat view
	initToastMsgView();
	if (!config_get_bool(App()->GlobalConfig(), LIVINGMSGVIEW, "showMode")) {
		if (m_livingToastViewShow) {
			m_livingMsgView.setShow(true);
		} else {
			m_livingMsgView.setShow(false);
		}
	} else {
		m_livingMsgView.setShow(true);
	}

	if (!m_livingToastViewShow) {
		m_livingMsgView.setShow(false);
	} else {
	}

	if (m_IsChatShowWhenRebackLogin) {
		//show chat view
		showChatView(false, true);
	}
}
void PLSMainView::hideMainFloatView()
{
	//hide toast view ;chat view
	m_livingToastViewShow = m_livingMsgView.isVisible();
	m_livingMsgView.hide();

	m_IsChatShowWhenRebackLogin = !config_get_bool(App()->GlobalConfig(), "Basic", "chatIsHidden");
	showChatView(true);
}

QRect PLSMainView::titleBarRect() const
{
	return ui->titleBar->geometry();
}

bool PLSMainView::canMaximized() const
{
	return true;
}

bool PLSMainView::canFullScreen() const
{
	return true;
}

void PLSMainView::flushMaxFullScreenStateStyle()
{
	pls_flush_style(ui->maxres);
}

void PLSMainView::onMaxFullScreenStateChanged()
{
	config_set_bool(App()->GlobalConfig(), "BasicWindow", "MaximizedState", isMaxState);
	config_set_bool(App()->GlobalConfig(), "BasicWindow", "FullScreenState", isFullScreenState);
	config_save_safe(App()->GlobalConfig(), "tmp", nullptr);
	emit maxFullScreenStateChanged(isMaxState, isFullScreenState);
}

void PLSMainView::onSaveNormalGeometry()
{
	config_set_string(App()->GlobalConfig(), "BasicWindow", "geometry", saveGeometry().toBase64().constData());
	config_save_safe(App()->GlobalConfig(), "tmp", nullptr);
}

bool PLSMainView::isClosing() const
{
	return closing;
}
void PLSMainView::close()
{
	if (!closing) {
		closing = true;
		ToplevelView::close();
	}
}

void PLSMainView::OnBeautySourceDownloadFinished()
{
	emit BeautySourceDownloadFinished();
}

void PLSMainView::onUpdateChatSytle(bool isShow)
{
	ui->chat->setProperty("hasShowChat", isShow);
	LoginCommonHelpers::refreshStyle(ui->chat);
}

void PLSMainView::resetToastViewPostion(pls_frontend_event event, const QVariantList &params, void *context)
{
	if (event == pls_frontend_event::PLS_FRONTEND_EVENT_PRISM_LOGOUT) {
		config_set_string(App()->GlobalConfig(), "LivingMsgView", "geometry", "");
		config_set_bool(App()->GlobalConfig(), "LivingMsgView", "showMode", false);
		static_cast<PLSMainView *>(pls_get_main_view())->m_livingToastViewShow = false;
		config_set_bool(App()->GlobalConfig(), "General", "Watermark", true);
		config_save(App()->GlobalConfig());
	}
}

void PLSMainView::helpMenuAboutToHide()
{
	ui->help->setChecked(false);
}

void PLSMainView::on_menu_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Menu Button", ACTION_CLICK);
}

void PLSMainView::on_user_clicked()
{
	if (!ui->settings->isEnabled()) {
		return;
	}
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar User Button", ACTION_CLICK);
	emit popupSettingView(QStringLiteral("General"), QStringLiteral("user"));
}

void PLSMainView::on_studioMode_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Studio Mode Button", ACTION_CLICK);
	emit studioModeChanged();
}

void PLSMainView::on_chat_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Chat Button", ACTION_CLICK);
	showChatView(false, true);
}

void PLSMainView::on_alert_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Alert Button", ACTION_CLICK);
	m_livingMsgView.setShow(!m_livingMsgView.isVisible());
}

void PLSMainView::on_help_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Help Button", ACTION_CLICK);
	bool isChecked = ui->help->isChecked();
	if (isChecked) {
		PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
		checkMenuItem->setBadgeVisible(main->getUpdateResult() == MainCheckUpdateResult::HasUpdate);
		bool disabled = pls_is_living_or_recording();
		if (disabled) {
			checkListWidgetItem->setFlags(checkListWidgetItem->flags() & ~Qt::ItemIsEnabled);
		} else {
			checkListWidgetItem->setFlags(checkListWidgetItem->flags() | Qt::ItemIsEnabled);
		}
		checkMenuItem->setItemDisabled(disabled);
		helpMenu->exec(QCursor::pos());
	} else {
		helpMenu->setHidden(true);
	}
}

void PLSMainView::on_settings_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Setting Button", ACTION_CLICK);
	emit popupSettingView(QStringLiteral("General"), QString());
	ui->settings->setChecked(false);
}

void PLSMainView::on_listWidget_itemClicked(QListWidgetItem *item)
{
	helpMenu->setHidden(true);
	int tagInt = item->data(Qt::UserRole).toInt();
	if (tagInt == PrismFAQ) {
		PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Help MenuItemList PrismFAQ Button", ACTION_CLICK);
		QMetaObject::invokeMethod(PLSBasic::Get(), "on_actionPrismFAQ_triggered");
	} else if (tagInt == PrismWebsite) {
		PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Help MenuItemList PrismWebsite Button", ACTION_CLICK);
		QMetaObject::invokeMethod(PLSBasic::Get(), "on_actionPrismWebsite_triggered");
	} 
}

void PLSMainView::on_beauty_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Beauty Button", ACTION_CLICK);

	bool show = config_get_bool(App()->GlobalConfig(), BEAUTY_CONFIG, "showMode");
	RefreshBeautyButtonStyle(!show);

	emit beautyClicked();
}

void PLSMainView::on_bgmBtn_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Music Playlist Button", ACTION_CLICK);

	bool show = config_get_bool(App()->GlobalConfig(), BGM_CONFIG, "showMode");
	pls_flush_style(ui->bgmBtn, "showMode", !show);

	emit bgmClicked();
}

void PLSMainView::on_stickers_clicked()
{
	PLS_UI_STEP(MAINFRAME_MODULE, "PLSMainView SideBar Stickers Button", ACTION_CLICK);
	bool show = config_get_bool(App()->GlobalConfig(), GIPHY_STICKERS_CONFIG, "showMode");
	RefreshStickersButtonStyle(!show);
	emit stickersClicked();
}

void PLSMainView::closeEvent(QCloseEvent *event)
{
	closing = true;

	bool isLivingView = config_get_bool(App()->GlobalConfig(), LIVINGMSGVIEW, "showMode");

	//config_set_bool(App()->GlobalConfig(), LIVINGMSGVIEW, MAINVIEWMODE, isLivingView);

	closeEventCallback(event);
}

void PLSMainView::showEvent(QShowEvent *event)
{
	ui->bottomArea->StartStatusMonitor();
	PLSBlockDump::Instance()->StartMonitor();

	emit isshowSignal(true);
	ToplevelView::showEvent(event);
}

void PLSMainView::hideEvent(QHideEvent *event)
{
	//config_set_bool(App()->GlobalConfig(), LIVINGMSGVIEW, MAINVIEWMODE, m_livingMsgView.isVisible());
	//m_livingMsgView.setShow(false);
	emit isshowSignal(false);
	if (!getMaxState() && !getFullScreenState()) {
		config_set_string(App()->GlobalConfig(), "BasicWindow", "geometry", saveGeometry().toBase64().constData());
	}
	config_save(App()->GlobalConfig());
	ToplevelView::hideEvent(event);
}

void PLSMainView::mousePressEvent(QMouseEvent *event)
{
	mousePress(event);
}

void PLSMainView::mouseReleaseEvent(QMouseEvent *event)
{
	mouseRelease(event);
}

void PLSMainView::mouseDoubleClickEvent(QMouseEvent *event)
{
	mouseDbClick(event);
}

void PLSMainView::mouseMoveEvent(QMouseEvent *event)
{
	mouseMove(event);
}

bool PLSMainView::eventFilter(QObject *watcher, QEvent *event)
{

	if (watcher == ui->channelsArea) {
		switch (event->type()) {
		case QEvent::ChildAdded:
			if (QWidget *widget = dynamic_cast<QWidget *>(static_cast<QChildEvent *>(event)->child()); widget) {
				widget->setCursor(Qt::ArrowCursor);
			}
			break;
		}
	} else if (watcher == ui->content) {
		if (event->type() == QEvent::Resize) {
			if (m_toastMsg->isVisible()) {
				int width = ui->content->width();
				QPoint pt = ui->content->mapToGlobal(QPoint(width, 0));
				pt = mapFromGlobal(pt);
				m_toastMsg->move(pt.x() - m_toastMsg->width() - PLSDpiHelper::calculate(this, 10), pt.y() + PLSDpiHelper::calculate(this, 10));
				m_toastMsg->adjustSize();
			}
		}
	}

	return ToplevelView::eventFilter(watcher, event);
}
