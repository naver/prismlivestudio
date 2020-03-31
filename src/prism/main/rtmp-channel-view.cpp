#include "rtmp-channel-view.hpp"
#include "ui_PLSRtmpChannelView.h"
#include "login-info.hpp"
#include "pls-common-language.hpp"
#include "pls-common-define.hpp"
#include <qcombobox.h>

PLSRtmpChannelView::PLSRtmpChannelView(PLSLoginInfo *li, QJsonObject &res, QWidget *parent) : QDialog(parent), ui(new Ui::PLSRtmpChannelView), loginInfo(li), result(res)
{
	initUi();
}

PLSRtmpChannelView::~PLSRtmpChannelView()
{
	delete ui;
}

void PLSRtmpChannelView::initUi()
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	languageChange();
	initCommbox();
	ui->pushButton->setEnabled(false);
	connect(ui->comboBox, &QComboBox::currentTextChanged, this, [=]() { ui->comboBox->currentData(Qt::UserRole); });
	connect(ui->lineEdit, &QLineEdit::textEdited, this, &PLSRtmpChannelView::updateSaveBtnAvailable);
	connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &PLSRtmpChannelView::updateSaveBtnAvailable);
	connect(ui->lineEdit_3, &QLineEdit::textEdited, this, &PLSRtmpChannelView::updateSaveBtnAvailable);
}

void PLSRtmpChannelView::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		languageChange();
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
void PLSRtmpChannelView::on_pushButton_clicked()
{
	done(Accepted);
}
void PLSRtmpChannelView::on_pushButton_2_clicked()
{
	done(Rejected);
}
void PLSRtmpChannelView::updateSaveBtnAvailable(const QString &)
{
	if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty()) {
		ui->pushButton->setEnabled(false);
	} else {
		ui->pushButton->setEnabled(true);
	}
}
void PLSRtmpChannelView::languageChange()
{
	ui->lineEdit_4->setPlaceholderText(tr(SETTING_CHANNEL_RTMP_GUIDE_OPTIONAL));
	ui->lineEdit_5->setPlaceholderText(tr(SETTING_CHANNEL_RTMP_GUIDE_OPTIONAL));
}

void PLSRtmpChannelView::initCommbox()
{
	if (!loginInfo) {
		ui->comboBox->addItems(QStringList() << SELECT << YOUTUBE << FACEBOOK << TWITCH << USER_INPUT);
	} else {
		ui->lineEdit->setText(loginInfo->name());
		ui->comboBox->addItems(QStringList() << YOUTUBE << FACEBOOK << TWITCH << USER_INPUT);
		ui->comboBox->setCurrentText(loginInfo->name());
		ui->lineEdit_2->setText(loginInfo->rtmpUrl());
	}
}
