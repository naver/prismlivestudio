#include "interaction_manager.h"
#include "obs-browser-source.hpp"

//------------------------------------------------------------------------
InteractionManager *InteractionManager::Instance()
{
	static InteractionManager instance;
	return &instance;
}

InteractionManager::InteractionManager()
{
	qRegisterMetaType<SOURCE_HANDLE>("SOURCE_HANDLE");
}

InteractionManager::~InteractionManager() {}

void InteractionManager::OnSourceCreated(BrowserSource *src)
{
	std::lock_guard<std::mutex> lock(source_lock);
	source_list[src] = src;
}

void InteractionManager::OnSourceDeleted(BrowserSource *src)
{
	std::lock_guard<std::mutex> lock(source_lock);
	source_list[src] = NULL;
}

void InteractionManager::RequestHideInteraction(SOURCE_HANDLE hdl)
{
	QMetaObject::invokeMethod(this, "OnHideInteractionSlot",
				  Q_ARG(SOURCE_HANDLE, hdl));
}

void InteractionManager::OnHideInteractionSlot(SOURCE_HANDLE hdl)
{
	std::string source_name = "";

	{
		std::lock_guard<std::mutex> lock(source_lock);
		BrowserSource *src = source_list[hdl];
		if (src) {
			const char *name = obs_source_get_name(src->source);
			if (name) {
				source_name = name;
			}
		}
	}

	if (!source_name.empty()) {
		obs_source_t *temp =
			obs_get_source_by_name(source_name.c_str());
		if (temp) {
			// Here after getting source_lock, we never invoke func of BrowserSource directly.
			// Instead, we only save source_name, then release source_lock and use obs_get_source_by_name to get reference obs_source_t.
			// In this way, avoid invoking more func in source_lock.
			obs_data_t *data = obs_data_create();
			obs_data_set_string(data, "method", "HideInterct");
			obs_source_set_private_data(temp, data);
			obs_data_release(data);

			obs_source_release(temp);
			return;
		}
	}

	// Generally code here won't be invoked. However for logic safe, we also add this code.
	assert(false && "cann't find source by name");
	blog(LOG_ERROR,
	     "OnHideInteractionSlot cann't find source_t, name:%s BrowserSource:%p",
	     source_name.c_str(), hdl);

	std::lock_guard<std::mutex> lock(source_lock);
	BrowserSource *src = source_list[hdl];
	if (src) {
		src->ShowInteraction(false);
	}
}
