#pragma once

#include <QSpinBox>

class PLSSpinBox : public QSpinBox {
	Q_OBJECT

public:
	explicit PLSSpinBox(QWidget *parent = nullptr);
	~PLSSpinBox();

protected:
	virtual void wheelEvent(QWheelEvent *event) override;
};

class PLSDoubleSpinBox : public QDoubleSpinBox {
	Q_OBJECT

public:
	explicit PLSDoubleSpinBox(QWidget *parent = nullptr);
	~PLSDoubleSpinBox();

protected:
	virtual void wheelEvent(QWheelEvent *event) override;
};
