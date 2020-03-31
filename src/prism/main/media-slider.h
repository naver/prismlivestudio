#pragma once
#include <QSlider>
#include <QMouseEvent>
#include "double-slider.hpp"

class MediaSlider : public SliderIgnoreScroll {
	Q_OBJECT

public:
	inline MediaSlider(QWidget *parent = nullptr) : SliderIgnoreScroll(parent) { setMouseTracking(true); };

	void setMediaSliderEnabled(bool isDisabled);
	bool isMediaSliderEnabled();
signals:
	void mediaSliderHovered(int value);
	void mediaSliderReleased(int value);
	void mediaSliderClicked(int value);
	void mediaSliderMoved(int value);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	virtual void wheelEvent(QWheelEvent *event) override;
	virtual void keyPressEvent(QKeyEvent *event) override;

private:
	bool mousePress = false;
	int prePos = 0;
	bool enabled = false;
	bool mouseEnter = false;
};
