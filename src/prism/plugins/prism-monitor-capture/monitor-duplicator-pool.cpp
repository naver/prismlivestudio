#include "monitor-duplicator-pool.h"
#include "obs.h"
#include <log.h>

PLSMonitorDuplicatorPool *PLSMonitorDuplicatorPool::get_instance()
{
	static PLSMonitorDuplicatorPool pls_pool;
	return &pls_pool;
}

void PLSMonitorDuplicatorPool::clear()
{
	std::lock_guard<std::mutex> lock(mtx);
	duplicator_vector.clear();
}

DUPLICATOR_PTR PLSMonitorDuplicatorPool::get_duplicator(int adapter_index, int output_index, int display_id)
{
	std::lock_guard<std::mutex> lock(mtx);

	for (auto item : duplicator_vector) {
		if (item->get_adapter_id() == adapter_index && item->get_monitor_dev_id() == output_index) {
			if (!display_id || item->get_hardware_id() == display_id) {
				return item;
			} else {
				duplicator_vector.clear();
				break;
			}
		}
	}

	obs_video_info ovi;
	if (!obs_get_video_info(&ovi)) {
		PLS_PLUGIN_ERROR("Failed to get obs video info for creating duplicator");
		return DUPLICATOR_PTR();
	}

	DuplicatorType type;
	if (adapter_index == ovi.adapter) {
		type = DUPLICATOR_SHARED_HANDLE;
		PLS_PLUGIN_INFO("Create duplicator with shared handle. renderAdapater:%d adapter:%d, monitor:%d", ovi.adapter, adapter_index, output_index);
	} else {
		type = DUPLICATOR_MEMORY_COPY;
		PLS_PLUGIN_INFO("Create duplicator with different adapter. renderAdapater:%d adapter:%d, monitor:%d", ovi.adapter, adapter_index, output_index);
	}

	DUPLICATOR_PTR duplicator = DUPLICATOR_PTR(new (std::nothrow) PLSDuplicatorInstance(type, adapter_index, output_index, display_id));
	duplicator_vector.push_back(duplicator);

	return duplicator;
}
