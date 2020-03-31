#ifndef PLSTHEMEMANAGER_H
#define PLSTHEMEMANAGER_H

#include "frontend-api-global.h"

#include <QMap>
#include <QObject>
#include <QReadWriteLock>

class FRONTEND_API PLSThemeManager : public QObject {
	Q_OBJECT
private:
	PLSThemeManager();
	~PLSThemeManager() override;

public:
	enum CssIndex {
		Common,
		CommonDialog,
		PLSLoadingBtn,
		PLSMainView,
		PLSDialogView,
		PLSAlertView,
		PLSBasicSettings,
		PLSPopupMenu,
		PLSBasicStats,
		PLSDock,
		PrismAudioMixer,
		QCheckBox,
		QLineEdit,
		QMenu,
		QPlainTextEdit,
		QPushButton,
		QRadioButton,
		QScrollBar,
		QSlider,
		QSpinBox,
		QTableView,
		QTabWidget,
		QTextEdit,
		QToolButton,
		QToolTip,
		QComboBox,
		QFontDialog,
		QColorDialog,
		DecklinkOutputUI,
		OutputTimer,
		SceneSwitcher,
		PLSAboutView,
		PLSBasicAdvAudio,
		PLSBasicProperties,
		PLSBasicSourceSelect,
		PLSChatDialog,
		PLSColorDialogView,
		PLSContactView,
		PLSFontDialogView,
		PLSLiveEndDialog,
		PLSLiveInfoBase,
		PLSLivingMsgView,
		PLSPropertiesView,
		PLSSceneListView,
		PLSSceneTransitionsView,
		PLSUpdateView,
		ScriptsTool,
		PLSOpenSourceView,
		ScriptLogWindow,
		PLSScene,
		PLSSource,
		NameDialog,
		VisibilityCheckBox,
		PLSBasicFilters,
		ChannelCapsule,
		ChannelConfigPannel,
		ChannelsAddWin,
		ChannelsSettingsWidget,
		DefaultPlatformAddList,
		GoLivePannel,
		PLSChannelsArea,
		PLSRTMPChannelView,
		PLSRTMPConfig,
		PLSShareDialog,
		PLSShareSourceItem,
		PLSLiveInfoBand,
		PLSLiveInfoNaverTV,
		PLSLiveInfoTwitch,
		PLSLiveInfoVLive,
		PLSLiveInfoYoutube,
		PLSLiveinfoAfreecaTV,
		PLSLiveInfoFacebook,
		PLSLoadingCombox,
		PLSLoadingComboxMenu,
		PLSScheduleCombox,
		PLSScheLiveNotice,
		PLSSelectImageButton,
		PLSTakeCameraSnapshot,
		PLSCropImage,
		PrismLoginView,
		PrismPasswordView,
		prismTermOfAgreeView,
		ScenesDock,
		SourcesDock,
		MixerDock,
		PLSCompleterPopupList,
		EditableItemDialog,
		PLSQTDisplay,
		PLSNoticeView,
		Beauty,
		PLSFanshipWidget,
		PLSBackgroundMusicView,
		PLSMediaController,
		GiphyStickers,
		PLSBgmLibraryView,
		PLSBgmItemView,
		PLSToastMsgFrame
	};
	Q_ENUM(CssIndex)

public:
	static PLSThemeManager *instance();

public:
	QString loadCss(double dpi, const QList<CssIndex> &cssIndexes);
	QString loadCss(double dpi, CssIndex cssIndex);

	QString preprocessCss(double dpi, const QByteArray &css);
	QString preprocessCss(double dpi, const QString &css);

	void addCachedCss(double dpi, CssIndex cssIndex, const QString &css);
	bool findCachedCss(QString &css, double dpi, CssIndex cssIndex) const;

signals:
	void parentChange(QWidget *widget);

private:
	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	mutable QReadWriteLock cssCacheLock;
	QMap<QPair<CssIndex, int>, QString> cssCache;
};

using PLSCssIndex = PLSThemeManager::CssIndex;

#define PLS_THEME_MANAGER PLSThemeManager::instance()

#endif // PLSTHEMEMANAGER_H
