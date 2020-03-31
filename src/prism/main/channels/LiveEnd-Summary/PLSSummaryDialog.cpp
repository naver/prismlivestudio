#include "PLSSummaryDialog.h"
#include "PLSSummaryItem.h"
#include "ui_PLSSummaryDialog.h"

PLSSummaryDialog::PLSSummaryDialog(const QMap<QString, QVariantMap> &sourceLst, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper), ui(new Ui::PLSSummaryDialog), mChannelsLst(sourceLst)
{
	setupFirstUI();
}

PLSSummaryDialog::~PLSSummaryDialog()
{
	delete ui;
}

void PLSSummaryDialog::setupFirstUI()
{
	ui->setupUi(this->content());
	setWindowTitle(QString());
	setIsMoveInContent(true);
	setHasMinButton(false);
	setHasHLine(false);

	setupData();

	connect(ui->okButton, &QPushButton::clicked, this, &PLSSummaryDialog::okButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &PLSSummaryDialog::cancelButtonClicked);
}

void PLSSummaryDialog::setupData()
{
	ui->titleLabel->setText("Check the following info, and click on Go Live to start the broadcast.");
	ui->cancelButton->setText("Cancel");
	ui->okButton->setText("OK");

	setupDetailData();
	setupScrollData();
}

void PLSSummaryDialog::setupDetailData()
{
	QString str("<span style = \" font-size:12px; font-weight: normal; color:#ffffff;\">%1</span>\
		     <span style = \" font-size:12px; font-weight: bold; color:#ffffff;\">%2</span>\
		     <span style = \" font-size:12px; font-weight: bold; color:#666666;\">·</span>\
		     <span style = \" font-size:12px; font-weight: normal; color:#ffffff;\">%3</span>\
		     <span style = \" font-size:12px; font-weight: bold; color:#ffffff;\">%4</span>\
		     <span style = \" font-size:12px; font-weight: bold; color:#666666;\">·</span>\
		     <span style = \" font-size:12px; font-weight: normal; color:#ffffff;\">%5</span>\
		     <span style = \" font-size:12px; font-weight: bold; color:#ffffff;\">%6</span>");

	QString quality = "999p";
	QString fpsString = "1fps";
	QString videoQualityString = "자동조정";
	ui->detailLabel->setText(str.arg("해상도 : ", quality, "FPS : ", fpsString, "영상화질 : ", videoQualityString));
}

void PLSSummaryDialog::setupScrollData()
{
	mChannelVBoxLayout = static_cast<QVBoxLayout *>(ui->channelScroll->widget()->layout());
	ui->channelScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	int liveCount = 0;
	for (const QMap<QString, QVariant> &data : mChannelsLst) {
		PLSSummaryItem *endItem = new PLSSummaryItem(data, this);
		this->mChannelVBoxLayout->addWidget(endItem);
		liveCount++;
	}

	int windowHeight = 108;
	if (liveCount > 3) {
		windowHeight += 10;
		liveCount = 3;
	}
	windowHeight += liveCount * (75);
	windowHeight += 121;
	this->resize(630, windowHeight);
}

void PLSSummaryDialog::okButtonClicked()
{
	this->accept();
}
void PLSSummaryDialog::cancelButtonClicked()
{
	this->reject();
}
