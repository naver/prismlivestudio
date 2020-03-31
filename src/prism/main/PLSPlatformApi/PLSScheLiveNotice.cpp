#include "PLSScheLiveNotice.h"
#include "ui_PLSScheLiveNotice.h"

#include "ChannelCommonFunctions.h"

#include "log.h"
#include "action.h"

#define MODULE_NAME "PLSScheLiveNotice"

PLSScheLiveNotice::PLSScheLiveNotice(PLSPlatformBase *platform, const QString &title, const QString &startTime, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper), ui(new Ui::PLSScheLiveNotice)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSScheLiveNotice});

	setResizeEnabled(false);

	ui->setupUi(content());
	QMetaObject::connectSlotsByName(this);

	ui->scheLiveTitleLabel->setText(title);
	ui->scheLiveStartTimeLabel->setText(startTime);

	activateWindow();
}

PLSScheLiveNotice::~PLSScheLiveNotice()
{
	delete ui;
}

void PLSScheLiveNotice::on_okButton_clicked()
{
	PLS_UI_STEP(MODULE_NAME, "Schedule Live Notice's Ok Button", ACTION_CLICK);
	accept();
}
