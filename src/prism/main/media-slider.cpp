#include "media-slider.h"
#include "pls-common-define.hpp"
#include <QStyle>

void MediaSlider::mouseMoveEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::LeftButton) && mousePress) {
		mousePress = false;
	}

	int val = minimum() + ((maximum() - minimum()) * event->x()) / width();

	if (val > maximum())
		val = maximum();
	else if (val < minimum())
		val = minimum();

	if (mousePress && prePos != val) {
		emit mediaSliderMoved(val);
		prePos = val;
	} else
		emit mediaSliderHovered(val);

	event->accept();
	QSlider::mouseMoveEvent(event);
}

void MediaSlider::mousePressEvent(QMouseEvent *event)
{
	if (!isMediaSliderEnabled())
		return;

	if (event->button() == Qt::LeftButton) {
		int val = minimum() + ((maximum() - minimum()) * event->x()) / width();

		if (val > maximum())
			val = maximum();
		else if (val < minimum())
			val = minimum();

		prePos = val;
		emit mediaSliderClicked(val);
		event->accept();
	}
	mousePress = true;
}

void MediaSlider::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		int val = minimum() + ((maximum() - minimum()) * event->x()) / width();

		if (val > maximum())
			val = maximum();
		else if (val < minimum())
			val = minimum();

		emit mediaSliderReleased(val);
		event->accept();
	}

	mousePress = false;
	QSlider::mouseReleaseEvent(event);
}

void MediaSlider::enterEvent(QEvent *event)
{
	QSlider::enterEvent(event);
	if (isEnabled()) {
		setProperty(STATUS_ENTER, true);
		style()->unpolish(this);
		style()->polish(this);
	}
	mouseEnter = true;
}

void MediaSlider::leaveEvent(QEvent *event)
{
	QSlider::enterEvent(event);
	setProperty(STATUS_ENTER, false);
	style()->unpolish(this);
	style()->polish(this);
	mouseEnter = false;
}

void MediaSlider::wheelEvent(QWheelEvent *event)
{
	QWidget::wheelEvent(event);
}

void MediaSlider::keyPressEvent(QKeyEvent *event)
{
	if (hasFocus())
		event->ignore();
	else
		QSlider::keyPressEvent(event);
}

void MediaSlider::setMediaSliderEnabled(bool isEnabled)
{
	enabled = isEnabled;
	if (mouseEnter && isEnabled) {
		setProperty(STATUS_ENTER, true);
		style()->unpolish(this);
		style()->polish(this);
	}
}

bool MediaSlider::isMediaSliderEnabled()
{
	return enabled;
}
