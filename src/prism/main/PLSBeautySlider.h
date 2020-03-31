#ifndef PLSBEAUTYSLIDER_H
#define PLSBEAUTYSLIDER_H

#include "slider-ignorewheel.hpp"

class PLSBeautySlider : public SliderIgnoreScroll {
public:
	PLSBeautySlider(QWidget *parent = nullptr);
	void SetRecomendValue(const int &value);

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;

private:
	int GetPosition(double value);

private:
	bool hover{false};
	int recommendValue{0};
};

#endif // PLSBEAUTYSLIDER_H
