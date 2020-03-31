#include "PLSNetworkMonitor.h"
#include "liblog.h"
#include "log/module_names.h"

#ifdef WIN32
#pragma comment(lib, "ole32.lib")

class CNetWorkEvent : public INetworkListManagerEvents {
	friend class PLSNetworkMonitor;

	enum E_NETWORK_STATUS {
		NET_UNINIT = 0,
		NET_CONNECTED,
		NET_DISCONNECT,
	};

public:
	CNetWorkEvent(PLSNetworkMonitor &monitor) : networkMonitor(monitor) {}
	virtual ~CNetWorkEvent() {}

public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	virtual HRESULT STDMETHODCALLTYPE ConnectivityChanged(NLM_CONNECTIVITY newConnectivity);

private:
	LONG comReference = 1;
	E_NETWORK_STATUS networkState = NET_UNINIT;
	PLSNetworkMonitor &networkMonitor;
};

//------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CNetWorkEvent::QueryInterface(REFIID riid, void **ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_IUnknown)) {
		AddRef();
		*ppvObject = (IUnknown *)this;
		return S_OK;

	} else if (IsEqualIID(riid, IID_INetworkListManagerEvents)) {
		AddRef();
		*ppvObject = (INetworkListManagerEvents *)this;
		return S_OK;

	} else {
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
}

ULONG STDMETHODCALLTYPE CNetWorkEvent::AddRef(void)
{
	return (ULONG)InterlockedIncrement(&comReference);
}

ULONG STDMETHODCALLTYPE CNetWorkEvent::Release(void)
{
	LONG Result = InterlockedDecrement(&comReference);
	if (Result == 0) {
		delete this;
	}
	return (ULONG)Result;
}

static bool IsInternetOK(NLM_CONNECTIVITY nlmConnectivity)
{
	if ((nlmConnectivity & NLM_CONNECTIVITY_IPV4_INTERNET) || (nlmConnectivity & NLM_CONNECTIVITY_IPV6_INTERNET)) {
		return true;
	} else {
		return false;
	}
}

HRESULT STDMETHODCALLTYPE CNetWorkEvent::ConnectivityChanged(NLM_CONNECTIVITY newConnectivity)
{
	bool bNetOK = IsInternetOK(newConnectivity);
	E_NETWORK_STATUS st = bNetOK ? NET_CONNECTED : NET_DISCONNECT;

	if (networkState != st) {
		networkState = st;
		QMetaObject::invokeMethod(&networkMonitor, "OnNotifyNetworkState", Qt::QueuedConnection, Q_ARG(bool, bNetOK));
	}

	return S_OK;
}

//----------------------------------------------------------------
PLSNetworkMonitor *PLSNetworkMonitor::Instance()
{
	static PLSNetworkMonitor networkMonitor;
	return &networkMonitor;
}

PLSNetworkMonitor::~PLSNetworkMonitor()
{
	StopListen();
}

bool PLSNetworkMonitor::StartListen()
{
	do {
		HRESULT hr = CoCreateInstance(CLSID_NetworkListManager, NULL, CLSCTX_ALL, IID_INetworkListManager, (LPVOID *)&networkListManager);
		if (FAILED(hr)) {
			PLS_WARN(NETWORK_ENVIRONMENT, "Failed to init networkListManager, 0X%X", hr);
			break;
		}

		hr = networkListManager->QueryInterface(IID_IConnectionPointContainer, (void **)&connectContainer);
		if (FAILED(hr)) {
			PLS_WARN(NETWORK_ENVIRONMENT, "Failed to init connectContainer, 0X%X", hr);
			break;
		}

		hr = connectContainer->FindConnectionPoint(IID_INetworkListManagerEvents, &connectPoint);
		if (FAILED(hr)) {
			PLS_WARN(NETWORK_ENVIRONMENT, "Failed to init connectPoint, 0X%X", hr);
			break;
		}

		networkEvent = new CNetWorkEvent(*this);
		hr = connectPoint->Advise((IUnknown *)networkEvent, &adviseCookie);
		if (FAILED(hr)) {
			PLS_WARN(NETWORK_ENVIRONMENT, "Failed to invoke Advise(), 0X%X", hr);
			break;
		}

		NLM_CONNECTIVITY nlmConnectivity;
		if (S_OK == networkListManager->GetConnectivity(&nlmConnectivity)) {
			internetAvailable = IsInternetOK(nlmConnectivity);
		}

		PLS_INFO(NETWORK_ENVIRONMENT, "Successed to init PLSNetworkMonitor. Internet state : %s", internetAvailable ? "CONNECTED" : "DISCONNECTED");
		networkInited = true;
		return true;

	} while (false);

	StopListen();
	return false;
}

void PLSNetworkMonitor::StopListen()
{
#define SAFE_RELEASE_COM(x)   \
	if (x) {              \
		x->Release(); \
		x = NULL;     \
	}

	bool showLog = (adviseCookie != 0);
	if (showLog) {
		PLS_INFO(NETWORK_ENVIRONMENT, "To clear PLSNetworkMonitor");
	}

	if (connectPoint && adviseCookie != 0) {
		PLS_INFO(NETWORK_ENVIRONMENT, "Before invoke Unadvise()");
		connectPoint->Unadvise(adviseCookie);
		adviseCookie = 0;
		PLS_INFO(NETWORK_ENVIRONMENT, "Finish invoke Unadvise()");
	}

	SAFE_RELEASE_COM(networkEvent);
	SAFE_RELEASE_COM(connectPoint);
	SAFE_RELEASE_COM(connectContainer);
	SAFE_RELEASE_COM(networkListManager);

	if (showLog) {
		PLS_INFO(NETWORK_ENVIRONMENT, "Finish clear PLSNetworkMonitor");
	}
}

bool PLSNetworkMonitor::IsInternetAvailable()
{
	assert(networkInited && "PLSNetworkMonitor is not inited !");
	return internetAvailable;
}

void PLSNetworkMonitor::OnNotifyNetworkState(bool isConnected)
{
	PLS_INFO(NETWORK_ENVIRONMENT, "Internet state is changed : %s", isConnected ? "CONNECTED" : "DISCONNECTED");
	internetAvailable = isConnected;
	emit OnNetWorkStateChanged(isConnected);
}

#endif // WIN32
