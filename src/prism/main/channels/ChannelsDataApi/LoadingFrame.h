#ifndef LOADINGFRAME_H
#define LOADINGFRAME_H

#include <QFrame>
#include <QImage>
#include <QPixmap>
#include <QTimer>

/* this is UI for busy view while requesting */
namespace Ui {
class LoadingFrame;
}

class LoadingFrame : public QFrame {
	Q_OBJECT

public:
	explicit LoadingFrame(QWidget *parent = nullptr);
	~LoadingFrame();
	void initialize(const QString &file, int tick = 300);
public slots:

	void setImage(const QString &file);
	void setTick(int mssecond = 300);
	void next();
	void start();

	void setTitleVisible(bool isVisible = true);

protected:
	void changeEvent(QEvent *e);
	void paintEvent(QPaintEvent *event) override;

private:
	Ui::LoadingFrame *ui;
	QTimer mTimer;
	int mTick;
	QImage mPic;
	int mStepCount;
};

#endif // LOADINGFRAME_H
