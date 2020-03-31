#include "pls-complex-header-icon.hpp"
#include "ui_PLSComplexHeaderIcon.h"

#include "channels/ChannelsDataApi/ChannelCommonFunctions.h"
#include <qsvgrenderer.h>

PLSComplexHeaderIcon::PLSComplexHeaderIcon(QWidget *parent) : QLabel(parent), ui(new Ui::PLSComplexHeaderIcon)
{
	ui->setupUi(this);
	ui->PlatformLabel->hide();
}

PLSComplexHeaderIcon::~PLSComplexHeaderIcon()
{
	delete ui;
}

void PLSComplexHeaderIcon::setPixmap(const QString &pix)
{
	QPixmap pixmap(pix);

	circleMaskImage(pixmap);
	this->QLabel::setPixmap(pixmap);
}

void PLSComplexHeaderIcon::setPixmap(const QString &pix, const QSize &size)
{
	QPixmap pixmap(size);
	if (pix.contains(".svg")) {
		//svg handler
		QSvgRenderer svgRenderer(pix);
		pixmap.fill(Qt::transparent);
		QPainter painter(&pixmap);
		svgRenderer.render(&painter);
	} else {
		pixmap.load(pix);
	}
	if (pixmap.size().width() < 110 && pixmap.height() < 110) {
		pixmap = pixmap.scaled(QSize(300, 300), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	circleMaskImage(pixmap);
	this->QLabel::setPixmap(pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void PLSComplexHeaderIcon::setPlatformPixmap(const QString &pix, const QSize &size)
{
	QSvgRenderer svgRenderer(pix);
	QPixmap pixmap(size);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	svgRenderer.render(&painter);
	setPlatformPixmap(pixmap);
}

void PLSComplexHeaderIcon::setPlatformPixmap(const QPixmap &pix)
{

	ui->PlatformLabel->setPixmap(pix);
	ui->PlatformLabel->show();
}

void PLSComplexHeaderIcon::changeEvent(QEvent *e)
{
	QLabel::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
