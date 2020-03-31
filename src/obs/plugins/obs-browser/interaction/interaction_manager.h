#pragma once
#include <QObject>
#include <mutex>
#include <map>

typedef void *SOURCE_HANDLE;

struct BrowserSource;
class InteractionManager : public QObject {
	Q_OBJECT

protected:
	InteractionManager();

public:
	static InteractionManager *Instance();

	virtual ~InteractionManager();

	void OnSourceCreated(BrowserSource *src);
	void OnSourceDeleted(BrowserSource *src);
	void RequestHideInteraction(SOURCE_HANDLE hdl);

public slots:
	void OnHideInteractionSlot(SOURCE_HANDLE hdl);

private:
	// here we should not save shared-ptr
	std::map<SOURCE_HANDLE, BrowserSource *> source_list;
	std::mutex source_lock;
};
