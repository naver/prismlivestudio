#include "PLSToastMsgPopup.hpp"
#include "ui_PLSToastMsgPopup.h"
#include <QStyle>

#define SHOWTIME 5000
#define MSGTYPE "toastWaning"
PLSToastMsgPopup::PLSToastMsgPopup(QWidget *parent) : QLabel(parent), ui(new Ui::PLSToastMsgPopup), m_timer(new QTimer(this))
{
	ui->setupUi(this);
	PLSDpiHelper dpiHelper;
	dpiHelper.setFixedWidth(this, 325);
	connect(m_timer, &QTimer::timeout, [=]() {
		m_timer->stop();
		this->hide();
	});
	this->style()->unpolish(this);
	this->style()->polish(this);
}

PLSToastMsgPopup::~PLSToastMsgPopup()
{
	delete ui;
}

void PLSToastMsgPopup::showMsg(const QString &msg, pls_toast_info_type type)
{
	if (m_timer->isActive()) {
		m_timer->stop();
	}
	m_timer->start(SHOWTIME);

	switch (type) {
	case pls_toast_info_type::PLS_TOAST_NOTICE:
	case pls_toast_info_type::PLS_TOAST_REPLY_BUFFER:
		setProperty(MSGTYPE, true);
		break;
	case pls_toast_info_type::PLS_TOAST_ERROR:
		setProperty(MSGTYPE, false);
		break;
	default:
		break;
	}
	this->style()->unpolish(this);
	this->style()->polish(this);

	setText(msg);
	adjustSize();
}

void PLSToastMsgPopup::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		hide();
	}
	QLabel::mousePressEvent(event);
}
