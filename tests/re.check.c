/*
    re.c - regular expressions
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
#include <stdio.h>

static int check_matches(void) {
	tc_uint64_t i;

	char *pass[][2] = {
		{ "13.37", "[123456789]+[0123456789]*\\.[0123456789][0123456789]" },
		{ "a03.37a", "[123456789]+[0123456789]*\\.[0123456789][0123456789]" },
		{ "Hello", "[Hh]ello" },
		{ "abababc", "a[ab]+c" },
		{ "Hello", "[^h]ello" },
		{ "hello", "[Hh]ello" },
		{ "Hello", "^H.l*o$" },
		{ "Hello", "Hel+o"   },
		{ "Hello", "^"       },
		{ "Hello", "$"       },
		{ "Hello", "Hell?o"  },
		{ "Hello", "Hell?o"  },
		{ "Helo",  "Hell?o"  },
		{ "",      "^$"      },
		{ "",      ""        },
		{ "Hello", "Hello"   },
		{ "\\^\\.\\*\\$", "^.*$" },
		{ "This is text", "This is text"},
		{ "a4", "[a-z0-9]+"},
		{ "z", "[a-z0-9]"},
		{ "a", "[^b-z0-9]"},
		{ "9", "[^b-z0-8]"},
		{ "-", "[-]"},
		{ "4", "[:digit:]" },
		{ "C42C", "C[:digit:]+C" }
	};
#define NPASS (sizeof(pass)/sizeof(pass[0]))

	for (i = 0; i < NPASS; i++) {
		printf("subject='%s' pattern='%s'\n", pass[i][0], pass[i][1]);
		if (tc_match(pass[i][0], pass[i][1]) != 1) {
			return TC_CHECK_FAIL;
		}
	}

	return TC_CHECK_PASS;
}

static int check_no_matches(void) {

	tc_uint64_t i;

	char *fail[][2] = {
		{ "a", "[b-z0-9]"},
		{ "9", "[b-z0-8]"},
		{ "yello", "[Hh]ello" },
		{ "Heo",  "Hel+o"   },
		{ "abc",  NULL      },
		{ NULL,   "abc"     },
		{ NULL,   NULL      },
		{ "Hola", "^H.l*o$" },
		{ "CxxC", ".[:digit:]."    }
	};
#define NFAIL (sizeof(fail)/sizeof(fail[0]))


	for (i = 0; i < NFAIL; i++) {
		printf("subject='%s' pattern='%s'\n", fail[i][0], fail[i][1]);
		if (tc_match(fail[i][0], fail[i][1]) != 0) {
			return TC_CHECK_FAIL;
		}
	}

	return TC_CHECK_PASS;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_matches,	"tc_match(pattern, subject) matches" },
		{ check_no_matches,	"tc_match(pattern, subject) no matches" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
