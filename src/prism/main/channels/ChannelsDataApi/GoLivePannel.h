#ifndef GOLIVEPANNEL_H
#define GOLIVEPANNEL_H

#include <QFrame>
#include <QMenu>
#include <QStateMachine>

class PLSAddingFrame;

namespace Ui {
class GoLivePannel;
}

class GoLivePannel : public QFrame {
	Q_OBJECT

public:
	explicit GoLivePannel(QWidget *parent = nullptr);
	~GoLivePannel();
	void holdOnAll(bool holdOn);

protected:
	void changeEvent(QEvent *e);

private slots:

	void on_GoLiveShift_clicked();

	void on_Record_toggled(bool isCheck);
	void shitftRecordState(int state);

private:
	Ui::GoLivePannel *ui;
	QStateMachine mMachine;
	PLSAddingFrame *mBusyFrame;
};

#endif // GOLIVEPANNEL_H
