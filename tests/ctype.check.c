/*
    ctype.c - character type functions
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

#include <tc/tc.h>

static int check_isdigit_positive(void) {
	return
		tc_isdigit('0') &&
		tc_isdigit('1') &&
		tc_isdigit('2') &&
		tc_isdigit('3') &&
		tc_isdigit('4') &&
		tc_isdigit('5') &&
		tc_isdigit('6') &&
		tc_isdigit('7') &&
		tc_isdigit('8') &&
		tc_isdigit('9') ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_isdigit_count(void) {
	int i;
	int cnt;
	for (i = cnt = 0; i < 128; i++) {
		cnt += tc_isdigit(i);
	}
	return cnt == 10 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_isupper_count(void) {
	int i;
	int cnt;
	for (i = cnt = 0; i < 128; i++) {
		cnt += tc_isupper(i);
	}
	return cnt == 26 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_islower_count(void) {
	int i;
	int cnt;
	for (i = cnt = 0; i < 128; i++) {
		cnt += tc_islower(i);
	}
	return cnt == 26 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_isalpha_count(void) {
	int i;
	int cnt;
	for (i = cnt = 0; i < 128; i++) {
		cnt += tc_isalpha(i);
	}
	return cnt == 52 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_isspace_count(void) {
	int i;
	int cnt;
	for (i = cnt = 0; i < 128; i++) {
		cnt += tc_isspace(i);
	}
	return cnt == 6 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_isdigit_positive, "digits 0-9 return true" },
		{ check_isdigit_count, "10 characters are digits" },
		{ check_isupper_count, "26 characters are upper" },
		{ check_islower_count, "26 characters are lower" },
		{ check_isalpha_count, "52 characters are alpha" },
		{ check_isspace_count, "6 characters are space" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
