#pragma once
#include "util/dstr.h"

enum obs_watermark_show_type {
	OBS_WATERMARK_SHOW_TYPE_NOT_SHOW,
	OBS_WATERMARK_SHOW_TYPE_BEGIN,
	OBS_WATERMARK_SHOW_TYPE_SHOWING,
	OBS_WATERMARK_SHOW_TYPE_END
};

struct obs_watermark {
	enum obs_watermark_policy policy;
	enum obs_watermark_show_type show_type;

	/* must */
	uint32_t top_margin;
	uint32_t left_margin;

	/* optional, needed when policy is OBS_WATERMARK_POLICY_CUSTOM */
	uint64_t start_time_nsec;
	uint64_t show_time_nsec;
	uint64_t fade_in_time_nsec;
	uint64_t fade_out_time_nsec;
	uint64_t interval_nsec;

	uint32_t render_width;
	uint32_t render_height;

	gs_texture_t *texture;
	gs_texture_t *render_texture;
	gs_effect_t *opaque_effect;
	volatile bool enabled;
	volatile bool updated;
	volatile bool need_update;
	bool first_show;
	uint64_t next_timestamp;
	struct dstr file_path;
};
