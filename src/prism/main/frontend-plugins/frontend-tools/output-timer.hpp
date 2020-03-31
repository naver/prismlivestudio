#pragma once

#include <memory>

#include <dialog-view.hpp>

#include "ui_output-timer.h"
#include "PLSDpiHelper.h"

class QCloseEvent;

class OutputTimer : public PLSDialogView {
	Q_OBJECT

public:
	std::unique_ptr<Ui_OutputTimer> ui;
	OutputTimer(QWidget *parent, PLSDpiHelper dpiHelper = PLSDpiHelper());

	void closeEvent(QCloseEvent *event) override;
	void PauseRecordingTimer();
	void UnpauseRecordingTimer();

	void showEvent(QShowEvent *event) override;

public slots:
	void StreamingTimerButton();
	void RecordingTimerButton();
	void StreamTimerStart();
	void RecordTimerStart();
	void StreamTimerStop();
	void RecordTimerStop();
	void UpdateStreamTimerDisplay();
	void UpdateRecordTimerDisplay();
	void ShowHideDialog();
	void EventStopStreaming();
	void EventStopRecording();

	void updateStreamButtonState();
	void updateRecordButtonState();

private:
	void initializeTimers();

private:
	bool streamingAlreadyActive = false;
	bool recordingAlreadyActive = false;

	QTimer *streamingTimer;
	QTimer *recordingTimer;
	QTimer *streamingTimerDisplay;
	QTimer *recordingTimerDisplay;

	int recordingTimeLeft;
};
