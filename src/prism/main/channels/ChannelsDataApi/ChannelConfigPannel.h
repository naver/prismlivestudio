#ifndef CHANNELCONFIGPANNEL_H
#define CHANNELCONFIGPANNEL_H

#include <QFrame>
class QPropertyAnimation;
namespace Ui {
class ChannelConfigPannel;
}

class ChannelConfigPannel : public QFrame {
	Q_OBJECT

public:
	explicit ChannelConfigPannel(QWidget *parent = nullptr);
	~ChannelConfigPannel();
	void setChannelID(const QString &channelID);
	const QString &getChannelID() { return mChannelID; }

	void updateUI();

protected:
	void changeEvent(QEvent *e) override;
	bool eventFilter(QObject *watched, QEvent *event);

private slots:
	void on_showInfoBtn_clicked();

	void on_ShareBtn_clicked();

	void on_ConfigBtn_clicked();

	void on_EnableSwitch_toggled(bool checked);

private:
	void shiftState(const QVariantMap &info);

	void doChildrenExclusive(bool &retflag);

	void checkExclusiveChannel(bool &retflag);

private:
	Ui::ChannelConfigPannel *ui;
	QString mChannelID;
};

#endif // CHANNELCONFIGPANNEL_H

void childExclusive();
