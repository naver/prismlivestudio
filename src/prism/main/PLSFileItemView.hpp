#ifndef PLSFILEITEMVIEW_H
#define PLSFILEITEMVIEW_H

#include <QFrame>

namespace Ui {
class PLSFileItemView;
}

class PLSFileItemView : public QFrame {
	Q_OBJECT

public:
	explicit PLSFileItemView(int index, QWidget *parent = nullptr);
	~PLSFileItemView();
	QFont fileNameLabelFont() const;
	void setFileName(const QString &filename);

	// QWidget interface
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private slots:
	void on_deleteIcon_clicked();

signals:
	void deleteItem(int index);

private:
	Ui::PLSFileItemView *ui;
	int m_index;
};

#endif // PLSFILEITEMVIEW_H
