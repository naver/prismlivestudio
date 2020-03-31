#include <obs.h>
#include <stdio.h>
#include <util/dstr.h>
#include <graphics/image-file.h>
#include <util/platform.h>
#include "prism-sticker-source.hpp"

#define debug(format, ...) blog(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) blog(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) blog(LOG_WARNING, format, ##__VA_ARGS__)

static time_t get_modified_timestamp(const char *filename)
{
	struct stat stats;
	if (os_stat(filename, &stats) != 0)
		return -1;
	return stats.st_mtime;
}

static const char *PRISMStickerSourceGetName(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("PRISMStickerSource");
}

struct sticker_source {
	obs_source_t *source;

	char *file;
	char *id;
	char *type;
	char *title;
	char *rating;
	uint32_t preview_width;
	uint32_t preview_height;
	uint32_t original_width;
	uint32_t original_height;
	char *preview_url;
	char *original_url;

	bool persistent;
	time_t file_timestamp;
	float update_time_elapsed;
	uint64_t last_time;
	bool active;

	gs_image_file2_t if2;
};

static const char *image_source_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("PRISMStickerSource");
}

static void image_source_load(struct sticker_source *context)
{
	char *file = context->file;

	obs_enter_graphics();
	gs_image_file2_free(&context->if2);
	obs_leave_graphics();

	if (file && *file) {
		debug("loading texture '%s'", file);
		context->file_timestamp = get_modified_timestamp(file);
		gs_image_file2_init(&context->if2, file);
		context->update_time_elapsed = 0;

		obs_enter_graphics();
		gs_image_file2_init_texture(&context->if2);
		obs_leave_graphics();

		if (!context->if2.image.loaded)
			warn("failed to load texture '%s'", file);
	}
}

static void image_source_unload(struct sticker_source *context)
{
	obs_enter_graphics();
	gs_image_file2_free(&context->if2);
	obs_leave_graphics();
}

static void image_source_update(void *data, obs_data_t *settings)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);
	const char *file = obs_data_get_string(settings, "file");
	const char *id = obs_data_get_string(settings, "id");
	const char *type = obs_data_get_string(settings, "type");
	const char *title = obs_data_get_string(settings, "title");
	const char *rating = obs_data_get_string(settings, "rating");
	uint32_t preview_width = (uint32_t)obs_data_get_int(settings, "preview_width");
	uint32_t preview_height = (uint32_t)obs_data_get_int(settings, "preview_height");
	uint32_t original_width = (uint32_t)obs_data_get_int(settings, "original_width");
	uint32_t original_height = (uint32_t)obs_data_get_int(settings, "original_height");
	const char *preview_url = obs_data_get_string(settings, "preview_url");
	const char *original_url = obs_data_get_string(settings, "original_url");
	const bool unload = obs_data_get_bool(settings, "unload");

	if (context->file)
		bfree(context->file);
	context->file = bstrdup(file);

	if (context->id)
		bfree(context->id);
	context->id = bstrdup(id);

	if (context->type)
		bfree(context->type);
	context->type = bstrdup(type);

	if (context->title)
		bfree(context->title);
	context->title = bstrdup(title);

	if (context->rating)
		bfree(context->rating);
	context->rating = bstrdup(rating);

	context->preview_width = preview_width;
	context->preview_height = preview_height;
	context->original_width = original_width;
	context->original_height = original_height;

	if (context->preview_url)
		bfree(context->preview_url);
	context->preview_url = bstrdup(preview_url);

	if (context->original_url)
		bfree(context->original_url);
	context->original_url = bstrdup(original_url);

	context->persistent = !unload;

	/* Load the image if the source is persistent or showing */
	if (context->persistent || obs_source_showing(context->source))
		image_source_load(context);
	else
		image_source_unload(context);
}

static void image_source_defaults(obs_data_t *settings)
{
	obs_data_set_default_bool(settings, "unload", false);
}

static void image_source_show(void *data)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);

	if (!context->persistent)
		image_source_load(context);
}

static void image_source_hide(void *data)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);

	if (!context->persistent)
		image_source_unload(context);
}

static void *image_source_create(obs_data_t *settings, obs_source_t *source)
{
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);

	struct sticker_source *context = reinterpret_cast<sticker_source *>(bzalloc(sizeof(struct sticker_source)));
	context->source = source;

	image_source_update(context, settings);
	return context;
}

static void image_source_destroy(void *data)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);

	image_source_unload(context);

	if (context->file)
		bfree(context->file);
	if (context->id)
		bfree(context->id);
	if (context->type)
		bfree(context->type);
	if (context->title)
		bfree(context->title);
	if (context->rating)
		bfree(context->rating);
	if (context->preview_url)
		bfree(context->preview_url);
	if (context->original_url)
		bfree(context->original_url);

	bfree(context);
}

static uint32_t image_source_getwidth(void *data)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);
	return context->if2.image.cx;
}

static uint32_t image_source_getheight(void *data)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);
	return context->if2.image.cy;
}

static void image_source_render(void *data, gs_effect_t *effect)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);

	//PRISM/WangShaohui/20200117/#281/for source unavailable
	if (context->if2.image.texture || (!context->file || 0 == strlen(context->file))) {
		obs_source_set_capture_valid(context->source, true, OBS_SOURCE_ERROR_OK);
	} else {
		obs_source_set_capture_valid(context->source, false, os_is_file_exist(context->file) ? OBS_SOURCE_ERROR_UNKNOWN : OBS_SOURCE_ERROR_NOT_FOUND);
	}

	if (!context->if2.image.texture)
		return;

	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), context->if2.image.texture);
	gs_draw_sprite(context->if2.image.texture, 0, context->if2.image.cx, context->if2.image.cy);
}

static void image_source_tick(void *data, float seconds)
{
	struct sticker_source *context = reinterpret_cast<sticker_source *>(data);
	uint64_t frame_time = obs_get_video_frame_time();

	context->update_time_elapsed += seconds;

	if (context->update_time_elapsed >= 1.0f) {
		time_t t = get_modified_timestamp(context->file);
		context->update_time_elapsed = 0.0f;

		if (context->file_timestamp != t) {
			image_source_load(context);
		}
	}

	//PRISM/WangShaohui/20200303/#872/for playing deactive gif
	if (!context->active) {
		if (context->if2.image.is_animated_gif)
			context->last_time = frame_time;
		context->active = true;
	}

	if (context->last_time && context->if2.image.is_animated_gif) {
		uint64_t elapsed = frame_time - context->last_time;
		bool updated = gs_image_file2_tick(&context->if2, elapsed);

		if (updated) {
			obs_enter_graphics();
			gs_image_file2_update_texture(&context->if2);
			obs_leave_graphics();
		}
	}

	context->last_time = frame_time;
}

uint64_t image_source_get_memory_usage(void *data)
{
	struct sticker_source *s = reinterpret_cast<sticker_source *>(data);
	return s->if2.mem_usage;
}

void RegisterPRISMStickerSource()
{
	obs_source_info info = {};
	info.id = "prism_sticker_source";
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_VIDEO;
	info.get_name = image_source_get_name;
	info.create = image_source_create;
	info.destroy = image_source_destroy;
	info.update = image_source_update;
	info.get_defaults = image_source_defaults;
	info.show = image_source_show;
	info.hide = image_source_hide;
	info.get_width = image_source_getwidth;
	info.get_height = image_source_getheight;
	info.video_render = image_source_render;
	info.video_tick = image_source_tick;
	info.icon_type = OBS_ICON_TYPE_GIPHY;

	obs_register_source(&info);
}
