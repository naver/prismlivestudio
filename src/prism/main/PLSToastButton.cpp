#include "PLSToastButton.hpp"
#include "ui_PLSToastButton.h"
#include "login-common-helper.hpp"
#include <qdebug.h>
#include <QPainter>
#include <QStyleOption>
#include "PLSDpiHelper.h"

#define RADIU 10
#define OFFSET_X 14
#define OFFSET_Y 6
#define MAXNUM 9
PLSToastButton::PLSToastButton(QWidget *parent) : QWidget(parent), ui(new Ui::PLSToastButton), m_num(0)
{
	ui->setupUi(this);
	this->installEventFilter(this);
	connect(ui->pushButton, &QPushButton::clicked, this, &PLSToastButton::clickButton);
	m_numLabel = new QLabel(this);
	m_numLabel->setObjectName("alertNumLabel");
	m_numLabel->setAlignment(Qt::AlignCenter);
	m_numLabel->setProperty("maxNum", false);
	LoginCommonHelpers::refreshStyle(m_numLabel);
	m_numLabel->raise();
	ui->pushButton->lower();
	m_numLabel->hide();

	//updateIconStyle(m_num);
	PLSDpiHelper dpiHelper;
	setNumLabelPositon(dpiHelper.getDpi(this));
	dpiHelper.notifyDpiChanged(this, [=](double dpi) { setNumLabelPositon(dpi); });
}

PLSToastButton::~PLSToastButton()
{
	delete ui;
}

void PLSToastButton::setNum(const int num)
{
	qDebug() << "num =" << num;
	if (num > 0) {
		m_numLabel->setProperty("maxNum", false);
		LoginCommonHelpers::refreshStyle(m_numLabel);
		m_numLabel->setNum(num);

		if (num >= 9) {
			m_numLabel->setProperty("maxNum", true);
			LoginCommonHelpers::refreshStyle(m_numLabel);
			m_numLabel->setText("9+");
		}
		m_numLabel->show();
	} else {
		m_numLabel->hide();
	}
	m_num = num;
	//updateIconStyle(m_num);
	update();
}

int PLSToastButton::num() const
{
	return m_num;
}

QString PLSToastButton::getNumText() const
{
	if (m_num > 9) {
		return QString("9+");
	} else {
		return QString::number(m_num);
	}
}

void PLSToastButton::setNumLabelPositon(const double dpi)
{
	QPointF center(this->rect().center());
	m_numLabel->move(center.x(), center.y() - PLSDpiHelper::calculate(dpi, 18));
}

bool PLSToastButton::eventFilter(QObject *o, QEvent *e)
{
	QMouseEvent *event = static_cast<QMouseEvent *>(e);
	if (o == this && event->type() == QMouseEvent::MouseButtonPress && event->button() == Qt::LeftButton) {
		ui->pushButton->clicked();
		return true;
	}
	return QWidget::eventFilter(o, e);
}

void PLSToastButton::setShowAlert(bool showAlert)
{
	ui->pushButton->setProperty("showAlert", showAlert);
	LoginCommonHelpers::refreshStyle(ui->pushButton);
}
