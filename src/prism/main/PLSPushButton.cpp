#include "PLSPushButton.h"

void PLSPushButton::resizeEvent(QResizeEvent *event)
{
	this->setText(GetNameElideString());

	QPushButton::resizeEvent(event);
}

QString PLSPushButton::GetNameElideString()
{
	QFontMetrics fontWidth(this->font());
	if (fontWidth.width(text) > this->width())
		return fontWidth.elidedText(text, Qt::ElideRight, this->width());
	return text;
}

void PLSPushButton::SetText(const QString &text)
{
	this->text = text;
	this->setText(GetNameElideString());
}
