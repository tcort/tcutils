/*
    wav.c - Waveform Audio File Format
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

#include "tc/stdlib.h"
#include "tc/stdint.h"
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/wav.h"

int tc_wav_open_writer(char *filepath) {

	int fd;
	int rc;
        struct tc_wav_riff_chunk riff;
	struct tc_wav_fmt_chunk fmt;

	fd = tc_open_writer(filepath);
	if (fd == TC_ERR) {
		return TC_ERR;
	}

	/* riff chunk */
	tc_memcpy(riff.chunk_id, TC_WAV_RIFF_MAGIC, tc_strlen(TC_WAV_RIFF_MAGIC));
	riff.chunk_size = 0;
	tc_memcpy(riff.wave_id, TC_WAV_WAVE_MAGIC, tc_strlen(TC_WAV_WAVE_MAGIC));

	rc = tc_write(fd, &riff, sizeof(struct tc_wav_riff_chunk));
	if (rc == TC_ERR) {
		tc_close(fd);
		return TC_ERR;
	}

	/* fmt chunk */
	tc_memcpy(fmt.chunk_id, TC_WAV_FMT_MAGIC, tc_strlen(TC_WAV_FMT_MAGIC));
	fmt.chunk_size = 16;
	fmt.audio_format = TC_WAV_WAVE_FORMAT_PCM;
	fmt.nchannels = TC_WAV_NCHANNELS;
	fmt.sample_rate = TC_WAV_SAMPLE_RATE;
	fmt.data_rate = TC_WAV_SAMPLE_RATE * (TC_WAV_BITS_PER_SAMPLE/8);
	fmt.block_align = TC_WAV_BITS_PER_SAMPLE/8;
	fmt.bits_per_sample = TC_WAV_BITS_PER_SAMPLE;
	tc_memcpy(fmt.data_tag, TC_WAV_DATA_MAGIC, tc_strlen(TC_WAV_DATA_MAGIC));
	fmt.data_size = 0;

	rc = tc_write(fd, &fmt, sizeof(struct tc_wav_fmt_chunk));
	if (rc == TC_ERR) {
		tc_close(fd);
		return TC_ERR;
	}

	return fd;
}

int tc_wav_write(int fd, tc_int16_t data[], int len) {

	return tc_write(fd, data, len * 2);
}

int tc_wav_close(int fd) {

	int rc;

	tc_int32_t file_len = (tc_int32_t) tc_lseek(fd, 0, TC_SEEK_END);
	tc_int32_t data_len = (tc_int32_t) (file_len - (tc_int32_t) (sizeof(struct tc_wav_riff_chunk) - sizeof(struct tc_wav_fmt_chunk)));
	tc_int32_t riff_len = (tc_int32_t) (file_len - 8);

	/* last field in header */
	tc_int32_t data_pos = sizeof(struct tc_wav_riff_chunk) + sizeof(struct tc_wav_fmt_chunk) - sizeof(tc_int32_t);
	tc_int32_t riff_pos = sizeof(tc_int32_t);

	tc_lseek(fd, data_pos, TC_SEEK_SET);
	if (rc == TC_ERR) {
		tc_close(fd);
		return TC_ERR;
	}

	rc = tc_write(fd, &data_len, sizeof(tc_int32_t));
	if (rc == TC_ERR) {
		tc_close(fd);
		return TC_ERR;
	}

	rc = tc_lseek(fd, riff_pos, TC_SEEK_SET);
	if (rc == TC_ERR) {
		tc_close(fd);
		return TC_ERR;
	}

	rc = tc_write(fd, &riff_len, sizeof(tc_int32_t));
	if (rc == TC_ERR) {
		tc_close(fd);
		return TC_ERR;
	}

	return tc_close(fd);
}
