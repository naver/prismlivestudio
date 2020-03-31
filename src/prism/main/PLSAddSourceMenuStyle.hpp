#pragma once

#include <QProxyStyle>

class PLSAddSourceMenuStyle : public QProxyStyle {
public:
	explicit PLSAddSourceMenuStyle(double dpi, QStyle *baseStyle = Q_NULLPTR);
	virtual ~PLSAddSourceMenuStyle() {}

	virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;

private:
	double m_dpi;
};
