/*
* @file		PLSLiveInfoTwitch.h
* @brief	To show twitch liveinfo dialog
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
*/

#pragma once

#include <QListWidget>

#include "..\PLSLiveInfoBase.h"
#include "PLSPlatformTwitch.h"

#include "ui_PLSLiveInfoTwitch.h"

namespace Ui {
class PLSLiveInfoTwitch;
}

class PLSLiveInfoTwitchListWidget : public PLSWidgetDpiAdapterHelper<QListWidget> {
	Q_OBJECT
public:
	PLSLiveInfoTwitchListWidget(QWidget *parent) : WidgetDpiAdapter(parent) {}

protected:
	virtual bool needQtProcessDpiChangedEvent() const { return false; };
};

class PLSLiveInfoTwitch : public PLSLiveInfoBase {
	Q_OBJECT
public:
	PLSLiveInfoTwitch(PLSPlatformBase *pPlatformBase, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSLiveInfoTwitch();

protected:
	virtual void showEvent(QShowEvent *event) override;
	virtual bool eventFilter(QObject *, QEvent *) override;

private:
	void refreshChannel(PLSPlatformApiResult);
	void refreshServer(PLSPlatformApiResult);
	void showListCategory();
	bool isModified();
	void titleEdited();
private slots:
	void doOk();
	void doGetCategory(QJsonObject root, const QString &request);
	void doUpdateChannel(PLSPlatformApiResult);
	void doUpdateOkState();

private:
	Ui::PLSLiveInfoTwitch *ui;

	QPushButton *pushButtonClear;
	QPushButton *pushButtonSearch;
	PLSLiveInfoTwitchListWidget *listCategory;
};
