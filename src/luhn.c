 /*
    luhn -- luhn checker
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

#include "tc/args.h"
#include "tc/const.h"
#include "tc/ctype.h"
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int doubles[] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};

static int ctoi(int ch) {
	switch (ch) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		default:  return 0;
	}
}

static int luhn(char *s) {

	int i;
	int dub;
	int check_digit;
	int computed_check_digit;

	/* must be at least 2 digits (1 digit + check digit) */
	if (tc_strlen(s) < 2) {
		return -1;
	}

	/* all characters in string must be digits */
	for (i = 0; i < tc_strlen(s); i++) {
		if (!tc_isdigit(s[i])) {
			return -1;
		}
	}

	dub = 1;
	check_digit = ctoi(s[tc_strlen(s) - 1]);
	computed_check_digit = 0;
	for (i = tc_strlen(s) - 2; i >= 0; i--) {
		computed_check_digit += dub ? doubles[ctoi(s[i])] : ctoi(s[i]);
		dub = !dub;
	}

	computed_check_digit = (10 - (computed_check_digit % 10)) % 10;

	return computed_check_digit == check_digit ? 0 : -1;
}

int main(int argc, char *argv[]) {

	int ch;
	int i;
	int rc;
	int failure;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "luhn 4030000010001234", .description = "check the luhn of a string of digits" },
		{ .command = "luhn 4030000010001234 4003050500040005 5100000010001004", .description = "check several at once" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "luhn",
		.usage = "[OPTIONS] [DIGITS...]",
		.description = "luhn checker",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	failure = 0;
	for (i = 0; i < argc; i++) {
		rc = luhn(argv[i]);
		if (rc == 0) {
			fprintf(stdout, "%s\tOK\n", argv[i]);
		} else {
			fprintf(stdout, "%s\tBAD\n", argv[i]);
			failure = 1;

		}
	}

	tc_exit(failure == 0 ? TC_EXIT_SUCCESS : TC_EXIT_FAILURE);
}
