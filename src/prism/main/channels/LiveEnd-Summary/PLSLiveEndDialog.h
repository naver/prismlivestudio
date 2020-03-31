#ifndef PLSLIVEENDDIALOG_H
#define PLSLIVEENDDIALOG_H

#include <QVBoxLayout>
#include <dialog-view.hpp>

namespace Ui {
class PLSLiveEndDialog;
}

class PLSLiveEndDialog : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSLiveEndDialog(bool isRecord, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSLiveEndDialog();

private:
	Ui::PLSLiveEndDialog *ui;
	void setupFirstUI(PLSDpiHelper dpiHelper);
	QVBoxLayout *mChannelVBoxLayout;
	void setupScrollData(PLSDpiHelper dpiHelper);
	bool m_bRecord;
	const QString getRecordPath();

private slots:
	void okButtonClicked();
	void openFileSavePath();

protected:
	virtual void onScreenAvailableGeometryChanged(const QRect &screenAvailableGeometry) override;
};

#endif // PLSLIVEENDDIALOG_H
