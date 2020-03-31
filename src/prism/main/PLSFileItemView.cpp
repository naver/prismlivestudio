#include "PLSFileItemView.hpp"
#include "ui_PLSFileItemView.h"
#include "pls-common-define.hpp"
#include "login-common-helper.hpp"
#include <qstyle.h>

PLSFileItemView::PLSFileItemView(int index, QWidget *parent) : QFrame(parent), ui(new Ui::PLSFileItemView), m_index(index)
{
	ui->setupUi(this);
	ui->fileNameLabel->style()->unpolish(ui->fileNameLabel);
	ui->fileNameLabel->style()->polish(ui->fileNameLabel);
}

PLSFileItemView::~PLSFileItemView()
{
	delete ui;
}

QFont PLSFileItemView::fileNameLabelFont() const
{
	return ui->fileNameLabel->font();
}

void PLSFileItemView::setFileName(const QString &filename)
{
	ui->fileNameLabel->setText(filename);
}

void PLSFileItemView::enterEvent(QEvent *event)
{
	this->setStyleSheet("background-color:#666666");
	ui->deleteIcon->setProperty(STATUS, STATUS_HOVER);
	LoginCommonHelpers::refreshStyle(ui->deleteIcon);
}

void PLSFileItemView::leaveEvent(QEvent *event)
{
	this->setStyleSheet("background-color:#444444");
	ui->deleteIcon->setProperty(STATUS, STATUS_NORMAL);
	LoginCommonHelpers::refreshStyle(ui->deleteIcon);
}

void PLSFileItemView::on_deleteIcon_clicked()
{
	emit deleteItem(m_index);
}
