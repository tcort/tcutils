/*
    md2.c -- calculate a message-digest fingerprint for a file
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

#include <tc/tc.h>

struct testcase {
	char *input;
	char *expected;
};

static int check_md2(void) {
	int i;
	char *md2;

	struct testcase cases[] = {
		{ "", "8350e5a3e24c153df2275c9f80692773" },
		{ "a", "32ec01ec4a6dac72c0ab96fb34c0b5d1" },
		{ "abc", "da853b0d3f88d99b30283a69e6ded6bb" },
		{ "message digest", "ab4f496bfb2a530b219ff33031fe06b0" },
		{ "abcdefghijklmnopqrstuvwxyz", "4e8ddff3650292ab5a4108c3aa47940b" },
		{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "da33def2a42df13975352846c30338cd" },
		{ "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "d5976f79d83d3a0dc9806c3c66f3efd8" }
	}; 

	for (i = 0; i < sizeof(cases)/sizeof(cases[0]); i++) {

		md2 = tc_md2((tc_uint8_t *) cases[i].input, tc_strlen(cases[i].input));

		if (md2 == TC_NULL) {
			return TC_CHECK_FAIL;
		} else if (tc_streql(md2, cases[i].expected) == 0) {
			md2 = tc_free(md2);
			return TC_CHECK_FAIL;
		}

		md2 = tc_free(md2);
	}

	return TC_CHECK_PASS;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_md2,	"Test Suite (RFC1319)" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
