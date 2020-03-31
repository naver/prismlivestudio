#include "obs.h"
#include "obs-internal.h"
#include "thumbnail.h"

static inline void set_render_size(uint32_t width, uint32_t height)
{
	gs_enable_depth_test(false);
	gs_set_cull_mode(GS_NEITHER);

	gs_ortho(0.0f, (float)width, 0.0f, (float)height, -100.0f, 100.0f);
	gs_set_viewport(0, 0, width, height);
}

static inline gs_effect_t *
get_scale_effect_internal(struct obs_core_video *video, uint32_t width,
			  uint32_t height)
{
	/* if the dimension is under half the size of the original image,
	 * bicubic/lanczos can't sample enough pixels to create an accurate
	 * image, so use the bilinear low resolution effect instead */
	if (width < (video->base_width / 2) &&
	    height < (video->base_height / 2)) {
		return video->bilinear_lowres_effect;
	}

	switch (video->scale_type) {
	case OBS_SCALE_BILINEAR:
		return video->default_effect;
	case OBS_SCALE_LANCZOS:
		return video->lanczos_effect;
	case OBS_SCALE_AREA:
		return video->area_effect;
	case OBS_SCALE_BICUBIC:
	default:;
	}

	return video->bicubic_effect;
}

static inline bool resolution_close(struct obs_core_video *video,
				    uint32_t width, uint32_t height)
{
	long width_cmp = (long)video->base_width - (long)width;
	long height_cmp = (long)video->base_height - (long)height;

	return labs(width_cmp) <= 16 && labs(height_cmp) <= 16;
}

static inline gs_effect_t *get_scale_effect(struct obs_core_video *video,
					    uint32_t width, uint32_t height)
{
	if (resolution_close(video, width, height)) {
		return video->default_effect;
	} else {
		/* if the scale method couldn't be loaded, use either bicubic
		 * or bilinear by default */
		gs_effect_t *effect =
			get_scale_effect_internal(video, width, height);
		if (!effect)
			effect = !!video->bicubic_effect
					 ? video->bicubic_effect
					 : video->default_effect;
		return effect;
	}
}

static bool reset_thumbnail(obs_thumbnail_t *thumbnail, uint32_t width,
			    uint32_t height)
{
	thumbnail->data = brealloc(thumbnail->data, width * height * 4); //RGBA
	if (!thumbnail->data) {
		blog(LOG_WARNING,
		     "fail to alloc thumbnail buffer (width/height: %d/%d)",
		     width, height);
		return false;
	}

	gs_enter_context(obs->video.graphics);
	gs_texture_destroy(thumbnail->texture);
	thumbnail->texture = gs_texture_create(width, height, thumbnail->format,
					       1, NULL, GS_RENDER_TARGET);
	if (!thumbnail->texture) {
		gs_leave_context();
		blog(LOG_WARNING, "Fail to create thumbnail texture");
		bfree(thumbnail->data);
		thumbnail->data = NULL;
		return false;
	}

	gs_stagesurface_destroy(thumbnail->stage_surface);
	thumbnail->stage_surface =
		gs_stagesurface_create(width, height, thumbnail->format);
	if (!thumbnail->stage_surface) {
		gs_texture_destroy(thumbnail->texture);
		gs_leave_context();
		thumbnail->texture = NULL;
		blog(LOG_WARNING, "Fail to create thumbnail stage surface");
		bfree(thumbnail->data);
		thumbnail->data = NULL;
		return false;
	}
	gs_leave_context();

	thumbnail->width = width;
	thumbnail->height = height;
	return true;
}

static void save_texture(obs_thumbnail_t *thumbnail, gs_texture_t *texture)
{
	if (!thumbnail->data || !texture)
		return;

	uint8_t *data[8] = {0};
	uint32_t line_size[8] = {0};

	uint32_t width = gs_texture_get_width(texture);
	uint32_t height = gs_texture_get_height(texture);
	gs_stage_texture(thumbnail->stage_surface, texture);
	if (gs_stagesurface_map(thumbnail->stage_surface, &data, &line_size)) {
		if (line_size[0] == width * 4)
			memcpy(thumbnail->data, data[0], line_size[0] * height);
		else {
			uint32_t copy_length = width * 4;
			for (int i = 0; i < height; i++)
				memcpy(thumbnail->data + copy_length * i,
				       data[0] + line_size[0] * i, copy_length);
		}
		gs_stagesurface_unmap(thumbnail->stage_surface);
		os_atomic_set_bool(&thumbnail->ready, true);
		os_atomic_set_bool(&thumbnail->requested, false);
		if (thumbnail->callback)
			thumbnail->callback(thumbnail->callback_param,
					    thumbnail->width, thumbnail->width,
					    true);
	} else
		blog(LOG_WARNING, "fail to map thumbnail surface");
}

static obs_thumbnail_t *create_thumbnail(uint32_t width, uint32_t height)
{
	obs_thumbnail_t *thumbnail = bzalloc(sizeof(struct obs_thumbnail));
	if (!thumbnail) {
		blog(LOG_WARNING, "fail to alloc thumbnail");
		return NULL;
	}

	// The default data format of thumbnail is RGBA
	thumbnail->format = GS_RGBA;

	if (!reset_thumbnail(thumbnail, width, height)) {
		bfree(thumbnail);
		thumbnail = NULL;
	}

	return thumbnail;
}

void obs_thumbnail_destroy(obs_thumbnail_t *thumbnail)
{
	if (thumbnail) {
		if (thumbnail->data)
			bfree(thumbnail->data);

		gs_enter_context(obs->video.graphics);
		gs_texture_destroy(thumbnail->texture);
		gs_stagesurface_destroy(thumbnail->stage_surface);
		gs_leave_context();

		bfree(thumbnail);
		blog(LOG_INFO, "Thumbnail is destroyed");
	}
}

bool obs_thumbnail_requested()
{
	obs_thumbnail_t *thumbnail = obs->video.thumbnail;
	if (thumbnail)
		return os_atomic_load_bool(&thumbnail->requested);
	return false;
}

void obs_thumbnail_save(struct obs_core_video *video)
{
	gs_texture_t *target = video->thumbnail->texture;
	gs_texture_t *base = video->render_texture;
	uint32_t width = gs_texture_get_width(target);
	uint32_t height = gs_texture_get_height(target);
	uint32_t base_width = gs_texture_get_width(base);
	uint32_t base_height = gs_texture_get_height(base);

	gs_effect_t *effect = get_scale_effect(video, width, height);
	gs_technique_t *tech;
	if (video->thumbnail->format == GS_RGBA) {
		tech = gs_effect_get_technique(effect, "DrawAlphaDivide");
	} else {
		if ((effect == video->default_effect) &&
		    (width == base_width) && (height == base_height)) {
			save_texture(video->thumbnail, video->render_texture);
			return;
		}

		tech = gs_effect_get_technique(effect, "Draw");
	}

	gs_eparam_t *image = gs_effect_get_param_by_name(effect, "image");
	gs_eparam_t *bres =
		gs_effect_get_param_by_name(effect, "base_dimension");
	gs_eparam_t *bres_i =
		gs_effect_get_param_by_name(effect, "base_dimension_i");
	size_t passes, i;

	gs_set_render_target(target, NULL);
	set_render_size(width, height);

	if (bres) {
		struct vec2 base;
		vec2_set(&base, (float)base_width, (float)base_height);
		gs_effect_set_vec2(bres, &base);
	}

	if (bres_i) {
		struct vec2 base_i;
		vec2_set(&base_i, 1.0f / (float)base_width,
			 1.0f / (float)base_height);
		gs_effect_set_vec2(bres_i, &base_i);
	}

	gs_effect_set_texture(image, base);

	gs_enable_blending(false);
	passes = gs_technique_begin(tech);
	for (i = 0; i < passes; i++) {
		gs_technique_begin_pass(tech, i);
		gs_draw_sprite(base, 0, width, height);
		gs_technique_end_pass(tech);
	}
	gs_technique_end(tech);
	gs_enable_blending(true);

	save_texture(video->thumbnail, target);
}

void obs_thumbnail_request(uint32_t width, uint32_t height,
			   bool (*callback)(void *param, uint32_t width,
					    uint32_t height,
					    bool request_succeed),
			   void *param)
{
	if (width <= 0 || height <= 0) {
		blog(LOG_WARNING, "wrong width/height (%d/%d)", width, height);
		if (callback)
			callback(param, 0, 0, false);
		return;
	}
	obs_thumbnail_t *thumbnail = obs->video.thumbnail;
	if (!thumbnail) {
		obs->video.thumbnail = thumbnail =
			create_thumbnail(width, height);
		if (!thumbnail) {
			if (callback)
				callback(param, 0, 0, false);
			return;
		}
	}

	if (thumbnail->width != width || thumbnail != height) {
		if (!reset_thumbnail(thumbnail, width, height)) {
			if (callback)
				callback(param, 0, 0, false);
			return;
		}
	}

	thumbnail->callback = callback;
	thumbnail->callback_param = param;
	os_atomic_set_bool(&thumbnail->requested, true);
	blog(LOG_INFO, "Thumbnail is requested : %d x %d", width, height);
}

bool obs_thumbnail_retrieve(void **data, uint32_t *width, uint32_t *height)
{
	obs_thumbnail_t *thumbnail = obs->video.thumbnail;
	if (thumbnail && os_atomic_load_bool(&thumbnail->ready)) {
		*data = thumbnail->data;
		*width = thumbnail->width;
		*height = thumbnail->height;
		return true;
	}
	blog(LOG_WARNING, "Thumbnail is not ready, thumbnail %p, ready %d",
	     thumbnail,
	     thumbnail ? os_atomic_load_bool(&thumbnail->ready) : false);
	return false;
}

void obs_thumbnail_free()
{
	obs_thumbnail_t *thumbnail = obs->video.thumbnail;
	if (thumbnail) {
		if (thumbnail->data) {
			bfree(thumbnail->data);
			thumbnail->data = NULL;
		}

		gs_enter_context(obs->video.graphics);
		gs_texture_destroy(thumbnail->texture);
		gs_stagesurface_destroy(thumbnail->stage_surface);
		gs_leave_context();

		thumbnail->texture = NULL;
		thumbnail->stage_surface = NULL;
		thumbnail->width = 0;
		thumbnail->height = 0;
		thumbnail->callback = NULL;
		thumbnail->callback_param = NULL;
		os_atomic_set_bool(&thumbnail->ready, false);
	}
}
