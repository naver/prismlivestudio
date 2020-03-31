#include "PLSPreviewTitle.h"
#include "window-basic-main.hpp"
#include "pls-common-define.hpp"
#include "PLSChannelDataAPI.h"

#include <QHBoxLayout>

#define TRANSITION_APPLY_BUTTON_SIZE 34

#define TIMER_REFRESH_INTERVAL 500 // in milliseconds

#define PREVIEW_TITLE_LABEL "previewTitleLabel"
#define PREVIEW_LIVEREC_TIME "previewLiveRecTime"
#define PREVIEW_LIVEREC_SPACEICON "previewLiveRecSpace"

#define PREVIEW_TITLE_STATUS "status"
#define PREVIEW_TITLE_STATUS_START "true"
#define PREVIEW_TITLE_STATUS_END "false"

#define PREVIEW_TITLE_TIMERTYPE "timerType"

PLSTimerDisplay::PLSTimerDisplay(TimerType type, QWidget *parent) : QWidget(parent), started(false), timerID(INTERNAL_ERROR), startTime(0)
{
	title = new QLabel(this);
	title->setObjectName(PREVIEW_TITLE_LABEL);
	SetTimerType(type);

	time = new QLabel(this);
	time->setText("00:00:00");
	time->setObjectName(PREVIEW_LIVEREC_TIME);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(5);
	layout->addWidget(title);
	layout->addWidget(time);

	OnStatus(false);
}

void PLSTimerDisplay::SetTimerType(TimerType type)
{
	switch (type) {
	case PLSTimerDisplay::TimerRecord:
		title->setText("REC");
		title->setProperty(PREVIEW_TITLE_TIMERTYPE, "TimerRecord");
		break;
	case PLSTimerDisplay::TimerRehearsal:
		title->setText("Rehearsal");
		title->setProperty(PREVIEW_TITLE_TIMERTYPE, "TimerRehearsal");
		break;
	case PLSTimerDisplay::TimerLive:
	default:
		title->setText("LIVE");
		title->setProperty(PREVIEW_TITLE_TIMERTYPE, "TimerLive");
		break;
	}
}

void PLSTimerDisplay::OnStatus(bool isStarted)
{
	UpdateProperty(isStarted);

	started = isStarted;

	time->setText("00:00:00");
	startTime = QDateTime::currentDateTime().toTime_t();

	if (isStarted) {
		timerID = this->startTimer(TIMER_REFRESH_INTERVAL);
	} else {
		if (INTERNAL_ERROR != timerID) {
			killTimer(timerID);
			timerID = INTERNAL_ERROR;
		}
	}
}

void PLSTimerDisplay::timerEvent(QTimerEvent *e)
{
	if (e->timerId() == timerID) {
		uint secs = QDateTime::currentDateTime().toTime_t() - startTime;
		time->setText(FormatTimeString(secs));
	}
}

QString PLSTimerDisplay::FormatTimeString(uint sec)
{
	int hours = sec / 3600;
	int minutes = (sec % 3600) / 60;
	int seconds = (sec % 60);

	QString strH, strM, strS;
	strM.sprintf("%02d", minutes);
	strS.sprintf("%02d", seconds);
	if (hours > 99) {
		strH.sprintf("%d", hours);
	} else {
		strH.sprintf("%02d", hours);
	}

	QString ret = strH + QString(":") + strM + QString(":") + strS;
	return ret;
}

void PLSTimerDisplay::UpdateProperty(bool isStarted)
{
	title->setProperty(PREVIEW_TITLE_STATUS, isStarted ? PREVIEW_TITLE_STATUS_START : PREVIEW_TITLE_STATUS_END);
	title->style()->unpolish(title);
	title->style()->polish(title);
}

//-------------------------------------------------------------------
PLSPreviewTitle::PLSPreviewTitle(QWidget *parent, PLSDpiHelper dpiHelper) : QWidget(parent)
{
	dpiHelper.setFixedHeight(this, 40);
	//setFixedHeight(40);

	//---------------------------------------------------
	leftContainer = new QWidget(this);
	leftContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	labelLeftEdit = new QLabel(leftContainer);
	labelLeftEdit->setObjectName(PREVIEW_TITLE_LABEL);
	labelLeftEdit->setText("EDIT");

	QHBoxLayout *leftLayout = new QHBoxLayout(leftContainer);
	leftLayout->setContentsMargins(0, 0, 0, 0);
	leftLayout->setSpacing(0);
	leftLayout->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Maximum));
	leftLayout->addWidget(labelLeftEdit);
	leftLayout->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Maximum));

	//---------------------------------------------------
	QWidget *rightContainer = new QWidget(this);
	rightContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	liveUI = new PLSTimerDisplay(PLSTimerDisplay::TimerLive, rightContainer);
	recordUI = new PLSTimerDisplay(PLSTimerDisplay::TimerRecord, rightContainer);

	spaceIcon = new QLabel(rightContainer);
	spaceIcon->setObjectName(PREVIEW_LIVEREC_SPACEICON);

	QHBoxLayout *rightLayout = new QHBoxLayout(rightContainer);
	rightLayout->setContentsMargins(0, 0, 0, 0);
	rightLayout->setSpacing(5);
	rightLayout->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Maximum));
	rightLayout->addWidget(liveUI);
	rightLayout->addWidget(spaceIcon);
	rightLayout->addWidget(recordUI);
	rightLayout->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Maximum));

	//---------------------------------------------------
	transApply = new QPushButton(this);
	transApply->setObjectName(TRANSITION_APPLY_BUTTON);
	transApply->setToolTip(QTStr("StudioMode.Apply.Tooltips"));

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addSpacerItem(new QSpacerItem(TRANSITION_APPLY_BUTTON_SIZE / 2, 1, QSizePolicy::Fixed, QSizePolicy::Fixed));
	layout->addWidget(leftContainer);
	layout->addWidget(transApply);
	layout->addWidget(rightContainer);
	layout->addSpacerItem(new QSpacerItem(TRANSITION_APPLY_BUTTON_SIZE / 2, 1, QSizePolicy::Fixed, QSizePolicy::Fixed));

	OnLiveStatus(false);
	OnRecordStatus(false);
}

void PLSPreviewTitle::OnLiveStatus(bool isStarted)
{
	liveUI->SetTimerType(isStarted && PLSCHANNELS_API->isRehearsaling() ? PLSTimerDisplay::TimerRehearsal : PLSTimerDisplay::TimerLive);
	liveUI->OnStatus(isStarted);
}

void PLSPreviewTitle::OnRecordStatus(bool isStarted)
{
	recordUI->OnStatus(isStarted);

	if (isStarted) {
		spaceIcon->show();
		recordUI->show();
	} else {
		spaceIcon->hide();
		recordUI->hide();
	}
}

void PLSPreviewTitle::OnStudioModeStatus(bool isStudioMode)
{
	if (isStudioMode) {
		leftContainer->show();
		transApply->show();
	} else {
		leftContainer->hide();
		transApply->hide();
	}
}
