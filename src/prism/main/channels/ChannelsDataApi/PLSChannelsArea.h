#ifndef CHANNELSAREA_H
#define CHANNELSAREA_H

#include <QFrame>
#include <QVariantMap>
#include "ChannelDefines.h"

class QPushButton;
class PLSAddingFrame;
class GoLivePannel;
class QToolButton;

namespace Ui {
class ChannelsArea;
}

class PLSChannelsArea : public QFrame {
	Q_OBJECT

public:
	explicit PLSChannelsArea(QWidget *parent = nullptr);
	~PLSChannelsArea();

	void beginInitChannels();

	void holdOnChannelArea(bool holdOn);

signals:
	void sigNextInitialize();

private slots:

	void addChannel(const QString &channelUUID);

	void removeChannel(const QString &channelUUID);

	void updateChannelUi(const QString &channelUUID);

	void refreshChannels();

	void showChannelsAdd();

	void clearAll();

	void clearAllRTMP();

	void initializeNextStep();

	void endInitialize();

protected:
	void changeEvent(QEvent *e);

	void wheelEvent(QWheelEvent *event) override;

	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	enum ScrollDirection { NOScroll, ForwardScroll, BackScroll };

private:
	ChannelData::ChannelCapsulePtr addChannel(const QVariantMap &channelInfo);

	QHash<void *, QTimer *> mTimerContainer;
	template<typename FunctionType> void delayTask(FunctionType function, int tick = 200)
	{
		auto pointer = getMemberPointer<void *>(function);
		QTimer *delayTimer = mTimerContainer.value(pointer);
		if (delayTimer == nullptr) {
			delayTimer = new QTimer(this);
			delayTimer->setSingleShot(true);
			auto func = std::bind(function, this);
			delayTimer->connect(delayTimer, &QTimer::timeout, this, [=]() { func(); });
			mTimerContainer.insert(pointer, delayTimer);
		}
		delayTimer->start(tick);
	}

	bool checkIsEmptyUi();
	void updateUi();
	void delayUpdateUi();
	void updateAllChannelsUi();
	void delayUpdateAllChannelsUi();
	void hideLoading();

	void initScollButtons();
	void checkScrollButtonsState(ScrollDirection direction = ForwardScroll);
	bool isScrollButtonsNeeded();
	void displayScrollButtons(bool isShow = true);
	void enabledScrollButtons(bool isEnabled = true);
	void scrollNext(bool forwartStep = true);
	void ensureCornerChannelVisible(bool forwartStep = true);
	void buttonLimitCheck();

	void insertChannelCapsule(QWidget *wid, int index = -1);

	void refreshOrder();

	void initializeMychannels();

	int visibleCount();

private:
	Ui::ChannelsArea *ui;

	QMap<QString, ChannelData::ChannelCapsulePtr> mChannelsWidget;

	QPushButton *mLeftButon;

	QPushButton *mRightButton;

	PLSAddingFrame *mbusyFrame;

	GoLivePannel *goliveWid;

	bool isHolding;
	bool isUiInitialized;

	QList<QVariantMap> mInitializeInfos;

	QToolButton *myChannelsIconBtn;
	QPushButton *myChannelsTxtBtn;
};

#endif // CHANNELSAREA_H
