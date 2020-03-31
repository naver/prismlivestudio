#include "PLSBackgroundMusicView.h"
#include "PLSBgmItemCoverView.h"
#include "PLSBgmItemView.h"
#include "PLSBgmLibraryView.h"
#include "PLSNetworkMonitor.h"
#include "ui_PLSBackgroundMusicView.h"

#include "action.h"
#include "frontend-api.h"
#include "liblog.h"
#include "log/module_names.h"
#include "main-view.hpp"
#include "pls-app.hpp"
#include "pls-common-define.hpp"
#include "pls/media-info.h"

#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFinalState>
#include <QGraphicsBlurEffect>
#include <QMenu>
#include <QMimeData>
#include <QRadioButton>
#include <QRandomGenerator>
#include <QStateMachine>
#include <ctime>
#include <sstream>

static const char *GEOMETRY_BGM_DATA = "geometryBgm"; //key of the bgm window geometry in global ini
static const char *MAXIMIZED_STATE = "isMaxState";    //key of the bgm window is maximized in global ini
static const char *SHOW_MADE = "showMode";            //key of the bgm window is shown in global ini
static const char *DEFAULT_COVER_IMAGE = ":/images/bgm/bgm-default.png";

static const int COVER_MAX_NUMBER = 15;
static const int FLOW_LAYOUT_MARGIN_LEFT_RIGHT = 19;
static const int FLOW_LAYOUT_MARGIN_TOP_BOTTOM = 19;
static const int FLOW_LAYOUT_H_SPACING = 19;
static const int FLOW_LAYOUT_V_SPACING = 22;
static const int LoadingTimeoutMS = 10000;
const static int COVER_WIDTH = 130;

void SendThread::DoWork(const PLSBgmItemData &data)
{
	SendMusicMetaData(data);
}

PLSBackgroundMusicView::PLSBackgroundMusicView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent), ui(new Ui::PLSBackgroundMusicView)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSBackgroundMusicView, PLSCssIndex::PLSToastMsgFrame});
	dpiHelper.notifyDpiChanged(this, [this](double dpi) {
		ui->playListWidget->SetDpi(dpi);
		ui->coverLabel->DpiChanged(dpi);
	});
	ui->playListWidget->SetDpi(PLSDpiHelper::getDpi(this));

	notifyFirstShow([=]() { this->InitGeometry(); });
	initUI();
	initStateMachine();

	sliderTimer = new QTimer(this);
	connect(sliderTimer, &QTimer::timeout, this, &PLSBackgroundMusicView::SetSliderPos, Qt::QueuedConnection);
	connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &PLSBackgroundMusicView::OnCurrentPageChanged);
	connect(ui->playBtn, &QPushButton::clicked, this, &PLSBackgroundMusicView::OnPlayButtonClicked);
	connect(ui->preBtn, &QPushButton::clicked, this, &PLSBackgroundMusicView::OnPreButtonClicked);
	connect(ui->nextBtn, &QPushButton::clicked, this, &PLSBackgroundMusicView::OnNextButtonClicked);

	connect(ui->loopBtn, &QRadioButton::clicked, this, [=](bool checked) { OnLoopBtnClicked(currentSceneItem, checked); });
	connect(ui->addSourceBtn, &QPushButton::clicked, this, &PLSBackgroundMusicView::OnAddSourceBtnClicked);
	connect(ui->addMusicBtn, &QPushButton::clicked, this, &PLSBackgroundMusicView::OnAddMusicBtnClicked);
	connect(ui->coverLabel, &PLSBgmItemCoverView::CoverPressed, this, [=](const QPoint &point) { move(this->frameGeometry().topLeft() + point); });
	connect(ui->noSongLabel, &DragLabel::CoverPressed, this, [=](const QPoint &point) { move(this->frameGeometry().topLeft() + point); });

	connect(ui->playListWidget, &PLSBgmDragView::MouseDoublePressedSignal, this, &PLSBackgroundMusicView::OnPlayListItemDoublePressed);
	connect(ui->playListWidget, &PLSBgmDragView::RowChanged, this, &PLSBackgroundMusicView::OnPlayListItemRowChanged);
	connect(ui->playListWidget, &PLSBgmDragView::AudioFileDraggedIn, this, &PLSBackgroundMusicView::OnAudioFileDraggedIn);
	connect(ui->playListWidget, &PLSBgmDragView::DelButtonClickedSignal, this, &PLSBackgroundMusicView::OnDelButtonClicked);
	connect(ui->playListWidget, &PLSBgmDragView::LoadingFailed, this, &PLSBackgroundMusicView::OnLoadFailed);
	connect(ui->noPlayListFrame, &DragInFrame::AudioFileDraggedIn, this, &PLSBackgroundMusicView::OnAudioFileDraggedIn);

	connect(ui->playingSlider, SIGNAL(mediaSliderClicked(int)), this, SLOT(SliderClicked(int)), Qt::QueuedConnection);
	connect(ui->playingSlider, SIGNAL(mediaSliderReleased(int)), this, SLOT(SliderReleased(int)));
	connect(ui->playingSlider, SIGNAL(mediaSliderMoved(int)), this, SLOT(SliderMoved(int)), Qt::QueuedConnection);

	obs_frontend_add_event_callback(PLSFrontendEvent, this);
#ifdef _WIN32
	connect(PLSNetworkMonitor::Instance(), &PLSNetworkMonitor::OnNetWorkStateChanged, [=](bool isConnected) { networkAvailable = isConnected; });
#endif
}

PLSBackgroundMusicView::~PLSBackgroundMusicView()
{
	SaveShowModeToConfig();
	obs_frontend_remove_event_callback(PLSFrontendEvent, this);

	PLSBgmDataViewManager::Instance()->DeleteGroupButton();
	PLSBgmDataViewManager::Instance()->ClearCachePlayList();

	if (nullptr != manager)
		manager->deleteLater();

	if (nullptr != sendThreadObj) {
		sendThreadObj->deleteLater();
	}
	sendThread.quit();
	sendThread.wait();

	if (nullptr != coverThreadObj) {
		coverThreadObj->deleteLater();
	}
	coverThread.quit();
	coverThread.wait();

	if (nullptr != checkThreadObj) {
		checkThreadObj->deleteLater();
	}
	checkThread.quit();
	checkThread.wait();
	delete ui;
}

void PLSBackgroundMusicView::InitGeometry()
{
	auto initGeometry = [this](double dpi, bool inConstructor) {
		extern void setGeometrySys(PLSWidgetDpiAdapter * adapter, const QRect &geometry);

		const char *geometry = config_get_string(App()->GlobalConfig(), BGM_CONFIG, GEOMETRY_BGM_DATA);
		if (!geometry || !geometry[0]) {
			const int defaultWidth = 298;
			const int defaultHeight = 800;
			const int mainRightOffest = 5;
			PLSMainView *mainView = App()->getMainView();
			QPoint mainTopRight = App()->getMainView()->mapToGlobal(QPoint(mainView->frameGeometry().width(), 0));
			geometryOfNormal = QRect(mainTopRight.x() + PLSDpiHelper::calculate(dpi, mainRightOffest), mainTopRight.y(), PLSDpiHelper::calculate(dpi, defaultWidth),
						 PLSDpiHelper::calculate(dpi, defaultHeight));
			setGeometrySys(this, geometryOfNormal);
		} else if (inConstructor) {
			QByteArray byteArray = QByteArray::fromBase64(QByteArray(geometry));
			restoreGeometry(byteArray);
			if (config_get_bool(App()->GlobalConfig(), BGM_CONFIG, MAXIMIZED_STATE)) {
				showMaximized();
			}
		}
	};

	PLSDpiHelper dpiHelper;
	dpiHelper.notifyDpiChanged(this, [=](double dpi, double, bool isFirstShow) {
		extern QRect normalShow(PLSWidgetDpiAdapter * adapter, QRect & geometryOfNormal);

		if (isFirstShow) {
			initGeometry(dpi, false);
			if (!isMaxState && !isFullScreenState) {
				normalShow(this, geometryOfNormal);
			}
		}
	});

	initGeometry(PLSDpiHelper::getDpi(this), true);
}

void PLSBackgroundMusicView::AddSource(const QString &sourceName, quint64 sceneItem, bool createNew)
{
	InitSourceSettingsData(sourceName, sceneItem, createNew);
}

void PLSBackgroundMusicView::AddSourceAndRefresh(const QString &sourceName, quint64 sceneItem)
{
	AddSource(sourceName, sceneItem, true);
}

void PLSBackgroundMusicView::RemoveSourceAndRefresh(const QString &sourceName, quint64 sceneItem)
{
	RemoveSource(sourceName, sceneItem);
	UpdateSourceSelectUI();
}

void PLSBackgroundMusicView::RemoveSource(const QString &sourceName, quint64 sceneItem)
{
	//default
	clearUI(sceneItem);

	if (SameWithCurrentSource(sceneItem)) {
		ui->playListWidget->Clear();
		currentSceneItem = 0;
		currentSourceName = "";
	}
}

void PLSBackgroundMusicView::RemoveBgmSourceList(const BgmSourceVecType &sourceList)
{
	for (auto &bgm : sourceList) {
		RemoveSource(bgm.first, bgm.second);
	}
}

void PLSBackgroundMusicView::InitSourceSettingsData(const QString &sourceName, const quint64 &sceneItem, bool createNew, bool setCurrent)
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source) {
		return;
	}
	OBSData settings = obs_data_create();
	obs_source_get_private_data(source, settings);
	if (createNew) {
		currentSceneItem = sceneItem;
		currentSourceName = sourceName;
	}
	UpdateUIBySourceSettings(settings, source, sceneItem, createNew);
	obs_data_release(settings);
}

void PLSBackgroundMusicView::OnCurrentPageChanged(int index)
{
	QWidget *currentPage = ui->stackedWidget->widget(index);
	if (currentPage == ui->playListPage) {
		UpdatePlayListUI(currentSceneItem);
		if (0 == ui->playListWidget->Count()) {
			AddPlayListUI(currentSceneItem);
		}
		ui->addSourceBtn->setVisible(false);
		ui->addMusicBtn->setVisible(true);
		ui->tabWidget->setVisible(true);

		int row = ui->playListWidget->GetCurrentRow();
		if (-1 != row && isVisible()) {
			ui->playListWidget->scrollTo(ui->playListWidget->GetModelIndex(row));
		}

	} else if (currentPage == ui->noSourcePage) {
		ResetControlView();
		ui->playListWidget->Clear();
		ui->sourceNameLabel->SetText("");
		ui->addSourceBtn->setVisible(true);
		ui->addMusicBtn->setVisible(false);
		ui->tabWidget->setVisible(false);
	}
}

void PLSBackgroundMusicView::SliderClicked(int val)
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Music playlist slider", ACTION_CLICK);
	isDragging = true;
	SeekTo(val);
}

void PLSBackgroundMusicView::SliderReleased(int val)
{
	isDragging = false;
}

void PLSBackgroundMusicView::SliderMoved(int val)
{
	SeekTo(val);
}

void PLSBackgroundMusicView::SetSliderPos()
{
	OBSSource source = pls_get_source_by_name(currentSourceName.toStdString().c_str());
	if (!source) {
		return;
	}

	obs_media_state state = obs_source_media_get_state(source);
	bool visible = obs_sceneitem_visible(pls_get_sceneitem_by_pointer_address((void *)currentSceneItem));
	if (state != OBS_MEDIA_STATE_PLAYING && state != OBS_MEDIA_STATE_PAUSED || !visible) {
		return;
	}

	if (obs_source_media_is_update_done(source)) {
		float time = (float)obs_source_media_get_time(source);
		float duration = (float)obs_source_media_get_duration(source);
		float sliderPosition = 0.0f;

		sliderPosition = (time / duration) * (float)ui->playingSlider->maximum();
		ui->currentTimeLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString((int)(time / 1000.0f)));
		ui->durationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString((int)(duration / 1000.0f)));

		if (isDragging)
			return;

		ui->playingSlider->setValue((int)(sliderPosition));
	}
}

void PLSBackgroundMusicView::UpdateUIBySourceSettings(obs_data_t *settings, OBSSource source, const quint64 &sceneItem, bool createNew)
{
	if (!settings) {
		return;
	}

	if (!SameWithCurrentSource(sceneItem)) {
		return;
	}

	bool playInOrder = obs_data_get_bool(settings, PLAY_IN_ORDER);
	if (playInOrder) {
		if (mode != PlayMode::InOrderMode) {
			SetCurrentPlayMode(static_cast<int>(PlayMode::InOrderMode));
		}
	}

	bool random = obs_data_get_bool(settings, RANDOM_PLAY);
	if (random) {
		if (mode != PlayMode::RandomMode) {
			SetCurrentPlayMode(static_cast<int>(PlayMode::RandomMode));
		}
	}

	if (!playInOrder && !random) {
		SetCurrentPlayMode(static_cast<int>(PlayMode::InOrderMode));
	}

	bool isLoop = true;
	if (!createNew) {
		isLoop = obs_data_get_bool(settings, IS_LOOP);
	}

	ui->loopBtn->setChecked(isLoop);
	SetLoop(sceneItem, isLoop);
	AddPlayListUI(sceneItem);
}

void PLSBackgroundMusicView::ClearUrlInfo()
{
	PLSBasic *main = PLSBasic::Get();
	BgmSourceVecType playList = main->EnumAllBgmSource();
	for (auto iter = playList.begin(); iter != playList.end(); ++iter) {
		obs_source_t *source = GetSource(iter->second);
		SetUrlInfo(source, PLSBgmItemData());
	}
}

void PLSBackgroundMusicView::OnSceneChanged()
{
	QString name{};
	quint64 item{};
	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	if (!main) {
		return;
	}
	main->GetSelectBgmSourceName(name, item);
	BgmSourceVecType sourceList = main->GetCurrentSceneBgmSourceList();
	UpdateSourceList(name, item, sourceList);

	ui->sourceNameLabel->SetText(currentSourceName);
	UpdateSourceSelectUI();
}

void PLSBackgroundMusicView::UpdateLoadingStartState(const QString &sourceName)
{
	if (!CheckNetwork()) {
		return;
	}
	int row = ui->playListWidget->GetCurrentRow();
	if (-1 != row) {
		StopLoadingTimer();
		PLSBgmItemDelegate::totalFrame(8);
		PLSBgmItemDelegate::setCurrentFrame(1);
		indexLoading = row;
		ui->playListWidget->SetMediaStatus(row, MediaStatus::stateLoading);
		StartLoadingTimer();
	}
	isLoading = true;
	SetPlayerControllerStatus(currentSceneItem);
}

void PLSBackgroundMusicView::UpdateLoadingEndState(const QString &sourceName)
{
	if (!SameWithCurrentSource(sourceName)) {
		return;
	}

	if (!CheckNetwork()) {
		return;
	}

	int row = ui->playListWidget->GetCurrentRow();
	if (-1 != row) {
		StopLoadingTimer();

		obs_source_t *source = pls_get_source_by_name(sourceName.toStdString().c_str());
		obs_media_state state = obs_source_media_get_state(source);
		if (state == OBS_MEDIA_STATE_PLAYING) {
			indexLoading = row;
			PLSBgmItemDelegate::totalFrame(21);
			PLSBgmItemDelegate::setCurrentFrame(1);
			ui->playListWidget->SetMediaStatus(row, MediaStatus::statePlaying);
			StartLoadingTimer(100);
		} else if (state == OBS_MEDIA_STATE_PAUSED) {
			ui->playListWidget->SetMediaStatus(row, MediaStatus::statePause);
		}
	}
	SetPlayerControllerStatus(currentSceneItem);
}

void PLSBackgroundMusicView::OnNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
	if (QNetworkAccessManager::Accessible != accessible) {
		//OnNoNetwork();
	}
}

void PLSBackgroundMusicView::OnMediaStateChanged(const QString &name, obs_media_state state)
{
	OBSSource source = pls_get_source_by_name(name.toStdString().c_str());
	if (!source) {
		return;
	}

	switch (state) {

	case OBS_MEDIA_STATE_STOPPED:
		UpdateStopUIState(name);
		break;
	case OBS_MEDIA_STATE_OPENING:
		UpdateOpeningUIState(name);
		break;
	case OBS_MEDIA_STATE_PLAYING:
		UpdatePlayingUIState(name);
		break;
	case OBS_MEDIA_STATE_PAUSED:
		UpdatePauseUIState(name);
		break;
	case OBS_MEDIA_STATE_ERROR:
		UpdateErrorUIState(name, true);
		break;
	default:
		break;
	}
}

void PLSBackgroundMusicView::OnPropertiesChanged(const QString &name)
{
	OBSSource source = pls_get_source_by_name(name.toStdString().c_str());
	if (!source) {
		return;
	}

	if (!SameWithCurrentSource(name)) {
		return;
	}

	obs_media_state state = obs_source_media_get_state(source);
	if (state != OBS_MEDIA_STATE_PLAYING && state != OBS_MEDIA_STATE_PAUSED && state != OBS_MEDIA_STATE_OPENING) {
		return;
	}

	obs_data_t *settings = pls_get_source_setting(source);
	if (!settings) {
		return;
	}
}

void PLSBackgroundMusicView::UpdatePlayingUIState(const QString &name)
{
	if (!SameWithCurrentSource(name)) {
		return;
	}

	int row = ui->playListWidget->GetCurrentRow();
	if (-1 != row) {
		StopLoadingTimer();
		indexLoading = row;
		PLSBgmItemDelegate::totalFrame(21);
		PLSBgmItemDelegate::setCurrentFrame(1);
		ui->playListWidget->SetMediaStatus(row, MediaStatus::statePlaying);
		StartLoadingTimer(100);
		ui->playingSlider->setMediaSliderEnabled(true);
		ui->playingSlider->setEnabled(true);
	}

	SetPlayerControllerStatus(currentSceneItem);
	ShowCoverGif(true);
}

void PLSBackgroundMusicView::UpdatePauseUIState(const QString &name)
{
	if (!SameWithCurrentSource(name)) {
		return;
	}

	StopLoadingTimer();
	int row = ui->playListWidget->GetCurrentRow();
	if (-1 != row) {
		ui->playListWidget->SetMediaStatus(row, MediaStatus::statePause);
	}
	SetPlayerControllerStatus(currentSceneItem);
	ShowCoverGif(false);
}

void PLSBackgroundMusicView::UpdateStopUIState(const QString &name)
{
	if (!SameWithCurrentSource(name)) {
		if (!CheckNetwork()) {
			OnInvalidSongs(name);
		}
		return;
	}

	PLSBgmItemData data = GetCurrentPlayListData();
	if (!CheckNetwork() && !data.isLocalFile) {
		OnNoNetwork(QTStr("Bgm.No.Network.Toast"), data);
	}

	StopLoadingTimer();
	StopSliderPlayingTimer();
	pls_flush_style(ui->playBtn, STATUS_STATE, STATUS_PLAY);
	ui->playBtn->setToolTip(QTStr("Bgm.Play"));

	int row = ui->playListWidget->GetCurrentRow();
	if (-1 != row) {
		ui->playListWidget->SetMediaStatus(row, MediaStatus::stateNormal);
	}

	SetPlayerControllerStatus(currentSceneItem);
	ShowCoverGif(false);
}

void PLSBackgroundMusicView::UpdateOpeningUIState(const QString &name)
{
	OnUpdateOpeningUIState(name);

	OBSOutput streamingOutput = obs_frontend_get_streaming_output();
	obs_output_release(streamingOutput);
	if (obs_output_active(streamingOutput)) {
		if (!sendThreadObj) {
			sendThreadObj = new SendThread;
			sendThreadObj->moveToThread(&sendThread);
			sendThread.start();
		}
		PLSBgmItemData data = GetCurrentPlayListDataBySettings(name);
		QMetaObject::invokeMethod(sendThreadObj, "DoWork", Qt::QueuedConnection, Q_ARG(PLSBgmItemData, data));
	}
}

void PLSBackgroundMusicView::UpdateErrorUIState(const QString &name, bool gotoNextSongs)
{
	if (!SameWithCurrentSource(name)) {
		OnInvalidSongs(name);
		return;
	}

	PLSBgmItemData data = GetCurrentPlayListData();
	QString toast = CheckNetwork() ? QTStr("Bgm.Songs.Invalid.Toast") : QTStr("Bgm.No.Network.Toast");
	OnNoNetwork(toast, data);
	ShowCoverGif(false);
}

void PLSBackgroundMusicView::OnUpdateOpeningUIState(const QString &name)
{
	OBSSource source = pls_get_source_by_name(name.toStdString().c_str());
	if (source) {
		obs_data_t *settings = obs_data_create();
		obs_data_set_string(settings, "method", "bgm_get_opening");
		obs_source_set_private_data(source, settings);
		obs_data_release(settings);
	}

	if (!SameWithCurrentSource(name)) {
		return;
	}

	if (!coverThreadObj) {
		coverThreadObj = new GetCoverThread;
		coverThreadObj->moveToThread(&coverThread);
		connect(coverThreadObj, &GetCoverThread::Finished, this, &PLSBackgroundMusicView::UpdateBgmCoverPath, Qt::QueuedConnection);
		connect(
			coverThreadObj, &GetCoverThread::GetPreviewImage, this,
			[=](const QImage &image, const PLSBgmItemData &data) {
				PLSBgmItemData curData = GetCurrentPlayListData();
				if (curData.id != data.id || curData.GetUrl(curData.id) != data.GetUrl(data.id)) {
					return;
				}
				if (image.isNull()) {
					auto temp = data;
					temp.coverPath = DEFAULT_COVER_IMAGE;
					ui->coverLabel->SetCoverPath(temp.coverPath, !temp.isLocalFile);
					UpdateBgmCoverPath(temp);
				} else {
					ui->coverLabel->SetImage(image);
				}
			},
			Qt::QueuedConnection);
		coverThread.start();
	}

	int row = ui->playListWidget->GetCurrentRow();
	if (-1 != row) {
		ui->playListWidget->SetMediaStatus(row, MediaStatus::stateNormal);
	}

	PLSBgmItemData data = GetCurrentPlayListDataBySettings();
	ui->playingSlider->setMediaSliderEnabled(false);
	ui->playingSlider->setEnabled(false);
	ui->playingSlider->setValue(0);
	ui->currentTimeLabel->setText("00:00");
	ui->coverLabel->SetMusicInfo(data.title, data.producer);
	ui->playListWidget->SetCurrentRow(data);

	ShowCoverGif(false);
	ShowCoverImage(data);
	SetPlayerControllerStatus(currentSceneItem);
}

void PLSBackgroundMusicView::UpdateLoadUIState(const QString &name, bool load, bool isOpen)
{
	if (!CheckNetwork()) {
		return;
	}
	if (!SameWithCurrentSource(name)) {
		if (!CheckNetwork()) {
			OnLoadFailed(name);
		}
		isLoading = false;
		return;
	}

	if (load) {
		UpdateLoadingStartState(name);
		return;
	}

	UpdateLoadingEndState(name);
}

void PLSBackgroundMusicView::SetCurrentPlayMode(const int &mode)
{
	this->mode = static_cast<PlayMode>(mode);

	QString tooltip = (static_cast<PlayMode>(mode) == PlayMode::InOrderMode ? QTStr("Bgm.PlayInOrder") : QTStr("Bgm.Shuffle"));
	ui->refreshBtn->setToolTip(tooltip);

	emit PlayModeChanged(this->mode, false);
}

void PLSBackgroundMusicView::SetSourceSelect(const QString &sourceName, quint64 sceneItem, bool selected)
{
	PLSBasic *main = PLSBasic::Get();
	if (!main) {
		return;
	}
	obs_sceneitem_t *sceneitem = pls_get_sceneitem_by_pointer_address(main->GetCurrentScene(), (void *)sceneItem);
	if (!sceneitem || !obs_sceneitem_visible(sceneitem)) {
		return;
	}

	if (!SameWithCurrentSource(sourceName)) {
		if (isLoading && !CheckNetwork()) {
			OnLoadFailed(currentSourceName);
		}
		currentSceneItem = sceneItem;
		currentSourceName = sourceName;
		isLoading = false;
		InitSourceSettingsData(currentSourceName, currentSceneItem, false);
	}

	currentSceneItem = sceneItem;
	ui->sourceNameLabel->SetText(currentSourceName);
	ui->stackedWidget->setCurrentWidget(ui->playListPage);
	UpdatePlayListUI(sceneItem);
	SetPlayerControllerStatus(sceneItem);
	PLSBgmItemData data = GetCurrentPlayListDataBySettings(sourceName);
	ShowCoverImage(data);
	SetPlayListStatus(data);
}

void PLSBackgroundMusicView::SetSourceVisible(const QString &sourceName, quint64 sceneItem, bool visible)
{
	UpdateSourceSelectUI();
}

void PLSBackgroundMusicView::UpdateSourceList(const QString &sourceName, quint64 sceneItem, const BgmSourceVecType &sourceList)
{
	for (auto &bgm : sourceList) {
		if (!obs_sceneitem_visible(pls_get_sceneitem_by_pointer_address((void *)sceneItem))) {
			continue;
		}
		AddSource(bgm.first, bgm.second, false);
	}

	if (sourceName.isEmpty()) {
		SetPlayerControllerStatus(sceneItem);
		UpdatePlayListUI(sceneItem);
	}
}

void PLSBackgroundMusicView::RenameSourceName(const quint64 &item, const QString &newName, const QString &prevName)
{
	if (prevName == currentSourceName) {
		currentSourceName = newName;
		ui->sourceNameLabel->SetText(currentSourceName);
	}
}

void PLSBackgroundMusicView::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}

void PLSBackgroundMusicView::showEvent(QShowEvent *event)
{
	PLSDialogView::showEvent(event);
	SaveShowModeToConfig();

	OnRetryNetwork();
	UpdateSourceSelectUI();
	emit bgmViewVisibleChanged(true);
}

void PLSBackgroundMusicView::hideEvent(QHideEvent *event)
{
	PLSDialogView::hideEvent(event);

	SaveShowModeToConfig();
	emit bgmViewVisibleChanged(false);
}

void PLSBackgroundMusicView::resizeEvent(QResizeEvent *event)
{
	PLSDialogView::resizeEvent(event);
	if (toastView.isVisible()) {
		QTimer::singleShot(0, this, [=]() { ResizeToastView(); });
	}
}

bool PLSBackgroundMusicView::eventFilter(QObject *watcher, QEvent *event)
{
	return PLSDialogView::eventFilter(watcher, event);
}

void PLSBackgroundMusicView::OnDelButtonClicked(const PLSBgmItemData &data)
{
	if (data.title.isEmpty()) {
		return;
	}

	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] Del Button").arg(data.title).toStdString().c_str(), ACTION_CLICK);
	ui->playListWidget->Remove(data);
	auto index = ui->playListWidget->indexAt(ui->playListWidget->mapFromGlobal(QCursor::pos()));
	ui->playListWidget->UpdataData(index.row(), QVariant::fromValue(RowStatus::stateHover), CustomDataRole::RowStatusRole);
	ui->playListWidget->update(index);
	obs_data_t *delData = obs_data_create();
	obs_data_set_string(delData, "method", "bgm_remove");
	obs_data_set_string(delData, "remove_url", data.GetUrl(data.id).toStdString().c_str());
	obs_data_set_string(delData, BGM_DURATION_TYPE, QString::number(data.id).toStdString().c_str());

	obs_source_set_private_data(source, delData);
	obs_data_release(delData);

	UpdateCurrentPlayStatus(currentSourceName);
	UpdatePlayListUI(currentSceneItem);
	Save();
	RefreshPropertyWindow();
}

void PLSBackgroundMusicView::OnNoNetwork(const QString &toast, const PLSBgmItemData &data_)
{
	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	obs_media_state state = obs_source_media_get_state(source);
	if (state != OBS_MEDIA_STATE_PLAYING && state != OBS_MEDIA_STATE_PAUSED && state != OBS_MEDIA_STATE_OPENING && state != OBS_MEDIA_STATE_ERROR && state != OBS_MEDIA_STATE_STOPPED) {
		return;
	}

	int row = ui->playListWidget->GetCurrentRow();
	if (-1 != row) {
		PLSBgmItemData data = data_;
		data.isDisable = true;
		data.isCurrent = false;
		ui->playListWidget->UpdataData(row, data);
		ui->playListWidget->SetMediaStatus(row, MediaStatus::stateInvalid);
		data.isLocalFile ? ShowToastView(QTStr("Bgm.Songs.Invalid.Toast").arg(data.title)) : ShowToastView(toast.arg(data.title));
		StopSliderPlayingTimer();
		StopLoadingTimer();
		OnInvalidSongs(currentSourceName);
	} else {
		ResetControlView();
	}
}

void PLSBackgroundMusicView::OnInvalidSongs(const QString &name)
{
	OBSSource source = pls_get_source_by_name(name.toStdString().c_str());
	if (!source) {
		return;
	}

	PLSBgmItemData data = GetCurrentPlayListDataBySettings(name);
	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "method", "bgm_disable");
	obs_data_set_string(settings, BGM_URL, data.GetUrl(data.id).toStdString().c_str());
	obs_data_set_string(settings, BGM_DURATION_TYPE, QString::number(data.id).toStdString().c_str());
	obs_data_set_bool(settings, "goto_next_songs", true);

	obs_source_set_private_data(source, settings);
	obs_data_release(settings);
}

void PLSBackgroundMusicView::OnRetryNetwork()
{
	if (!CheckNetwork()) {
		return;
	}

	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	QVector<PLSBgmItemData> availableDatas;
	bool needUpdateDisableConfig = false;
	for (int i = 0; i < ui->playListWidget->Count(); i++) {
		PLSBgmItemData data = ui->playListWidget->Get(i);
		if (data.isDisable) {
			if (data.isLocalFile) {
				CreateCheckValidThread();
				QMetaObject::invokeMethod(checkThreadObj, "CheckUrlAvailable", Qt::QueuedConnection, Q_ARG(PLSBgmItemData, data));
				continue;
			}

			needUpdateDisableConfig = true;
			ui->playListWidget->UpdataData(i, QVariant::fromValue(MediaStatus::stateNormal), CustomDataRole::MediaStatusRole);
			availableDatas.push_back(data);
		}
	}
	if (needUpdateDisableConfig) {
		RefreshMulicEnabledPlayList(currentSceneItem, availableDatas);
	}
	SetPlayListStatus(ui->playListWidget->GetCurrent());
	Save();
}

void PLSBackgroundMusicView::OnPlayListItemDoublePressed(const QModelIndex &index)
{
	PLSBgmItemData data = ui->playListWidget->GetData(index);
	PLS_UI_STEP(MAIN_BGM_MODULE, QString("[%1] music").arg(data.title).toStdString().c_str(), ACTION_DBCLICK);
	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	if (data.isDisable) {
		return;
	}

	PLS_INFO(MAIN_BGM_MODULE, "switch to next music: %s", data.title.toStdString().c_str());
	SetUrlInfo(source, data);
}

void PLSBackgroundMusicView::OnPlayListItemRowChanged(const int &srcIndex, const int &destIndex)
{
	if (srcIndex == destIndex) {
		return;
	}

	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "method", "bgm_row_changed");
	obs_data_set_int(settings, "src_index", srcIndex);
	obs_data_set_int(settings, "dest_index", destIndex);
	obs_source_set_private_data(source, settings);

	obs_source_get_private_data(source, settings);
	QVector<PLSBgmItemData> datas = GetPlayListData(settings);
	obs_data_release(settings);

	ui->playListWidget->UpdateWidget(datas);

	PLSBgmItemData data = GetCurrentPlayListData();
	SetPlayListStatus(data);
	SetPlayerControllerStatus(currentSceneItem);
	UpdateCurrentPlayStatus(currentSourceName);
	Save();
	RefreshPropertyWindow();
}

static QString GetTempImageFilePath(const QString &suffix, const int &index)
{
	QDir temp = QDir::temp();
	temp.mkdir("musicImages");
	temp.cd("musicImages");
	QString tempImageFilePath = temp.absoluteFilePath(QString("musicImages-%1").arg(QDateTime::currentMSecsSinceEpoch() + index) + suffix);
	return tempImageFilePath;
}

static QImage CaptureImage(uint32_t width, uint32_t height, char *data, int size, const int &index)
{
	if (!data) {
		return QImage();
	}

	uchar *buffer = (uchar *)malloc(size);
	if (!buffer) {
		return QImage();
	}

	if (width * height * 4 != size) {
		PLS_WARN(MAIN_BGM_MODULE, "cover image size was not correct, width=[%d], height=[%d], size=[%d].", width, height, size);
	}

	memset(buffer, 0, size);
	memmove(buffer, data, size);
	QImage image(buffer, width, height, QImage::Format_RGBA8888);
	QImage imageCopy = image.copy();
	free(buffer);
	return imageCopy;
}

void PLSBackgroundMusicView::OnAudioFileDraggedIn(const QStringList &paths)
{
	if (0 == paths.size()) {
		return;
	}
	PLS_INFO(MAIN_BGM_MODULE, QString("Add %1 Local Songs").arg(paths.size()).toStdString().c_str());

	QVector<PLSBgmItemData> datas;
	int index = 0;
	for (auto &path : paths) {

		PLSBgmItemData data;
		data.id = 0;
		if (ui->playListWidget->Existed(path)) {
			data.id = ui->playListWidget->GetId(path);
		}
		data.SetUrl(path, data.id);

		media_info_t media_info;
		memset(&media_info, 0, sizeof(media_info_t));
		bool open = mi_open(&media_info, path.toStdString().c_str(), MI_OPEN_DIRECTLY);
		if (open && 0 == path.right(3).toLower().compare("mp3")) {
			data.title = mi_get_string(&media_info, "title");
			data.producer = mi_get_string(&media_info, "artist");
			data.SetDuration(data.id, mi_get_int(&media_info, "duration") / 1000);
			data.haveCover = mi_get_bool(&media_info, "has_cover");
		}
		if (data.title.isEmpty()) {
			data.title = path.mid(path.lastIndexOf('/') + 1);
		}
		if (data.producer.isEmpty()) {
			data.producer = "Unknown";
		}
		if (0 == data.GetDuration(data.id)) {
			data.SetDuration(data.id, mi_get_int(&media_info, "duration") / 1000);
		}
		data.isLocalFile = true;

		datas.push_back(data);
		mi_free(&media_info);
	}

	OnAddCachePlayList(datas);
}

void PLSBackgroundMusicView::OnAddCachePlayList(const QVector<PLSBgmItemData> &datas_)
{
	QVector<PLSBgmItemData> datas = datas_;

	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	obs_data_t *settings = obs_data_create();
	obs_data_array_t *playLists = obs_data_array_create();

	for (auto &data : datas) {
		obs_data_t *playList = obs_data_create();
		if (!data.group.isEmpty()) {
			data.coverPath = QString(":/images/bgm/group-%1.png").arg(data.group.toLower());
		}

		SetUrlInfoToSettings(playList, data);
		obs_data_array_insert(playLists, 0, playList);
		obs_data_release(playList);
	}
	obs_data_set_array(settings, PLAY_LIST, playLists);
	obs_data_set_string(settings, "method", "bgm_insert_playlist");
	obs_source_set_private_data(source, settings);

	obs_data_array_release(playLists);
	obs_data_release(settings);

	ui->playListWidget->InsertWidget(datas);
	UpdateCurrentPlayStatus(currentSourceName);
	UpdatePlayListUI(currentSceneItem);
	Save();
	RefreshPropertyWindow();
}

void PLSBackgroundMusicView::OnPlayButtonClicked()
{
	OBSSource source = pls_get_source_by_name(currentSourceName.toStdString().c_str());
	if (!source) {
		return;
	}
	obs_media_state state = obs_source_media_get_state(source);
	switch (state) {
	case OBS_MEDIA_STATE_STOPPED:
	case OBS_MEDIA_STATE_ENDED:
	case OBS_MEDIA_STATE_NONE:
		obs_source_media_restart(source);
		PLS_UI_STEP(MAIN_BGM_MODULE, "Music Playlist Playing Button", ACTION_CLICK);
		break;
	case OBS_MEDIA_STATE_PLAYING:
		obs_source_media_play_pause(source, true);
		PLS_UI_STEP(MAIN_BGM_MODULE, "Music Playlist Pause Button", ACTION_CLICK);
		break;
	case OBS_MEDIA_STATE_PAUSED:
		obs_source_media_play_pause(source, false);
		PLS_UI_STEP(MAIN_BGM_MODULE, "Music Playlist Playing Button", ACTION_CLICK);
		break;
	default:
		break;
	}
}

void PLSBackgroundMusicView::OnPreButtonClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Pre Button", ACTION_CLICK);
	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	obs_source_media_previous(source);
}

void PLSBackgroundMusicView::OnNextButtonClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Next Button", ACTION_CLICK);
	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	obs_source_media_next(source);
}

void PLSBackgroundMusicView::OnLoopBtnClicked(const quint64 &sceneItem, bool checked)
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "loop Button", ACTION_CLICK);

	SetLoop(sceneItem, checked);
}

void PLSBackgroundMusicView::OnLocalFileBtnClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Add Local File Button", ACTION_CLICK);

	QString filter("(*.mp3 *.aac *.ogg *.wav)");
	QStringList paths;

	if (localFilePath.isEmpty()) {
		localFilePath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
		paths = QFileDialog::getOpenFileNames(this, QString(), localFilePath, filter);
	} else {
		paths = QFileDialog::getOpenFileNames(this, QString(), "", filter);
	}

	OnAudioFileDraggedIn(paths);
}

void PLSBackgroundMusicView::OnLibraryBtnClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Add Prism Free Music Button", ACTION_CLICK);

	auto libraryView = new PLSBgmLibraryView(this);
	const int defaultWidth = 720;
	const int defaultHeight = 650;
	libraryView->resize(PLSDpiHelper::calculate(this, defaultWidth), PLSDpiHelper::calculate(this, defaultHeight));
	libraryView->setAttribute(Qt::WA_DeleteOnClose);
	connect(libraryView, &PLSBgmLibraryView::AddCachePlayList, this, &PLSBackgroundMusicView::OnAddCachePlayList);
	libraryView->exec();
}

void PLSBackgroundMusicView::OnAddSourceBtnClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Add source Button", ACTION_CLICK);

	PLSBasic *main = PLSBasic::Get();
	if (!main) {
		return;
	}

	main->AddSource(BGM_SOURCE_ID);
}

void PLSBackgroundMusicView::OnAddMusicBtnClicked()
{
	PLS_UI_STEP(MAIN_BGM_MODULE, "Add music Button", ACTION_CLICK);

	QMenu popup(ui->addMusicBtn);
	popup.setObjectName("addMusicMenu");
	QAction *addLocalAction = new QAction(QTStr("Bgm.Add.Local.File"));
	QAction *addLibraryAction = new QAction(QTStr("Bgm.Add.Free.Music"));
	connect(addLocalAction, &QAction::triggered, this, &PLSBackgroundMusicView::OnLocalFileBtnClicked);
	connect(addLibraryAction, &QAction::triggered, this, &PLSBackgroundMusicView::OnLibraryBtnClicked);

	popup.addAction(addLocalAction);
	popup.addAction(addLibraryAction);

	popup.exec(QCursor::pos());
}

void PLSBackgroundMusicView::OnLoadFailed(const QString &name)
{
	UpdateLoadingEndState(name.isEmpty() ? currentSourceName : name);
	UpdateErrorUIState(name.isEmpty() ? currentSourceName : name, true);
}

void PLSBackgroundMusicView::initUI()
{
	ui->setupUi(this->content());
	ui->currentTimeLabel->setText("00:00");
	ui->stackedWidget->setCurrentWidget(ui->noSourcePage);
	ui->addSourceBtn->setVisible(true);
	ui->addMusicBtn->setVisible(false);
	ui->playListFrame->setVisible(false);
	ui->tabWidget->setVisible(false);
	ui->verticalLayout->setAlignment(Qt::AlignTop);
	ui->loadingBtn->hide();
	ui->infoframe->setCursor(Qt::ArrowCursor);
	ui->noticeLabel->setContentsMargins(20, 0, 20, 0);
	ui->preBtn->setToolTip(QTStr("Bgm.Previous"));
	ui->nextBtn->setToolTip(QTStr("Bgm.Next"));
	ui->loopBtn->setToolTip(QTStr("Bgm.Repeat"));
	ui->refreshBtn->setToolTip(QTStr("Bgm.PlayInOrder"));
	ResetControlView();
	InitToast();
	pls_flush_style(ui->playBtn, STATUS_STATE, STATUS_PLAY);
	ui->playBtn->setToolTip(QTStr("Bgm.Play"));
	this->setAttribute(Qt::WA_AlwaysShowToolTips, true);

	setHasMaxResButton(true);
	setWindowTitle(QTStr("Bgm.Title"));

	connect(&timerLoading, &QTimer::timeout, [=]() {
		if (indexLoading != -1) {
			PLSBgmItemDelegate::nextLoadFrame();
			auto index = ui->playListWidget->model()->index(indexLoading, 0);
			auto data = ui->playListWidget->model()->data(index, CustomDataRole::MediaStatusRole).value<MediaStatus>();
			ui->playListWidget->update(index);
			if (data == MediaStatus::stateLoading) {
				if ((os_gettime_ns() - loadingTimeout) / 1000000 >= LoadingTimeoutMS) {
					PLSBgmItemData data = ui->playListWidget->Get(indexLoading);
					PLS_INFO(MAIN_BGM_MODULE, "Loading [%s] timeout.", data.title.toStdString().c_str());
					OnLoadFailed(currentSourceName);
				}
			}
		}
	});

	if (nullptr == manager) {
		manager = new QNetworkAccessManager;
		networkAvailable = PLSNetworkMonitor::Instance()->IsInternetAvailable();
#ifndef _WIN32
		connect(manager, &QNetworkAccessManager::networkAccessibleChanged, [=](QNetworkAccessManager::NetworkAccessibility accessible) { networkAvailable = accessible; });
#endif
	}
}

void PLSBackgroundMusicView::clearUI(quint64 sceneItem)
{
	SetPlayerControllerStatus(sceneItem);
	SetCurrentPlayMode(static_cast<int>(PlayMode::InOrderMode));

	UpdatePlayListUI(sceneItem);
	ui->loadingBtn->setVisible(false); //for play btn
	loadingEvent.stopLoadingTimer();
}

void PLSBackgroundMusicView::Save()
{
	PLSBasic *main = PLSBasic::Get();
	if (main) {
		PLSBasic::Get()->SaveProject();
	}
}

void PLSBackgroundMusicView::SetLoop(const quint64 &sceneItem, bool checked)
{
	pls_flush_style(ui->loopBtn, STATUS_PRESSED, checked);

	OBSData settings = obs_data_create();
	obs_source_t *source = GetSource(sceneItem);
	if (!source) {
		return;
	}
	obs_data_set_string(settings, "method", "bgm_loop");
	obs_data_set_bool(settings, IS_LOOP, checked);
	obs_source_set_private_data(source, settings);
	obs_data_release(settings);
}

void PLSBackgroundMusicView::CreateCheckValidThread()
{
	if (!checkThreadObj) {
		checkThreadObj = new CheckValidThread;
		connect(
			checkThreadObj, &CheckValidThread::checkFinished, this,
			[=](const PLSBgmItemData &data, bool result) {
				if (!result) {
					return;
				}
				ui->playListWidget->UpdataData(ui->playListWidget->GetRow(data), QVariant::fromValue(MediaStatus::stateNormal), CustomDataRole::MediaStatusRole);
				QVector<PLSBgmItemData> vecs;
				vecs.push_back(data);
				RefreshMulicEnabledPlayList(currentSceneItem, vecs);
			},
			Qt::QueuedConnection);
		checkThreadObj->moveToThread(&checkThread);
		checkThread.start();
	}
}

void PLSBackgroundMusicView::initStateMachine()
{
	QState *startState = new QState;

	auto stateChanged = [=](const PlayMode &mode, bool ignoreUpdate = true) {
		this->mode = mode;
		QString tooltip = (static_cast<PlayMode>(mode) == PlayMode::InOrderMode ? QTStr("Bgm.PlayInOrder") : QTStr("Bgm.Shuffle"));
		ui->refreshBtn->setToolTip(tooltip);
		PLS_INFO(MAIN_BGM_MODULE, "Set Play Mode: %s", QMetaEnum::fromType<PlayMode>().valueToKey(static_cast<int>(mode)));
		if (!ignoreUpdate) {
			int imode = static_cast<int>(mode);
			OBSSource source = pls_get_source_by_name(currentSourceName.toStdString().c_str());
			if (!source) {
				return;
			}
			OBSData settings = obs_data_create();
			obs_data_set_string(settings, "method", "bgm_play_mode");
			obs_data_set_bool(settings, PLAY_IN_ORDER, imode == 0);
			obs_data_set_bool(settings, RANDOM_PLAY, imode == 1);
			obs_source_set_private_data(source, settings);
			obs_data_release(settings);
		}
	};

	inOrderState = new QState(startState);
	connect(inOrderState, &QState::entered, [=]() {
		pls_flush_style(ui->refreshBtn, "playMode", "inOrder");
		stateChanged(PlayMode::InOrderMode, false);
	});

	randomState = new QState(startState);
	connect(randomState, &QState::entered, [=]() {
		pls_flush_style(ui->refreshBtn, "playMode", "random");
		stateChanged(PlayMode::RandomMode, false);
	});

	QFinalState *finalState = new QFinalState;
	startState->setInitialState(inOrderState);
	startState->addTransition(this, &PLSBackgroundMusicView::PlayModeChanged, finalState);

	inOrderState->addTransition(ui->refreshBtn, &QPushButton::clicked, randomState);
	randomState->addTransition(ui->refreshBtn, &QPushButton::clicked, inOrderState);

	connect(&machine, &QStateMachine::finished, [=]() {
		startState->setInitialState(GetStateMachine(mode));
		machine.setInitialState(startState);
		machine.start();
	});
	machine.addState(startState);
	machine.addState(finalState);
	machine.setInitialState(startState);
	machine.start();
}

QState *PLSBackgroundMusicView::GetStateMachine(const PlayMode &mode)
{
	switch (mode) {
	case PlayMode::InOrderMode:
		return inOrderState;
	case PlayMode::RandomMode:
		return randomState;
	default:
		return nullptr;
	}
}

obs_source_t *PLSBackgroundMusicView::GetSource(const quint64 &sceneItem)
{
	PLSBasic *main = PLSBasic::Get();
	if (!main) {
		return nullptr;
	}

	return obs_sceneitem_get_source(pls_get_sceneitem_by_pointer_address(main->GetCurrentScene(), (void *)sceneItem));
}

bool PLSBackgroundMusicView::SameWithCurrentSource(const QString &sourceName)
{
	return pls_get_source_by_name(sourceName.toStdString().c_str()) == GetSource(currentSceneItem);
}

bool PLSBackgroundMusicView::SameWithCurrentSource(const quint64 &sceneItem)
{
	return GetSource(sceneItem) == GetSource(currentSceneItem);
}

QVector<PLSBgmItemData> PLSBackgroundMusicView::GetPlayListData(obs_data_t *settings)
{
	QVector<PLSBgmItemData> datas{};
	if (!settings) {
		return datas;
	}

	obs_data_array_t *playListArray = obs_data_get_array(settings, PLAY_LIST);
	int item_count = obs_data_array_count(playListArray);
	if (0 == item_count) {
		return datas;
	}

	for (int i = 0; i < item_count; i++) {
		obs_data_t *playList = obs_data_array_item(playListArray, i);
		QString playlistType = obs_data_get_string(playList, BGM_DURATION_TYPE);

		PLSBgmItemData bgmData = GetPlayListDataBySettings(playList);
		datas.push_back(bgmData);
		obs_data_release(playList);
	}

	obs_data_array_release(playListArray);
	return datas;
}

PLSBgmItemData PLSBackgroundMusicView::GetPlayListDataBySettings(obs_data_t *settings)
{
	if (!settings) {
		return PLSBgmItemData();
	}

	PLSBgmItemData bgmData;
	bgmData.isCurrent = obs_data_get_bool(settings, BGM_IS_CURRENT);
	bgmData.title = obs_data_get_string(settings, BGM_TITLE);
	bgmData.producer = obs_data_get_string(settings, BGM_PRODUCER);
	bgmData.id = atoi(obs_data_get_string(settings, BGM_DURATION_TYPE));
	bgmData.SetUrl(obs_data_get_string(settings, BGM_URL), bgmData.id);
	bgmData.SetDuration(bgmData.id, atoi(obs_data_get_string(settings, BGM_DURATION)));
	bgmData.isLocalFile = obs_data_get_bool(settings, BGM_IS_LOCAL_FILE);
	bgmData.isDisable = obs_data_get_bool(settings, BGM_IS_DISABLE);
	bgmData.haveCover = obs_data_get_bool(settings, BGM_HAVE_COVER);
	bgmData.coverPath = obs_data_get_string(settings, BGM_COVER_PATH);

	return bgmData;
}

PLSBgmItemData PLSBackgroundMusicView::GetCurrentPlayListData()
{
	return ui->playListWidget->GetCurrent();
}

PLSBgmItemData PLSBackgroundMusicView::GetCurrentPlayListDataBySettings()
{
	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return PLSBgmItemData();
	}
	obs_data_t *settings = obs_data_create();
	obs_source_get_private_data(source, settings);
	QString method = obs_data_get_string(settings, "method");
	if (method != "get_current_url") {
		obs_data_release(settings);
		return PLSBgmItemData();
	}

	PLSBgmItemData bgmData = GetPlayListDataBySettings(settings);
	obs_data_release(settings);

	return bgmData;
}

PLSBgmItemData PLSBackgroundMusicView::GetCurrentPlayListDataBySettings(const QString &name)
{
	OBSSource source = pls_get_source_by_name(name.toStdString().c_str());
	if (!source) {
		return PLSBgmItemData();
	}
	obs_data_t *settings = obs_data_create();
	obs_source_get_private_data(source, settings);

	QString method = obs_data_get_string(settings, "method");
	if (method != "get_current_url") {
		obs_data_release(settings);
		return PLSBgmItemData();
	}

	PLSBgmItemData bgmData = GetPlayListDataBySettings(settings);
	obs_data_release(settings);

	return bgmData;
}

int PLSBackgroundMusicView::GetCurrentPlayListDataSize()
{
	return ui->playListWidget->Count();
}

int PLSBackgroundMusicView::GetCurrentBgmSourceSize()
{
	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());
	if (!main) {
		return 0;
	}
	BgmSourceVecType sourceList = main->GetCurrentSceneBgmSourceList();
	return sourceList.size();
}

QVector<PLSBgmItemData> PLSBackgroundMusicView::GetPlayListDatas()
{
	return ui->playListWidget->GetData();
}

void PLSBackgroundMusicView::RefreshPropertyWindow()
{
	if (currentSourceName.isEmpty()) {
		return;
	}

	emit RefreshSourceProperty(currentSourceName, this->isVisible());
}

bool PLSBackgroundMusicView::CurrentPlayListBgmDataExisted(const QString &url)
{
	return ui->playListWidget->Existed(url);
}

void PLSBackgroundMusicView::UpdateSourceSelectUI()
{
	PLSBasic *main = PLSBasic::Get();
	if (!main) {
		return;
	}
	QString name{};
	quint64 item{};
	bool selectBgm = false;
	selectBgm = main->GetSelectBgmSourceName(name, item);
	selectBgm = selectBgm && obs_sceneitem_visible(pls_get_sceneitem_by_pointer_address(main->GetCurrentScene(), (void *)item));

	BgmSourceVecType sourceList = main->GetCurrentSceneBgmSourceList();
	if (0 == sourceList.size()) {
		currentSceneItem = 0;
		currentSourceName = "";
		ui->stackedWidget->setCurrentWidget(ui->noSourcePage);
	} else if (selectBgm) {
		SetSourceSelect(name, item, true);
	} else {
		for (auto iter = sourceList.begin(); iter != sourceList.end(); ++iter) {
			obs_sceneitem_t *sceneitem = pls_get_sceneitem_by_pointer_address(main->GetCurrentScene(), (void *)iter->second);
			if (!sceneitem) {
				continue;
			}
			bool visible = obs_sceneitem_visible(sceneitem);
			if (!visible) {
				continue;
			}
			SetSourceSelect(iter->first, iter->second, true);
			return;
		}
		currentSceneItem = 0;
		currentSourceName = "";
		ui->stackedWidget->setCurrentWidget(ui->noSourcePage);
	}
}

void PLSBackgroundMusicView::UpdatePlayListUI(const quint64 &sceneItem)
{
	if (!SameWithCurrentSource(sceneItem)) {
		return;
	}

	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		ui->tabWidget->hide();
		ui->noPlayListFrame->show();
		ui->playListFrame->hide();
		ResetControlView();
		return;
	}

	OBSData settings = obs_data_create();
	obs_source_get_private_data(source, settings);
	obs_data_array_t *playListArray = obs_data_get_array(settings, PLAY_LIST);
	int item_count = obs_data_array_count(playListArray);
	if (0 == item_count) {
		ResetControlView();
		ui->noPlayListFrame->show();
		ui->playListFrame->hide();
	} else {
		ui->noPlayListFrame->hide();
		ui->playListFrame->show();
	}
	ui->tabWidget->show();
	obs_data_release(settings);
}

void PLSBackgroundMusicView::AddPlayListUI(const quint64 &sceneItem)
{
	obs_source_t *source = GetSource(sceneItem);
	if (!source) {
		return;
	}
	OBSData settings = obs_data_create();
	obs_source_get_private_data(source, settings);
	obs_data_release(settings);
	obs_data_array_t *playListArray = obs_data_get_array(settings, PLAY_LIST);
	int item_count = obs_data_array_count(playListArray);
	if (0 == item_count) {
		ui->playListWidget->Clear();
		ResetControlView();
		return;
	}

	QVector<PLSBgmItemData> datas;
	QVector<PLSBgmItemData> availableDatas;
	bool needUpdateDisableConfig = false;
	PLSBgmItemData currentBgmData;
	obs_media_state state = obs_source_media_get_state(source);
	for (int i = 0; i < item_count; i++) {
		obs_data_t *playList = obs_data_array_item(playListArray, i);
		PLSBgmItemData bgmData = GetPlayListDataBySettings(playList);
		if (bgmData.isCurrent && state != OBS_MEDIA_STATE_NONE) {
			bgmData.isCurrent = true;
			currentBgmData = bgmData;
		} else {
			bgmData.isCurrent = false;
		}
		if (bgmData.isDisable) {
			if (bgmData.isLocalFile) {
				CreateCheckValidThread();
				QMetaObject::invokeMethod(checkThreadObj, "CheckUrlAvailable", Qt::QueuedConnection, Q_ARG(PLSBgmItemData, bgmData));
			} else {
				if (CheckNetwork()) {
					availableDatas.push_back(bgmData);
					bgmData.isDisable = false;
					needUpdateDisableConfig = true;
				}
			}
		}
		datas.push_back(bgmData);
		obs_data_release(playList);
	}

	if (needUpdateDisableConfig) {
		RefreshMulicEnabledPlayList(sceneItem, availableDatas);
	}

	ui->playListWidget->UpdateWidget(datas);
	SetPlayListStatus(currentBgmData);

	int currentRow = ui->playListWidget->GetCurrentRow();
	if (!currentBgmData.isCurrent || -1 == currentRow) {
		StopLoadingTimer();
		return;
	}
	if (isVisible()) {
		ui->playListWidget->scrollTo(ui->playListWidget->GetModelIndex(currentRow));
	}
	indexLoading = currentRow;
	if (state == OBS_MEDIA_STATE_PLAYING || state == OBS_MEDIA_STATE_OPENING) {
		if (obs_source_media_is_update_done(source)) {
			isLoading = false;
			PLSBgmItemDelegate::totalFrame(21);
			PLSBgmItemDelegate::setCurrentFrame(1);
			ui->playListWidget->SetMediaStatus(currentRow, MediaStatus::statePlaying);
			StartLoadingTimer();
		} else {
			isLoading = true;
			PLSBgmItemDelegate::totalFrame(8);
			PLSBgmItemDelegate::setCurrentFrame(1);
			ui->playListWidget->SetMediaStatus(currentRow, MediaStatus::stateLoading);
			StartLoadingTimer();
		}
	} else {
		StopLoadingTimer();
	}
	SetPlayerControllerStatus(sceneItem, true);
}

void PLSBackgroundMusicView::SetPlayerControllerStatus(const quint64 &sceneItem, bool listChanged)
{
	if (!SameWithCurrentSource(sceneItem)) {
		return;
	}

	obs_source_t *source = GetSource(sceneItem);
	if (!source) {
		ResetControlView();
		return;
	}

	bool visible = obs_sceneitem_visible(pls_get_sceneitem_by_pointer_address((void *)sceneItem));
	obs_media_state state = obs_source_media_get_state(source);
	if ((state == OBS_MEDIA_STATE_NONE) || !visible) {
		ResetControlView();
		return;
	}

	PLSBgmItemData data = GetCurrentPlayListDataBySettings();
	if (data.title.isEmpty()) {
		ResetControlView();
		return;
	}

	if (state == OBS_MEDIA_STATE_PLAYING) {
		if (listChanged) {
			float time = (float)obs_source_media_get_time(source);
			float duration = (float)obs_source_media_get_duration(source);
			float sliderPosition = 0.0f;
			sliderPosition = (time / duration) * (float)ui->playingSlider->maximum();
			ui->currentTimeLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString((int)(time / 1000.0f)));
			ui->durationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString((int)(duration / 1000.0f)));
			ui->playingSlider->setValue((int)(sliderPosition));
		}
		ui->playingSlider->setMediaSliderEnabled(true);
		ui->playingSlider->setEnabled(true);
		StartSliderPlayingTimer();
		pls_flush_style(ui->playBtn, STATUS_STATE, STATUS_PAUSE);
		ui->playBtn->setToolTip(QTStr("Bgm.Pause"));
	} else if (state == OBS_MEDIA_STATE_OPENING) {
		ui->playingSlider->setMediaSliderEnabled(false);
		ui->playingSlider->setEnabled(false);
		ui->currentTimeLabel->setText("00:00");
		ui->durationLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString(data.GetDuration(data.id)));
		ui->playingSlider->setValue(0);
	} else if (state == OBS_MEDIA_STATE_PAUSED) {
		ui->playingSlider->setMediaSliderEnabled(true);
		ui->playingSlider->setEnabled(true);
		StopSliderPlayingTimer();
		SetSliderPos();
		pls_flush_style(ui->playBtn, STATUS_STATE, STATUS_PLAY);
		ui->playBtn->setToolTip(QTStr("Bgm.Play"));
	}

	isLoading ? loadingEvent.startLoadingTimer(ui->loadingBtn) : loadingEvent.stopLoadingTimer();
	if (isLoading) {
		ui->playBtn->setVisible(!isLoading);
		ui->loadingBtn->setVisible(isLoading);
	} else {
		ui->loadingBtn->setVisible(isLoading);
		ui->playBtn->setVisible(!isLoading);
	}

	ui->coverLabel->SetMusicInfo(data.title, data.producer);
	DisablePlayerControlUI(false);
	ShowCoverGif(state == OBS_MEDIA_STATE_PLAYING);

	ui->playingSlider->setMinimum(0);
}

void PLSBackgroundMusicView::ResetControlView()
{
	ui->durationLabel->setText("00:00");
	ui->currentTimeLabel->setText("00:00");
	ui->playingSlider->setValue(0);
	StopSliderPlayingTimer();
	StopLoadingTimer();
	ui->loadingBtn->setVisible(false);
	ui->playBtn->setVisible(true);
	pls_flush_style(ui->playBtn, STATUS_STATE, STATUS_PLAY);
	DisablePlayerControlUI(true);
}

void PLSBackgroundMusicView::DisablePlayerControlUI(bool disable)
{
	QString status = disable ? STATUS_DISABLE : STATUS_ENABLE;
	pls_flush_style(ui->refreshBtn, STATUS, status);
	pls_flush_style(ui->playBtn, STATUS, status);
	pls_flush_style(ui->preBtn, STATUS, status);
	pls_flush_style(ui->nextBtn, STATUS, status);
	pls_flush_style(ui->loopBtn, STATUS, status);
	ui->noSongFrame->setVisible(disable);
	ui->coverLabel->setVisible(!disable);
	if (disable)
		ui->coverLabel->SetCoverPath(DEFAULT_COVER_IMAGE, false);
	SetPlayerPaneEnabled(!disable);
}

void PLSBackgroundMusicView::SaveShowModeToConfig()
{
	config_set_bool(App()->GlobalConfig(), BGM_CONFIG, "showMode", isVisible());
	config_save(App()->GlobalConfig());
}

void PLSBackgroundMusicView::onMaxFullScreenStateChanged()
{
	config_set_bool(App()->GlobalConfig(), BGM_CONFIG, MAXIMIZED_STATE, getMaxState());
	config_save(App()->GlobalConfig());
}

void PLSBackgroundMusicView::onSaveNormalGeometry()
{
	config_set_string(App()->GlobalConfig(), BGM_CONFIG, GEOMETRY_BGM_DATA, saveGeometry().toBase64().constData());
	config_save(App()->GlobalConfig());
}

void PLSBackgroundMusicView::PLSFrontendEvent(obs_frontend_event event, void *ptr)
{
	PLSBackgroundMusicView *view = reinterpret_cast<PLSBackgroundMusicView *>(ptr);

	switch ((int)event) {
	case OBS_FRONTEND_EVENT_SCENE_COLLECTION_CHANGED:
	case OBS_FRONTEND_EVENT_SCENE_LIST_CURRENT_CHANGED:
	case OBS_FRONTEND_EVENT_SCENE_LIST_ADD:
	case OBS_FRONTEND_EVENT_SCENE_LIST_DELETE:
	case OBS_FRONTEND_EVENT_SCENE_CHANGED:
		QMetaObject::invokeMethod(view, "OnSceneChanged", Qt::QueuedConnection);
		break;
	case OBS_FRONTEND_EVENT_SCENE_COPY:
		QMetaObject::invokeMethod(view, "OnSceneCopy", Qt::QueuedConnection);
		break;
	}
}

int PLSBackgroundMusicView::GetPlayListItemIndexByKey(const QString &key)
{
	for (int i = 0; i < ui->playListWidget->Count(); i++) {
		PLSBgmItemData data = ui->playListWidget->Get(i);
		QString targetKey = data.title.append(QString::number(static_cast<int>(data.GetDuration(data.id))));
		if (0 == key.compare(targetKey)) {
			return i;
		}
	}
	return -1;
}

void PLSBackgroundMusicView::SetUrlInfo(OBSSource source, PLSBgmItemData data)
{
	if (!source) {
		return;
	}

	OBSData settings = obs_data_create();

	obs_data_set_string(settings, "method", "bgm_play");
	SetUrlInfoToSettings(settings, data);

	obs_source_set_private_data(source, settings);
	obs_data_release(settings);

	obs_source_media_restart(source);
}

void PLSBackgroundMusicView::SetUrlInfoToSettings(obs_data_t *settings, const PLSBgmItemData &data)
{
	if (!settings) {
		return;
	}

	obs_data_set_string(settings, BGM_TITLE, data.title.toStdString().c_str());
	obs_data_set_string(settings, BGM_PRODUCER, data.producer.toStdString().c_str());
	obs_data_set_string(settings, BGM_URL, data.GetUrl(data.id).toStdString().c_str());
	obs_data_set_string(settings, BGM_DURATION_TYPE, QString::number(static_cast<int>(data.id)).toStdString().c_str());
	obs_data_set_string(settings, BGM_DURATION, QString::number(data.GetDuration(data.id)).toStdString().c_str());
	obs_data_set_string(settings, BGM_GROUP, data.group.toStdString().c_str());
	obs_data_set_string(settings, BGM_COVER_PATH, data.coverPath.toStdString().c_str());
	obs_data_set_bool(settings, BGM_IS_LOCAL_FILE, data.isLocalFile);
	obs_data_set_bool(settings, BGM_HAVE_COVER, data.haveCover);
	obs_data_set_bool(settings, BGM_IS_CURRENT, data.isCurrent);
	obs_data_set_bool(settings, BGM_IS_DISABLE, data.isDisable);
}

QString PLSBackgroundMusicView::StrcatString(const QString &title, const QString &producer) const
{
	if (title.isEmpty() || producer.isEmpty()) {
		return "";
	}
	return title + QString("-") + producer;
}

bool PLSBackgroundMusicView::IsSourceAvailable(const QString &sourceName, quint64 sceneItem)
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source) {
		return false;
	}

	obs_sceneitem_t *item = (obs_sceneitem_t *)sceneItem;
	return obs_sceneitem_visible(item);
}

void PLSBackgroundMusicView::StartSliderPlayingTimer()
{
	if (!sliderTimer) {
		return;
	}
	if (!sliderTimer->isActive()) {
		sliderTimer->start(200);
	}
}

void PLSBackgroundMusicView::StopSliderPlayingTimer()
{
	if (!sliderTimer) {
		return;
	}
	if (sliderTimer->isActive()) {
		sliderTimer->stop();
	}
}

void PLSBackgroundMusicView::StartLoadingTimer(int timeOutMs)
{
	if (!timerLoading.isActive()) {
		timerLoading.start(timeOutMs);
		loadingTimeout = os_gettime_ns();
	}
}

void PLSBackgroundMusicView::StopLoadingTimer()
{
	isLoading = false;
	if (timerLoading.isActive()) {
		timerLoading.stop();
		indexLoading = -1;
		loadingTimeout = 0;
	}
}

bool PLSBackgroundMusicView::CheckNetwork()
{
	return networkAvailable;
}

bool PLSBackgroundMusicView::CheckValidLocalAudioFile(const QString &url)
{
	media_info_t media_info;
	memset(&media_info, 0, sizeof(media_info_t));

	bool open = mi_open(&media_info, url.toStdString().c_str(), static_cast<mi_open_mode>(MI_OPEN_DIRECTLY | MI_OPEN_TRY_DECODER));
	mi_free(&media_info);

	return open;
}

void PLSBackgroundMusicView::RefreshMulicEnabledPlayList(const quint64 &sceneItem, const QVector<PLSBgmItemData> &datas)
{
	obs_source_t *source = GetSource(sceneItem);
	if (!source) {
		return;
	}
	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "method", "bgm_enable");

	obs_data_array_t *urlArray = obs_data_array_create();
	for (auto &data : datas) {
		obs_data_t *url = obs_data_create();
		obs_data_set_string(url, BGM_URL, data.GetUrl(data.id).toStdString().c_str());
		obs_data_set_string(url, BGM_DURATION_TYPE, QString::number(data.id).toStdString().c_str());
		obs_data_array_push_back(urlArray, url);
		obs_data_release(url);
	}
	obs_data_set_array(settings, BGM_URLS, urlArray);
	obs_source_set_private_data(source, settings);
	obs_data_array_release(urlArray);
	obs_data_release(settings);
}

void PLSBackgroundMusicView::UpdateCurrentPlayStatus(const QString &sourceName)
{
	if (sourceName != currentSourceName) {
		return;
	}

	OBSSource source = pls_get_source_by_name(currentSourceName.toStdString().c_str());
	if (!source) {
		return;
	}

	int currentRow = ui->playListWidget->GetCurrentRow();
	if (-1 == currentRow) {
		return;
	}

	obs_media_state state = obs_source_media_get_state(source);
	if (state == OBS_MEDIA_STATE_PLAYING) {
		indexLoading = currentRow;
		ui->playListWidget->SetMediaStatus(currentRow, isLoading ? MediaStatus::stateLoading : MediaStatus::statePlaying);
	} else if (state == OBS_MEDIA_STATE_OPENING) {
		indexLoading = currentRow;
		ui->playListWidget->SetMediaStatus(currentRow, MediaStatus::stateLoading);
	}
}

void PLSBackgroundMusicView::SetPlayListStatus(const PLSBgmItemData &data)
{
	for (int i = 0; i < ui->playListWidget->Count(); i++) {
		PLSBgmItemData data_ = ui->playListWidget->Get(i);
		SetPlayListItemStatus(i, data_);
	}
}

void PLSBackgroundMusicView::SetPlayListItemStatus(const int &index, const PLSBgmItemData &data_)
{
	bool current = data_.isCurrent;
	OBSSource source = pls_get_source_by_name(currentSourceName.toStdString().c_str());
	if (!source) {
		ui->playListWidget->SetMediaStatus(index, MediaStatus::stateNormal);
		return;
	}

	if (data_.isDisable) {
		ui->playListWidget->SetMediaStatus(index, MediaStatus::stateInvalid);
		return;
	}

	if (current) {
		obs_media_state state = obs_source_media_get_state(source);
		if (state == OBS_MEDIA_STATE_PLAYING) {
			if (!isLoading) {
				ui->playListWidget->SetMediaStatus(index, MediaStatus::statePlaying);
			}
		} else if (state == OBS_MEDIA_STATE_OPENING) {
			if (!isLoading) {
				ui->playListWidget->SetMediaStatus(index, MediaStatus::stateLoading);
			}
		} else if (state == OBS_MEDIA_STATE_PAUSED) {
			ui->playListWidget->SetMediaStatus(index, MediaStatus::statePause);
		} else if (state == state == OBS_MEDIA_STATE_ERROR) {
			ui->playListWidget->SetMediaStatus(index, MediaStatus::stateInvalid);
		} else {
			data_.isDisable ? ui->playListWidget->SetMediaStatus(index, MediaStatus::stateInvalid) : ui->playListWidget->SetMediaStatus(index, MediaStatus::stateNormal);
		}
		return;
	}
	ui->playListWidget->SetMediaStatus(index, MediaStatus::stateNormal);
}

void PLSBackgroundMusicView::SetPlayerPaneEnabled(bool enabled)
{
	ui->loopBtn->setEnabled(enabled);
	ui->preBtn->setEnabled(enabled);
	ui->playBtn->setEnabled(enabled);
	ui->loadingBtn->setEnabled(enabled);
	ui->nextBtn->setEnabled(enabled);
	ui->refreshBtn->setEnabled(enabled);
	ui->currentTimeLabel->setEnabled(enabled);
	ui->durationLabel->setEnabled(enabled);

	if (!enabled) {
		ui->playingSlider->setEnabled(enabled);
	}
}

void PLSBackgroundMusicView::CreateLibraryView()
{
	if (!libraryView) {
		libraryView = new PLSBgmLibraryView(this);
		connect(libraryView, &PLSBgmLibraryView::AddCachePlayList, this, &PLSBackgroundMusicView::OnAddCachePlayList);
		libraryView->setModal(true);
	}
}

void PLSBackgroundMusicView::InitToast()
{
	toastView.setParent(this);
	toastView.hide();
}

void PLSBackgroundMusicView::ShowToastView(const QString &text)
{
	toastView.SetMessage(text);
	ResizeToastView();
	toastView.ShowToast();
}

void PLSBackgroundMusicView::ResizeToastView()
{
	toastView.SetShowWidth(this->width() - 2 * PLSDpiHelper::calculate(this, 10));
	int toastWidth = this->width() - PLSDpiHelper::calculate(this, 2 * 10);
	toastView.move(PLSDpiHelper::calculate(this, 10), PLSDpiHelper::calculate(this, 460));
}

QImage PLSBackgroundMusicView::GetCoverImage(const QString &url)
{
	QImage image{};
	media_info_t media_info;
	memset(&media_info, 0, sizeof(media_info_t));
	bool open = mi_open(&media_info, url.toStdString().c_str(), MI_OPEN_DIRECTLY);
	if (!open) {
		return image;
	}
	mi_cover_t *cover = (mi_cover_t *)mi_get_obj(&media_info, "cover_obj");
	if (cover) {
		image = CaptureImage(cover->width, cover->height, cover->data, cover->size, 0);
	}
	mi_free(&media_info);
	return image;
}

void PLSBackgroundMusicView::SetCoverImage(const PLSBgmItemData &data)
{
	QMetaObject::invokeMethod(coverThreadObj, "GetCoverImage", Qt::QueuedConnection, Q_ARG(const PLSBgmItemData &, data));
}

void PLSBackgroundMusicView::ShowCoverImage(const PLSBgmItemData &data)
{
	if (data.title.isEmpty()) {
		return;
	}
	QString coverPath;
	if (!data.coverPath.isEmpty()) {
		coverPath = data.coverPath;
		QFileInfo fileinfo(coverPath);
		if (!fileinfo.exists()) {
			if (!data.haveCover) {
				coverPath = DEFAULT_COVER_IMAGE;
				goto end;
			}
		}

		if (!data.haveCover) {
			goto end;
		}
		if (fileinfo.exists()) {
			goto end;
		}

		SetCoverImage(data);
		return;
	} else {
		if (data.haveCover) {
			SetCoverImage(data);
			return;
		}
		coverPath = DEFAULT_COVER_IMAGE;
	}

	if (0 != data.coverPath.compare(coverPath)) {
		PLSBgmItemData data_ = data;
		data_.coverPath = coverPath;
		ui->coverLabel->SetCoverPath(data_.coverPath, !data.isLocalFile);
		UpdateBgmCoverPath(data_);
		return;
	}
end:

	ui->coverLabel->SetCoverPath(coverPath, !data.isLocalFile);
}

void PLSBackgroundMusicView::ShowCoverGif(bool show)
{
	ui->coverLabel->ShowPlayingGif(show);
}

void PLSBackgroundMusicView::UpdateBgmCoverPath(const PLSBgmItemData &data)
{
	obs_source_t *source = GetSource(currentSceneItem);
	if (!source) {
		return;
	}

	ui->playListWidget->UpdataData(ui->playListWidget->GetRow(data), data.coverPath, CustomDataRole::CoverPathRole);
	obs_data_t *settings = obs_data_create();
	obs_source_get_private_data(source, settings);
	QVector<PLSBgmItemData> datas = GetPlayListData(settings);
	for (int i = 0; i < datas.size(); i++) {
		PLSBgmItemData data_ = datas[i];
		if (data_.id == data.id && data_.GetUrl(data_.id) == data.GetUrl(data.id)) {
			QFileInfo fileinfo(data_.coverPath);
			bool existed = fileinfo.exists();
			if (data_.coverPath.isEmpty() || !existed) {
				OBSData settings_ = obs_data_create();
				obs_data_set_string(settings_, "method", "bgm_update_cover_path");
				obs_data_set_string(settings_, BGM_URL, data.GetUrl(data.id).toStdString().c_str());
				obs_data_set_string(settings_, BGM_DURATION_TYPE, QString::number(data.id).toStdString().c_str());
				obs_data_set_string(settings_, BGM_COVER_PATH, data.coverPath.toStdString().c_str());
				obs_source_set_private_data(source, settings_);
				obs_data_release(settings_);
			}
			break;
		}
	}
	obs_data_release(settings);
}

void CheckValidThread::CheckUrlAvailable(const PLSBgmItemData &data)
{
	media_info_t media_info;
	memset(&media_info, 0, sizeof(media_info_t));

	bool open = mi_open(&media_info, data.GetUrl(data.id).toStdString().c_str(), static_cast<mi_open_mode>(MI_OPEN_DIRECTLY | MI_OPEN_TRY_DECODER));
	mi_free(&media_info);

	emit checkFinished(data, open);
}

void PLSBackgroundMusicView::SeekTo(int val)
{
	OBSSource source = pls_get_source_by_name(currentSourceName.toStdString().c_str());
	if (!source)
		return;

	obs_media_state state = obs_source_media_get_state(source);
	float percent = (float)val / float(ui->playingSlider->maximum());
	int64_t duration = obs_source_media_get_duration(source);

	if (duration > 0) {
		int64_t seekTo = (int64_t)(percent * duration);
		obs_source_media_set_time(source, seekTo);
		ui->playingSlider->setValue(val);
		ui->currentTimeLabel->setText(PLSBgmDataViewManager::Instance()->ConvertIntToTimeString((int)(seekTo / 1000.0f)));
	}
}

void GetCoverThread::SaveCoverToLocalPath(const PLSBgmItemData &data_, const QImage &image)
{
	PLSBgmItemData data = data_;
	data.coverPath = GetTempImageFilePath(".png", 0);
	image.save(data.coverPath);
	emit Finished(data);
}

void GetCoverThread::GetCoverImage(const PLSBgmItemData &data)
{
	if (taskQueue.isEmpty())
		QTimer::singleShot(0, this, SLOT(NextTask()));
	taskQueue.enqueue(data);
}

void GetCoverThread::NextTask()
{
	if (taskQueue.isEmpty())
		return;

	auto data = taskQueue.dequeue();

	QImage image{};
	QString url = data.GetUrl(data.id);
	if (url.isEmpty()) {
		emit GetPreviewImage(image, data);
		NextTask();
		return;
	}
	media_info_t media_info;
	memset(&media_info, 0, sizeof(media_info_t));
	bool open = mi_open(&media_info, url.toStdString().c_str(), MI_OPEN_DIRECTLY);
	if (!open) {
		emit GetPreviewImage(image, data);
		NextTask();
		return;
	}
	mi_cover_t *cover = (mi_cover_t *)mi_get_obj(&media_info, "cover_obj");
	if (cover) {
		image = CaptureImage(cover->width, cover->height, cover->data, cover->size, 0);
	}
	mi_free(&media_info);
	bool widthLonger = (image.width() > image.height());
	if (image.width() > COVER_WIDTH * 3 || image.height() > COVER_WIDTH * 3) {
		image = widthLonger ? image.scaledToWidth(COVER_WIDTH * 3, Qt::SmoothTransformation) : image.scaledToHeight(COVER_WIDTH * 3, Qt::SmoothTransformation);
		emit GetPreviewImage(image, data);
	} else {
		emit GetPreviewImage(image, data);
	}
	// save image to local
	PLSBgmItemData temp = data;
	temp.coverPath = GetTempImageFilePath(".png", 0);
	image.save(temp.coverPath);
	emit Finished(temp);
	NextTask();
}

void SendThread::SendMusicMetaData(const PLSBgmItemData &data)
{
	if (data.GetUrl(data.id).isEmpty()) {
		return;
	}

	media_info_t media_info;
	memset(&media_info, 0, sizeof(media_info_t));
	bool open = mi_open(&media_info, data.GetUrl(data.id).toStdString().c_str(), MI_OPEN_DEFER);
	if (!open) {
		return;
	}

	mi_id3v2_t *id3 = (mi_id3v2_t *)mi_get_obj(&media_info, "id3v2_obj");
	if (id3) {
		mi_send_id3v2(id3);
	}
	mi_free(&media_info);
}

void DragInFrame::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls()) {
		if (PLSBgmDataViewManager::Instance()->IsSupportFormat(event->mimeData()->urls())) {
			event->acceptProposedAction();
			return;
		}
		event->ignore();
	}
}

void DragInFrame::dropEvent(QDropEvent *event)
{
	QFrame::dropEvent(event);

	if (event->mimeData()->hasUrls()) {
		QStringList paths;
		QList<QUrl> urls = event->mimeData()->urls();
		for (int i = 0; i < urls.size(); i++) {
			QString file = urls.at(i).toLocalFile();
			if (!PLSBgmDataViewManager::Instance()->IsSupportFormat(file)) {
				continue;
			}

			paths << file;
		}
		PLS_UI_STEP(MAIN_BGM_MODULE, QString("%1 Songs").arg(paths.size()).toStdString().c_str(), ACTION_DRAG);
		emit AudioFileDraggedIn(paths);
	}
}

bool DragLabel::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::LeftButton) {
			startPoint = mouseEvent->pos();
			mousePressed = true;
			return true;
		}
	} else if (event->type() == QEvent::MouseMove) {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mousePressed) {
			emit CoverPressed(mouseEvent->pos() - startPoint);
			return true;
		}
	} else if (event->type() == QEvent::MouseButtonRelease) {
		mousePressed = false;
		return true;
	}

	return QLabel::eventFilter(object, event);
}
