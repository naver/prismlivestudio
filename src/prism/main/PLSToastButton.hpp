#ifndef PLSTOASTBUTTON_H
#define PLSTOASTBUTTON_H

#include <QWidget>
#include <qlabel.h>

namespace Ui {
class PLSToastButton;
}

class PLSToastButton : public QWidget {
	Q_OBJECT

public:
	explicit PLSToastButton(QWidget *parent = nullptr);
	~PLSToastButton();
	void setNum(const int num);
	int num() const;
	QString getNumText() const;
	void setShowAlert(bool showAlert);
signals:
	void clickButton();

protected:
	void setNumLabelPositon(const double dpi);
	virtual bool eventFilter(QObject *o, QEvent *e) override;

private:
	Ui::PLSToastButton *ui;
	int m_num;
	QLabel *m_numLabel;
};

#endif // PLSTOASTBUTTON_H
