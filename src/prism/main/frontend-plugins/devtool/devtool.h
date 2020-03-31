#ifndef PLSDEVTOOL_H
#define PLSDEVTOOL_H

#include <QMap>

#include "dialog-view.hpp"

namespace Ui {
class PLSDevTool;
}

class PLSDevTool : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSDevTool(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSDevTool();

public:
	bool canPick() const;
	bool isPtInPickButton() const;
	void pickingWidget();
	void pickWidget(bool containsSelf);
	void setParent(QWidget *parent);

private slots:
	void on_clear_clicked();
	void on_replace_clicked();
	void on_append_clicked();
	void on_reset_clicked();

protected:
	void hideEvent(QHideEvent *event) override;

private:
	Ui::PLSDevTool *ui;
	QWidget *pickedWidget = nullptr;
	QMap<QWidget *, QString> styleSheets;
	QWidget *currentWidget = nullptr;
};

#endif // PLSDEVTOOL_H
