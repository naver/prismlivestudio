#include "PLSFileButton.hpp"
#include "ui_PLSFileButton.h"
#include "login-common-helper.hpp"
#include "pls-common-define.hpp"

PLSFileButton::PLSFileButton(QWidget *parent) : QPushButton(parent), ui(new Ui::PLSFileButton)
{
	ui->setupUi(this);
}

PLSFileButton::~PLSFileButton()
{
	delete ui;
}

void PLSFileButton::setFileButtonEnabled(bool enabled)
{
	m_enabled = enabled;
	if (m_enabled) {
		this->setProperty(STATUS, STATUS_NORMAL);
	} else {
		this->setProperty(STATUS, STATUS_DISABLE);
	}
	LoginCommonHelpers::refreshStyle(ui->fileTitleLabel);
	LoginCommonHelpers::refreshStyle(ui->fileIconLabel);
}

void PLSFileButton::enterEvent(QEvent *event)
{
	if (!m_enabled) {
		return;
	}
	this->setProperty(STATUS, STATUS_HOVER);
	LoginCommonHelpers::refreshStyle(ui->fileTitleLabel);
	LoginCommonHelpers::refreshStyle(ui->fileIconLabel);
	QPushButton::enterEvent(event);
}

void PLSFileButton::leaveEvent(QEvent *event)
{
	if (!m_enabled) {
		return;
	}
	this->setProperty(STATUS, STATUS_NORMAL);
	LoginCommonHelpers::refreshStyle(ui->fileTitleLabel);
	LoginCommonHelpers::refreshStyle(ui->fileIconLabel);
	QPushButton::leaveEvent(event);
}

void PLSFileButton::mousePressEvent(QMouseEvent *event)
{
	if (!m_enabled) {
		return;
	}
	this->setProperty(STATUS, STATUS_CLICKED);
	LoginCommonHelpers::refreshStyle(ui->fileTitleLabel);
	LoginCommonHelpers::refreshStyle(ui->fileIconLabel);
	QPushButton::mousePressEvent(event);
}

void PLSFileButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (!m_enabled) {
		return;
	}
	this->setProperty(STATUS, STATUS_NORMAL);
	LoginCommonHelpers::refreshStyle(ui->fileTitleLabel);
	LoginCommonHelpers::refreshStyle(ui->fileIconLabel);
	QPushButton::mouseReleaseEvent(event);
}
