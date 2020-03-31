#include "obs.h"
#include "obs-internal.h"
#include "watermark.h"

static const uint64_t USEC_TO_NSEC = 1000;
static const uint64_t FADE_IN_TIME_NSEC = 500000000;
static const uint64_t FADE_OUT_TIME_NSEC = 500000000;
static const float DEFAULT_OPACITY = 1.0f; // default opacity

static void reset_watermark(obs_watermark_t *watermark)
{
	watermark->policy = OBS_WATERMARK_POLICY_NONE;
	watermark->show_type = OBS_WATERMARK_SHOW_TYPE_NOT_SHOW;
	watermark->top_margin = 0;
	watermark->left_margin = 0;
	watermark->start_time_nsec = 0;
	watermark->show_time_nsec = 0;
	watermark->fade_in_time_nsec = 0;
	watermark->fade_out_time_nsec = 0;
	watermark->interval_nsec = 0;
	watermark->render_width = 0;
	watermark->render_height = 0;
	watermark->texture = NULL;
	watermark->render_texture = NULL;
	watermark->opaque_effect = NULL;
	watermark->enabled = false;
	watermark->first_show = true;
	watermark->updated = false;
	watermark->next_timestamp = 0;
	watermark->need_update = true;
	dstr_free(&watermark->file_path);
}

static void set_info(struct obs_watermark *watermark,
		     const struct obs_watermark_info *info)
{
	watermark->first_show = true;
	os_atomic_set_bool(&watermark->need_update, false);
	watermark->next_timestamp = 0;
	watermark->policy = info->policy;
	watermark->top_margin = info->top_margin;
	watermark->left_margin = info->left_margin;
	watermark->start_time_nsec = info->start_time_usec * USEC_TO_NSEC;
	watermark->show_time_nsec = info->show_time_usec * USEC_TO_NSEC;
	watermark->fade_in_time_nsec = info->fade_in_time_usec * USEC_TO_NSEC;
	watermark->fade_out_time_nsec = info->fade_out_time_usec * USEC_TO_NSEC;
	watermark->interval_nsec = info->interval_usec * USEC_TO_NSEC;
	dstr_free(&watermark->file_path);
	dstr_copy(&watermark->file_path, info->file_path);
	os_atomic_set_bool(&watermark->updated, true);
}

static enum obs_watermark_show_type
get_show_type_internal(struct obs_watermark *watermark)
{
	enum obs_watermark_show_type show_type =
		OBS_WATERMARK_SHOW_TYPE_NOT_SHOW;

	uint64_t current_time = os_gettime_ns();
	if (watermark->next_timestamp == 0) {
		show_type = OBS_WATERMARK_SHOW_TYPE_BEGIN;
		watermark->first_show = true;
		watermark->next_timestamp =
			current_time + watermark->fade_in_time_nsec;
	} else {
		if (current_time < watermark->next_timestamp)
			show_type = watermark->show_type;
		else {
			switch (watermark->show_type) {
			case OBS_WATERMARK_SHOW_TYPE_BEGIN:
				show_type = OBS_WATERMARK_SHOW_TYPE_SHOWING;
				watermark->next_timestamp =
					current_time +
					(watermark->first_show
						 ? watermark->start_time_nsec
						 : watermark->show_time_nsec) -
					watermark->fade_out_time_nsec -
					watermark->fade_in_time_nsec;
				watermark->first_show = false;
				break;
			case OBS_WATERMARK_SHOW_TYPE_SHOWING:
				show_type = OBS_WATERMARK_SHOW_TYPE_END;
				watermark->next_timestamp =
					current_time +
					watermark->fade_out_time_nsec;
				break;
			case OBS_WATERMARK_SHOW_TYPE_END:
				show_type = OBS_WATERMARK_SHOW_TYPE_NOT_SHOW;
				watermark->next_timestamp =
					current_time + watermark->interval_nsec;
				break;
			case OBS_WATERMARK_SHOW_TYPE_NOT_SHOW:
				show_type = OBS_WATERMARK_SHOW_TYPE_BEGIN;
				watermark->next_timestamp =
					current_time +
					watermark->fade_in_time_nsec;
				break;
			default:
				show_type = OBS_WATERMARK_SHOW_TYPE_NOT_SHOW;
				break;
			}
		}
	}
	return show_type;
}

static enum obs_watermark_show_type
get_show_type(struct obs_watermark *watermark)
{
	enum obs_watermark_show_type show_type =
		OBS_WATERMARK_SHOW_TYPE_NOT_SHOW;
	switch (watermark->policy) {
	case OBS_WATERMARK_POLICY_CUSTOM:
		show_type = get_show_type_internal(watermark);
		break;
	case OBS_WATERMARK_POLICY_ALWAYS_SHOW:
		show_type = OBS_WATERMARK_SHOW_TYPE_SHOWING;
		break;
	case OBS_WATERMARK_POLICY_NONE:
	default:
		break;
	}
	return show_type;
}

static inline void set_render_size(uint32_t width, uint32_t height)
{
	gs_enable_depth_test(false);
	gs_set_cull_mode(GS_NEITHER);

	gs_ortho(0.0f, (float)width, 0.0f, (float)height, -100.0f, 100.0f);
	gs_set_viewport(0, 0, width, height);
}

static bool reset_texture_and_effect(obs_watermark_t *watermark,
				     const char *watermark_file_path)
{
	gs_enter_context(obs->video.graphics);
	if (watermark_file_path) {
		gs_texture_destroy(watermark->texture);
		watermark->texture =
			gs_texture_create_from_file(watermark_file_path);
		if (!watermark->texture) {
			gs_leave_context();
			blog(LOG_WARNING,
			     "Fail to create texture for watermark.");
			return false;
		}
	}

	if (!watermark->render_texture ||
	    watermark->render_width != obs->video.ovi.output_width ||
	    watermark->render_height != obs->video.ovi.output_height) {
		gs_texture_destroy(watermark->render_texture);
		watermark->render_texture =
			gs_texture_create(obs->video.ovi.output_width,
					  obs->video.ovi.output_height, GS_RGBA,
					  1, NULL, GS_RENDER_TARGET);
		if (!watermark->render_texture) {
			gs_texture_destroy(watermark->texture);
			watermark->texture = NULL;
			gs_leave_context();
			blog(LOG_WARNING,
			     "Fail to create render texture for watermark.");
			return false;
		}
		watermark->render_width = obs->video.ovi.output_width;
		watermark->render_height = obs->video.ovi.output_height;
	}

	if (!watermark->opaque_effect) {
		char *filename = obs_find_data_file("opaque_pls.effect");
		watermark->opaque_effect =
			gs_effect_create_from_file(filename, NULL);
		bfree(filename);

		if (!watermark->opaque_effect)
			blog(LOG_WARNING,
			     "Opaque effect for watermark is not created.");
	}

	gs_leave_context();

	return true;
}

static void release_texture_and_effect(obs_watermark_t *watermark)
{
	gs_enter_context(obs->video.graphics);
	gs_texture_destroy(watermark->texture);
	gs_texture_destroy(watermark->render_texture);
	gs_effect_destroy(watermark->opaque_effect);
	watermark->texture = NULL;
	watermark->render_texture = NULL;
	watermark->opaque_effect = NULL;
	gs_leave_context();
}

static gs_texture_t *obs_watermark_render_internal(obs_watermark_t *watermark,
						   gs_texture_t *texture,
						   float opacity)
{
	uint32_t target_width = gs_texture_get_width(watermark->render_texture);
	uint32_t target_height =
		gs_texture_get_height(watermark->render_texture);

	uint32_t base_width = gs_texture_get_width(watermark->texture);
	uint32_t base_height = gs_texture_get_height(watermark->texture);

	struct vec4 clear_color;
	vec4_set(&clear_color, 0.0f, 0.0f, 0.0f, 0.0f);
	gs_set_render_target(watermark->render_texture, NULL);
	gs_clear(GS_CLEAR_COLOR, &clear_color, 1.0f, 0);

	set_render_size(target_width, target_height);

	gs_effect_t *effect = watermark->opaque_effect;
	gs_technique_t *tech = gs_effect_get_technique(effect, "Draw");

	gs_eparam_t *param = gs_effect_get_param_by_name(effect, "opacity");
	gs_effect_set_float(param, 1.0);

	gs_technique_begin(tech);
	gs_technique_begin_pass(tech, 0);

	param = gs_effect_get_param_by_name(effect, "image");
	gs_effect_set_texture(param, texture);

	gs_blend_state_push();
	gs_enable_blending(false);

	gs_draw_sprite(texture, 0, 0, 0);

	param = gs_effect_get_param_by_name(effect, "opacity");
	gs_effect_set_float(param, opacity);

	gs_set_viewport(watermark->left_margin, watermark->top_margin,
			base_width, base_height);

	param = gs_effect_get_param_by_name(effect, "image");
	gs_effect_set_texture(param, watermark->texture);

	gs_reset_blend_state();

	gs_draw_sprite(watermark->texture, 0, target_width, target_height);

	gs_blend_state_pop();

	gs_technique_end_pass(tech);
	gs_technique_end(tech);

	return watermark->render_texture;
}

static bool obs_watermark_update_internal(obs_watermark_t *watermark,
					  const struct obs_watermark_info *info)
{
	if (!obs_ptr_valid(watermark, "obs_watermark_update_internal"))
		return false;
	else if (!info) {
		blog(LOG_INFO,
		     "watermark info is null, aka, output resolution is not support.");
		watermark->policy = OBS_WATERMARK_POLICY_NONE;
		return false;
	} else if (!obs_ptr_valid(info->file_path,
				  "obs_watermark_update_internal"))
		return false;

	set_info(watermark, info);
	blog(LOG_INFO,
	     "Watermark updated: \n\tfile : %s \n\tposition : (%d, %d) \n\tpolicy : %d \
		\n\tstart_time_usec : %d \n\tshow_time_usec : %d \n\tfade_in_time_usec : %d \
		\n\tfade_out_time_usec : %d \n\tinterval_usec : %d.",
	     info->file_path, info->top_margin, info->left_margin, info->policy,
	     info->start_time_usec, info->show_time_usec,
	     info->fade_in_time_usec, info->fade_out_time_usec,
	     info->interval_usec);
	return true;
}

obs_watermark_t *obs_get_watermark(void)
{
	if (!obs || !obs->video.graphics) {
		blog(LOG_WARNING,
		     "Can not get watermark, because obs or video has not been initialized.");
		return NULL;
	}

	return obs->video.watermark;
}

bool obs_watermark_enabled(const obs_watermark_t *watermark)
{
	if (watermark)
		return os_atomic_load_bool(&watermark->enabled);
	return false;
}

obs_watermark_t *obs_watermark_create(void)
{
	struct obs_watermark *watermark = bzalloc(sizeof(struct obs_watermark));
	if (!watermark) {
		blog(LOG_WARNING, "Fail to alloc watermark.");
		return NULL;
	}

	reset_watermark(watermark);

	if (!reset_texture_and_effect(watermark, NULL)) {
		bfree(watermark);
		watermark = NULL;
	}

	return watermark;
}

void obs_watermark_destroy(obs_watermark_t *watermark)
{
	if (watermark) {
		release_texture_and_effect(watermark);
		reset_watermark(watermark);
		bfree(watermark);
		blog(LOG_INFO, "Watermark destroyed.");
	}
}

gs_texture_t *obs_watermark_render(obs_watermark_t *watermark,
				   gs_texture_t *texture)
{
	if (!obs_ptr_valid(watermark, "obs_watermark_render") ||
	    !obs_ptr_valid(texture, "obs_watermark_render"))
		return texture;

	if (os_atomic_load_bool(&watermark->updated)) {
		if (!reset_texture_and_effect(watermark,
					      watermark->file_path.array))
			return false;
		os_atomic_set_bool(&watermark->updated, false);
	}

	watermark->show_type = get_show_type(watermark);

	float opacity = 1.0f;
	int64_t time_offset = watermark->next_timestamp - os_gettime_ns();
	if (time_offset > 0) {
		if (watermark->show_type == OBS_WATERMARK_SHOW_TYPE_BEGIN)
			opacity =
				1.0 - ((float)time_offset / FADE_IN_TIME_NSEC);
		else if (watermark->show_type == OBS_WATERMARK_SHOW_TYPE_END)
			opacity = (float)time_offset / FADE_OUT_TIME_NSEC;
	}

	if (watermark->show_type == OBS_WATERMARK_SHOW_TYPE_SHOWING) {
		return obs_watermark_render_internal(watermark, texture,
						     DEFAULT_OPACITY);

	} else if (watermark->show_type == OBS_WATERMARK_SHOW_TYPE_BEGIN ||
		   watermark->show_type == OBS_WATERMARK_SHOW_TYPE_END) {
		return obs_watermark_render_internal(watermark, texture,
						     opacity);
	}

	return texture;
}

void obs_watermark_set_refresh(bool update)
{
	obs_watermark_t *watermark = obs_get_watermark();
	if (watermark) {
		os_atomic_set_bool(&watermark->need_update, update);
		char *msg = update ? "NEED UPDATE" : "NO NEED TO UPDATE";
		blog(LOG_INFO, "Watermark is set to be %s.", msg);
	}
}

void obs_watermark_set_enabled(bool enabled)
{
	obs_watermark_t *watermark = obs_get_watermark();
	if (watermark) {
		os_atomic_set_bool(&watermark->enabled, enabled);
		char *msg = enabled ? "ON" : "OFF";
		blog(LOG_INFO, "Watermark is %s.", msg);
	}
}

bool obs_watermark_update(const struct obs_watermark_info *info)
{
	obs_watermark_t *watermark = obs_get_watermark();
	if (watermark && os_atomic_load_bool(&watermark->need_update))
		return obs_watermark_update_internal(watermark, info);
}
