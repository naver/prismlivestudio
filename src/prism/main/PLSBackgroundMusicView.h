#ifndef PLSBACKGROUNDMUSICVIEW_H
#define PLSBACKGROUNDMUSICVIEW_H

#include "PLSBgmDataManager.h"

#include "PLSDpiHelper.h"
#include "frontend-api/dialog-view.hpp"
#include "loading-event.hpp"
#include "PLSToastMsgFrame.h"
#include "obs.hpp"
#include "window-basic-main.hpp"

#include <QFinalState>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QStateMachine>
#include <QThread>
#include <QQueue>

namespace Ui {
class PLSBackgroundMusicView;
}

class QRadioButton;
class QMediaPlayer;
class PLSBgmItemView;
class PLSBgmItemCoverView;
class PLSBgmLibraryView;

class SendThread : public QObject {
	Q_OBJECT
public:
	SendThread(QObject *parent = nullptr) : QObject(parent) {}
	~SendThread() {}

private:
	void SendMusicMetaData(const PLSBgmItemData &data);

public slots:
	void DoWork(const PLSBgmItemData &data);
};

class CheckValidThread : public QObject {
	Q_OBJECT
public:
	CheckValidThread(QObject *parent = nullptr) : QObject(parent) {}
	~CheckValidThread() {}

private slots:
	void CheckUrlAvailable(const PLSBgmItemData &data);

signals:
	void checkFinished(const PLSBgmItemData &data, bool result);
};

class GetCoverThread : public QObject {
	Q_OBJECT
public:
	GetCoverThread(QObject *parent = nullptr) : QObject(parent) {}
	~GetCoverThread() {}

private slots:
	void SaveCoverToLocalPath(const PLSBgmItemData &data, const QImage &image);
	void GetCoverImage(const PLSBgmItemData &data);
	void NextTask();
signals:
	void Finished(const PLSBgmItemData &data);
	void GetPreviewImage(const QImage &image, const PLSBgmItemData &data);

private:
	QQueue<PLSBgmItemData> taskQueue;
};

class DragLabel : public QLabel {
	Q_OBJECT
public:
	DragLabel(QWidget *parent = nullptr) : QLabel(parent)
	{
		installEventFilter(this);
		setCursor(Qt::ArrowCursor);
	}

protected:
	virtual bool eventFilter(QObject *object, QEvent *event) override;
signals:
	void CoverPressed(const QPoint &point);

private:
	QPoint startPoint{};
	bool mousePressed{false};
};

class DragInFrame : public QFrame {

	Q_OBJECT
public:
	DragInFrame(QWidget *parent = nullptr) : QFrame(parent) { setAcceptDrops(true); }

protected:
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;

signals:
	void AudioFileDraggedIn(const QStringList &paths);
};

class PLSBackgroundMusicView : public PLSDialogView {
	Q_OBJECT

public:
	enum class PlayMode { InOrderMode, RandomMode, UnknownMode };
	Q_ENUM(PlayMode)

	explicit PLSBackgroundMusicView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSBackgroundMusicView();
	void InitGeometry();
	void UpdateSourceList(const QString &sourceName, quint64 sceneItem, const BgmSourceVecType &sourceList);
	void RenameSourceName(const quint64 &item, const QString &newName, const QString &prevName);
	void ClearUrlInfo();
	bool CurrentPlayListBgmDataExisted(const QString &url);
	int GetCurrentPlayListDataSize();
	int GetCurrentBgmSourceSize();
	QVector<PLSBgmItemData> GetPlayListDatas();
	void RefreshPropertyWindow();
	void UpdateSourceSelectUI();
	PLSBgmItemData GetCurrentPlayListDataBySettings(const QString &name);

public slots:
	void UpdateLoadUIState(const QString &name, bool load, bool isOpen);
	void AddSourceAndRefresh(const QString &sourceName, quint64 sceneItem);
	void RemoveSourceAndRefresh(const QString &sourceName, quint64 sceneItem);
	void RemoveBgmSourceList(const BgmSourceVecType &sourceList);
	void InitSourceSettingsData(const QString &sourceName, const quint64 &sceneItem, bool createNew, bool setCurrent = true);
	void SetSourceSelect(const QString &sourceName, quint64 sceneItem, bool selected);
	void SetSourceVisible(const QString &sourceName, quint64 sceneItem, bool visible);
	void OnSceneChanged();
	void UpdateLoadingStartState(const QString &sourceName);
	void UpdateLoadingEndState(const QString &sourceName);

	void OnNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);

protected:
	virtual void closeEvent(QCloseEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;
	virtual void hideEvent(QHideEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual bool eventFilter(QObject *watcher, QEvent *event) override;

private slots:
	void OnDelButtonClicked(const PLSBgmItemData &data);
	void OnNoNetwork(const QString &toast, const PLSBgmItemData &data);
	void OnInvalidSongs(const QString &name);
	void OnRetryNetwork();
	void OnPlayListItemDoublePressed(const QModelIndex &index);
	void OnPlayListItemRowChanged(const int &srcIndex, const int &destIndex);
	void OnAudioFileDraggedIn(const QStringList &paths);
	void OnAddCachePlayList(const QVector<PLSBgmItemData> &datas);

	void OnPlayButtonClicked();
	void OnPreButtonClicked();
	void OnNextButtonClicked();
	void OnLoopBtnClicked(const quint64 &sceneItem, bool checked);
	void OnLocalFileBtnClicked();
	void OnLibraryBtnClicked();
	void OnAddSourceBtnClicked();
	void OnAddMusicBtnClicked();
	void OnLoadFailed(const QString &name);

	void OnCurrentPageChanged(int index);
	// slider state
	void SliderClicked(int val);
	void SliderReleased(int val);
	void SliderMoved(int val);
	void SetSliderPos();

	void UpdateUIBySourceSettings(obs_data_t *settings, OBSSource source, const quint64 &sceneItem, bool createNew);

	// media callback
	void OnMediaStateChanged(const QString &name, obs_media_state state);
	void OnPropertiesChanged(const QString &name);

	void UpdatePlayingUIState(const QString &name);
	void UpdatePauseUIState(const QString &name);
	void UpdateStopUIState(const QString &name);
	void UpdateOpeningUIState(const QString &name);
	void UpdateErrorUIState(const QString &name, bool gotoNextSongs);
	void OnUpdateOpeningUIState(const QString &name);

private:
	void initUI();
	void clearUI(quint64 sceneItem);
	void Save();
	void SetLoop(const quint64 &sceneItem, bool checked);

	/* create thread */
	void CreateCheckValidThread();

	void initStateMachine();
	QState *GetStateMachine(const PlayMode &mode);
	void AddSource(const QString &sourceName, quint64 sceneItem, bool createNew = true);
	void RemoveSource(const QString &sourceName, quint64 sceneItem);
	obs_source_t *GetSource(const quint64 &sceneItem);
	bool SameWithCurrentSource(const QString &sourceName);
	bool SameWithCurrentSource(const quint64 &sceneItem);
	QVector<PLSBgmItemData> GetPlayListData(obs_data_t *settings);
	PLSBgmItemData GetPlayListDataBySettings(obs_data_t *settings);

	PLSBgmItemData GetCurrentPlayListData();
	PLSBgmItemData GetCurrentPlayListDataBySettings();

	void UpdatePlayListUI(const quint64 &sceneItem);
	void AddPlayListUI(const quint64 &sceneItem);
	void SetPlayerControllerStatus(const quint64 &sceneItem, bool listChanged = false);
	void ResetControlView();
	void DisablePlayerControlUI(bool disable);
	void SaveShowModeToConfig();
	void onMaxFullScreenStateChanged() override;
	void onSaveNormalGeometry() override;
	void SetCurrentPlayMode(const int &mode);
	static void PLSFrontendEvent(enum obs_frontend_event event, void *ptr);

	int GetPlayListItemIndexByKey(const QString &key);

	void SetUrlInfo(OBSSource source, PLSBgmItemData data);
	void SetUrlInfoToSettings(obs_data_t *settings, const PLSBgmItemData &data);

	QString StrcatString(const QString &title, const QString &producer) const;
	bool IsSourceAvailable(const QString &sourceName, quint64 sceneItem);

	void StartSliderPlayingTimer();
	void StopSliderPlayingTimer();

	void StartLoadingTimer(int timeOutMs = 200);
	void StopLoadingTimer();

	bool CheckNetwork();
	bool CheckValidLocalAudioFile(const QString &url);
	void RefreshMulicEnabledPlayList(const quint64 &sceneItem, const QVector<PLSBgmItemData> &datas);
	void UpdateCurrentPlayStatus(const QString &sourceName);
	void SetPlayListStatus(const PLSBgmItemData &data);
	void SetPlayListItemStatus(const int &index, const PLSBgmItemData &data);
	void SetPlayerPaneEnabled(bool enabled);

	// library
	void CreateLibraryView();

	// toast
	void InitToast();
	void ShowToastView(const QString &text);
	void ResizeToastView();

	QImage GetCoverImage(const QString &url);
	void SetCoverImage(const PLSBgmItemData &data);
	void ShowCoverImage(const PLSBgmItemData &data);
	void ShowCoverGif(bool show);
	void UpdateBgmCoverPath(const PLSBgmItemData &data);

	void SeekTo(int val);
signals:
	void bgmViewVisibleChanged(bool isVisible);
	void RefreshSourceProperty(const QString &sourceName, bool disable = false);
	void PlayModeChanged(const PlayMode &mode, bool ignoreUpdate = false);

private:
	Ui::PLSBackgroundMusicView *ui;
	QString currentSourceName{};
	quint64 currentSceneItem{};

	QStateMachine machine;
	QState *inOrderState{};
	QState *randomState{};
	PlayMode mode = PlayMode::UnknownMode;
	QNetworkAccessManager *manager{nullptr};
	PLSBgmLibraryView *libraryView{nullptr};

	QTimer *sliderTimer{};
	PLSLoadingEvent loadingEvent;

	bool networkAvailable{true};
	QString localFilePath{};
	bool isLoading{false};
	QTimer timerLoading;
	int indexLoading{-1};
	uint64_t loadingTimeout{0};

	PLSToastMsgFrame toastView;
	QThread sendThread;
	QThread coverThread;
	QThread checkThread;

	SendThread *sendThreadObj{};
	GetCoverThread *coverThreadObj{};
	CheckValidThread *checkThreadObj{};

	bool isDragging{false};
};

#endif // PLSBACKGROUNDMUSICVIEW_H
