#include "PLSLiveInfoBand.h"
#include "ui_PLSLiveInfoBand.h"
#include "PLSChannelDataAPI.h"
#include "ChannelCommonFunctions.h"
#include "PLSPlatformApi.h"
#include "QTextFrame"
#include <QTextDocument>
#include <QTextFrameFormat>
#include <qdebug.h>
#include "login-common-helper.hpp"
#include "PLSPlatformBand.h"
#include "../channels/ChannelsDataApi/PLSChannelDataAPI.h"

#define MAXINPUTCONTENT 500
static const char *liveInfoBandModule = "PLSLiveInfoBand";

PLSLiveInfoBand::PLSLiveInfoBand(PLSPlatformBase *pPlatformBase, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSLiveInfoBase(pPlatformBase, parent, dpiHelper), ui(new Ui::PLSLiveInfoBand), platform(dynamic_cast<PLSPlatformBand *>(pPlatformBase)), m_isModify(false)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSLiveInfoBand});
	dpiHelper.setFixedSize(this, {720, 550});

	ui->setupUi(this->content());
	platform->setAlertParent(this);
	ui->plainTextEdit->setProperty("readonly", false);
	if (PLSCHANNELS_API->isLiving()) {
		ui->plainTextEdit->setProperty("readonly", true);
		ui->plainTextEdit->setReadOnly(true);
		ui->okButton->setDisabled(true);
		ui->describeLabel->setDisabled(true);
	}

	m_uuid = platform->getChannelUUID();
	content()->setFocusPolicy(Qt::StrongFocus);
	auto description = QString::fromUtf8(platform->getTitle().c_str());
	if (!description.isEmpty()) {
		ui->plainTextEdit->setPlainText(description);
	}
	connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &PLSLiveInfoBand::textChangeHandler);
	connect(ui->okButton, &QPushButton::clicked, this, &PLSLiveInfoBand::okButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &PLSLiveInfoBand::cancelButtonClicked);

	updateStepTitle(ui->okButton);

	if (PLS_PLATFORM_API->isPrepareLive()) {
		ui->horizontalLayout_2->addWidget(ui->okButton);
	}
}

PLSLiveInfoBand::~PLSLiveInfoBand()
{
	delete ui;
}

QString PLSLiveInfoBand::getDescription() const
{
	return m_description;
}

bool PLSLiveInfoBand::isModify()
{
	return (m_description == ui->plainTextEdit->toPlainText());
}

void PLSLiveInfoBand::okButtonClicked()
{
	PLS_UI_STEP(liveInfoBandModule, "Band liveinfo OK Button Click", ACTION_CLICK);
	m_description = ui->plainTextEdit->toPlainText();
	platform->setTitle(m_description.toUtf8().data());

	auto checkStreamKey = [=](int value) {
		hideLoading();
		if (value == 0) {
			done(Accepted);
		} else if (value == static_cast<int>(PLSPlatformApiResult::PAR_TOKEN_EXPIRED)) {
			PLSAlertView::Button btn = PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.LiveInfo.Refresh.Band.Expired"));
			done(Rejected);
			if (btn != PLSAlertView::Button::NoButton) {
				PLSCHANNELS_API->channelExpired(platform->getChannelUUID(), false);
			}
		}
	};

	if (PLS_PLATFORM_API->isPrepareLive()) {
		showLoading(content());
		platform->setDescription(m_description.toUtf8().data());
		platform->requestLiveStreamKey(checkStreamKey);

	} else {
		done(Accepted);
	}
}

void PLSLiveInfoBand::cancelButtonClicked()
{
	PLS_UI_STEP(liveInfoBandModule, "Band liveinfo Cancel Button Click", ACTION_CLICK);
	done(Rejected);
}

void PLSLiveInfoBand::textChangeHandler()
{
	m_isModify = true;
	QString text = ui->plainTextEdit->toPlainText();
	if (text.length() > MAXINPUTCONTENT) {
		ui->plainTextEdit->setPlainText(text.left(MAXINPUTCONTENT));
		PLSAlertView::warning(this, QTStr("Live.Check.Alert.Title"), QTStr("Live.Check.Band.Description.Max.Limit"));
		ui->plainTextEdit->moveCursor(QTextCursor::End);
	}
}
