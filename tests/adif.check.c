/*
    ctype.c - character type functions
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

static int check_valid_data_type_specifiers(void) {
	return
		tc_adif_is_valid_data_type_specifier('B') &&
		tc_adif_is_valid_data_type_specifier('N') &&
		tc_adif_is_valid_data_type_specifier('D') &&
		tc_adif_is_valid_data_type_specifier('T') &&
		tc_adif_is_valid_data_type_specifier('S') &&
		tc_adif_is_valid_data_type_specifier('M') &&
		tc_adif_is_valid_data_type_specifier('E') &&
		tc_adif_is_valid_data_type_specifier('L') ?
			TC_CHECK_PASS : TC_CHECK_FAIL;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_valid_data_type_specifiers, "valid data type specifiers are recognized correctly" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
