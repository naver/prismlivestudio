#pragma once

struct obs_thumbnail {
	volatile bool requested;
	volatile bool ready;
	uint32_t width;
	uint32_t height;
	uint8_t *data;
	enum gs_color_format format;
	gs_texture_t *texture;
	gs_stagesurf_t *stage_surface;
	bool (*callback)(void *param, uint32_t width, uint32_t height,
			 bool request_succeed);
	void *callback_param;
};
