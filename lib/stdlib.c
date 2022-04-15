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

	tmp 	 = (rng_state[0] - rotate(rng_state[1], 27));
	rng_state[0] = (rng_state[1] ^ rotate(rng_state[2], 17));
	rng_state[1] = (rng_state[2] + rng_state[3]);
	rng_state[2] = (rng_state[3] + tmp);
	rng_state[3] = (rng_state[0] + tmp);

	return (int) tmp;
}
