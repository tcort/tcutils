/*
    array.c - array utility functions
    Copyright (C) 2022, 2023, 2024  Thomas Cort

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

#include <tc/tc.h>

static int check_array_index_of(void) {
	char *array[] = {
		"FOO",
		"BAR",
		"BAZ",
		TC_NULL
	};

	return 
		tc_array_index_of(array, "FOO", tc_strcmp) ==  0 &&
		tc_array_index_of(array, "BAR", tc_strcmp) ==  1 &&
		tc_array_index_of(array, "BAZ", tc_strcmp) ==  2 &&
		tc_array_index_of(array, "BAM", tc_strcmp) == -1 ?
			TC_CHECK_PASS : TC_CHECK_FAIL;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_array_index_of, "can find needle in haystack using compar" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
