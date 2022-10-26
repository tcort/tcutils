 /*
    luhn.c -- luhn checker
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

    SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tc/ctype.h"
#include "tc/string.h"
#include "tc/sys.h"

static int doubles[] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};

int tc_luhn_check(char *s) {

	int i;
	int dub;
	int check_digit;
	int computed_check_digit;

	/* must be at least 2 digits (1 digit + check digit) */
	if (tc_strlen(s) < 2) {
		return 0;
	}

	/* all characters in string must be digits */
	for (i = 0; i < tc_strlen(s); i++) {
		if (!tc_isdigit(s[i])) {
			return 0;
		}
	}

	dub = 1;
	check_digit = tc_ctoi(s[tc_strlen(s) - 1]);
	computed_check_digit = 0;
	for (i = tc_strlen(s) - 2; i >= 0; i--) {
		computed_check_digit += dub ? doubles[tc_ctoi(s[i])] : tc_ctoi(s[i]);
		dub = !dub;
	}

	computed_check_digit = (10 - (computed_check_digit % 10)) % 10;

	return computed_check_digit == check_digit;
}

