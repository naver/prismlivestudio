#ifndef CHANNELSADDWIN_H
#define CHANNELSADDWIN_H

#include <QFrame>
#include "PLSWidgetDpiAdapter.hpp"

class QListWidgetItem;
class QListWidget;

namespace Ui {
class ChannelsAddWin;
}

class ChannelsAddWin : public PLSWidgetDpiAdapterHelper<QFrame> {
	Q_OBJECT

public:
	explicit ChannelsAddWin(QWidget *parent = nullptr);
	~ChannelsAddWin();
	void updateUi();

protected:
	void changeEvent(QEvent *e);
	bool eventFilter(QObject *watched, QEvent *event);
	void appendItem(const QString &text);
private slots:
	void runBtnCMD();
	void on_ClosePtn_clicked();

private:
	void initDefault();
	void updateItem(int index);

private:
	Ui::ChannelsAddWin *ui;
};

#endif // CHANNELSADDWIN_H
