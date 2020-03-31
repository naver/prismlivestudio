#include "frontend-api/frontend-api.h"

#include "window-basic-stats.hpp"
#include "window-basic-main.hpp"
#include "platform.hpp"
#include "pls-app.hpp"
#include "qt-wrappers.hpp"

#include <QDesktopWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <string>

#define TIMER_INTERVAL 2000
#define REC_TIME_LEFT_INTERVAL 30000

extern double getProcessCurrentCPUUsage();

PLSBasicStats::PLSBasicStats(PLSDialogView *dialogView, QWidget *parent, PLSDpiHelper dpiHelper) : QWidget(parent), ui(new Ui::PLSBasicStats), cpu_info(os_cpu_usage_info_start()), timer(this)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSBasicStats});

	this->dialogView = dialogView;
	if (dialogView) {
		dialogView->installEventFilter(this);
	}

	ui->setupUi(this);

	outputLabels.push_back(OutputLabels());
	outputLabels.push_back(OutputLabels());

	QObject::connect(&timer, &QTimer::timeout, this, &PLSBasicStats::Update);
	timer.setInterval(TIMER_INTERVAL);

	Update();
}

PLSBasicStats::~PLSBasicStats()
{
	delete shortcutFilter;
	os_cpu_usage_info_destroy(cpu_info);
}

static uint32_t first_encoded = 0xFFFFFFFF;
static uint32_t first_skipped = 0xFFFFFFFF;
static uint32_t first_rendered = 0xFFFFFFFF;
static uint32_t first_lagged = 0xFFFFFFFF;

void PLSBasicStats::InitializeValues()
{
	video_t *video = obs_get_video();
	first_encoded = video_output_get_total_frames(video);
	first_skipped = video_output_get_skipped_frames(video);
	first_rendered = obs_get_total_frames();
	first_lagged = obs_get_lagged_frames();
}

void PLSBasicStats::Update()
{
	PLSBasic *main = PLSBasic::Get();

	OBSOutput streamingOutput = obs_frontend_get_streaming_output();
	OBSOutput recordingOutput = obs_frontend_get_recording_output();
	obs_output_release(streamingOutput);
	obs_output_release(recordingOutput);

	if (!streamingOutput && !recordingOutput)
		return;

	QString str;

	double usage = getProcessCurrentCPUUsage(); // os_cpu_usage_info_query(cpu_info);
	str = QString::number(usage, 'f', 1) + QStringLiteral("%");
	ui->cpuUsage->setText(str);

	const char *path = main->GetCurrentOutputPath();

#define MBYTE (1024ULL * 1024ULL)
#define GBYTE (1024ULL * 1024ULL * 1024ULL)
#define TBYTE (1024ULL * 1024ULL * 1024ULL * 1024ULL)
	num_bytes = os_get_free_disk_space(path);
	QString abrv = QStringLiteral(" MB");
	long double num;

	num = (long double)num_bytes / (1024.0l * 1024.0l);
	if (num_bytes > TBYTE) {
		num /= 1024.0l * 1024.0l;
		abrv = QStringLiteral(" TB");
	} else if (num_bytes > GBYTE) {
		num /= 1024.0l;
		abrv = QStringLiteral(" GB");
	}

	str = QString::number(num, 'f', 1) + abrv;
	ui->diskAvailable->setText(str);

	// if (num_bytes < GBYTE)
	// 	setThemeID(ui->diskAvailable, "error");
	// else if (num_bytes < (5 * GBYTE))
	// 	setThemeID(ui->diskAvailable, "warning");
	// else
	// 	setThemeID(ui->diskAvailable, "");

	num = (long double)os_get_proc_resident_size() / (1024.0l * 1024.0l);

	str = QString::number(num, 'f', 1) + QStringLiteral(" MB");
	ui->memoryUsage->setText(str);

	video_t *video = obs_get_video();
	uint32_t total_encoded = video_output_get_total_frames(video);
	uint32_t total_skipped = video_output_get_skipped_frames(video);

	if (total_encoded < first_encoded || total_skipped < first_skipped) {
		first_encoded = total_encoded;
		first_skipped = total_skipped;
	}
	total_encoded -= first_encoded;
	total_skipped -= first_skipped;

	num = total_encoded ? (long double)total_skipped / (long double)total_encoded : 0.0l;
	num *= 100.0l;

	str = QString("%1 / %2 (%3%)").arg(QString::number(total_skipped), QString::number(total_encoded), QString::number(num, 'f', 1));
	ui->encodingFramedrop->setText(str);

	if (num > 5.0l)
		pls_flush_style(ui->encodingFramedropState, "state", "error");
	else if (num > 1.0l)
		pls_flush_style(ui->encodingFramedropState, "state", "warning");
	else
		pls_flush_style(ui->encodingFramedropState, "state", "");

	// rendering frame drop
	uint32_t total_rendered = obs_get_total_frames();
	uint32_t total_lagged = obs_get_lagged_frames();

	if (total_rendered < first_rendered || total_lagged < first_lagged) {
		first_rendered = total_rendered;
		first_lagged = total_lagged;
	}
	total_rendered -= first_rendered;
	total_lagged -= first_lagged;

	num = total_rendered ? (long double)total_lagged / (long double)total_rendered : 0.0l;
	num *= 100.0l;

	str = QString("%1 / %2 (%3%)").arg(QString::number(total_lagged), QString::number(total_rendered), QString::number(num, 'f', 1));
	ui->renderingFramedrop->setText(str);

	if (num > 5.0l)
		pls_flush_style(ui->renderingFramedropState, "state", "error");
	else if (num > 1.0l)
		pls_flush_style(ui->renderingFramedropState, "state", "warning");
	else
		pls_flush_style(ui->renderingFramedropState, "state", "");

	/* recording/streaming stats                   */
	outputLabels[0].Update(ui->networkFramedropState, ui->networkFramedropLabel, ui->networkFramedrop, ui->streamBitrateLabel, ui->streamBitrate, streamingOutput, false);
	outputLabels[1].Update(nullptr, nullptr, nullptr, ui->recordingBitrateLabel, ui->recordingBitrate, recordingOutput, true);
}

void PLSBasicStats::Reset()
{
	timer.start();

	first_encoded = 0xFFFFFFFF;
	first_skipped = 0xFFFFFFFF;
	first_rendered = 0xFFFFFFFF;
	first_lagged = 0xFFFFFFFF;

	OBSOutput strOutput = obs_frontend_get_streaming_output();
	OBSOutput recOutput = obs_frontend_get_recording_output();
	obs_output_release(strOutput);
	obs_output_release(recOutput);

	outputLabels[0].Reset(strOutput);
	outputLabels[1].Reset(recOutput);
	Update();
}

void PLSBasicStats::OutputLabels::Update(QLabel *framedropState, QLabel *framedropLabel, QLabel *framedrop, QLabel *bitrateLabel, QLabel *bitrate, obs_output_t *output, bool rec)
{
	bool active = obs_output_active(output);
	uint64_t totalBytes = output ? obs_output_get_total_bytes(output) : 0;
	uint64_t curTime = os_gettime_ns();
	uint64_t bytesSent = totalBytes;

	if (bytesSent < lastBytesSent)
		bytesSent = 0;
	if (bytesSent == 0)
		lastBytesSent = 0;

	uint64_t bitsBetween = (bytesSent - lastBytesSent) * 8;
	long double timePassed = (long double)(curTime - lastBytesSentTime) / 1000000000.0l;
	kbps = (long double)bitsBetween / timePassed / 1000.0l;

	if (timePassed < 0.01l)
		kbps = 0.0l;

	if (bitrate) {
		pls_flush_style(bitrateLabel, "active", active);

		bitrate->setText(QString("%1 kb/s").arg(QString::number(kbps, 'f', 0)));
		pls_flush_style(bitrate, "active", active);
	}

	if (!rec) {
		int total = output ? obs_output_get_total_frames(output) : 0;
		int dropped = output ? obs_output_get_frames_dropped(output) : 0;

		if (total < first_total || dropped < first_dropped) {
			first_total = 0;
			first_dropped = 0;
		}

		total -= first_total;
		dropped -= first_dropped;

		long double num = total ? (long double)dropped / (long double)total * 100.0l : 0.0l;

		pls_flush_style(framedropLabel, "active", active);

		framedrop->setText(QString("%1 / %2 (%3%)").arg(QString::number(dropped), QString::number(total), QString::number(num, 'f', 1)));
		pls_flush_style(framedrop, "active", active);

		if (!active) {
			pls_flush_style(framedropState, "state", "disabled");
		} else if (num > 5.0l) {
			pls_flush_style(framedropState, "state", "error");
		} else if (num > 1.0l) {
			pls_flush_style(framedropState, "state", "warning");
		} else {
			pls_flush_style(framedropState, "state", "");
		}
	}

	lastBytesSent = bytesSent;
	lastBytesSentTime = curTime;
}

void PLSBasicStats::OutputLabels::Reset(obs_output_t *output)
{
	if (!output)
		return;

	first_total = obs_output_get_total_frames(output);
	first_dropped = obs_output_get_frames_dropped(output);
	kbps = 0.0;
}

bool PLSBasicStats::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == dialogView) {
		switch (event->type()) {
		case QEvent::Show:
			timer.start(TIMER_INTERVAL);
			emit showSignal();
			break;
		case QEvent::Hide:
			timer.stop();
			emit hideSignal();
			break;
		}
	}
	return QWidget::eventFilter(watched, event);
}

void PLSBasicStats::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);

	if (event->button() == Qt::LeftButton) {
		grabMouse();
	}
}

void PLSBasicStats::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);

	if (event->button() == Qt::LeftButton) {
		releaseMouse();

		if (dialogView->geometry().contains(event->globalPos())) {
			dialogView->hide();
		}
	}
}
