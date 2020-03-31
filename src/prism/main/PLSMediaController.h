#pragma once

#include <QTimer>
#include <obs.hpp>
#include <dialog-view.hpp>
#include "window-basic-main.hpp"
#include "qt-wrappers.hpp"
#include "frontend-api.h"
#include "loading-event.hpp"

#include "ui_PLSMediaController.h"

class PLSMediaController : public QWidget {
	Q_OBJECT

private:
	QString sourceName{};
	quint64 sceneItem{};
	PLSBasic *main;
	QTimer *timer;
	bool isDragging = false;
	bool visible = false;

	QLabel *lableLoad;
	PLSLoadingEvent m_loadingEvent;

	QString FormatSeconds(int totalSeconds);
	void StartTimer();
	void StopTimer();
	void RefreshControls();
	void UpdateControlsStatus();
	void SetScene(OBSScene scene);
	void SeekTo(int val);

	bool UpdateLoading();
	void StartLoading();
	void StopLoading();
	bool CheckValid();

	bool GetSettingsValueForBool(QString str);
	void SetMediaIsValidForNetwork(bool isConnect);

	static bool UpdateSelectItem(obs_scene_t *scene, obs_sceneitem_t *item, void *param);

	static void MediaEof(void *data, calldata_t *calldata);
	static void MediaRenamed(void *data, calldata_t *calldata);
	static void MediaStateChanged(void *data, calldata_t *calldata);
	static void PropertiesChanged(void *param, calldata_t *data);

	static void FrontendEvent(obs_frontend_event event, void *ptr);

	std::unique_ptr<Ui::PLSMediaController> ui;

	bool eventFilter(QObject *target, QEvent *event);

private slots:
	void on_playPauseButton_clicked();
	void on_loopButton_clicked();

	// slider state
	void SliderClicked(int val);
	void SliderReleased(int val);
	void SliderMoved(int val);
	void UpdateSliderPosition();

	// media state
	void SetPlayingState();
	void SetPausedState();
	void SetRestartState();
	void SetEofState(const QString &name);
	void SetLoadState(bool load, obs_source_t *sourcePtr);
	void OnPropertiesChanged(const QString &name);
	void OnMediaStateChanged(const QString &name);
	void SetResetStatus();
	void UpdateControlsForInvalidDuration();

	void RefreshControllerAndShow();
	void ResetAndHide();

	void MediaSourceRenamed(const QString &name, obs_source_t *sourcePtr);

signals:
	void RefreshSourceProperty(const QString &sourceName);

public:
	explicit PLSMediaController(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSMediaController();

	quint64 GetSceneItem();
	void SetSceneItem(QString newSourceName, quint64 newSceneItem);

	void UpdateMediaSource(obs_scene_item *item, bool visibleChanged = false);
	void SceneItemVisibleChanged(obs_sceneitem_t *item, bool isVisible);
	void SceneItemRemoveChanged(obs_sceneitem_t *item);

	void ResizeLoading();
};
