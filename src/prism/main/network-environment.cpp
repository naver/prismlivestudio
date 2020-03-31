#include "network-environment.hpp"
#include "pls-common-define.hpp"
#include <QEventLoop>
#include "log/log.h"
#include "log/module_names.h"

NetworkEnvironment::NetworkEnvironment(QObject *parent) : QObject(parent), m_isOnline(false) {}

NetworkEnvironment::~NetworkEnvironment() {}

void NetworkEnvironment::checkNetworkOnline()
{
	QHostInfo::lookupHost(NETWORK_CHECK_URL, this, SLOT(onConnectHost(QHostInfo)));
}

bool NetworkEnvironment::getNetWorkEnvironment()
{
	QEventLoop loop;
	connect(this, &NetworkEnvironment::networkEnvironmentState, this, [&](bool) { loop.exit(); });

	checkNetworkOnline();
	loop.exec();
	return m_isOnline;
}

void NetworkEnvironment::onConnectHost(QHostInfo host)
{
	if (host.error() != QHostInfo::NoError) {
		m_isOnline = false;
		PLS_WARN(NETWORK_ENVIRONMENT, "network error has occurred");
	} else {
		m_isOnline = true;
	}
	emit networkEnvironmentState(m_isOnline);
}
