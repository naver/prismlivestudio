#pragma once
#ifdef WIN32
#include <windows.h>
#include <netlistmgr.h>
#include <QObject>

class PLSNetworkMonitor : public QObject {
	Q_OBJECT

protected:
	PLSNetworkMonitor() {}
	virtual ~PLSNetworkMonitor();

public:
	static PLSNetworkMonitor *Instance();

	bool StartListen();
	void StopListen();
	bool IsInternetAvailable();

signals:
	void OnNetWorkStateChanged(bool isConnected);

public slots:
	void OnNotifyNetworkState(bool isConnected);

private:
	INetworkListManager *networkListManager = NULL;
	IConnectionPointContainer *connectContainer = NULL;
	IConnectionPoint *connectPoint = NULL;
	INetworkListManagerEvents *networkEvent = NULL;
	DWORD adviseCookie = 0;

	bool internetAvailable = true;
	bool networkInited = false;
};

#endif // WIN32
