#include "toast-view.hpp"
#include "ui_PLSToastView.h"

#include <QTimer>
#include <QList>
#include <QMap>
#include <QStyle>

// class PLSToastView Implements
PLSToastView::PLSToastView(QWidget *parent, PLSDpiHelper dpiHelper) : PLSDialogView(parent, dpiHelper), ui(new Ui::PLSToastView), icon(Warning), timer(nullptr)
{
	setHasCaption(false);
	ui->setupUi(this->content());
	QObject::connect(ui->closeButton, &QPushButton::clicked, this, &PLSToastView::hide);
	ui->closeButton->hide();
}

PLSToastView::~PLSToastView()
{
	delete ui;
}

PLSToastView::Icon PLSToastView::getIcon() const
{
	return this->icon;
}

void PLSToastView::setIcon(Icon icon)
{
	this->icon = icon;
}

QPixmap PLSToastView::getPixmap() const
{
	return *ui->iconLabel->pixmap();
}

void PLSToastView::setPixmap(const QPixmap &pixmap)
{
	ui->iconLabel->setPixmap(pixmap);
}

void PLSToastView::setMessage(const QString &message)
{
	ui->messageLabel->setText(message);
}

void PLSToastView::show(Icon icon, const QString &message, int autoHide)
{
	if (timer) {
		timer->stop();
		delete timer;
		timer = nullptr;
	}

	setIcon(icon);
	setMessage(message);

	style()->unpolish(this);
	style()->polish(this);

	if (autoHide > 0) {
		timer = new QTimer(this);
		timer->setSingleShot(true);
		connect(timer, &QTimer::timeout, this, &PLSToastView::autoHide);
		timer->start(autoHide);
	}

	PLSDialogView::show();
}

void PLSToastView::autoHide()
{
	hide();
	delete timer;
	timer = nullptr;
}
