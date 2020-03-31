#ifndef PLSSCHELIVENOTICE_H
#define PLSSCHELIVENOTICE_H

#include "dialog-view.hpp"

namespace Ui {
class PLSScheLiveNotice;
}

class PLSPlatformBase;

class PLSScheLiveNotice : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSScheLiveNotice(PLSPlatformBase *platform, const QString &title, const QString &startTime, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSScheLiveNotice();

private slots:
	void on_okButton_clicked();

private:
	Ui::PLSScheLiveNotice *ui;
};

#endif // PLSSCHELIVENOTICE_H
