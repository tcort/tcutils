/*
    md2.c -- calculate a message-digest fingerprint for a file
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

#include <tc/tc.h>

struct testcase {
	char *input;
	char *expected;
};

static int check_md2(void) {
	int i;
	char *md2;

	struct testcase cases[] = {
		{ "", "8350e5a3e24c153df2275c9f80692773" }
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
