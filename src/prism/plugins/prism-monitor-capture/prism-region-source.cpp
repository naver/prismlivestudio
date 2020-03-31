#include <obs-module.h>
#include <log.h>
#include "monitor-info.h"
#include "monitor-duplicator-pool.h"
#include "window-version.h"
#include "gdi-capture.h"
#include "cursor-capture.h"

#define REGION_MIN_CX 0
#define REGION_MIN_CY 0
#define REGION_DEFAULT_CX REGION_MIN_CX
#define REGION_DEFAULT_CY REGION_MIN_CY

#define RETRY_DUPLICATOR_INTERVAL 2000 // millisecond

// keys of language strings
#define TEXT_REGION_NAME "RegionSourceName"
#define TEXT_REGION_CURSOR "CaptureCursor"

// keys of obs_data_t
#define REGION_KEY_REGION_SELECT "region_select"
#define REGION_KEY_CAPTURE_CURSOR "capture_cursor"
#define REGION_KEY_LEFT "left"
#define REGION_KEY_TOP "top"
#define REGION_KEY_WIDTH "width"
#define REGION_KEY_HEIGHT "height"

#define RELEASE_TEXTURE(tex)             \
	if (tex) {                       \
		gs_texture_destroy(tex); \
		tex = NULL;              \
	}

#define CHECK_TEXTURE(tex, cx, cy)                                                         \
	if (tex) {                                                                         \
		if (gs_texture_get_width(tex) != cx || gs_texture_get_height(tex) != cy) { \
			gs_texture_destroy(tex);                                           \
			tex = NULL;                                                        \
		}                                                                          \
	}

enum RegionCaptureType {
	RegionTypeNone = 0,
	RegionTypeGDI,
	RegionTypeGPU,
};

struct prism_region_settings {
	int left = 0;
	int top = 0;
	int width = REGION_DEFAULT_CX;
	int height = REGION_DEFAULT_CY;
	bool capture_cursor = true;

	//---------------------------------------
	void keep_valid();
	bool region_empty();
};

struct hit_info {
	int display_id;
	int rotation;
	int adapter_index;
	int adapter_output_index;

	// rect in monitor texture without rotation, its left-top is in duplicator texture without rotation.
	// if there exist rotation with monitor, before using these variables, we should firstly rotate duplicator texture.
	int src_left;
	int src_top;
	int src_right;
	int src_bottom;

	// rect in canvas texture
	int canvas_left;
	int canvas_top;
	int canvas_right;
	int canvas_bottom;
};

struct prism_region_gpu {
	DWORD pre_fail_time = 0;
	gs_texture_t *output_texture = NULL;
	gs_texture_t *rotate_target = NULL;

	//---------------------------------------
	prism_region_gpu() {}
	virtual ~prism_region_gpu();

	void clear_texture();
	bool update_gpu_texture(const prism_region_settings &region_settings, vector<hit_info> &hit_list);
	gs_texture_t *get_texture();

private:
	bool check_output_texture(int cx, int cy);
	bool render_output_texture(const prism_region_settings &region_settings, vector<hit_info> &hit_list);
	gs_texture_t *render_rotate_texture(gs_texture_t *monitor_texture, int rotation);
};

struct prism_region_source {
	obs_source_t *source = NULL;

	prism_region_settings region_settings;

	PLSGdiCapture gdi_capture;
	prism_region_gpu gpu_capture;
	PLSCursorCapture cursor_capture; // used for GPU capture
	RegionCaptureType capture_type = RegionTypeNone;

	//---------------------------------------
	prism_region_source(obs_data_t *settings, obs_source_t *source_);
	virtual ~prism_region_source();

	void tick();
	void render();
	void update(obs_data_t *settings);

private:
	gs_texture_t *get_region_texture();
	void set_capture_type(RegionCaptureType type);
	void hit_monitor_test(const prism_region_settings &region_settings, vector<hit_info> &hit_list);
};

//------------------------------------------------------------------------
void prism_region_settings::keep_valid()
{
	uint64_t max_size = 0;

	{
		PLSAutoLockRender alr;
		max_size = gs_texture_get_max_size();
	}

	if (max_size > 0) {
		if (width > max_size) {
			width = max_size;
		}

		if (height > max_size) {
			height = max_size;
		}
	}

	if (width < REGION_MIN_CX) {
		width = REGION_MIN_CX;
	}

	if (height < REGION_MIN_CY) {
		height = REGION_MIN_CY;
	}
}

bool prism_region_settings::region_empty()
{
	if (width > 0 && height > 0) {
		return false;
	} else {
		return true;
	}
}

prism_region_gpu::~prism_region_gpu()
{
	clear_texture();
}

gs_texture_t *prism_region_gpu::get_texture()
{
	return output_texture;
}

bool prism_region_gpu::update_gpu_texture(const prism_region_settings &region_settings, vector<hit_info> &hit_list)
{
	// Sometimes, duplicator is not available, we should retry it with interval
	if (GetTickCount() - pre_fail_time < RETRY_DUPLICATOR_INTERVAL) {
		return false;
	}

	do {
		if (!check_output_texture(region_settings.width, region_settings.height)) {
			break;
		}

		if (!render_output_texture(region_settings, hit_list)) {
			break;
		}

		pre_fail_time = 0;
		return true;

	} while (0);

	clear_texture();
	pre_fail_time = GetTickCount();

	return false;
}

void prism_region_gpu::clear_texture()
{
	PLSAutoLockRender alr;

	RELEASE_TEXTURE(output_texture);
	RELEASE_TEXTURE(rotate_target);
}

bool prism_region_gpu::check_output_texture(int cx, int cy)
{
	PLSAutoLockRender alr;

	CHECK_TEXTURE(output_texture, cx, cy);
	if (!output_texture) {
		output_texture = gs_texture_create(cx, cy, GS_RGBA, 1, NULL, GS_RENDER_TARGET);
		if (!output_texture) {
			PLS_PLUGIN_WARN("Failed to create output texture for region source. %dx%d", cx, cy);
		}
	}

	return !!output_texture;
}

bool prism_region_gpu::render_output_texture(const prism_region_settings &region_settings, vector<hit_info> &hit_list)
{
	struct vec4 clear_color;
	vec4_set(&clear_color, 0.0f, 0.0f, 0.0f, 0.0f);

	PLSAutoLockRender alr;

	gs_viewport_push();
	gs_projection_push();
	gs_matrix_push();

	gs_set_render_target(output_texture, NULL);
	gs_clear(GS_CLEAR_COLOR, &clear_color, 1.0f, 0);

	bool ret = true;
	for (auto item : hit_list) {
		DUPLICATOR_PTR duplicator = PLSMonitorDuplicatorPool::get_instance()->get_duplicator(item.adapter_index, item.adapter_output_index, item.display_id);
		if (!duplicator) {
			ret = false;
			break;
		}

		if (!duplicator->is_adapter_valid()) {
			PLSMonitorDuplicatorPool::get_instance()->clear();
			ret = false;
			break;
		}

		if (!duplicator->update_frame()) {
			ret = false;
			break;
		}

		gs_texture_t *rotated_texture = render_rotate_texture(duplicator->get_texture(), item.rotation);
		if (!rotated_texture) {
			ret = false;
			break;
		}

		gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
		gs_technique_t *tech = gs_effect_get_technique(effect, "Draw");
		gs_eparam_t *param = gs_effect_get_param_by_name(effect, "image");

		int dest_cx = (item.canvas_right - item.canvas_left);
		int dest_cy = (item.canvas_bottom - item.canvas_top);

		gs_effect_set_texture(param, rotated_texture);
		gs_matrix_identity();
		gs_ortho(0.0f, (float)dest_cx, 0.0f, (float)dest_cy, -100.0f, 100.0f);
		gs_set_viewport(item.canvas_left, item.canvas_top, dest_cx, dest_cy);

		size_t passes = gs_technique_begin(tech);
		for (size_t i = 0; i < passes; i++) {
			gs_technique_begin_pass(tech, i);
			gs_draw_sprite_subregion(rotated_texture, false, item.src_left, item.src_top, (item.src_right - item.src_left), (item.src_bottom - item.src_top));
			gs_technique_end_pass(tech);
		}
		gs_technique_end(tech);
	}

	gs_matrix_pop();
	gs_projection_pop();
	gs_viewport_pop();

	return ret;
}

gs_texture_t *prism_region_gpu::render_rotate_texture(gs_texture_t *monitor_texture, int rotation)
{
	if (rotation == 0) {
		return monitor_texture;
	}

	int tex_cx = gs_texture_get_width(monitor_texture);
	int tex_cy = gs_texture_get_height(monitor_texture);

	int target_cx;
	int target_cy;
	if ((rotation % 180) == 0) {
		target_cx = tex_cx;
		target_cy = tex_cy;
	} else {
		target_cx = tex_cy;
		target_cy = tex_cx;
	}

	struct vec4 clear_color;
	vec4_set(&clear_color, 0.0f, 0.0f, 0.0f, 0.0f);

	PLSAutoLockRender alr;

	CHECK_TEXTURE(rotate_target, target_cx, target_cy);
	if (!rotate_target) {
		rotate_target = gs_texture_create(target_cx, target_cy, GS_RGBA, 1, NULL, GS_RENDER_TARGET);
		if (!rotate_target) {
			PLS_PLUGIN_WARN("Failed to create rotate texture for region source. %dx%d", target_cx, target_cy);
			return NULL;
		}
	}

	gs_viewport_push();
	gs_projection_push();
	gs_matrix_push();
	gs_texture_t *pre_target = gs_get_render_target();
	gs_zstencil_t *pre_ztl = gs_get_zstencil_target();

	gs_set_render_target(rotate_target, NULL);
	gs_clear(GS_CLEAR_COLOR, &clear_color, 1.0f, 0);

	gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
	gs_eparam_t *param = gs_effect_get_param_by_name(effect, "image");

	gs_effect_set_texture(param, monitor_texture);
	gs_matrix_identity();
	gs_ortho(0.0f, (float)target_cx, 0.0f, (float)target_cy, -100.0f, 100.0f);
	gs_set_viewport(0, 0, target_cx, target_cy);

	while (gs_effect_loop(effect, "Draw")) {
		if (rotation != 0) {
			float x = 0.0f;
			float y = 0.0f;
			switch (rotation) {
			case 90:
				x = (float)tex_cy;
				break;
			case 180:
				x = (float)tex_cx;
				y = (float)tex_cy;
				break;
			case 270:
				y = (float)tex_cx;
				break;
			}
			gs_matrix_push();
			gs_matrix_translate3f(x, y, 0.0f);
			gs_matrix_rotaa4f(0.0f, 0.0f, 1.0f, RAD((float)rotation));
		}

		obs_source_draw(monitor_texture, 0, 0, 0, 0, false);

		if (rotation != 0) {
			gs_matrix_pop();
		}
	}

	gs_set_render_target(pre_target, pre_ztl);
	gs_matrix_pop();
	gs_projection_pop();
	gs_viewport_pop();

	return rotate_target;
}

prism_region_source::prism_region_source(obs_data_t *settings, obs_source_t *source_) : source(source_)
{
	obs_source_set_capture_valid(source_, true, OBS_SOURCE_ERROR_OK);
	update(settings);
}

prism_region_source ::~prism_region_source()
{
	gdi_capture.uninit();
	if (PLSWindowVersion::is_support_monitor_duplicate()) {
		gpu_capture.clear_texture();
	}
}

extern void handle_hooked_message();
void prism_region_source::tick()
{
	handle_hooked_message();

	if (region_settings.region_empty()) {
		obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);
	} else {
		vector<hit_info> hit_list;
		hit_monitor_test(region_settings, hit_list);

		if (hit_list.empty()) {
			obs_source_set_capture_valid(source, false, OBS_SOURCE_ERROR_NOT_FOUND);
		} else {
			obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);
		}

		PLSAutoLockRender alr;

		if (PLSWindowVersion::is_support_monitor_duplicate()) {
			if (gpu_capture.update_gpu_texture(region_settings, hit_list)) {
				return; // GPU works normally, we will skip GDI capture
			}
		}

		// GPU is unavailable, we have to use GDI capture
		if (gdi_capture.check_init(region_settings.width, region_settings.height)) {
			gdi_capture.capture(region_settings.left, region_settings.top, region_settings.capture_cursor, region_settings.left, region_settings.top, 1.0);
		}
	}
}

void prism_region_source::render()
{
	PLSAutoLockRender alr;

	gs_texture_t *tex = get_region_texture();
	if (!tex) {
		return;
	}

	gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_OPAQUE);
	while (gs_effect_loop(effect, "Draw")) {
		obs_source_draw(tex, 0, 0, 0, 0, false);
	}

	if (region_settings.capture_cursor && capture_type == RegionTypeGPU) {
		effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
		while (gs_effect_loop(effect, "Draw")) {
			cursor_capture.cursor_render(-region_settings.left, -region_settings.top, 1.0f, 1.0f, region_settings.width, region_settings.height);
		}
	}
}

gs_texture_t *prism_region_source::get_region_texture()
{
	if (region_settings.region_empty()) {
		return NULL;
	}

	if (!obs_source_get_capture_valid(source, NULL)) {
		return NULL;
	}

	if (PLSWindowVersion::is_support_monitor_duplicate()) {
		gs_texture_t *texture = gpu_capture.get_texture();
		if (texture) {
			set_capture_type(RegionTypeGPU);
			return texture;
		}
	}

	gs_texture_t *texture = gdi_capture.get_texture();
	if (texture) {
		set_capture_type(RegionTypeGDI);
		return texture;
	}

	return NULL;
}

void prism_region_source::set_capture_type(RegionCaptureType type)
{
	if (capture_type == type) {
		return;
	}

	capture_type = type;
	if (type == RegionTypeGPU) {
		PLS_PLUGIN_INFO("Region capture type is GPU.");
	} else {
		PLS_PLUGIN_INFO("Region capture type is GDI.");
	}
}

void prism_region_source::hit_monitor_test(const prism_region_settings &region_settings, vector<hit_info> &hit_list)
{
	int dest_left = region_settings.left;
	int dest_top = region_settings.top;
	int dest_right = region_settings.left + region_settings.width;
	int dest_bottom = region_settings.top + region_settings.height;

	vector<monitor_info> monitors = PLSMonitorManager::get_instance()->get_monitor();
	for (auto item : monitors) {
		int monitor_right = item.offset_x + item.width;
		int monitor_bottom = item.offset_y + item.height;

		if (dest_right <= item.offset_x || dest_bottom <= item.offset_y || dest_left >= monitor_right || dest_top >= monitor_bottom) {
			continue; // never hit this monitor
		}

		hit_info info;
		info.display_id = item.display_id;
		info.rotation = item.rotation;
		info.adapter_index = item.adapter_index;
		info.adapter_output_index = item.adapter_output_index;

		if (dest_left < item.offset_x) {
			info.src_left = 0;
			info.canvas_left = item.offset_x - dest_left;
		} else {
			info.src_left = dest_left - item.offset_x;
			info.canvas_left = 0;
		}

		if (dest_top < item.offset_y) {
			info.src_top = 0;
			info.canvas_top = item.offset_y - dest_top;
		} else {
			info.src_top = dest_top - item.offset_y;
			info.canvas_top = 0;
		}

		if (dest_right < monitor_right) {
			info.src_right = dest_right - item.offset_x;
			info.canvas_right = region_settings.width;
		} else {
			info.src_right = item.width;
			info.canvas_right = monitor_right - dest_left;
		}

		if (dest_bottom < monitor_bottom) {
			info.src_bottom = dest_bottom - item.offset_y;
			info.canvas_bottom = region_settings.height;
		} else {
			info.src_bottom = item.height;
			info.canvas_bottom = monitor_bottom - dest_top;
		}

		hit_list.push_back(info);
	}
}

void prism_region_source::update(obs_data_t *settings)
{
	prism_region_settings temp;

	obs_data_t *region_obj = obs_data_get_obj(settings, REGION_KEY_REGION_SELECT);
	temp.left = (int)obs_data_get_int(region_obj, REGION_KEY_LEFT);
	temp.top = (int)obs_data_get_int(region_obj, REGION_KEY_TOP);
	temp.width = (int)obs_data_get_int(region_obj, REGION_KEY_WIDTH);
	temp.height = (int)obs_data_get_int(region_obj, REGION_KEY_HEIGHT);
	obs_data_release(region_obj);

	temp.capture_cursor = obs_data_get_bool(settings, REGION_KEY_CAPTURE_CURSOR);

	temp.keep_valid();

	//-------------------------------------------------
	region_settings = temp;

	if (PLSWindowVersion::is_support_monitor_duplicate()) {
		tick(); // Because GPU texture is updated in worker thread, so here we invoke tick() to active worker thread early.
	}
}

static obs_properties_t *region_source_get_properties(void *data)
{
	obs_properties_t *props = obs_properties_create();

	obs_properties_add_region_select(props, REGION_KEY_REGION_SELECT, "");
	obs_properties_add_bool(props, REGION_KEY_CAPTURE_CURSOR, obs_module_text(TEXT_REGION_CURSOR));

	return props;
}

static void region_source_get_defaults(obs_data_t *settings)
{
	obs_data_set_flags(settings, PROPERTY_FLAG_NO_LABEL_HEADER);

	obs_data_t *region_obj = obs_data_create();
	obs_data_set_default_int(region_obj, REGION_KEY_LEFT, 0);
	obs_data_set_default_int(region_obj, REGION_KEY_TOP, 0);
	obs_data_set_default_int(region_obj, REGION_KEY_WIDTH, REGION_DEFAULT_CX);
	obs_data_set_default_int(region_obj, REGION_KEY_HEIGHT, REGION_DEFAULT_CY);

	obs_data_set_default_obj(settings, REGION_KEY_REGION_SELECT, region_obj);
	obs_data_set_default_bool(settings, REGION_KEY_CAPTURE_CURSOR, true);

	obs_data_release(region_obj);
}

void register_prism_region_source()
{
	obs_source_info info = {};

	info.id = "prism_region_source";
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_DO_NOT_DUPLICATE;
	info.icon_type = OBS_ICON_TYPE_REGION;
	info.get_name = [](void *) { return obs_module_text(TEXT_REGION_NAME); };

	info.create = [](obs_data_t *settings, obs_source_t *source) -> void * {
		PLSMonitorManager::get_instance()->reload_monitor(false);
		return new prism_region_source(settings, source);
	};

	info.destroy = [](void *data) { delete static_cast<prism_region_source *>(data); };

	info.get_width = [](void *data) {
		prism_region_source *region = static_cast<prism_region_source *>(data);
		if (obs_source_get_capture_valid(region->source, NULL)) {
			return (uint32_t)region->region_settings.width;
		} else {
			return (uint32_t)0;
		}
	};

	info.get_height = [](void *data) {
		prism_region_source *region = static_cast<prism_region_source *>(data);
		if (obs_source_get_capture_valid(region->source, NULL)) {
			return (uint32_t)region->region_settings.height;
		} else {
			return (uint32_t)0;
		}
	};

	info.video_tick = [](void *data, float) { static_cast<prism_region_source *>(data)->tick(); };
	info.video_render = [](void *data, gs_effect_t *effect) { static_cast<prism_region_source *>(data)->render(); };
	info.get_properties = region_source_get_properties;
	info.get_defaults = region_source_get_defaults;
	info.update = [](void *data, obs_data_t *settings) { static_cast<prism_region_source *>(data)->update(settings); };

	obs_register_source(&info);
}
