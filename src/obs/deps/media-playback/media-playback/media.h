/*
 * Copyright (c) 2017 Hugh Bailey <obs.jim@gmail.com>
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

#pragma once

#include "decode.h"
#include <obs.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4204)
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <util/threading.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

typedef void (*mp_video_cb)(void *opaque, struct obs_source_frame *frame);
typedef void (*mp_audio_cb)(void *opaque, struct obs_source_audio *audio);
typedef void (*mp_stop_cb)(void *opaque, bool thread_active);
//PRISM/ZengQin/20200706/#3179/for media controller
typedef void (*mp_eof_cb)(void *opaque);
//PRISM/WangShaohui/20200117/#281/for source unavailable
typedef void (*mp_error_cb)(void *opaque, bool open_error);
//PRISM/ZengQin/20200709/#3179/for media controller
typedef void (*mp_load_cb)(void *opaque, bool load);
//PRISM/ZengQin/20200716/#3179/for media controller
typedef void (*mp_clear_cb)(void *opaque, bool seeking);
//PRISM/ZengQin/20200729/#3179/for media controller
typedef void (*mp_started_cb)(void *opaque);
typedef void (*mp_state_cb)(void *opaque, enum obs_media_state state,
			    bool file_changed);
//PRISM/LiuHaibin/20201029/#None/media skipped message for BGM
typedef void (*mp_skipped_cb)(void *opaque, const char *url);

struct mp_media_info;
struct mp_media {
	AVFormatContext *fmt;

	mp_video_cb v_preload_cb;
	mp_stop_cb stop_cb;
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	mp_error_cb error_cb;
	mp_video_cb v_cb;
	mp_audio_cb a_cb;
	//PRISM/ZengQin/20200706/#3179/for media controller
	mp_eof_cb eof_cb;
	//PRISM/ZengQin/20200709/#3179/for media controller
	mp_load_cb load_cb;
	//PRISM/ZengQin/20200716/#3179/for media controller
	mp_clear_cb clear_cb;
	//PRISM/ZengQin/20200729/#3179/for media controller
	mp_started_cb started_cb;
	mp_state_cb state_cb;
	//PRISM/LiuHaibin/20201029/#None/media skipped message for BGM
	mp_skipped_cb skipped_cb;

	void *opaque;

	char *path;
	char *format_name;
	int buffering;
	int speed;

	enum AVPixelFormat scale_format;
	struct SwsContext *swscale;
	int scale_linesizes[4];
	uint8_t *scale_pic[4];

	struct mp_decode v;
	struct mp_decode a;
	bool is_local_file;
	bool has_video;
	bool has_audio;
	bool is_file;
	bool eof;
	bool hw;

	struct obs_source_frame obsframe;
	enum video_colorspace cur_space;
	enum video_range_type cur_range;
	enum video_range_type force_range;

	int64_t play_sys_ts;
	int64_t next_pts_ns;
	uint64_t next_ns;
	int64_t start_ts;
	int64_t base_ts;

	uint64_t interrupt_poll_ts;

	pthread_mutex_t mutex;
	os_sem_t *sem;
	bool stopping;
	bool looping;
	bool active;
	bool reset;
	bool kill;

	bool thread_valid;
	pthread_t thread;

	bool pause;
	bool reset_ts;
	//PRISM/LiuHaibin/20200819/#none/for seek
	//bool seek;
	int64_t seek_pos;

	//PRISM/ZengQin/20200618/#3179/for media controller
	bool just_seek;
	bool just_eof;
	bool seek_video;
	int64_t seek_time;
	int64_t start_pos;

	//PRISM/ZengQin/20200723/#3179/for media controller
	int64_t opening_input_ts;
	int64_t read_frame_ts;
	bool open_timeout;
	bool read_timeout;
	bool first_read;

	bool speed_changing;

	//PRISM/LiuHaibin/20200813/#4192/back to start
	bool back_to_start;

	//PRISM/LiuHaibin/20200819/#none/for seek
	DARRAY(int64_t) seek_positions;

	//PRISM/LiuHaibin/20200818/#4309/media update
	bool reopen;
	//PRISM/LiuHaibin/20200820/#4079/for media controller
	bool starting;
	//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
	bool file_changed;
	bool exit_ffmpeg;
	DARRAY(struct mp_media_info) update_info_array;
	int64_t current_v_pts;
	int64_t current_a_pts;

	//PRISM/LiuHaibin/20200831/#4626/for media controller
	int64_t v_start_time;
	int64_t a_start_time;
	int64_t duration;

	bool pause_state_changed;
	bool stop_callback_called;
	//PRISM/LiuHaibin/20200924/#2174/cover for audio, mark if the media object is opened for the first time.
	bool first_time_open;
};

typedef struct mp_media mp_media_t;

struct mp_media_info {
	void *opaque;

	mp_video_cb v_cb;
	mp_video_cb v_preload_cb;
	mp_audio_cb a_cb;
	mp_stop_cb stop_cb;
	//PRISM/ZengQin/20200706/#3179/for media controller
	mp_eof_cb eof_cb;
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	mp_error_cb error_cb;
	//PRISM/ZengQin/20200709/#3179/for media controller
	mp_load_cb load_cb;
	//PRISM/ZengQin/20200716/#3179/for media controller
	mp_clear_cb clear_cb;
	//PRISM/ZengQin/20200729/#3179/for media controller
	mp_started_cb started_cb;
	mp_state_cb state_cb;
	//PRISM/LiuHaibin/20201029/#None/media skipped message for BGM
	mp_skipped_cb skipped_cb;

	const char *path;
	const char *format;
	int buffering;
	int speed;
	enum video_range_type force_range;
	bool hardware_decoding;
	bool is_local_file;

	//PRISM/ZengQin/20200811/#4018/for media controller
	bool is_looping;
	int64_t start_pos;

	//PRISM/LiuHaibin/20200818/#4309/media update
	bool reopen;
	//PRISM/LiuHaibin/20200827/#/file changed
	bool file_changed;
};

extern bool mp_media_init(mp_media_t *media, const struct mp_media_info *info);
extern void mp_media_free(mp_media_t *media);

extern void mp_media_play(mp_media_t *media, bool loop, bool restart);
extern void mp_media_stop(mp_media_t *media);
extern void mp_media_play_pause(mp_media_t *media, bool pause);
extern int64_t mp_get_current_time(mp_media_t *m);
extern void mp_media_seek_to(mp_media_t *m, int64_t pos);
extern bool mp_media_is_update_done(mp_media_t *m);

//PRISM/ZengQin/20200811/#4018/for media controller
extern void mp_media_update(mp_media_t *m, const struct mp_media_info *info);

//PRISM/ZengQin/20200811/#4018/for media controller
extern void mp_media_update(mp_media_t *m, const struct mp_media_info *info);
extern void mp_media_set_pause_state(mp_media_t *m, bool pause,
				     bool notifyStateChanged);

//PRISM/ZengQin/20200827/#none/for loading update
extern bool mp_media_is_open_loading(mp_media_t *m);

//PRISM/ZengQin/20200923/#4787/for loading update
extern bool mp_media_is_loading(mp_media_t *m);

//PRISM/ZengQin/20200902/#none/update invalid duration for local file
extern bool mp_media_is_invalid_durtion_local_file(mp_media_t *m);

//PRISM/ZengQin/20200902/#none/get duration
extern int64_t mp_media_get_duration(mp_media_t *m);

/* #define DETAILED_DEBUG_INFO */

#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(57, 48, 101)
#define USE_NEW_FFMPEG_DECODE_API
#endif

#ifdef __cplusplus
}
#endif
