#include "PLSTakeCameraSnapshot.h"
#include "ui_PLSTakeCameraSnapshot.h"

#include <QUuid>
#include <QSignalBlocker>

#include "properties-view.hpp"
#include "display-helpers.hpp"
#include "ChannelCommonFunctions.h"
#include "frontend-api.h"
#include "pls-common-define.hpp"
#include "window-basic-properties.hpp"

#include "log.h"

#define CSTR_VIDEO_DEVICE_ID "video_device_id"
#define CSTR_CAPTURE_STATE "capture_state"
#define CSTR_SOURCE_IMAGE_STATUS "source_image_status"

extern QString getTempImageFilePath(const QString &suffix);

namespace {
struct FindSourceResult {
	const char *camera;
	obs_source_t *source;
};

bool findAllSource(void *context, obs_source_t *source)
{
	FindSourceResult *result = (FindSourceResult *)context;

	obs_source_error ose;
	if (!obs_source_get_capture_valid(source, &ose)) {
		return true;
	}

	obs_data_t *settings = obs_source_get_settings(source);
	if (!settings) {
		return true;
	}

	const char *camera = obs_data_get_string(settings, CSTR_VIDEO_DEVICE_ID);
	if (camera && !strcmp(camera, result->camera)) {
		result->source = source;
		obs_data_release(settings);
		return false;
	}

	obs_data_release(settings);
	return true;
}
obs_source_t *createSource(const char *camera, const char *name)
{
	FindSourceResult result = {camera, nullptr};
	obs_enum_sources(&findAllSource, &result);

	obs_source_t *source = nullptr;
	if (!result.source) {
		obs_data_t *settings = obs_data_create();
		obs_data_set_string(settings, CSTR_VIDEO_DEVICE_ID, camera);
		source = obs_source_create_private(DSHOW_SOURCE_ID, name, settings);
		obs_data_release(settings);
	} else {
		obs_source_addref(result.source);
		source = result.source;
	}
	return source;
}
QString captureImage(uint32_t width, uint32_t height, enum gs_color_format format, uint8_t **data, uint32_t *linesize)
{
	if (format == gs_color_format::GS_BGRA) {
		uchar *buffer = (uchar *)malloc(width * height * 4);
		if (!buffer) {
			return QString();
		}

		memset(buffer, 0, width * height * 4);

		if (width * 4 == linesize[0]) {
			memmove(buffer, data[0], linesize[0] * height);
		} else {
			for (uint32_t i = 0; i < height; i++) {
				memmove(buffer + i * width * 4, data[0] + i * linesize[0], width * 4);
			}
		}

		QImage image(buffer, width, height, QImage::Format_ARGB32);

		QString cropedImageFile1 = getTempImageFilePath(".bmp");
		image.save(cropedImageFile1, "BMP");

		free(buffer);
		return cropedImageFile1;
	}
	return QString();
}

}

PLSTakeCameraSnapshotTakeTimerMask::PLSTakeCameraSnapshotTakeTimerMask(QWidget *parent) : QDialog(parent, Qt::Tool | Qt::FramelessWindowHint)
{
	setObjectName("PLSTakeCameraSnapshotTakeTimerMask");
	setAttribute(Qt::WA_TranslucentBackground);

	maskLabel = new QLabel(this);
	maskLabel->setObjectName("maskLabel");
	maskLabel->setAlignment(Qt::AlignCenter);
}
PLSTakeCameraSnapshotTakeTimerMask::~PLSTakeCameraSnapshotTakeTimerMask() {}

void PLSTakeCameraSnapshotTakeTimerMask::start(int ctime)
{
	maskLabel->setText(QString::number(ctime));

	QTimer::singleShot(1000, this, [=]() {
		if (ctime > 1) {
			start(ctime - 1);
		} else {
			emit timeout();
		}
	});
}

bool PLSTakeCameraSnapshotTakeTimerMask::event(QEvent *event)
{
	if (event->type() == QEvent::Resize) {
		maskLabel->setGeometry(QRect(QPoint(0, 0), dynamic_cast<QResizeEvent *>(event)->size()));
	}

	return QDialog::event(event);
}

PLSTakeCameraSnapshot::PLSTakeCameraSnapshot(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::PLSTakeCameraSnapshot())
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSTakeCameraSnapshot});

	setResizeEnabled(false);

	timerMask = new PLSTakeCameraSnapshotTakeTimerMask(this);
	connect(timerMask, &PLSTakeCameraSnapshotTakeTimerMask::timeout, this, &PLSTakeCameraSnapshot::onTimerEnd);
	timerMask->hide();

	ui->setupUi(content());
	QMetaObject::connectSlotsByName(this);
	dpiHelper.setMinimumSize(ui->preview, {20, 150});
	ui->preview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	ui->preview->installEventFilter(this);

	cameraUninitMask = new QFrame(content());
	cameraUninitMask->setObjectName("cameraUninitMask");

	QVBoxLayout *cameraUninitMaskLayout = new QVBoxLayout(cameraUninitMask);
	cameraUninitMaskLayout->setMargin(0);
	cameraUninitMaskLayout->setSpacing(15);

	QLabel *cameraUninitMaskIcon = new QLabel(cameraUninitMask);
	cameraUninitMaskIcon->setObjectName("cameraUninitMaskIcon");
	cameraUninitMaskText = new QLabel(cameraUninitMask);
	cameraUninitMaskText->setObjectName("cameraUninitMaskText");
	cameraUninitMaskText->setAlignment(Qt::AlignCenter);
	cameraUninitMaskText->setText(tr("TakeCameraSnapshot.Wait"));
	cameraUninitMaskLayout->addStretch(1);
	cameraUninitMaskLayout->addWidget(cameraUninitMaskIcon, 0, Qt::AlignHCenter);
	cameraUninitMaskLayout->addWidget(cameraUninitMaskText, 0, Qt::AlignHCenter);
	cameraUninitMaskLayout->addStretch(1);

	QLabel *cameraSettingButtonIcon = new QLabel(ui->cameraSettingButton);
	cameraSettingButtonIcon->setObjectName("cameraSettingButtonIcon");
	QHBoxLayout *cameraSettingButtonLayout = new QHBoxLayout(ui->cameraSettingButton);
	cameraSettingButtonLayout->setMargin(0);
	cameraSettingButtonLayout->addWidget(cameraSettingButtonIcon, 0, Qt::AlignCenter);

	auto addDrawCallback = [this]() { obs_display_add_draw_callback(ui->preview->GetDisplay(), PLSTakeCameraSnapshot::drawPreview, this); };
	connect(ui->preview, &PLSQTDisplay::DisplayCreated, addDrawCallback);
	ui->preview->show();

	QList<QPair<QString, QString>> cameraList = getCameraList();
	if (!cameraList.isEmpty()) {
		for (auto &camera : cameraList) {
			QSignalBlocker blocker(ui->cameraList);
			ui->cameraList->addItem(camera.first, camera.second);
		}

		init(cameraList.first().second);
	} else {
		cameraUninitMaskText->setText(tr("TakeCameraSnapshot.NoDevice"));
		ui->cameraList->addItem(tr("TakeCameraSnapshot.NoDevice"));
		ui->cameraList->setEnabled(false);
		ui->cameraSettingButton->setEnabled(false);
		ui->okButton->setEnabled(false);
	}
}

PLSTakeCameraSnapshot::~PLSTakeCameraSnapshot()
{
	ClearSource();
	obs_display_remove_draw_callback(ui->preview->GetDisplay(), PLSTakeCameraSnapshot::drawPreview, this);
	delete ui;
}

QString PLSTakeCameraSnapshot::getSnapshot()
{
	if (hasError) {
		return QString();
	} else if (exec() == Accepted) {
		return imageFilePath;
	}
	return QString();
}

QList<QPair<QString, QString>> PLSTakeCameraSnapshot::getCameraList()
{
	QList<QPair<QString, QString>> cameraList;
	if (obs_properties_t *properties = obs_get_source_properties(DSHOW_SOURCE_ID); properties) {
		if (obs_property_t *property = obs_properties_get(properties, CSTR_VIDEO_DEVICE_ID); property) {
			for (size_t i = 0, count = obs_property_list_item_count(property); i < count; ++i) {
				QString name = QString::fromUtf8(obs_property_list_item_name(property, i));
				QString value = QString::fromUtf8(obs_property_list_item_string(property, i));
				cameraList.append({name, value});
			}
		}

		obs_properties_destroy(properties);
	}
	return cameraList;
}

void PLSTakeCameraSnapshot::init(const QString &camera)
{
	cameraUninitMaskText->setText(tr("TakeCameraSnapshot.Wait"));
	cameraUninitMask->show();
	ui->okButton->setEnabled(false);

	ClearSource();

	QByteArray name = "take-camera-snapshot-" + QUuid::createUuid().toString().toUtf8();
	source = createSource(camera.toUtf8().constData(), name.constData());
	if (!source) {
		hasError = true;
		return;
	}

	PLSBasicProperties::SetOwnerWindow(source, window()->winId());

	signal_handler_connect_ref(obs_source_get_signal_handler(source), CSTR_CAPTURE_STATE, &PLSTakeCameraSnapshot::onSourceCaptureState, this);
	signal_handler_connect_ref(obs_source_get_signal_handler(source), CSTR_SOURCE_IMAGE_STATUS, &PLSTakeCameraSnapshot::onSourceImageStatus, this);
	ui->preview->UpdateSourceState(source);
	ui->preview->AttachSource(source);

	onSourceCaptureState();
	onSourceImageStatus(obs_source_get_image_status(source));

	obs_source_inc_showing(source);

	obs_source_release(source);
}

void PLSTakeCameraSnapshot::ClearSource()
{
	if (source) {
		PLSBasicProperties::SetOwnerWindow(source, 0);
		signal_handler_disconnect(obs_source_get_signal_handler(source), CSTR_CAPTURE_STATE, &PLSTakeCameraSnapshot::onSourceCaptureState, this);
		signal_handler_disconnect(obs_source_get_signal_handler(source), CSTR_SOURCE_IMAGE_STATUS, &PLSTakeCameraSnapshot::onSourceImageStatus, this);
		obs_source_dec_showing(source);
		source = nullptr;
	}
}

void PLSTakeCameraSnapshot::drawPreview(void *data, uint32_t cx, uint32_t cy)
{
	PLSTakeCameraSnapshot *window = static_cast<PLSTakeCameraSnapshot *>(data);

	if (!window->source) {
		return;
	}

	uint32_t sourceCX = std::max(obs_source_get_width(window->source), 1u);
	uint32_t sourceCY = std::max(obs_source_get_height(window->source), 1u);

	int x, y;
	int newCX, newCY;
	float scale;

	GetScaleAndCenterPos(sourceCX, sourceCY, cx, cy, x, y, scale);

	newCX = int(scale * float(sourceCX));
	newCY = int(scale * float(sourceCY));

	gs_viewport_push();
	gs_projection_push();
	gs_ortho(0.0f, float(sourceCX), 0.0f, float(sourceCY), -100.0f, 100.0f);
	gs_set_viewport(x, y, newCX, newCY);

	obs_source_video_render(window->source);

	gs_projection_pop();
	gs_viewport_pop();

	// capture image
	if (window->captureImage) {
		window->captureImage = false;

		uint32_t width, height;
		enum gs_color_format format;
		texture_map_info tmi;
		if (auto ss = gs_device_canvas_map(&width, &height, &format, tmi.data, tmi.linesize); ss) {
			window->imageFilePath = ::captureImage(width, height, format, tmi.data, tmi.linesize);
			gs_device_canvas_unmap(ss);
		}

		QMetaObject::invokeMethod(window, &PLSTakeCameraSnapshot::accept, Qt::QueuedConnection);
	}
}

void PLSTakeCameraSnapshot::onSourceCaptureState(void *data, calldata_t *calldata)
{
	QMetaObject::invokeMethod(static_cast<PLSTakeCameraSnapshot *>(data), "onSourceCaptureState", Qt::QueuedConnection);
}

void PLSTakeCameraSnapshot::onSourceImageStatus(void *data, calldata_t *calldata)
{
	PLSTakeCameraSnapshot *tcs = static_cast<PLSTakeCameraSnapshot *>(data);
	bool status = calldata_bool(calldata, "image_status");
	QMetaObject::invokeMethod(tcs, "onSourceImageStatus", Qt::QueuedConnection, Q_ARG(bool, status));
}

void PLSTakeCameraSnapshot::on_cameraList_currentIndexChanged(int index)
{
	QString camera = ui->cameraList->itemData(index).toString();
	if (!camera.isEmpty()) {
		init(camera);
	}
}
void PLSTakeCameraSnapshot::on_cameraSettingButton_clicked()
{
	if (source) {
		if (obs_properties_t *properties = obs_source_properties(source); properties) {
			if (obs_property_t *property = obs_properties_get(properties, "button group"); property) {
				obs_property_button_group_clicked_by_name(property, source, "video_config");
			}
		}
	}
}
void PLSTakeCameraSnapshot::on_okButton_clicked()
{
	ui->cameraList->setEnabled(false);
	ui->cameraSettingButton->setEnabled(false);
	ui->okButton->setEnabled(false);
	ui->cancelButton->setEnabled(false);

	timerMask->setGeometry(QRect(ui->preview->mapToGlobal(QPoint(0, 0)), ui->preview->size()));
	timerMask->start(3);
	timerMask->show();
}
void PLSTakeCameraSnapshot::on_cancelButton_clicked()
{
	reject();
}

void PLSTakeCameraSnapshot::onTimerEnd()
{
	timerMask->hide();
	captureImage = true;
	// accept();
}

void PLSTakeCameraSnapshot::onSourceCaptureState()
{
	enum obs_source_error error;
	obs_source_get_capture_valid(source, &error);
	if (error == OBS_SOURCE_ERROR_OK) {
		cameraUninitMask->hide();
		ui->okButton->setEnabled(true);
	} else if (error == OBS_SOURCE_ERROR_NOT_FOUND) {
		cameraUninitMaskText->setText(tr("TakeCameraSnapshot.NotFound"));
		cameraUninitMask->show();
		ui->okButton->setEnabled(false);
	} else if (error == OBS_SOURCE_ERROR_BE_USING) {
		cameraUninitMaskText->setText(tr("TakeCameraSnapshot.Used"));
		cameraUninitMask->show();
		ui->okButton->setEnabled(false);
	}
}

void PLSTakeCameraSnapshot::onSourceImageStatus(bool status)
{
	cameraUninitMask->setHidden(status);
	ui->okButton->setEnabled(status);
}

bool PLSTakeCameraSnapshot::event(QEvent *event)
{
	if ((event->type() == QEvent::Move) && timerMask->isVisible()) {
		timerMask->setGeometry(QRect(ui->preview->mapToGlobal(QPoint(0, 0)), ui->preview->size()));
	}
	return PLSDialogView::event(event);
}

bool PLSTakeCameraSnapshot::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == ui->preview) {
		switch (event->type()) {
		case QEvent::Resize:
			cameraUninitMask->setGeometry(QRect(ui->preview->pos(), dynamic_cast<QResizeEvent *>(event)->size()));
			break;
		case QEvent::Move:
			cameraUninitMask->setGeometry(QRect(dynamic_cast<QMoveEvent *>(event)->pos(), ui->preview->size()));
			break;
		}
	}
	return PLSDialogView::eventFilter(watched, event);
}
