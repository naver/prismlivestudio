#ifndef PLSSCROLLINGLABEL_H
#define PLSSCROLLINGLABEL_H

#include <QLabel>

class PLSScrollingLabel : public QLabel {
	Q_OBJECT
public:
	PLSScrollingLabel(QWidget *parent = nullptr);
	~PLSScrollingLabel();
	void SetText(const QString &text);

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void timerEvent(QTimerEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;

protected slots:
	void handleScorllingText();

private:
	void StartTimer(const int interval = 100);
	void StopTimer();
	void UpateRollingStatus();

private:
	int timerId{-1};
	int left{0};
	QString text{};
};

#endif // PLSSCROLLINGLABEL_H
