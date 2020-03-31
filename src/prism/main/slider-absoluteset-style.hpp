#pragma once

#include <QProxyStyle>

class SliderAbsoluteSetStyle : public QProxyStyle {
public:
	explicit SliderAbsoluteSetStyle(const QString &baseStyle);
	explicit SliderAbsoluteSetStyle(QStyle *baseStyle = Q_NULLPTR);
	int styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const;
};
