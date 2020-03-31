#ifndef PLSOPENSOURCEVIEW_H
#define PLSOPENSOURCEVIEW_H

#include "dialog-view.hpp"
#include "PLSDpiHelper.h"
#include <browser-panel.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class PLSOpenSourceView;
}
QT_END_NAMESPACE

class PLSOpenSourceView : public PLSDialogView {
	Q_OBJECT

public:
	PLSOpenSourceView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSOpenSourceView();
	void initUI();

private slots:
	void on_confirmButton_clicked();
	void onTimeOut();

private:
	Ui::PLSOpenSourceView *ui;
	QTimer *timer;
	QCefWidget *m_cefWidget;
	QString m_updateInfoUrl;
};
#endif // PLSOPENSOURCEVIEW_H
