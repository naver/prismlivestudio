#ifndef PLSSUMMARYDIALOG_H
#define PLSSUMMARYDIALOG_H

#include <QVBoxLayout>
#include <dialog-view.hpp>

namespace Ui {
class PLSSummaryDialog;
}

class PLSSummaryDialog : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSSummaryDialog(const QMap<QString, QVariantMap> &sourceLst, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSSummaryDialog();

private:
	Ui::PLSSummaryDialog *ui;
	void setupFirstUI();
	void setupData();
	void setupScrollData();
	void setupDetailData();
	QVBoxLayout *mChannelVBoxLayout;
	const QMap<QString, QVariantMap> &mChannelsLst;

private slots:
	void okButtonClicked();
	void cancelButtonClicked();
};

#endif // PLSSUMMARYDIALOG_H
