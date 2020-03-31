#ifndef RTMPCHANNELVIEW_HPP
#define RTMPCHANNELVIEW_HPP

#include <QDialog>

namespace Ui {
class PLSRtmpChannelView;
}

class PLSLoginInfo;

class PLSRtmpChannelView : public QDialog {
	Q_OBJECT

public:
	explicit PLSRtmpChannelView(PLSLoginInfo *loginInfo, QJsonObject &result, QWidget *parent = nullptr);
	~PLSRtmpChannelView();
	void initUi();

protected:
	void changeEvent(QEvent *e);
private slots:
	void updateSaveBtnAvailable(const QString &);
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

private:
	void languageChange();
	void initCommbox();

private:
	Ui::PLSRtmpChannelView *ui;
	PLSLoginInfo *loginInfo;
	QJsonObject &result;
};

#endif // RTMPCHANNELVIEW_HPP
