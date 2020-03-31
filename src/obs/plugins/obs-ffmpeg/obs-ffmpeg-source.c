/*
 * Copyright (c) 2015 John R. Bradley <jrb@turrettech.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <obs-module.h>
#include <util/dstr.h>
#include <util/platform.h>

#include "obs-ffmpeg-compat.h"
#include "obs-ffmpeg-formats.h"

#include <media-playback/media.h>

#define FF_LOG(level, format, ...) \
	blog(level, "[Media Source]: " format, ##__VA_ARGS__)
#define FF_LOG_S(source, level, format, ...)        \
	blog(level, "[Media Source '%s']: " format, \
	     obs_source_get_name(source), ##__VA_ARGS__)
#define FF_BLOG(level, format, ...) \
	FF_LOG_S(s->source, level, format, ##__VA_ARGS__)

struct ffmpeg_source {
	mp_media_t media;
	bool media_valid;
	bool destroy_media;

	struct SwsContext *sws_ctx;
	int sws_width;
	int sws_height;
	enum AVPixelFormat sws_format;
	uint8_t *sws_data;
	int sws_linesize;
	enum video_range_type range;
	obs_source_t *source;
	obs_hotkey_id hotkey;

	char *input;
	char *input_format;
	int buffering_mb;
	int speed_percent;
	bool is_looping;
	bool is_local_file;
	bool is_hw_decoding;
	bool is_clear_on_media_end;
	bool restart_on_activate;
	bool close_when_inactive;

	//PRISM/LiuHaibin/20200818/#None/comment out useless code
	//bool seekable;

	enum obs_media_state state;
	obs_hotkey_pair_id play_pause_hotkey;
	obs_hotkey_id stop_hotkey;

	//PRISM/ZengQin/20200618/#3179/for media controller
	bool reopen;
	bool reseek;
	bool source_created;

	//PRISM/ZengQin/20200812/#3787/for media controller
	int64_t start_pos;
	//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
	int64_t current_timestamp;

	pthread_mutex_t state_mutex;

	bool network_off;
	bool network_changed;
	//PRISM/LiuHaibin/20201029/#None/mark if current source is BGM source
	bool bgm_source;
};

//PRISM/ZengQin/20201017/#5142/file changed, send openning force
static void set_media_state(void *data, enum obs_media_state state,
			    bool file_changed)
{
	struct ffmpeg_source *s = data;
	bool state_changed = false;
	pthread_mutex_lock(&s->state_mutex);
	state_changed = s->state != state;
	s->state = state;
	pthread_mutex_unlock(&s->state_mutex);
	if (!state_changed && !file_changed)
		return;

	//if (state == OBS_MEDIA_STATE_OPENING)
	//	obs_source_set_capture_valid(s->source, true,
	//				     OBS_SOURCE_ERROR_OK);

	obs_source_media_state_changed(s->source);
}

//PRISM/LiuHaibin/20200831/#None/media state
static enum obs_media_state get_media_state(void *data)
{
	enum obs_media_state state;
	struct ffmpeg_source *s = data;
	pthread_mutex_lock(&s->state_mutex);
	state = s->state;
	pthread_mutex_unlock(&s->state_mutex);
	return state;
}

static bool is_local_file_modified(obs_properties_t *props,
				   obs_property_t *prop, obs_data_t *settings)
{
	UNUSED_PARAMETER(prop);

	bool enabled = obs_data_get_bool(settings, "is_local_file");
	obs_property_t *input = obs_properties_get(props, "input");
	obs_property_t *input_format =
		obs_properties_get(props, "input_format");
	obs_property_t *local_file = obs_properties_get(props, "local_file");
	obs_property_t *looping = obs_properties_get(props, "looping");
	obs_property_t *buffering = obs_properties_get(props, "buffering_mb");
	obs_property_t *close =
		obs_properties_get(props, "close_when_inactive");
	//PRISM/ZengQin/20200710/#3179/for media controller
	//obs_property_t *seekable = obs_properties_get(props, "seekable");
	obs_property_t *speed = obs_properties_get(props, "speed_percent");
	obs_property_set_visible(input, !enabled);
	obs_property_set_visible(input_format, !enabled);
	obs_property_set_visible(buffering, !enabled);
	obs_property_set_visible(close, enabled);
	obs_property_set_visible(local_file, enabled);
	obs_property_set_visible(looping, enabled);
	obs_property_set_visible(speed, enabled);
	//PRISM/ZengQin/20200710/#3179/for media controller
	//obs_property_set_visible(seekable, !enabled);

	return true;
}

static void ffmpeg_source_defaults(obs_data_t *settings)
{
	obs_data_set_default_bool(settings, "is_local_file", true);
	obs_data_set_default_bool(settings, "looping", false);
	/*
	Modify By : Wang Shaohui
	Modify Date : 2020.01.08
	Modify Reason : In multiple views, if values of "clear_on_media_end" and "restart_on_activate" are true,
			video source will be clear after deactiving current scene.
			So we change default values from true to false to avoid this situation.
	*/
	obs_data_set_default_bool(settings, "clear_on_media_end", false);
	obs_data_set_default_bool(settings, "restart_on_activate", false);

	obs_data_set_default_int(settings, "buffering_mb", 2);
	obs_data_set_default_int(settings, "speed_percent", 100);

	//PRISM/Liuying/20200908/for music playlist
	obs_data_set_default_bool(settings, "bgm_source", false);

	obs_data_set_default_bool(settings, "media_load", false);
}

//OBS Modification:
//Liu ying / 20190214 / Related Issue ID
//Reason: code review
//Solution: remove *.gif file in media_filter
static const char *media_filter =
	" (*.mp4 *.ts *.mov *.flv *.mkv *.avi *.mp3 *.ogg *.aac *.wav *.webm);;";
//PRISM/WangShaohui/20200221/#465/remove gif from video settings
static const char *video_filter =
	" (*.mp4 *.ts *.mov *.flv *.mkv *.avi *.webm);;";
static const char *audio_filter = " (*.mp3 *.aac *.ogg *.wav);;";

static obs_properties_t *ffmpeg_source_getproperties(void *data)
{
	struct ffmpeg_source *s = data;
	struct dstr filter = {0};
	struct dstr path = {0};
	UNUSED_PARAMETER(data);

	obs_properties_t *props = obs_properties_create();

	//PRISM/WangShaohui/20200306/#891/for applying settings of media soon
	//obs_properties_set_flags(props, OBS_PROPERTIES_DEFER_UPDATE);

	obs_property_t *prop;
	// use this when obs allows non-readonly paths
	prop = obs_properties_add_bool(props, "is_local_file",
				       obs_module_text("LocalFile"));

	obs_property_set_modified_callback(prop, is_local_file_modified);

	dstr_copy(&filter, obs_module_text("MediaFileFilter.AllMediaFiles"));
	dstr_cat(&filter, media_filter);
	dstr_cat(&filter, obs_module_text("MediaFileFilter.VideoFiles"));
	dstr_cat(&filter, video_filter);
	dstr_cat(&filter, obs_module_text("MediaFileFilter.AudioFiles"));
	dstr_cat(&filter, audio_filter);
	dstr_cat(&filter, obs_module_text("MediaFileFilter.AllFiles"));
	dstr_cat(&filter, " (*.*)");

	if (s && s->input && *s->input) {
		const char *slash;

		dstr_copy(&path, s->input);
		dstr_replace(&path, "\\", "/");
		slash = strrchr(path.array, '/');
		if (slash)
			dstr_resize(&path, slash - path.array + 1);
	}

	obs_properties_add_path(props, "local_file",
				obs_module_text("LocalFile"), OBS_PATH_FILE,
				filter.array, path.array);
	dstr_free(&filter);
	dstr_free(&path);

	prop = obs_properties_add_bool(props, "looping",
				       obs_module_text("Looping"));

	obs_properties_add_bool(props, "restart_on_activate",
				obs_module_text("RestartWhenActivated"));

	prop = obs_properties_add_int_slider(props, "buffering_mb",
					     obs_module_text("BufferingMB"), 1,
					     16, 1);
	obs_property_int_set_suffix(prop, " MB");

	obs_properties_add_text(props, "input", obs_module_text("Input"),
				OBS_TEXT_DEFAULT);

	obs_properties_add_text(props, "input_format",
				obs_module_text("InputFormat"),
				OBS_TEXT_DEFAULT);

#ifndef __APPLE__
	obs_properties_add_bool(props, "hw_decode",
				obs_module_text("HardwareDecode"));
#endif

	obs_properties_add_bool(props, "clear_on_media_end",
				obs_module_text("ClearOnMediaEnd"));

	prop = obs_properties_add_bool(
		props, "close_when_inactive",
		obs_module_text("CloseFileWhenInactive"));

	obs_property_set_long_description(
		prop, obs_module_text("CloseFileWhenInactive.ToolTip"));

	prop = obs_properties_add_int_slider(props, "speed_percent",
					     obs_module_text("SpeedPercentage"),
					     1, 200, 1);
	obs_property_int_set_suffix(prop, "%");

	prop = obs_properties_add_list(props, "color_range",
				       obs_module_text("ColorRange"),
				       OBS_COMBO_TYPE_LIST,
				       OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(prop, obs_module_text("ColorRange.Auto"),
				  VIDEO_RANGE_DEFAULT);
	obs_property_list_add_int(prop, obs_module_text("ColorRange.Partial"),
				  VIDEO_RANGE_PARTIAL);
	obs_property_list_add_int(prop, obs_module_text("ColorRange.Full"),
				  VIDEO_RANGE_FULL);

	//PRISM/ZengQin/20200710/#3179/for media controller
	//obs_properties_add_bool(props, "seekable", obs_module_text("Seekable"));

	return props;
}

static void dump_source_info(struct ffmpeg_source *s, const char *input,
			     const char *input_format)
{
	FF_BLOG(LOG_INFO,
		"settings:\n"
		"\tinput:                   %s\n"
		"\tinput_format:            %s\n"
		"\tspeed:                   %d\n"
		"\tis_looping:              %s\n"
		"\tis_hw_decoding:          %s\n"
		"\tis_clear_on_media_end:   %s\n"
		"\trestart_on_activate:     %s\n"
		"\tclose_when_inactive:     %s",
		input ? input : "(null)",
		input_format ? input_format : "(null)", s->speed_percent,
		s->is_looping ? "yes" : "no", s->is_hw_decoding ? "yes" : "no",
		s->is_clear_on_media_end ? "yes" : "no",
		s->restart_on_activate ? "yes" : "no",
		s->close_when_inactive ? "yes" : "no");
}

static void get_frame(void *opaque, struct obs_source_frame *f)
{
	struct ffmpeg_source *s = opaque;
	obs_source_output_video(s->source, f);
}

static void preload_frame(void *opaque, struct obs_source_frame *f)
{
	struct ffmpeg_source *s = opaque;
	if (s->close_when_inactive)
		return;

	if (s->is_clear_on_media_end || s->is_looping)
		obs_source_preload_video(s->source, f);
}

static void get_audio(void *opaque, struct obs_source_audio *a)
{
	struct ffmpeg_source *s = opaque;
	obs_source_output_audio(s->source, a);
}

//PRISM/ZengQin/20200716/#3179/for media controller
static void media_cache_clear(void *opaque, bool seeking)
{
	struct ffmpeg_source *s = opaque;
	/* We do not destroy texture and reset async_active flag
	 * but only clear frame cache when seeking */
	if (seeking)
		obs_source_clear_video_cache(s->source);
	else
		obs_source_output_video(s->source, NULL);
	obs_source_output_audio(s->source, NULL);
}

static void media_stopped(void *opaque, bool thread_active)
{
	struct ffmpeg_source *s = opaque;

	//PRISM/LiuHaibin/20200831/#None/update current timestamp
	s->current_timestamp = mp_get_current_time(&s->media);

	//PRISM/LiuHaibin/20200803/#None/clear cache
	if (s->is_clear_on_media_end)
		media_cache_clear(s, false);

	//PRISM/ZengQin/20200909/#4832/for media controller
	if (s->media_valid && (s->close_when_inactive || !thread_active)) {
		s->destroy_media = true;
	}

	//PRISM/ZengQin/20200909/#4832/for media controller
	if (thread_active) {
		set_media_state(s, OBS_MEDIA_STATE_STOPPED, false);
	}
}

//PRISM/ZengQin/20200706/#3179/for media controller
static void media_eof(void *opaque)
{
	struct ffmpeg_source *s = opaque;
	obs_source_media_eof(s->source);
}

//PRISM/ZengQin/20200729/#3179/for media controller
static void media_started(void *opaque)
{
	struct ffmpeg_source *s = opaque;

	//PRISM/LiuHaibin/20200831/#None/update current timestamp
	s->current_timestamp = mp_get_current_time(&s->media);

	if (s->media.pause)
		set_media_state(s, OBS_MEDIA_STATE_PAUSED, false);
	else
		set_media_state(s, OBS_MEDIA_STATE_PLAYING, false);
}

//PRISM/Liuying/20200908/#281/for music playlist
static void media_load(void *opaque, bool load)
{
	struct ffmpeg_source *s = opaque;
	obs_source_media_load(s->source, load);
}

//PRISM/WangShaohui/20200117/#281/for source unavailable
static void media_error(void *opaque, bool open_error)
{
	struct ffmpeg_source *s = opaque;
	if (!s->input || 0 == strlen(s->input)) {
		obs_source_set_capture_valid(s->source, true,
					     OBS_SOURCE_ERROR_OK);
	}
	//PRISM/ZengQin/20200928/#4766/read failed don't change source valid status.
	else if (open_error) {
		obs_source_set_capture_valid(
			s->source, false,
			os_is_file_exist(s->input)
				? OBS_SOURCE_ERROR_UNKNOWN
				: OBS_SOURCE_ERROR_NOT_FOUND);
	}

	blog(LOG_INFO, "MP: media_error");
	set_media_state(s, OBS_MEDIA_STATE_ERROR, false);

	//PRISM/LiuHaibin/20200803/#None/clear cache
	media_cache_clear(s, false);
}

//PRISM/LiuHaibin/20201029/#None/media skipped message for BGM
static void media_skipped(void *opaque, const char *url)
{
	struct ffmpeg_source *s = opaque;
	obs_source_media_skipped(s->source, url);
}

static void ffmpeg_source_open(struct ffmpeg_source *s)
{
	if (s->input && *s->input) {
		struct mp_media_info info = {
			.opaque = s,
			.v_cb = get_frame,
			.v_preload_cb = preload_frame,
			.a_cb = get_audio,
			.stop_cb = media_stopped,
			//PRISM/ZengQin/20200706/#3179/for media controller
			.eof_cb = media_eof,
			//PRISM/WangShaohui/20200117/#281/for source unavailable
			.error_cb = media_error,
			//PRISM/ZengQin/20200716/#3179/for media controller
			.clear_cb = media_cache_clear,
			//PRISM/ZengQin/20200729/#3179/for media controller
			.started_cb = media_started,
			.load_cb = media_load,
			.path = s->input,
			.format = s->input_format,
			.buffering = s->buffering_mb * 1024 * 1024,
			.speed = s->speed_percent,
			.force_range = s->range,
			.hardware_decoding = s->is_hw_decoding,
			//PRISM/LiuHaibin/20200818/#None/comment out useless code
			.is_local_file = s->is_local_file /* || s->seekable*/,
			//PRISM/ZengQin/20200812/#3787/for media controller
			.start_pos = s->start_pos,
			.state_cb = set_media_state,
			//PRISM/LiuHaibin/20201029/#None/media skipped message for BGM
			.skipped_cb = s->bgm_source ? media_skipped : NULL};

		s->media_valid = mp_media_init(&s->media, &info);

		//PRISM/WangShaohui/20200117/#281/for source unavailable
		if (s->media_valid) {
			obs_source_set_capture_valid(s->source, true,
						     OBS_SOURCE_ERROR_OK);
		} else {
			obs_source_set_capture_valid(
				s->source, false,
				os_is_file_exist(s->input)
					? OBS_SOURCE_ERROR_UNKNOWN
					: OBS_SOURCE_ERROR_NOT_FOUND);

			//PRISM/LiuHaibin/20200803/#None/clear cache
			media_cache_clear(s, false);
		}
	} else {
		//PRISM/WangShaohui/20200117/#281/for source unavailable
		obs_source_set_capture_valid(s->source, true,
					     OBS_SOURCE_ERROR_OK);

		//PRISM/LiuHaibin/20200803/#None/clear cache
		media_cache_clear(s, false);
	}
}

static void ffmpeg_source_tick(void *data, float seconds)
{
	UNUSED_PARAMETER(seconds);

	struct ffmpeg_source *s = data;
	if (s->destroy_media) {
		if (s->media_valid) {
			mp_media_free(&s->media);
			s->media_valid = false;
		}
		s->destroy_media = false;
	}

	//PRISM/ZengQin/20200911/#4670/for media controller
	if (s->network_off && !s->network_changed) {

		if (s->input && strlen(s->input) &&
		    !os_is_file_exist(s->input)) {
			if (s->media_valid) {
				mp_media_stop(&s->media);
			}
			media_cache_clear(s, false);
			obs_source_set_capture_valid(s->source, false,
						     OBS_SOURCE_ERROR_UNKNOWN);
		}

		s->network_changed = true;
	}
}

static void ffmpeg_source_start(struct ffmpeg_source *s)
{
	if (!s->media_valid)
		ffmpeg_source_open(s);

	if (s->media_valid) {
		mp_media_play(&s->media, s->is_looping, true);

		if (s->is_local_file)
			obs_source_show_preloaded_video(s->source);
		obs_source_media_started(s->source);
	}
}

//PRISM/WangShaohui/20200512/#2637/for checking media's settings changed
static bool ffmpeg_source_setting_changed(struct ffmpeg_source *s,
					  obs_data_t *settings)
{
	bool is_local_file = obs_data_get_bool(settings, "is_local_file");

	char *input = NULL;
	char *input_format = NULL;
	bool is_looping = false;
	bool close_when_inactive = false;

	if (is_local_file) {
		input = (char *)obs_data_get_string(settings, "local_file");
		input_format = NULL;
		is_looping = obs_data_get_bool(settings, "looping");
		close_when_inactive =
			obs_data_get_bool(settings, "close_when_inactive");
	} else {
		input = (char *)obs_data_get_string(settings, "input");
		input_format =
			(char *)obs_data_get_string(settings, "input_format");
		//PRISM/ZengQin/20200709/#3179/for media controller
		//is_looping = false;
		is_looping = obs_data_get_bool(settings, "looping");

		close_when_inactive = true;
	}

#ifndef __APPLE__
	bool is_hw_decoding = obs_data_get_bool(settings, "hw_decode");
#endif
	bool is_clear_on_media_end =
		obs_data_get_bool(settings, "clear_on_media_end");
	bool restart_on_activate =
		obs_data_get_bool(settings, "restart_on_activate");
	enum video_range_type range = (enum video_range_type)obs_data_get_int(
		settings, "color_range");
	int buffering_mb = (int)obs_data_get_int(settings, "buffering_mb");
	int speed_percent = (int)obs_data_get_int(settings, "speed_percent");

	if ((!!s->input) != (!!input)) {
		//PRISM/ZengQin/20200730/#3179/for media controller
		s->reopen = true;
		//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
		s->reseek = false;
		return true;
	}
	if (s->input && input && 0 != strcmp(s->input, input)) {
		//PRISM/ZengQin/20200730/#3179/for media controller
		s->reopen = true;
		//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
		s->reseek = false;
		return true;
	}

#ifndef __APPLE__
	if (s->is_hw_decoding != is_hw_decoding) {
		//PRISM/ZengQin/20200730/#3179/for media controller
		s->reopen = s->reseek = true;
		return true;
	}
#endif

	if ((!!s->input_format) != (!!input_format)) {
		//PRISM/ZengQin/20200811/#3179/for media controller
		s->reopen = s->reseek = true;
		return true;
	}
	if (s->input_format && input_format &&
	    0 != strcmp(s->input_format, input_format)) {
		//PRISM/ZengQin/20200811/#3179/for media controller
		s->reopen = s->reseek = true;
		return true;
	}
	//PRISM/ZengQin/20200811/#4191/for media controller
	if (s->buffering_mb != buffering_mb) {
		s->reopen = s->reseek = true;
		return true;
	}

	if (s->is_local_file == is_local_file && s->is_looping == is_looping &&
	    s->close_when_inactive == close_when_inactive &&
	    s->is_clear_on_media_end == is_clear_on_media_end &&
	    s->restart_on_activate == restart_on_activate &&
	    s->range == range && s->speed_percent == speed_percent) {
		//params no change
		//PRISM/ZengQin/20200907/#none/for media controller
		enum obs_source_error error;
		if (!obs_source_get_capture_valid(s->source, &error)) {
			s->reopen = true;
			s->reseek = false;
			return true;
		}
		return false;
	}

	return true;
}

static void ffmpeg_source_play_pause(void *data, bool pause);
static void ffmpeg_source_update(void *data, obs_data_t *settings)
{
	struct ffmpeg_source *s = data;

	//PRISM/WangShaohui/20200512/#2637/for checking media's settings changed
	bool setting_changed = ffmpeg_source_setting_changed(s, settings);

	bool is_local_file = obs_data_get_bool(settings, "is_local_file");

	char *input;
	char *input_format;

	bfree(s->input);
	bfree(s->input_format);

	//PRISM/Liuying/20200908/for music playlist : support playing same url
	s->bgm_source = obs_data_get_bool(settings, "bgm_source");
	if (s->bgm_source) {
		s->reopen = true;
		s->reseek = false;
	}

	if (is_local_file) {
		input = (char *)obs_data_get_string(settings, "local_file");
		input_format = NULL;
		s->is_looping = obs_data_get_bool(settings, "looping");
		s->close_when_inactive =
			obs_data_get_bool(settings, "close_when_inactive");
	} else {
		input = (char *)obs_data_get_string(settings, "input");
		input_format =
			(char *)obs_data_get_string(settings, "input_format");
		//PRISM/ZengQin/20200709/#3179/for media controller
		//s->is_looping = false;
		s->is_looping = obs_data_get_bool(settings, "looping");

		//PRISM/Liuying/20200908/for music playlist
		s->close_when_inactive = s->bgm_source ? false : true;
	}

	s->input = input ? bstrdup(input) : NULL;
	s->input_format = input_format ? bstrdup(input_format) : NULL;
#ifndef __APPLE__
	s->is_hw_decoding = obs_data_get_bool(settings, "hw_decode");
#endif
	s->is_clear_on_media_end =
		obs_data_get_bool(settings, "clear_on_media_end");
	s->restart_on_activate =
		obs_data_get_bool(settings, "restart_on_activate");
	s->range = (enum video_range_type)obs_data_get_int(settings,
							   "color_range");
	s->buffering_mb = (int)obs_data_get_int(settings, "buffering_mb");
	s->speed_percent = (int)obs_data_get_int(settings, "speed_percent");
	s->is_local_file = is_local_file;
	//PRISM/ZengQin/20200710/#3179/for media controller
	//s->seekable = obs_data_get_bool(settings, "seekable");

	if (s->speed_percent < 1 || s->speed_percent > 200)
		s->speed_percent = 100;

	//PRISM/WangShaohui/20200512/#2637/for checking media's settings changed
	if (!setting_changed && !s->bgm_source) {
		FF_BLOG(LOG_INFO,
			"Settings of media is not changed and won't restart.");
		return;
	}

	//PRISM/ZengQin/20200630/#3179/for media controller
	obs_source_properties_changed(s->source);

	//PRISM/ZengQin/20200618/#3179/for media controller
	if (s->source_created) {
		if (!s->media_valid && s->reopen) {
			/* NOTE! Here we use sync clear.
			 * If we use async clear, the cover of some audio files may be cleared in render loop */
			obs_source_sync_clear(s->source);

			//PRISM/ZengQin/20201029/#5506/for media controller
			/*bool active = obs_source_active(s->source);
			if (!s->close_when_inactive || active)*/
			ffmpeg_source_open(s);

			dump_source_info(s, input, input_format);
			//PRISM/ZengQin/20200914/#4832/for media controller
			//if (!s->restart_on_activate || active)
			//	ffmpeg_source_start(s);
		}
		if (s->media_valid) {
			if (s->reseek)
				s->start_pos = s->current_timestamp * 1000;
			bool input_changed = s->reopen && !s->reseek;
			struct mp_media_info info = {
				.speed = s->speed_percent,
				.force_range = s->range,
				.is_looping = s->is_looping,
				.reopen = s->reopen,
				.start_pos = s->start_pos,
				.path = s->input ? bstrdup(s->input) : NULL,
				.format = s->input_format
						  ? bstrdup(s->input_format)
						  : NULL,
				.buffering = s->buffering_mb * 1024 * 1024,
				.hardware_decoding = s->is_hw_decoding,
				.is_local_file = s->is_local_file,
				.file_changed = input_changed};

			mp_media_update(&s->media, &info);
			bool active = obs_source_active(s->source);
			if (input_changed && s->input && strlen(s->input)) {
				obs_source_sync_clear(s->source);
				mp_media_set_pause_state(&s->media, false,
							 false);
				mp_media_play(&s->media, s->is_looping, false);
			}

			//PRISM/ZengQin/20200907/#4773/for media controller
			if (input_changed)
				obs_source_set_capture_valid(
					s->source, true, OBS_SOURCE_ERROR_OK);
		}
		s->reseek = false;
		s->start_pos = 0;
		s->reopen = false;
	}

	//PRISM/ZengQin/20200630/#3179/for media controller
	if (s->is_clear_on_media_end &&
	    get_media_state(s) == OBS_MEDIA_STATE_STOPPED)
		media_cache_clear(s, false);
}

static const char *ffmpeg_source_getname(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("FFMpegSource");
}

static void restart_hotkey(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey,
			   bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (!pressed)
		return;

	struct ffmpeg_source *s = data;

	//PRISM/ZengQin/20200911/#4670/source become invalid when network disconnect
	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(s->source, &error);

	if (obs_source_showing(s->source) && valid) {
		//PRISM/LiuHaibin/20200901/#None/do not call ffmpeg_source_restart
		if (s->media_valid) {
			mp_media_set_pause_state(&s->media, false, true);
		}

		ffmpeg_source_start(s);
	}
}

static void restart_proc(void *data, calldata_t *cd)
{
	restart_hotkey(data, 0, NULL, true);
	UNUSED_PARAMETER(cd);
}

static void get_duration(void *data, calldata_t *cd)
{
	struct ffmpeg_source *s = data;
	int64_t dur = 0;
	if (s->media.fmt)
		dur = s->media.fmt->duration;

	calldata_set_int(cd, "duration", dur * 1000);
}

static void get_nb_frames(void *data, calldata_t *cd)
{
	struct ffmpeg_source *s = data;
	int64_t frames = 0;

	if (!s->media.fmt) {
		calldata_set_int(cd, "num_frames", frames);
		return;
	}

	int video_stream_index = av_find_best_stream(
		s->media.fmt, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);

	if (video_stream_index < 0) {
		FF_BLOG(LOG_WARNING, "Getting number of frames failed: No "
				     "video stream in media file!");
		calldata_set_int(cd, "num_frames", frames);
		return;
	}

	AVStream *stream = s->media.fmt->streams[video_stream_index];

	if (stream->nb_frames > 0) {
		frames = stream->nb_frames;
	} else {
		FF_BLOG(LOG_DEBUG, "nb_frames not set, estimating using frame "
				   "rate and duration");
		AVRational avg_frame_rate = stream->avg_frame_rate;
		frames = (int64_t)ceil((double)s->media.fmt->duration /
				       (double)AV_TIME_BASE *
				       (double)avg_frame_rate.num /
				       (double)avg_frame_rate.den);
	}

	calldata_set_int(cd, "num_frames", frames);
}

static bool ffmpeg_source_play_hotkey(void *data, obs_hotkey_pair_id id,
				      obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (!pressed)
		return false;

	struct ffmpeg_source *s = data;

	//PRISM/ZengQin/20200911/#4670/source become invalid when network disconnect
	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(s->source, &error);

	//PRISM/LiuHaibin/20200831/#/for media controller
	if (obs_source_showing(s->source) && valid) {
		obs_source_media_play_pause(s->source, false);
		return true;
	}
	return false;
}

static bool ffmpeg_source_pause_hotkey(void *data, obs_hotkey_pair_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (!pressed)
		return false;

	struct ffmpeg_source *s = data;

	//PRISM/ZengQin/20200911/#4670/source become invalid when network disconnect
	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(s->source, &error);

	//PRISM/LiuHaibin/20200831/#/for media controller
	if (obs_source_showing(s->source) && valid) {
		obs_source_media_play_pause(s->source, true);
		return true;
	}

	return false;
}

static void ffmpeg_source_stop_hotkey(void *data, obs_hotkey_id id,
				      obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (!pressed)
		return;

	struct ffmpeg_source *s = data;

	//PRISM/ZengQin/20200911/#4670/source become invalid when network disconnect
	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(s->source, &error);

	if (obs_source_showing(s->source) && valid)
		obs_source_media_stop(s->source);
}

static void ffmpeg_source_destroy(void *data);
static void *ffmpeg_source_create(obs_data_t *settings, obs_source_t *source)
{
	UNUSED_PARAMETER(settings);

	//PRISM/WangShaohui/20200117/#281/for source unavailable
	obs_source_set_capture_valid(source, true, OBS_SOURCE_ERROR_OK);

	struct ffmpeg_source *s = bzalloc(sizeof(struct ffmpeg_source));
	s->source = source;

	s->hotkey = obs_hotkey_register_source(source, "MediaSource.Restart",
					       obs_module_text("RestartMedia"),
					       restart_hotkey, s);

	s->play_pause_hotkey = obs_hotkey_pair_register_source(
		s->source, "MediaSource.Play", obs_module_text("Play"),
		"MediaSource.Pause", obs_module_text("Pause"),
		ffmpeg_source_play_hotkey, ffmpeg_source_pause_hotkey, s, s);

	s->stop_hotkey = obs_hotkey_register_source(source, "MediaSource.Stop",
						    obs_module_text("Stop"),
						    ffmpeg_source_stop_hotkey,
						    s);

	proc_handler_t *ph = obs_source_get_proc_handler(source);
	proc_handler_add(ph, "void restart()", restart_proc, s);
	proc_handler_add(ph, "void get_duration(out int duration)",
			 get_duration, s);
	proc_handler_add(ph, "void get_nb_frames(out int num_frames)",
			 get_nb_frames, s);

	pthread_mutex_init_value(&s->state_mutex);
	if (pthread_mutex_init(&s->state_mutex, NULL) != 0) {
		ffmpeg_source_destroy(s);
		blog(LOG_WARNING, "FFmpeg source: Failed to init state mutex");
		return NULL;
	}

	//PRISM/ZengQin/20200730/#3179/for media controller
	//s->seekable = true; //default
	s->reopen = false;
	s->reseek = false;
	s->source_created = false;

	s->network_changed = false;

	ffmpeg_source_update(s, settings);

	if (s->media_valid) {
		mp_media_free(&s->media);
		s->media_valid = false;
	}

	bool active = obs_source_active(s->source);
	if (!s->close_when_inactive || active)
		ffmpeg_source_open(s);

	dump_source_info(s, s->input, s->input_format);
	if (!s->restart_on_activate || active)
		ffmpeg_source_start(s);

	//PRISM/ZengQin/20200730/#3179/for media controller
	s->reopen = false;
	s->reseek = false;
	s->source_created = true;

	return s;
}

static void ffmpeg_source_destroy(void *data)
{
	struct ffmpeg_source *s = data;

	if (s->hotkey)
		obs_hotkey_unregister(s->hotkey);
	if (s->media_valid)
		mp_media_free(&s->media);

	pthread_mutex_destroy(&s->state_mutex);

	if (s->sws_ctx != NULL)
		sws_freeContext(s->sws_ctx);
	bfree(s->sws_data);
	bfree(s->input);
	bfree(s->input_format);
	bfree(s);
}

static void ffmpeg_source_activate(void *data)
{
	struct ffmpeg_source *s = data;

	//PRISM/ZengQin/20200911/#4670/source become invalid when network disconnect
	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(s->source, &error);

	if (s->restart_on_activate && valid) {
		obs_source_media_restart(s->source);
	}
}

static void ffmpeg_source_deactivate(void *data)
{
	struct ffmpeg_source *s = data;

	//PRISM/ZengQin/20200911/#4670/source become invalid when network disconnect
	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(s->source, &error);

	if (s->restart_on_activate && valid) {
		if (s->media_valid) {
			mp_media_stop(&s->media);

			if (s->is_clear_on_media_end)
				//PRISM/LiuHaibin/20200803/#None/clear cache
				media_cache_clear(s, false);
		}
	}
}

static void ffmpeg_source_play_pause(void *data, bool pause)
{
	struct ffmpeg_source *s = data;

	mp_media_play_pause(&s->media, pause);
}

static void ffmpeg_source_stop(void *data)
{
	struct ffmpeg_source *s = data;

	if (s->media_valid) {
		mp_media_stop(&s->media);
		//PRISM/LiuHaibin/20200803/#None/clear cache
		media_cache_clear(s, false);
	}
}

static void ffmpeg_source_restart(void *data)
{
	struct ffmpeg_source *s = data;

	//PRISM/ZengQin/20200911/#4670/source become invalid when network disconnect
	enum obs_source_error error;
	bool valid = obs_source_get_capture_valid(s->source, &error);

	if (obs_source_showing(s->source) && valid) {
		bool media_valid = false;
		//PRISM/ZengQin/20200820/#4028/for media controller
		if (s->media_valid) {
			media_valid = true;
			mp_media_set_pause_state(&s->media, false, true);
		}

		ffmpeg_source_start(s);
		//PRISM/ZengQin/20200825/#4465/for media controller
		if (s->destroy_media)
			s->destroy_media = false;
	}
}

static int64_t ffmpeg_source_get_duration(void *data)
{
	struct ffmpeg_source *s = data;
	int64_t dur = 0;

	//PRISM/ZengQin/20200902/#none/for get duration
	if (s->media_valid)
		dur = mp_media_get_duration(&s->media);

	//PRISM/ZengQin/20200709/#3179/for media controller
	return (dur == AV_NOPTS_VALUE || dur <= 0) ? -1 : dur;
}

static int64_t ffmpeg_source_get_time(void *data)
{
	struct ffmpeg_source *s = data;

	//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
	s->current_timestamp = mp_get_current_time(&s->media);
	return s->current_timestamp;
	//return mp_get_current_time(&s->media);
}

static void ffmpeg_source_set_time(void *data, int64_t ms)
{
	struct ffmpeg_source *s = data;

	mp_media_seek_to(&s->media, ms);
}

static enum obs_media_state ffmpeg_source_get_state(void *data)
{
	struct ffmpeg_source *s = data;
	return get_media_state(s);
}

//PRISM/ZengQin/20200616/#3179/for media controller
static bool ffmpeg_source_is_update_done(void *data)
{
	struct ffmpeg_source *s = data;
	return mp_media_is_update_done(&s->media);
}

//PRISM/ZengQin/20200818/#4283/for media controller
static void ffmpeg_source_restart_to_pos(void *data, bool pause, int64_t pos)
{
	struct ffmpeg_source *s = data;

	if (!s->media_valid)
		return;

	mp_media_set_pause_state(&s->media, pause, true);
	mp_media_play(&s->media, s->is_looping, false);
	mp_media_seek_to(&s->media, pos);
}

//PRISM/ZengQin/20200827/#none/for loading update
static void ffmpeg_source_get_private_data(void *data, obs_data_t *data_output)
{
	if (!data_output)
		return;

	if (!data)
		return;

	const char *method = obs_data_get_string(data_output, "method");
	if (!method)
		return;

	struct ffmpeg_source *s = data;
	if (s->media_valid) {
		if (0 == strcmp(method, "media_opening"))
			obs_data_set_bool(data_output, "media_opening",
					  mp_media_is_open_loading(&s->media));

		else if (0 == strcmp(method, "invalid_duration_local_file")) {
			obs_data_set_bool(
				data_output, "invalid_duration_local_file",
				mp_media_is_invalid_durtion_local_file(
					&s->media));
		}

		else if (0 == strcmp(method, "media_load")) {
			obs_data_set_bool(data_output, "media_load",
					  mp_media_is_loading(&s->media));
		}

	} else {
		if (0 == strcmp(method, "media_opening"))
			obs_data_set_bool(data_output, "media_opening", false);

		else if (0 == strcmp(method, "invalid_duration_local_file"))
			obs_data_set_bool(data_output,
					  "invalid_duration_local_file", false);

		else if (0 == strcmp(method, "media_load"))
			obs_data_set_bool(data_output, "media_load", false);
	}

	//PRISM/Liuying/20200925/#4925
	if (0 == strcmp(method, "network_off")) {
		obs_data_set_bool(data_output, "network_off", s->network_off);
	}

	return;
}

//PRISM/ZengQin/20200911/#4670/for media controller
static void ffmpeg_source_network_state_changed(void *data, bool off)
{
	if (!data)
		return;

	struct ffmpeg_source *s = data;
	s->network_off = off;
	s->network_changed = false;
}

struct obs_source_info ffmpeg_source = {
	.id = "ffmpeg_source",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO |
			OBS_SOURCE_DO_NOT_DUPLICATE |
			OBS_SOURCE_CONTROLLABLE_MEDIA,
	.get_name = ffmpeg_source_getname,
	.create = ffmpeg_source_create,
	.destroy = ffmpeg_source_destroy,
	.get_defaults = ffmpeg_source_defaults,
	.get_properties = ffmpeg_source_getproperties,
	.activate = ffmpeg_source_activate,
	.deactivate = ffmpeg_source_deactivate,
	.video_tick = ffmpeg_source_tick,
	.update = ffmpeg_source_update,
	.icon_type = OBS_ICON_TYPE_MEDIA,
	.media_play_pause = ffmpeg_source_play_pause,
	.media_restart = ffmpeg_source_restart,
	.media_stop = ffmpeg_source_stop,
	.media_get_duration = ffmpeg_source_get_duration,
	.media_get_time = ffmpeg_source_get_time,
	.media_set_time = ffmpeg_source_set_time,
	.media_get_state = ffmpeg_source_get_state,
	//PRISM/ZengQin/20200730/#3179/for media controller
	.is_update_done = ffmpeg_source_is_update_done,
	//PRISM/ZengQin/20200818/#4283/for media controller
	.media_restart_to_pos = ffmpeg_source_restart_to_pos,
	//PRISM/ZengQin/20200827/#none/for loading update
	.get_private_data = ffmpeg_source_get_private_data,
	//PRISM/ZengQin/20200911/#4670/for media controller
	.network_state_changed = ffmpeg_source_network_state_changed,
};
