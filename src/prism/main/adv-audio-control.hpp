#pragma once

#include <obs.hpp>
#include <QWidget>
#include <QPointer>
#include <QDoubleSpinBox>
#include "balance-slider.hpp"

#define NAME_LABEL_WIDTH 130
#define MONITOR_LABEL_SPACE 20
#define MONITOR_LABEL_LEFT_SPACE 3
#define MONITOR_TYPE_WIDTH 200
#define VOLUME_SPINBOX_SPACE 10
#define VOLUME_SPINBOX_WIDTH 110
#define BALANCE_SPACE 25
#define BALANCE_WIDTH 159
#define MONO_LEFT_SPACE 10
#define MONO_TITLE_WIDTH 95
#define MONO_CONTAINER_WIDTH 120
#define SYNC_OFFSET_SPACE 15
#define TRACKS_SPACE 25
#define TRACKS_MAX_CONTAINER_WIDTH 268
#define TRACKS_MIN_CONTAINER_WIDTH 258

class QGridLayout;
class QLabel;
class QSpinBox;
class QCheckBox;
class QComboBox;

class PLSAdvAudioCtrl : public QObject {
	Q_OBJECT

private:
	OBSSource source;

	QPointer<QWidget> forceMonoContainer;
	QPointer<QWidget> mixerContainer;
	QPointer<QWidget> balanceContainer;
	QPointer<QLabel> nameLabel;
	QPointer<QLabel> monitoringTypeSpacer;
	QPointer<QLabel> volumeSpacer;
	QPointer<QLabel> balanceSpacer;
	QPointer<QLabel> trackSpacer;
	QPointer<QDoubleSpinBox> volume;
	QPointer<QCheckBox> forceMono;
	QPointer<BalanceSlider> balance;
	QPointer<QLabel> labelL;
	QPointer<QLabel> labelR;
	QPointer<QSpinBox> syncOffset;
	QPointer<QComboBox> monitoringType;
	QPointer<QCheckBox> mixer1;
	QPointer<QCheckBox> mixer2;
	QPointer<QCheckBox> mixer3;
	QPointer<QCheckBox> mixer4;
	QPointer<QCheckBox> mixer5;
	QPointer<QCheckBox> mixer6;

	OBSSignal volChangedSignal;
	OBSSignal syncOffsetSignal;
	OBSSignal flagsSignal;
	OBSSignal mixersSignal;

	static void OBSSourceFlagsChanged(void *param, calldata_t *calldata);
	static void OBSSourceVolumeChanged(void *param, calldata_t *calldata);
	static void OBSSourceSyncChanged(void *param, calldata_t *calldata);
	static void OBSSourceMixersChanged(void *param, calldata_t *calldata);
	static void monitorControlChange(pls_frontend_event event, const QVariantList &params, void *context);

public:
	explicit PLSAdvAudioCtrl(QWidget *parent, obs_source_t *source_);
	virtual ~PLSAdvAudioCtrl();

	inline obs_source_t *GetSource() const { return source; }
	void ShowAudioControl(QGridLayout *layout);
	void setSourceName();
public slots:
	void SourceFlagsChanged(uint32_t flags);
	void SourceVolumeChanged(float volume);
	void SourceSyncChanged(int64_t offset);
	void SourceMixersChanged(uint32_t mixers);

	void volumeChanged(double db);
	void downmixMonoChanged(bool checked);
	void balanceChanged(int val);
	void syncOffsetChanged(int milliseconds);
	void monitoringTypeChanged(int index);
	void mixer1Changed(bool checked);
	void mixer2Changed(bool checked);
	void mixer3Changed(bool checked);
	void mixer4Changed(bool checked);
	void mixer5Changed(bool checked);
	void mixer6Changed(bool checked);
	void ResetBalance();
};
