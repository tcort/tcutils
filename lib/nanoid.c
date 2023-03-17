/*
    nanoid - generate URL friendly unique ID strings
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

#include "tc/mtrand.h"
#include "tc/nanoid.h"
#include "tc/stdlib.h"
#include "tc/sys.h"

const int length = 21;
static const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-";

char *tc_nanoid(void) {

	int i;
	char *nanoid;
	struct tc_mtrand_t *mtrand;

	nanoid = tc_malloc(length + 1);
	if (nanoid == TC_NULL) {
		return TC_NULL;
	}

	mtrand = tc_mtrand_alloc(tc_seed());
	if (mtrand == TC_NULL) {
		nanoid = tc_free(nanoid);
		return TC_NULL;
	}

	for (i = 0; i < length; i++) {
		nanoid[i] = alphabet[tc_mtrand_next(mtrand) & 0x3F];
	}

	mtrand = tc_mtrand_free(mtrand);
	return nanoid;
}
