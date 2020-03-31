#ifndef PLSADDINGFRAME_H
#define PLSADDINGFRAME_H

#include <QFrame>
#include <QTimer>

namespace Ui {
class PLSAddingFrame;
}

class PLSAddingFrame : public QFrame {
	Q_OBJECT

public:
	explicit PLSAddingFrame(QWidget *parent = nullptr);
	~PLSAddingFrame();

	void setSourceFirstFile(const QString &sfile);
	void setContent(const QString &txt);
	void start(int time = 300);
	void stop();

protected:
	void changeEvent(QEvent *e);

private:
	void nextFrame();

private:
	Ui::PLSAddingFrame *ui;
	QTimer mUpdateTimer;
	QString mSourceFile;
	int mStep;
};

#endif // PLSADDINGFRAME_H
