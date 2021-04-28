#include <stdio.h>
#include <util/dstr.h>
#include <util/darray.h>
#include <util/platform.h>
#include <obs-module.h>

#include <x265.h>
#include <obs-hevc.h>
#include <libavcodec/avcodec.h>

#ifndef _STDINT_H_INCLUDED
#define _STDINT_H_INCLUDED
#endif

// only support AV_PIX_FMT_YUV444P, AV_PIX_FMT_YUV420P
#define LIBX265_DEPTH 8

#define do_log(level, format, ...) blog(level, "[x265 encoder: '%s'] " format, obs_encoder_get_name(obj->obs_encoder), ##__VA_ARGS__)

#define warn(format, ...) do_log(LOG_WARNING, format, ##__VA_ARGS__)
#define info(format, ...) do_log(LOG_INFO, format, ##__VA_ARGS__)
#define debug(format, ...) do_log(LOG_DEBUG, format, ##__VA_ARGS__)

#define TEXT_RATE_CONTROL obs_module_text("RateControl")
#define TEXT_BITRATE obs_module_text("Bitrate")
#define TEXT_KEYINT_SEC obs_module_text("KeyframeIntervalSec")
#define TEXT_PRESET obs_module_text("CPUPreset")
#define TEXT_PROFILE obs_module_text("Profile")
#define TEXT_TUNE obs_module_text("Tune")
#define TEXT_NONE obs_module_text("None")

#define PROPERTY_KEY_RATE_METHOD "rate_control"
#define PROPERTY_KEY_KEYFRAME "keyframe_second"
#define PROPERTY_KEY_PROFILE "profile"
#define PROPERTY_KEY_PRESET "preset"
#define PROPERTY_KEY_TUNE "tune"
#define PROPERTY_KEY_BITRATE "bitrate"
#define PROPERTY_KEY_CQP "CQP"
#define PROPERTY_KEY_CRF "CRF"

enum rate_control_method {
	RATE_CONTROL_CBR = 0,
	RATE_CONTROL_ABR,
	RATE_CONTROL_CQP,
	RATE_CONTROL_CRF,
};

/* ------------------------------------------------------------------------- */
struct prism_x265 {
	obs_encoder_t *obs_encoder;

	const x265_api *x265_api;
	x265_param *x265_params;
	x265_encoder *x265_encoder;

	DARRAY(uint8_t) packet_data;

	uint8_t *extra_data;
	size_t extra_data_size;

	uint8_t *sei;
	size_t sei_size;
};

/* ------------------------------------------------------------------------- */
static bool valid_format(enum video_format format)
{
	// NOTE! For this release of libx265-3.4, only X265_CSP_I420 and X265_CSP_I444 are supported
	return format == VIDEO_FORMAT_I420 || format == VIDEO_FORMAT_I444;
}

static const char *prism_x265_getname(void *unused)
{
	return "x265";
}

static void prism_x265_clear(struct prism_x265 *obj)
{
	if (!obj) {
		return;
	}

	if (obj->x265_api) {
		if (obj->x265_encoder) {
			obj->x265_api->encoder_close(obj->x265_encoder);
			obj->x265_encoder = NULL;
		}

		if (obj->x265_params) {
			obj->x265_api->param_free(obj->x265_params);
			obj->x265_params = NULL;
		}
	}

	if (obj->extra_data) {
		bfree(obj->extra_data);
		obj->extra_data = NULL;
	}

	if (obj->sei) {
		bfree(obj->sei);
		obj->sei = NULL;
	}

	da_free(obj->packet_data);
}

static void prism_x265_video_info(void *data, struct video_scale_info *info)
{
	struct prism_x265 *obj = data;
	enum video_format pref_format;

	pref_format = obs_encoder_get_preferred_video_format(obj->obs_encoder);

	if (!valid_format(pref_format)) {
		pref_format = valid_format(info->format) ? info->format : VIDEO_FORMAT_I420;
	}

	info->format = pref_format;
}

static bool update_settings(struct prism_x265 *obj, obs_data_t *settings, bool update)
{
	video_t *video = obs_encoder_video(obj->obs_encoder);
	const struct video_output_info *voi = video_output_get_info(video);

	struct video_scale_info info;
	info.format = voi->format;
	info.colorspace = voi->colorspace;
	info.range = voi->range;
	prism_x265_video_info(obj, &info);

	enum rate_control_method method = (enum rate_control_method)obs_data_get_int(settings, "rate_control");
	const char *profile = obs_data_get_string(settings, "profile");
	const char *preset = obs_data_get_string(settings, "preset");
	const char *tune = obs_data_get_string(settings, "tune");
	int keyframe_sec = (int)obs_data_get_int(settings, "keyframe_second");
	int bitrate = (int)obs_data_get_int(settings, "bitrate");
	int qp = (int)obs_data_get_int(settings, "cqp");
	int rf = (int)obs_data_get_int(settings, "crf");

	if (obj->x265_api->param_default_preset(obj->x265_params, preset, tune) < 0) {
		warn("Unsupported preset and tune. preset:%s tune:%s", preset ? preset : "null", tune ? tune : "null");
		return false;
	}

	if (profile && (*profile)) {
		if (obj->x265_api->param_apply_profile(obj->x265_params, profile) < 0) {
			warn("Unsupported profile value : %s", profile ? profile : "null");
			return false;
		}
	}

	obj->x265_params->frameNumThreads = 0; // 0 for auto-detection
	obj->x265_params->bRepeatHeaders = 0;
	obj->x265_params->bAnnexB = true;
	obj->x265_params->bDisableLookahead = 1;

	assert(voi->fps_den > 0);
	obj->x265_params->keyframeMax = keyframe_sec * voi->fps_num / voi->fps_den;

	obj->x265_params->fpsNum = voi->fps_num;
	obj->x265_params->fpsDenom = voi->fps_den;

	obj->x265_params->sourceWidth = voi->width;
	obj->x265_params->sourceHeight = voi->height;

	if (obj->x265_params->sourceWidth < 64 || obj->x265_params->sourceHeight < 64)
		obj->x265_params->maxCUSize = 32;
	if (obj->x265_params->sourceWidth < 32 || obj->x265_params->sourceHeight < 32)
		obj->x265_params->maxCUSize = 16;
	if (obj->x265_params->sourceWidth < 16 || obj->x265_params->sourceHeight < 16) {
		warn("Resolution of video is too small (%dx%d)", obj->x265_params->sourceWidth, obj->x265_params->sourceHeight);
		return false;
	}

#if 0
	// these params can increase CPU usage, but seems no improvement for encode speed
	obj->x265_params->maxCUSize = 32;
	x265_param_parse(obj->x265_params, "pmode", NULL);
	x265_param_parse(obj->x265_params, "pme", NULL);
#endif

	switch (info.format) {
	case VIDEO_FORMAT_I420:
		obj->x265_params->internalCsp = X265_CSP_I420;
		break;
	case VIDEO_FORMAT_I444:
		obj->x265_params->internalCsp = X265_CSP_I444;
		break;
	default:
		warn("Unsupported video format : %d, libx265 only support YUV420 and YUV444.", info.format);
		return false;
	}

	switch (method) {
	case RATE_CONTROL_CBR:
	case RATE_CONTROL_ABR:
		obj->x265_params->rc.bitrate = bitrate;
		obj->x265_params->rc.vbvBufferSize = bitrate;
		obj->x265_params->rc.vbvMaxBitrate = bitrate;
		obj->x265_params->rc.rateControlMode = X265_RC_ABR;
		obj->x265_params->rc.bStrictCbr = (method == RATE_CONTROL_CBR);
		break;
	case RATE_CONTROL_CQP:
		obj->x265_params->rc.qp = qp;
		obj->x265_params->rc.rateControlMode = X265_RC_CQP;
		break;
	case RATE_CONTROL_CRF:
		obj->x265_params->rc.rfConstant = rf;
		obj->x265_params->rc.rateControlMode = X265_RC_CRF;
		break;
	default:
		warn("Unsupported rate control method: %d", method);
		return false;
	}

	return true;
}

static void load_headers(struct prism_x265 *obj)
{
	x265_nal *nals;
	int nal_count;

	int extradata_size = obj->x265_api->encoder_headers(obj->x265_encoder, &nals, &nal_count);
	if (extradata_size <= 0) {
		warn("x265 failed to encode headers");
		return;
	}

	DARRAY(uint8_t) header;
	DARRAY(uint8_t) sei;
	da_init(header);
	da_init(sei);

	for (int i = 0; i < nal_count; i++) {
		x265_nal *nal = nals + i;

		switch (nal->type) {
		case NAL_UNIT_PREFIX_SEI:
		case NAL_UNIT_SUFFIX_SEI:
			da_push_back_array(sei, nal->payload, nal->sizeBytes);
			break;

		default:
			da_push_back_array(header, nal->payload, nal->sizeBytes);
			break;
		}
	}

	obj->extra_data = header.array;
	obj->extra_data_size = header.num;
	obj->sei = sei.array;
	obj->sei_size = sei.num;
}

static void *prism_x265_create(obs_data_t *settings, obs_encoder_t *encoder)
{
	struct prism_x265 *obj = bzalloc(sizeof(struct prism_x265));
	obj->obs_encoder = encoder;

	do {
		obj->x265_api = x265_api_get(LIBX265_DEPTH);
		if (!obj->x265_api) {
			obj->x265_api = x265_api_get(0);
		}

		if (!obj->x265_api) {
			warn("x265 failed to get api");
			break;
		}

		obj->x265_params = obj->x265_api->param_alloc();
		if (!obj->x265_params) {
			warn("x265 failed to alloc params object");
			break;
		}

		obj->x265_api->param_default(obj->x265_params);
		if (!update_settings(obj, settings, false)) {
			warn("bad settings specified for x265");
			break;
		}

		obj->x265_encoder = obj->x265_api->encoder_open(obj->x265_params);
		if (!obj->x265_encoder) {
			warn("x265 failed to open encoder");
			break;
		}

		load_headers(obj);

		info("x265 is created successfully");
		return obj;

	} while (false);

	prism_x265_clear(obj);
	bfree(obj);

	assert(false);
	return NULL;
}

static void prism_x265_destroy(void *data)
{
	struct prism_x265 *obj = data;
	if (obj) {
		prism_x265_clear(obj);
		bfree(obj);
	}
}

static void prism_x265_defaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, PROPERTY_KEY_RATE_METHOD, RATE_CONTROL_CBR);
	obs_data_set_default_int(settings, PROPERTY_KEY_KEYFRAME, HEVC_KEYFRAME_SEC);
	obs_data_set_default_string(settings, PROPERTY_KEY_PRESET, "ultrafast");
	obs_data_set_default_string(settings, PROPERTY_KEY_TUNE, "zerolatency");
	obs_data_set_default_string(settings, PROPERTY_KEY_PROFILE, "");

	obs_data_set_default_int(settings, PROPERTY_KEY_BITRATE, HEVC_DEFAULT_BITRATT);
	obs_data_set_default_int(settings, PROPERTY_KEY_CQP, HEVC_DEFAULT_CQP);
	obs_data_set_default_int(settings, PROPERTY_KEY_CRF, HEVC_DEFAULT_CRF);
}

static bool rate_control_modified(obs_properties_t *ppts, obs_property_t *p, obs_data_t *settings)
{
	enum rate_control_method method = (enum rate_control_method)obs_data_get_int(settings, PROPERTY_KEY_RATE_METHOD);

	p = obs_properties_get(ppts, PROPERTY_KEY_CRF);
	obs_property_set_visible(p, method == RATE_CONTROL_CRF);

	p = obs_properties_get(ppts, PROPERTY_KEY_CQP);
	obs_property_set_visible(p, method == RATE_CONTROL_CQP);

	p = obs_properties_get(ppts, PROPERTY_KEY_BITRATE);
	obs_property_set_visible(p, method == RATE_CONTROL_CBR || method == RATE_CONTROL_ABR);

	return true;
}

static const char *const x265_preset_list[] = {"ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow", "placebo", NULL};
static const char *const x265_tune_list[] = {"psnr", "ssim", "grain", "zerolatency", "fastdecode", "animation", NULL};
static const char *const x265_profile_list[] = {
	/* HEVC v1 */
	"main",
	"main10",
	"mainstillpicture",
	/* alias */ "msp",

	/* HEVC v2 (Range Extensions) */
	"main-intra",
	"main10-intra",
	"main444-8",
	"main444-intra",
	"main444-stillpicture",

	"main422-10",
	"main422-10-intra",
	"main444-10",
	"main444-10-intra",

	"main12",
	"main12-intra",
	"main422-12",
	"main422-12-intra",
	"main444-12",
	"main444-12-intra",

	NULL,

	"main444-16-intra",        /* Not Supported! */
	"main444-16-stillpicture", /* Not Supported! */

	NULL,
};

static obs_properties_t *prism_x265_properties(void *unused)
{
	obs_properties_t *props = obs_properties_create();
	obs_property_t *list;
	obs_property_t *p;

	list = obs_properties_add_list(props, PROPERTY_KEY_RATE_METHOD, TEXT_RATE_CONTROL, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(list, "CBR", RATE_CONTROL_CBR);
	obs_property_list_add_int(list, "ABR", RATE_CONTROL_ABR);
	obs_property_list_add_int(list, "CQP", RATE_CONTROL_CQP);
	obs_property_list_add_int(list, "CRF", RATE_CONTROL_CRF);
	obs_property_set_modified_callback(list, rate_control_modified);

	p = obs_properties_add_int(props, PROPERTY_KEY_BITRATE, TEXT_BITRATE, 50, 60000, 50);
	obs_property_int_set_suffix(p, " Kbps");

	obs_properties_add_int(props, PROPERTY_KEY_CRF, "rf", 0, 51, 1);
	obs_properties_add_int(props, PROPERTY_KEY_CQP, "qp", 0, 51, 1);
	obs_properties_add_int(props, PROPERTY_KEY_KEYFRAME, TEXT_KEYINT_SEC, 1, 20, 1);

	list = obs_properties_add_list(props, PROPERTY_KEY_PRESET, TEXT_PRESET, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	for (size_t i = 0; x265_preset_list[i] != NULL; i++) {
		obs_property_list_add_string(list, x265_preset_list[i], x265_preset_list[i]);
	}

	list = obs_properties_add_list(props, PROPERTY_KEY_TUNE, TEXT_TUNE, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	for (size_t i = 0; x265_tune_list[i] != NULL; i++) {
		obs_property_list_add_string(list, x265_tune_list[i], x265_tune_list[i]);
	}

	list = obs_properties_add_list(props, PROPERTY_KEY_PROFILE, TEXT_PROFILE, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(list, TEXT_NONE, "");
	for (size_t i = 0; x265_profile_list[i] != NULL; i++) {
		obs_property_list_add_string(list, x265_profile_list[i], x265_profile_list[i]);
	}

	return props;
}

static bool prism_x265_update(void *data, obs_data_t *settings)
{
	struct prism_x265 *obj = data;

	bool success = update_settings(obj, settings, true);
	if (!success) {
		warn("Failed to update settings for reconfig");
		return false;
	}

	int ret = obj->x265_api->encoder_reconfig(obj->x265_encoder, obj->x265_params);
	if (ret != 0) {
		warn("Failed to reconfigure: %d", ret);
		return false;
	} else {
		return true;
	}
}

static bool prism_x265_extra_data(void *data, uint8_t **extra_data, size_t *size)
{
	struct prism_x265 *obj = data;

	if (!obj->x265_encoder) {
		return false;
	}

	*extra_data = obj->extra_data;
	*size = obj->extra_data_size;
	return true;
}

static bool prism_x265_sei(void *data, uint8_t **sei, size_t *size)
{
	struct prism_x265 *obj = data;

	if (!obj->x265_encoder) {
		return false;
	}

	*sei = obj->sei;
	*size = obj->sei_size;
	return true;
}

static bool prism_x265_encode(void *data, struct encoder_frame *frame, struct encoder_packet *packet, bool *received_packet)
{
	if (!packet || !received_packet) {
		return false;
	}

	struct prism_x265 *obj = data;
	x265_picture x265pic = {0};
	x265_picture x265pic_out = {0};
	x265_nal *nals;
	int nal_count;

	obj->x265_api->picture_init(obj->x265_params, &x265pic);

	if (frame) {
		for (int i = 0; i < 3; i++) {
			x265pic.planes[i] = frame->data[i];
			x265pic.stride[i] = (int)frame->linesize[i];
		}

		x265pic.pts = frame->pts;
		x265pic.bitDepth = LIBX265_DEPTH;
		x265pic.sliceType = X265_TYPE_AUTO;
	}

	uint64_t start = os_gettime_ns();
	int ret = obj->x265_api->encoder_encode(obj->x265_encoder, &nals, &nal_count, frame ? &x265pic : NULL, &x265pic_out);
	uint64_t end = os_gettime_ns();
	if (ret < 0) {
		warn("Failed to encoder video. error_code:%d", ret);
		return false;
	}

	if (nal_count > 0) {
		bool keyframe = false;
		da_resize(obj->packet_data, 0);

		for (int i = 0; i < nal_count; i++) {
			x265_nal *nal = nals + i;
			da_push_back_array(obj->packet_data, nal->payload, nal->sizeBytes);

			if (!keyframe && obs_hevc_keynal(nal->type)) {
				keyframe = true;
			}
		}

		packet->data = obj->packet_data.array;
		packet->size = obj->packet_data.num;
		packet->type = OBS_ENCODER_VIDEO;
		packet->pts = x265pic_out.pts;
		packet->dts = x265pic_out.dts;
		packet->keyframe = keyframe;

		//blog(LOG_DEBUG, "pts:%lld, len:%u key:%d  taketime:%lldms ", packet->pts, packet->size, keyframe, (end - start) / 1000000);
	}

	*received_packet = (nal_count > 0);
	return true;
}

/* ------------------------------------------------------------------------- */
struct obs_encoder_info prism_x265_encoder = {
	.id = "prism_x265",
	.type = OBS_ENCODER_VIDEO,
	.codec = "hevc",
	.get_name = prism_x265_getname,

	.create = prism_x265_create,
	.destroy = prism_x265_destroy,
	.encode = prism_x265_encode,

	.get_defaults = prism_x265_defaults,
	.get_properties = prism_x265_properties,
	.update = prism_x265_update,

	.get_extra_data = prism_x265_extra_data,
	.get_sei_data = prism_x265_sei,

	.get_video_info = prism_x265_video_info,
	.caps = OBS_ENCODER_CAP_DYN_BITRATE,
};
