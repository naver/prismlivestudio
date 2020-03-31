#ifndef PLSLOADINGTITLEITEM_H
#define PLSLOADINGTITLEITEM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class PLSLoadingTitleItem;
}

class PLSLoadingTitleItem : public QWidget {
	Q_OBJECT

public:
	explicit PLSLoadingTitleItem(QWidget *parent = nullptr);
	~PLSLoadingTitleItem();

private:
	void setupTimer();

private slots:
	void updateProgress();

private:
	Ui::PLSLoadingTitleItem *ui;
	QTimer *m_timer;
};

#endif // PLSLOADINGTITLEITEM_H
