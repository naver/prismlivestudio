#pragma once

#include <QSlider>
#include <QInputEvent>
#include <QtCore/QObject>

class SliderIgnoreScroll : public QSlider {
	Q_OBJECT

public:
	explicit SliderIgnoreScroll(QWidget *parent = nullptr);
	explicit SliderIgnoreScroll(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
	virtual void wheelEvent(QWheelEvent *event) override;
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
	void SetSliderValue(const QPoint &position);

signals:
	void mouseReleaseSignal();
};
