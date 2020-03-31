#ifndef DEFAULTPLATFORMSADDLIST_H
#define DEFAULTPLATFORMSADDLIST_H

#include <QFrame>

namespace Ui {
class DefaultPlatformsAddList;
}

class DefaultPlatformsAddList : public QFrame {
	Q_OBJECT

public:
	explicit DefaultPlatformsAddList(QWidget *parent = nullptr);
	~DefaultPlatformsAddList();

public slots:
	void runBtnCMD();

protected:
	void changeEvent(QEvent *e);
	bool eventFilter(QObject *watched, QEvent *event);

private:
	void initUi();

private:
	Ui::DefaultPlatformsAddList *ui;
};

#endif // DEFAULTPLATFORMSADDLIST_H
