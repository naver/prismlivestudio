/******************************************************************************
    Copyright (C) 2014 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <obs.h>
#include <stdio.h>
#include <util/dstr.h>
#include <util/array-serializer.h>
#include "flv-mux.h"
#include "obs-output-ver.h"
#include "rtmp-helpers.h"
//PRISM/LiuHaibin/20200908/#4748/add mp3 info
#include "pls/media-info.h"
//PRISM/LiuHaibin/20201015/#None/add publisher info
#ifdef _WIN32
#include <util/windows/win-version.h>
#endif

/* TODO: FIXME: this is currently hard-coded to h264 and aac!  ..not that we'll
 * use anything else for a long time. */

//#define DEBUG_TIMESTAMPS
//#define WRITE_FLV_HEADER

#define VIDEO_HEADER_SIZE 5

static inline double encoder_bitrate(obs_encoder_t *encoder)
{
	obs_data_t *settings = obs_encoder_get_settings(encoder);
	double bitrate = obs_data_get_double(settings, "bitrate");

	obs_data_release(settings);
	return bitrate;
}

#define FLV_INFO_SIZE_OFFSET 42

void write_file_info(FILE *file, int64_t duration_ms, int64_t size)
{
	char buf[64];
	char *enc = buf;
	char *end = enc + sizeof(buf);

	fseek(file, FLV_INFO_SIZE_OFFSET, SEEK_SET);

	enc_num_val(&enc, end, "duration", (double)duration_ms / 1000.0);
	enc_num_val(&enc, end, "fileSize", (double)size);

	fwrite(buf, 1, enc - buf, file);
}

static bool build_flv_meta_data(obs_output_t *context, uint8_t **output,
				size_t *size, size_t a_idx)
{
	obs_encoder_t *vencoder = obs_output_get_video_encoder(context);
	obs_encoder_t *aencoder = obs_output_get_audio_encoder(context, a_idx);
	video_t *video = obs_encoder_video(vencoder);
	audio_t *audio = obs_encoder_audio(aencoder);
	char buf[4096];
	char *enc = buf;
	char *end = enc + sizeof(buf);
	struct dstr encoder_name = {0};
	struct dstr publisher_info = {0};

	if (a_idx > 0 && !aencoder)
		return false;

	enc_str(&enc, end, "onMetaData");

	*enc++ = AMF_ECMA_ARRAY;

	//PRISM/LiuHaibin/20200701/#2440/support NOW
	enc = AMF_EncodeInt32(enc, end, a_idx == 0 ? 22 : 17);

	enc_num_val(&enc, end, "duration", 0.0);
	enc_num_val(&enc, end, "fileSize", 0.0);

	if (a_idx == 0) {
		enc_num_val(&enc, end, "width",
			    (double)obs_encoder_get_width(vencoder));
		enc_num_val(&enc, end, "height",
			    (double)obs_encoder_get_height(vencoder));

		enc_str_val(&enc, end, "videocodecid", "avc1");
		enc_num_val(&enc, end, "videodatarate",
			    encoder_bitrate(vencoder));
		enc_num_val(&enc, end, "framerate",
			    video_output_get_frame_rate(video));
	}

	enc_str_val(&enc, end, "audiocodecid", "mp4a");
	enc_num_val(&enc, end, "audiodatarate", encoder_bitrate(aencoder));
	enc_num_val(&enc, end, "audiosamplerate",
		    (double)obs_encoder_get_sample_rate(aencoder));
	enc_num_val(&enc, end, "audiosamplesize", 16.0);
	enc_num_val(&enc, end, "audiochannels",
		    (double)audio_output_get_channels(audio));

	enc_bool_val(&enc, end, "stereo",
		     audio_output_get_channels(audio) == 2);
	enc_bool_val(&enc, end, "2.1", audio_output_get_channels(audio) == 3);
	enc_bool_val(&enc, end, "3.1", audio_output_get_channels(audio) == 4);
	enc_bool_val(&enc, end, "4.0", audio_output_get_channels(audio) == 4);
	enc_bool_val(&enc, end, "4.1", audio_output_get_channels(audio) == 5);
	enc_bool_val(&enc, end, "5.1", audio_output_get_channels(audio) == 6);
	enc_bool_val(&enc, end, "7.1", audio_output_get_channels(audio) == 8);

	//PRISM/LiuHaibin/20201015/#None/add publisher info
	dstr_printf(&encoder_name, "PRISM-%s (libobs version %d.%d.%d)",
		    MODULE_NAME, LIBOBS_API_MAJOR_VER, LIBOBS_API_MINOR_VER,
		    LIBOBS_API_PATCH_VER);

	//	dstr_printf(&encoder_name, "prism-%s (libobs version ", MODULE_NAME);
	//#ifdef HAVE_OBSCONFIG_H
	//	dstr_cat(&encoder_name, OBS_VERSION);
	//#else
	//	dstr_catf(&encoder_name, "%d.%d.%d", LIBOBS_API_MAJOR_VER,
	//		  LIBOBS_API_MINOR_VER, LIBOBS_API_PATCH_VER);
	//#endif
	//	dstr_cat(&encoder_name, ")");

	enc_str_val(&enc, end, "encoder", encoder_name.array);
	dstr_free(&encoder_name);

	// NOTE!!!
	// Here we only support Windows, if we start to support other platform, modify this.
	struct win_version_info win_ver = {0};
	get_win_ver(&win_ver);
	dstr_printf(
		&publisher_info,
		"osType=Windows&appName=PRISM&appVersion=%s&osVersion=%d.%d.%d&sdkVersion=%d.%d.%d",
		OBS_VERSION, win_ver.major, win_ver.minor, win_ver.build,
		LIBOBS_API_MAJOR_VER, LIBOBS_API_MINOR_VER,
		LIBOBS_API_PATCH_VER);

	enc_str_val(&enc, end, "publisherinfo", publisher_info.array);
	dstr_free(&publisher_info);

	//PRISM/LiuHaibin/20200701/#2440/support NOW
	enc_num_val(&enc, end, "timestamp",
		    obs_output_get_base_timestamp(context));

	*enc++ = 0;
	*enc++ = 0;
	*enc++ = AMF_OBJECT_END;

	*size = enc - buf;
	*output = bmemdup(buf, *size);
	return true;
}

//PRISM/LiuHaibin/20200918/#4748/add id3v2
#define USING_BASE64_ENCODE 1
#define BUFFER_PADDING_SIZE 64
#if USING_BASE64_ENCODE
// refer to FFmpeg intreadwrite.h
#ifndef AV_RB32
#define AV_RB32(x)                                     \
	(((uint32_t)((const uint8_t *)(x))[0] << 24) | \
	 (((const uint8_t *)(x))[1] << 16) |           \
	 (((const uint8_t *)(x))[2] << 8) | ((const uint8_t *)(x))[3])
#endif

// refer to FFmpeg base64.c and base64.h
#define AV_BASE64_SIZE(x) (((x) + 2) / 3 * 4 + 1)
static char *av_base64_encode(char *out, int out_size, const uint8_t *in,
			      int in_size)
{
	static const char b64[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char *ret, *dst;
	unsigned i_bits = 0;
	int i_shift = 0;
	int bytes_remaining = in_size;

	if (in_size >= UINT_MAX / 4 || out_size < AV_BASE64_SIZE(in_size))
		return NULL;
	ret = dst = out;
	while (bytes_remaining > 3) {
		i_bits = AV_RB32(in);
		in += 3;
		bytes_remaining -= 3;
		*dst++ = b64[i_bits >> 26];
		*dst++ = b64[(i_bits >> 20) & 0x3F];
		*dst++ = b64[(i_bits >> 14) & 0x3F];
		*dst++ = b64[(i_bits >> 8) & 0x3F];
	}
	i_bits = 0;
	while (bytes_remaining) {
		i_bits = (i_bits << 8) + *in++;
		bytes_remaining--;
		i_shift += 8;
	}
	while (i_shift > 0) {
		*dst++ = b64[(i_bits << 6 >> i_shift) & 0x3f];
		i_shift -= 6;
	}
	while ((dst - ret) & 3)
		*dst++ = '=';
	*dst = '\0';

	return ret;
}
#endif

//PRISM/LiuHaibin/20200915/#4748/add id3v2
static bool build_id3v2(uint8_t **output, size_t *size)
{
	mi_id3v2_t id3 = {0};
	if (mi_id3v2_queued() && mi_get_id3v2(&id3)) {
		char *buf = NULL;
		size_t buf_size = 0;
#if USING_BASE64_ENCODE
		char *base64_buf = NULL;
		size_t base64_buf_size = 0;

		base64_buf_size = AV_BASE64_SIZE(id3.size);
		base64_buf = bmalloc(base64_buf_size);
		if (!base64_buf) {
			mi_free_id3v2(&id3);
			return false;
		}

		buf_size = base64_buf_size + strlen("@insertId3Tag") +
			   BUFFER_PADDING_SIZE;
		buf = bmalloc(buf_size);
		if (!buf) {
			mi_free_id3v2(&id3);
			bfree(base64_buf);
			return false;
		}

		memset(base64_buf, 0, base64_buf_size);
		memset(buf, 0, buf_size);
		char *enc = buf;
		char *end = enc + buf_size - 1;

		if (!av_base64_encode(base64_buf, base64_buf_size, id3.data,
				      id3.size)) {
			blog(LOG_INFO,
			     "Failed: cannot Base64-encode the id3 data, size (src %d : dst %d)",
			     id3.size, base64_buf_size);
			mi_free_id3v2(&id3);
			bfree(base64_buf);
			bfree(buf);
			return false;
		}

		enc_str(&enc, end, "@insertId3Tag");
		enc_str(&enc, end, base64_buf);
		bfree(base64_buf);
#else
		size_t buf_size = id3.size + strlen("@insertId3Tag") +
				  BUFFER_PADDING_SIZE;
		char *buf = bmalloc(buf_size);
		if (!buf) {
			mi_free_id3v2(&id3);
			return false;
		}

		memset(buf, 0, buf_size);
		char *enc = buf;
		char *end = enc + buf_size - 1;

		enc_str(&enc, end, "@insertId3Tag");
		enc_str_ex(&enc, end, id3.data, id3.size);
#endif
		*size = enc - buf;
		*output = bmemdup(buf, *size);

		mi_free_id3v2(&id3);
		bfree(buf);

		return true;
	}
	return false;
}

//PRISM/LiuHaibin/20200915/#4748/add id3v2
bool flv_id3v2(uint8_t **output, size_t *size, int64_t dts, int32_t dts_offset,
	       int32_t timebase_den)
{
	struct array_output_data data;
	struct serializer s;
	uint8_t *id3v2_data = NULL;
	size_t id3v2_size;
	uint32_t start_pos;

	int32_t time_ms = get_ms_time_ex(timebase_den, dts) - dts_offset;

	array_output_serializer_init(&s, &data);

	if (!build_id3v2(&id3v2_data, &id3v2_size)) {
		bfree(id3v2_data);
		return false;
	}

	start_pos = serializer_get_pos(&s);

	s_w8(&s, RTMP_PACKET_TYPE_INFO);

	s_wb24(&s, (uint32_t)id3v2_size);
	s_wb24(&s, time_ms);
	s_w8(&s, (time_ms >> 24) & 0x7F);
	s_wb24(&s, 0);

	s_write(&s, id3v2_data, id3v2_size);

	s_wb32(&s, (uint32_t)serializer_get_pos(&s) - start_pos - 1);

	*output = data.bytes.array;
	*size = data.bytes.num;

	bfree(id3v2_data);
	return true;
}

bool flv_meta_data(obs_output_t *context, uint8_t **output, size_t *size,
		   bool write_header, size_t audio_idx)
{
	struct array_output_data data;
	struct serializer s;
	uint8_t *meta_data = NULL;
	size_t meta_data_size;
	uint32_t start_pos;

	array_output_serializer_init(&s, &data);

	if (!build_flv_meta_data(context, &meta_data, &meta_data_size,
				 audio_idx)) {
		bfree(meta_data);
		return false;
	}

	if (write_header) {
		s_write(&s, "FLV", 3);
		s_w8(&s, 1);
		s_w8(&s, 5);
		s_wb32(&s, 9);
		s_wb32(&s, 0);
	}

	start_pos = serializer_get_pos(&s);

	s_w8(&s, RTMP_PACKET_TYPE_INFO);

	s_wb24(&s, (uint32_t)meta_data_size);
	s_wb32(&s, 0);
	s_wb24(&s, 0);

	s_write(&s, meta_data, meta_data_size);

	s_wb32(&s, (uint32_t)serializer_get_pos(&s) - start_pos - 1);

	*output = data.bytes.array;
	*size = data.bytes.num;

	bfree(meta_data);
	return true;
}

#ifdef DEBUG_TIMESTAMPS
static int32_t last_time = 0;
#endif

static void flv_video(struct serializer *s, int32_t dts_offset,
		      struct encoder_packet *packet, bool is_header)
{
	int64_t offset = packet->pts - packet->dts;
	int32_t time_ms = get_ms_time(packet, packet->dts) - dts_offset;

	if (!packet->data || !packet->size)
		return;

	s_w8(s, RTMP_PACKET_TYPE_VIDEO);

#ifdef DEBUG_TIMESTAMPS
	blog(LOG_DEBUG, "Video: %lu", time_ms);

	if (last_time > time_ms)
		blog(LOG_DEBUG, "Non-monotonic");

	last_time = time_ms;
#endif

	s_wb24(s, (uint32_t)packet->size + 5);
	s_wb24(s, time_ms);
	s_w8(s, (time_ms >> 24) & 0x7F);
	s_wb24(s, 0);

	/* these are the 5 extra bytes mentioned above */
	s_w8(s, packet->keyframe ? 0x17 : 0x27);
	s_w8(s, is_header ? 0 : 1);
	s_wb24(s, get_ms_time(packet, offset));
	s_write(s, packet->data, packet->size);

	/* write tag size (starting byte doesn't count) */
	s_wb32(s, (uint32_t)serializer_get_pos(s) - 1);
}

static void flv_audio(struct serializer *s, int32_t dts_offset,
		      struct encoder_packet *packet, bool is_header)
{
	int32_t time_ms = get_ms_time(packet, packet->dts) - dts_offset;

	if (!packet->data || !packet->size)
		return;

	s_w8(s, RTMP_PACKET_TYPE_AUDIO);

#ifdef DEBUG_TIMESTAMPS
	blog(LOG_DEBUG, "Audio: %lu", time_ms);

	if (last_time > time_ms)
		blog(LOG_DEBUG, "Non-monotonic");

	last_time = time_ms;
#endif

	s_wb24(s, (uint32_t)packet->size + 2);
	s_wb24(s, time_ms);
	s_w8(s, (time_ms >> 24) & 0x7F);
	s_wb24(s, 0);

	/* these are the two extra bytes mentioned above */
	s_w8(s, 0xaf);
	s_w8(s, is_header ? 0 : 1);
	s_write(s, packet->data, packet->size);

	/* write tag size (starting byte doesn't count) */
	s_wb32(s, (uint32_t)serializer_get_pos(s) - 1);
}

void flv_packet_mux(struct encoder_packet *packet, int32_t dts_offset,
		    uint8_t **output, size_t *size, bool is_header)
{
	struct array_output_data data;
	struct serializer s;

	array_output_serializer_init(&s, &data);

	if (packet->type == OBS_ENCODER_VIDEO)
		flv_video(&s, dts_offset, packet, is_header);
	else
		flv_audio(&s, dts_offset, packet, is_header);

	*output = data.bytes.array;
	*size = data.bytes.num;
}
