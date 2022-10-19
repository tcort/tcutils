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

#include <tc/tc.h>

int main(int argc, char *argv[]) {

	int i;
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
		tc_puts(TC_STDOUT, argv[i]);
		tc_puts(TC_STDOUT, "\t");

		if (tc_luhn_check(argv[i])) {
			tc_puts(TC_STDOUT, "OK\n");
		} else {
			tc_puts(TC_STDOUT, "BAD\n");
			failure = 1;

		}
	}

	tc_exit(failure == 0 ? TC_EXIT_SUCCESS : TC_EXIT_FAILURE);
}
