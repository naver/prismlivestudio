#include <Windows.h>

#include "PLSLiveInfoBase.h"

#include <QPushButton>
#include <QHBoxLayout>

#include "pls-app.hpp"
#include "PLSPlatformApi.h"
#include "PLSLiveInfoDialogs.h"

PLSLiveInfoBase::PLSLiveInfoBase(PLSPlatformBase *pPlatformBase, QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper), m_pPlatformBase(pPlatformBase), m_pWidgetLoadingBG(nullptr)
{
	App()->DisableHotkeys();
	dpiHelper.setCss(this, {PLSCssIndex::PLSLoadingBtn, PLSCssIndex::PLSLiveInfoBase});
	dpiHelper.notifyDpiChanged(this, [=]() {
		if (m_pWidgetLoadingBG != nullptr && m_pWidgetLoadingBG->isVisible()) {
			m_pWidgetLoadingBG->setGeometry(m_pWidgetLoadingBG->parentWidget()->geometry());
		}
	});
	setHasCloseButton(false);
	setResizeEnabled(false);
}

PLSLiveInfoBase::~PLSLiveInfoBase()
{
	App()->UpdateHotkeyFocusSetting();
}

void PLSLiveInfoBase::updateStepTitle(QPushButton *button)
{
	button->setProperty("prepareLive", PLS_PLATFORM_API->isPrepareLive());
	button->style()->unpolish(button);
	button->style()->polish(button);

	if (PLS_PLATFORM_API->isPrepareLive()) {
		int iTotalSteps = PLS_PLATFORM_API->getTotalSteps();
		int iCurrStep = m_pPlatformBase->getCurrentStep();

		button->setText(iCurrStep == iTotalSteps ? QTStr("Live.Check.GoLive") : QTStr("Live.Check.Next"));

		if (iTotalSteps > 1) {
			auto title = windowTitle();
			title.append(QString("(%1/%2)").arg(iCurrStep).arg(iTotalSteps));
			setWindowTitle(title);
		}
	}
}

void PLSLiveInfoBase::showLoading(QWidget *parent)
{
	hideLoading();
	m_pWidgetLoadingBG = new QWidget(parent);
	m_pWidgetLoadingBG->setObjectName("loadingBG");
	m_pWidgetLoadingBG->setGeometry(parent->geometry());
	m_pWidgetLoadingBG->show();

	auto layout = new QHBoxLayout(m_pWidgetLoadingBG);
	auto loadingBtn = new QPushButton(m_pWidgetLoadingBG);
	layout->addWidget(loadingBtn);
	loadingBtn->setObjectName("loadingBtn");
	loadingBtn->show();

	m_loadingEvent.startLoadingTimer(loadingBtn);
}

void PLSLiveInfoBase::hideLoading()
{
	if (nullptr != m_pWidgetLoadingBG) {
		m_loadingEvent.stopLoadingTimer();

		delete m_pWidgetLoadingBG;
		m_pWidgetLoadingBG = nullptr;
	}
}

void PLSLiveInfoBase::closeEvent(QCloseEvent *event)
{
	if ((GetAsyncKeyState(VK_MENU) < 0) && (GetAsyncKeyState(VK_F4) < 0)) { // ALT+F4
		event->ignore();
	} else {
		PLSDialogView::closeEvent(event);
	}
}
