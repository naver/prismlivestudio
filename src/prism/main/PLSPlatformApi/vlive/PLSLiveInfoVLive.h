#pragma once

#include <QCheckBox>
#include <QWidget>
#include <vector>
#include "..\PLSLiveInfoBase.h"
#include "PLSScheduleCombox.h"

namespace Ui {
class PLSLiveInfoVLive;
}

class PLSLiveInfoVLive : public PLSLiveInfoBase {
	Q_OBJECT

public:
	explicit PLSLiveInfoVLive(PLSPlatformBase *pPlatformBase, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSLiveInfoVLive();

protected:
	/**
	* show the loading UI, then request the api.
	*/
	virtual void showEvent(QShowEvent *event) override;

private:
	Ui::PLSLiveInfoVLive *ui;
	void setupFirstUI();
	vector<PLSScheComboxItemData> m_vecItemDatas;
	void refreshUI();
	void refreshTitleEdit();
	void refreshThumbnailButton();

	QString m_enteredID;

	/**
	* check whether the ui is changed,
	* for example, the user input another title
	*/
	bool isModified();
private slots:

	void okButtonClicked();
	void cancelButtonClicked();
	void rehearsalButtonClicked();

	void fanshipButtonClicked(int index);

	void saveDateWhenClickButton();

	void scheduleButtonClicked();
	void scheduleItemClick(const QString selectID);
	void scheduleItemExpired(vector<QString> &ids);

	void titleEdited();

	void refreshSchedulePopButton();

	void updateScheduleComboxItems();
	/**
	* check whether the "ok" button can be enabled.
	* 1. if some api request is failed, "ok" button can't be enabled.
	* 2. if some required content is empty, same as 1.
	* 3. if user didn't modify any content, same as 1.
	*/
	void doUpdateOkState();

	void refreshFanshipView();

	void onImageSelected(const QString &imageFilePath);

	void saveTempNormalDataWhenSwitch();

private:
	void updateUIEnabel();
};
