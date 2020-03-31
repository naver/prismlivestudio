/******************************************************************************
    Copyright (C) 2013-2014 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include <QBuffer>
#include <QAction>
#include <QWidgetAction>
#include <QSystemTrayIcon>
#include <QStyledItemDelegate>
#include <obs.hpp>
#include <vector>
#include <memory>
#include "window-main.hpp"
#include "window-basic-properties.hpp"
#include "window-basic-transform.hpp"
#include "window-basic-adv-audio.hpp"
#include "window-basic-filters.hpp"
#include "window-projector.hpp"
#include "window-basic-about.hpp"
#include "auth-base.hpp"

#include <frontend-internal.hpp>

#include <util/platform.h>
#include <util/threading.h>
#include <util/util.hpp>

#include <QPointer>

class QListWidgetItem;
class VolControl;
class PLSBasicStats;
class PLSApp;
class PLSDialogView;
struct QCefCookieManager;
struct GiphyData;
struct TaskResponData;

#include "ui_PLSBasic.h"
#include "ui_ColorSelect.h"

#define DESKTOP_AUDIO_1 Str("DesktopAudioDevice1")
#define DESKTOP_AUDIO_2 Str("DesktopAudioDevice2")
#define AUX_AUDIO_1 Str("AuxAudioDevice1")
#define AUX_AUDIO_2 Str("AuxAudioDevice2")
#define AUX_AUDIO_3 Str("AuxAudioDevice3")
#define AUX_AUDIO_4 Str("AuxAudioDevice4")

#define SIMPLE_ENCODER_X264 "x264"
#define SIMPLE_ENCODER_X264_LOWCPU "x264_lowcpu"
#define SIMPLE_ENCODER_QSV "qsv"
#define SIMPLE_ENCODER_NVENC "nvenc"
#define SIMPLE_ENCODER_AMD "amd"

#define PREVIEW_EDGE_SIZE 25
#define APP_INSTALL_ACCEPT 200

extern std::vector<QString> presetColorList;
struct BasicOutputHandler;
class PLSMainView;
class PLSPopupMenu;
class PLSBeautyFilterView;
class PLSBackgroundMusicView;
class PLSBackgroundMusicPlayer;
//zq
class PLSMediaController;
class PLSGipyStickerView;

class PLSChatDialog;
class PLSPlatformApi;
class PLSRegionCapture;

using DShowSourceVecType = std::vector<std::pair<QString, OBSSceneItem>>;
using BgmSourceVecType = std::vector<std::pair<QString, quint64>>;

struct AudioStatus {
	int audioSourceCount = 0; //audio device or audio source
	bool isAllAudioMuted = true;
};

enum class MainCheckUpdateResult {
	Failed,
	HasUpdate,
	NoUpdate,
};

enum class QtDataRole {
	PLSRef = Qt::UserRole,
	OBSSignals,
};

struct SavedProjectorInfo {
	ProjectorType type;
	int monitor;
	std::string geometry;
	std::string name;
};

class ColorSelect : public QWidget {
	Q_OBJECT
public:
	explicit ColorSelect(QWidget *parent = 0);

protected:
	// Here we just ignore mouse click messages to avoid hiding menu
	virtual void mousePressEvent(QMouseEvent *event) {}

private:
	std::unique_ptr<Ui::ColorSelect> ui;
};

class PLSBasic : public PLSMainWindow {
	Q_OBJECT
	Q_PROPERTY(QIcon imageIcon READ GetImageIcon WRITE SetImageIcon DESIGNABLE true)
	Q_PROPERTY(QIcon colorIcon READ GetColorIcon WRITE SetColorIcon DESIGNABLE true)
	Q_PROPERTY(QIcon slideshowIcon READ GetSlideshowIcon WRITE SetSlideshowIcon DESIGNABLE true)
	Q_PROPERTY(QIcon audioInputIcon READ GetAudioInputIcon WRITE SetAudioInputIcon DESIGNABLE true)
	Q_PROPERTY(QIcon audioOutputIcon READ GetAudioOutputIcon WRITE SetAudioOutputIcon DESIGNABLE true)
	Q_PROPERTY(QIcon desktopCapIcon READ GetDesktopCapIcon WRITE SetDesktopCapIcon DESIGNABLE true)
	Q_PROPERTY(QIcon windowCapIcon READ GetWindowCapIcon WRITE SetWindowCapIcon DESIGNABLE true)
	Q_PROPERTY(QIcon gameCapIcon READ GetGameCapIcon WRITE SetGameCapIcon DESIGNABLE true)
	Q_PROPERTY(QIcon cameraIcon READ GetCameraIcon WRITE SetCameraIcon DESIGNABLE true)
	Q_PROPERTY(QIcon textIcon READ GetTextIcon WRITE SetTextIcon DESIGNABLE true)
	Q_PROPERTY(QIcon mediaIcon READ GetMediaIcon WRITE SetMediaIcon DESIGNABLE true)
	Q_PROPERTY(QIcon browserIcon READ GetBrowserIcon WRITE SetBrowserIcon DESIGNABLE true)
	Q_PROPERTY(QIcon groupIcon READ GetGroupIcon WRITE SetGroupIcon DESIGNABLE true)
	Q_PROPERTY(QIcon sceneIcon READ GetSceneIcon WRITE SetSceneIcon DESIGNABLE true)
	Q_PROPERTY(QIcon bgmIcon READ GetBgmIcon WRITE SetBgmIcon DESIGNABLE true)
	Q_PROPERTY(QIcon stickerIcon READ GetStickerIcon WRITE SetStickerIcon DESIGNABLE true)
	Q_PROPERTY(QIcon textMotionIcon READ GetTextMotionIcon WRITE SetTextMotionIcon DESIGNABLE true)
	Q_PROPERTY(QIcon chatIcon READ GetChatIcon WRITE SetChatIcon DESIGNABLE true)
	Q_PROPERTY(QIcon defaultIcon READ GetDefaultIcon WRITE SetDefaultIcon DESIGNABLE true)
	Q_PROPERTY(QIcon ndiIcon READ GetNdiIcon WRITE SetNdiIcon DESIGNABLE true)

	friend class PLSAbout;
	friend class PLSBasicPreview;
	friend class PLSBasicStatusBar;
	friend class PLSBasicSourceSelect;
	friend class PLSBasicSettings;
	friend class Auth;
	friend class AutoConfig;
	friend class AutoConfigStreamPage;
	friend class RecordButton;
	friend class ExtraBrowsersModel;
	friend class ExtraBrowsersDelegate;
	friend struct PLSStudioAPI;
	friend class PLSApp;
	friend class SourceTree;
	friend class PLSChatDialog;
	friend class PLSPlatformApi;

	enum class MoveDir { Up, Down, Left, Right };

	enum DropType {
		DropType_RawText,
		DropType_Text,
		DropType_Image,
		DropType_Media,
		DropType_Html,
	};

private:
	pls_frontend_callbacks *api = nullptr;

	std::shared_ptr<Auth> auth;

	std::vector<VolControl *> volumes;

	std::vector<OBSSignal> signalHandlers;

	QList<QPointer<QDockWidget>> extraDocks;

	bool loaded = false;
	long disableSaving = 1;
	bool projectChanged = false;
	bool previewEnabled = true;
	bool isSelected = false;
	bool isCopyScene = false;
	//#4606 crash /const char * to QString / by zengqin
	QString copyString;
	QString copyFiltersString;
	bool copyVisible = true;
	bool loadingScene = false;

	QAction *actionSeperateScene;
	QAction *actionSeperateSource;
	QAction *actionSperateMixer;

	QScopedPointer<QThread> logUploadThread;

	QPointer<PLSBasicProperties> properties;
	QPointer<PLSBasicTransform> transformWindow;
	QPointer<PLSBasicAdvAudio> advAudioWindow;
	QPointer<PLSBasicFilters> filters;
	QPointer<PLSAbout> about;

	QPointer<QTimer> cpuUsageTimer;
	QPointer<QTimer> diskFullTimer;

	os_cpu_usage_info_t *cpuUsageInfo = nullptr;

	OBSService service;
	std::unique_ptr<BasicOutputHandler> outputHandler;
	bool streamingStopping = false;
	bool recordingStopping = false;
	bool replayBufferStopping = false;

	gs_vertbuffer_t *box = nullptr;
	gs_vertbuffer_t *boxLeft = nullptr;
	gs_vertbuffer_t *boxTop = nullptr;
	gs_vertbuffer_t *boxRight = nullptr;
	gs_vertbuffer_t *boxBottom = nullptr;
	gs_vertbuffer_t *circle = nullptr;

	bool sceneChanging = false;
	bool ignoreSelectionUpdate = false;

	int previewX = 0, previewY = 0;
	int previewCX = 0, previewCY = 0;
	float previewScale = 0.0f;

	ConfigFile basicConfig;

	std::vector<SavedProjectorInfo *> savedProjectorsArray;
	QPair<QPointer<PLSDialogView>, PLSProjector *> projectors[10];
	QList<QPair<QPointer<PLSDialogView>, PLSProjector *>> windowProjectors;

	QPointer<PLSDialogView> stats;
	QPointer<QWidget> remux;
	//add by zq
	QPointer<PLSMediaController> mediaController{nullptr};

	QPointer<QMenu> startStreamMenu;

	//QPointer<QPushButton> replayBufferButton;
	QScopedPointer<QPushButton> pause;

	QScopedPointer<QSystemTrayIcon> trayIcon;
	QPointer<QAction> sysTrayStream;
	QPointer<QAction> sysTrayRecord;
	//QPointer<QAction> sysTrayReplayBuffer;
	QPointer<QAction> showHide;
	QPointer<QAction> exit;
	QPointer<QMenu> trayMenu;
	QPointer<QMenu> previewProjector;
	QPointer<QMenu> studioProgramProjector;
	QPointer<QMenu> multiviewProjectorMenu;
	QPointer<QMenu> previewProjectorSource;
	QPointer<QMenu> previewProjectorMain;
	QPointer<QMenu> sceneProjectorMenu;
	QPointer<QMenu> sourceProjector;
	QPointer<QMenu> scaleFilteringMenu;
	QPointer<QMenu> colorMenu;
	QPointer<QWidgetAction> colorWidgetAction;
	QPointer<ColorSelect> colorSelect;
	QPointer<QMenu> deinterlaceMenu;
	QPointer<QMenu> perSceneTransitionMenu;
	QPointer<QObject> shortcutFilter;

	QScopedPointer<QThread> patronJsonThread;
	std::string patronJson;
	bool m_requestUpdate;

	PLSMainView *mainView;
	PLSPopupMenu *mainMenu;
	QPointer<PLSBeautyFilterView> beautyFilterView{nullptr};
	PLSBackgroundMusicView *backgroundMusicView{nullptr};
	QPointer<PLSGipyStickerView> giphyStickerView{nullptr};
	QPointer<PLSRegionCapture> regionCapture{nullptr};

	bool m_isUpdateLanguage;
	bool m_isSessionExpired; //session expired

	//Fast to stop core if click "Close" title button
	bool m_bFastStop = false;
	bool *m_allMuted{nullptr};

	void *interaction_sceneitem_pointer = NULL;

	std::map<unsigned long long, QString> stickerSourceMap;
	bool isCreateSouceInLoading = true;
	bool isAudioMonitorToastDisplay = false;

	void UpdateMultiviewProjectorMenu();

	void DrawBackdrop(float cx, float cy);

	void SetupEncoders();

	void CreateFirstRunSources();
	void CreateDefaultScene(bool firstStart);

	void UpdateVolumeControlsDecayRate();
	void UpdateVolumeControlsPeakMeterType();
	void ClearVolumeControls();

	void Save(const char *file);
	void Load(const char *file);

	void InitHotkeys();
	void CreateHotkeys();
	void ClearHotkeys();

	bool InitService();

	bool InitBasicConfigDefaults();
	void InitBasicConfigDefaults2();
	bool InitBasicConfig();

	void InitPLSCallbacks();

	void InitPrimitives();

	void initMainMenu();

	void OnFirstLoad();

	//add by zq
	void CreateMediaController();
	void ResizeMediaController();

	OBSSceneItem GetSceneItem(QListWidgetItem *item);

	void GetFPSCommon(uint32_t &num, uint32_t &den) const;
	void GetFPSInteger(uint32_t &num, uint32_t &den) const;
	void GetFPSFraction(uint32_t &num, uint32_t &den) const;
	void GetFPSNanoseconds(uint32_t &num, uint32_t &den) const;

	void UpdatePreviewScalingMenu();

	void LoadSceneListOrder(obs_data_array_t *array, const char *file);
	obs_data_array_t *SaveSceneListOrder();

	void LoadBeautyConfig(obs_data_t *obj);
	obs_data_t *SaveBeautyConfig();

	void TempFileOutput(const char *path, int vBitrate, int aBitrate);
	void TempStreamOutput(const char *url, const char *key, int vBitrate, int aBitrate);

	void CloseDialogs();
	void ClearSceneData();

	void Nudge(int dist, MoveDir dir);

	PLSDialogView *OpenProjector(obs_source_t *source, int monitor, QString title, ProjectorType type);

	void GetAudioSourceFilters();
	void GetAudioSourceProperties();
	void VolControlContextMenu();
	void ToggleVolControlLayout();
	void ToggleMixerLayout(bool vertical);

	void UpdateSceneCollection(QAction *action);
	void ChangeSceneCollection();
	void LoadSceneCollection(QAction *action);
	void LogScenes();

	void LoadProfile();
	void ResetProfileData();
	bool AddProfile(bool create_new, const char *title, const char *text, const char *init_text = nullptr, bool rename = false, bool addDefault = false);
	void DeleteProfile(const char *profile_name, const char *profile_dir);
	void RefreshProfiles();
	void ChangeProfile();
	void CheckForSimpleModeX264Fallback();

	void SaveProjectNow();

	obs_hotkey_pair_id streamingHotkeys, recordingHotkeys, pauseHotkeys, replayBufHotkeys, togglePreviewHotkeys, toggleMuteAllHotkeys;
	obs_hotkey_id selectRegionHotkey;
	obs_hotkey_id forceStreamingStopHotkey;

	void InitDefaultTransitions();

	obs_source_t *cutTransition;
	void CreateProgramDisplay();

	QMenu *CreatePerSceneTransitionMenu();

	void SetPreviewProgramMode(bool enabled);
	void ResizeProgram(uint32_t cx, uint32_t cy);
	void SetCurrentScene(obs_scene_t *scene, bool force = false);
	static void RenderProgram(void *data, uint32_t cx, uint32_t cy);

	QPointer<PLSQTDisplay> program;
	OBSWeakSource lastScene;
	OBSWeakSource swapScene;
	OBSWeakSource programScene;
	bool editPropertiesMode = false;
	bool sceneDuplicationMode = true;
	bool swapScenesMode = false;
	volatile bool previewProgramMode = false;
	obs_hotkey_id togglePreviewProgramHotkey = 0;
	obs_hotkey_id transitionHotkey = 0;
	obs_hotkey_id statsHotkey = 0;
	int quickTransitionIdCounter = 1;
	bool overridingTransition = false;

	int programX = 0, programY = 0;
	int programCX = 0, programCY = 0;
	float programScale = 0.0f;

	int disableOutputsRef = 0;

	inline void OnActivate();
	inline void OnDeactivate();

	void AddDropSource(const char *file, DropType image);
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

	bool CheckSuffix(const QString &file, const char *suffix, const char *extensions[], enum DropType type);

	void ReplayBufferClicked();
	//add by zzc
	bool ReplayBufferPreCheck();
	bool StartReplayBufferWithNoCheck();
	//end add

	bool sysTrayMinimizeToTray();

	void EnumDialogs();

	QList<QDialog *> visDialogs;
	QList<QDialog *> modalDialogs;
	QList<PLSDialogView *> visMsgBoxes;

	QString m_updateGcc;
	QString m_updatevVersion;
	QString m_updateFileUrl;
	QString m_updateInfoUrl;
	bool m_updateForceUpdate;
	MainCheckUpdateResult m_checkUpdateResult;
	bool m_checkLastesVersiontResult;

	QList<QPoint> visDlgPositions;

	QByteArray startingDockLayout;

	obs_data_array_t *SaveProjectors();
	void LoadSavedProjectors(obs_data_array_t *savedProjectors);

	bool NoSourcesConfirmation();

	void ClearStickerCache();
	void DownloadStickerForSource(OBSSource source, unsigned long long uniqueId);
	void SetStickerSourceSetting(OBSSource source, const QString &file, const GiphyData &giphyData);
	GiphyData GetStickerSourceData(OBSSource source);

#ifdef BROWSER_AVAILABLE
	QList<QSharedPointer<QDockWidget>> extraBrowserDocks;
	QList<QSharedPointer<QAction>> extraBrowserDockActions;
	QStringList extraBrowserDockTargets;

	void ClearExtraBrowserDocks();
	void LoadExtraBrowserDocks();
	void SaveExtraBrowserDocks();
	void AddExtraBrowserDock(const QString &title, const QString &url, bool firstCreate);
#endif

	QIcon imageIcon;
	QIcon colorIcon;
	QIcon slideshowIcon;
	QIcon audioInputIcon;
	QIcon audioOutputIcon;
	QIcon desktopCapIcon;
	QIcon windowCapIcon;
	QIcon gameCapIcon;
	QIcon cameraIcon;
	QIcon textIcon;
	QIcon mediaIcon;
	QIcon browserIcon;
	QIcon groupIcon;
	QIcon sceneIcon;
	QIcon stickerIcon;
	QIcon defaultIcon;
	QIcon bgmIcon;
	QIcon ndiIcon;
	QIcon textMotionIcon;
	QIcon chatIcon;

	QIcon GetImageIcon() const;
	QIcon GetColorIcon() const;
	QIcon GetSlideshowIcon() const;
	QIcon GetAudioInputIcon() const;
	QIcon GetAudioOutputIcon() const;
	QIcon GetDesktopCapIcon() const;
	QIcon GetWindowCapIcon() const;
	QIcon GetGameCapIcon() const;
	QIcon GetCameraIcon() const;
	QIcon GetTextIcon() const;
	QIcon GetMediaIcon() const;
	QIcon GetBrowserIcon() const;
	QIcon GetBgmIcon() const;
	QIcon GetStickerIcon() const;
	QIcon GetNdiIcon() const;
	QIcon GetTextMotionIcon() const;
	QIcon GetChatIcon() const;
	QIcon GetDefaultIcon() const;
public slots:
	void DeferSaveBegin();
	void DeferSaveEnd();

	bool StartStreaming();
	void StopStreaming();
	void ForceStopStreaming();

	void StreamDelayStarting(int sec);
	void StreamDelayStopping(int sec);

	void StreamingStart();
	void StreamStopping();
	void StreamingStop(int errorcode, QString last_error);

	bool StartRecording();
	bool StopRecording();

	void RecordingStart();
	void RecordStopping();
	void RecordingStop(int code, QString last_error);

	void ShowReplayBufferPauseWarning();
	void StartReplayBuffer();
	void StopReplayBuffer();

	void ReplayBufferStart();
	void ReplayBufferSave();
	void ReplayBufferStopping();
	void ReplayBufferStop(int code);
	void ReplayBufferSaved(int code, const QString &errorStr);

	void SaveProjectDeferred();
	void SaveProject();

	void OverrideTransition(OBSSource transition);
	void TransitionToScene(OBSScene scene, bool force = false);
	void TransitionToScene(OBSSource scene, bool force = false, bool quickTransition = false, int quickDuration = 0, bool black = false);
	void SetCurrentScene(OBSSource scene, bool force = false);
	void SetCutTransition(obs_source_t *source);

	void OnTransitionAdded();
	void onTransitionRemoved(OBSSource source);
	void OnTransitionRenamed();
	void OnTransitionSet();
	void OnTransitionDurationValueChanged(int value);

	bool SceneCollectionExists(const char *findName);
	bool GetSceneCollectionName(QWidget *parent, std::string &name, std::string &file, const char *oldName = nullptr);
	bool AddSceneCollection(bool create_new, const QString &name = QString());

	void UpdatePatronJson(const QString &text, const QString &error);

	void PauseRecording();
	void UnpauseRecording();
	void SetSceneDockSeperated(bool state);
	bool GetSceneDockSeperated();
	void SetSourceDockSeperated(bool state);
	bool GetSourceDockSeperated();

	void toastMessage(pls_toast_info_type type, const QString &message, int autoClose);
	void toastMessage(pls_toast_info_type type, const QString &message, const QString &url, const QString &replaceStr, int autoClose);
	void toastClear();

	void OnSceneDockTopLevelChanged();
	void OnSourceDockTopLevelChanged();
	void onMixerDockLocationChanged();

	void onUpdateChatActionText(bool shown);
	void RefreshSceneCollections();

	void GetSelectDshowSourceAndList(QString &selectSourceName, OBSSceneItem &item, DShowSourceVecType &dshowList);
	void OnBeautyCurrentSourceChanged(const QString &name, OBSSceneItem item);
	bool ExistSameSource(OBSSceneItem item, OBSScene sceneCurrent);
	BgmSourceVecType EnumAllBgmSource();

private slots:
	void AddSceneUI(const QString &name, OBSScene scene, SignalContainer<OBSScene> handler);

	// from signals of libobs
	void AddSceneItem(OBSSceneItem item);
	void RemoveSceneItem(OBSSceneItem item);

	void OnRemoveSourceTreeItem(OBSSceneItem item);
	void AddBeautyItem(OBSSceneItem item, bool existConfig, bool isCurrent);

	void SceneItemSetVisible(OBSSceneItem item, bool isVisible);
	void AddScene(OBSSource source);
	void RemoveScene(OBSSource source);
	void RenameSources(OBSSource source, QString newName, QString prevName);

	//bgm
	void AddBgmItem(obs_scene_item *item);
	void RemoveBgmItem(obs_scene_item *item);
	void RemoveBgmList(obs_scene_t *scene);
	void SetBgmItemSelected(obs_scene_item *item, bool isSelected);
	void SetBgmItemVisible(obs_scene_item *item, bool isVisible);
	void RenameBgmSourceName(obs_sceneitem_t *sceneItem, const QString &newName, const QString &prevName);

	void SelectSceneItem(OBSScene scene, OBSSceneItem item, bool select);
	void SelectSceneItemForBeauty(OBSScene scene, OBSSceneItem item, bool select);
	void OnBeautySourceStatusChanged(const QString &sourceName, bool status);
	void ActivateAudioSource(OBSSource source);
	void DeactivateAudioSource(OBSSource source);
	void RemoveAudioControl(OBSSource source);

	void DuplicateSelectedScene();
	void RemoveSelectedScene();
	void RemoveSelectedSceneItem();

	void ToggleAlwaysOnTop();

	void ReorderSources(OBSScene scene);

	void ProcessHotkey(obs_hotkey_id id, bool pressed);

	void TransitionClicked();
	void TransitionStopped();
	void TransitionFullyStopped();

	void SetDeinterlacingMode();
	void SetDeinterlacingOrder();

	void SetScaleFilter();

	void IconActivated(QSystemTrayIcon::ActivationReason reason);
	void SetShowing(bool showing);

	void ToggleShowHide();

	void HideAudioControl();
	void UnhideAllAudioControls();
	void ToggleHideMixer();

	void MixerRenameSource();

	void on_vMixerScrollArea_customContextMenuRequested();
	void on_hMixerScrollArea_customContextMenuRequested();

	void on_actionCopySource_triggered();
	void on_actionPasteRef_triggered();
	void on_actionPasteDup_triggered();

	void on_actionCopyFilters_triggered();
	void on_actionPasteFilters_triggered();
	void OnSourceDockSeperatedBtnClicked();
	void OnSceneDockSeperatedBtnClicked();
	void onMixerDockSeperateBtnClicked();

	void OnSourceNotifyAsync(QString name, int msg, int code);

	void SetDocksMovePolicy(PLSDock *dock);
	void ColorChange();

	SourceTreeItem *GetItemWidgetFromSceneItem(obs_sceneitem_t *sceneItem);

	void on_actionOpenSourceLicense_triggered();

	void AudioMixerCopyFilters();
	void AudioMixerPasteFilters();

	void EnablePreview();
	void DisablePreview();

	void MuteAllAudio();
	void UnmuteAllAudio();

	void SceneCopyFilters();
	void ScenePasteFilters();

	void CheckDiskSpaceRemaining();

	void ScenesReordered(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);

	void ResetStatsHotkey();

	void onPopupSettingView(const QString &tab, const QString &group);
	void OnBeautyClicked();
	void OnStickerClicked();
	void OnSetBeautyVisible(bool state);
	void InitBeautyView();
	void InitGiphyStickerView();
	void InitGiphyStickerViewVisible();
	void CreateBeautyView();
	void SetBeautyViewVisible(bool visible);
	void ReorderBeautySourceList();
	void CleanBeautySourceConfig(obs_scene_t *scene);
	void UpdateBeautySourceStatus(obs_source_t *source, OBSSceneItem item, bool status);
	bool LoadBeautyConfigFromSource(OBSSceneItem item, bool &isCurrent);

	// bgm
	void OnBgmClicked();
	void createBackgroundMusicView();
	void SetBgmViewVisible(bool visible);
	void OnSetBgmViewVisible(bool state);
	void ReorderBgmSourceList();
	bool GetBgmViewVisible();

	void SetImageIcon(const QIcon &icon);
	void SetColorIcon(const QIcon &icon);
	void SetSlideshowIcon(const QIcon &icon);
	void SetAudioInputIcon(const QIcon &icon);
	void SetAudioOutputIcon(const QIcon &icon);
	void SetDesktopCapIcon(const QIcon &icon);
	void SetWindowCapIcon(const QIcon &icon);
	void SetGameCapIcon(const QIcon &icon);
	void SetCameraIcon(const QIcon &icon);
	void SetTextIcon(const QIcon &icon);
	void SetMediaIcon(const QIcon &icon);
	void SetBrowserIcon(const QIcon &icon);
	void SetGroupIcon(const QIcon &icon);
	void SetSceneIcon(const QIcon &icon);
	void SetStickerIcon(const QIcon &icon);
	void SetDefaultIcon(const QIcon &icon);
	void SetBgmIcon(const QIcon &icon);
	void SetNdiIcon(const QIcon &icon);
	void SetTextMotionIcon(const QIcon &icon);
	void SetChatIcon(const QIcon &icon);

	void CreateStickerDownloader(OBSSource source);
	void VolumeMuted(const QString &sourceName, bool muted);
	void OpenRegionCapture();

private:
	/* PLS Callbacks */
	static void SceneReordered(void *data, calldata_t *params);
	static void SceneItemAdded(void *data, calldata_t *params);
	static void SceneItemRemoved(void *data, calldata_t *params);
	static void SceneItemSelected(void *data, calldata_t *params);
	static void SceneItemDeselected(void *data, calldata_t *params);
	static void OnSourceNotify(void *data, calldata_t *calldata);
	static void SourceCreated(void *data, calldata_t *params);
	static void SourceRemoved(void *data, calldata_t *params);
	static void SourceActivated(void *data, calldata_t *params);
	static void SourceDeactivated(void *data, calldata_t *params);
	static void SourceAudioActivated(void *data, calldata_t *params);
	static void SourceAudioDeactivated(void *data, calldata_t *params);
	static void SourceRenamed(void *data, calldata_t *params);
	static void RenderMain(void *data, uint32_t cx, uint32_t cy);

	//bgm
	static void MediaStateChanged(void *data, calldata_t *calldata);
	static void MediaLoad(void *data, calldata_t *calldata);
	static void PropertiesChanged(void *param, calldata_t *data);
	//audio source
	static void PLSVolumeMuted(void *data, calldata_t *calldata);

	static void frontendEventHandler(enum obs_frontend_event event, void *private_data);
	static void frontendEventHandler(pls_frontend_event event, const QVariantList &params, void *context);
	static void LogoutCallback(pls_frontend_event event, const QVariantList &params, void *context);

	void ResizePreview(uint32_t cx, uint32_t cy);

	void AddStickerSource(const char *id, const QString &file, const GiphyData &giphyData);
	void SetFileForStickerSource(OBSSource source, const QString &file);
	void BindActionData(QWidgetAction *popupItem, const char *type);
	QMenu *CreateAddSourcePopupMenu(QWidget *parent);
	bool AddMonitorSourceMenu(QMenu *popup, std::map<QString, bool> monitors);
	void AddMonitorSourceAction(QMenu *monitorMenu, QString id, QString displayName);
	void AddSourcePopupMenu(const QPoint &pos, QWidget *parent);
	void copyActionsDynamicProperties();

	void GetSourceTypeList(std::vector<std::vector<QString>> &preset, std::vector<QString> &other, std::map<QString, bool> &monitors);
	QPushButton *CreateSourcePopupMenuCustomWidget(const char *id, const QString &display, const QSize &iconSize);

	static void HotkeyTriggered(void *data, obs_hotkey_id id, bool pressed);

	void AutoRemux();

	void UpdatePause(bool activate = true);

	bool LowDiskSpace();
	void DiskSpaceMessage();

	void onPropertyChanged(OBSSource source);
	void SetAttachWindowBtnText(QAction *action, bool isFloating);
	OBSSource prevFTBSource = nullptr;

	bool CheckMasterAudioStatus(obs_data_t *data);
	void UpdateMasterSwitch(bool mute);
	bool ConfigAllMuted();
	void UpdateMixer(obs_source_t *source);

	AudioStatus GetMasterAudioStatusCurrent(obs_source_t *ignoreSource = nullptr, obs_source_t *ignoreDevice = nullptr);
	AudioStatus GetDeviceAudioStatusCurrent(obs_source_t *ignoreDevice);
	AudioStatus GetSourceAudioStatusCurrent(obs_source_t *ignoreSource);

	AudioStatus GetMasterAudioStatusPrevious();
	AudioStatus GetDeviceAudioStatusPrevious();
	AudioStatus GetSourceAudioStatusPrevious();
	bool hasSourceMonitor();
	void AddSelectRegionSource(const char *id, const QRect &rectSelected);

public:
	void resetWindowGeometry(QWidget *loginView, const QPoint prismLoginViewCenterPoint);
	OBSSource GetProgramSource();
	OBSScene GetCurrentScene();
	PLSSceneItemView *GetCurrentSceneItem();
	void AddSource(const char *id);

	void SysTrayNotify(const QString &text, QSystemTrayIcon::MessageIcon n);

	inline OBSSource GetCurrentSceneSource()
	{
		OBSScene curScene = GetCurrentScene();
		return OBSSource(obs_scene_get_source(curScene));
	}

	obs_service_t *GetService();
	void SetService(obs_service_t *service);

	inline bool IsPreviewProgramMode() const { return os_atomic_load_bool(&previewProgramMode); }

	bool StreamingActive() const;
	bool Active() const;

	void ResetUI();
	int ResetVideo();
	bool ResetAudio();

	void ResetOutputs();

	void ResetAudioDevice(const char *sourceId, const char *deviceId, const char *deviceDesc, int channel);

	void NewProject();
	void LoadProject();
	inline void setSessionExpired(bool isSessionExpired) { m_isSessionExpired = isSessionExpired; }
	inline void GetDisplayRect(int &x, int &y, int &cx, int &cy)
	{
		x = previewX;
		y = previewY;
		cx = previewCX;
		cy = previewCY;
	}

	inline bool SavingDisabled() const { return disableSaving; }

	inline double GetAppCPUUsage() const { return os_cpu_usage_info_query(cpuUsageInfo); }

	void SaveService();
	bool LoadService();

	inline Auth *GetAuth() { return auth.get(); }

	inline void EnableOutputs(bool enable)
	{
		if (enable) {
			if (--disableOutputsRef < 0)
				disableOutputsRef = 0;
		} else {
			disableOutputsRef++;
		}
	}

	QMenu *AddDeinterlacingMenu(QMenu *menu, obs_source_t *source);
	QMenu *AddScaleFilteringMenu(QMenu *menu, obs_sceneitem_t *item);
	QMenu *AddBackgroundColorMenu(QMenu *menu, QWidgetAction *widgetAction, ColorSelect *select, obs_sceneitem_t *item);
	void CreateSourcePopupMenu(int idx, bool preview, QWidget *parent);

	void UpdateTitleBar();
	void UpdateSceneSelection(OBSSource source);

	void SystemTrayInit();
	void SystemTray(bool firstStarted);

	void OpenSavedProjectors();

	void DeletePropertiesWindowInScene(obs_scene_t *scene);
	void DeleteFiltersWindowInScene(obs_scene_t *scene);
	void DeletePropertiesWindow(obs_source_t *source);
	void DeleteFiltersWindow(obs_source_t *source);
	void CreatePropertiesWindow(obs_source_t *source, unsigned flags = OPERATION_NONE, QWidget *parent = nullptr);
	void CreateFiltersWindow(obs_source_t *source);

	QAction *AddDockWidget(QDockWidget *dock);

	static PLSBasic *Get();

	void delSpecificUrlCookie(const QString &url, const QString &cookieName);
	const char *GetCurrentOutputPath();
	bool QueryRemoveSourceWithNoNotifier(obs_source_t *source);
	bool QueryRemoveSource(obs_source_t *source);
	int GetTransitionDuration();
	QSpinBox *GetTransitionDurationSpinBox();
	QComboBox *GetTransitionCombobox();
	OBSSource GetCurrentTransition();
	bool willShow();

	void mainViewClose(QCloseEvent *event);

	// update
	void CheckUpdate();
	void CheckLastVersion();
	bool ShowUpdateView(QWidget *parent);
	void ShowLoginView();
	MainCheckUpdateResult getUpdateResult() const;
	const QString &getUpdateFileUrl() const;
	const QString &getUpdateGcc() const;
	bool getCheckLastVersionResult() const;
	bool needShowUpdateView() const;
	void setActionShowAboutHasIcon(bool hasIcon);

	void showEncodingInStatusBar();

	QIcon GetSourceIcon(const char *id) const;
	QIcon GetGroupIcon() const;
	QIcon GetSceneIcon() const;

	void popupStats(bool show, const QPoint &pos);
	QWidget *getChannelsContainer() const;
	void docksMovePolicy(PLSDock *dock, const QPoint &pre);
	bool isDshowSourceBySourceId(OBSSceneItem &item);
	bool isBgmSourceBySourceId(obs_scene_item *item);
	int GetTopSelectedSourceItem();
	std::vector<int> GetAllSelectedSourceItem();
	OBSSceneItem GetCurrentSceneItemData();
	//add by zq
	int CountSelectedItem();
	void GetConfigFPS(uint32_t &num, uint32_t &den) const;
	//add by xw
	void InitSourceAudioStatus(obs_source_t *source);

	bool CheckHideInteraction(OBSSceneItem items);
	void HideAllInteraction(obs_source_t *except_source);
	void ShowInteractionUI(obs_source_t *source, bool show);

protected:
	virtual void showEvent(QShowEvent *event) override;
	virtual bool eventFilter(QObject *watcher, QEvent *event) override;

private slots:
	void on_actionFullscreenInterface_triggered();

	void on_actionShowVideosFolder_triggered();
	void on_actionRemux_triggered();
	void on_action_Settings_triggered();
	void on_actionAdvAudioProperties_triggered();
	void on_actionAudioMasterControl_triggered();
	void on_advAudioProps_clicked();
	void on_advAudioProps_destroyed();

	void on_actionEditTransform_triggered();
	void on_actionCopyTransform_triggered();
	void on_actionPasteTransform_triggered();
	void on_actionRotate90CW_triggered();
	void on_actionRotate90CCW_triggered();
	void on_actionRotate180_triggered();
	void on_actionFlipHorizontal_triggered();
	void on_actionFlipVertical_triggered();
	void on_actionFitToScreen_triggered();
	void on_actionStretchToScreen_triggered();
	void on_actionCenterToScreen_triggered();
	void on_actionVerticalCenter_triggered();
	void on_actionHorizontalCenter_triggered();

	void on_sources_customContextMenuRequested(const QPoint &pos);
	void on_actionAddSource_triggered();
	void on_actionRemoveScene_triggered();
	void on_actionRemoveSource_triggered();
	void on_actionInteract_triggered();
	void on_actionSourceProperties_triggered();
	void on_actionSourceUp_triggered();
	void on_actionSourceDown_triggered();

	void on_actionMoveUp_triggered();
	void on_actionMoveDown_triggered();
	void on_actionMoveToTop_triggered();
	void on_actionMoveToBottom_triggered();

	void on_actionLockPreview_triggered();

	void on_scalingMenu_aboutToShow();
	void on_actionScaleWindow_triggered();
	void on_actionScaleCanvas_triggered();
	void on_actionScaleOutput_triggered();

	void on_streamButton_clicked();
	/*add by zhangzhuchao*/
	bool startStreamingCheck();
	bool stopStreamingCheck();
	/*add end */

	/*add by zhangzhuchao*/
	bool startRecordCheck();
	bool stopRecordCheck();
	void on_recordButton_clicked();
	void on_settingsButton_clicked();

	void on_actionPrismFAQ_triggered();
	void on_actionPrismWebsite_triggered();
	void on_actionObsFAQPortal_triggered();

	void on_preview_customContextMenuRequested(const QPoint &pos);
	void on_program_customContextMenuRequested(const QPoint &pos);
	void PreviewDisabledMenu(const QPoint &pos);

	void on_actionNewSceneCollection_triggered();
	void on_actionDupSceneCollection_triggered();
	void on_actionRenameSceneCollection_triggered();
	void on_actionRemoveSceneCollection_triggered();
	void on_actionImportSceneCollection_triggered();
	void on_actionExportSceneCollection_triggered();

	void on_actionNewProfile_triggered();
	void on_actionDupProfile_triggered();
	void on_actionRenameProfile_triggered();
	void on_actionRemoveProfile_triggered();
	void on_actionImportProfile_triggered();
	void on_actionExportProfile_triggered();

	void on_actionShowSettingsFolder_triggered();
	void on_actionShowProfileFolder_triggered();

	void on_actionAlwaysOnTop_triggered();

	void on_toggleListboxToolbars_toggled(bool visible);
	void on_toggleStatusBar_toggled(bool visible);

	void on_autoConfigure_triggered();

	void on_resetUI_triggered();
	void on_lockUI_toggled(bool lock);

	void on_actionStudioMode_triggered();
	void on_actionChat_triggered();

	void PauseToggled();

	void AddSourceFromAction();

	void OnMultiviewShowTriggered(bool checked);
	void OnMultiviewHideTriggered(bool checked);

	void EditSceneName();
	void EditSceneItemName();

	void OpenSceneFilters();

	void EnablePreviewDisplay(bool enable);
	void TogglePreview();

	void NudgeUp();
	void NudgeDown();
	void NudgeLeft();
	void NudgeRight();

	void OpenStudioProgramProjector();
	void OpenPreviewProjector();
	void OpenSourceProjector();
	void OpenMultiviewProjector();
	void OpenSceneProjector();

	void OpenStudioProgramWindow();
	void OpenPreviewWindow();
	void OpenSourceWindow();
	void OpenMultiviewWindow();
	void OpenSceneWindow();

	void DeferredSysTrayLoad(int requeueCount);

	void StackedMixerAreaContextMenuRequested();

	void ResizeOutputSizeOfSource();

	void singletonWakeup();
	void OnDshowSourceStatusChanged(const QString &name, OBSSceneItem item, bool valid);
	void OnSelectItemChanged(OBSSceneItem item, bool selected);
	void OnVisibleItemChanged(OBSSceneItem item, bool visible);
	void OnSourceItemsRemove(QVector<OBSSceneItem> items);
	void OnStickerDownloadCallback(const TaskResponData &responData);

public slots:

	// bgm
	BgmSourceVecType GetCurrentSceneBgmSourceList();
	obs_sceneitem_t *GetCurrentSceneItemBySourceName(const QString &name);
	obs_sceneitem_t *GetSceneItemBySourceName(const QString &name);
	bool GetSelectBgmSourceName(QString &selectSourceName, quint64 &item);

	void on_actionResetTransform_triggered();
	void OnScenesCurrentItemChanged();
	void OnScenesCustomContextMenuRequested(PLSSceneItemView *item);
	void OnScenesItemDoubleClicked();
	void OpenFilters();
	void OpenSourceFilters(OBSSource source);
	void OpenSourceStickers(OBSSource source);
	void OnPropertiesWindowClose();

	bool StreamingActive();
	bool RecordingActive();
	bool ReplayBufferActive();

	void showSettingVideo();
	void OnStickerApply(const QString &fileName, const GiphyData &taskData, const QVariant &extra);
	void OnBeautySourceDownloadFinished();

public:
	explicit PLSBasic(PLSMainView *mainView, PLSDpiHelper dpiHelper = PLSDpiHelper());
	virtual ~PLSBasic();

	virtual bool PLSInit() override;

	virtual config_t *Config() const override;

	virtual int GetProfilePath(char *path, size_t size, const char *file) const override;

	virtual PLSMainView *getMainView() const override;

	static void InitBrowserPanelSafeBlock();

	static QCefCookieManager *getBrowserPannelCookieMgr(const QString &pannelName);

	static void delPannelCookies(const QString &pannelName);

	pls_frontend_callbacks *getApi() { return api; }

signals:
	void propertiesChaned(OBSSource source);
	void mainMenuPopupSubmenu(PLSMenu *submenu);
	void statusBarDataChanged();
	void beautyViewVisibleChanged(bool);
	void bgmViewVisibleChanged(bool);
	void RefreshSourceProperty(const QString &name, bool disable = false);

private:
	std::unique_ptr<Ui::PLSBasic> ui;
	bool m_isPopUpdateView;

	/**
	* Author: WuLongyue
	* Date: 2020-03-28
	* Desc: The stream/recording can't be stoped if choice some `Advanced` settting
	*       So invoke force stop after 5 seconds
	**/
	QTimer m_timerStreamIn5s, m_timerRecordIn5s, m_timerRelayBufferIn5s;
};

class SceneRenameDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit SceneRenameDelegate(QObject *parent);
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;

protected:
	virtual bool eventFilter(QObject *editor, QEvent *event) override;
};
