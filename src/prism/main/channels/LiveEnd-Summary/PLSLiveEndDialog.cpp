#include "PLSLiveEndDialog.h"
#include <frontend-api.h>
#include <util/config-file.h>
#include <QMap>
#include <QVariant>
#include "ChannelCommonFunctions.h"
#include "ChannelConst.h"
#include "PLSChannelDataAPI.h"
#include "PLSLiveEndItem.h"
#include "PLSPlatformApi.h"
#include "QDebug"
#include "QDesktopServices"
#include "QGridLayout"
#include "QImage"
#include "QPushButton"
#include "QUrl"
#include "log/log.h"
#include "pls-app.hpp"
#include "qfile.h"
#include "ui_PLSLiveEndDialog.h"

PLSLiveEndDialog::PLSLiveEndDialog(bool isRecord, QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::PLSLiveEndDialog), m_bRecord(isRecord)
{
	App()->DisableHotkeys();
	dpiHelper.setCss(this, {PLSCssIndex::PLSLiveEndDialog});

	ui->setupUi(this->content());
	setWindowTitle(QString());
	setIsMoveInContent(true);
	setHasMinButton(false);
	setHasCloseButton(true);
	setHasHLine(false);
	setResizeEnabled(false);
	this->setAttribute(Qt::WA_AlwaysShowToolTips, true);

	PLS_INFO(END_MODULE, "PLSEnd Dialog Show");

	this->setupFirstUI(dpiHelper);
	ui->titleLabel->adjustSize();
	connect(ui->okButton, &QPushButton::clicked, this, &PLSLiveEndDialog::okButtonClicked);
	connect(ui->savePushButton, &QPushButton::clicked, this, &PLSLiveEndDialog::openFileSavePath);
}

PLSLiveEndDialog::~PLSLiveEndDialog()
{
	App()->UpdateHotkeyFocusSetting();
	delete ui;
}

void PLSLiveEndDialog::setupFirstUI(PLSDpiHelper dpiHelper)
{
	const static QString placeholderString = "       ";
	bool recordWhenStreaming = config_get_bool(GetGlobalConfig(), "BasicWindow", "RecordWhenStreaming");
	ui->saveWidget->setHidden(!recordWhenStreaming && !m_bRecord);

	dpiHelper.notifyDpiChanged(this, [=](double dpi) {
		QString appendString = getRecordPath();
		appendString = appendString.append(placeholderString);
		QFontMetrics fontWidth(ui->savePushButton->font());
		QString pathString = fontWidth.elidedText(appendString, Qt::ElideRight, PLSDpiHelper::calculate(dpi, 550));
		if (!pathString.endsWith(placeholderString)) {
			pathString = pathString.append(placeholderString);
		}
		ui->savePushButton->setText(pathString);
		ui->savePushButton->setToolTip(getRecordPath());
	});

	ui->titleLabel->setText(tr(m_bRecord ? "broadcast.end.record" : "broadcast.end.live"));

	ui->okButton->setText(tr("OK"));

	setupScrollData(dpiHelper);
}

void PLSLiveEndDialog::setupScrollData(PLSDpiHelper dpiHelper)
{

	const static int endItemHeight = 75;
	const static int windowWidth = 720;
	const static int maxCurrentShowCount = 3;
	const static int recordWidnowHeight = 240;
	const static int liveModeOffest = 5;
	if (m_bRecord) {
		ui->channelScroll->setHidden(true);
		dpiHelper.setFixedSize(this, {windowWidth, recordWidnowHeight});
		ui->placehoderLabel->setFixedHeight(0);
		return;
	}

	mChannelVBoxLayout = static_cast<QVBoxLayout *>(ui->channelScroll->widget()->layout());
	ui->channelScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	int tmpLiveCount = PLS_PLATFORM_API->getUuidOnStarted().size();
	for (const auto &uid : PLS_PLATFORM_API->getUuidOnStarted()) {
		PLSLiveEndItem *endItem = new PLSLiveEndItem(uid, this);
		this->mChannelVBoxLayout->addWidget(endItem);
	}

	bool isExceendMax = tmpLiveCount > maxCurrentShowCount;
	ui->channelScroll->setVerticalScrollBarPolicy(isExceendMax ? Qt::ScrollBarPolicy::ScrollBarAlwaysOn : Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	ui->channelScroll->verticalScrollBar()->setEnabled(isExceendMax ? true : false);

	dpiHelper.notifyDpiChanged(this, [=](double dpi, double /*oldDpi*/, bool firstShow) {
		int liveCount = tmpLiveCount;

		int windowHeight = PLSDpiHelper::calculate(dpi, recordWidnowHeight);
		bool isExeend = false;
		if (isExceendMax) {
			liveCount = maxCurrentShowCount;
			isExeend = true;
		}

		if (liveCount != 0) {
			int scrolHeight = liveCount * PLSDpiHelper::calculate(dpi, endItemHeight);
			int dpiLiveModeOffest = isExeend ? 0 : PLSDpiHelper::calculate(dpi, liveModeOffest);
			windowHeight += scrolHeight;
			ui->channelScroll->setFixedHeight(scrolHeight - dpiLiveModeOffest);
			ui->placehoderLabel->setFixedHeight(dpiLiveModeOffest);
		} else {
			ui->channelScroll->setHidden(true);
		}

		this->resize(PLSDpiHelper::calculate(dpi, windowWidth), windowHeight);
	});
}

void PLSLiveEndDialog::okButtonClicked()
{
	PLS_UI_STEP(END_MODULE, "PLSEnd Dialog OK Button Click", ACTION_CLICK);
	this->close();
}

void PLSLiveEndDialog::openFileSavePath()
{
	QUrl videoPath = QUrl::fromLocalFile(getRecordPath());
	string logString = std::string("PLSEnd Dialog openFileSavePath Click, with path") + videoPath.toString().toStdString();
	PLS_UI_STEP(END_MODULE, logString.c_str(), ACTION_CLICK);
	QDesktopServices::openUrl(videoPath);
}

void PLSLiveEndDialog::onScreenAvailableGeometryChanged(const QRect &screenAvailableGeometry)
{
	extern QRect centerShow(PLSWidgetDpiAdapter * adapter, const QRect &screenAvailableGeometry, QRect &geometryOfNormal);

	PLSDialogView::onScreenAvailableGeometryChanged(screenAvailableGeometry);
	centerShow(this, screenAvailableGeometry, geometryOfNormal);
}

const QString PLSLiveEndDialog::getRecordPath()
{

	const char *mode = pls_basic_config_get_string("Output", "Mode");
	bool adv = astrcmpi(mode, "Advanced") == 0;

	const char *path = pls_basic_config_get_string("SimpleOutput", "FilePath");
	if (adv) {
		path = pls_basic_config_get_string("AdvOut", "RecFilePath");
	}

	return QString(path);
}
