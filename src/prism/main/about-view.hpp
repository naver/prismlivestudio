#ifndef PLSABOUTVIEW_HPP
#define PLSABOUTVIEW_HPP

#include <dialog-view.hpp>
#include "PLSDpiHelper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PLSAboutView;
}
QT_END_NAMESPACE

class PLSAboutView : public PLSDialogView {
	Q_OBJECT

public:
	PLSAboutView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSAboutView();

private slots:
	void on_checkUpdateButton_clicked();

private:
	Ui::PLSAboutView *ui;

private:
	void initConnect();
};
#endif // PLSABOUTVIEW_HPP
