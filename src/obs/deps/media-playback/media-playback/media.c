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

#include <obs.h>
#include <util/platform.h>

#include <assert.h>

#include "media.h"
#include "closest-format.h"

#include <libavdevice/avdevice.h>
#include <libavutil/imgutils.h>

//PRISM/ZengQin/20200724/#3179/for media controller and free bgm
static const uint64_t TIMEOUT = 100000000; // 10ms

static int64_t base_sys_ts = 0;
//PRISM/ZengQin/20200623/#3179/for media controller
static bool mp_media_reset(mp_media_t *m, bool starting);

static inline enum video_format convert_pixel_format(int f)
{
	switch (f) {
	case AV_PIX_FMT_NONE:
		return VIDEO_FORMAT_NONE;
	case AV_PIX_FMT_YUV420P:
		return VIDEO_FORMAT_I420;
	case AV_PIX_FMT_NV12:
		return VIDEO_FORMAT_NV12;
	case AV_PIX_FMT_YUYV422:
		return VIDEO_FORMAT_YUY2;
	case AV_PIX_FMT_YUV444P:
		return VIDEO_FORMAT_I444;
	case AV_PIX_FMT_UYVY422:
		return VIDEO_FORMAT_UYVY;
	case AV_PIX_FMT_RGBA:
		return VIDEO_FORMAT_RGBA;
	case AV_PIX_FMT_BGRA:
		return VIDEO_FORMAT_BGRA;
	case AV_PIX_FMT_BGR0:
		return VIDEO_FORMAT_BGRX;
	case AV_PIX_FMT_YUVA420P:
		return VIDEO_FORMAT_I40A;
	case AV_PIX_FMT_YUVA422P:
		return VIDEO_FORMAT_I42A;
	case AV_PIX_FMT_YUVA444P:
		return VIDEO_FORMAT_YUVA;
	default:;
	}

	return VIDEO_FORMAT_NONE;
}

static inline enum audio_format convert_sample_format(int f)
{
	switch (f) {
	case AV_SAMPLE_FMT_U8:
		return AUDIO_FORMAT_U8BIT;
	case AV_SAMPLE_FMT_S16:
		return AUDIO_FORMAT_16BIT;
	case AV_SAMPLE_FMT_S32:
		return AUDIO_FORMAT_32BIT;
	case AV_SAMPLE_FMT_FLT:
		return AUDIO_FORMAT_FLOAT;
	case AV_SAMPLE_FMT_U8P:
		return AUDIO_FORMAT_U8BIT_PLANAR;
	case AV_SAMPLE_FMT_S16P:
		return AUDIO_FORMAT_16BIT_PLANAR;
	case AV_SAMPLE_FMT_S32P:
		return AUDIO_FORMAT_32BIT_PLANAR;
	case AV_SAMPLE_FMT_FLTP:
		return AUDIO_FORMAT_FLOAT_PLANAR;
	default:;
	}

	return AUDIO_FORMAT_UNKNOWN;
}

static inline enum speaker_layout convert_speaker_layout(uint8_t channels)
{
	switch (channels) {
	case 0:
		return SPEAKERS_UNKNOWN;
	case 1:
		return SPEAKERS_MONO;
	case 2:
		return SPEAKERS_STEREO;
	case 3:
		return SPEAKERS_2POINT1;
	case 4:
		return SPEAKERS_4POINT0;
	case 5:
		return SPEAKERS_4POINT1;
	case 6:
		return SPEAKERS_5POINT1;
	case 8:
		return SPEAKERS_7POINT1;
	default:
		return SPEAKERS_UNKNOWN;
	}
}

static inline enum video_colorspace convert_color_space(enum AVColorSpace s)
{
	return s == AVCOL_SPC_BT709 ? VIDEO_CS_709 : VIDEO_CS_DEFAULT;
}

static inline enum video_range_type convert_color_range(enum AVColorRange r)
{
	return r == AVCOL_RANGE_JPEG ? VIDEO_RANGE_FULL : VIDEO_RANGE_DEFAULT;
}

static inline struct mp_decode *get_packet_decoder(mp_media_t *media,
						   AVPacket *pkt)
{
	if (media->has_audio && pkt->stream_index == media->a.stream->index)
		return &media->a;
	if (media->has_video && pkt->stream_index == media->v.stream->index)
		return &media->v;

	return NULL;
}

//PRISM/ZengQin/20200723/#3179/for media controller and free bgm
// open timeout is open intpu tream to first read frame
static void set_open_timeout(mp_media_t *m, bool loading)
{
	if (loading) {
		pthread_mutex_lock(&m->mutex);
		m->open_timeout = true;
		pthread_mutex_unlock(&m->mutex);

		if (m->load_cb)
			m->load_cb(m->opaque, true);

		blog(LOG_INFO,
		     "MP: Opening input stream timeout, loading start.");
	} else {
		if (m->load_cb)
			m->load_cb(m->opaque, false);

		pthread_mutex_lock(&m->mutex);
		m->open_timeout = false;
		m->opening_input_ts = 0;
		pthread_mutex_unlock(&m->mutex);

		blog(LOG_INFO,
		     "MP: Opening input stream timeout, loading end.");
	}
}

//PRISM/ZengQin/20200723/#3179/for media controller and free bgm
static void set_read_timeout(mp_media_t *m, bool loading)
{
	if (loading) {
		pthread_mutex_lock(&m->mutex);
		m->read_timeout = true;
		pthread_mutex_unlock(&m->mutex);

		if (m->load_cb)
			m->load_cb(m->opaque, true);

		blog(LOG_INFO,
		     "MP: Reading frame stream timeout, loading start.");
	} else {
		if (m->load_cb)
			m->load_cb(m->opaque, false);

		pthread_mutex_lock(&m->mutex);
		m->read_timeout = false;
		m->read_frame_ts = 0;
		pthread_mutex_unlock(&m->mutex);

		blog(LOG_INFO,
		     "MP: Reading frame stream timeout, loading end.");
	}
}

//PRISM/ZengQin/20200812/#4168/for media controller and bgm
static void mp_media_open_loading_stopped(mp_media_t *m)
{
	pthread_mutex_lock(&m->mutex);
	bool open_timeout = m->open_timeout;
	m->opening_input_ts = 0;
	pthread_mutex_unlock(&m->mutex);

	if (open_timeout) {
		set_open_timeout(m, false);
	}
}

//PRISM/ZengQin/20200908/#none/for media controller and bgm
static void mp_media_read_loading_stopped(mp_media_t *m)
{
	pthread_mutex_lock(&m->mutex);
	bool read_timeout = m->read_timeout;
	m->read_frame_ts = 0;
	pthread_mutex_unlock(&m->mutex);

	if (read_timeout) {
		set_read_timeout(m, false);
	}
}

static int mp_media_next_packet(mp_media_t *media)
{
	AVPacket new_pkt;
	AVPacket pkt;
	av_init_packet(&pkt);
	new_pkt = pkt;

	//PRISM/ZengQin/20200723/#3179/for media controller and free bgm
	pthread_mutex_lock(&media->mutex);
	if (media->first_read)
		media->read_frame_ts = os_gettime_ns();
	pthread_mutex_unlock(&media->mutex);

	int ret = av_read_frame(media->fmt, &pkt);

	mp_media_read_loading_stopped(media);

	if (ret < 0) {
		//PRISM/ZengQin/20201102/#5142 #5546/for bgm and audio file with cover
		if (ret != AVERROR_EOF && ret != AVERROR_EXIT)
			blog(LOG_WARNING, "MP: av_read_frame failed: %s (%d)",
			     av_err2str(ret), ret);
		return ret;
	}

	struct mp_decode *d = get_packet_decoder(media, &pkt);
	if (d && pkt.size) {
		av_packet_ref(&new_pkt, &pkt);
		mp_decode_push_packet(d, &new_pkt);
	}

	av_packet_unref(&pkt);
	return ret;
}

static inline bool mp_media_ready_to_start(mp_media_t *m)
{
	//PRISM/ZengQin/20200713/#3179/for media controller
	if (m->has_audio && !m->a.eof &&
	    (!m->a.frame_ready || (m->a.frame_ready && m->a.seek_flag)))
		return false;
	if (m->has_video && !m->v.eof &&
	    (!m->v.frame_ready || (m->v.frame_ready && m->v.seek_flag)))
		return false;

	//PRISM/ZengQin/20200713/#3179/for media controller
	//if (m->has_audio && !m->a.eof && !m->a.frame_ready)
	//	return false;
	//if (m->has_video && !m->v.eof && !m->v.frame_ready)
	//	return false;
	return true;
}

static inline bool mp_decode_frame(struct mp_decode *d)
{
	//PRISM/ZengQin/20200713/#3179/for media controller
	//return d->frame_ready || mp_decode_next(d);

	//PRISM/ZengQin/20200713/#3179/for media controller
	return (d->frame_ready && !d->seek_flag) || mp_decode_next(d);
}

static inline int get_sws_colorspace(enum AVColorSpace cs)
{
	switch (cs) {
	case AVCOL_SPC_BT709:
		return SWS_CS_ITU709;
	case AVCOL_SPC_FCC:
		return SWS_CS_FCC;
	case AVCOL_SPC_SMPTE170M:
		return SWS_CS_SMPTE170M;
	case AVCOL_SPC_SMPTE240M:
		return SWS_CS_SMPTE240M;
	default:
		break;
	}

	return SWS_CS_ITU601;
}

static inline int get_sws_range(enum AVColorRange r)
{
	return r == AVCOL_RANGE_JPEG ? 1 : 0;
}

#define FIXED_1_0 (1 << 16)

static bool mp_media_init_scaling(mp_media_t *m)
{
	int space = get_sws_colorspace(m->v.decoder->colorspace);
	int range = get_sws_range(m->v.decoder->color_range);
	const int *coeff = sws_getCoefficients(space);

	m->swscale = sws_getCachedContext(NULL, m->v.decoder->width,
					  m->v.decoder->height,
					  m->v.decoder->pix_fmt,
					  m->v.decoder->width,
					  m->v.decoder->height, m->scale_format,
					  SWS_FAST_BILINEAR, NULL, NULL, NULL);
	if (!m->swscale) {
		blog(LOG_WARNING, "MP: Failed to initialize scaler");
		return false;
	}

	sws_setColorspaceDetails(m->swscale, coeff, range, coeff, range, 0,
				 FIXED_1_0, FIXED_1_0);

	int ret = av_image_alloc(m->scale_pic, m->scale_linesizes,
				 m->v.decoder->width, m->v.decoder->height,
				 m->scale_format, 1);
	if (ret < 0) {
		blog(LOG_WARNING, "MP: Failed to create scale pic data");
		return false;
	}

	return true;
}

static bool mp_media_prepare_frames(mp_media_t *m)
{
	while (!mp_media_ready_to_start(m)) {
		if (!m->eof) {
			int ret = mp_media_next_packet(m);
			//PRISM/ZengQin/20201102/#5142 #5546/for bgm and audio file with cover
			if (ret == AVERROR_EOF || ret == AVERROR_EXIT)
				m->eof = true;
			else if (ret < 0)
				return false;
		}

		if (m->has_video && !mp_decode_frame(&m->v))
			return false;
		if (m->has_audio && !mp_decode_frame(&m->a))
			return false;

		//PRISM/ZengQin/20200713/#3179/for media controller
		if (m->v.frame_ready && m->v.seek_flag && !m->eof) {
			//PRISM/LiuHaibin/20200827/#/modify seek timestamp
			int64_t video_pts = m->v.frame_pts;
			if (video_pts >= m->seek_time || m->v.is_cover)
				m->v.seek_flag = false;
			else {
				AVRational frame_rate = av_guess_frame_rate(
					m->fmt, m->v.stream, NULL);
				int64_t interval =
					1 / av_q2d(frame_rate) * AV_TIME_BASE;
				if (video_pts + interval > m->seek_time)
					m->v.seek_flag = false;
				else
					continue;
			}
			//PRISM/ZengQin/20200826/#no issue
			/*In the paused state, seek to the end of the file, the picture is different from the last frame*/
		} else if (m->eof && m->v.seek_flag)
			m->v.seek_flag = false;

		//PRISM/ZengQin/20200713/#3179/for media controller
		if (m->a.frame_ready && m->a.seek_flag && !m->eof) {
			//PRISM/LiuHaibin/20200827/#/modify seek timestamp
			int64_t audio_pts = m->a.frame_pts;
			if (audio_pts >= m->seek_time)
				m->a.seek_flag = false;
			else {
				int64_t interval =
					av_q2d((AVRational){
						m->a.frame->nb_samples,
						m->a.frame->sample_rate}) *
					AV_TIME_BASE;
				if (audio_pts + interval > m->seek_time)
					m->a.seek_flag = false;
				else
					continue;
			}
			//PRISM/ZengQin/20200826/#no issue
			/*In the paused state, seek to the end of the file, the picture is different from the last frame*/
		} else if (m->eof && m->a.seek_flag)
			m->a.seek_flag = false;

		//PRISM/ZengQin/20200901/#4677/Contains audio only, video only, audio and video, audio with cover
		if (m->speed_changing &&
		    (m->v.speed_changed || !m->has_video || m->v.is_cover) &&
		    (m->a.speed_changed || !m->has_audio)) {
			m->speed_changing = false;
		}
	}

	if (m->has_video && m->v.frame_ready && !m->swscale) {
		m->scale_format = closest_format(m->v.frame->format);
		if (m->scale_format != m->v.frame->format) {
			if (!mp_media_init_scaling(m)) {
				return false;
			}
		}
	}

	return true;
}

static inline int64_t mp_media_get_next_min_pts(mp_media_t *m)
{
	int64_t min_next_ns = 0x7FFFFFFFFFFFFFFFLL;

	if (m->has_video && m->v.frame_ready) {
		if (m->v.frame_pts < min_next_ns)
			min_next_ns = m->v.frame_pts;
	}
	if (m->has_audio && m->a.frame_ready) {
		if (m->a.frame_pts < min_next_ns)
			min_next_ns = m->a.frame_pts;
	}

	return min_next_ns;
}

static inline int64_t mp_media_get_base_pts(mp_media_t *m)
{
	int64_t base_ts = 0;

	if (m->has_video && m->v.next_pts > base_ts)
		base_ts = m->v.next_pts;
	if (m->has_audio && m->a.next_pts > base_ts)
		base_ts = m->a.next_pts;

	return base_ts;
}

static inline int64_t mp_media_get_current_pts(mp_media_t *m)
{
	int64_t base_ts = 0;
	//PRISM/ZengQin/20200720/#3179/for media controller
	int64_t start_pts = 0;
	int64_t v_base_ts = 0;
	int64_t a_base_ts = 0;

	//PRISM/ZengQin/20200901/#4677/if audio with cover, don't get video pts
	if (m->has_video && !m->v.is_cover) {
		//PRISM/ZengQin/20200720/#3179/for media controller
		start_pts = m->v_start_time;

		//PRISM/ZengQin/20200806/#3795/for media controller
		if (m->speed != 100) {
			start_pts = av_rescale_q(start_pts,
						 (AVRational){1, m->speed},
						 (AVRational){1, 100});
		}

		v_base_ts = m->current_v_pts /*m->v.next_pts*/ - start_pts;
	}

	if (m->has_audio) {
		//PRISM/ZengQin/20200720/#3179/for media controller
		start_pts = m->a_start_time;

		//PRISM/ZengQin/20200806/#3795/for media controller
		if (m->speed != 100) {
			start_pts = av_rescale_q(start_pts,
						 (AVRational){1, m->speed},
						 (AVRational){1, 100});
		}

		a_base_ts = m->current_a_pts /*m->a.next_pts*/ - start_pts;
	}

	//PRISM/LiuHaibin/20200826/#4491&4482/for media controller and free bgm
	base_ts = FFMAX3(v_base_ts, a_base_ts, 0);

	return base_ts;
}

static inline bool mp_media_can_play_frame(mp_media_t *m, struct mp_decode *d)
{
	//PRISM/ZengQin/20200813/#3795/for media controller
	//return d->frame_ready && (d->frame_pts <= m->next_pts_ns);
	return d->frame_ready &&
	       (d->frame_pts <= m->next_pts_ns || m->speed_changing);
}

static void reset_ts(mp_media_t *m)
{
	m->base_ts += mp_media_get_base_pts(m);
	m->play_sys_ts = (int64_t)os_gettime_ns();
	m->start_ts = m->next_pts_ns = mp_media_get_next_min_pts(m);
	m->next_ns = 0;
}

static void mp_media_next_audio(mp_media_t *m)
{
	struct mp_decode *d = &m->a;
	struct obs_source_audio audio = {0};
	AVFrame *f = d->frame;

	if (!mp_media_can_play_frame(m, d))
		return;

	d->frame_ready = false;
	if (!m->a_cb)
		return;

	for (size_t i = 0; i < MAX_AV_PLANES; i++)
		audio.data[i] = f->data[i];

	audio.samples_per_sec = f->sample_rate * m->speed / 100;
	audio.speakers = convert_speaker_layout(f->channels);
	audio.format = convert_sample_format(f->format);
	audio.frames = f->nb_samples;
	audio.timestamp = m->base_ts + d->frame_pts - m->start_ts +
			  m->play_sys_ts - base_sys_ts;

	if (audio.format == AUDIO_FORMAT_UNKNOWN)
		return;

	//PRISM/LiuHaibin/20200826/#/for media controller and free bgm
	m->current_a_pts = d->frame_pts;

	//PRISM/ZengQin/20200618/#3179/for media controller
	bool need_cb = false;
	pthread_mutex_lock(&m->mutex);
	if (!m->seek_video) {
		if (m->just_seek)
			m->just_seek = false;
		if (!m->first_read) {
			m->first_read = true;
			need_cb = true;
		}
	}
	pthread_mutex_unlock(&m->mutex);
	if (need_cb && m->started_cb) {
		//PRISM/ZengQin/20200812/#4168/for media controller and free bgm
		mp_media_open_loading_stopped(m);
		m->started_cb(m->opaque);
	}

	//PRISM/ZengQin/20200618/#3179/for media controller
	if (m->pause)
		return;

	m->a_cb(m->opaque, &audio);
}

static void mp_media_next_video(mp_media_t *m, bool preload)
{
	struct mp_decode *d = &m->v;
	struct obs_source_frame *frame = &m->obsframe;
	enum video_format new_format;
	enum video_colorspace new_space;
	enum video_range_type new_range;
	AVFrame *f = d->frame;

	if (!preload) {
		if (!mp_media_can_play_frame(m, d))
			return;

		d->frame_ready = false;

		if (!m->v_cb)
			return;
	} else if (!d->frame_ready) {
		return;
	}

	bool flip = false;
	if (m->swscale) {
		int ret = sws_scale(m->swscale, (const uint8_t *const *)f->data,
				    f->linesize, 0, f->height, m->scale_pic,
				    m->scale_linesizes);
		if (ret < 0)
			return;

		flip = m->scale_linesizes[0] < 0 && m->scale_linesizes[1] == 0;
		for (size_t i = 0; i < 4; i++) {
			frame->data[i] = m->scale_pic[i];
			frame->linesize[i] = abs(m->scale_linesizes[i]);
		}

	} else {
		flip = f->linesize[0] < 0 && f->linesize[1] == 0;

		for (size_t i = 0; i < MAX_AV_PLANES; i++) {
			frame->data[i] = f->data[i];
			frame->linesize[i] = abs(f->linesize[i]);
		}
	}

	if (flip)
		frame->data[0] -= frame->linesize[0] * (f->height - 1);

	new_format = convert_pixel_format(m->scale_format);
	new_space = convert_color_space(f->colorspace);
	new_range = m->force_range == VIDEO_RANGE_DEFAULT
			    ? convert_color_range(f->color_range)
			    : m->force_range;

	if (new_format != frame->format || new_space != m->cur_space ||
	    new_range != m->cur_range) {
		bool success;

		frame->format = new_format;
		frame->full_range = new_range == VIDEO_RANGE_FULL;

		success = video_format_get_parameters(new_space, new_range,
						      frame->color_matrix,
						      frame->color_range_min,
						      frame->color_range_max);

		frame->format = new_format;
		m->cur_space = new_space;
		m->cur_range = new_range;

		if (!success) {
			frame->format = VIDEO_FORMAT_NONE;
			return;
		}
	}

	if (frame->format == VIDEO_FORMAT_NONE)
		return;

	frame->timestamp = m->base_ts + d->frame_pts - m->start_ts +
			   m->play_sys_ts - base_sys_ts;
	frame->width = f->width;
	frame->height = f->height;
	frame->flip = flip;

	//PRISM/LiuHaibin/20200820/#None/comment out useless code
	//if (!m->is_local_file && !d->got_first_keyframe) {
	//	if (!f->key_frame)
	//		return;
	//	d->got_first_keyframe = true;
	//}

	//PRISM/LiuHaibin/20200826/#/for media controller and free bgm
	m->current_v_pts = d->frame_pts;

	//PRISM/LiuHaibin/20200924/#2174/cover for audio
	frame->is_cover = d->is_cover;

	if (preload)
		m->v_preload_cb(m->opaque, frame);
	else {
		//PRISM/ZengQin/20200618/#3179/for media controller
		bool need_cb = false;
		pthread_mutex_lock(&m->mutex);
		if (m->seek_video) {
			if (m->just_seek)
				m->just_seek = false;
			if (!m->first_read) {
				m->first_read = true;
				need_cb = true;
			}
		}
		pthread_mutex_unlock(&m->mutex);
		if (need_cb && m->started_cb) {
			//PRISM/ZengQin/20200812/#4168/for media controller and free bgm
			mp_media_open_loading_stopped(m);
			m->started_cb(m->opaque);
		}

		m->v_cb(m->opaque, frame);
	}
}

static void mp_media_calc_next_ns(mp_media_t *m)
{
	int64_t min_next_ns = mp_media_get_next_min_pts(m);

	int64_t delta = min_next_ns - m->next_pts_ns;
	//PRISM/ZengQin/20200618/#3179/for media controller
	//#ifdef _DEBUG
	//	assert(delta >= 0);
	//#endif
	if (delta < 0)
		delta = 0;
	//PRISM/ZengQin/20200910/#4768/for media controller 3000000000 to 9000000000
	if (delta > 9000000000)
		delta = 0;

	m->next_ns += delta;
	m->next_pts_ns = min_next_ns;
}

static void seek_to(mp_media_t *m, int64_t pos)
{
	int stream_index = av_find_default_stream_index(m->fmt);
	AVStream *stream = m->fmt->streams[stream_index];
	int64_t seek_pos = (pos == AV_NOPTS_VALUE) ? 0 : pos;
	int seek_flags;

	//PRISM/ZengQin/20200714/#3179/for media controller
	if (!m->is_local_file && m->fmt && m->fmt->duration <= 0)
		return;

	if (m->fmt->duration == AV_NOPTS_VALUE)
		seek_flags = AVSEEK_FLAG_FRAME;
	else
		seek_flags = AVSEEK_FLAG_BACKWARD;

	int64_t seek_target = seek_flags == AVSEEK_FLAG_BACKWARD
				      ? av_rescale_q(seek_pos, AV_TIME_BASE_Q,
						     stream->time_base)
				      : seek_pos;

	//PRISM/ZengQin/20200713/#3179/for media controllerd
	//int ret = av_seek_frame(m->fmt, stream->index,
	//			seek_target + stream->start_time,
	//			seek_flags);

	//PRISM/ZengQin/20200923/#4787/for media controller
	pthread_mutex_lock(&m->mutex);

	//PRISM/ZengQin/20200929/#5088/for media controller
	bool v_ended = !m->has_video || !m->v.frame_ready;
	bool a_ended = !m->has_audio || !m->a.frame_ready;
	bool eof = v_ended && a_ended;

	if (m->first_read && !eof && m->active)
		m->read_frame_ts = os_gettime_ns();
	pthread_mutex_unlock(&m->mutex);

	//PRISM/ZengQin/20200713/#3179/for media controller
	int ret = avformat_seek_file(m->fmt, stream_index, INT64_MIN,
				     seek_target + stream->start_time,
				     INT64_MAX, seek_flags);

	if (!eof && m->active)
		mp_media_read_loading_stopped(m);

	if (ret < 0) {
		m->just_seek = false;
		blog(LOG_WARNING, "MP: Failed to seek: %s", av_err2str(ret));
		return;
	}

	//PRISM/ZengQin/20200806/#3179/for media controller
	m->just_seek = true;
	m->seek_pos = seek_pos;

	//PRISM/LiuHaibin/20200818/#none/remove useless cpde
	if (m->has_video /* && m->is_local_file*/)
		mp_decode_flush(&m->v);
	if (m->has_audio /* && m->is_local_file*/)
		mp_decode_flush(&m->a);

	//PRISM/LiuHaibin/20200813/#4192/back to start
	/* only clear audio cache when seeking back to start position,
	 * because the first frame of the file may have not been rendered to async texture
	 * before cleared */
	if (m->back_to_start) {
		m->back_to_start = false;
		if (m->a_cb)
			m->a_cb(m->opaque, NULL);
	}
	//PRISM/ZengQin/20200717/#3179/for media controller
	else if (m->clear_cb)
		m->clear_cb(m->opaque, true);

	//PRISM/ZengQin/20200818/#4261/for media controller
	m->seek_time = av_rescale_q(seek_target + stream->start_time,
				    stream->time_base,
				    (AVRational){1, 1000000000});

	//PRISM/ZengQin/20200806/#3795/for media controller
	if (m->speed != 100)
		m->seek_time = av_rescale_q(m->seek_time,
					    (AVRational){1, m->speed},
					    (AVRational){1, 100});

	//PRISM/ZengQin/20200831/#4626/for media controller
	m->current_v_pts = m->current_a_pts = m->seek_time;

	if (m->has_audio) {
		m->a.seek_flag = true;
		if (stream_index == m->a.stream->index)
			m->seek_video = false;
	}

	if (m->has_video) {
		m->v.seek_flag = true;
		if (stream_index == m->v.stream->index)
			m->seek_video = true;
	}
}

//PRISM/ZengQin/20201012/#5142/if read frame failed or media eof, call stop loading and media
static void mp_media_stop_cb(mp_media_t *m)
{
	pthread_mutex_lock(&m->mutex);
	bool stop_cb = m->stopping && m->stop_cb && !m->starting;
	m->starting = false;
	m->stopping = false;
	m->reset = false;
	m->exit_ffmpeg = false;
	pthread_mutex_unlock(&m->mutex);

	if (stop_cb) {
		mp_media_open_loading_stopped(m);
		mp_media_read_loading_stopped(m);

		blog(LOG_INFO, "MP: media stopped.");

		m->stop_cb(m->opaque, true);
		m->stop_callback_called = true;
	}
}

//PRISM/ZengQin/20200811/#4018/for media controller
//static bool mp_media_reset(mp_media_t *m);
static bool mp_media_reset(mp_media_t *m, bool starting)
{
	//PRISM/LiuHaibin/20200820/#4079/for media controller
	//bool stopping;
	bool active;
	int64_t start_pos;

	int64_t next_ts = mp_media_get_base_pts(m);
	int64_t offset = next_ts - m->next_pts_ns;

	m->eof = false;
	m->base_ts += next_ts;
	//PRISM/ZengQin/20200716/#3179/for media controller
	m->just_seek = false;
	m->seek_video = false;
	pthread_mutex_lock(&m->mutex);

	//PRISM/ZengQin/20200811/#4018/for media controller
	//stopping = m->stopping;

	active = m->active;

	//PRISM/LiuHaibin/20200820/#4079/for media controller
	//m->stopping = false;

	//PRISM/ZengQin/20200811/#3787/for media controller
	start_pos = m->start_pos;
	m->start_pos = 0;

	pthread_mutex_unlock(&m->mutex);

	if (starting) {
		seek_to(m, start_pos);
	} else {
		seek_to(m, 0);
	}

	if (!mp_media_prepare_frames(m)) {
		mp_media_stop_cb(m);
		return false;
	}

	if (active) {
		if (!m->play_sys_ts)
			m->play_sys_ts = (int64_t)os_gettime_ns();
		m->start_ts = m->next_pts_ns = mp_media_get_next_min_pts(m);
		if (m->next_ns)
			m->next_ns += offset;
	} else {
		m->start_ts = m->next_pts_ns = mp_media_get_next_min_pts(m);
		m->play_sys_ts = (int64_t)os_gettime_ns();
		m->next_ns = 0;
	}

	//PRISM/ZengQin/20200730/#3179/for media controller
	//m->pause = false;

	//PRISM/LiuHaibin/20200818/#None/comment out useless code
	if (!active /*&& m->is_local_file*/ && m->v_preload_cb)
		mp_media_next_video(m, true);

	mp_media_stop_cb(m);
	return true;
}

static inline bool mp_media_sleepto(mp_media_t *m)
{
	bool timeout = false;

	if (!m->next_ns) {
		m->next_ns = os_gettime_ns();
	} else if (!m->just_seek) {
		uint64_t t = os_gettime_ns();
		const uint64_t timeout_ns = 200000000;

		if (m->next_ns > t && (m->next_ns - t) > timeout_ns) {
			os_sleepto_ns(t + timeout_ns);
			timeout = true;
			blog(LOG_WARNING, "MP: timeout.");
		} else {
			os_sleepto_ns(m->next_ns);
		}
	}

	return timeout;
}

static inline bool mp_media_eof(mp_media_t *m)
{
	bool v_ended = !m->has_video || !m->v.frame_ready;
	bool a_ended = !m->has_audio || !m->a.frame_ready;
	bool eof = v_ended && a_ended;

	//PRISM/ZengQin/20200713/#3179/for media controller
	//if (eof) {
	//	bool looping;

	//	pthread_mutex_lock(&m->mutex);
	//	looping = m->looping;
	//	if (!looping) {
	//		m->active = false;
	//		m->stopping = true;
	//	}
	//	pthread_mutex_unlock(&m->mutex);

	//	mp_media_reset(m);
	//}

	//PRISM/ZengQin/20200713/#3179/for media controller
	bool looping;
	pthread_mutex_lock(&m->mutex);
	looping = m->looping;
	pthread_mutex_unlock(&m->mutex);

	if (eof) {
		pthread_mutex_lock(&m->mutex);
		bool pause = m->pause;
		m->just_seek = false;
		pthread_mutex_unlock(&m->mutex);
		if (pause) {
			pthread_mutex_lock(&m->mutex);
			m->eof = false;
			pthread_mutex_unlock(&m->mutex);
			return false;
		}

		if (m->eof_cb && !m->just_seek)
			m->eof_cb(m->opaque);
	}

	if (!looping) {
		if (eof && !m->just_eof && !m->just_seek) {
			seek_to(m, 0);
			reset_ts(m);

			pthread_mutex_lock(&m->mutex);
			m->eof = false;
			m->just_eof = true;
			pthread_mutex_unlock(&m->mutex);
			return false;
		}

		if (m->just_eof && !m->just_seek) {
			pthread_mutex_lock(&m->mutex);
			m->active = false;
			m->stopping = true;
			m->just_eof = false;
			pthread_mutex_unlock(&m->mutex);

			//PRISM/LiuHaibin/20200813/#4192/back to start
			m->back_to_start = true;

			mp_media_reset(m, false);

			return true;
		}

	} else if (eof) {
		mp_media_reset(m, false);
	}

	return eof;
}

//PRISM/ZengQin/20200723/#3179/for media controller and free bgm
static void timeout_logic(mp_media_t *m)
{
	int64_t open_timeout_interval = 0;
	int64_t read_timeout_interval = 0;
	uint64_t ts = os_gettime_ns();

	pthread_mutex_lock(&m->mutex);
	int64_t open_timeout_ts = m->opening_input_ts;
	bool open_timeout = m->open_timeout;
	int64_t read_timeout_ts = m->read_frame_ts;
	bool read_timeout = m->read_timeout;
	pthread_mutex_unlock(&m->mutex);

	if (open_timeout_ts)
		open_timeout_interval = ts - open_timeout_ts;

	if (!open_timeout && open_timeout_interval > TIMEOUT)
		set_open_timeout(m, true);

	if (read_timeout_ts)
		read_timeout_interval = ts - read_timeout_ts;

	if (!read_timeout && !open_timeout &&
	    read_timeout_interval > TIMEOUT * 10)
		set_read_timeout(m, true);

	return;
}

static int interrupt_callback(void *data)
{
	mp_media_t *m = data;
	bool stop = false;

	//PRISM/ZengQin/20200723/#3179/for media controller and free bgm
	timeout_logic(m);

	//PRISM/ZengQin/20200723/#3179/for media controller and free bgm
	uint64_t ts = os_gettime_ns();
	if ((ts - m->interrupt_poll_ts) > 20000000) {
		pthread_mutex_lock(&m->mutex);
		//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
		stop = m->kill || m->exit_ffmpeg /* || m->stopping*/;
		pthread_mutex_unlock(&m->mutex);

		m->interrupt_poll_ts = ts;
	}

	if (stop)
		blog(LOG_INFO, "MP: FFmpeg interrupt.");

	return stop;
}

static bool init_avformat(mp_media_t *m)
{
	if (m->file_changed && m->state_cb)
		m->state_cb(m->opaque, OBS_MEDIA_STATE_OPENING, true);

	m->file_changed = false;

	AVInputFormat *format = NULL;

	if (m->format_name && *m->format_name) {
		format = av_find_input_format(m->format_name);
		if (!format)
			blog(LOG_INFO,
			     "MP: Unable to find input format for "
			     "'%s'",
			     m->path);
	}

	AVDictionary *opts = NULL;
	if (m->buffering && !m->is_local_file)
		av_dict_set_int(&opts, "buffer_size", m->buffering, 0);

	m->fmt = avformat_alloc_context();
	if (m->buffering == 0) {
		m->fmt->flags |= AVFMT_FLAG_NOBUFFER;
	}
	//PRISM/ZengQin/20200723/#3179/for media controller
	//if (!m->is_local_file) {
	//	m->fmt->interrupt_callback.callback = interrupt_callback;
	//	m->fmt->interrupt_callback.opaque = m;
	//}

	//PRISM/ZengQin/20200723/#3179/for media controller
	m->fmt->interrupt_callback.callback = interrupt_callback;
	m->fmt->interrupt_callback.opaque = m;
	m->opening_input_ts = os_gettime_ns();

	//PRISM/ZengQin/20200817/#4290/for media controller
	av_dict_set(&opts, "http_persistent", "0", 0);
	av_dict_set(&opts, "reconnect", "1", 0); // http reconnection
	av_dict_set(&opts, "stimeout", "3000000",
		    0); // I/O operation timeout for rtsp

	blog(LOG_INFO, "MP: Opening media: '%s'", m->path);

	int ret = avformat_open_input(&m->fmt, m->path, format,
				      opts ? &opts : NULL);
	av_dict_free(&opts);

	if (ret < 0) {
		//PRISM/WangShaohui/20200219/#461/for error code of openning file
		blog(LOG_WARNING, "MP: Failed to open media: [%d] '%s'", ret,
		     m->path);
		return false;
	}

	//PRISM/ZengQin/20200730/#3179/for media controller
	m->first_read = false;

	if (avformat_find_stream_info(m->fmt, NULL) < 0) {
		blog(LOG_WARNING, "MP: Failed to find stream info for '%s'",
		     m->path);
		return false;
	}

	//PRISM/ZengQin/20200706/#3179/for media controller
	for (int i = 0; i < m->fmt->nb_streams; ++i) {
		m->fmt->streams[i]->discard = AVDISCARD_ALL;
	}

	m->has_video = mp_decode_init(m, AVMEDIA_TYPE_VIDEO, m->hw);
	m->has_audio = mp_decode_init(m, AVMEDIA_TYPE_AUDIO, m->hw);

	if (!m->has_video && !m->has_audio) {
		blog(LOG_WARNING,
		     "MP: Could not initialize audio or video: "
		     "'%s'",
		     m->path);
		return false;
	}

	//PRISM/ZengQin/202000803/#3179/for media controller
	m->fmt->start_time =
		(m->fmt->start_time == AV_NOPTS_VALUE) ? 0 : m->fmt->start_time;

	//PRISM/ZengQin/202000803/#3179/for media controller
	if (m->has_video) {
		m->v.stream->start_time =
			(m->v.stream->start_time == AV_NOPTS_VALUE)
				? 0
				: m->v.stream->start_time;

		//PRISM/ZengQin/202000831/#4626/for media controller
		m->v_start_time = av_rescale_q(m->v.stream->start_time,
					       m->v.stream->time_base,
					       (AVRational){1, 1000000000});
	}

	if (m->has_audio) {
		m->a.stream->start_time =
			(m->a.stream->start_time == AV_NOPTS_VALUE)
				? 0
				: m->a.stream->start_time;

		//PRISM/ZengQin/202000831/#4626/for media controller
		m->a_start_time = av_rescale_q(m->a.stream->start_time,
					       m->a.stream->time_base,
					       (AVRational){1, 1000000000});
	}

	//PRISM/ZengQin/20200902/#none/for media controller
	if (m->fmt)
		m->duration = m->fmt->duration;

	//PRISM/WangShaohui/20200312/#1490/for clear texture
	if (!m->has_video && m->v_cb)
		m->v_cb(m->opaque, NULL);

	//PRISM/LiuHaibin/20200803/#None/clear audio cache
	if (!m->has_audio && m->a_cb)
		m->a_cb(m->opaque, NULL);

	return true;
}

//PRISM/LiuHaibin/20200818/#4309/media update
static inline bool mp_media_reopen(mp_media_t *m)
{
	//PRISM/ZengQin/20200917/#none/for media controller and bgm
	mp_media_open_loading_stopped(m);
	mp_media_read_loading_stopped(m);

	/* When the media object is opened for the first time,
	 * the clear callback does not need to be called.
	 * Actually, if it is called, it will clean up the cover of the audio file.*/
	if (m->file_changed && m->clear_cb && !m->first_time_open) {
		// The clear operation here should act as same as seeking,
		// otherwise, the cover of audio file may be cleared in render loop
		m->clear_cb(m->opaque, true);
	}

	m->first_time_open = false;

	bool succeed = true;
	pthread_mutex_lock(&m->mutex);
	m->exit_ffmpeg = true;
	pthread_mutex_unlock(&m->mutex);

	mp_decode_free(&m->v);
	mp_decode_free(&m->a);
	avformat_close_input(&m->fmt);
	sws_freeContext(m->swscale);
	m->swscale = NULL;
	av_freep(&m->scale_pic[0]);

	pthread_mutex_lock(&m->mutex);
	m->exit_ffmpeg = false;
	pthread_mutex_unlock(&m->mutex);

	if (!init_avformat(m)) {
		succeed = false;
		goto fail;
	}

	if (!mp_media_reset(m, true)) {
		succeed = false;
		goto fail;
	}
fail:
	pthread_mutex_lock(&m->mutex);
	m->reopen = false;
	pthread_mutex_unlock(&m->mutex);
	return succeed;
}

//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
void mp_media_update_internal(mp_media_t *m, const struct mp_media_info *info);
static inline bool mp_media_thread(mp_media_t *m)
{
	os_set_thread_name("mp_media_thread");

	//PRISM/LiuHaibin/20201027/#5448/set init value to false
	bool reopen_succeed = false;
	m->stop_callback_called = false;
	/* flag shows if it's force exit by user */
	bool force_exit_ffmpg = false;

	for (;;) {
		bool reset, kill, is_active, seek, pause, reset_time, reopen,
			fmt_valid;
		bool timeout = false;

		pthread_mutex_lock(&m->mutex);
		is_active = m->active;
		pthread_mutex_unlock(&m->mutex);

		if (!is_active) {
			if (os_sem_wait(m->sem) < 0)
				return false;
		} else {
			timeout = mp_media_sleepto(m);
		}

		pthread_mutex_lock(&m->mutex);

		reset = m->reset;
		kill = m->kill;
		//PRISM/LiuHaibin/20200820/#4079/for media controller
		/* the m->reset is not set to false here but where mp_media_reset is finished */
		//m->reset = false;
		m->kill = false;

		pause = m->pause;
		fmt_valid = (m->fmt != NULL);

		if (m->pause_state_changed) {
			//PRISM/ZengQin/20200927/#5043/for media controller
			if (m->state_cb && fmt_valid)
				m->state_cb(m->opaque,
					    pause ? OBS_MEDIA_STATE_PAUSED
						  : OBS_MEDIA_STATE_PLAYING,
					    false);
			m->pause_state_changed = false;
		}

		//PRISM/LiuHaibin/20200819/#none/for seek
		if (!m->just_seek && m->seek_positions.num > 0) {
			seek = true;
			m->just_seek = true;
			m->seek_pos = m->seek_positions.array[0];
			da_free(m->seek_positions);
		} else
			seek = false;

		//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
		if (m->update_info_array.num > 0) {
			struct mp_media_info info;
			/* Here we only update the latest one, and notify the skipped event (for BGM source) */
			for (int i = 0; i < m->update_info_array.num - 1; i++) {
				info = m->update_info_array.array[i];
				if (m->skipped_cb)
					m->skipped_cb(m->opaque, info.path);

				blog(LOG_DEBUG, "[TEST] SKIP ----- URL %s",
				     info.path);

				bfree(info.path);
				bfree(info.format);
			}

			info = m->update_info_array
				       .array[m->update_info_array.num - 1];

			blog(LOG_DEBUG, "[TEST] UPDATE/OPEN ----- URL %s",
			     info.path);

			mp_media_update_internal(m, &info);
			bfree(info.path);
			bfree(info.format);
			da_free(m->update_info_array);
			if (m->fmt == NULL && m->path)
				m->reopen = true;
		}

		reset_time = m->reset_ts;
		m->reset_ts = false;

		reopen = m->reopen;

		pthread_mutex_unlock(&m->mutex);

		if (kill) {
			break;
		}

		//PRISM/LiuHaibin/20200927/#4309 & #5132/media update
		if (reopen) {
			if (!mp_media_reopen(m)) {

				//PRISM/ZengQin/20200927/#none/for media controller and bgm
				mp_media_open_loading_stopped(m);
				mp_media_read_loading_stopped(m);

				reopen_succeed = false;

				/* Do not go to error when force exit ffmpeg
				 * This should only happens when path have changed */
				pthread_mutex_lock(&m->mutex);
				force_exit_ffmpg = m->exit_ffmpeg;
				reset = m->reset;
				pthread_mutex_unlock(&m->mutex);
				if (!force_exit_ffmpg && m->error_cb)
					m->error_cb(m->opaque, true);
				//PRISM/ZengQin/20201019/#5275/for listen music
				if (reset)
					mp_media_stop_cb(m);

			} else
				reopen_succeed = true;
			reset_ts(m);
			continue;
		}

		if (!reopen_succeed) {
			os_sleep_ms(5);
			continue;
		}

		if (reset) {
			//PRISM/ZengQin/20200811/#4018/for media controller
			//mp_media_reset(m);
			mp_media_reset(m, false);
			//PRISM/ZengQin/20200903/#4722/checked 'Restart playback when..',when source become deactive,should reset ts.
			reset_ts(m);
			continue;
		}

		if (seek) {
			seek_to(m, m->seek_pos);
			//PRISM/ZengQin/20200706/#3179/for media controller
			reset_ts(m);
			if (m->eof)
				m->eof = false;
			continue;
		}

		//PRISM/ZengQin/202000708/#3179/for media controller
		if (reset_time || timeout) {
			reset_ts(m);
			continue;
		}

		//PRISM/ZengQin/20200618/#3179/for media controller
		if (pause && !m->just_seek) {
			//PRISM/LiuHaibin/20200826/#None/for media controller
			os_sleep_ms(5);
			continue;
		}

		/* frames are ready */
		if (is_active && !timeout) {
			if (m->has_video)
				mp_media_next_video(m, false);
			if (m->has_audio)
				mp_media_next_audio(m);
			if (!mp_media_prepare_frames(m)) {
				//PRISM/LiuHaibin/20200827/#/force exit ffmpeg
				/* Same as reopen, do not go to error when force exit ffmpeg
				 * This should only happens when path have changed */
				pthread_mutex_lock(&m->mutex);
				force_exit_ffmpg = m->exit_ffmpeg;
				pthread_mutex_unlock(&m->mutex);
				if (!force_exit_ffmpg && m->error_cb)
					m->error_cb(m->opaque, false);
				else
					continue;
			}

			if (mp_media_eof(m))
				continue;

			mp_media_calc_next_ns(m);
		}
	}

	return true;
}

static void *mp_media_thread_start(void *opaque)
{
	mp_media_t *m = opaque;

	if (!mp_media_thread(m)) {
		//PRISM/ZengQin/20200812/#4168/for media controller and bgm
		mp_media_open_loading_stopped(m);
		mp_media_read_loading_stopped(m);

		if (m->stop_cb) {
			m->stop_cb(m->opaque, false);
		}
	}

	return NULL;
}

static inline bool mp_media_init_internal(mp_media_t *m,
					  const struct mp_media_info *info)
{
	if (pthread_mutex_init(&m->mutex, NULL) != 0) {
		blog(LOG_WARNING, "MP: Failed to init mutex");
		return false;
	}
	if (os_sem_init(&m->sem, 0) != 0) {
		blog(LOG_WARNING, "MP: Failed to init semaphore");
		return false;
	}

	m->path = info->path ? bstrdup(info->path) : NULL;
	m->format_name = info->format ? bstrdup(info->format) : NULL;
	m->hw = info->hardware_decoding;

	if (pthread_create(&m->thread, NULL, mp_media_thread_start, m) != 0) {
		blog(LOG_WARNING, "MP: Could not create media thread");
		return false;
	}

	m->thread_valid = true;
	return true;
}

bool mp_media_init(mp_media_t *media, const struct mp_media_info *info)
{
	memset(media, 0, sizeof(*media));
	pthread_mutex_init_value(&media->mutex);
	media->opaque = info->opaque;
	media->v_cb = info->v_cb;
	media->a_cb = info->a_cb;
	media->stop_cb = info->stop_cb;
	//PRISM/ZengQin/20200706/#3179/for media controller
	media->eof_cb = info->eof_cb;
	//PRISM/WangShaohui/20200117/#281/for source unavailable
	media->error_cb = info->error_cb;
	//PRISM/ZengQin/20200706/#3179/for media controller
	media->load_cb = info->load_cb;
	//PRISM/ZengQin/20200729/#3179/for media controller
	media->started_cb = info->started_cb;
	media->clear_cb = info->clear_cb;
	media->v_preload_cb = info->v_preload_cb;
	media->force_range = info->force_range;
	media->buffering = info->buffering;
	media->speed = info->speed;
	media->is_local_file = info->is_local_file;
	//PRISM/ZengQin/20200721/#3179/for bgm player
	media->stopping = false;
	//PRISM/ZengQin/20200812/#3787/for media controller
	media->start_pos = info->start_pos;
	media->starting = true;
	media->state_cb = info->state_cb;
	media->file_changed = true;
	//PRISM/ZengQin/20200909/#4832/for media controller
	media->kill = false;
	media->exit_ffmpeg = false;
	media->reopen = true;
	//PRISM/LiuHaibin/20200924/#2174/cover for audio
	media->first_time_open = true;
	//PRISM/LiuHaibin/20201029/#None/media skipped message for BGM
	media->skipped_cb = info->skipped_cb;

	if (!info->is_local_file || media->speed < 1 || media->speed > 200)
		media->speed = 100;

	static bool initialized = false;
	if (!initialized) {
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
		av_register_all();
		avcodec_register_all();
#endif
		avdevice_register_all();
		avformat_network_init();
		initialized = true;
	}

	if (!base_sys_ts)
		base_sys_ts = (int64_t)os_gettime_ns();

	if (!mp_media_init_internal(media, info)) {
		mp_media_free(media);

		return false;
	}

	return true;
}

static void mp_kill_thread(mp_media_t *m)
{
	//PRISM/ZengQin/20200909/#4832/for media controller
	if (m->thread_valid) {
		pthread_mutex_lock(&m->mutex);
		m->kill = true;
		pthread_mutex_unlock(&m->mutex);
		os_sem_post(m->sem);

		pthread_join(m->thread, NULL);
	}
}

void mp_media_free(mp_media_t *media)
{
	if (!media)
		return;

	mp_media_stop(media);
	mp_kill_thread(media);
	mp_decode_free(&media->v);
	mp_decode_free(&media->a);
	avformat_close_input(&media->fmt);
	pthread_mutex_destroy(&media->mutex);
	os_sem_destroy(media->sem);
	sws_freeContext(media->swscale);
	av_freep(&media->scale_pic[0]);
	bfree(media->path);
	bfree(media->format_name);
	//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
	da_free(media->seek_positions);
	for (int i = 0; i < media->update_info_array.num; i++) {
		struct mp_media_info info = media->update_info_array.array[i];
		if (media->skipped_cb) {
			media->skipped_cb(media->opaque, info.path);
			blog(LOG_DEBUG, "[TEST] SKIP ----- URL %s", info.path);
		}
		bfree(info.path);
		bfree(info.format);
	}
	da_free(media->update_info_array);
	memset(media, 0, sizeof(*media));
	pthread_mutex_init_value(&media->mutex);
}

void mp_media_play(mp_media_t *m, bool loop, bool restart)
{
	pthread_mutex_lock(&m->mutex);

	//PRISM/ZengQin/20200923/#4815 & #4766/for media controller
	if (restart) {
		m->reset = true;
		//PRISM/ZengQin/20201013/#5043/for media controller
		if (m->fmt == NULL) {
			m->reopen = true;
		}
	} else if (m->active)
		m->reset = true;

	m->looping = loop;
	m->active = true;

	//PRISM/LiuHaibin/20200820/#4079/for media controller
	/* if stopping logic is not finished, reset it to starting state */
	if (m->stopping && m->reset) {
		m->starting = true;
		m->stopping = false;
	}

	pthread_mutex_unlock(&m->mutex);

	os_sem_post(m->sem);
}

void mp_media_play_pause(mp_media_t *m, bool pause)
{
	pthread_mutex_lock(&m->mutex);
	if (m->active) {
		//PRISM/LiuHaibin/20200831/#None/media state
		m->pause_state_changed = true;
		m->pause = pause;
		m->reset_ts = !pause;
	}

	pthread_mutex_unlock(&m->mutex);

	os_sem_post(m->sem);
}

void mp_media_stop(mp_media_t *m)
{
	pthread_mutex_lock(&m->mutex);
	if (m->active) {
		m->reset = true;
		m->active = false;
		m->stopping = true;
		//PRISM/LiuHaibin/20200820/#4079/for media controller
		m->starting = false;
		m->exit_ffmpeg = true;
	}
	pthread_mutex_unlock(&m->mutex);

	os_sem_post(m->sem);
}

int64_t mp_get_current_time(mp_media_t *m)
{
	//PRISM/ZengQin/20200806/#3795/for media controller
	//int speed = (int)((float)m->speed / 100.0f);
	//return (mp_media_get_current_pts(m) / 1000000) * speed;

	float speed = ((float)m->speed / 100.0f);
	return (int64_t)(((float)mp_media_get_current_pts(m) / 1000000.0f) *
			 speed);
}

void mp_media_seek_to(mp_media_t *m, int64_t pos)
{
	pthread_mutex_lock(&m->mutex);
	//PRISM/LiuHaibin/20200819/#none/for seek
	if (m->active) {
		int64_t seek_pos = pos * 1000;
		da_free(m->seek_positions);
		da_push_back(m->seek_positions, &seek_pos);
	}
	pthread_mutex_unlock(&m->mutex);

	os_sem_post(m->sem);
}

//PRISM/ZengQin/20200616/#3179/for media controller
bool mp_media_is_update_done(mp_media_t *m)
{
	pthread_mutex_lock(&m->mutex);

	bool is_update_done = !m->seek_positions.num && !m->just_seek &&
			      m->first_read && !m->speed_changing && !m->reopen;
	pthread_mutex_unlock(&m->mutex);

	return is_update_done;
}

//PRISM/LiuHaibin/20200825/#4491&4482/for media controller and free bgm
void mp_media_update(mp_media_t *m, const struct mp_media_info *info)
{
	pthread_mutex_lock(&m->mutex);
	blog(LOG_DEBUG, "[TEST] PUSH ----- URL %s", info->path);
	da_push_back(m->update_info_array, info);
	if (info->file_changed) {
		m->exit_ffmpeg = true;
		blog(LOG_INFO, "MP: path changed, force exit ffmpeg");
	}

	pthread_mutex_unlock(&m->mutex);
}

//PRISM/ZengQin/20200811/#4018/for media controller
void mp_media_update_internal(mp_media_t *m, const struct mp_media_info *info)
{
	/* NOTE!!
	 * This function is called inside decode thread, it has been locked outside the function.
	 * If you want to use it in other place please lock it up. */
	int real_speed;
	if (!info->is_local_file || info->speed < 1 || info->speed > 200)
		real_speed = 100;
	else
		real_speed = info->speed;

	if (real_speed != m->speed) {
		m->speed_changing = true;
		m->v.speed_changed = false;
		m->a.speed_changed = false;
	}

	m->looping = info->is_looping;
	m->speed = real_speed;
	m->force_range = info->force_range;
	m->reopen = info->reopen;

	if (strcmp(m->path, info->path) != 0) {
		m->start_pos = 0;
		//PRISM/ZengQin/20200908/#4793/reset value when path has changed.
		m->current_a_pts = m->current_v_pts = 0;
		if (m->seek_positions.num > 0)
			da_free(m->seek_positions);
	} else {
		if (m->seek_positions.num > 0) {
			m->start_pos = m->seek_positions.array[0];
			da_free(m->seek_positions);
		} else
			m->start_pos = m->just_seek ? m->seek_pos
						    : info->start_pos;
	}

	m->path = info->path ? bstrdup(info->path) : NULL;
	m->format_name = info->format ? bstrdup(info->format) : NULL;
	m->buffering = info->buffering;
	m->hw = info->hardware_decoding;
	m->is_local_file = info->is_local_file;
	m->file_changed = info->file_changed;
}

//PRISM/ZengQin/20200908/#4831/for media controller
void mp_media_set_pause_state(mp_media_t *m, bool pause,
			      bool notifyStateChanged)
{
	pthread_mutex_lock(&m->mutex);
	m->pause_state_changed = notifyStateChanged;
	m->pause = pause;
	pthread_mutex_unlock(&m->mutex);
}

//PRISM/ZengQin/20200827/#none/for loading update
bool mp_media_is_open_loading(mp_media_t *m)
{
	pthread_mutex_lock(&m->mutex);
	bool loading = m->open_timeout;
	pthread_mutex_unlock(&m->mutex);
	return loading;
}

//PRISM/ZengQin/20200923/#4787/for loading update
bool mp_media_is_loading(mp_media_t *m)
{
	pthread_mutex_lock(&m->mutex);
	bool opening = m->open_timeout;
	bool reading = m->read_timeout;
	pthread_mutex_unlock(&m->mutex);
	return opening || reading;
}

//PRISM/ZengQin/20200902/#none/update invalid duration for local file
bool mp_media_is_invalid_durtion_local_file(mp_media_t *m)
{
	if (!m)
		return false;
	if (m->is_local_file && m->fmt &&
	    (m->fmt->duration == AV_NOPTS_VALUE || m->fmt->duration <= 0))
		return true;
	else
		return false;
}

//PRISM/ZengQin/20200902/#none/get duration
int64_t mp_media_get_duration(mp_media_t *m)
{
	if (!m)
		return 0;

	pthread_mutex_lock(&m->mutex);
	int64_t duration = m->duration / INT64_C(1000);
	pthread_mutex_unlock(&m->mutex);
	return duration;
}
