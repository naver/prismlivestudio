#include "PLSBgmLibraryView.h"
#include "PLSBackgroundMusicView.h"
#include "PLSFloatScrollBarScrollArea.h"
#include "ui_PLSBgmLibraryItem.h"
#include "ui_PLSBgmLibraryView.h"

#include "PLSNetworkMonitor.h"
#include "action.h"
#include "liblog.h"
#include "log/module_names.h"
#include "pls-app.hpp"
#include "pls-common-define.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QScrollBar>
#include <QUuid>

static const char *LIBRARY_HOT_TAB = "HOT";
static const char *LISTEN_MEDIA_SOURCE = "background-music-audition";
static const char *PLAY = "play";
static const char *SELECTED = "selected";

static const int TOAST_KEEP_MILLISECONDS = 5000;
static const int TOAST_LEFT_TOP_MARGIN = 10;
static const int TOAST_DEL_BUTTON_MARGIN = 5;

PLSBgmLibraryView::PLSBgmLibraryView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent), ui(new Ui::PLSBgmLibraryView)
{
	ui->setupUi(this->content());
	qRegisterMetaType<obs_media_state>("obs_media_state");
	dpiHelper.setCss(this, {PLSCssIndex::PLSBgmLibraryView, PLSCssIndex::PLSToastMsgFrame});
	setWindowTitle(QTStr("Bgm.library.title"));
	setAttribute(Qt::WA_AlwaysShowToolTips);
	initToast();
	initGroup();
	connect(ui->okBtn, &QPushButton::clicked, this, &PLSBgmLibraryView::OnOkButtonClicked);
	connect(ui->cancelBtn, &QPushButton::clicked, this, &PLSBgmLibraryView::OnCancelButtonClicked);
	connect(ui->preGroupBtn, &QPushButton::clicked, this, &PLSBgmLibraryView::OnPreGroupButtonClicked);
	connect(ui->nextGroupBtn, &QPushButton::clicked, this, &PLSBgmLibraryView::OnNextGroupButtonClicked);
	connect(ui->groupScrollArea->horizontalScrollBar(), &QScrollBar::valueChanged, this, &PLSBgmLibraryView::OnGroupScrolled);
	connect(ui->groupScrollArea->horizontalScrollBar(), &QScrollBar::rangeChanged, this, &PLSBgmLibraryView::OnGroupScrolled);

	networkAvailable = networkMonitor.networkAccessible();
	connect(&networkMonitor, &QNetworkAccessManager::networkAccessibleChanged, this, &PLSBgmLibraryView::OnNetworkAccessibleChanged);
#ifdef _WIN32
	connect(PLSNetworkMonitor::Instance(), &PLSNetworkMonitor::OnNetWorkStateChanged, this, &PLSBgmLibraryView::OnNetWorkStateChanged);
#endif
}

PLSBgmLibraryView::~PLSBgmLibraryView()
{
	PLSBgmDataViewManager::Instance()->ClearCachePlayList();
#ifdef _WIN32
	disconnect(PLSNetworkMonitor::Instance(), &PLSNetworkMonitor::OnNetWorkStateChanged, this, &PLSBgmLibraryView::OnNetWorkStateChanged);
#endif
	if (sourceAudition) {
		signal_handler_disconnect(obs_source_get_signal_handler(sourceAudition), "media_state_changed", MediaStateChanged, this);
		signal_handler_disconnect(obs_source_get_signal_handler(sourceAudition), "media_load", MediaLoad, this);
		obs_source_media_stop(sourceAudition);
		obs_source_dec_active(sourceAudition);
		obs_source_release(sourceAudition);
	}
	delete ui;
}

void PLSBgmLibraryView::initGroup()
{
	ui->nextFrame->setVisible(false);
	ui->preFrame->setVisible(false);
	const BgmItemGroupVectorType &groupList = PLSBgmDataViewManager::Instance()->GetGroupList();
	if (groupList.empty())
		InitCategoryData();
	int index = 0;
	for (auto &group : groupList) {
		if (index > 0)
			ui->groupHLayout->addStretch();
		CreateGroup(group.first);
		++index;
	}

	auto groupName = PLSBgmDataViewManager::Instance()->GetFirstGroupName();
	OnGroupButtonClicked(groupName);
}

void PLSBgmLibraryView::InitButtonState(const QString &group)
{
	auto iter = musicListItem.find(group);
	if (iter == musicListItem.end())
		return;
	auto listMusic = iter->second;
	if (listMusic.empty())
		return;

	PLSBackgroundMusicView *bmv = static_cast<PLSBackgroundMusicView *>(this->parent());
	if (!bmv) {
		return;
	}

	auto judgeListItem = [=](const QVector<PLSBgmItemData> &listData) {
		auto iterMusicList = listMusic.cbegin();
		while (iterMusicList != listMusic.cend()) {
			auto item = (*iterMusicList).second;
			if (!item) {
				++iterMusicList;
				continue;
			}
			for (auto data : listData) {
				auto url = data.GetUrl(data.id);
				if (item->ContainUrl(url)) {
					item->SetDurationTypeSelected(url);
				}
			}
			++iterMusicList;
		}
	};

	QVector<PLSBgmItemData> playlist = bmv->GetPlayListDatas();
	judgeListItem(playlist);
}

void PLSBgmLibraryView::InitCategoryData()
{
	QByteArray array;
	if (!PLSBgmDataViewManager::Instance()->LoadDataFormLocalFile(array)) {
		return;
	}

	BgmLibraryData hotMusicList{};
	QJsonObject object = QJsonDocument::fromJson(array).object();
	QJsonArray groups = object["group"].toArray();
	for (auto arrIter = groups.constBegin(); arrIter != groups.constEnd(); ++arrIter) {
		QJsonObject groupObj = (*arrIter).toObject();
		QJsonArray items = groupObj["items"].toArray();
		if (0 == items.size()) {
			continue;
		}

		QString groupId = groupObj["groupId"].toString();

		BgmLibraryData musicList{};
		for (auto itemIter = items.constBegin(); itemIter != items.constEnd(); ++itemIter) {
			PLSBgmItemData data;
			QJsonObject itemObj = (*itemIter).toObject();
			data.title = itemObj["title"].toString();
			data.producer = itemObj["producer"].toString();
			bool recommendFlag = itemObj["recommendFlag"].toBool();
			data.id = (int)DurationType::FullSeconds;

			QJsonObject propertyObj = itemObj["properties"].toObject();
			if (propertyObj.empty()) {
				continue;
			}

			//60s
			QJsonObject duration60Obj = propertyObj["duration60SecondsMusicInfo"].toObject();
			data.SetUrl(duration60Obj["url"].toString(), (int)DurationType::SixtySeconds);
			data.SetDuration((int)DurationType::SixtySeconds, duration60Obj["duration"].toInt());

			//15s
			QJsonObject duration15Obj = propertyObj["duration15SecondsMusicInfo"].toObject();
			data.SetUrl(duration15Obj["url"].toString(), (int)DurationType::FifteenSeconds);
			data.SetDuration((int)DurationType::FifteenSeconds, duration15Obj["duration"].toInt());

			//30s
			QJsonObject duration30Obj = propertyObj["duration30SecondsMusicInfo"].toObject();
			data.SetUrl(duration30Obj["url"].toString(), (int)DurationType::ThirtySeconds);
			data.SetDuration((int)DurationType::ThirtySeconds, duration30Obj["duration"].toInt());

			//full
			QJsonObject fullObj = propertyObj["durationFullMusicInfo"].toObject();
			data.SetUrl(fullObj["url"].toString(), (int)DurationType::FullSeconds);
			data.SetDuration((int)DurationType::FullSeconds, fullObj["duration"].toInt());
			if (recommendFlag) {
				hotMusicList.insert(hotMusicList.begin(), BgmLibraryData::value_type(data.title, data));
			}

			musicList.insert(musicList.begin(), BgmLibraryData::value_type(data.title, data));
		}
		PLSBgmDataViewManager::Instance()->AddGroupList(groupId, musicList);
	}

	// add library hot tab
	bool haveHot = !hotMusicList.empty();
	if (haveHot) {
		PLSBgmDataViewManager::Instance()->InsertFrontGroupList(LIBRARY_HOT_TAB, hotMusicList);
	}
}

void PLSBgmLibraryView::UpdateSelectedString()
{
	auto cacheCount = PLSBgmDataViewManager::Instance()->GetCachePlayListSize();
	QString strCountText;
	if (cacheCount > 0) {
		strCountText = ((cacheCount == 1) ? QTStr("Bgm.library.selected.one") : QTStr("Bgm.library.selected.more").arg(cacheCount));
		auto strList = strCountText.split("|");
		if (strList.size() == 2) {
			strCountText = QString("<span style='color:#effc35'>%1</span>").arg(strList[0]);
			strCountText.append(" ");
			strCountText.append(strList[1]);
		}
	}
	ui->countLabel->setText(strCountText);
}

void PLSBgmLibraryView::resizeEvent(QResizeEvent *event)
{
	PLSDialogView::resizeEvent(event);
	toastView.SetShowWidth(this->width() - 2 * PLSDpiHelper::calculate(this, 10));
}

void PLSBgmLibraryView::showEvent(QShowEvent *event)
{
	PLSDialogView::showEvent(event);
}

void PLSBgmLibraryView::OnListenButtonClicked(PLSBgmLibraryItem *item)
{
	if (!item) {
		return;
	}

	if (!networkAvailable) {
		ShowToastView(QTStr("Bgm.library.no.network"));
		return;
	}

	if (!sourceAudition) {
		CreateListenSource();
	}

	obs_media_state state = obs_source_media_get_state(sourceAudition);
	bool playing = state == OBS_MEDIA_STATE_PLAYING;

	// should not show toast when music stoped.
	if (firstListen && (!playing)) {
		ShowToastView(QTStr("Bgm.Preview.Played"));
		firstListen = false;
	}

	if (listenUrl == item->GetUrl(item->GetDurationType())) {
		PLS_INFO(MAIN_BGM_MODULE, "obs_source_media_stop called: %s", listenUrl.toStdString().c_str());
		obs_source_media_stop(sourceAudition);
		listenUrl.clear(); //#5275
		return;
	}

	OBSData settings = pls_get_source_setting(sourceAudition);
	QString url = item->GetUrl(item->GetDurationType());
	QString currentUrl = obs_data_get_string(settings, "input");
	listenUrl = url;
	if (url == currentUrl && ((state != OBS_MEDIA_STATE_PLAYING) && (state != OBS_MEDIA_STATE_OPENING))) {
		obs_source_media_restart(sourceAudition);
	} else {
		emit signal_meida_state_changed(currentUrl, OBS_MEDIA_STATE_STOPPED);
		obs_data_set_string(settings, "input", url.toStdString().c_str());
		obs_source_update(sourceAudition, settings);
	}
}

void PLSBgmLibraryView::OnAddButtonClicked(PLSBgmLibraryItem *item)
{
	if (!item) {
		return;
	}

	PLSBackgroundMusicView *bmv = static_cast<PLSBackgroundMusicView *>(this->parent());
	if (!bmv) {
		return;
	}

	PLSBgmItemData data = item->GetData();
	if (PLSBgmDataViewManager::Instance()->CachePlayListExisted(data)) {
		ShowToastView(QTStr("Bgm.Add.Free.Music.Once"));
		return;
	}

	item->SetCheckedButtonVisible(true);
	PLSBgmDataViewManager::Instance()->AddCachePlayList(data);
	UpdateSelectedString();
}

void PLSBgmLibraryView::OnCheckedButtonClicked(PLSBgmLibraryItem *item)
{
	if (!item) {
		return;
	}

	PLSBgmItemData data = item->GetData();
	item->SetCheckedButtonVisible(false);
	PLSBgmDataViewManager::Instance()->DeleteCachePlayList(data);
	UpdateSelectedString();
}

void PLSBgmLibraryView::OnDurationTypeChanged(PLSBgmLibraryItem *item)
{
	if (!item) {
		return;
	}

	PLSBgmItemData data = item->GetData();

	if (PLSBgmDataViewManager::Instance()->CachePlayListExisted(data)) {
		item->SetCheckedButtonVisible(true);
	} else
		item->SetCheckedButtonVisible(false);
	OnListenButtonClicked(item);
}

void PLSBgmLibraryView::OnOkButtonClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Library ok Button", ACTION_CLICK);
	BgmItemCacheType cachePlayList = PLSBgmDataViewManager::Instance()->GetCachePlayList();
	if (cachePlayList.isEmpty()) {
		this->accept();
		return;
	}

	PLSBackgroundMusicView *bmv = static_cast<PLSBackgroundMusicView *>(this->parent());
	if (!bmv) {
		return;
	}
	//check if selected songs are exsit in play list.
	auto iter = cachePlayList.begin();
	while (iter != cachePlayList.end()) {
		if (bmv->CurrentPlayListBgmDataExisted(iter->GetUrl(iter->id))) {
			iter = cachePlayList.erase(iter);
			continue;
		}
		++iter;
	}

	emit AddCachePlayList(cachePlayList);
	PLSBgmDataViewManager::Instance()->ClearCachePlayList();
	ui->countLabel->setText("");
	this->accept();
}

void PLSBgmLibraryView::OnCancelButtonClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Library cancel Button", ACTION_CLICK);
	BgmItemCacheType cachePlayList = PLSBgmDataViewManager::Instance()->GetCachePlayList();
	PLSBgmDataViewManager::Instance()->ClearCachePlayList();
	ui->countLabel->setText("");
	this->reject();
}

void PLSBgmLibraryView::OnPreGroupButtonClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "pre group Button", ACTION_CLICK);
	auto bar = ui->groupScrollArea->horizontalScrollBar();

	double dpi = PLSDpiHelper::getDpi(this);
	int tmp_value = ui->groupFrame->width() / PLSDpiHelper::calculate(dpi, 100 + 10);
	double val = tmp_value * PLSDpiHelper::calculate(dpi, 100);

	bar->setValue(bar->value() - val);
}

void PLSBgmLibraryView::OnNextGroupButtonClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "next group Button", ACTION_CLICK);
	auto bar = ui->groupScrollArea->horizontalScrollBar();

	double dpi = PLSDpiHelper::getDpi(this);
	int tmp_value = this->width() / PLSDpiHelper::calculate(dpi, 100);
	double val = tmp_value * PLSDpiHelper::calculate(dpi, 100);

	bar->setValue(bar->value() + val);
}

void PLSBgmLibraryView::OnGroupScrolled()
{
	int value = ui->groupScrollArea->horizontalScrollBar()->value();
	int maxValue = ui->groupScrollArea->horizontalScrollBar()->maximum();
	int minValue = ui->groupScrollArea->horizontalScrollBar()->minimum();
	bool showBtn = (maxValue > 0);
	ui->nextFrame->setVisible(showBtn);
	ui->preFrame->setVisible(showBtn);
	if (maxValue > 0) {
		bool scrollToEnd = (value == maxValue);
		bool scrollToStart = (value == minValue);
		ui->preGroupBtn->setEnabled(!scrollToStart);
		ui->nextGroupBtn->setEnabled(!scrollToEnd);
		ui->horizontalLayout->update();
	}
}

void PLSBgmLibraryView::OnGroupButtonClicked(const QString &group)
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("%1 group Button").arg(group).toStdString().c_str(), ACTION_CLICK);

	if (group == selectedGroup) {
		return;
	}
	selectedGroup = group;
	RefreshSelectedGroupButtonStyle(group);

	auto iterFind = groupWidget.find(group);
	if (iterFind == groupWidget.end()) {
		auto musicList = PLSBgmDataViewManager::Instance()->GetGroupList(group);
		CreateMusicList(group, musicList);
		InitButtonState(group);
	} else {
		UpdateMusiclist(group);
	}
	ShowList(group);
}

void PLSBgmLibraryView::MediaStateChanged(void *data, calldata_t *calldata)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(calldata, "source");
	if (!source) {
		return;
	}

	const char *name = obs_source_get_name(source);
	if (name && 0 == strcmp(name, LISTEN_MEDIA_SOURCE)) {
		OBSData settings = pls_get_source_setting(source);
		QString url = obs_data_get_string(settings, "input");
		obs_media_state state = obs_source_media_get_state(source);
		PLSBgmLibraryView *media = static_cast<PLSBgmLibraryView *>(data);
		QMetaObject::invokeMethod(media, "OnMediaStateChanged", Qt::QueuedConnection, Q_ARG(const QString &, url), Q_ARG(obs_media_state, state));
	}
}

void PLSBgmLibraryView::MediaLoad(void *data, calldata_t *calldata)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(calldata, "source");
	if (!source) {
		return;
	}
	const char *name = obs_source_get_name(source);
	if (name && 0 == strcmp(name, LISTEN_MEDIA_SOURCE)) {
		OBSData settings = pls_get_source_setting(source);
		QString url = obs_data_get_string(settings, "input");
		bool load = calldata_bool(calldata, "load");
		PLSBgmLibraryView *media = static_cast<PLSBgmLibraryView *>(data);
		QMetaObject::invokeMethod(media, "OnMediaLoad", Qt::QueuedConnection, Q_ARG(const QString &, url), Q_ARG(bool, load));
	}
}

void PLSBgmLibraryView::OnMediaStateChanged(const QString &url, obs_media_state state)
{
	emit signal_meida_state_changed(url, state);
	switch (state) {
	case OBS_MEDIA_STATE_STOPPED:
	case OBS_MEDIA_STATE_ENDED:
	case OBS_MEDIA_STATE_NONE:
	case OBS_MEDIA_STATE_ERROR:
		listenUrl.clear();
		break;
	default:
		break;
	}
}

void PLSBgmLibraryView::OnMediaLoad(const QString &url, bool load)
{
	emit signal_meida_state_changed(url, load ? OBS_MEDIA_STATE_OPENING : obs_source_media_get_state(sourceAudition));
}

void PLSBgmLibraryView::OnNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
	if (accessible == QNetworkAccessManager::Accessible) {
		networkAvailable = true;
	}
}

void PLSBgmLibraryView::OnNetWorkStateChanged(bool isConnected)
{
	if (!isConnected) {
		networkAvailable = isConnected;
		if (sourceAudition)
			obs_source_media_stop(sourceAudition);
	}
}

PLSBgmLibraryItem *PLSBgmLibraryView::CreateLibraryBgmItemView(const PLSBgmItemData &data, QWidget *parent)
{
	PLSBgmLibraryItem *item = new PLSBgmLibraryItem(data, parent);
	if (item->ContainUrl(listenUrl)) {
		if (sourceAudition) {
			obs_media_state state = obs_source_media_get_state(sourceAudition);
			item->onMediaStateChanged(listenUrl, state);
		}
	}
	PLSDpiHelper::dpiDynamicUpdate(item, true);
	connect(this, &PLSBgmLibraryView::signal_meida_state_changed, item, &PLSBgmLibraryItem::onMediaStateChanged);
	connect(item, &PLSBgmLibraryItem::ListenButtonClickedSignal, this, &PLSBgmLibraryView::OnListenButtonClicked, Qt::QueuedConnection);
	connect(item, &PLSBgmLibraryItem::AddButtonClickedSignal, this, &PLSBgmLibraryView::OnAddButtonClicked, Qt::QueuedConnection);
	connect(item, &PLSBgmLibraryItem::CheckedButtonClickedSignal, this, &PLSBgmLibraryView::OnCheckedButtonClicked, Qt::QueuedConnection);
	connect(item, &PLSBgmLibraryItem::DurationTypeChangeSignal, this, &PLSBgmLibraryView::OnDurationTypeChanged, Qt::QueuedConnection);
	return item;
}

void PLSBgmLibraryView::CreateListenSource()
{
	if (sourceAudition) {
		return;
	}

	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "input_format", "MP3");
	obs_data_set_bool(settings, "is_local_file", false);
	obs_data_set_bool(settings, "bgm_source", true);
	sourceAudition = obs_source_create_private(MEDIA_SOURCE_ID, LISTEN_MEDIA_SOURCE, settings);
	if (!sourceAudition) {
		obs_data_release(settings);
		return;
	}
	obs_source_set_monitoring_type(sourceAudition, OBS_MONITORING_TYPE_MONITOR_ONLY);

	obs_data_release(settings);
	obs_source_inc_active(sourceAudition);
	signal_handler_connect_ref(obs_source_get_signal_handler(sourceAudition), "media_state_changed", MediaStateChanged, this);
	signal_handler_connect_ref(obs_source_get_signal_handler(sourceAudition), "media_load", MediaLoad, this);
}

void PLSBgmLibraryView::CreateGroup(const QString &groupName)
{
	CategoryButton *btn = new CategoryButton(ui->groupFrame);
	btn->SetDisplayText(groupName);
	connect(btn, &QPushButton::clicked, [=]() { OnGroupButtonClicked(groupName); });

	ui->groupHLayout->addWidget(btn);
	listCatgaoryButton.append(btn);
}

void PLSBgmLibraryView::ScrollToCategoryButton(QPushButton *categoryButton)
{
	if (!categoryButton)
		return;
	int leftPosX = categoryButton->mapToParent(QPoint(0, 0)).x();
	int rightPosX = categoryButton->mapToParent(QPoint(0, 0)).x() + categoryButton->width();
	int value = ui->groupScrollArea->horizontalScrollBar()->value();
	int viewportLeft = value;
	int viewportRight = value + ui->groupScrollArea->width();
	int offset = leftPosX - viewportLeft;
	if (offset < 0) {
		ui->groupScrollArea->horizontalScrollBar()->setValue(viewportLeft + offset);
	}
	offset = rightPosX - viewportRight;
	if (offset > 0) {
		int maxValue = ui->groupScrollArea->horizontalScrollBar()->maximum();
		int newValue = value + offset;
		if (maxValue < newValue)
			newValue = maxValue;
		ui->groupScrollArea->horizontalScrollBar()->setValue(newValue);
	}
}

void PLSBgmLibraryView::RefreshSelectedGroupButtonStyle(const QString &group)
{
	auto iter = listCatgaoryButton.cbegin();
	while (iter != listCatgaoryButton.cend()) {
		auto item = *iter;
		if (!item) {
			++iter;
			continue;
		}
		if (item->text() == group) {
			pls_flush_style(item, STATUS, STATUS_SELECTED);
			ScrollToCategoryButton(item);
			++iter;
			continue;
		}
		pls_flush_style(item, STATUS, STATUS_UNSELECTED);
		++iter;
	}
}

void PLSBgmLibraryView::initToast()
{
	toastView.setParent(this);
	toastView.hide();
	connect(&toastView, &PLSToastMsgFrame::resizeFinished, this, &PLSBgmLibraryView::ResizeToastView);
}

void PLSBgmLibraryView::ShowToastView(const QString &text)
{
	toastView.SetMessage(text);
	toastView.SetShowWidth(this->width() - 2 * PLSDpiHelper::calculate(this, 10));
	toastView.ShowToast();
}

void PLSBgmLibraryView::ResizeToastView()
{
	QPoint pos = ui->frame_bottom->mapTo(this, QPoint(PLSDpiHelper::calculate(dpi, 10), 0));
	toastView.move(pos.x(), pos.y() - toastView.height() - PLSDpiHelper::calculate(dpi, 10));
}

void PLSBgmLibraryView::ShowList(const QString &group)
{
	auto iter = groupWidget.cbegin();
	while (iter != groupWidget.cend()) {
		auto w = iter->second;
		if (!w) {
			++iter;
			continue;
		}
		w->setVisible(false);
		++iter;
	}
	iter = groupWidget.find(group);
	iter->second->show();
}

void PLSBgmLibraryView::CreateMusicList(const QString &group, const BgmLibraryData &listData)
{
	auto scrollArea = new PLSFloatScrollBarScrollArea(this);
	scrollArea->SetScrollBarRightMargin(1);
	scrollArea->setObjectName("musicLibraryList");
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setWidgetResizable(true);
	QWidget *content = new QWidget();
	content->setObjectName("musicLibraryContent");
	scrollArea->setWidget(content);
	QVBoxLayout *layoutContent = new QVBoxLayout(content);
	layoutContent->setAlignment(Qt::AlignTop);
	layoutContent->setContentsMargins(0, 0, 0, 0);
	layoutContent->setSpacing(0);

	auto iter = listData.cbegin();
	std::map<QString, PLSBgmLibraryItem *> listItem;
	while (iter != listData.cend()) {
		PLSBgmLibraryItem *musicItem = nullptr;
		if (group != QString(LIBRARY_HOT_TAB)) {
			auto musicList = musicListItem.at(LIBRARY_HOT_TAB);
			auto findIter = musicList.find(iter->second.title);
			if (findIter != musicList.end()) {
				musicItem = findIter->second;
			}
		}
		if (nullptr == musicItem)
			musicItem = CreateLibraryBgmItemView(iter->second, this);
		layoutContent->addWidget(musicItem);
		musicItem->SetGroup(group);
		listItem.insert({iter->second.title, musicItem});
		++iter;
	}
	musicListItem.insert({group, listItem});
	ui->verticalLayout_list->addWidget(scrollArea);
	groupWidget.insert({group, scrollArea});
}

void PLSBgmLibraryView::UpdateMusiclist(const QString &group)
{
	auto iter = groupWidget.find(group);
	if (iter != groupWidget.end()) {
		auto layout = iter->second->widget()->layout();
		int itemCount = layout->count();
		for (int i = 0; i < itemCount; ++i) {
			layout->removeItem(layout->itemAt(i));
		}

		auto iterFind = musicListItem.find(group);
		if (iterFind != musicListItem.end()) {
			auto getItemIter = iterFind->second.cbegin();
			while (getItemIter != iterFind->second.cend()) {
				layout->addWidget(getItemIter->second);
				++getItemIter;
			}
		}
	}
}

PLSBgmLibraryItem::PLSBgmLibraryItem(const PLSBgmItemData &data_, QWidget *parent) : QFrame(parent), ui(new Ui::PLSBgmLibraryItem), data(data_)
{
	ui->setupUi(this);
	ui->nameLabel->setText(data.title);
	ui->nameLabel->installEventFilter(this);
	ui->itemProducerLabel->setText(data.producer);
	ui->fullButton->setChecked(true);
	ui->loadingBtn->hide();
	ui->checkBtn->hide();
	ui->listenBtn->setToolTip(QTStr("Bgm.library.preview.on"));
	ui->itemDurationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString(data.GetDuration(data.id)));
	SetDurationButtonVisible();
}

PLSBgmLibraryItem::~PLSBgmLibraryItem()
{
	delete ui;
}

QString PLSBgmLibraryItem::GetNameElideString(const QString &name, QLabel *label)
{
	if (!label) {
		return "";
	}

	QFontMetrics fontWidth(label->font());
	if (fontWidth.width(name) > label->width())
		return fontWidth.elidedText(name, Qt::ElideRight, label->width());

	return name;
}

void PLSBgmLibraryItem::SetDurations(const int &full, const int &fiftten, const int &thirty, const int &sixty)
{
	data.fullDuration = full;
	data.fifteenDuration = fiftten;
	data.thirtyDuration = thirty;
	data.sixtyDuration = sixty;
}

void PLSBgmLibraryItem::SetDurationType(int type)
{
	data.id = type;
	ui->itemDurationLabel->setText(data.ConvertIntToTimeString(GetDuration(type)));
}

void PLSBgmLibraryItem::SetDurationTypeSelected(int type)
{
	DurationType enumType = static_cast<DurationType>(type);
	switch (enumType) {
	case DurationType::FifteenSeconds:
		selectedCount++;
		pls_flush_style(ui->fifteenButton, STATUS_SELECTED, true);
		break;
	case DurationType::FullSeconds:
		selectedCount++;
		pls_flush_style(ui->fullButton, STATUS_SELECTED, true);
		break;
	case DurationType::SixtySeconds:
		selectedCount++;
		pls_flush_style(ui->sixtyButton, STATUS_SELECTED, true);
		break;
	case DurationType::ThirtySeconds:
		selectedCount++;
		pls_flush_style(ui->thirtyButton, STATUS_SELECTED, true);
		break;
	default:
		break;
	}
}

void PLSBgmLibraryItem::SetDurationTypeClicked(int type)
{
	DurationType enumType = static_cast<DurationType>(type);
	switch (enumType) {
	case DurationType::FifteenSeconds:
		ui->fifteenButton->setChecked(true);
		SetDurationType(type);
		break;
	case DurationType::FullSeconds:
		ui->fullButton->setChecked(true);
		SetDurationType(type);
		break;
	case DurationType::SixtySeconds:
		ui->sixtyButton->setChecked(true);
		SetDurationType(type);
		break;
	case DurationType::ThirtySeconds:
		ui->thirtyButton->setChecked(true);
		SetDurationType(type);
		break;
	default:
		break;
	}
}

void PLSBgmLibraryItem::SetDurationTypeSelected(const QString &url)
{
	auto durationType = data.urls.at(url);
	SetDurationTypeSelected(durationType);
}

QString PLSBgmLibraryItem::GetTitle() const
{
	return data.title;
}

QString PLSBgmLibraryItem::GetProducer() const
{
	return data.producer;
}

QString PLSBgmLibraryItem::GetGroup() const
{
	return data.group;
}

QString PLSBgmLibraryItem::GetUrl(int type) const
{
	return data.GetUrl(type);
}

int PLSBgmLibraryItem::GetDurationType() const
{
	return data.id;
}

int PLSBgmLibraryItem::GetDuration(int type) const
{
	return data.GetDuration(type);
}

inline bool PLSBgmLibraryItem::IsCurrentSong(const QString &title, int id) const
{
	return (title == data.title) && (id == data.id);
}

const PLSBgmItemData &PLSBgmLibraryItem::GetData() const
{
	return data;
}

void PLSBgmLibraryItem::SetCheckedButtonVisible(bool visible)
{
	isChecked = visible;
	ui->addBtn->setVisible(!isChecked);
	ui->checkBtn->setVisible(isChecked);
}

void PLSBgmLibraryItem::SetDurationButtonVisible()
{
	ui->fullButton->setVisible(data.fullUrl != "");
	ui->fifteenButton->setVisible(data.fifteenUrl != "");
	ui->thirtyButton->setVisible(data.thirtyUrl != "");
	ui->sixtyButton->setVisible(data.sixtyUrl != "");
}

void PLSBgmLibraryItem::SetGroup(const QString &group)
{
	data.group = group;
}

void PLSBgmLibraryItem::SetData(const PLSBgmItemData &data)
{
	this->data = data;
}

bool PLSBgmLibraryItem::ContainUrl(const QString &url)
{
	auto iter = data.urls.find(url);
	return (iter != data.urls.end());
}

void PLSBgmLibraryItem::leaveEvent(QEvent *event)
{
	mouseEnter = false;
	QFrame::leaveEvent(event);
	OnMouseEventChanged(PROPERTY_VALUE_MOUSE_STATUS_NORMAL);
}

void PLSBgmLibraryItem::enterEvent(QEvent *event)
{
	mouseEnter = true;
	QFrame::enterEvent(event);
	OnMouseEventChanged(PROPERTY_VALUE_MOUSE_STATUS_HOVER);
}

bool PLSBgmLibraryItem::eventFilter(QObject *object, QEvent *event)
{
	if (object == ui->nameLabel && event->type() == QEvent::Resize) {
		QTimer::singleShot(0, this, [=]() { ui->nameLabel->setText(GetNameElideString(data.title, ui->nameLabel)); });
	}
	return QFrame::eventFilter(object, event);
}

void PLSBgmLibraryItem::OnMouseEventChanged(const QString &state)
{
	pls_flush_style(this, PROPERTY_NAME_MOUSE_STATUS, state);
}

int PLSBgmLibraryItem::GetUrlCount()
{
	int count = 0;
	auto iter = data.urls.cbegin();
	while (iter != data.urls.cend()) {
		++count;
		++iter;
	}
	return count;
}

void PLSBgmLibraryItem::on_listenBtn_clicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] add Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	emit ListenButtonClickedSignal(this);
}

void PLSBgmLibraryItem::on_checkBtn_clicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] checked Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	emit CheckedButtonClickedSignal(this);
}

void PLSBgmLibraryItem::on_addBtn_clicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] add Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	emit AddButtonClickedSignal(this);
}

void PLSBgmLibraryItem::on_fullButton_clicked(bool checked)
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] full sec Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	data.id = static_cast<int>(DurationType::FullSeconds);

	ui->itemDurationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString(data.fullDuration));
	emit DurationTypeChangeSignal(this);
}

void PLSBgmLibraryItem::onMediaStateChanged(const QString &url, obs_media_state state)
{
	if (!ContainUrl(url))
		return;
	int durationType = data.urls[url];
	switch (state) {
	case OBS_MEDIA_STATE_STOPPED:
	case OBS_MEDIA_STATE_ERROR:
	case OBS_MEDIA_STATE_ENDED:
	case OBS_MEDIA_STATE_NONE: {
		ui->loadingBtn->hide();
		ui->listenBtn->show();
		ui->listenBtn->setToolTip(QTStr("Bgm.library.preview.on"));
		pls_flush_style(ui->listenBtn, PLAY, false);
		pls_flush_style(ui->nameLabel, SELECTED, false);
		pls_flush_style(ui->itemPointLabel, SELECTED, false);
		pls_flush_style(ui->itemProducerLabel, SELECTED, false);
		pls_flush_style(ui->itemDurationLabel, SELECTED, false);
		loadingEvent.stopLoadingTimer();
		SetDurationTypeClicked(durationType);
		break;
	}
	case OBS_MEDIA_STATE_PLAYING: {
		ui->loadingBtn->hide();
		ui->listenBtn->show();
		ui->listenBtn->setToolTip(QTStr("Bgm.library.preview.off"));
		pls_flush_style(ui->listenBtn, PLAY, true);
		pls_flush_style(ui->nameLabel, SELECTED, true);
		pls_flush_style(ui->itemPointLabel, SELECTED, true);
		pls_flush_style(ui->itemProducerLabel, SELECTED, true);
		pls_flush_style(ui->itemDurationLabel, SELECTED, true);
		loadingEvent.stopLoadingTimer();
		SetDurationTypeClicked(durationType);
		break;
	}
	case OBS_MEDIA_STATE_PAUSED:
		break;
	case OBS_MEDIA_STATE_OPENING: {
		ui->loadingBtn->show();
		ui->listenBtn->hide();
		loadingEvent.startLoadingTimer(ui->loadingBtn);
		SetDurationTypeClicked(durationType);
		break;
	}
	default:
		break;
	}
}

void PLSBgmLibraryItem::on_sixtyButton_clicked(bool checked)
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] Sixty sec Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	data.id = static_cast<int>(DurationType::SixtySeconds);

	ui->itemDurationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString(data.sixtyDuration));
	emit DurationTypeChangeSignal(this);
}

void PLSBgmLibraryItem::on_thirtyButton_clicked(bool checked)
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] Thirty sec Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	data.id = static_cast<int>(DurationType::ThirtySeconds);

	ui->itemDurationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString(data.thirtyDuration));
	emit DurationTypeChangeSignal(this);
}

void PLSBgmLibraryItem::on_fifteenButton_clicked(bool checked)
{
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] Fifteen sec Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	data.id = static_cast<int>(DurationType::FifteenSeconds);

	ui->itemDurationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString(data.fifteenDuration));
	emit DurationTypeChangeSignal(this);
}
