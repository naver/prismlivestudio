#include "spinbox.hpp"

#include <QWheelEvent>

PLSSpinBox::PLSSpinBox(QWidget *parent) : QSpinBox(parent)
{
	// setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
}

PLSSpinBox::~PLSSpinBox() {}

void PLSSpinBox::wheelEvent(QWheelEvent *event)
{
	event->ignore();
}

PLSDoubleSpinBox::PLSDoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{
	// setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
}
PLSDoubleSpinBox ::~PLSDoubleSpinBox() {}

void PLSDoubleSpinBox::wheelEvent(QWheelEvent *event)
{
	event->ignore();
}
