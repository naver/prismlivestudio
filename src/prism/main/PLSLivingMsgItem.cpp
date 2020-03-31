#include "PLSLivingMsgItem.hpp"
#include "ui_PLSLivingMsgItem.h"
#include "login-common-helper.hpp"
#include <QStyle>

#define MINTIME 60000
#define HOURTIME 3600000
#define HOUR 60
#define INTERVAL 71
PLSLivingMsgItem::PLSLivingMsgItem(const QString &info, const long long time, pls_toast_info_type type, QWidget *parent)
	: QWidget(parent), ui(new Ui::PLSLivingMsgItem), m_msgInfo(info), m_times(time), m_type(type)
{
	ui->setupUi(this);
	ui->label_msg->style()->unpolish(ui->label_msg);
	ui->label_msg->style()->polish(ui->label_msg);

	ui->label_time->setText(tr("chat.note.time.justnow"));
	setToastIcon(type);
	adjustSize();
}

PLSLivingMsgItem::~PLSLivingMsgItem()
{
	delete ui;
}

void PLSLivingMsgItem::setMsgInfo(const QString &info)
{
	m_msgInfo = info;
	ui->label_msg->setText(m_msgInfo);
	ui->label_msg->adjustSize();
	ui->label_msg->setWordWrap(true);
	ui->label_msg->setAlignment(Qt::AlignTop);
}

QString PLSLivingMsgItem::getMsgInfo() const
{
	return m_msgInfo;
}

void PLSLivingMsgItem::setTimes(long long time)
{
	m_times = time;
}

qint64 PLSLivingMsgItem::getTimes() const
{
	return m_times;
}

void PLSLivingMsgItem::updateTimeView(const qint64 currentTime)
{
	QString timeStr;
	qint64 timeOffset = (currentTime - m_times) / MINTIME;
	if (0 == timeOffset) {
		timeStr = QString("%1").arg(tr("chat.note.time.justnow"));
	} else if (timeOffset > 0 && timeOffset <= 1) {
		timeStr = QString("%1").arg(tr("chat.note.time.1minute"));
	} else if (timeOffset > 1 && timeOffset < HOUR) {
		timeStr = QString("%1%2").arg(QString::number(timeOffset)).arg(tr("chat.note.time.minutesago"));

	} else if (timeOffset > HOUR && timeOffset < HOUR * 2) {
		timeStr = QString("%1").arg(tr("chat.note.time.1hour"));
	} else if (timeOffset >= HOUR * 2) {
		timeStr = QString("%1%2").arg(QString::number(timeOffset / HOUR)).arg(tr("chat.note.time.hoursago"));
	}
	ui->label_time->setText(timeStr);
}

void PLSLivingMsgItem::setToastIcon(pls_toast_info_type type)
{
	switch (type) {
	case pls_toast_info_type::PLS_TOAST_ERROR:
		break;
	case pls_toast_info_type::PLS_TOAST_REPLY_BUFFER:
		break;
	case pls_toast_info_type::PLS_TOAST_NOTICE:
		break;
	default:
		break;
	}
	ui->label_icon->setProperty("type", static_cast<int>(type));
	LoginCommonHelpers::refreshStyle(ui->label_icon);
}

void PLSLivingMsgItem::setMsgType(pls_toast_info_type type)
{
	m_type = type;
}

pls_toast_info_type PLSLivingMsgItem::getMsgType() const
{
	return m_type;
}
