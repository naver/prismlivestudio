#pragma once

struct obs_outro {
	volatile bool ready;
	volatile bool active;
	volatile bool exit;
	volatile bool stopping_output;

	bool thread_initialized;

	char *name;
	struct obs_source *source;
	gs_texture_t *texture;

	uint32_t render_width;
	uint32_t render_height;

	uint64_t timeout_nsec;
	uint64_t start_time_nsec;

	pthread_t outro_thread;
	
	float fake_audio_mix_buffer[MAX_AUDIO_CHANNELS][AUDIO_OUTPUT_FRAMES];
};

extern void *obs_outro_thread(void *param);
