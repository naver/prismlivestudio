#ifndef PLSTOASTMSGFRAME_H
#define PLSTOASTMSGFRAME_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QTextEdit>

class PLSToastMsgFrame : public QFrame {
	Q_OBJECT
public:
	PLSToastMsgFrame(QWidget *parent = nullptr);
	~PLSToastMsgFrame() override;

	void SetMessage(const QString &message);
	QString GetMessage() const;
	void SetShowWidth(int width);
	void ShowToast();
	void HideToast();

protected:
	virtual void resizeEvent(QResizeEvent *event) override;

private slots:
	void resizeToContent();

signals:
	void resizeFinished();

private:
	QPushButton *btnClose = nullptr;
	QTextEdit *editMessage = nullptr;
	int showWidth{0};
	QTimer timerDisappear;
};

#endif // PLSTOASTMSGFRAME_H
