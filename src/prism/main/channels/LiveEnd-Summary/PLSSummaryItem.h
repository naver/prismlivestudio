#ifndef PLSSUMMARYITEM_H
#define PLSSUMMARYITEM_H

#include <QFrame>

namespace Ui {
class PLSSummaryItem;
}

class PLSSummaryItem : public QFrame {
	Q_OBJECT

public:
	explicit PLSSummaryItem(const QMap<QString, QVariant> &source, QWidget *parent = nullptr);
	~PLSSummaryItem();

private:
	Ui::PLSSummaryItem *ui;

	void setupData();
	void setupDetailLabelData(QString platform);
	void combineTwoImage();

	const QMap<QString, QVariant> &mSourceData;
	QPixmap pixmapToRound(QPixmap &src, int radius);
};

#endif // PLSSUMMARYITEM_H
