#include "loading-event.hpp"

#include <QTimerEvent>
#include <QWidget>
#include <QVariant>

#include "login-common-helper.hpp"

PLSLoadingEvent *PLSLoadingEvent::instance()
{
	static PLSLoadingEvent loadEvent;
	return &loadEvent;
}
PLSLoadingEvent::PLSLoadingEvent() : m_loadingTimer(INTERNAL_ERROR), m_loadingPictureNums(0), m_loadingButton(nullptr) {}

PLSLoadingEvent::~PLSLoadingEvent() {}

void PLSLoadingEvent::startLoadingTimer(QWidget *loadingButton, const int timeout)
{
	m_loadingButton = loadingButton;
	if (INTERNAL_ERROR != m_loadingTimer) {
		return;
	}
	m_loadingTimer = this->startTimer(timeout);
	emit startLoading();
}

void PLSLoadingEvent::stopLoadingTimer()
{
	if (INTERNAL_ERROR != m_loadingTimer) {
		killTimer(m_loadingTimer);
		m_loadingTimer = INTERNAL_ERROR;
	}
	emit stopLoading();
}

void PLSLoadingEvent::timerEvent(QTimerEvent *e)
{
	if (e->timerId() == m_loadingTimer) {
		handlePLSLoadingEvent();
	}
}

void PLSLoadingEvent::handlePLSLoadingEvent()
{
	if (m_loadingPictureNums >= LOADING_PICTURE_MAX_NUMBER) {
		m_loadingPictureNums = 0;
	}

	m_loadingButton->setProperty(STATUS, QVariant(++m_loadingPictureNums));
	LoginCommonHelpers::refreshStyle(m_loadingButton);
}
