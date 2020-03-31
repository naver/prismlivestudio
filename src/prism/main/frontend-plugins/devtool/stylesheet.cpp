#include <QWidget>

#include <private/qstylesheetstyle_p.h>
#include <private/qcssparser_p.h>

class QStyleSheetStylePrivate {
public:
	static QVector<QCss::StyleRule> from(QWidget *widget, QStyleSheetStyle *style) { return style->styleRules(widget); }
};

QVector<QCss::StyleRule> getStyleRules(QWidget *widget, QStyleSheetStyle *style)
{
	return QStyleSheetStylePrivate::from(widget, style);
}
