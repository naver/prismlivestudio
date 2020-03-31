/*
* @file		PLSLiveInfoBase.h
* @brief	A base class to support moving the window in any client area
* @author	wu.longyue@navercorp.com
* @date		2020-01-06
*/

#pragma once

#include <QDialog>

#include "PLSPlatformBase.hpp"

#include "dialog-view.hpp"
#include "loading-event.hpp"

class PLSPlatformBase;

class PLSLiveInfoBase : public PLSDialogView {
	Q_OBJECT
public:
	PLSLiveInfoBase(PLSPlatformBase *pPlatformBase, QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSLiveInfoBase();

	void setPlatformBase(PLSPlatformBase *pPlatformBase) { m_pPlatformBase = pPlatformBase; }

protected:
	void updateStepTitle(QPushButton *button);

	void showLoading(QWidget *parent);
	void hideLoading();

protected:
	void closeEvent(QCloseEvent *event);

protected:
	PLSPlatformBase *m_pPlatformBase;
	PLSLoadingEvent m_loadingEvent;
	QWidget *m_pWidgetLoadingBG;
};
