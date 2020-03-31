#ifndef NETWORKENVIRONMENT_H
#define NETWORKENVIRONMENT_H

#include <QObject>
#include <QHostInfo>

class NetworkEnvironment : public QObject {
	Q_OBJECT
public:
	explicit NetworkEnvironment(QObject *parent = nullptr);
	~NetworkEnvironment();
	bool getNetWorkEnvironment();

private:
	void checkNetworkOnline();

private slots:
	void onConnectHost(QHostInfo host);
signals:
	void networkEnvironmentState(bool);

private:
	bool m_isOnline;
};

#endif // NETWORKENVIRONMENT_H
