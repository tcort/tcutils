/*
    wav.h - Waveform Audio File Format
    Copyright (C) 2022, 2023  Thomas Cort

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef TC_WAV_H
#define TC_WAV_H

#include "stdint.h"

#define TC_WAV_RIFF_MAGIC "RIFF"
#define TC_WAV_WAVE_MAGIC "WAVE"
#define TC_WAV_FMT_MAGIC "fmt "
#define TC_WAV_DATA_MAGIC "data"
#define TC_WAV_WAVE_FORMAT_PCM 1
#define TC_WAV_NCHANNELS 1
#define TC_WAV_SAMPLE_RATE 44100
#define TC_WAV_BITS_PER_SAMPLE 16

struct tc_wav_riff_chunk {
	char chunk_id[4]; /* RIFF */
	tc_int32_t chunk_size; /* 4+n */
	char wave_id[4]; /* WAVE */
};

struct tc_wav_fmt_chunk {
	char chunk_id[4]; /* "fmt " */
	tc_int32_t chunk_size;
	tc_int16_t audio_format; /* format code */
	tc_int16_t nchannels;
	tc_int32_t sample_rate;
	tc_int32_t data_rate;
	tc_int16_t block_align;
	tc_int16_t bits_per_sample;
	char data_tag[4];
	tc_int32_t data_size;
};

int tc_wav_open_writer(char *filepath);
int tc_wav_write(int fd, tc_int16_t data[], int len);
int tc_wav_close(int fd);

#endif
