/*************************************************************************
 * This file is part of spectralizer
 * github.con/univrsal/spectralizer
 * Copyright 2020 univrsal <universailp@web.de>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#pragma once
#include "spectrum-visualizer.hpp"
#include "graphics-d2d.h"

class bar_visualizer : public spectrum_visualizer {
public:
	explicit bar_visualizer(config *cfg);
	~bar_visualizer();

	void render() override;
	bool update_frame() override;
	gs_texture *get_texture() override;

private:
	void draw_bars();
	void render_begin(gs_texture_t *render_target);
	void render_end();
	void fill_solid_color();
	void render_bars();
	void render_mix_sampling(gs_texture_t *color_texture, gs_texture_t *src_texture);
	void release_frame();

	void extend_blur_render(gs_texture_t *src);
	void region_extend_render(gs_texture_t *src);

	PLSGraphicsD2D *graphics_d2d{};

	/* effect */
	gs_effect_t *mask_effect{};
	gs_technique_t *mask_tech;
	gs_samplerstate_t *sampler{};

	gs_texture_t *render_texture{};
	gs_texture_t *source_texture{};
	gs_texture_t *solid_texture{};
	gs_texture_t *pre_target{};

	bar_mode old_bar_mode;
	gradient_color old_gradient_mode;

	gs_texture_t *extend_texture{};
	gs_texture_t *alpha_blur_texture{};

	gs_effect_t *outter_blur_effect{};
	gs_technique_t *outter_blur_tech{};

	gs_effect_t *region_extend_effect{};
	gs_technique_t *region_extend_tech{};
};
