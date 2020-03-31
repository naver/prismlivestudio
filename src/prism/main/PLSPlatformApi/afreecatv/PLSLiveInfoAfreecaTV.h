#ifndef PLSLIVEINFOAFREECATV_H
#define PLSLIVEINFOAFREECATV_H

#include <QWidget>
#include "..\PLSLiveInfoBase.h"

namespace Ui {
class PLSLiveInfoAfreecaTV;
}

class PLSLiveInfoAfreecaTV : public PLSLiveInfoBase {
	Q_OBJECT

public:
	explicit PLSLiveInfoAfreecaTV(PLSPlatformBase *pPlatformBase, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSLiveInfoAfreecaTV();

private:
	Ui::PLSLiveInfoAfreecaTV *ui;

protected:
	/**
	* show the loading UI, then request the api.
	*/
	virtual void showEvent(QShowEvent *event) override;

private slots:

	void okButtonClicked();
	void cancelButtonClicked();
	void titleEdited();

private:
	void setupFirstUI();
	void saveDateWhenClickButton();
	void refreshUI();
	void doUpdateOkState();
};

#endif // PLSLIVEINFOAFREECATV_H
