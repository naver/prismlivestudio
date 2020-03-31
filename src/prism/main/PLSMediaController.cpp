#include "PLSMediaController.h"
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QToolTip>
#include <qfuture.h>
#include <inttypes.h>
#include <QDebug>

#include "liblog.h"
#include "log/module_names.h"
#include "PLSNetworkMonitor.h"

#define SLIDER_MOVE_INTERVALL 200000000

static const QString START_TIME_TEXT = "00:00:00";
static const QString LOOPING = "looping";
static const QString IS_LOCAL_FILE = "is_local_file";

void PLSMediaController::FrontendEvent(obs_frontend_event event, void *ptr)
{
	PLSMediaController *controls = reinterpret_cast<PLSMediaController *>(ptr);
	PLSBasic *main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());

	//PRISM/ZengQin/20200807/#3805/fix switch studio model controller hide or show wrong
	switch (event) {
	case OBS_FRONTEND_EVENT_SCENE_CHANGED:
	case OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED:
	case OBS_FRONTEND_EVENT_STUDIO_MODE_ENABLED:
	case OBS_FRONTEND_EVENT_STUDIO_MODE_DISABLED:
	case OBS_FRONTEND_EVENT_SCENE_LIST_ADD:
	case OBS_FRONTEND_EVENT_SCENE_COLLECTION_CHANGED:
		QMetaObject::invokeMethod(
			controls, [=]() { controls->SetScene(main->GetCurrentScene()); }, Qt::QueuedConnection);
		break;
	default:
		break;
	}
}

bool PLSMediaController::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui->slider) {
		if (event->type() == QEvent::Show) {
			StartTimer();
		} else if (event->type() == QEvent::Hide) {
			StopTimer();
		}
	} else if (target == ui->playPauseButton) {
		if (event->type() == QEvent::KeyPress) {
			return true;
		}
	}

	return QWidget::eventFilter(target, event);
}

void PLSMediaController::MediaEof(void *data, calldata_t *calldata)
{
	UNUSED_PARAMETER(calldata);

	PLSMediaController *media = static_cast<PLSMediaController *>(data);
	obs_source_t *source = (obs_source_t *)calldata_ptr(calldata, "source");
	QString name = obs_source_get_name(source);
	QMetaObject::invokeMethod(media, "SetEofState", Qt::QueuedConnection, Q_ARG(const QString &, name));
}

void PLSMediaController::MediaRenamed(void *data, calldata_t *calldata)
{
	PLSMediaController *media = static_cast<PLSMediaController *>(data);
	const char *name = calldata_string(calldata, "new_name");
	obs_source_t *source = (obs_source_t *)calldata_ptr(calldata, "source");
	QMetaObject::invokeMethod(media, "MediaSourceRenamed", Q_ARG(QString, QT_UTF8(name)), Q_ARG(obs_source_t *, source));
}

void PLSMediaController::MediaStateChanged(void *data, calldata_t *calldata)
{
	PLSMediaController *media = static_cast<PLSMediaController *>(data);
	obs_source_t *source = (obs_source_t *)calldata_ptr(calldata, "source");
	QString name = obs_source_get_name(source);
	QMetaObject::invokeMethod(media, "OnMediaStateChanged", Qt::QueuedConnection, Q_ARG(const QString &, name));
}

bool PLSMediaController::UpdateSelectItem(obs_scene_t *scene, obs_sceneitem_t *item, void *param)
{
	PLSMediaController *media = reinterpret_cast<PLSMediaController *>(param);

	if (obs_sceneitem_is_group(item)) {
		media->visible = obs_sceneitem_visible(item);
		obs_sceneitem_group_enum_items(item, UpdateSelectItem, media);
	}

	obs_source_t *source = obs_sceneitem_get_source(item);
	if (!source)
		return true;

	bool select = obs_sceneitem_selected(item);
	bool flag = obs_source_get_output_flags(source) & OBS_SOURCE_CONTROLLABLE_MEDIA;
	if (!select || !flag)
		return true;

	if (media->visible)
		QMetaObject::invokeMethod(
			media, [=]() { media->UpdateMediaSource(item); }, Qt::QueuedConnection);
	else
		QMetaObject::invokeMethod(
			media, [=]() { media->ResetAndHide(); }, Qt::QueuedConnection);

	UNUSED_PARAMETER(scene);
	return true;
}

void PLSMediaController::PropertiesChanged(void *param, calldata_t *data)
{
	PLSMediaController *controls = reinterpret_cast<PLSMediaController *>(param);
	obs_source_t *source = (obs_source_t *)calldata_ptr(data, "source");
	QString name = obs_source_get_name(source);
	QMetaObject::invokeMethod(controls, "OnPropertiesChanged", Qt::QueuedConnection, Q_ARG(const QString &, name));
}

void PLSMediaController::SetScene(OBSScene scene)
{
	if (scene) {
		OBSSceneItem sceneItem = main->GetCurrentSceneItemData();
		UpdateMediaSource(sceneItem);
	}
}

void PLSMediaController::SeekTo(int val)
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return;

	obs_media_state state = obs_source_media_get_state(source);
	float percent = (float)val / float(ui->slider->maximum());
	int64_t duration = obs_source_media_get_duration(source);

	if (duration > 0) {
		int64_t seekTo = (int64_t)(percent * duration);
		if (state != OBS_MEDIA_STATE_PLAYING && state != OBS_MEDIA_STATE_PAUSED) {
			obs_source_media_restart_to_pos(source, true, seekTo);
		} else {
			int64_t seekTo = (int64_t)(percent * duration);
			obs_source_media_set_time(source, seekTo);
		}
		ui->slider->setValue(val);
		ui->timerLabel->setText(FormatSeconds((int)((float)seekTo / 1000.0f)));
	}
}

bool PLSMediaController::UpdateLoading()
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return false;

	obs_data_t *loadData = obs_data_create();
	obs_data_set_string(loadData, "method", "media_opening");
	obs_source_get_private_data(source, loadData);
	bool loading = obs_data_get_bool(loadData, "media_opening");
	obs_data_release(loadData);

	SetLoadState(loading, source);
	return loading;
}

void PLSMediaController::StartLoading()
{
	this->setDisabled(true);

	ResizeLoading();
	lableLoad->show();

	m_loadingEvent.startLoadingTimer(lableLoad);
}

void PLSMediaController::StopLoading()
{
	this->setDisabled(false);

	lableLoad->hide();
	m_loadingEvent.stopLoadingTimer();
}

bool PLSMediaController::CheckValid()
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return false;

	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(source, &error);
	bool visible = obs_sceneitem_visible(pls_get_sceneitem_by_pointer_address(main->GetCurrentScene(), (void *)sceneItem));
	obs_media_state state = obs_source_media_get_state(source);
	if (!valid || state == OBS_MEDIA_STATE_NONE || state == OBS_MEDIA_STATE_ERROR)
		hide();

	return valid && visible;
}

bool PLSMediaController::GetSettingsValueForBool(QString str)
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return false;

	bool result = obs_data_get_bool(pls_get_source_setting(source), str.toStdString().c_str());

	return result;
}

void PLSMediaController::SetMediaIsValidForNetwork(bool isConnect)
{
	auto EnumSources = [](void *param, obs_source_t *source) {
		if (!source)
			return true;

		if (!(obs_source_get_output_flags(source) & OBS_SOURCE_CONTROLLABLE_MEDIA))
			return true;

		OBSData settings = pls_get_source_setting(source);
		if (!settings)
			return true;

		bool available = *reinterpret_cast<bool *>(param);
		obs_source_network_state_changed(source, !available);

		return true;
	};

	obs_enum_all_sources(EnumSources, &isConnect);
}

PLSMediaController::PLSMediaController(QWidget *parent, PLSDpiHelper dpiHelper) : QWidget(parent), ui(new Ui::PLSMediaController)
{
	ui->setupUi(this);
	dpiHelper.setCss(this, {PLSCssIndex::PLSMediaController});
	dpiHelper.setFixedHeight(this, 35);

	main = reinterpret_cast<PLSBasic *>(App()->GetMainWindow());

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSliderPosition()), Qt::QueuedConnection);
	connect(ui->slider, SIGNAL(mediaSliderClicked(int)), this, SLOT(SliderClicked(int)), Qt::QueuedConnection);
	connect(ui->slider, SIGNAL(mediaSliderReleased(int)), this, SLOT(SliderReleased(int)));
	connect(ui->slider, SIGNAL(mediaSliderMoved(int)), this, SLOT(SliderMoved(int)), Qt::QueuedConnection);

	obs_frontend_add_event_callback(FrontendEvent, this);
	ui->playPauseButton->installEventFilter(this);
	ui->slider->installEventFilter(this);

	SetScene(main->GetCurrentScene());

	ui->label_live->hide();
	lableLoad = new QLabel(this);
	lableLoad->setAlignment(Qt::AlignCenter);
	lableLoad->setObjectName("loadingBtn");
	lableLoad->hide();
	hide();

	dpiHelper.setFixedSize(this->lableLoad, QSize(24, 24));

	SetMediaIsValidForNetwork(PLSNetworkMonitor::Instance()->IsInternetAvailable());
	connect(PLSNetworkMonitor::Instance(), &PLSNetworkMonitor::OnNetWorkStateChanged, [=](bool isConnected) { SetMediaIsValidForNetwork(isConnected); });
}

PLSMediaController::~PLSMediaController()
{
	obs_frontend_remove_event_callback(FrontendEvent, this);
	if (nullptr != lableLoad) {
		delete lableLoad;
		lableLoad = nullptr;
	}
	if (nullptr != timer) {
		delete timer;
		timer = nullptr;
	}
	deleteLater();
}

void PLSMediaController::SliderClicked(int val)
{
	PLS_UI_STEP(MAIN_MEDIA_CONTROL, "media control clicked Slider", ACTION_CLICK);

	isDragging = true;
	SeekTo(val);
}

void PLSMediaController::SliderReleased(int val)
{
	qDebug() << "slider release pos is ." << val;
	isDragging = false;
}

void PLSMediaController::SliderMoved(int val)
{
	qDebug() << "slider move pos is " << val;
	SeekTo(val);
}

void PLSMediaController::StartTimer()
{
	if (!timer->isActive())
		timer->start(200);
}

void PLSMediaController::StopTimer()
{
	if (timer->isActive())
		timer->stop();
}

void PLSMediaController::SetPlayingState()
{
	pls_flush_style(ui->playPauseButton, "playing", true);
}

void PLSMediaController::SetPausedState()
{
	pls_flush_style(ui->playPauseButton, "playing", false);
}

void PLSMediaController::SetRestartState()
{
	SetPausedState();

	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return;

	obs_data_t *invalidDuration = obs_data_create();
	obs_data_set_string(invalidDuration, "method", "invalid_duration_local_file");
	obs_source_get_private_data(source, invalidDuration);
	bool isInvalidDuration = obs_data_get_bool(invalidDuration, "invalid_duration_local_file");
	obs_data_release(invalidDuration);

	if (isInvalidDuration) {
		UpdateControlsForInvalidDuration();
		return;
	}

	if (!GetSettingsValueForBool(IS_LOCAL_FILE) || GetSettingsValueForBool("close_when_inactive")) {
		ui->slider->setMediaSliderEnabled(false);
		SetResetStatus();
		return;
	}

	int64_t duration = obs_source_media_get_duration(source);

	if (duration > 0)
		ui->durationLabel->setText(FormatSeconds((int)(duration / 1000.0f)));
	else
		ui->durationLabel->setText(START_TIME_TEXT);

	ui->slider->setValue(0);
	ui->timerLabel->setText(START_TIME_TEXT);
	ui->label_live->hide();
	ui->widget_time->show();
	ui->slider->setMediaSliderEnabled(true);
	pls_flush_style(ui->loopButton, "isLoop", GetSettingsValueForBool(LOOPING));
}

void PLSMediaController::SetResetStatus()
{
	pls_flush_style(ui->playPauseButton, "playing", false);
	pls_flush_style(ui->loopButton, "isLoop", GetSettingsValueForBool(LOOPING));
	ui->slider->setValue(0);
	ui->timerLabel->setText(START_TIME_TEXT);
	ui->durationLabel->setText(START_TIME_TEXT);
	ui->label_live->hide();
	ui->widget_time->show();
}

void PLSMediaController::UpdateControlsForInvalidDuration()
{
	pls_flush_style(ui->loopButton, "isLoop", GetSettingsValueForBool(LOOPING));
	ui->slider->setMediaSliderEnabled(false);
	ui->slider->setValue(ui->slider->maximum());
	ui->timerLabel->setText(START_TIME_TEXT);
	ui->durationLabel->setText(START_TIME_TEXT);
	ui->loopButton->setEnabled(true);
	ui->label_live->hide();
	ui->widget_time->show();
}

void PLSMediaController::RefreshControllerAndShow()
{
	UpdateSliderPosition();
	UpdateControlsStatus();
}

void PLSMediaController::ResetAndHide()
{
	if (!isHidden())
		hide();

	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (source) {
		signal_handler_disconnect(obs_source_get_signal_handler(source), "media_properties_changed", PropertiesChanged, this);
		signal_handler_disconnect(obs_source_get_signal_handler(source), "media_eof", MediaEof, this);
		signal_handler_disconnect(obs_source_get_signal_handler(source), "rename", MediaRenamed, this);
		signal_handler_disconnect(obs_source_get_signal_handler(source), "media_state_changed", MediaStateChanged, this);
	}

	sourceName = QString();
	sceneItem = 0;
}

void PLSMediaController::MediaSourceRenamed(const QString &name, obs_source_t *sourcePtr)
{
	sourceName = name;
}

void PLSMediaController::SetEofState(const QString &name)
{
	if (name.isEmpty() || name != sourceName)
		return;

	ui->slider->setValue(ui->slider->maximum());
}

void PLSMediaController::SetLoadState(bool load, obs_source_t *sourcePtr)
{
	if (!sourcePtr || sourcePtr != pls_get_source_by_name(sourceName.toStdString().c_str()))
		return;

	if (load && lableLoad->isHidden())
		StartLoading();
	else if (!load && !lableLoad->isHidden())
		StopLoading();
}

void PLSMediaController::OnPropertiesChanged(const QString &name)
{
	if (name.isEmpty() || name != sourceName)
		return;

	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	bool flag = obs_source_get_output_flags(source) & OBS_SOURCE_CONTROLLABLE_MEDIA;
	if (flag) {
		bool loop = GetSettingsValueForBool(LOOPING);
		pls_flush_style(ui->loopButton, "isLoop", loop);

		OBSData settings = pls_get_source_setting(source);
		bool isLocalFile = obs_data_get_bool(settings, IS_LOCAL_FILE.toStdString().c_str());
		QString locafile = obs_data_get_string(settings, "local_file");
		QString input = obs_data_get_string(settings, "input");
		bool urlValid = isLocalFile ? !locafile.isEmpty() : !input.isEmpty();
		obs_media_state state = obs_source_media_get_state(source);
		bool clearEnd = obs_data_get_bool(settings, "clear_on_media_end") && (state == OBS_MEDIA_STATE_ENDED || state == OBS_MEDIA_STATE_STOPPED);

		enum obs_source_error error;
		bool valid = obs_source_get_capture_valid(source, &error);
		bool visible = obs_sceneitem_visible(pls_get_sceneitem_by_pointer_address(main->GetCurrentScene(), (void *)sceneItem));

		//PRISM/ZengQin/20200807/#3813 #3877/fix hide or show when check 'Show nothing when playback'
		if (urlValid && !clearEnd && valid && visible && !(state == OBS_MEDIA_STATE_NONE || state == OBS_MEDIA_STATE_ERROR)) {
			show();
		} else
			hide();
	}
}

void PLSMediaController::OnMediaStateChanged(const QString &name)
{
	if (name.isEmpty() || name != sourceName)
		ResetAndHide();
	else if (name == sourceName)
		UpdateControlsStatus();
	return;
}

void PLSMediaController::UpdateControlsStatus()
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return;

	obs_media_state state = obs_source_media_get_state(source);

	switch (state) {
	case OBS_MEDIA_STATE_ERROR:
	case OBS_MEDIA_STATE_NONE:
		hide();
		SetResetStatus();
		return;
	case OBS_MEDIA_STATE_OPENING:
		SetResetStatus();
		break;
	case OBS_MEDIA_STATE_ENDED:
	case OBS_MEDIA_STATE_STOPPED:
		// when media state is stopped or ended, duration maybe invalid,so dont't need call RefreshControls();
		SetRestartState();
		if (GetSettingsValueForBool("clear_on_media_end")) {
			hide();
			return;
		}
		break;
	case OBS_MEDIA_STATE_PLAYING:
		SetPlayingState();
		RefreshControls();
		break;
	case OBS_MEDIA_STATE_PAUSED:
		SetPausedState();
		RefreshControls();
		break;
	default:
		break;
	}
show:
	if (CheckValid() && isHidden())
		show();
	return;
}

void PLSMediaController::RefreshControls()
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return;

	bool is_looping = GetSettingsValueForBool(LOOPING);
	int64_t duration = obs_source_media_get_duration(source);
	if (duration < 0) {
		obs_data_t *invalidDuration = obs_data_create();
		obs_data_set_string(invalidDuration, "method", "invalid_duration_local_file");
		obs_source_get_private_data(source, invalidDuration);
		bool isInvalidDuration = obs_data_get_bool(invalidDuration, "invalid_duration_local_file");
		obs_data_release(invalidDuration);

		if (isInvalidDuration) {
			UpdateControlsForInvalidDuration();
		} else {
			pls_flush_style(ui->loopButton, "isLoop", false);
			ui->label_live->setText("LIVE");
			ui->slider->setValue(ui->slider->maximum());
			ui->slider->setMediaSliderEnabled(false);
			ui->loopButton->setEnabled(false);

			ui->widget_time->hide();
			ui->label_live->show();
		}
	} else {
		pls_flush_style(ui->loopButton, "isLoop", is_looping);

		float time = (float)obs_source_media_get_time(source);
		float sliderPosition = (time / (float)duration) * (float)ui->slider->maximum();
		ui->slider->setValue((int)sliderPosition);
		ui->timerLabel->setText(FormatSeconds((int)(time / 1000.0f)));
		ui->durationLabel->setText(FormatSeconds((int)(duration / 1000.0f)));

		ui->slider->setMediaSliderEnabled(true);
		ui->loopButton->setEnabled(true);

		ui->label_live->hide();
		ui->widget_time->show();
	}
}

quint64 PLSMediaController::GetSceneItem()
{
	return sceneItem;
}

void PLSMediaController::SetSceneItem(QString newSourceName, quint64 newSceneItem)
{
	if (newSceneItem == sceneItem)
		return;

	OBSSource preSource = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (preSource) {
		signal_handler_disconnect(obs_source_get_signal_handler(preSource), "media_properties_changed", PropertiesChanged, this);
		signal_handler_disconnect(obs_source_get_signal_handler(preSource), "media_eof", MediaEof, this);
		signal_handler_disconnect(obs_source_get_signal_handler(preSource), "rename", MediaRenamed, this);
		signal_handler_disconnect(obs_source_get_signal_handler(preSource), "media_state_changed", MediaStateChanged, this);
	}

	sceneItem = newSceneItem;
	sourceName = newSourceName;

	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (source) {
		signal_handler_connect_ref(obs_source_get_signal_handler(source), "media_properties_changed", PropertiesChanged, this);
		signal_handler_connect_ref(obs_source_get_signal_handler(source), "media_eof", MediaEof, this);
		signal_handler_connect_ref(obs_source_get_signal_handler(source), "rename", MediaRenamed, this);
		signal_handler_connect_ref(obs_source_get_signal_handler(source), "media_state_changed", MediaStateChanged, this);
	}
}

void PLSMediaController::UpdateMediaSource(obs_scene_item *item, bool visibleChanged)
{
	obs_source_t *source = obs_sceneitem_get_source(item);
	if (!source) {
		ResetAndHide();
		return;
	}

	OBSScene parent = obs_sceneitem_get_scene(item);
	OBSScene currentScene = main->GetCurrentScene();
	if (parent != currentScene && obs_scene_is_group(parent)) {
		obs_source_t *groupSource = obs_scene_get_source(parent);
		obs_sceneitem_t *sceneitem = obs_scene_get_group(main->GetCurrentScene(), obs_source_get_name(groupSource));
		bool groupVisible = obs_sceneitem_visible(sceneitem);
		if (!groupVisible) {
			ResetAndHide();
			return;
		}
	}

	bool flag = obs_source_get_output_flags(source) & OBS_SOURCE_CONTROLLABLE_MEDIA;
	bool select = obs_sceneitem_selected(item);

	if (main->CountSelectedItem() != 1 || !flag || !select) {
		ResetAndHide();
		return;
	}

	if (main->CountSelectedItem() == 1) {
		QString name = obs_source_get_name(source);
		SetSceneItem(name, (quint64)item);

		enum obs_source_error error;
		bool valid = obs_source_get_capture_valid(source, &error);
		bool visible = obs_sceneitem_visible(pls_get_sceneitem_by_pointer_address(main->GetCurrentScene(), (void *)sceneItem));
		if (!visible || !valid)
			hide();
		else
			RefreshControllerAndShow();
	}
	return;
}

void PLSMediaController::SceneItemVisibleChanged(obs_sceneitem_t *item, bool isVisible)
{
	visible = isVisible;
	if (obs_sceneitem_is_group(item)) {
		//PRISM/ZengQin/20200807/#3825 #3824/fix hide or show in group
		obs_sceneitem_group_enum_items(item, UpdateSelectItem, this);
	} else if (obs_sceneitem_selected(item)) {
		QMetaObject::invokeMethod(
			this, [=]() { UpdateMediaSource(item, true); }, Qt::QueuedConnection);
	}
}

void PLSMediaController::SceneItemRemoveChanged(obs_sceneitem_t *item)
{
	if ((quint64)item != sceneItem)
		return;
	QMetaObject::invokeMethod(
		this, [=]() { ResetAndHide(); }, Qt::QueuedConnection);
}

void PLSMediaController::ResizeLoading()
{
	if (lableLoad != nullptr) {
		lableLoad->setGeometry((width() - lableLoad->width()) / 2, (height() - lableLoad->height() + PLSDpiHelper::calculate(this, 5)) / 2, lableLoad->width(), lableLoad->height());
	}
}

void PLSMediaController::UpdateSliderPosition()
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return;

	if (!CheckValid() || UpdateLoading())
		return;

	int64_t duration = obs_source_media_get_duration(source);
	obs_media_state state = obs_source_media_get_state(source);
	if (duration > 0 && (state == OBS_MEDIA_STATE_PLAYING || state == OBS_MEDIA_STATE_PAUSED)) {

		if (!obs_source_media_is_update_done(source))
			return;

		float time = (float)obs_source_media_get_time(source);
		float sliderPosition = (time / (float)duration) * (float)ui->slider->maximum();
		ui->timerLabel->setText(FormatSeconds((int)(time / 1000.0f)));
		ui->durationLabel->setText(FormatSeconds((int)(duration / 1000.0f)));

		if (isDragging)
			return;

		ui->slider->setValue((int)sliderPosition);
	}
	return;
}

QString PLSMediaController::FormatSeconds(int totalSeconds)
{
	int seconds = totalSeconds % 60;
	int totalMinutes = totalSeconds / 60;
	int minutes = totalMinutes % 60;
	int hours = totalMinutes / 60;

	QString text;
	text.sprintf("%02d:%02d:%02d", hours, minutes, seconds);

	return text;
}

void PLSMediaController::on_playPauseButton_clicked()
{
	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return;

	obs_media_state state = obs_source_media_get_state(source);

	switch (state) {
	case OBS_MEDIA_STATE_STOPPED:
	case OBS_MEDIA_STATE_ENDED:
		obs_source_media_restart(source);
		PLS_UI_STEP(MAIN_MEDIA_CONTROL, "media control restart Button", ACTION_CLICK);
		break;
	case OBS_MEDIA_STATE_PLAYING:
		obs_source_media_play_pause(source, true);
		PLS_UI_STEP(MAIN_MEDIA_CONTROL, "media control pause Button", ACTION_CLICK);
		break;
	case OBS_MEDIA_STATE_PAUSED:
		obs_source_media_play_pause(source, false);
		PLS_UI_STEP(MAIN_MEDIA_CONTROL, "media control playing Button", ACTION_CLICK);
		break;
	default:
		break;
	}
}

void PLSMediaController::on_loopButton_clicked()
{
	PLS_UI_STEP(MAIN_MEDIA_CONTROL, "loop Button", ACTION_CLICK);

	OBSSource source = pls_get_source_by_name(sourceName.toStdString().c_str());
	if (!source)
		return;

	OBSData settings = pls_get_source_setting(source);

	bool loop = obs_data_get_bool(settings, LOOPING.toStdString().c_str());
	obs_data_set_bool(settings, LOOPING.toStdString().c_str(), !loop);
	emit RefreshSourceProperty(sourceName);

	obs_source_update(source, settings);

	pls_flush_style(ui->loopButton, "isLoop", !loop);
}
