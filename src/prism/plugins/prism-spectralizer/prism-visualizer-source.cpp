/*************************************************************************
 * This file is part of spectralizer
 * github.con/univrsal/spectralizer
 * Copyright 2020 univrsal <universailp@web.de>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "prism-visualizer-source.hpp"
#include "bar-visualizer.hpp"
#include "util.hpp"
#include "util/platform.h"

#include "obs-internal-source.hpp"
#include "obs-frontend-api.h"
#include <map>
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>

static int MARGIN = 20;

/*
Audio Visualization Related Resources.
*/
PLSVisualizerResource::PLSVisualizerResource()
{
	signal_handler_connect_ref(obs_get_signal_handler(), "source_create", source_changed, &this->create_type);
	signal_handler_connect_ref(obs_get_signal_handler(), "source_destroy", source_changed, &this->destroy_type);
	signal_handler_connect_ref(obs_get_signal_handler(), "source_rename", source_changed, &this->rename_type);

	solid_rgb[SOLID_COLOR_0] = C_SOLID_COLOR_0;
	solid_rgb[SOLID_COLOR_1] = C_SOLID_COLOR_1;
	solid_rgb[SOLID_COLOR_2] = C_SOLID_COLOR_2;
	solid_rgb[SOLID_COLOR_3] = C_SOLID_COLOR_3;
	solid_rgb[SOLID_COLOR_4] = C_SOLID_COLOR_4;
	solid_rgb[SOLID_COLOR_5] = C_SOLID_COLOR_5;
	solid_rgb[SOLID_COLOR_6] = C_SOLID_COLOR_6;
	solid_rgb[SOLID_COLOR_7] = C_SOLID_COLOR_7;
	solid_rgb[SOLID_COLOR_8] = C_SOLID_COLOR_8;
	solid_rgb[SOLID_COLOR_9] = C_SOLID_COLOR_9;
	solid_rgb[SOLID_COLOR_10] = C_SOLID_COLOR_10;
	solid_rgb[SOLID_COLOR_11] = C_SOLID_COLOR_11;
	solid_rgb[SOLID_COLOR_12] = C_SOLID_COLOR_12;

	obs_enter_graphics();
	char *path = obs_module_file(P_GRADIENT_MODE_0);
	gradient_texture[GRADIENT_COLOR_0] = gs_texture_create_from_file(path);
	bfree(path);
	path = obs_module_file(P_GRADIENT_MODE_1);
	gradient_texture[GRADIENT_COLOR_1] = gs_texture_create_from_file(path);
	bfree(path);
	path = obs_module_file(P_GRADIENT_MODE_2);
	gradient_texture[GRADIENT_COLOR_2] = gs_texture_create_from_file(path);
	bfree(path);
	path = obs_module_file(P_GRADIENT_MODE_3);
	gradient_texture[GRADIENT_COLOR_3] = gs_texture_create_from_file(path);
	bfree(path);
	path = obs_module_file(P_GRADIENT_MODE_4);
	gradient_texture[GRADIENT_COLOR_4] = gs_texture_create_from_file(path);
	bfree(path);

	obs_leave_graphics();
}

PLSVisualizerResource::~PLSVisualizerResource()
{
	signal_handler_disconnect(obs_get_signal_handler(), "source_create", source_changed, &this->create_type);
	signal_handler_disconnect(obs_get_signal_handler(), "source_destroy", source_changed, &this->destroy_type);
	signal_handler_disconnect(obs_get_signal_handler(), "source_rename", source_changed, &this->rename_type);

	obs_enter_graphics();
	for (auto it = gradient_texture.rbegin(); it != gradient_texture.rend(); it++) {
		gs_texture_destroy(it->second);
		it->second = nullptr;
	}
	obs_leave_graphics();
}

void PLSVisualizerResource::source_changed(void *data, calldata_t *calldata)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(calldata, "source");
	if (!source || !data)
		return;

	uint32_t flags = obs_source_get_output_flags(source);
	if (flags & OBS_SOURCE_AUDIO) {
		auto update_properties = [](void *param, obs_source_t *source) {
			if (!source || !param)
				return true;

			const char *id = obs_source_get_id(source);
			if (!id || strcmp(id, "prism_audio_visualizer_source"))
				return true;

			changed_source_info info = (*reinterpret_cast<changed_source_info *>(param));
			if (info.type != SET_CREATE) {
				obs_data_t *settings = obs_source_get_settings(source);
				if (settings) {
					if (info.type == SET_DESTROY) {
						if (!strcmp(obs_data_get_string(settings, S_AUDIO_SOURCE), info.cur_name.c_str())) {
							obs_data_set_string(settings, S_AUDIO_SOURCE, defaults::audio_source);
							obs_source_update(source, settings);
						}
					} else if (info.type == SET_RENAME) {
						if (!strcmp(obs_data_get_string(settings, S_AUDIO_SOURCE), info.prev_name.c_str()))
							obs_data_set_string(settings, S_AUDIO_SOURCE, info.cur_name.c_str());
					}
					obs_data_release(settings);
				}
			}
			obs_source_update_properties(source);
			return true;
		};

		changed_source_info source_param;
		source_param.type = *reinterpret_cast<source_event_type *>(data);
		source_param.cur_name = obs_source_get_name(source);

		if (source_param.type == SET_RENAME) {
			source_param.cur_name = calldata_string(calldata, "new_name");
			source_param.prev_name = calldata_string(calldata, "prev_name");
		}

		obs_enum_sources(update_properties, &source_param);
	}
}

visualizer_source::visualizer_source(obs_source_t *source, obs_data_t *settings)
{
	config.settings = settings;
	config.source = source;

	init_params();
	update(settings);
}

void visualizer_source::destory()
{
	if (visualizer) {
		delete visualizer;
		visualizer = nullptr;
	}

	if (config.buffer) {
		bfree(config.buffer);
		config.buffer = nullptr;
	}
	if (config.capture_source) {
		obs_weak_source_release(config.capture_source);
	}
	if (config.settings)
		obs_data_release(config.settings);
}

void visualizer_source::init_params()
{
	visual_params params;

	params.bar_mode = BM_BASIC;
	params.color = defaults::basic_color;
	params.bar_width = defaults::basic_width;
	params.bar_height = defaults::basic_height;
	params.bar_min_height = defaults::basic_min_height;
	params.bar_space = defaults::basic_space;
	params.detail = defaults::basic_detail;
	params.falloff_weight = defaults::falloff_weight;
	params.gravity = defaults::gravity;
	params.stereo = false;
	params.smoothing = SM_NONE;
	config.vm_params[VM_BASIC_BARS] = params;

	params.bar_mode = BM_FILLET;
	params.color = defaults::basic_color;
	params.bar_width = defaults::fillet_width;
	params.bar_height = defaults::fillet_height;
	params.bar_min_height = defaults::fillet_min_height;
	params.bar_space = defaults::fillet_space;
	params.detail = defaults::fillet_detail;
	params.falloff_weight = defaults::falloff_weight;
	params.gravity = defaults::gravity;
	params.stereo = true;
	params.smoothing = SM_SGS;
	config.vm_params[VM_FILLET_BARS] = params;

	params.bar_mode = BM_LINEAR;
	params.solid_mode = SOLID_COLOR_1;
	params.bar_width = defaults::linear_width;
	params.bar_height = defaults::linear_height;
	params.bar_min_height = defaults::linear_min_height;
	params.bar_space = defaults::linear_space;
	params.detail = defaults::linear_detail;
	params.falloff_weight = defaults::falloff_weight;
	params.gravity = defaults::gravity;
	params.stereo = false;
	params.smoothing = SM_NONE;
	config.vm_params[VM_LINEAR_BARS] = params;

	params.bar_mode = BM_GRADIENT;
	params.gradient_mode = GRADIENT_COLOR_0;
	params.bar_width = defaults::gradient_width;
	params.bar_height = defaults::gradient_height;
	params.bar_min_height = defaults::gradient_min_height;
	params.bar_space = defaults::gradient_space;
	params.detail = defaults::gradient_detail;
	params.falloff_weight = defaults::falloff_weight;
	params.gravity = defaults::gravity;
	params.stereo = false;
	params.smoothing = SM_NONE;
	config.vm_params[VM_GRADIENT_BARS] = params;
}

void visualizer_source::update(obs_data_t *settings)
{
	visual_mode old_mode = config.visual;
	gradient_color old_gradient_mode = config.vm_params[config.visual].gradient_mode;

	config.audio_source_name = obs_data_get_string(settings, S_AUDIO_SOURCE);
	config.sample_rate = obs_data_get_int(settings, S_SAMPLE_RATE);
	config.sample_size = config.sample_rate / config.fps;
	config.visual = (visual_mode)(obs_data_get_int(settings, S_TEMPLATE_LIST));

	visual_params params;

	params.stereo = obs_data_get_bool(settings, S_STEREO);
	params.stereo_space = obs_data_get_int(settings, S_STEREO_SPACE);
	params.bar_width = obs_data_get_int(settings, S_BAR_WIDTH);
	params.bar_space = obs_data_get_int(settings, S_BAR_SPACE);
	params.detail = obs_data_get_int(settings, S_DETAIL);
	params.bar_height = obs_data_get_int(settings, S_BAR_HEIGHT);
	params.radius = 0.0;

	//params.smoothing = (smooting_mode)obs_data_get_int(settings, S_FILTER_MODE);
	params.smoothing = (smooting_mode)obs_data_get_int(settings, "custom_font");

	params.use_auto_scale = obs_data_get_bool(settings, S_AUTO_SCALE);
	params.scale_size = obs_data_get_double(settings, S_SCALE_SIZE);
	params.scale_boost = obs_data_get_double(settings, S_SCALE_BOOST);

	params.sgs_passes = obs_data_get_int(settings, S_SGS_PASSES);
	params.sgs_points = obs_data_get_int(settings, S_SGS_POINTS);
	params.falloff_weight = obs_data_get_double(settings, S_FALLOFF);
	params.gravity = obs_data_get_double(settings, S_GRAVITY);
	params.mcat_smoothing_factor = obs_data_get_double(settings, S_FILTER_STRENGTH);
	config.draw_cx = UTIL_MAX(params.detail * (params.bar_width + params.bar_space) - params.bar_space, 10);
	config.draw_cy = UTIL_MAX(params.bar_height + (params.stereo ? params.stereo_space : 0), 10);
	config.cx = config.draw_cx + MARGIN * 2;
	config.cy = config.draw_cy + MARGIN * 2;
	config.margin = MARGIN;

	params.gradient_mode = (gradient_color)obs_data_get_int(settings, S_GRADIENT_COLOR);
	params.solid_mode = (solid_color)obs_data_get_int(settings, S_SOLID_COLOR);

	switch (config.visual) {
	case VM_BASIC_BARS:
		params.bar_mode = BM_BASIC;
		params.color = obs_data_get_int(settings, S_COLOR);
		break;
	case VM_FILLET_BARS:
		params.bar_mode = BM_FILLET;
		params.smoothing = SM_SGS;
		params.sgs_points = defaults::sgs_points;
		params.sgs_passes = defaults::sgs_passes;
		params.color = obs_data_get_int(settings, S_COLOR);
		params.bar_min_height = params.bar_width;
		params.radius = params.bar_width > 5 ? static_cast<int64_t>(params.bar_width / 2) - 1 : params.bar_width > 3 ? 1 : 0.5;
		break;
	case VM_LINEAR_BARS:
		params.bar_mode = BM_LINEAR;
		params.color = PLSVisualizerResource::Instance()->getSolidColor(params.solid_mode);
		break;
	case VM_GRADIENT_BARS:
		params.bar_mode = BM_GRADIENT;
		break;
	}

	config.vm_params[config.visual] = params;

	if (visualizer) /* this modifies sample size, if an internal audio source is used */
		visualizer->update();

	if (old_mode != config.visual || !visualizer) {
		if (visualizer) {
			delete visualizer;
			visualizer = nullptr;
		}

		if (config.buffer)
			bfree(config.buffer);
		config.buffer = static_cast<pcm_stereo_sample *>(bzalloc(config.sample_size * sizeof(pcm_stereo_sample)));

		switch (config.visual) {
		case VM_BASIC_BARS:
		case VM_FILLET_BARS:
		case VM_LINEAR_BARS:
		case VM_GRADIENT_BARS:
			visualizer = new bar_visualizer(&config);
			break;
		}
	}
}

void visualizer_source::tick(float seconds)
{
	if (visualizer)
		visualizer->tick(seconds);

	if (visualizer) {
		visualizer->render();
	}
}

void visualizer_source::render(gs_effect_t *effect_)
{
	if (!visualizer)
		return;

	config.render_texture = visualizer->get_texture();
	if (!config.render_texture) {
		return;
	}

	if (config.render_texture) {
		gs_effect_set_texture(gs_effect_get_param_by_name(effect_, "image"), config.render_texture);
		gs_draw_sprite(config.render_texture, 0, 0, 0);
	}
}

bool visualizer_source::set_private_data(obs_data_t *private_data)
{
	if (!private_data)
		return false;

	std::string method = obs_data_get_string(private_data, "method");
	if (method == "set_default_audio_source") {
		std::string default_audio = obs_data_get_string(private_data, "default_audio_source");
		default_audio = default_audio.empty() ? defaults::audio_source : default_audio.c_str();

		if (config.audio_source_name != default_audio) {
			obs_data_set_string(config.settings, S_AUDIO_SOURCE, default_audio.c_str());
			obs_source_update(config.source, config.settings);
		}

		obs_data_t *priv_settings = obs_source_get_private_settings(config.source);
		obs_data_set_string(priv_settings, "default_audio_source", default_audio.c_str());
		obs_data_release(priv_settings);

	} else if (method == "default_properties") {
		init_params();

		obs_data_t *priv_settings = obs_source_get_private_settings(config.source);
		std::string name = obs_data_get_string(priv_settings, "default_audio_source");

		std::vector<std::string> audio_sources(PLSVisualizerResource::Instance()->getAudioSources());

		auto finder = [name](std::string elem) { return elem == name; };
		auto found_at = std::find_if(audio_sources.begin(), audio_sources.end(), finder);
		if (found_at != audio_sources.end())
			obs_data_set_string(config.settings, S_AUDIO_SOURCE, name.c_str());
		else {
			obs_data_set_string(priv_settings, "default_audio_source", defaults::audio_source);
			obs_data_set_string(config.settings, S_AUDIO_SOURCE, defaults::audio_source);
		}

		obs_data_release(priv_settings);
	}
	return true;
}

static bool filter_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *data)
{
	int mode = obs_data_get_int(data, S_FILTER_MODE);
	auto *strength = obs_properties_get(props, S_FILTER_STRENGTH);
	auto *sgs_pass = obs_properties_get(props, S_SGS_PASSES);
	auto *sgs_points = obs_properties_get(props, S_SGS_POINTS);

	if (mode == SM_NONE) {
		obs_property_set_visible(strength, false);
		obs_property_set_visible(sgs_pass, false);
		obs_property_set_visible(sgs_points, false);
	} else if (mode == SM_SGS) {
		obs_property_set_visible(sgs_pass, true);
		obs_property_set_visible(sgs_points, true);
		obs_property_set_visible(strength, false);
	} else if (mode == SM_MONSTERCAT) {
		obs_property_set_visible(strength, true);
		obs_property_set_visible(sgs_pass, false);
		obs_property_set_visible(sgs_points, false);
	}
	return true;
}

static bool use_auto_scale_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *data)
{
	auto state = !obs_data_get_bool(data, S_AUTO_SCALE);
	auto boost = obs_properties_get(props, S_SCALE_BOOST);
	auto size = obs_properties_get(props, S_SCALE_SIZE);

	obs_property_set_enabled(boost, state);
	obs_property_set_enabled(size, state);
	return true;
}

static bool stereo_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *data)
{
	UNUSED_PARAMETER(p);
	auto stereo = obs_data_get_bool(data, S_STEREO);
	auto *space = obs_properties_get(props, S_STEREO_SPACE);
	obs_property_set_enabled(space, stereo);
	return true;
}

static bool tab_changed(obs_properties_t *props, obs_property_t *, obs_data_t *settings)
{
	auto *template_list = obs_properties_get(props, S_TEMPLATE_LIST);
	auto *color = obs_properties_get(props, S_COLOR);
	auto *solid_color = obs_properties_get(props, S_SOLID_COLOR);
	auto *gradient_color = obs_properties_get(props, S_GRADIENT_COLOR);
	auto *gravity = obs_properties_get(props, S_GRAVITY);
	auto *stereo = obs_properties_get(props, S_STEREO);
	auto *stereo_space = obs_properties_get(props, S_STEREO_SPACE);
	auto *bar_settings = obs_properties_get(props, S_BAR_SETTINGS);
	auto *bar_title = obs_properties_get(props, T_BAR_SETTINGS);
	auto *h_line_0 = obs_properties_get(props, S_H_LINE_0);
	auto *h_line_1 = obs_properties_get(props, S_H_LINE_1);
	auto *h_line_2 = obs_properties_get(props, S_H_LINE_2);

	int tab_index = obs_data_get_int(settings, S_TAB);
	obs_property_set_visible(template_list, tab_index == 0);
	obs_property_set_visible(gravity, tab_index == 1);
	obs_property_set_visible(stereo, tab_index == 1);
	obs_property_set_visible(stereo_space, tab_index == 1);
	obs_property_set_visible(bar_settings, tab_index == 1);
	obs_property_set_visible(bar_title, tab_index == 1);
	obs_property_set_visible(h_line_0, tab_index == 1);
	obs_property_set_visible(h_line_1, tab_index == 1);
	obs_property_set_visible(h_line_2, tab_index == 1);

	visual_mode vm = (visual_mode)obs_data_get_int(settings, S_TEMPLATE_LIST);
	obs_property_set_visible(color, (tab_index == 1) && (vm == VM_BASIC_BARS || vm == VM_FILLET_BARS));
	obs_property_set_visible(solid_color, (tab_index == 1) && (vm == VM_LINEAR_BARS));
	obs_property_set_visible(gradient_color, (tab_index == 1) && (vm == VM_GRADIENT_BARS));

	obs_properties_custom_group_set_int_params(bar_settings, BAR_MIN_SPACE, vm == VM_BASIC_BARS ? BAR_BASIC_MAX_SPACE : BAR_OTHER_MAX_SPACE, 1, 2);

	return true;
}
static bool template_changed(void *data, obs_properties_t *props, obs_property_t *, obs_data_t *settings)
{
	visual_mode vm = (visual_mode)obs_data_get_int(settings, S_TEMPLATE_LIST);

	visualizer_source *vs = reinterpret_cast<visualizer_source *>(data);
	if (vs->get_old_visual_mode() != vm) {
		vs->init_params();

		visual_params params = vs->get_current_visual_params(vm);
		switch (vm) {
		case VM_BASIC_BARS:
			obs_data_set_int(settings, S_COLOR, params.color);
			break;
		case VM_FILLET_BARS:
			obs_data_set_int(settings, S_COLOR, params.color);
			break;
		case VM_LINEAR_BARS:
			obs_data_set_int(settings, S_SOLID_COLOR, params.solid_mode);
			break;
		case VM_GRADIENT_BARS:
			obs_data_set_int(settings, S_GRADIENT_COLOR, params.gradient_mode);
			break;
		}
		obs_data_set_int(settings, S_DETAIL, params.detail);
		obs_data_set_bool(settings, S_STEREO, params.stereo);
		obs_data_set_int(settings, S_STEREO_SPACE, params.stereo_space);
		obs_property_set_enabled(obs_properties_get(props, S_STEREO_SPACE), params.stereo);
		obs_data_set_double(settings, S_FILTER_STRENGTH, params.mcat_smoothing_factor);
		obs_data_set_double(settings, S_GRAVITY, params.gravity);
		obs_data_set_double(settings, S_FALLOFF, params.falloff_weight);
		obs_data_set_int(settings, S_SGS_PASSES, params.sgs_passes);
		obs_data_set_int(settings, S_SGS_POINTS, params.sgs_points);
		obs_data_set_int(settings, S_BAR_WIDTH, params.bar_width);
		obs_data_set_int(settings, S_BAR_HEIGHT, params.bar_height);
		obs_data_set_int(settings, S_BAR_SPACE, params.bar_space);
	}

	return true;
}

obs_properties_t *visualizer_source::get_properties_for_visualiser()
{
	obs_properties_t *props = obs_properties_create();

	/*audio source mode*/
	auto *src = obs_properties_add_list(props, S_AUDIO_SOURCE, T_AUDIO_SOURCE, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_add_flags(src, PROPERTY_FLAG_NO_LABEL_SINGLE);
	obs_property_list_add_string(src, T_AUDIO_SOURCE_NONE, defaults::audio_source);

	/*tab*/
	obs_property_t *tm_tab_prop = obs_properties_add_tm_tab(props, S_TAB);
	obs_property_set_modified_callback(tm_tab_prop, tab_changed);

	/*template setting*/
	obs_property_t *template_group = obs_properties_add_image_group(props, S_TEMPLATE_LIST, "", 1, 4, OBS_IMAGE_STYLE_TEMPLATE);
	obs_property_add_flags(template_group, PROPERTY_FLAG_NO_LABEL_SINGLE);
	obs_property_image_group_add_item(template_group, T_MODE_BARS_BASIC, P_BASIC_BARS, (int)VM_BASIC_BARS);
	obs_property_image_group_add_item(template_group, T_MODE_BARS_FILLET, P_FILLET_BARS, (int)VM_FILLET_BARS);
	obs_property_image_group_add_item(template_group, T_MODE_BARS_LINEAR, P_LINEAR_BARS, (int)VM_LINEAR_BARS);
	obs_property_image_group_add_item(template_group, T_MODE_BARS_GRADIENT, P_GRADIENT_BARS, (int)VM_GRADIENT_BARS);
	obs_property_set_modified_callback2(template_group, template_changed, this);

	/*solid color setting*/
	obs_property_t *solid_color_group = obs_properties_add_image_group(props, S_SOLID_COLOR, T_SOLID_COLOR, 1, 13, OBS_IMAGE_STYLE_SOLID_COLOR);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_0, P_SOLID_COLOR_0, (int)SOLID_COLOR_0);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_1, P_SOLID_COLOR_1, (int)SOLID_COLOR_1);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_2, P_SOLID_COLOR_2, (int)SOLID_COLOR_2);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_3, P_SOLID_COLOR_3, (int)SOLID_COLOR_3);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_4, P_SOLID_COLOR_4, (int)SOLID_COLOR_4);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_5, P_SOLID_COLOR_5, (int)SOLID_COLOR_5);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_6, P_SOLID_COLOR_6, (int)SOLID_COLOR_6);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_7, P_SOLID_COLOR_7, (int)SOLID_COLOR_7);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_8, P_SOLID_COLOR_8, (int)SOLID_COLOR_8);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_9, P_SOLID_COLOR_9, (int)SOLID_COLOR_9);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_10, P_SOLID_COLOR_10, (int)SOLID_COLOR_10);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_11, P_SOLID_COLOR_11, (int)SOLID_COLOR_11);
	obs_property_image_group_add_item(solid_color_group, S_SOLID_COLOR_12, P_SOLID_COLOR_12, (int)SOLID_COLOR_12);

	/*gradient color setting*/
	obs_property_t *gradient_color = obs_properties_add_image_group(props, S_GRADIENT_COLOR, T_GRADIENT_COLOR, 1, 5, OBS_IMAGE_STYLE_GRADIENT_COLOR);
	obs_property_image_group_add_item(gradient_color, S_GRADIENT_COLOR_0, P_GRADIENT_COLOR_0, (int)GRADIENT_COLOR_0);
	obs_property_image_group_add_item(gradient_color, S_GRADIENT_COLOR_1, P_GRADIENT_COLOR_1, (int)GRADIENT_COLOR_1);
	obs_property_image_group_add_item(gradient_color, S_GRADIENT_COLOR_2, P_GRADIENT_COLOR_2, (int)GRADIENT_COLOR_2);
	obs_property_image_group_add_item(gradient_color, S_GRADIENT_COLOR_3, P_GRADIENT_COLOR_3, (int)GRADIENT_COLOR_3);
	obs_property_image_group_add_item(gradient_color, S_GRADIENT_COLOR_4, P_GRADIENT_COLOR_4, (int)GRADIENT_COLOR_4);

	/*basic color setting*/
	obs_properties_add_color(props, S_COLOR, T_COLOR);

	obs_properties_add_h_line(props, S_H_LINE_0, "");

	/* bar settings */
	obs_property_add_flags(obs_properties_add_tips(props, T_BAR_SETTINGS, ""), PROPERTY_FLAG_NO_LABEL_SINGLE);
	obs_property_t *bar_settings = obs_properties_add_custom_group(props, S_BAR_SETTINGS, T_BAR_SETTINGS, 2, 2);
	obs_property_add_flags(bar_settings, PROPERTY_FLAG_NO_LABEL_SINGLE);

	visual_mode vm = (visual_mode)obs_data_get_int(config.settings, S_TEMPLATE_LIST);
	obs_properties_custom_group_add_int(bar_settings, S_BAR_WIDTH, T_BAR_WIDTH, BAR_MIN_WIDTH, BAR_MAX_WIDTH, 1, " Pixel");
	obs_properties_custom_group_add_int(bar_settings, S_BAR_HEIGHT, T_BAR_HEIGHT, BAR_MIN_HEIGHT, BAR_MAX_HEIGHT, 1, " Pixel");
	obs_properties_custom_group_add_int(bar_settings, S_BAR_SPACE, T_BAR_SPACING, BAR_MIN_SPACE, vm == VM_BASIC_BARS ? BAR_BASIC_MAX_SPACE : BAR_OTHER_MAX_SPACE, 1, " Pixel");
	obs_properties_custom_group_add_int(bar_settings, S_DETAIL, T_DETAIL, BAR_MIN_GRAPH_NUM, BAR_MAX_GRAPH_NUM, 1, "");

	/* Smoothing stuff */
	auto *gravity = obs_properties_add_int_slider(props, S_GRAVITY, T_GRAVITY, 0, 100, 1);
	obs_property_add_flags(gravity, PROPERTY_FLAG_CHILD_CONTROL);
	auto *falloff = obs_properties_add_int_slider(props, S_FALLOFF, T_FALLOFF, 0, 200, 1);
	obs_property_add_flags(falloff, PROPERTY_FLAG_CHILD_CONTROL);
	obs_property_set_visible(falloff, false);
	obs_properties_add_h_line(props, S_H_LINE_1, "");

	/* Scale stuff */
	auto auto_scale = obs_properties_add_bool_left(props, S_AUTO_SCALE, T_AUTO_SCALE);
	obs_property_set_modified_callback(auto_scale, use_auto_scale_changed);
	auto *scale_size = obs_properties_add_float_slider(props, S_SCALE_SIZE, T_SCALE_SIZE, 0.001, 2, 0.001);
	obs_property_add_flags(scale_size, PROPERTY_FLAG_CHILD_CONTROL);
	auto *scale_boost = obs_properties_add_float_slider(props, S_SCALE_BOOST, T_SCALE_BOOST, 0.001, 100, 0.001);
	obs_property_add_flags(scale_boost, PROPERTY_FLAG_CHILD_CONTROL);

	obs_property_set_visible(auto_scale, false);
	obs_property_set_visible(scale_size, false);
	obs_property_set_visible(scale_boost, false);

	/* filter setting */
	auto *filter = obs_properties_add_list(props, S_FILTER_MODE, T_FILTER_MODE, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_set_modified_callback(filter, filter_changed);
	obs_property_list_add_int(filter, T_FILTER_NONE, (int)SM_NONE);
	obs_property_list_add_int(filter, T_FILTER_MONSTERCAT, (int)SM_MONSTERCAT);
	obs_property_list_add_int(filter, T_FILTER_SGS, (int)SM_SGS);

	obs_property_set_visible(filter, false);
	obs_property_set_visible(obs_properties_add_float_slider(props, S_FILTER_STRENGTH, T_FILTER_STRENGTH, 1, 1.5, 0.01), false);
	obs_property_set_visible(obs_properties_add_int(props, S_SGS_POINTS, T_SGS_POINTS, 1, 32, 1), false);
	obs_property_set_visible(obs_properties_add_int(props, S_SGS_PASSES, T_SGS_PASSES, 1, 32, 1), false);

	/* stereo setting */
	auto *stereo = obs_properties_add_bool_left(props, S_STEREO, T_STEREO);
	obs_property_add_flags(stereo, PROPERTY_FLAG_NO_LABEL_SINGLE);
	auto *space = obs_properties_add_int(props, S_STEREO_SPACE, T_STEREO_SPACE, 0, 50, 1);
	obs_property_add_flags(space, PROPERTY_FLAG_CHILD_CONTROL);
	obs_property_int_set_suffix(space, " Pixel");
	obs_property_set_modified_callback(stereo, stereo_changed);

	obs_properties_add_h_line(props, S_H_LINE_2, "");

	std::vector<std::string> audio_sources{};
	auto add_source = [](void *param, obs_source_t *source) {
		if (!source || !param)
			return true;

		uint32_t caps = obs_source_get_output_flags(source);
		if ((caps & OBS_SOURCE_AUDIO) == 0)
			return true;

		std::vector<std::string> &audio_sources = *reinterpret_cast<std::vector<std::string> *>(param);
		std::string name = obs_source_get_name(source);
		auto finder = [name](std::string elem) { return name < elem; };
		auto found_at = std::find_if(audio_sources.begin(), audio_sources.end(), finder);
		audio_sources.insert(found_at, name);
		return true;
	};

	obs_enum_sources(add_source, &audio_sources);

	std::for_each(audio_sources.begin(), audio_sources.end(), [=](std::string name) { obs_property_list_add_string(src, name.c_str(), name.c_str()); });

	PLSVisualizerResource::Instance()->setAudioSources(audio_sources);
	return props;
}

visual_params visualizer_source::get_current_visual_params(visual_mode vm)
{
	return config.vm_params[vm];
}

visual_mode visualizer_source::get_old_visual_mode()
{
	return config.visual;
}

void visualizer_source::SaveTexture(gs_texture_t *tex, const char *path)
{
	obs_enter_graphics();

	uint32_t cx = gs_texture_get_width(tex);
	uint32_t cy = gs_texture_get_height(tex);
	enum gs_color_format fmt = gs_texture_get_color_format(tex);

	if (fmt == GS_RGBA || fmt == GS_BGRX || fmt == GS_BGRA) {
		gs_stagesurf_t *surface = gs_stagesurface_create(cx, cy, fmt);
		if (surface) {
			gs_stage_texture(surface, tex);

			uint8_t *data = NULL;
			uint32_t linesize = 0;
			if (gs_stagesurface_map(surface, &data, &linesize)) {
				save_as_bitmap_file((char *)path, data, linesize, cx, cy, 4, true);
				gs_stagesurface_unmap(surface);
			}

			gs_stagesurface_destroy(surface);
		}
	}

	obs_leave_graphics();
}

void register_visualiser()
{
	std::thread([] { PLSVisualizerResource::Instance(); }).detach();
	obs_source_info si = {};
	si.id = "prism_audio_visualizer_source";
	si.type = OBS_SOURCE_TYPE_INPUT;
	si.icon_type = OBS_ICON_TYPE_SPECTRALIZER;
	si.output_flags = OBS_SOURCE_VIDEO;
	si.get_properties = [](void *data) { return reinterpret_cast<visualizer_source *>(data)->get_properties_for_visualiser(); };

	si.get_name = [](void *) { return T_SOURCE; };
	si.create = [](obs_data_t *settings, obs_source_t *source) { return static_cast<void *>(new visualizer_source(source, settings)); };
	si.destroy = [](void *data) {
		reinterpret_cast<visualizer_source *>(data)->destory();
		delete reinterpret_cast<visualizer_source *>(data);
	};
	si.get_width = [](void *data) { return reinterpret_cast<visualizer_source *>(data)->get_width(); };
	si.get_height = [](void *data) { return reinterpret_cast<visualizer_source *>(data)->get_height(); };

	si.get_defaults = [](obs_data_t *settings) {
		obs_data_set_default_int(settings, S_COLOR, 0xFFFFFFFF);
		obs_data_set_default_int(settings, S_DETAIL, defaults::basic_detail);
		obs_data_set_default_bool(settings, S_STEREO, defaults::stereo);
		obs_data_set_default_int(settings, S_TAB, 0);
		obs_data_set_default_int(settings, S_TEMPLATE_LIST, (int)VM_BASIC_BARS);
		obs_data_set_default_int(settings, S_SAMPLE_RATE, defaults::sample_rate);
		obs_data_set_default_int(settings, S_FILTER_MODE, (int)SM_NONE);
		obs_data_set_default_double(settings, S_FILTER_STRENGTH, defaults::mcat_smooth);
		obs_data_set_default_double(settings, S_GRAVITY, defaults::gravity);
		obs_data_set_default_double(settings, S_FALLOFF, defaults::falloff_weight);
		obs_data_set_default_int(settings, S_SGS_PASSES, defaults::sgs_passes);
		obs_data_set_default_int(settings, S_SGS_POINTS, defaults::sgs_points);
		obs_data_set_default_int(settings, S_BAR_WIDTH, defaults::basic_width);
		obs_data_set_default_int(settings, S_BAR_HEIGHT, defaults::basic_height);
		obs_data_set_default_int(settings, S_BAR_SPACE, defaults::basic_space);
		obs_data_set_default_bool(settings, S_AUTO_SCALE, defaults::use_auto_scale);
		obs_data_set_default_double(settings, S_SCALE_SIZE, defaults::scale_size);
		obs_data_set_default_double(settings, S_SCALE_BOOST, defaults::scale_boost);
	};

	si.update = [](void *data, obs_data_t *settings) { reinterpret_cast<visualizer_source *>(data)->update(settings); };
	si.video_tick = [](void *data, float seconds) { reinterpret_cast<visualizer_source *>(data)->tick(seconds); };
	si.video_render = [](void *data, gs_effect_t *effect) { reinterpret_cast<visualizer_source *>(data)->render(effect); };
	si.set_private_data = [](void *data, obs_data_t *private_data) { return reinterpret_cast<visualizer_source *>(data)->set_private_data(private_data); };

	obs_register_source(&si);
}
