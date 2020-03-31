#include "color-circle.hpp"

#include <QPainter>

void PLSColorCircle::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect rc = rect();
	painter.setRenderHint(QPainter::HighQualityAntialiasing);
	painter.setBackground(Qt::transparent);
	painter.setBrush(bkgColor);
	painter.setPen(bkgColor);
	painter.drawEllipse(rc);
}

QColor PLSColorCircle::getBkgColor() const
{
	return bkgColor;
}

void PLSColorCircle::setBkgColor(const QColor &color)
{
	this->bkgColor = color;
	update();
}
