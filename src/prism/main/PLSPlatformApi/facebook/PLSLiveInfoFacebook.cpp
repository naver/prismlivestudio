#include "PLSLiveInfoFacebook.h"
#include <QBitmap>
#include <QPainter>
#include "ui_PLSLiveInfoFacebook.h"
#include "../PLSPlatformApi.h"
#include "../channels/ChannelsDataApi/PLSChannelDataAPI.h"
#include "../../frontend-api/frontend-api.h"
#include "../../frontend-api/alert-view.hpp"

static const char *liveInfoMoudule = "PLSLiveInfoFacebook";

#define GAMELIST_ITEM_HEIGHT 40
#define GAMELIST_BORDER 1
#define GAME_ITEM_MAX_SIZE 5
#define TITLE_MAX_LENGTH 250

#define IsTimelineObject ui->shareFirstObject->getComboBoxTitle() == TimelineObjectFlags
#define IsGroupObjectFlags ui->shareFirstObject->getComboBoxTitle() == GroupObjectFlags
#define IsPageObjectFlags ui->shareFirstObject->getComboBoxTitle() == PageObjectFlags
#define IsUpdatePage PLSCHANNELS_API->isLiving()

PLSLiveInfoFacebookListWidget::PLSLiveInfoFacebookListWidget(QWidget *parent) : WidgetDpiAdapter(parent) {}

PLSLiveInfoFacebookListWidget ::~PLSLiveInfoFacebookListWidget() {}

void PLSLiveInfoFacebook::handleRequestFunctionType(PLSAPIFacebookType type)
{
	switch (type) {
	case PLSAPIFacebookType::PLSFacebookInvalidAccessToken: {
		showLoading(content());
		PLSAlertView::Button button = PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("facebook.liveinfo.login.token.expired"));
		hideLoading();
		reject();
		if (button == PLSAlertView::Button::Ok) {
			PLSCHANNELS_API->channelExpired(PLS_PLATFORM_FACEBOOK->getChannelUUID(), false);
		}
		break;
	}
	case PLSAPIFacebookType::PLSRequestPermissionReject:
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("facebook.liveinfo.request.permission.refused"));
		break;
	case PLSAPIFacebookType::PLSLivingPermissionReject:
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("facebook.liveinfo.living.permission.refused"));
		break;
	case PLSAPIFacebookType::PLSUpdateLiveInfoFailed:
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("facebook.liveinfo.golive.update.liveInfo.failed"));
		break;
	default:
		break;
	}
}

bool PLSLiveInfoFacebookListWidget::needQtProcessDpiChangedEvent() const
{
	return false;
}

PLSLiveInfoFacebook::PLSLiveInfoFacebook(PLSPlatformBase *pPlatformBase, QWidget *parent, PLSDpiHelper dpiHelper) : PLSLiveInfoBase(pPlatformBase, parent, dpiHelper), ui(new Ui::PLSLiveInfoFacebook)
{
	PLS_INFO(liveInfoMoudule, "VLive liveinfo Will show");
	dpiHelper.setCss(this, {PLSCssIndex::PLSLiveInfoFacebook});
	dpiHelper.setFixedSize(this, {720, 550});

	ui->setupUi(content());
	this->setWindowTitle(tr("LiveInfo.Dialog.Title"));
	QMetaObject::connectSlotsByName(this);
	setFocusPolicy(Qt::StrongFocus);
	setFocus();
	PLS_PLATFORM_FACEBOOK->insertParentPointer(this);

	//set first class menu and second class menu
	initComboBoxList();

	//window active or deactive
	m_gameListWidget = new PLSLiveInfoFacebookListWidget(this);
	m_gameListWidget->setObjectName("gameListWidget");
	m_gameListWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
	m_gameListWidget->setVisible(false);
	m_gameListWidget->setAttribute(Qt::WA_ShowWithoutActivating, true);

	//set the gamelineEditWidget list
	connect(ui->gameLineEdit, &PLSFacebookGameLineEdit::searchKeyword, this, &PLSLiveInfoFacebook::searchGameKeyword);
	connect(ui->gameLineEdit, &PLSFacebookGameLineEdit::clearText, this, [=] {
		m_gameListWidget->clear();
		m_gameListWidget->setVisible(false);
	});
	connect(m_gameListWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem *item) {
		PLSLoadingComboxItemData data = item->data(Qt::UserRole).value<PLSLoadingComboxItemData>();
		if (data.type != PLSLoadingComboxItemType::Fa_NormalTitle) {
			return;
		}
		ui->gameLineEdit->setText(data.title);
		hideSearchGameList();
		this->setFocus();
	});

	//set title line edit
	initLineEdit();

	//set the ok button state
	connect(ui->titleField, &QLineEdit::textChanged, this, [=](QString inputText) {
		QByteArray output = inputText.toUtf8();
		if (output.size() > TITLE_MAX_LENGTH) {
			int truncateAt = 0;
			for (int i = TITLE_MAX_LENGTH; i > 0; i--) {
				if ((output[i] & 0xC0) != 0x80) {
					truncateAt = i;
					break;
				}
			}
			output.truncate(truncateAt);
			ui->titleField->setText(QString::fromUtf8(output));
			PLSAlertView::warning(PLSBasic::Get(), QTStr("Live.Check.Alert.Title"), QTStr("facebook.liveinfo.title.max.length"));
			return;
		}
		doUpdateOkState();
	});
	connect(ui->descriptionTitle, &QTextEdit::textChanged, this, [=] { doUpdateOkState(); });
	connect(ui->gameLineEdit, &PLSFacebookGameLineEdit::textChanged, this, [=] { doUpdateOkState(); });

	//connect qApp focusChaned
	connect(qApp, &QApplication::focusChanged, m_gameListWidget, [this](const QWidget *old, const QWidget *now) {
		if (ui->gameLineEdit == now) {
			QString text = ui->gameLineEdit->text();
			if (m_gameListWidget->isHidden() && text.size() > 0) {
				showSearchGameList();
			}
		} else {
			hideSearchGameList();
		}
	});
	dpiHelper.notifyDpiChanged(this, [this](double, double, bool firstShow) {
		if (firstShow) {
			QMetaObject::invokeMethod(
				this,
				[this] {
					pls_flush_style(ui->titleField);
					pls_flush_style(ui->gameLineEdit);
					pls_flush_style(ui->descriptionTitle);
				},
				Qt::QueuedConnection);
		}
	});

	//update ok title logic
	updateStepTitle(ui->okButton);
	if (PLS_PLATFORM_API->isPrepareLive()) {
		ui->horizontalLayout_5->addWidget(ui->okButton);
	}

	//update ok state logic
	doUpdateOkState();
}

void PLSLiveInfoFacebook::on_cancelButton_clicked()
{
	string log = "Facebook liveinfo on_cancelButton_clicked";
	PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	for (int i = 0; i < m_expiredObjectList.size(); i++) {
		QString expiredId = m_expiredObjectList.at(i);
		QString shareObjectName = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookShareObjectName_Key);
		QString dashboardItemId = "";
		if (shareObjectName == GroupObjectFlags) {
			dashboardItemId = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookGroupId_Key);
		} else if (shareObjectName == PageObjectFlags) {
			dashboardItemId = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookPageId_Key);
		}
		if (dashboardItemId == expiredId) {
			PLS_PLATFORM_FACEBOOK->setPrivacyToTimeline();
		}
	}
	reject();
}

void PLSLiveInfoFacebook::on_okButton_clicked()
{
	string log = "Facebook liveinfo on_okButton_clicked";
	PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	showLoading(content());
	QStringList permissionList;
	PLSAPIFacebook::PLSAPI apiType;
	if (IsTimelineObject) {
		permissionList << timeline_living_permission;
		apiType = PLSAPIFacebook::PLSAPICheckTimelineLivingPermission;
	} else if (IsGroupObjectFlags) {
		permissionList << group_living_permission;
		apiType = PLSAPIFacebook::PLSAPICheckGroupLivingPermission;
	} else if (IsPageObjectFlags) {
		permissionList << pages_manage_posts_permission;
		permissionList << pages_read_engagement_permission;
		permissionList << pages_read_user_content_permission;
		apiType = PLSAPIFacebook::PLSAPICheckPageLivingPermission;
	}
	auto permissionFinished = [=](PLSAPIFacebookType type) {
		if (type == PLSAPIFacebookType::PLSFacebookGranted) {
			startLiving();
			return;
		}
		hideLoading();
		if (type == PLSAPIFacebookType::PLSFacebookDeclined || type == PLSAPIFacebookType::PLSFacebookInvalidAccessToken) {
			type = (type == PLSAPIFacebookType::PLSFacebookDeclined) ? PLSAPIFacebookType::PLSRequestPermissionReject : type;
			handleRequestFunctionType(type);
			return;
		}
		type = PLSAPIFacebookType::PLSUpdateLiveInfoFailed;
		handleRequestFunctionType(type);
	};
	PLSFaceBookRquest->checkPermission(apiType, permissionList, permissionFinished, this);
}

void PLSLiveInfoFacebook::initComboBoxList()
{
	//the first comboBox
	ui->shareFirstObject->setDisabled(IsUpdatePage);
	ui->shareFirstObject->setComboBoxTitleData(PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookShareObjectName_Key));
	updateSecondComboBoxTitle();
	connect(ui->shareFirstObject, &PLSLoadingCombox::pressed, this, [=] { ui->shareFirstObject->showTitlesView(PLS_PLATFORM_FACEBOOK->getShareObjectList()); });
	connect(ui->shareFirstObject, &PLSLoadingCombox::clickItemIndex, this, [=](int showIndex) {
		QStringList list = PLS_PLATFORM_FACEBOOK->getShareObjectList();
		ui->shareFirstObject->setComboBoxTitleData(list.at(showIndex));
		updateSecondComboBoxTitle();
		string log = "Facebook liveinfo shareFirstObject: " + showIndex;
		PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	});

	//the second ComboBox
	connect(ui->shareSecondObject, &PLSLoadingCombox::pressed, this, [=] {
		if (IsTimelineObject) {
			ui->shareSecondObject->showTitlesView(PLS_PLATFORM_FACEBOOK->getItemNameList(FacebookPrivacyItemType));
		} else if (IsGroupObjectFlags) {
			onClickGroupComboBox();
		} else if (IsPageObjectFlags) {
			onClickPageComboBox();
		}
	});
	connect(ui->shareSecondObject, &PLSLoadingCombox::clickItemIndex, this, [=](int showIndex) {
		string log = "Facebook liveinfo shareSecondObject: " + showIndex;
		PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
		if (IsTimelineObject) {
			QList<QString> idList = PLS_PLATFORM_FACEBOOK->getItemIdList(FacebookPrivacyItemType);
			QString privacyId = idList.at(showIndex);
			QString privacyName = PLS_PLATFORM_FACEBOOK->getItemName(privacyId, FacebookPrivacyItemType);
			ui->shareSecondObject->setComboBoxTitleData(privacyName, privacyId);
		} else if (IsGroupObjectFlags) {
			QList<QString> idList = PLS_PLATFORM_FACEBOOK->getItemIdList(FacebookGroupItemType);
			QString groupId = idList.at(showIndex);
			QString groupName = PLS_PLATFORM_FACEBOOK->getItemName(groupId, FacebookGroupItemType);
			ui->shareSecondObject->setComboBoxTitleData(groupName, groupId);
		} else if (IsPageObjectFlags) {
			QList<QString> idList = PLS_PLATFORM_FACEBOOK->getItemIdList(FacebookPageItemType);
			QString pageId = idList.at(showIndex);
			QString pageName = PLS_PLATFORM_FACEBOOK->getItemName(pageId, FacebookPageItemType);
			ui->shareSecondObject->setComboBoxTitleData(pageName, pageId);
		}
		doUpdateOkState();
	});

	//when living, group and page second list is disabled
	if (IsUpdatePage && (IsGroupObjectFlags || IsPageObjectFlags)) {
		ui->shareSecondObject->setDisabled(true);
	}
}

void PLSLiveInfoFacebook::initLineEdit()
{
	ui->titleField->setText(PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveTitle_Key));
	ui->descriptionTitle->setText(PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveDescription_Key));
	ui->gameLineEdit->setText(PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveGameName_Key));
	initPlaceTextHorderColor(ui->titleField);
	initPlaceTextHorderColor(ui->descriptionTitle);
	initPlaceTextHorderColor(ui->gameLineEdit);
}

void PLSLiveInfoFacebook::updateSecondComboBoxTitle()
{
	if (IsTimelineObject) {
		QString privacyId = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookPrivacyId_Key);
		QString privacyName = PLS_PLATFORM_FACEBOOK->getItemName(privacyId, FacebookPrivacyItemType);
		ui->shareSecondObject->setComboBoxTitleData(privacyName, privacyId);
	} else if (IsGroupObjectFlags) {
		QString groupId = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookGroupId_Key);
		QString groupName = PLS_PLATFORM_FACEBOOK->getItemName(groupId, FacebookGroupItemType);
		ui->shareSecondObject->setComboBoxTitleData(groupName, groupId);
	} else if (IsPageObjectFlags) {
		QString pageId = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookPageId_Key);
		QString pageName = PLS_PLATFORM_FACEBOOK->getItemName(pageId, FacebookPageItemType);
		ui->shareSecondObject->setComboBoxTitleData(pageName, pageId);
	}
	doUpdateOkState();
}

void PLSLiveInfoFacebook::onClickGroupComboBox()
{
	string log = "Facebook liveinfo onClickGroupComboBox: ";
	PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	auto onFinish = [=](PLSAPIFacebookType type) {
		if (type != PLSAPIFacebookType::PLSFacebookSuccess) {
			handleRequestFunctionType(type);
			if (type == PLSAPIFacebookType::PLSFacebookNetworkError) {
				showNetworkErrorAlert(PLSLiveInfoFacebookErrorType::PLSLiveInfoFacebookGroupError);
			}
			return;
		}
		QList<QString> nameList = PLS_PLATFORM_FACEBOOK->getItemNameList(FacebookGroupItemType);
		if (nameList.size() > 0) {
			QList<QString> idList = PLS_PLATFORM_FACEBOOK->getItemIdList(FacebookGroupItemType);
			QString groupId = ui->shareSecondObject->getComboBoxId();
			if (!idList.contains(groupId)) {
				PLS_PLATFORM_FACEBOOK->resetLiveInfoGroupId();
				groupId = GROUPLIST_DEFAULT_ITEM_ID;
			}
			QString title = PLS_PLATFORM_FACEBOOK->getItemName(groupId, FacebookGroupItemType);
			ui->shareSecondObject->setComboBoxTitleData(title, groupId);
			ui->shareSecondObject->updateTitleIdsView(nameList, idList);
		} else {
			PLS_PLATFORM_FACEBOOK->resetLiveInfoGroupId();
			QString groupId = GROUPLIST_DEFAULT_ITEM_ID;
			QString title = PLS_PLATFORM_FACEBOOK->getItemName(groupId, FacebookGroupItemType);
			ui->shareSecondObject->setComboBoxTitleData(title, groupId);
			ui->shareSecondObject->refreshGuideView(tr("facebook.empty.grouplist.tip"));
		}
		doUpdateOkState();
	};
	PLS_PLATFORM_FACEBOOK->getMyGroupListRequestAndCheckPermission(onFinish, this);
	ui->shareSecondObject->showLoadingView();
}

void PLSLiveInfoFacebook::onClickPageComboBox()
{
	string log = "Facebook liveinfo onClickPageComboBox: ";
	PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	auto onFinish = [=](PLSAPIFacebookType type) {
		if (type != PLSAPIFacebookType::PLSFacebookSuccess) {
			handleRequestFunctionType(type);
			if (type == PLSAPIFacebookType::PLSFacebookNetworkError) {
				showNetworkErrorAlert(PLSLiveInfoFacebookErrorType::PLSLiveInfoFacebookPageError);
			}
			return;
		}
		QList<QString> nameList = PLS_PLATFORM_FACEBOOK->getItemNameList(FacebookPageItemType);
		if (nameList.size() > 0) {
			QList<QString> idList = PLS_PLATFORM_FACEBOOK->getItemIdList(FacebookPageItemType);
			QString pageId = ui->shareSecondObject->getComboBoxId();
			if (!idList.contains(pageId)) {
				PLS_PLATFORM_FACEBOOK->resetLiveInfoPageId();
				pageId = PAGELIST_DEFAULT_ITEM_ID;
			}
			QString title = PLS_PLATFORM_FACEBOOK->getItemName(pageId, FacebookPageItemType);
			ui->shareSecondObject->setComboBoxTitleData(title, pageId);
			ui->shareSecondObject->updateTitleIdsView(nameList, idList);
		} else {
			PLS_PLATFORM_FACEBOOK->resetLiveInfoPageId();
			QString pageId = PAGELIST_DEFAULT_ITEM_ID;
			QString title = PLS_PLATFORM_FACEBOOK->getItemName(pageId, FacebookPageItemType);
			ui->shareSecondObject->setComboBoxTitleData(title, pageId);
			ui->shareSecondObject->refreshGuideView(tr("facebook.empty.pagelist.tip"));
		}
		doUpdateOkState();
	};
	PLS_PLATFORM_FACEBOOK->getMyPageListRequestAndCheckPermission(onFinish, this);
	ui->shareSecondObject->showLoadingView();
}

PLSLiveInfoFacebook::~PLSLiveInfoFacebook()
{
	delete ui;
}

void PLSLiveInfoFacebook::searchGameKeyword(const QString keyword)
{
	string log = "Facebook liveinfo searchGameKeyword";
	PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	QPoint p = this->content()->mapToGlobal(QPoint(ui->gameLineEdit->pos().x(), ui->gameLineEdit->pos().y() + ui->gameLineEdit->height()));
	m_gameListWidget->move(p);
	double dpi = PLSDpiHelper::getDpi(this);
	int count = m_gameListWidget->count();
	int gameItemHeight = PLSDpiHelper::calculate(dpi, GAMELIST_ITEM_HEIGHT);
	int border = PLSDpiHelper::calculate(dpi, 1);
	int padding = PLSDpiHelper::calculate(dpi, 1);
	if (count > GAME_ITEM_MAX_SIZE || count == 0) {
		count = GAME_ITEM_MAX_SIZE;
	}
	m_gameListWidget->setFixedSize(ui->gameLineEdit->width(), gameItemHeight * count + (border + padding) * 2);
	setupGameListCornerRadius();
	m_gameListWidget->setVisible(true);
	m_gameListWidget->clear();
	auto onFinish = [=](PLSAPIFacebookType type) {
		if (type != PLSAPIFacebookType::PLSFacebookSuccess) {
			handleRequestFunctionType(type);
			if (type == PLSAPIFacebookType::PLSFacebookNetworkError) {
				showNetworkErrorAlert(PLSLiveInfoFacebookErrorType::PLSLiveInfoFacebookSearchGameError);
			}
			return;
		}
		QList<QString> list = PLS_PLATFORM_FACEBOOK->getItemNameList(FacebookGameItemType);
		m_gameListWidget->clear();
		for (int i = 0; i < list.size(); i++) {
			QString title = list.at(i);
			QListWidgetItem *item = new QListWidgetItem(title);
			PLSLoadingComboxItemData itemData = PLSLoadingComboxItemData();
			itemData.showIndex = i;
			itemData.title = title;
			item->setData(Qt::UserRole, QVariant::fromValue<PLSLoadingComboxItemData>(itemData));
			item->setSizeHint(QSize(item->sizeHint().width(), gameItemHeight));
			m_gameListWidget->addItem(item);
		}
		int size = list.size();
		if (size > GAME_ITEM_MAX_SIZE) {
			size = GAME_ITEM_MAX_SIZE;
		} else if (size == 0) {
			QString title = tr("facebook.liveinfo.game.empty.list");
			QListWidgetItem *item = new QListWidgetItem(title);
			item->setFlags(Qt::ItemIsEnabled);
			PLSLoadingComboxItemData itemData = PLSLoadingComboxItemData();
			itemData.showIndex = 0;
			itemData.title = title;
			itemData.type = PLSLoadingComboxItemType::Fa_Guide;
			item->setData(Qt::UserRole, QVariant::fromValue<PLSLoadingComboxItemData>(itemData));
			item->setSizeHint(QSize(item->sizeHint().width(), gameItemHeight));
			m_gameListWidget->addItem(item);
			size = 1;
		}
		m_gameListWidget->setFixedSize(ui->gameLineEdit->width(), gameItemHeight * size + (border + padding) * 2);
		QString title = ui->gameLineEdit->text();
		int index = MENU_DONT_SELECTED_INDEX;
		if (list.contains(title)) {
			index = list.indexOf(title);
			m_gameListWidget->setCurrentRow(index);
		}
		setupGameListCornerRadius();
	};
	PLS_PLATFORM_FACEBOOK->getGameTagListByKeyword(onFinish, keyword);
}

void PLSLiveInfoFacebook::hideSearchGameList()
{
	m_gameListWidget->setVisible(false);
	ui->gameLineEdit->clearFocus();
}

void PLSLiveInfoFacebook::showSearchGameList()
{
	int count = m_gameListWidget->count();
	double dpi = PLSDpiHelper::getDpi(this);
	int gameItemHeight = PLSDpiHelper::calculate(dpi, GAMELIST_ITEM_HEIGHT);
	int border = PLSDpiHelper::calculate(dpi, 1);
	int padding = PLSDpiHelper::calculate(dpi, 1);
	for (int i = 0; i < count; i++) {
		QListWidgetItem *item = m_gameListWidget->item(i);
		QSize size = item->sizeHint();
		size.setHeight(gameItemHeight);
		item->setSizeHint(size);
	}
	QPoint p = this->content()->mapToGlobal(QPoint(ui->gameLineEdit->pos().x(), ui->gameLineEdit->pos().y() + ui->gameLineEdit->height()));
	m_gameListWidget->move(p);
	if (count > GAME_ITEM_MAX_SIZE || count == 0) {
		count = GAME_ITEM_MAX_SIZE;
	}
	m_gameListWidget->setFixedSize(ui->gameLineEdit->width(), gameItemHeight * count + (border + padding) * 2);
	setupGameListCornerRadius();
	m_gameListWidget->setVisible(true);
}

void PLSLiveInfoFacebook::doUpdateOkState()
{
	QString newPrivacy = ui->shareSecondObject->getComboBoxTitle();
	if (newPrivacy == GROUPLIST_DEFAULT_SELECT_TEXT || newPrivacy == PAGELIST_DEFAULT_SELECT_TEXT) {
		ui->okButton->setEnabled(false);
		return;
	}
	ui->okButton->setEnabled(PLS_PLATFORM_API->isPrepareLive() || isModified());
}

void PLSLiveInfoFacebook::startLiving()
{
	m_expiredObjectList.clear();
	QMap<QString, QString> info;
	QString gameName = ui->gameLineEdit->text();
	m_startLivingApi = false;
	info.insert(FacebookLiveTitle_Key, ui->titleField->text());
	info.insert(FacebookLiveDescription_Key, ui->descriptionTitle->toPlainText());
	info.insert(FacebookLiveGameName_Key, gameName);
	info.insert(FacebookShareObjectName_Key, ui->shareFirstObject->getComboBoxTitle());
	QString secondObjectId = ui->shareSecondObject->getComboBoxId();
	if (IsTimelineObject) {
		info.insert(FacebookPrivacyId_Key, secondObjectId);
	} else if (IsGroupObjectFlags) {
		info.insert(FacebookGroupId_Key, secondObjectId);
	} else if (IsPageObjectFlags) {
		info.insert(FacebookPageId_Key, secondObjectId);
	}
	auto livingFinished = [=](PLSAPIFacebookType type) {
		hideLoading();
		if (type == PLSAPIFacebookType::PLSFacebookSuccess) {
			accept();
			return;
		}
		if (type == PLSAPIFacebookType::PLSFacebookInvalidAccessToken) {
			handleRequestFunctionType(type);
			return;
		}
		if (type == PLSAPIFacebookType::PLSFacebookDeclined || type == PLSAPIFacebookType::PLSFacebookObjectDontExist) {
			if (type == PLSAPIFacebookType::PLSFacebookObjectDontExist && m_startLivingApi) {
				m_expiredObjectList.append(secondObjectId);
			}
			type = PLSAPIFacebookType::PLSLivingPermissionReject;
			handleRequestFunctionType(type);
			return;
		}
		type = PLSAPIFacebookType::PLSUpdateLiveInfoFailed;
		handleRequestFunctionType(type);
	};
	if (IsUpdatePage) {
		PLS_PLATFORM_FACEBOOK->updateLiving(info, livingFinished);
	} else {
		if (PLS_PLATFORM_API->isPrepareLive()) {
			m_startLivingApi = true;
			PLS_PLATFORM_FACEBOOK->startLiving(info, livingFinished);
		} else {
			auto itemInfoFinished = [=](PLSAPIFacebookType type) {
				hideLoading();
				if (type == PLSAPIFacebookType::PLSFacebookSuccess) {
					accept();
				} else if (type != PLSAPIFacebookType::PLSFacebookInvalidAccessToken) {
					type = PLSAPIFacebookType::PLSUpdateLiveInfoFailed;
				}
				handleRequestFunctionType(type);
			};
			PLS_PLATFORM_FACEBOOK->requestItemInfoRequest(info, itemInfoFinished);
		}
	}
}

bool PLSLiveInfoFacebook::isModified()
{
	//the first share object name
	QString oldShareObjectName = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookShareObjectName_Key);
	QString newShareObjectName = ui->shareFirstObject->getComboBoxTitle();
	if (oldShareObjectName != newShareObjectName) {
		return true;
	}

	//the second share object name
	QString oldPrivacy = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookPrivacyId_Key);
	if (IsGroupObjectFlags) {
		oldPrivacy = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookGroupId_Key);
	} else if (IsPageObjectFlags) {
		oldPrivacy = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookPageId_Key);
	}
	QString newPrivacy = ui->shareSecondObject->getComboBoxId();
	if (newPrivacy != oldPrivacy) {
		return true;
	}

	//the title
	QString oldTitle = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveTitle_Key);
	QString newTitle = ui->titleField->text();
	if (oldTitle != newTitle) {
		return true;
	}

	//the description
	QString oldDescription = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveDescription_Key);
	QString newDescription = ui->descriptionTitle->toPlainText();
	if (oldDescription != newDescription) {
		return true;
	}

	//the game
	QString oldGame = PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveGameName_Key);
	QString newGame = ui->gameLineEdit->text();
	if (oldGame != newGame) {
		return true;
	}

	return false;
}

void PLSLiveInfoFacebook::initPlaceTextHorderColor(QWidget *widget)
{
	QPalette palette = widget->palette();
	palette.setColor(QPalette::All, QPalette::PlaceholderText, Qt::white);
	widget->setPalette(palette);
}

void PLSLiveInfoFacebook::setupGameListCornerRadius()
{
	double dpi = PLSDpiHelper::getDpi(this);
	const int radius = PLSDpiHelper::calculate(dpi, 3);
	QPainterPath path;
	path.addRoundedRect(m_gameListWidget->rect(), radius, radius);
	QRegion mask = QRegion(path.toFillPolygon().toPolygon());
	m_gameListWidget->setMask(mask);
}

void PLSLiveInfoFacebook::showNetworkErrorAlert(PLSLiveInfoFacebookErrorType errorType)
{
	string log = "Facebook liveinfo showNetworkErrorAlert show";
	PLS_UI_STEP(liveInfoMoudule, log.c_str(), ACTION_CLICK);
	switch (errorType) {
	case PLSLiveInfoFacebookErrorType::PLSLiveInfoFacebookGroupError:
		if (IsGroupObjectFlags && ui->shareSecondObject->isChecked()) {
			PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("main.message.error.netError"));
		}
		break;
	case PLSLiveInfoFacebookErrorType::PLSLiveInfoFacebookPageError:
		if (IsPageObjectFlags && ui->shareSecondObject->isChecked()) {
			PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("main.message.error.netError"));
		}
		break;
	case PLSLiveInfoFacebookErrorType::PLSLiveInfoFacebookSearchGameError:
		if (ui->gameLineEdit->hasFocus()) {
			PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("main.message.error.netError"));
		}
		break;
	default:
		break;
	}
}

void PLSLiveInfoFacebook::showEvent(QShowEvent *event)
{
	if (IsUpdatePage) {
		showLoading(content());
		auto onFinished = [=](PLSAPIFacebookType type) {
			hideLoading();
			if (type == PLSAPIFacebookType::PLSFacebookSuccess) {
				ui->titleField->setText(PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveTitle_Key));
				ui->descriptionTitle->setText(PLS_PLATFORM_FACEBOOK->getLiveInfoValue(FacebookLiveDescription_Key));
				return;
			}
			handleRequestFunctionType(type);
		};
		PLS_PLATFORM_FACEBOOK->getLivingVideoInfo(onFinished);
	}
	PLSLiveInfoBase::showEvent(event);
}
