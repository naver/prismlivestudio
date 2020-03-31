#ifndef CHANNELITEM_H
#define CHANNELITEM_H

#include <QPushButton>
#include <QVariantMap>

namespace Ui {
class ChannelItem;
}

class ChannelItem : public QPushButton {
	Q_OBJECT

public:
	explicit ChannelItem(QWidget *parent = nullptr);
	~ChannelItem();
	void setData(const QVariantMap &data);

signals:
	void sigSelectionChanged(const QString &uuid, bool isChecked);

protected:
	void changeEvent(QEvent *e) override;
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void on_checkBox_toggled(bool checked);
	void onSelectStateChanged(bool checked);
	void updateTextLabel();

private:
	Ui::ChannelItem *ui;
	QVariantMap mLastData;
	QString mLastUUid;
};

#endif // CHANNELITEM_H
