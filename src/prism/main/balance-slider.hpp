#pragma once

#include <QSlider>
#include <QMouseEvent>

class BalanceSlider : public QSlider {
	Q_OBJECT

public:
	explicit inline BalanceSlider(QWidget *parent = 0) : QSlider(parent) {}

signals:
	void doubleClicked();

protected:
	void mouseDoubleClickEvent(QMouseEvent *event)
	{
		emit doubleClicked();
		event->accept();
	}
	void mousePressEvent(QMouseEvent *event)
	{
		QSlider::mousePressEvent(event);
		if (this->orientation() == Qt::Horizontal) {
			double pos = event->pos().x() / (double)width();
			setValue(pos * (maximum() - minimum()) + minimum());
		} else {
			double pos = event->pos().y() / (double)height();
			setValue((1 - pos) * (maximum() - minimum()) + minimum());
		}
	}
};
