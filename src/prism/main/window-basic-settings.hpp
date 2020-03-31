/******************************************************************************
    Copyright (C) 2013 by Hugh Bailey <obs.jim@gmail.com>
                          Philippe Groarke <philippe.groarke@gmail.com>

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

#include <util/util.hpp>
#include <QPointer>
#include <memory>
#include <string>

#include <libff/ff-util.h>

#include <obs.hpp>

#include "auth-base.hpp"
#include "dialog-view.hpp"
#include "spinbox.hpp"

class PLSBasic;
class QAbstractButton;
class QComboBox;
class QCheckBox;
class QLabel;
class PLSPropertiesView;
class PLSHotkeyWidget;

#include "ui_PLSBasicSettings.h"

#define VOLUME_METER_DECAY_FAST 23.53
#define VOLUME_METER_DECAY_MEDIUM 11.76
#define VOLUME_METER_DECAY_SLOW 8.57

class SilentUpdateCheckBox : public QCheckBox {
	Q_OBJECT

public slots:
	void setCheckedSilently(bool checked)
	{
		bool blocked = blockSignals(true);
		setChecked(checked);
		blockSignals(blocked);
	}
};

class SilentUpdateSpinBox : public PLSSpinBox {
	Q_OBJECT

public slots:
	void setValueSilently(int val)
	{
		bool blocked = blockSignals(true);
		setValue(val);
		blockSignals(blocked);
	}
};

class PLSFFDeleter {
public:
	void operator()(const ff_format_desc *format) { ff_format_desc_free(format); }
	void operator()(const ff_codec_desc *codec) { ff_codec_desc_free(codec); }
};
using PLSFFCodecDesc = std::unique_ptr<const ff_codec_desc, PLSFFDeleter>;
using PLSFFFormatDesc = std::unique_ptr<const ff_format_desc, PLSFFDeleter>;

class PLSBasicSettings : public PLSDialogView {
	Q_OBJECT
	Q_PROPERTY(QIcon generalIcon READ GetGeneralIcon WRITE SetGeneralIcon DESIGNABLE true)
	Q_PROPERTY(QIcon streamIcon READ GetStreamIcon WRITE SetStreamIcon DESIGNABLE true)
	Q_PROPERTY(QIcon outputIcon READ GetOutputIcon WRITE SetOutputIcon DESIGNABLE true)
	Q_PROPERTY(QIcon audioIcon READ GetAudioIcon WRITE SetAudioIcon DESIGNABLE true)
	Q_PROPERTY(QIcon videoIcon READ GetVideoIcon WRITE SetVideoIcon DESIGNABLE true)
	Q_PROPERTY(QIcon hotkeysIcon READ GetHotkeysIcon WRITE SetHotkeysIcon DESIGNABLE true)
	Q_PROPERTY(QIcon advancedIcon READ GetAdvancedIcon WRITE SetAdvancedIcon DESIGNABLE true)

private:
	int m_doneValue;
	PLSBasic *main;
	bool m_isShowChangeLanguageMsg;
	int m_currentLanguageIndex;
	std::unique_ptr<Ui::PLSBasicSettings> ui;

	std::shared_ptr<Auth> auth;

	bool generalChanged = false;
	bool stream1Changed = false;
	bool outputsChanged = false;
	bool audioChanged = false;
	bool videoChanged = false;
	bool hotkeysChanged = false;
	bool advancedChanged = false;
	int pageIndex = 0;
	bool loading = true;
	bool forceAuthReload = false;
	std::string savedTheme;

	int lastSimpleRecQualityIdx = 0;
	int lastChannelSetupIdx = 0;

	PLSFFFormatDesc formats;

	PLSPropertiesView *streamProperties = nullptr;
	PLSPropertiesView *streamEncoderProps = nullptr;
	PLSPropertiesView *recordEncoderProps = nullptr;

	QString curPreset;
	QString curQSVPreset;
	QString curNVENCPreset;
	QString curAMDPreset;

	QString curAdvStreamEncoder;
	QString curAdvRecordEncoder;

	using AudioSource_t = std::tuple<OBSWeakSource, QPointer<QCheckBox>, QPointer<QSpinBox>, QPointer<QCheckBox>, QPointer<QSpinBox>>;
	std::vector<AudioSource_t> audioSources;
	std::vector<OBSSignal> audioSourceSignals;
	OBSSignal sourceCreated;
	OBSSignal channelChanged;

	std::vector<std::pair<bool, QPointer<PLSHotkeyWidget>>> hotkeys;
	OBSSignal hotkeyRegistered;
	OBSSignal hotkeyUnregistered;

	uint32_t outputCX = 0;
	uint32_t outputCY = 0;

	QWidget *outputSettingsAdvTabsHLine;
	QLabel *hotkeyFilterLabel = nullptr;
	PLSHotkeyWidget *replayBufferHotkeyWidget = nullptr;

	QList<std::tuple<QWidget *, QLabel *>> errorAlertMessages;
	QList<std::tuple<QWidget *, QLabel *>> warningAlertMessages;

	QWidget *outputSettingsAdvCurrentTab = nullptr;

	QList<std::tuple<bool, QLabel *, QWidget *, QWidget *>> hotkeyRows;

	void SaveCombo(QComboBox *widget, const char *section, const char *value);
	void SaveComboData(QComboBox *widget, const char *section, const char *value);
	void SaveCheckBox(QAbstractButton *widget, const char *section, const char *value, bool invert = false);
	void SaveEdit(QLineEdit *widget, const char *section, const char *value);
	void SaveSpinBox(QSpinBox *widget, const char *section, const char *value);
	void SaveFormat(QComboBox *combo);
	void SaveEncoder(QComboBox *combo, const char *section, const char *value);

	inline bool Changed() const { return generalChanged || outputsChanged || stream1Changed || audioChanged || videoChanged || advancedChanged || hotkeysChanged; }

	inline void EnableApplyButton(bool en) { ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(en); }

	inline void ClearChanged()
	{
		generalChanged = false;
		stream1Changed = false;
		outputsChanged = false;
		audioChanged = false;
		videoChanged = false;
		hotkeysChanged = false;
		advancedChanged = false;
		EnableApplyButton(false);
	}

#ifdef _WIN32
	bool aeroWasDisabled = false;
	QCheckBox *toggleAero = nullptr;
	void ToggleDisableAero(bool checked);
#endif

	void HookWidget(QWidget *widget, const char *signal, const char *slot);

	bool QueryChanges();

	void LoadEncoderTypes();
	void LoadColorRanges();
	void LoadFormats();
	void ReloadCodecs(const ff_format_desc *formatDesc);

	void LoadGeneralSettings();
	void LoadStream1Settings();
	void LoadOutputSettings();
	void LoadAudioSettings();
	void LoadVideoSettings();
	void LoadHotkeySettings(obs_hotkey_id ignoreKey = OBS_INVALID_HOTKEY_ID);
	void LoadAdvancedSettings();
	void LoadSettings(bool changedOnly);

	void CreateEncoderPropertyView(PLSPropertiesView *&view, QWidget *parent, const char *encoder, const char *path, bool changed = false);

	/* general */
	void LoadLanguageList();
	void LoadThemeList();
	/* general */
	void initGeneralView();
	/* stream */
	void InitStreamPage();
	inline bool IsCustomService() const;
	void LoadServices(bool showAll);
	void OnOAuthStreamKeyConnected();
	void OnAuthConnected();
	QString lastService;
private slots:
	void UpdateServerList();
	void UpdateKeyLink();
	void on_show_clicked();
	void on_authPwShow_clicked();
	void on_connectAccount_clicked();
	void on_disconnectAccount_clicked();
	void on_useStreamKey_clicked();
	void on_useAuth_toggled();

private:
	/* output */
	void LoadSimpleOutputSettings();
	void LoadAdvOutputStreamingSettings();
	void LoadAdvOutputStreamingEncoderProperties();
	void LoadAdvOutputRecordingSettings();
	void LoadAdvOutputRecordingEncoderProperties();
	void LoadAdvOutputFFmpegSettings();
	void LoadAdvOutputAudioSettings();
	void SetAdvOutputFFmpegEnablement(ff_codec_type encoderType, bool enabled, bool enableEncode = false);

	/* audio */
	void LoadListValues(QComboBox *widget, obs_property_t *prop, int index);
	void LoadAudioDevices();
	void LoadAudioSources();

	/* video */
	void LoadRendererList();
	void ResetDownscales(uint32_t cx, uint32_t cy);
	void LoadDownscaleFilters();
	void LoadResolutionLists();
	void LoadFPSData();

	void SaveGeneralSettings();
	void SaveStream1Settings();
	void SaveOutputSettings();
	void SaveAudioSettings();
	void SaveVideoSettings();
	void SaveHotkeySettings();
	void SaveAdvancedSettings();
	void SaveSettings();

	void ResetSettings();

	void UpdateSimpleOutStreamDelayEstimate();
	void UpdateAdvOutStreamDelayEstimate();

	void FillSimpleRecordingValues();
	void FillSimpleStreamingValues();
	void FillAudioMonitoringDevices();

	void RecalcOutputResPixels(const char *resText);
	void updateLabelSize(double dpi);

	QIcon generalIcon;
	QIcon streamIcon;
	QIcon outputIcon;
	QIcon audioIcon;
	QIcon videoIcon;
	QIcon hotkeysIcon;
	QIcon advancedIcon;

	QIcon GetGeneralIcon() const;
	QIcon GetStreamIcon() const;
	QIcon GetOutputIcon() const;
	QIcon GetAudioIcon() const;
	QIcon GetVideoIcon() const;
	QIcon GetHotkeysIcon() const;
	QIcon GetAdvancedIcon() const;

	int CurrentFLVTrack();

	QWidget *getPageOfSender(QObject *sender = nullptr) const;

private slots:
	void on_theme_activated(int idx);

	void on_listWidget_itemSelectionChanged();
	void on_buttonBox_clicked(QAbstractButton *button);
	void on_service_currentIndexChanged(int idx);
	void on_simpleOutputBrowse_clicked();
	void on_advOutRecPathBrowse_clicked();
	void on_advOutFFPathBrowse_clicked();
	void on_advOutEncoder_currentIndexChanged(int idx);
	void on_advOutRecEncoder_currentIndexChanged(int idx);
	void on_advOutFFIgnoreCompat_stateChanged(int state);
	void on_advOutFFFormat_currentIndexChanged(int idx);
	void on_advOutFFAEncoder_currentIndexChanged(int idx);
	void on_advOutFFVEncoder_currentIndexChanged(int idx);
	void on_advOutFFType_currentIndexChanged(int idx);

	void on_colorFormat_currentIndexChanged(const QString &text);

	void on_filenameFormatting_textEdited(const QString &text);
	void on_outputResolution_editTextChanged(const QString &text);
	void on_outputResolution_2_editTextChanged(const QString &text);
	void on_baseResolution_editTextChanged(const QString &text);

	void on_disableOSXVSync_clicked();

	void on_resetButton_clicked();

	void GeneralChanged();
	void AudioChanged();
	void AudioChangedRestart();
	void ReloadAudioSources();
	void SurroundWarning(int idx);
	void SpeakerLayoutChanged(int idx);
	void OutputsChanged();
	void Stream1Changed();
	void VideoChanged();
	void VideoChangedResolution();
	void VideoChangedRestart();
	void HotkeysChanged();
	void ReloadHotkeys(obs_hotkey_id ignoreKey = OBS_INVALID_HOTKEY_ID);
	void AdvancedChanged();
	void AdvancedChangedRestart();

	void UpdateStreamDelayEstimate();

	void UpdateAutomaticReplayBufferCheckboxes();

	void AdvOutRecCheckWarnings();

	void SimpleRecordingQualityChanged();
	void SimpleRecordingEncoderChanged();
	void SimpleRecordingQualityLosslessWarning(int idx);

	void SimpleReplayBufferChanged();
	void AdvReplayBufferChanged();

	void SimpleStreamingEncoderChanged();

	OBSService SpawnTempService();

	void SetGeneralIcon(const QIcon &icon);
	void SetStreamIcon(const QIcon &icon);
	void SetOutputIcon(const QIcon &icon);
	void SetAudioIcon(const QIcon &icon);
	void SetVideoIcon(const QIcon &icon);
	void SetHotkeysIcon(const QIcon &icon);
	void SetAdvancedIcon(const QIcon &icon);
	void onAsyncUpdateReplayBufferHotkeyMessage(void *output, PLSHotkeyWidget *hw);

	bool onCloseEvent(QCloseEvent *event);
	void hotkeysClearButtonClicked(QPushButton *button);

protected:
	virtual bool event(QEvent *event);
	virtual bool eventFilter(QObject *watched, QEvent *event);

public:
	explicit PLSBasicSettings(QWidget *parent, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSBasicSettings();
	int returnValue() { return m_doneValue; }

public:
	void switchTo(const QString &tab, const QString &group);

	enum class AlertMessageType { Error, Warning };
	void updateAlertMessage(AlertMessageType type, QWidget *widget, const QString &message);
	void clearAlertMessage(AlertMessageType type, QWidget *widget, bool update = true);
	void updateAlertMessage();
	virtual void done(int type);

signals:
	void updateStreamEncoderPropsSize(PLSPropertiesView *view);
	void asyncUpdateReplayBufferHotkeyMessage(void *output, PLSHotkeyWidget *hw);
	void asyncNotifyComponentValueChanged(QWidget *page, QObject *sender);
};
