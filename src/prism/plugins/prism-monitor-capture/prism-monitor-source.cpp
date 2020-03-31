#include <objbase.h>

#include <obs-module.h>
#include <util/dstr.h>
#include <string>
#include <vector>

#include "monitor-info.h"
#include "monitor-duplicator-pool.h"
#include "window-version.h"
#include "gdi-capture.h"
#include "cursor-capture.h"
#include <log.h>
using namespace std;

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D11.lib")

#define TEXT_MONITOR_CAPTURE obs_module_text("MonitorCapture")
#define TEXT_CAPTURE_CURSOR obs_module_text("CaptureCursor")
#define TEXT_COMPATIBILITY obs_module_text("Compatibility")
#define TEXT_MONITOR obs_module_text("Monitor")
#define TEXT_PRIMARY_MONITOR obs_module_text("PrimaryMonitor")

#define RESET_INTERVAL_MILLISEC 3000 // millisecond

struct prism_monitor_capture {
	obs_source_t *source = NULL;

	monitor_info mi;

	int display_id = 0;
	int monitor_index = -1; // index of PLSMonitorManager::monitor_info_array

	bool is_capture_cursor = true;

	bool monitor_lost = false;
	uint32_t preInitTime = 0; // millisecond

	DUPLICATOR_PTR duplicator = DUPLICATOR_PTR();
	PLSGdiCapture *gdi_capture = NULL;
	PLSCursorCapture *cursor_capture = NULL;
};

static void free_duplicator_data(struct prism_monitor_capture *capture)
{
	if (capture->duplicator) {
		PLSAutoLockRender alr;
		capture->duplicator.reset();
	}
}

static void free_capture_data(struct prism_monitor_capture *capture)
{
	free_duplicator_data(capture);

	if (capture->gdi_capture) {
		delete capture->gdi_capture;
		capture->gdi_capture = NULL;
	}

	if (capture->cursor_capture) {
		delete capture->cursor_capture;
		capture->cursor_capture = NULL;
	}
}

static inline void update_settings_properity(struct prism_monitor_capture *capture, obs_data_t *settings)
{
	int new_monitor = (int)obs_data_get_int(settings, "monitor");

	capture->display_id = new_monitor;
	capture->monitor_index = new_monitor;
	capture->is_capture_cursor = obs_data_get_bool(settings, "capture_cursor");
	capture->preInitTime = 0;
	capture->mi = monitor_info();
}

static const char *prism_monitor_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("MonitorCapture");
}

static void prism_monitor_update(void *data, obs_data_t *settings)
{
	update_settings_properity(reinterpret_cast<prism_monitor_capture *>(data), settings);
}

static uint32_t prism_monitor_width(void *data)
{
	struct prism_monitor_capture *capture = reinterpret_cast<prism_monitor_capture *>(data);
	if (capture->duplicator)
		return capture->mi.rotation % 180 == 0 ? capture->mi.width : capture->mi.height;
	else
		return capture->mi.width;
}

static uint32_t prism_monitor_height(void *data)
{
	struct prism_monitor_capture *capture = reinterpret_cast<prism_monitor_capture *>(data);
	if (capture->duplicator)
		return capture->mi.rotation % 180 == 0 ? capture->mi.height : capture->mi.width;
	else
		return capture->mi.height;
}

static void prism_monitor_defaults(obs_data_t *settings)
{
	obs_data_set_default_bool(settings, "capture_cursor", true);
}

static obs_properties_t *prism_monitor_properties(void *data)
{
	UNUSED_PARAMETER(data);

	PLSMonitorManager::get_instance()->reload_monitor(false);

	obs_properties_t *props = obs_properties_create();

	//PRISM/XieWei/20201109/#5641/Change the property order
	//obs_properties_add_bool(props, "capture_cursor", TEXT_CAPTURE_CURSOR);

	obs_property_t *monitors = obs_properties_add_list(props, "monitor", TEXT_MONITOR, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	vector<monitor_info> monitor_array = PLSMonitorManager::get_instance()->get_monitor();
	vector<monitor_info>::iterator iter = monitor_array.begin();
	char header_title[40];
	char monitor_desc[512];
	for (int index = 0; iter != monitor_array.end(); iter++) {
		if (iter->friendly_name.empty()) {
			snprintf(header_title, sizeof(header_title), "%s %d", TEXT_MONITOR, index);
		} else {
			snprintf(header_title, sizeof(header_title), "%s", iter->friendly_name.c_str());
		}

		if (iter->is_primary) {
			snprintf(monitor_desc, sizeof(monitor_desc), "%s: %ldx%ld %s", header_title, iter->width, iter->height, TEXT_PRIMARY_MONITOR);
		} else {
			snprintf(monitor_desc, sizeof(monitor_desc), "%s: %ldx%ld", header_title, iter->width, iter->height);
		}

		if (iter->display_id > 0) {
			obs_property_list_add_int(monitors, monitor_desc, iter->display_id);
		} else {
			obs_property_list_add_int(monitors, monitor_desc, index);
		}

		index++;
	}
	//PRISM/XieWei/20201109/#5641/Change the property order
	obs_properties_add_bool(props, "capture_cursor", TEXT_CAPTURE_CURSOR);
	return props;
}

static void *prism_monitor_create(obs_data_t *settings, obs_source_t *source)
{
	struct prism_monitor_capture *capture = new prism_monitor_capture();

	capture->source = source;
	capture->cursor_capture = new PLSCursorCapture;
	capture->gdi_capture = new PLSGdiCapture;
	capture->duplicator = DUPLICATOR_PTR();
	capture->is_capture_cursor = false;
	capture->display_id = 0;
	capture->monitor_index = -1;
	capture->preInitTime = 0;
	capture->monitor_lost = false;
	update_settings_properity(capture, settings);

	PLS_PLUGIN_INFO("create monitor capture source. displayID:%d", capture->display_id);
	return capture;
}

static void prism_monitor_destroy(void *data)
{
	prism_monitor_capture *mc = static_cast<prism_monitor_capture *>(data);
	free_capture_data(mc);
	delete mc;
}

static void draw_cursor(struct prism_monitor_capture *capture)
{
	capture->cursor_capture->cursor_render(-capture->mi.offset_x, -capture->mi.offset_y, 1.0f, 1.0f, capture->mi.rotation % 180 == 0 ? capture->mi.width : capture->mi.height,
					       capture->mi.rotation % 180 == 0 ? capture->mi.height : capture->mi.width);
}

gs_texture_t *get_monitor_texture(struct prism_monitor_capture *capture)
{
	if (capture->duplicator) {
		gs_texture_t *texture = capture->duplicator->get_texture();
		if (texture)
			return texture;
	}

	if (capture->gdi_capture->check_init(capture->mi.width, capture->mi.height)) {
		capture->gdi_capture->capture(capture->mi.offset_x, capture->mi.offset_y, capture->is_capture_cursor, capture->mi.offset_x, capture->mi.offset_y, 1.0);
		gs_texture_t *texture = capture->gdi_capture->get_texture();
		if (texture)
			return texture;
	}

	return NULL;
}

static void prism_monitor_render_texture(prism_monitor_capture *capture, gs_texture_t *texture, gs_effect_t *effect)
{
	PLSAutoLockRender alr;

	effect = obs_get_base_effect(OBS_EFFECT_OPAQUE);

	int rot = capture->mi.rotation;

	while (gs_effect_loop(effect, "Draw")) {
		if (rot != 0) {
			gs_matrix_push();

			if (capture->duplicator) {
				float x = 0.0f;
				float y = 0.0f;
				switch (rot) {
				case 90:
					x = (float)capture->mi.height;
					break;
				case 180:
					x = (float)capture->mi.width;
					y = (float)capture->mi.height;
					break;
				case 270:
					y = (float)capture->mi.width;
					break;
				}
				gs_matrix_translate3f(x, y, 0.0f);
				gs_matrix_rotaa4f(0.0f, 0.0f, 1.0f, RAD((float)rot));
			}
		}

		obs_source_draw(texture, 0, 0, 0, 0, false);

		if (rot != 0)
			gs_matrix_pop();
	}
}

static void prism_monitor_render(void *data, gs_effect_t *effect)
{
	struct prism_monitor_capture *capture = reinterpret_cast<prism_monitor_capture *>(data);

	if (capture->monitor_lost)
		return;

	PLSAutoLockRender alr;

	gs_texture_t *texture = get_monitor_texture(capture);
	if (texture)
		prism_monitor_render_texture(capture, texture, effect);

	if (capture->is_capture_cursor) {
		effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
		while (gs_effect_loop(effect, "Draw")) {
			draw_cursor(capture);
		}
	}

	UNUSED_PARAMETER(effect);
}

extern void handle_hooked_message();
static void prism_monitor_tick(void *data, float seconds)
{
	handle_hooked_message();

	struct prism_monitor_capture *mc = reinterpret_cast<prism_monitor_capture *>(data);

	monitor_info info;
	mc->monitor_lost = !PLSMonitorManager::get_instance()->find_monitor(mc->monitor_index, mc->display_id, info);
	if (mc->monitor_lost)
		return;

	if (mc->duplicator) {
		if (info.display_id != mc->mi.display_id || info.adapter_index != mc->mi.adapter_index || info.adapter_output_index != mc->mi.adapter_output_index) {
			free_duplicator_data(mc);
			PLSMonitorDuplicatorPool::get_instance()->clear();
			PLS_PLUGIN_INFO("Display source should be reset. oldAdapter:%d oldOutput:%d newAdapter:%d newOutput:%d", mc->mi.adapter_index, mc->mi.adapter_output_index, info.adapter_index,
					info.adapter_output_index);
		}
	}

	if (mc->duplicator) {
		if (!mc->duplicator->is_adapter_valid()) {
			free_duplicator_data(mc);
			PLSMonitorDuplicatorPool::get_instance()->clear();
			PLS_PLUGIN_INFO("Display source should be reset because adapter is changed.");
		}
	}

	if (!obs_source_showing(mc->source)) {
		return;
	}

	mc->mi = info;

	PLSAutoLockRender alr;
	if (PLSWindowVersion::is_support_monitor_duplicate()) {
		if (!mc->duplicator) {
			DWORD currentTime = GetTickCount();
			if (mc->preInitTime >= currentTime) // handle exception
				mc->preInitTime = currentTime;

			if (currentTime - mc->preInitTime >= RESET_INTERVAL_MILLISEC) {
				mc->duplicator = PLSMonitorDuplicatorPool::get_instance()->get_duplicator(mc->mi.adapter_index, mc->mi.adapter_output_index, mc->mi.display_id);
				mc->preInitTime = currentTime;
			}
		}

		if (mc->duplicator) {
			if (mc->duplicator->update_frame()) {
				gs_texture_t *texture = mc->duplicator->get_texture();
				if (texture) {
					mc->mi.width = gs_texture_get_width(texture);
					mc->mi.height = gs_texture_get_height(texture);
				}
			} else {
				free_duplicator_data(mc);
			}
		}
	}
}

void register_prism_monitor_source()
{
	obs_source_info info = {};
	info.id = "prism_monitor_capture";
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_DO_NOT_DUPLICATE;
	info.get_name = prism_monitor_get_name;
	info.create = prism_monitor_create;
	info.destroy = prism_monitor_destroy;
	info.video_render = prism_monitor_render;
	info.video_tick = prism_monitor_tick;
	info.update = prism_monitor_update;
	info.get_width = prism_monitor_width;
	info.get_height = prism_monitor_height;
	info.get_defaults = prism_monitor_defaults;
	info.get_properties = prism_monitor_properties;
	info.icon_type = OBS_ICON_TYPE_DESKTOP_CAPTURE;

	obs_register_source(&info);
}
