#ifndef PLSTOASTMSGPOPUP_H
#define PLSTOASTMSGPOPUP_H

#include <QLabel>
#include <QTimer>
#include <QWidget>
#include "PLSLivingMsgView.hpp"
#include <QMouseEvent>
#include "PLSDpiHelper.h"

namespace Ui {
class PLSToastMsgPopup;
}

class PLSToastMsgPopup : public QLabel {
	Q_OBJECT

public:
	explicit PLSToastMsgPopup(QWidget *parent = nullptr);
	~PLSToastMsgPopup();

	void showMsg(const QString &msg, pls_toast_info_type type);

protected:
	void mousePressEvent(QMouseEvent *event);

private:
	Ui::PLSToastMsgPopup *ui;
	QTimer *m_timer;
};

#endif // PLSTOASTMSGPOPUP_H
