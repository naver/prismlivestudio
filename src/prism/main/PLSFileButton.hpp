#ifndef PLSFILEBUTTON_H
#define PLSFILEBUTTON_H

#include <QPushButton>

namespace Ui {
class PLSFileButton;
}

class PLSFileButton : public QPushButton {
	Q_OBJECT
public:
	explicit PLSFileButton(QWidget *parent = nullptr);
	~PLSFileButton();
	void setFileButtonEnabled(bool enabled);

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	Ui::PLSFileButton *ui;
	bool m_enabled;
};

#endif // PLSFILEBUTTON_H
