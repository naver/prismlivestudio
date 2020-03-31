#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "PLSDpiHelper.h"

class PLSTimerDisplay : public QWidget {
	Q_OBJECT
public:
	enum TimerType {
		TimerLive = 0,
		TimerRecord,
		TimerRehearsal,
	};
	explicit PLSTimerDisplay(TimerType type, QWidget *parent);

	void SetTimerType(TimerType type);
	void OnStatus(bool isStarted);

protected:
	void timerEvent(QTimerEvent *e);

	QString FormatTimeString(uint sec);
	void UpdateProperty(bool isStarted);

private:
	bool started;
	uint startTime; // UNIX time in seconds
	int timerID;
	QLabel *title;
	QLabel *time;
};

class PLSBasic;
class PLSPreviewTitle : public QWidget {
	Q_OBJECT

	friend class PLSBasic;

public:
	explicit PLSPreviewTitle(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());

	void OnLiveStatus(bool isStarted);
	void OnRecordStatus(bool isStarted);
	void OnStudioModeStatus(bool isStudioMode);

private:
	QWidget *leftContainer;
	QLabel *labelLeftEdit;
	QPushButton *transApply;
	PLSTimerDisplay *liveUI;
	QLabel *spaceIcon;
	PLSTimerDisplay *recordUI;
};
