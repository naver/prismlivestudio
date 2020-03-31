#ifndef PLSFACEBOOKGAMELINEEDIT_H
#define PLSFACEBOOKGAMELINEEDIT_H

#include <QLineEdit>
#include <QListWidget>

class PLSLoadingComboxMenu;
namespace Ui {
class PLSFacebookGameLineEdit;
}

class PLSFacebookGameLineEdit : public QLineEdit {
	Q_OBJECT

public:
	explicit PLSFacebookGameLineEdit(QWidget *parent = nullptr);
	~PLSFacebookGameLineEdit();

protected:
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);

signals:
	void searchKeyword(const QString);
	void clearText();

private:
	Ui::PLSFacebookGameLineEdit *ui;
};

#endif // PLSFACEBOOKGAMELINEEDIT_H
