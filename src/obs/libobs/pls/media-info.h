#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4204)
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <util/threading.h>

enum mi_type {
	MI_TYPE_BOOL,
	MI_TYPE_INT,
	MI_TYPE_STRING,
	MI_TYPE_OBJ,
};

enum mi_open_mode {
	MI_OPEN_DIRECTLY = 1,
	MI_OPEN_DEFER =
		2, // this should be used when user only want to get 'id3v2_obj'
	MI_OPEN_TRY_DECODER = 4,
};

typedef struct mi_option {
	char *key;
	char *help;
	enum mi_type type;
} mi_option;

extern const mi_option mi_options[];

/** ----------------------------------------------------------- */
/** ---------- defines for packed information object ---------- */
typedef void *mi_obj;

/** id3v2 for mp3 */
typedef struct mi_id3v2 {
	char *data; // header + body
	int size;
} mi_id3v2_t;

/** audio metadata */
typedef struct mi_metadata {
	char *title;
	char *artist;
	char *album;
} mi_metadata_t;

/** audio cover */
typedef struct mi_cover {
	char *data;
	int size;
	int width;
	int height;
	enum AVPixelFormat format;
} mi_cover_t;

/** -------------------------------------------------------------- */
/** ---------- define of the main struct for media info ---------- */
typedef struct media_info {
	enum mi_open_mode open_mode;
	bool opened;
	AVFormatContext *fmt;
	AVCodecContext *codec_ctx;
	pthread_mutex_t mutex;
	const char *path;
	int64_t open_ts;
	int64_t io_open_ts;
	bool abort;
	bool has_cover;
	int cover_index;
	mi_cover_t cover;
	mi_metadata_t metadata;
	mi_id3v2_t id3v2;
	bool id3v2_ready;
} media_info_t;

/** --------------------------------------------------------------------- */
/** ---------- interfaces for getting media info from file/url ---------- */
/** open an media, return true if open succeed, false otherwise.
  * the parameter 'mode' should be set to MI_OPEN_DEFER if you only want to get 'id3v2_obj'*/
EXPORT bool mi_open(media_info_t *mi, const char *path, enum mi_open_mode mode);

/** free the opened media */
EXPORT void mi_free(media_info_t *mi);

/** get property in format of int64_t, please refer to mi_options defined in media-info.c */
EXPORT long long mi_get_int(media_info_t *mi, const char *key);

/** get property in format of bool, please refer to mi_options defined in media-info.c */
EXPORT bool mi_get_bool(media_info_t *mi, const char *key);

/** get property in format of string, please refer to mi_options defined in media-info.c */
EXPORT char *mi_get_string(media_info_t *mi, const char *key);

/** get property in format of packed information object,
  * please refer to mi_options defined in media-info.c */
EXPORT mi_obj mi_get_obj(media_info_t *mi, const char *key);

/** ------------------------------------------------------------------- */
/** ---------- interfaces for sending/getting audio metadata ---------- */
/** send id3v2 info to core audio */
EXPORT void mi_send_id3v2(mi_id3v2_t *id3v2);

/** get id3v2 from core audio.
  * NOTE!!
  * The data get from this function MUST be freed manully by calling mi_free_id3v2 */
EXPORT bool mi_get_id3v2(mi_id3v2_t *id3v2);

/** free the id3v2 obj locally. */
EXPORT void mi_free_id3v2(mi_id3v2_t *id3v2);

/** clear the id3v2 queue. */
EXPORT void mi_clear_id3v2_queue();

/** peek audio info from core audio.
  * NOTE!!
  * DO NOT manully free the data gotten from this function */
EXPORT bool mi_peek_id3v2(mi_id3v2_t *id3v2);

/** delete the id3v2 from queue.
  * NOTE!!
  * This funciton will not free the buffer, it MUST be freed manully by calling mi_free_id3v2 */
EXPORT bool mi_del_id3v2(mi_id3v2_t *id3v2);

/** return true if there are id3v2 infos still queued in core audio, false otherwise */
EXPORT bool mi_id3v2_queued();

#ifdef __cplusplus
}
#endif
