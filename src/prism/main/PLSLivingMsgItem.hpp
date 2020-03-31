#ifndef PLSLIVINGMSGITEM_H
#define PLSLIVINGMSGITEM_H

#include <QWidget>
#include "PLSLivingMsgView.hpp"
#include "frontend-api.h"

namespace Ui {
class PLSLivingMsgItem;
}

class PLSLivingMsgItem : public QWidget {
	Q_OBJECT

public:
	explicit PLSLivingMsgItem(const QString &info, const long long time, pls_toast_info_type type, QWidget *parent = nullptr);
	~PLSLivingMsgItem();

	void setMsgInfo(const QString &info);
	QString getMsgInfo() const;
	void setTimes(long long time);
	qint64 getTimes() const;
	void setMsgType(pls_toast_info_type type);
	pls_toast_info_type getMsgType() const;

	void updateTimeView(const qint64 currentTime);

private:
	void setToastIcon(pls_toast_info_type type);

private:
	Ui::PLSLivingMsgItem *ui;
	QString m_msgInfo;
	long long m_times;
	pls_toast_info_type m_type;
};

#endif // PLSLIVINGMSGITEM_H
