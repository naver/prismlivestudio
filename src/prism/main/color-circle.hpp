#pragma once

#include <QWidget>

class PLSColorCircle : public QWidget {
	Q_OBJECT
	Q_PROPERTY(QColor bkgColor READ getBkgColor WRITE setBkgColor)

protected:
	void paintEvent(QPaintEvent *event);

public:
	explicit inline PLSColorCircle(QWidget *parent = nullptr) : QWidget(parent) {}

public:
	QColor getBkgColor() const;
	void setBkgColor(const QColor &color);

private:
	QColor bkgColor;
};
