#include "obs.h"
#include "obs-internal.h"
#include "outro.h"

static const uint64_t USEC_TO_NSEC = 1000;
static const uint64_t MSEC_TO_NSEC = 1000000;
static const char *DEFAULT_SOURCE_ID = "ffmpeg_source";
static const char *DEFAULT_SOURCE_NAME = "pls_outro";

static void reset_outro(obs_outro_t *outro)
{
	outro->ready = false;
	outro->active = false;
	outro->thread_initialized = false;
	outro->exit = true;
	outro->stopping_output = false;

	outro->source = NULL;
	outro->texture = NULL;

	outro->render_width = 0;
	outro->render_height = 0;

	outro->timeout_nsec = 0;
	outro->start_time_nsec = 0;

	if (outro->name) {
		bfree(outro->name);
		outro->name = NULL;
	}

	memset(outro->fake_audio_mix_buffer, 0,
	       sizeof(float) * MAX_AUDIO_CHANNELS * AUDIO_OUTPUT_FRAMES);
}

static void activate_outro_source(struct obs_outro *outro)
{
	outro->source->info.activate(outro->source->context.data);
}

static bool activate_outro(struct obs_outro *outro)
{
	outro->start_time_nsec = os_gettime_ns();
	blog(LOG_INFO, "Outro activated, start time %lld.",
	     outro->start_time_nsec);
	return os_atomic_set_bool(&outro->active, true);
}

static void deactivate_outro(struct obs_outro *outro)
{
	outro->source->info.deactivate(outro->source->context.data);
	os_atomic_set_bool(&outro->active, false);
	outro->start_time_nsec = 0;
	blog(LOG_INFO, "Outro deactivated, start time %lld.",
	     outro->start_time_nsec);
}

static bool output_stopped()
{
	struct obs_core_data *data = &obs->data;
	bool output_active = false;
	pthread_mutex_lock(&data->outputs_mutex);
	struct obs_output *output = data->first_output;
	while (output) {
		if (!astrcmpi(obs_output_get_id(output), "rtmp_output")) {
			output_active |= obs_output_active(output);
		}
		output = (struct obs_output *)output->context.next;
	}
	pthread_mutex_unlock(&data->outputs_mutex);

	return !output_active;
}

static void stop_outputs()
{
	struct obs_core_data *data = &obs->data;
	struct obs_outro *outro = obs->video.outro;
	pthread_mutex_lock(&data->outputs_mutex);

	struct obs_output *output = data->first_output;
	while (output) {
		struct obs_output *next =
			(struct obs_output *)output->context.next;
		if (!astrcmpi(obs_output_get_id(output), "rtmp_output"))
			obs_output_stop(output);
		output = next;
	}
	pthread_mutex_unlock(&data->outputs_mutex);
	os_atomic_set_bool(&outro->stopping_output, true);
}

static bool update_source(struct obs_outro *outro, const char *outro_file_path)
{
	if (outro->source) {
		obs_source_destroy(outro->source);
		outro->source = NULL;
	}

	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "local_file", outro_file_path);
	if (!outro->name) {
		outro->source = obs_source_create_private(
			DEFAULT_SOURCE_ID, DEFAULT_SOURCE_NAME, settings);
	} else {
		outro->source = obs_source_create_private(
			DEFAULT_SOURCE_ID, outro->name, settings);
	}

	obs_data_release(settings);

	if (!outro->source) {
		blog(LOG_WARNING, "Fail to create outro source %s",
		     outro->name != NULL ? outro->name : DEFAULT_SOURCE_NAME);
		return false;
	}

	activate_outro_source(outro);

	return true;
}

static bool reset_texture(struct obs_outro *outro)
{
	if (outro->texture &&
	    outro->render_width == obs_source_get_width(outro->source) &&
	    outro->render_height == obs_source_get_height(outro->source))
		return true;

	uint32_t source_width = obs_source_get_width(outro->source);
	uint32_t source_height = obs_source_get_height(outro->source);
	if (source_width == 0 || source_height == 0) {
		blog(LOG_WARNING,
		     "Width[%d] / Height[%d] of Outro source still not available.",
		     source_width, source_height);
		return false;
	}

	gs_enter_context(obs->video.graphics);
	gs_texture_destroy(outro->texture);
	outro->texture = gs_texture_create(source_width, source_height, GS_RGBA,
					   1, NULL, GS_RENDER_TARGET);

	if (!outro->texture) {
		gs_leave_context();
		blog(LOG_WARNING, "Fail to create outro texture, w/h : %d/%d",
		     source_width, source_height);
		return false;
	}
	gs_leave_context();

	outro->render_width = source_width;
	outro->render_height = source_height;

	return true;
}

static void release_texture(struct obs_outro *outro)
{
	if (outro->texture) {
		gs_enter_context(obs->video.graphics);
		gs_texture_destroy(outro->texture);
		gs_leave_context();
		outro->texture = NULL;
	}
}

static bool start_thread(struct obs_outro *outro)
{
	os_atomic_set_bool(&outro->exit, false);
	int errorcode = pthread_create(&outro->outro_thread, NULL,
				       obs_outro_thread, obs);
	if (errorcode != 0) {
		blog(LOG_WARNING, "Fail to create outro thread, errorcode %d",
		     errorcode);
		return false;
	}

	outro->thread_initialized = true;
	return true;
}

static inline void set_render_size(uint32_t width, uint32_t height)
{
	gs_enable_depth_test(false);
	gs_set_cull_mode(GS_NEITHER);

	gs_ortho(0.0f, (float)width, 0.0f, (float)height, -100.0f, 100.0f);
	gs_set_viewport(0, 0, width, height);
}

static inline gs_effect_t *
get_scale_effect_internal(struct obs_core_video *video)
{
	/* if the dimension is under half the size of the original image,
	 * bicubic/lanczos can't sample enough pixels to create an accurate
	 * image, so use the bilinear low resolution effect instead */
	if (video->output_width < (video->outro->render_width / 2) &&
	    video->output_height < (video->outro->render_height / 2)) {
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
	long width_cmp = (long)video->outro->render_width - (long)width;
	long height_cmp = (long)video->outro->render_height - (long)height;

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
		gs_effect_t *effect = get_scale_effect_internal(video);
		if (!effect)
			effect = !!video->bicubic_effect
					 ? video->bicubic_effect
					 : video->default_effect;
		return effect;
	}
}

static gs_texture_t *render_outro_internal(obs_outro_t *outro)
{
	if (outro) {
		if (!reset_texture(outro))
			return NULL;

		struct vec4 clear_color;
		vec4_set(&clear_color, 0.0f, 0.0f, 0.0f, 0.0f);

		gs_set_render_target(obs->video.outro->texture, NULL);
		gs_clear(GS_CLEAR_COLOR, &clear_color, 1.0f, 0);

		set_render_size(outro->render_width, outro->render_height);

		struct obs_source *source = obs->video.outro->source;
		if (source) {
			if (source->removed) {
				obs_source_release(source);
			} else {
				obs_source_video_render(source);
			}
		}

		return obs->video.outro->texture;
	}
	return NULL;
}

static bool obs_outro_update(obs_outro_t *outro, const char *outro_file_path,
			     uint64_t timeout_usec)
{
	if (!obs_ptr_valid(outro, "obs_outro_update") ||
	    !obs_ptr_valid(outro_file_path, "obs_outro_update") ||
	    timeout_usec == 0) {
		blog(LOG_WARNING,
		     "Bad parameters for outro, timeout_usec %lld.",
		     timeout_usec);
		return false;
	}

	// outro is active, do not allow update
	if (obs_outro_active(outro)) {
		blog(LOG_WARNING, "Outro is active, do not allow update.");
		return false;
	}

	outro->timeout_nsec = timeout_usec * USEC_TO_NSEC;
	if (!update_source(outro, outro_file_path))
		return false;

	os_atomic_set_bool(&outro->ready, true);

	blog(LOG_INFO, "Outro updated: \n\tfile : %s \n\ttimeout_usec : %lld.",
	     outro_file_path, timeout_usec);

	return true;
}

static bool obs_outro_start(obs_outro_t *outro)
{
	if (!obs_ptr_valid(outro, "obs_outro_start"))
		return false;

	if (output_stopped()) {
		blog(LOG_WARNING,
		     "Can not start outro when there is no output enabled.");
		return false;
	}

	if (!os_atomic_load_bool(&outro->ready)) {
		blog(LOG_WARNING, "Outro is not ready to start.");
		return false;
	}

	os_atomic_set_bool(&outro->stopping_output, false);

	if (!outro->thread_initialized) {
		if (!start_thread(outro))
			return false;
	}
	activate_outro(outro);

	return true;
}

static void obs_outro_stop(obs_outro_t *outro)
{
	if (outro) {
		if (obs_outro_active(outro)) {
			stop_outputs();
			deactivate_outro(outro);
		}

		void *thread_retval;
		if (outro->thread_initialized) {
			os_atomic_set_bool(&outro->exit, true);
			pthread_join(outro->outro_thread, &thread_retval);
			outro->thread_initialized = false;
		}
		blog(LOG_INFO, "Outro stopped.");
	}
}

obs_outro_t *obs_get_outro(void)
{
	if (!obs || !obs->video.graphics)
		return NULL;
	return obs->video.outro;
}

obs_outro_t *obs_outro_create(const char *name)
{
	struct obs_outro *outro = bzalloc(sizeof(struct obs_outro));
	if (!outro) {
		blog(LOG_WARNING, "Fail to alloc outro.");
		return NULL;
	}

	reset_outro(outro);
	if (name) {
		int len = sizeof(char) * strlen(name);
		outro->name = (char *)bzalloc(len + 1);
		if (outro->name) {
			memset(outro->name, 0, len + 1);
			memcpy(outro->name, name, len);
		}
	}

	blog(LOG_INFO, "Outro [%s] is created.", name);

	return outro;
}

void obs_outro_destroy(obs_outro_t *outro)
{
	if (outro) {
		obs_outro_stop(outro);
		obs_source_destroy(outro->source);
		release_texture(outro);
		reset_outro(outro);
		bfree(outro);
		blog(LOG_INFO, "Outro is destroyed.");
	}
}

bool obs_outro_active(const obs_outro_t *outro)
{
	if (!obs_ptr_valid(outro, "obs_outro_active"))
		return false;

	return os_atomic_load_bool(&outro->active);
}

void *obs_outro_thread(void *param)
{
	struct obs_outro *outro = obs->video.outro;

	uint64_t interval =
		video_output_get_frame_time(obs->video.video) / MSEC_TO_NSEC;

	while (!os_atomic_load_bool(&outro->exit)) {
		int64_t current_time = os_gettime_ns();
		int64_t time_nsec = current_time - outro->start_time_nsec;
		if (outro->timeout_nsec > 0 && outro->start_time_nsec > 0 &&
		    time_nsec > outro->timeout_nsec &&
		    !os_atomic_load_bool(&outro->stopping_output)) {
			blog(LOG_INFO, "Outro timeout, try to stop outputs");
			stop_outputs();
		}

		if (output_stopped() && obs_outro_active(outro)) {
			blog(LOG_INFO, "Output is stopped, deactivate outro");
			deactivate_outro(outro);
		}

		os_sleep_ms(interval);
	}

	UNUSED_PARAMETER(param);
	return NULL;
}

gs_texture_t *obs_outro_render(struct obs_core_video *video)
{
	if (video && video->outro) {
		if (!reset_texture(video->outro))
			return NULL;
		
		uint32_t base_width = video->outro->render_width;
		uint32_t base_height = video->outro->render_height;

		gs_texture_t *texture = render_outro_internal(video->outro);

		gs_texture_t *target = video->output_texture;
		uint32_t width = gs_texture_get_width(target);
		uint32_t height = gs_texture_get_height(target);

		gs_effect_t *effect = get_scale_effect(video, width, height);
		gs_technique_t *tech;

		if (video->ovi.output_format == VIDEO_FORMAT_RGBA) {
			tech = gs_effect_get_technique(effect,
						       "DrawAlphaDivide");
		} else {
			if ((effect == video->default_effect) &&
			    (width == base_width) && (height == base_height))
				return texture;

			tech = gs_effect_get_technique(effect, "Draw");
		}

		gs_eparam_t *image =
			gs_effect_get_param_by_name(effect, "image");
		gs_eparam_t *bres =
			gs_effect_get_param_by_name(effect, "base_dimension");
		gs_eparam_t *bres_i =
			gs_effect_get_param_by_name(effect, "base_dimension_i");
		size_t passes, i;

		gs_set_render_target(target, NULL);

		gs_enable_depth_test(false);
		gs_set_cull_mode(GS_NEITHER);

		gs_ortho(0.0f, (float)width, 0.0f, (float)height, -100.0f,
			 100.0f);

		uint32_t x = 0;
		uint32_t y = 0;
		uint32_t cx = 0;
		uint32_t cy = 0;

		double target_ratio = (double)width / (double)height;
		double base_ratio = (double)base_width / (double)base_height;
		if (target_ratio > base_ratio) {
			cy = height;
			cx = cy * base_ratio;
		} else {
			cx = width;
			cy = cx / base_ratio;
		}

		x = (width - cx) / 2;
		y = (height - cy) / 2;

		gs_set_viewport(x, y, cx, cy);

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

		gs_effect_set_texture(image, texture);

		gs_enable_blending(false);
		passes = gs_technique_begin(tech);
		for (i = 0; i < passes; i++) {
			gs_technique_begin_pass(tech, i);
			gs_draw_sprite(texture, 0, cx, cy);
			gs_technique_end_pass(tech);
		}
		gs_technique_end(tech);
		gs_enable_blending(true);

		return target;
	}
	return NULL;
}

bool obs_outro_activate(const char *outro_file_path, uint64_t timeout_usec)
{
	obs_outro_t *outro = obs_get_outro();
	if (outro) {
		if (obs_outro_active(outro)) {
			blog(LOG_WARNING, "Outro is already started");
			return false;
		}

		if (!obs_outro_update(outro, outro_file_path, timeout_usec))
			return false;

		return obs_outro_start(outro);
	}

	blog(LOG_WARNING, "Outro is not exist.");
	return false;
}
