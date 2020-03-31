#ifndef PLSLOADINGMENUITEM_H
#define PLSLOADINGMENUITEM_H

#include <QWidget>
#include "PLSDpiHelper.h"

namespace Ui {
class PLSLoadingMenuItem;
}

class PLSLoadingMenuItem : public QWidget {
	Q_OBJECT

public:
	explicit PLSLoadingMenuItem(QWidget *parent = nullptr);
	~PLSLoadingMenuItem();
	void setTitle(const QString &title);
	void setSelected(bool selected);
	bool eventFilter(QObject *watched, QEvent *event);

private:
	Ui::PLSLoadingMenuItem *ui;
	QString m_title;
};

#endif // PLSLOADINGMENUITEM_H
