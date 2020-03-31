#include "PLSBeautySlider.h"

#include <QPainter>

#include "PLSDpiHelper.h"

#define SLIDER_DEFAULT_POINT_RADIUS 2
#define SLIDER_RECOMMEND_POINT_COLOR "#ffffff"
const int recommandFlagWidth = 2;
const int recommandFlagHeight = 6;
const qreal recommandFlagRadius = 1.0;

PLSBeautySlider::PLSBeautySlider(QWidget *parent) : SliderIgnoreScroll(parent) {}

void PLSBeautySlider::SetRecomendValue(const int &value)
{
	recommendValue = value;
	update();
}

void PLSBeautySlider::paintEvent(QPaintEvent *event)
{
	SliderIgnoreScroll::paintEvent(event);
	if (isEnabled() && hover) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		QPen pen(SLIDER_RECOMMEND_POINT_COLOR);
		double dpi = PLSDpiHelper::getDpi(this);
		pen.setWidthF(0.3);
		painter.setPen(pen);
		painter.setBrush(QBrush(SLIDER_RECOMMEND_POINT_COLOR));
		painter.drawRoundedRect(GetPosition(recommendValue) - PLSDpiHelper::calculate(dpi, recommandFlagWidth) / 2, (height() - PLSDpiHelper::calculate(dpi, recommandFlagHeight)) / 2,
					PLSDpiHelper::calculate(dpi, recommandFlagWidth), PLSDpiHelper::calculate(dpi, recommandFlagHeight), PLSDpiHelper::calculate(dpi, recommandFlagRadius),
					PLSDpiHelper::calculate(dpi, recommandFlagRadius));
	}
}

void PLSBeautySlider::enterEvent(QEvent *event)
{
	hover = true;
	SliderIgnoreScroll::enterEvent(event);
}

void PLSBeautySlider::leaveEvent(QEvent *event)
{
	hover = false;
	SliderIgnoreScroll::leaveEvent(event);
}

int PLSBeautySlider::GetPosition(double value)
{
	if (this->orientation() == Qt::Horizontal) {
		return value * width() / maximum();
	}
	return 0;
}
