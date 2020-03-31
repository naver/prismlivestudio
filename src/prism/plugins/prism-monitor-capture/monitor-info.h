#pragma once
#include <dxgi.h>
#include <string>
#include <vector>
#include <mutex>
#include "win_lock.hpp"

using namespace std;

struct monitor_info {
	string friendly_name;
	bool is_primary;
	int rotation;

	int display_id; // using to check monitor remove. For Windows, its value should be more than 0 if it is valid
	int adapter_index;
	int adapter_output_index;

	int offset_x;
	int offset_y;

	int width;
	int height;

	monitor_info() : friendly_name(""), is_primary(false), rotation(0), display_id(0), adapter_index(-1), adapter_output_index(-1), offset_x(0), offset_y(0), width(0), height(0) {}
};

class PLSMonitorManager {
public:
	PLSMonitorManager();
	~PLSMonitorManager();

public:
	static PLSMonitorManager *get_instance();

	void reload_monitor(bool save_log = false);
	bool find_monitor(int index, int display_id, monitor_info &info);
	vector<monitor_info> get_monitor();

private:
	bool enum_duplicator_array(vector<monitor_info> &outputList);

private:
	CCSection list_lock;
	vector<monitor_info> monitor_info_array;
};
