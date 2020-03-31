#pragma once
#include <qmetatype.h>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QTimer>
#include <QWidget>
#include <vector>

enum class PLSScheComboxItemType {
	Ty_NormalLive,
	Ty_Loading,
	Ty_Placehoder,
	Ty_Schedule,
};

struct PLSScheComboxItemData {
	QString _id;
	QString title;
	long timeStamp = 0;
	PLSScheComboxItemType type = PLSScheComboxItemType::Ty_NormalLive;
	QString time;

	//the blow data is use for vlive
	bool needShowTimeLeftTimer = false;
	bool isVliveUpcoming = true;
	long endTimeStamp = 0;
};

Q_DECLARE_METATYPE(PLSScheComboxItemData)

using namespace std;
class PLSScheduleComboxItem;

class PLSScheduleComboxMenu : public QMenu {
	Q_OBJECT

public:
	explicit PLSScheduleComboxMenu(QWidget *parent = nullptr);
	~PLSScheduleComboxMenu();
	void setupDatas(const vector<PLSScheComboxItemData> &datas, int btnWidth);
	static QString getDetailTime(const PLSScheComboxItemData &data, bool &willDelete);

protected:
	bool eventFilter(QObject *i_Object, QEvent *i_Event);

private:
	QListWidget *m_listWidget;
	QTimer *m_pLeftTimer;

	vector<PLSScheduleComboxItem *> m_vecItems;
	void addAItem(const QString &showStr, const PLSScheComboxItemData &itemData, int superWidth);
	void setupTimer();
	void itemDidSelect(QListWidgetItem *item);
	static QString formateTheLeftTime(long leftTimeStamp);

private slots:
	void updateDetailLabel();

signals:
	void scheduleItemClicked(const QString data);
	void scheduleItemExpired(vector<QString> ids);
};

class PLSScheduleComboxItem : public QWidget {
	Q_OBJECT
public:
	explicit PLSScheduleComboxItem(const PLSScheComboxItemData data, int superWidth, QWidget *parent = nullptr);
	~PLSScheduleComboxItem();

	void setDetailLabelStr(const QString &str);
	const PLSScheComboxItemData &getData() { return _data; };

private:
	QLabel *titleLabel;
	QLabel *detailLabel;

	QLabel *loadingDetailLabel;

	const PLSScheComboxItemData _data;
	QTimer *m_pTimer;
	int m_superWidth;

	void setupTimer();
	QString PLSScheduleComboxItem::GetNameElideString();

private slots:
	void updateProgress();

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	virtual void resizeEvent(QResizeEvent *event) override;
};
