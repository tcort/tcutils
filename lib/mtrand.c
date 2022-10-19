/*
    mtrand.c - MT19937 Random Number Generator Algorithm
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

#include "tc/mtrand.h"
#include "tc/stdlib.h"
#include "tc/stdint.h"
#include "tc/sys.h"

static const tc_uint32_t W =  32;
static const tc_uint32_t N = 624;
static const tc_uint32_t M = 397;
static const tc_uint32_t F = 1812433253;
static const tc_uint32_t U = 11;
static const tc_uint32_t D = 0xFFFFFFFF;
static const tc_uint32_t S = 7;
static const tc_uint32_t B = 0x9D2C5680;
static const tc_uint32_t T = 15;
static const tc_uint32_t C = 0xEFC60000;
static const tc_uint32_t L = 18;
static const tc_uint32_t R = 31;
static const tc_uint32_t HI = (1 << R);
static const tc_uint32_t LO = ((1 << R)) - ((tc_uint32_t) 1);
static const tc_uint32_t A = 0x9908B0DF;


struct tc_mtrand_t *tc_mtrand_alloc(tc_uint32_t seed) {

	struct tc_mtrand_t *mtrand;

	mtrand = tc_malloc(sizeof(struct tc_mtrand_t));
	if (mtrand == TC_NULL) {
		return TC_NULL;
	}

	while (seed == 0) {
		seed = tc_seed();
	}

	mtrand->mt[0] = seed;
	for (mtrand->index = 1; mtrand->index < N; mtrand->index++) {
		tc_uint32_t x = mtrand->mt[mtrand->index - 1] ^ (mtrand->mt[mtrand->index - 1] >> 30);
		mtrand->mt[mtrand->index] = (tc_uint32_t) (
			((((tc_uint32_t) (x & 0xffff0000) >> 16) * F) << 16) +
			((((tc_uint32_t) (x & 0x0000ffff) >>  0) * F) <<  0) +
			mtrand->index
		);
	}

	return mtrand;
}

tc_uint32_t tc_mtrand_next(struct tc_mtrand_t *mtrand) {

	tc_uint32_t y;

	if (mtrand->index >= N) {
		for (mtrand->index = 0; mtrand->index < N; mtrand->index++) {
			tc_uint32_t x = (mtrand->mt[mtrand->index] & HI) + (mtrand->mt[(mtrand->index + 1) % N] & LO);
			mtrand->mt[mtrand->index] = (mtrand->mt[(mtrand->index + M) % N] ^ x >> 1);
			mtrand->mt[mtrand->index] = ((x & 1) ? (mtrand->mt[mtrand->index] ^ A) : mtrand->mt[mtrand->index]);
		}
		mtrand->index = 0;
	}

	y = mtrand->mt[mtrand->index];
	y ^= (y >> U);
	y ^= (y << S) & B;
	y ^= (y << T) & C;
	y ^= (y >> L);
	mtrand->index++;

	return y;
}

struct tc_mtrand_t *tc_mtrand_free(struct tc_mtrand_t *mtrand) {
	return tc_free(mtrand);
}
