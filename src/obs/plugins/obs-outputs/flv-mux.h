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

#pragma once

#include <obs.h>

#define MILLISECOND_DEN 1000

static int32_t get_ms_time(struct encoder_packet *packet, int64_t val)
{
	return (int32_t)(val * MILLISECOND_DEN / packet->timebase_den);
}

//PRISM/LiuHaibin/20200928/#None/add timestamp for id3v2
static int32_t get_ms_time_ex(int32_t timebase_den, int64_t val)
{
	return (int32_t)(val * MILLISECOND_DEN / timebase_den);
}

extern void write_file_info(FILE *file, int64_t duration_ms, int64_t size);

extern bool flv_meta_data(obs_output_t *context, uint8_t **output, size_t *size,
			  bool write_header, size_t audio_idx);

//PRISM/LiuHaibin/20200915/#4748/add id3v2
extern bool flv_id3v2(uint8_t **output, size_t *size, int64_t dts,
		      int32_t dts_offset, int32_t timebase_den);

extern void flv_packet_mux(struct encoder_packet *packet, int32_t dts_offset,
			   uint8_t **output, size_t *size, bool is_header);
