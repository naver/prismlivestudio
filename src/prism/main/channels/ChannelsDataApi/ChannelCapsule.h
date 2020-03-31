#ifndef CHANNELCAPSULE_H
#define CHANNELCAPSULE_H

#include <QFrame>
#include <QStateMachine>
#include <QTimer>
#include <QVariantMap>
class QLabel;
class ChannelConfigPannel;

namespace Ui {
class ChannelCapsule;
}

class ChannelCapsule : public QFrame {
	Q_OBJECT
public:
	explicit ChannelCapsule(QWidget *parent = nullptr);
	~ChannelCapsule();
	/*init ui and state */
	void setChannelID(const QString &uuid);
	/*update ui */
	void updateUi();

	/*get if is active by user */
	bool isOnLine();
	void setOnLine(bool isActive = true);

	bool isSelectedDisplay();

	const QString &getChannelID() { return mInfoID; }

protected:
	void changeEvent(QEvent *e);
	/* to show config pannel*/
	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	/* update viewers and likes */
	bool updateStatisticInfo();
	/*start update viewers and likes */
	bool switchUpdateStatisticInfo(bool on = true);

	void updateTextFrames(const QVariantMap &srcData);
	void delayUpdateText();
	void updateIcons(const QVariantMap &srcData);

	void shiftState(const QVariantMap &srcData);

	void RTMPTypeState(const QVariantMap &info);
	void channelTypeState(const QVariantMap &info);
	void normalState(const QVariantMap &info);

	void errorState(const QVariantMap &info);
	void unInitializeState(const QVariantMap &info);
	void waitingState(const QVariantMap &info);
	void updateErrorLabel(const QVariantMap &info);

	bool isPannelOutOfView();

	void initializeConfigPannel();

private slots:
	void showConfigPannel();
	void hideConfigPannel();

private:
	Ui::ChannelCapsule *ui;

	QString mInfoID;
	ChannelConfigPannel *mConfigPannel;
	QTimer mUpdateTimer;

	QVariantMap mLastMap;
};

#endif // CHANNELCAPSULE_H
