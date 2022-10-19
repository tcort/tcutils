/*
    mtrand.h - MT19937 Random Number Generator Algorithm
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

#ifndef TC_MTRAND_H
#define TC_MTRAND_H

#include "stdint.h"

struct tc_mtrand_t {
	tc_uint32_t index;
	tc_uint32_t mt[624];
};

struct tc_mtrand_t *tc_mtrand_alloc(tc_uint32_t seed);
tc_uint32_t tc_mtrand_next(struct tc_mtrand_t *mtrand);
struct tc_mtrand_t *tc_mtrand_free(struct tc_mtrand_t *mtrand);

#endif
