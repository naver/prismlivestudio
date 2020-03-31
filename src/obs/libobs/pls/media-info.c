#include "obs.h"
#include "obs-internal.h"
#include "pls/media-info.h"

#include <libswscale/swscale.h>
#include <util/threading.h>
#include <libavutil/imgutils.h>

#define FFMPEG_OPEN_TIMEOUT_NS 1000000000 // opening timeout
#define ID3V2_HEADER_SIZE 10
#define ID3v2_DEFAULT_MAGIC "ID3"
#define USING_FFMPEG 1

#define SAFE_FREE_STRING(STR_PTR)        \
	{                                \
		if (*STR_PTR) {          \
			bfree(*STR_PTR); \
			*STR_PTR = NULL; \
		}                        \
	}

/* clang-format off */
static const mi_option mi_options[] = {
	/* options retrieved in format of string */
	{"title",		"title field for mp3 id3",				MI_TYPE_STRING},
	{"artist",		"artist field for mp3 id3",				MI_TYPE_STRING},
	{"album",		"album field for mp3 id3",				MI_TYPE_STRING},
	{"date",		"date field for mp3 id3",				MI_TYPE_STRING},
	{"comment",		"comment field for mp3 id3",				MI_TYPE_STRING},
	{"genre",		"genre field for mp3 id3",				MI_TYPE_STRING},
	{"copyright",		"copyright field for mp3 id3",				MI_TYPE_STRING},
	{"language",		"language field for mp3 id3",				MI_TYPE_STRING},
	{"album_artist",	"album_artist field for mp3 id3",			MI_TYPE_STRING},
	{"disc",		"disc field for mp3 id3",				MI_TYPE_STRING},
	{"publisher",		"publisher field for mp3 id3",				MI_TYPE_STRING},
	{"track",		"track field for mp3 id3",				MI_TYPE_STRING},
	{"encoder",		"encoder field for mp3 id3",				MI_TYPE_STRING},

	/* options retrieved in format of int64_t */
	{"duration",		"duration of meida file, in milliseconds",		MI_TYPE_INT},

	/* options retrieved in format of boolean */
	{"has_cover",		"flag shows if an audio file has a cover attached",	MI_TYPE_BOOL},

	/* options retrieved in format of packed information object */
	{"cover_obj",		"audio cover object, defined in mi_cover_t",		MI_TYPE_OBJ},
	{"metadata_obj",	"audio metadata object, defined in mi_metadata_t",	MI_TYPE_OBJ},
	{"id3v2_obj",		"mp3 id3 v2.3 object, defined in mi_id3v23_t",		MI_TYPE_OBJ},
};
/* clang-format on */

static int interrupt_callback(void *data)
{
	media_info_t *mi = data;

	uint64_t ts = os_gettime_ns();
	pthread_mutex_lock(&mi->mutex);
	if (mi->open_ts > 0 && (ts - mi->open_ts) > FFMPEG_OPEN_TIMEOUT_NS) {
		pthread_mutex_unlock(&mi->mutex);
		blog(LOG_INFO, "[mi] FFmpeg interrupt for openning '%s'",
		     mi->path);
		return true;
	}

	if (mi->io_open_ts > 0 &&
	    (ts - mi->io_open_ts) > FFMPEG_OPEN_TIMEOUT_NS) {
		pthread_mutex_unlock(&mi->mutex);
		blog(LOG_INFO, "[mi] FFmpeg interrupt for avio openning '%s'",
		     mi->path);
		return true;
	}

	if (mi->abort) {
		pthread_mutex_unlock(&mi->mutex);
		blog(LOG_WARNING,
		     "[mi] FFmpeg interrupt (due to abort) for openning '%s'",
		     mi->path);
		return true;
	}

	pthread_mutex_unlock(&mi->mutex);

	return false;
}

static bool mi_open_internal(media_info_t *mi)
{
	if (!mi || !mi->path)
		return false;

	mi->fmt = avformat_alloc_context();
	if (!mi->fmt) {
		blog(LOG_WARNING,
		     "[mi] Fail to alloc avformat context for '%s'", mi->path);
		return false;
	}

	AVDictionary *opts = NULL;
	av_dict_set(&opts, "http_persistent", "0", 0);
	av_dict_set(&opts, "reconnect", "1", 0);
	av_dict_set(&opts, "stimeout", "3000000", 0);

	mi->fmt->interrupt_callback.callback = interrupt_callback;
	mi->fmt->interrupt_callback.opaque = mi;

	pthread_mutex_lock(&mi->mutex);
	mi->open_ts = os_gettime_ns();
	pthread_mutex_unlock(&mi->mutex);

	blog(LOG_INFO, "[mi] %s opening media: '%s'",
	     mi->open_mode == MI_OPEN_DEFER ? "Defer" : "Directly", mi->path);
	int ret = avformat_open_input(&mi->fmt, mi->path, NULL,
				      opts ? &opts : NULL);
	pthread_mutex_lock(&mi->mutex);
	mi->open_ts = 0;
	pthread_mutex_unlock(&mi->mutex);
	av_dict_free(&opts);

	if (ret < 0) {
		blog(LOG_WARNING, "[mi] Failed to open media '%s': [%d] '%s'",
		     mi->path, ret, av_err2str(ret));
		return false;
	}

	ret = avformat_find_stream_info(mi->fmt, NULL);
	if (ret < 0) {
		blog(LOG_WARNING,
		     "[mi] Failed to find stream info for '%s': [%d] '%s'",
		     mi->path, ret, av_err2str(ret));
		return false;
	}

	mi->opened = true;

	return true;
}

static bool check_defer_open(media_info_t *mi)
{
	if (mi->open_mode == MI_OPEN_DEFER && !mi->opened &&
	    !mi_open_internal(mi)) {
		mi_free(mi);
		return false;
	}
	return true;
}

static get_duration_ms(media_info_t *mi)
{
	if (!mi || !mi->fmt)
		return 0;

	if (mi->fmt->duration != AV_NOPTS_VALUE)
		return mi->fmt->duration / INT64_C(1000);
	else {
		blog(LOG_WARNING, "[mi] Invalid duration for '%s'.", mi->path);
		return AV_NOPTS_VALUE;
	}
}

static bool has_cover(media_info_t *mi)
{
	if (!mi || !mi->fmt)
		return false;

	int ret = av_find_best_stream(mi->fmt, AVMEDIA_TYPE_VIDEO, -1, -1, NULL,
				      0);
	if (ret < 0) {
		blog(LOG_INFO,
		     "[mi] NO video stream (cover) for '%s': [%d] '%s'",
		     mi->path, ret, av_err2str(ret));
		return false;
	}

	AVStream *stream = mi->fmt->streams[ret];
	mi->has_cover = stream->disposition & AV_DISPOSITION_ATTACHED_PIC;
	if (mi->has_cover)
		mi->cover_index = ret;
	return mi->has_cover;
}

static bool mi_try_init_decoder(media_info_t *mi, enum AVMediaType type)
{
	enum AVCodecID id;
	AVStream *stream;
	AVCodec *codec;
	AVCodecContext *c;
	int ret;

	ret = av_find_best_stream(mi->fmt, type, -1, -1, NULL, 0);
	if (ret < 0) {
		blog(LOG_INFO, "MP: No %s stream.",
		     av_get_media_type_string(type));
		return false;
	}

	stream = mi->fmt->streams[ret];
	id = stream->codecpar->codec_id;
	codec = avcodec_find_decoder(id);
	if (!codec) {
		blog(LOG_WARNING, "MP: Failed to find %s codec",
		     av_get_media_type_string(type));
		return false;
	}

	c = avcodec_alloc_context3(codec);
	if (!c) {
		blog(LOG_WARNING, "MP: Failed to allocate context for %s",
		     av_get_media_type_string(type));
		return false;
	}

	ret = avcodec_parameters_to_context(c, stream->codecpar);
	if (ret < 0) 
		goto fail;

	if (c->thread_count == 1 && c->codec_id != AV_CODEC_ID_PNG &&
	    c->codec_id != AV_CODEC_ID_TIFF &&
	    c->codec_id != AV_CODEC_ID_JPEG2000 &&
	    c->codec_id != AV_CODEC_ID_MPEG4 && c->codec_id != AV_CODEC_ID_WEBP)
		c->thread_count = 0;

	ret = avcodec_open2(c, codec, NULL);
	if (ret < 0)
		blog(LOG_WARNING, "MP: Failed to open %s decoder: %s",
		     av_get_media_type_string(type), av_err2str(ret));
fail:
	avcodec_close(c);
	av_free(c);
	return ret < 0 ? false : true;
}

static bool mi_try_decoder(media_info_t *mi)
{
	bool has_video, has_audio;
	has_video = mi_try_init_decoder(mi, AVMEDIA_TYPE_VIDEO);
	has_audio = mi_try_init_decoder(mi, AVMEDIA_TYPE_AUDIO);
	return has_audio || has_video;
}

bool mi_open(media_info_t *mi, const char *path, enum mi_open_mode mode)
{
	if (!mi || !path) {
		blog(LOG_WARNING, "[mi] Null ptr : mi %p, path %p", mi, path);
		return false;
	}

	memset(mi, 0, sizeof(media_info_t));
	pthread_mutex_init_value(&mi->mutex);
	if (pthread_mutex_init(&mi->mutex, NULL) != 0) {
		blog(LOG_WARNING, "[mi] Failed to init mutex, for '%s'", path);
		return false;
	}

	mi->path = path ? bstrdup(path) : NULL;
	mi->open_mode = mode;
	if (mode & MI_OPEN_DIRECTLY && !mi_open_internal(mi))
		goto fail;

	if (mode & MI_OPEN_TRY_DECODER && !mi_try_decoder(mi))
		goto fail;

	return true;
fail:
	mi_free(mi);
	return false;
}

void mi_free(media_info_t *mi)
{
	if (!mi)
		return;

	pthread_mutex_lock(&mi->mutex);
	mi->abort = true;
	pthread_mutex_unlock(&mi->mutex);

	SAFE_FREE_STRING(&mi->path);
	SAFE_FREE_STRING(&mi->metadata.artist);
	SAFE_FREE_STRING(&mi->metadata.title);
	SAFE_FREE_STRING(&mi->metadata.album);

	if (mi->cover.data)
		av_freep(&mi->cover.data);

	if (mi->id3v2.data)
		bfree(mi->id3v2.data);

	avformat_close_input(&mi->fmt);
	if (mi->codec_ctx) {
#if LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(57, 40, 101)
		avcodec_free_context(&mi->codec_ctx);
#else
		avcodec_close(mi->codec_ctx);
#endif
	}
	pthread_mutex_destroy(&mi->mutex);
	memset(mi, 0, sizeof(media_info_t));
	pthread_mutex_init_value(&mi->mutex);
}

char *mi_get_string(media_info_t *mi, char *key)
{
	if (!mi || !key)
		return NULL;

	if (!check_defer_open(mi))
		return NULL;

	// here we only support strings inside metadata,
	// if other options needed, modify this function.
	AVDictionaryEntry *e;
	e = av_dict_get(mi->fmt->metadata, key, NULL, 0);
	if (e) {
		return e->value;
	}
	return NULL;
}

long long mi_get_int(media_info_t *mi, const char *key)
{
	if (!mi || !key)
		return 0;

	if (!check_defer_open(mi))
		return 0;

	if (0 == strcmp(key, "duration"))
		return get_duration_ms(mi);

	// other options could be added here

	return 0;
}

bool mi_get_bool(media_info_t *mi, const char *key)
{
	if (!mi || !key)
		return false;

	if (!check_defer_open(mi))
		return false;

	if (0 == strcmp(key, "has_cover"))
		return has_cover(mi);

	// other options could be added here

	return false;
}

static bool init_cover_decoder(media_info_t *mi)
{
	AVStream *stream = mi->fmt->streams[mi->cover_index];

	enum AVCodecID id = stream->codecpar->codec_id;
	AVCodec *codec = NULL;
	if (id == AV_CODEC_ID_VP8 || id == AV_CODEC_ID_VP9) {
		AVDictionaryEntry *tag = NULL;
		tag = av_dict_get(stream->metadata, "alpha_mode", tag,
				  AV_DICT_IGNORE_SUFFIX);

		if (tag && strcmp(tag->value, "1") == 0) {
			char *codec = (id == AV_CODEC_ID_VP8) ? "libvpx"
							      : "libvpx-vp9";
			codec = avcodec_find_decoder_by_name(codec);
		}
	}

	if (!codec)
		codec = avcodec_find_decoder(id);

	if (!codec) {
		blog(LOG_WARNING, "[mi] Failed to find codec for cover of '%s'",
		     mi->fmt->url);
		return false;
	}

	AVCodecContext *ctx = avcodec_alloc_context3(codec);
	if (!ctx) {
		blog(LOG_WARNING, "[mi] Failed to allocate context for '%s'",
		     mi->fmt->url);
		return false;
	}

	int ret = avcodec_parameters_to_context(ctx, stream->codecpar);
	if (ret < 0) {
		blog(LOG_WARNING,
		     "[mi] Failed to pass parameters to context for '%s': %d, '%s'",
		     mi->fmt->url, ret, av_err2str(ret));
		goto fail;
	}

	AVDictionary *opts = NULL;
	av_dict_set(&opts, "threads", "1", 0);
	ret = avcodec_open2(ctx, codec, &opts);
	av_dict_free(&opts);
	if (ret < 0) {
		blog(LOG_WARNING,
		     "[mi] Failed to open codec context for '%s': %d, '%s'",
		     mi->fmt->url, ret, av_err2str(ret));
		goto fail;
	}

	mi->codec_ctx = ctx;
	return true;
fail:
	avcodec_close(ctx);
#if LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(57, 40, 101)
	av_free(mi->codec_ctx);
	mi->codec_ctx = NULL;
#endif
	return false;
}

static bool convert_to_rgba(media_info_t *mi, AVFrame *frame)
{
	char *buffer[MAX_AV_PLANES] = {0};
	int linesize[MAX_AV_PLANES] = {0};

	int ret = av_image_alloc(buffer, linesize, frame->width, frame->height,
				 AV_PIX_FMT_RGBA, 1);
	if (ret < 0) {
		blog(LOG_WARNING,
		     "[mi] Failed to alloc image buffer for '%s': %d, '%s'",
		     mi->fmt->url, ret, av_err2str(ret));
		return false;
	}

	int size = ret;

	if (frame->format != AV_PIX_FMT_RGBA) {
		struct SwsContext *swsctx = sws_getContext(
			frame->width, frame->height, frame->format,
			frame->width, frame->height, AV_PIX_FMT_RGBA, SWS_POINT,
			NULL, NULL, NULL);
		if (!swsctx) {
			blog(LOG_WARNING,
			     "[mi] Failed to create scale context for '%s': %d, '%s'",
			     mi->fmt->url, ret, av_err2str(ret));
			goto fail;
		}

		ret = sws_scale(swsctx, (const uint8_t *const *)frame->data,
				frame->linesize, 0, frame->height, buffer,
				linesize);
		sws_freeContext(swsctx);
		swsctx = NULL;
		if (ret < 0) {
			blog(LOG_WARNING,
			     "[mi] Failed to create scale context for '%s': %d, '%s'",
			     mi->fmt->url, ret, av_err2str(ret));
			goto fail;
		}
	} else {
		ret = av_image_copy_to_buffer(
			buffer[0], size, (const uint8_t *const *)frame->data,
			(const int *)frame->linesize, frame->format,
			frame->width, frame->height, 1);
		if (ret < 0) {
			blog(LOG_WARNING,
			     "[mi] Can not copy image to buffer for '%s': %d, '%s'",
			     mi->fmt->url, ret, av_err2str(ret));
			goto fail;
		}
	}

	mi->cover.data = buffer[0];
	mi->cover.size = size;
	mi->cover.width = frame->width;
	mi->cover.height = frame->height;
	mi->cover.format = AV_PIX_FMT_RGBA;

	return true;
fail:
	av_freep(&buffer);
	return false;
}

static bool decode_cover(media_info_t *mi)
{
	AVPacket packet;
	av_init_packet(&packet);

	AVFrame *frame = av_frame_alloc();
	if (!frame) {
		blog(LOG_WARNING, "[mi] Failed to alloc cover frame for '%s'",
		     mi->fmt->url);
		return false;
	}
	bool got_frame = 0;
	bool eof = false;
	while (!got_frame) {
		int ret = 0;
		if (!eof) {
			ret = av_read_frame(mi->fmt, &packet);
			if (ret < 0) {
				if (ret == AVERROR_EOF) {
					eof = true;
				} else {
					blog(LOG_WARNING,
					     "[mi] Failed to read image frame from '%s': %d, '%s'",
					     mi->fmt->url, ret,
					     av_err2str(ret));
					goto fail;
				}
			}
		}

		if (packet.stream_index == mi->cover_index) {
			avcodec_send_packet(mi->codec_ctx, &packet);
			ret = avcodec_receive_frame(mi->codec_ctx, frame);
			if (ret < 0) {
				blog(LOG_WARNING,
				     "[mi] Failed to decode frame for '%s': %d, '%s'",
				     mi->fmt->url, ret, av_err2str(ret));
				goto fail;
			} else {
				got_frame = 1;
			}
		}
	}

	bool ret = convert_to_rgba(mi, frame);

fail:
	av_packet_unref(&packet);
	av_frame_free(&frame);
	if (got_frame && ret == true) {
		return true;
	} else {
		return false;
	}
}

static mi_obj get_cover(media_info_t *mi)
{
	if (!has_cover(mi))
		return NULL;

	for (int i = 0; i < mi->fmt->nb_streams; ++i) {
		if (i == mi->cover_index)
			mi->fmt->streams[i]->discard = AVDISCARD_DEFAULT;
		else
			mi->fmt->streams[i]->discard = AVDISCARD_ALL;
	}

	if (!init_cover_decoder(mi))
		return NULL;

	bool ret = decode_cover(mi);

	if (mi->codec_ctx) {
#if LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(57, 40, 101)
		avcodec_free_context(&mi->codec_ctx);
#else
		avcodec_close(mi->codec_ctx);
#endif
		mi->codec_ctx = NULL;
	}

	return ret ? &mi->cover : NULL;
}

static mi_obj get_metadata(media_info_t *mi)
{
	char *title = mi_get_string(mi, "title");
	char *artist = mi_get_string(mi, "artist");
	char *album = mi_get_string(mi, "album");
	mi->metadata.title = title ? bstrdup(title) : NULL;
	mi->metadata.artist = artist ? bstrdup(artist) : NULL;
	mi->metadata.album = album ? bstrdup(album) : NULL;

	return &mi->metadata;
}

static bool id3v2_match(const uint8_t *buf, const char *magic)
{
	return buf[0] == magic[0] && buf[1] == magic[1] && buf[2] == magic[2] &&
	       buf[3] != 0xff && buf[4] != 0xff && (buf[6] & 0x80) == 0 &&
	       (buf[7] & 0x80) == 0 && (buf[8] & 0x80) == 0 &&
	       (buf[9] & 0x80) == 0;
}

static mi_obj get_id3v2_from_ffmpeg(media_info_t *mi)
{
	bool succeed = false;
	char *header = NULL;
	char *buffer = NULL;
	AVIOContext *io = NULL;
	AVIOInterruptCB io_interrupt_cb = {interrupt_callback, mi};

	pthread_mutex_lock(&mi->mutex);
	mi->io_open_ts = os_gettime_ns();
	pthread_mutex_unlock(&mi->mutex);
	int ret = avio_open2(&io, mi->path, AVIO_FLAG_READ, &io_interrupt_cb,
			     NULL);
	pthread_mutex_lock(&mi->mutex);
	mi->io_open_ts = 0;
	pthread_mutex_unlock(&mi->mutex);
	if (ret < 0) {
		blog(LOG_WARNING, "[mi] Fail to open url: '%s': %s.\n",
		     mi->path, av_err2str(ret));
		goto fail;
	}

	header = bmalloc(ID3V2_HEADER_SIZE);
	if (!header) {
		blog(LOG_WARNING,
		     "[mi] Fail to malloc ID3V2 header buffer for '%s'.",
		     mi->path);
		goto fail;
	}

	ret = avio_read(io, header, ID3V2_HEADER_SIZE);
	if (ret < ID3V2_HEADER_SIZE) {
		blog(LOG_WARNING,
		     "[mi] Fail to read ID3V2 header for '%s', ret %d.",
		     mi->path, ret);
		goto fail;
	}

	if (!id3v2_match(header, ID3v2_DEFAULT_MAGIC)) {
		blog(LOG_WARNING, "[mi] Not an ID3V2 header for '%s'.",
		     mi->path);
		goto fail;
	}

	size_t body_size = ((header[6] & 0x7f) << 21) |
			   ((header[7] & 0x7f) << 14) |
			   ((header[8] & 0x7f) << 7) | (header[9] & 0x7f);
	if (body_size <= 0) {
		blog(LOG_WARNING, "[mi] Wrong body size for '%s'.", mi->path);
		goto fail;
	}

	size_t size = body_size + ID3V2_HEADER_SIZE;
	buffer = bmalloc(size);
	if (!buffer) {
		blog(LOG_WARNING,
		     "[mi] Fail to malloc ID3V2 body buffer for '%s'.",
		     mi->path);
		goto fail;
	}
	memset(buffer, 0, size);
	memmove(buffer, header, ID3V2_HEADER_SIZE);

	ret = avio_read(io, buffer + ID3V2_HEADER_SIZE, body_size);
	if (ret < body_size) {
		blog(LOG_WARNING,
		     "[mi] Fail to read ID3V2 body for '%s', ret %d.", mi->path,
		     ret);
		goto fail;
	}
	mi->id3v2.data = buffer;
	mi->id3v2.size = size;
	blog(LOG_INFO,
	     "[mi] Succeed to get ID3 for '%s' : version %d, size %d.",
	     mi->path, header[3], mi->id3v2.size);
	succeed = true;
	mi->id3v2_ready = true;
fail:
	avio_closep(&io);
	if (header)
		bfree(header);

	// buffer is reused for mi->id3v2.data if succeed
	if (succeed)
		return &mi->id3v2;

	if (buffer)
		bfree(buffer);
	return NULL;
}

static mi_obj get_id3v2(media_info_t *mi)
{
	// currently, we only support mp3 file
	if (!mi->path || NULL == astrstri(mi->path, "mp3")) {
		blog(LOG_WARNING, "[mi] Not a mp3 file '%s'.", mi->path);
		return NULL;
	}

	if (mi->id3v2_ready)
		return &mi->id3v2;

#if USING_FFMPEG
	return get_id3v2_from_ffmpeg(mi);
#else
	bool succeed = false;
	char *header = NULL;
	char *body = NULL;
	FILE *file = os_fopen(mi->path, "rb");
	if (!file) {
		blog(LOG_WARNING, "[mi] Failed open file '%s'.", mi->path);
		return NULL;
	}

	size_t file_size = os_fgetsize(file);
	if (file_size < 10) {
		blog(LOG_WARNING, "[mi] Size %lld of file '%s' is not right.",
		     file_size, mi->path);
		goto fail;
	}

	header = bmalloc(ID3V2_HEADER_SIZE);
	if (!header) {
		blog(LOG_WARNING,
		     "[mi] Fail to malloc ID3V2 header buffer for '%s'.",
		     mi->path);
		goto fail;
	}

	fseek(file, 0, SEEK_SET);
	int size_read = fread(header, 1, ID3V2_HEADER_SIZE, file);
	if (size_read != ID3V2_HEADER_SIZE) {
		blog(LOG_WARNING, "[mi] Fail to read ID3V2 header for '%s'.",
		     mi->path);
		goto fail;
	}

	if (!id3v2_match(header, ID3v2_DEFAULT_MAGIC)) {
		blog(LOG_WARNING, "[mi] Not an ID3V2 header for '%s'.",
		     mi->path);
		goto fail;
	}

	size_t body_size = ((header[6] & 0x7f) << 21) |
			   ((header[7] & 0x7f) << 14) |
			   ((header[8] & 0x7f) << 7) | (header[9] & 0x7f);
	if (file_size < body_size + ID3V2_HEADER_SIZE) {
		blog(LOG_WARNING, "[mi] Not an ID3V2 header for '%s'.",
		     mi->path);
		goto fail;
	}

	body = bmalloc(body_size);
	if (!body) {
		blog(LOG_WARNING,
		     "[mi] Fail to malloc ID3V2 body buffer for '%s'.",
		     mi->path);
		goto fail;
	}

	fseek(file, ID3V2_HEADER_SIZE, SEEK_SET);
	size_read = fread(body, 1, body_size, file);
	if (size_read != body_size) {
		blog(LOG_WARNING, "[mi] Fail to read ID3V2 header for '%s'.",
		     mi->path);
		goto fail;
	}

	mi->id3v2.data = bmalloc(body_size + ID3V2_HEADER_SIZE);
	if (!mi->id3v2.data) {
		blog(LOG_WARNING, "[mi] Fail to malloc ID3V2 buffer for '%s'.",
		     mi->path);
		goto fail;
	}

	memset(mi->id3v2.data, 0, body_size + ID3V2_HEADER_SIZE);
	memcpy(mi->id3v2.data, header, ID3V2_HEADER_SIZE);
	memcpy(mi->id3v2.data + ID3V2_HEADER_SIZE, body, body_size);
	mi->id3v2.size = body_size + ID3V2_HEADER_SIZE;
	succeed = true;

fail:
	if (file)
		fclose(file);
	if (header)
		bfree(header);
	if (body)
		bfree(body);
	if (succeed)
		return &mi->id3v2;
	else
		return NULL;
#endif
}

mi_obj mi_get_obj(media_info_t *mi, const char *key)
{
	if (!mi || !key)
		return NULL;

	if (0 != strcmp(key, "id3v2_obj") && !check_defer_open(mi))
		return NULL;

	if (0 == strcmp(key, "cover_obj"))
		return get_cover(mi);
	else if (0 == strcmp(key, "metadata_obj"))
		return get_metadata(mi);
	else if (0 == strcmp(key, "id3v2_obj"))
		return get_id3v2(mi);
}

void mi_send_id3v2(mi_id3v2_t *id3v2)
{
	if (!id3v2 || !id3v2->data || id3v2->size <= 0)
		return;

	mi_id3v2_t id3 = {0};
	id3.data = bmalloc(id3v2->size);
	if (!id3.data) {
		blog(LOG_WARNING,
		     "[mi] Fail to malloc new ID3V2 buffer, size %lld.",
		     id3v2->size);
		return;
	}
	memmove(id3.data, id3v2->data, id3v2->size);
	id3.size = id3v2->size;

	struct obs_core_audio *audio = &obs->audio;
	pthread_mutex_lock(&audio->id3v2_mutex);
	da_push_back(audio->id3v2_array, &id3);
	pthread_mutex_unlock(&audio->id3v2_mutex);
}

bool mi_get_id3v2(mi_id3v2_t *id3v2)
{
	if (!id3v2)
		return false;

	bool ret = false;
	struct obs_core_audio *audio = &obs->audio;
	pthread_mutex_lock(&audio->id3v2_mutex);
	if (audio->id3v2_array.num > 0) {
		struct mi_id3v2 id3 = audio->id3v2_array.array[0];
		id3v2->data = id3.data;
		id3v2->size = id3.size;
		da_erase(audio->id3v2_array, 0);
		ret = true;
	} else
		ret = false;
	pthread_mutex_unlock(&audio->id3v2_mutex);
	return ret;
}

void mi_free_id3v2(mi_id3v2_t *id3v2)
{
	if (!id3v2)
		return false;
	if (id3v2->data)
		bfree(id3v2->data);
	id3v2->size = 0;
}

void mi_clear_id3v2_queue()
{
	struct obs_core_audio *audio = &obs->audio;
	pthread_mutex_lock(&audio->id3v2_mutex);
	for (int i = 0; i < audio->id3v2_array.num; i++) {
		struct mi_id3v2 id3 = audio->id3v2_array.array[i];
		if (id3.data)
			bfree(id3.data);
	}
	da_free(audio->id3v2_array);
	pthread_mutex_unlock(&audio->id3v2_mutex);
}

bool mi_peek_id3v2(mi_id3v2_t *id3v2)
{
	if (!id3v2)
		return false;

	bool ret = false;
	struct obs_core_audio *audio = &obs->audio;
	pthread_mutex_lock(&audio->id3v2_mutex);
	if (audio->id3v2_array.num > 0) {
		struct mi_id3v2 id3 = audio->id3v2_array.array[0];
		id3v2->data = id3.data;
		id3v2->size = id3.size;
		ret = true;
	} else
		ret = false;
	pthread_mutex_unlock(&audio->id3v2_mutex);
	return ret;
}

bool mi_del_id3v2(mi_id3v2_t *id3v2)
{
	if (!id3v2)
		return false;

	bool ret = false;
	struct obs_core_audio *audio = &obs->audio;
	pthread_mutex_lock(&audio->id3v2_mutex);
	for (int i = 0; i < audio->id3v2_array.num; i++) {
		struct mi_id3v2 id3 = audio->id3v2_array.array[i];
		if (id3.data == id3v2->data && id3.size == id3v2->size) {
			da_erase(audio->id3v2_array, i);
			ret = true;
			break;
		}
	}
	pthread_mutex_unlock(&audio->id3v2_mutex);
	return ret;
}

bool mi_id3v2_queued()
{
	bool queued = false;
	struct obs_core_audio *audio = &obs->audio;
	pthread_mutex_lock(&audio->id3v2_mutex);
	queued = audio->id3v2_array.num > 0;
	pthread_mutex_unlock(&audio->id3v2_mutex);
	return queued;
}
