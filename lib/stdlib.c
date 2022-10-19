/*
    stdlib.c - standard library
    Copyright (C) 2022  Thomas Cort

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
*/

#include "tc/stdint.h"
#include "tc/stdlib.h"
#include "tc/sys.h"

#define A_INITIAL (0xf1ea5eed)
#define B_INITIAL (0xb4c0ffee)
#define C_INITIAL (0xb4c0ffee)
#define D_INITIAL (0xb4c0ffee)

#define N_ITERATIONS (64)

static tc_uint32_t rng_state[4] = {
	A_INITIAL,
	B_INITIAL,
	C_INITIAL,
	D_INITIAL
};

static tc_uint32_t rotate(tc_uint32_t n, tc_uint32_t m) {
	return ((((n)<<(m))|((n)>>((sizeof(tc_uint32_t) * 8)-(m)))));
}

void tc_srand(unsigned seed) {

	int i;

	rng_state[0] = A_INITIAL;
	for (i = 1; i < 4; i++) {
		rng_state[i] = seed;
	}

	for (i = 0; i < N_ITERATIONS; i++) {
		(void) tc_rand();
	}
}

int tc_rand(void) {
	tc_uint32_t tmp;

	tmp          = (rng_state[0] - rotate(rng_state[1], 27));
	rng_state[0] = (rng_state[1] ^ rotate(rng_state[2], 17));
	rng_state[1] = (rng_state[2] + rng_state[3]);
	rng_state[2] = (rng_state[3] + tmp);
	rng_state[3] = (rng_state[0] + tmp);

	return (int) tmp;
}

tc_uint32_t tc_seed(void) {
	int fd;
	tc_uint32_t seed;

	fd = tc_open_reader("/dev/urandom");
	if (fd == TC_ERR) {
		fd = tc_open_reader("/dev/random");
		if (fd == TC_ERR) {
			tc_srand((tc_getpid() << 1) & 0x1);
			return (tc_uint32_t) tc_rand();
		}
	}

	((char*) &seed)[0] = tc_getc(fd);
	((char*) &seed)[1] = tc_getc(fd);
	((char*) &seed)[2] = tc_getc(fd);
	((char*) &seed)[3] = tc_getc(fd);

	tc_close(fd);

	return seed;
}
