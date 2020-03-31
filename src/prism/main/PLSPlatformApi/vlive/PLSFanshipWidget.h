#pragma once

#include <qcheckbox.h>
#include <qlabel.h>
#include <QWidget>
#include <vector>
#include "PLSPlatformVLive.h"

class PLSFanshipWidgetItem;

namespace Ui {
class PLSFanshipWidget;
}

using namespace std;

class PLSFanshipWidget : public QWidget {
	Q_OBJECT

public:
	explicit PLSFanshipWidget(QWidget *parent = nullptr);
	~PLSFanshipWidget();

	void setupDatas(const vector<PLSVLiveFanshipData> &datas, bool isSchedule = false);

	void getChecked(vector<bool> &checks);

signals:
	void shipBoxClick(int index);

private slots:
	void updateSubViewBySelectIndex(int index);

private:
	Ui::PLSFanshipWidget *ui;
	vector<PLSFanshipWidgetItem *> m_vecViews;
	vector<PLSVLiveFanshipData> m_vecDatas;
	bool m_isSchedule = false;
};

class PLSFanshipWidgetItem : public QWidget {
	Q_OBJECT
public:
	//explicit PLSFanshipWidgetItem(const PLSVLiveFanshipData data, int superWidth, QWidget *parent = nullptr);
	//~PLSFanshipWidgetItem();

	//explicit PLSFanshipWidgetItem(const PLSVLiveFanshipData data, int superWidth, QWidget *parent = nullptr);
	explicit PLSFanshipWidgetItem(const PLSVLiveFanshipData data, int superWidth, QWidget *parent = nullptr);
	//explicit PLSFanshipWidgetItem(QWidget *parent = nullptr);
	~PLSFanshipWidgetItem();

	void updateData(const PLSVLiveFanshipData &data);

	//const PLSScheComboxItemData &getData() { return _data; };

signals:
	void checkBoxClick(int index);

private:
	QCheckBox *titleBox;
	QLabel *badgeLabel;

	int m_superWidth = 0;

	//QLabel *loadingDetailLabel;

	const PLSVLiveFanshipData _data;
	void setItemEnable(bool enable);

private slots:

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};
