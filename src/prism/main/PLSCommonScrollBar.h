#ifndef PLSCOMMONSCROLLBAR_H
#define PLSCOMMONSCROLLBAR_H

#include <QObject>
#include <qscrollbar.h>

class PLSCommonScrollBar : public QScrollBar {
	Q_OBJECT
public:
	PLSCommonScrollBar(QWidget *parent = nullptr);
	~PLSCommonScrollBar();

	// QWidget interface
protected:
	void showEvent(QShowEvent *event);

	// QWidget interface
protected:
	void hideEvent(QHideEvent *event);
signals:
	void isShowScrollBar(bool isShow);
};

#endif // PLSCOMMONSCROLLBAR_H
