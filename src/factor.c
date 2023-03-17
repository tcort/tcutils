 /*
    factor -- prints the prime factors of a number
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned long factor(unsigned long ul) {
        unsigned long i;
	unsigned long s;

	s = ul/2;
        if (ul < 2) {
		return 0;
	} else if (ul % 2 == 0) {
		return 2;
	}

        for (i = 2; i <= s; i++) {
		if (ul%i==0) {
			return i;
		}
	}

        return ul;
}


int main(int argc, char *argv[]) {

	unsigned long ul;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "factor 42", .description = "show the prime factors of 42" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "factor",
		.usage = "[OPTIONS] INTEGER",
		.description = "prints the prime factors of a number",
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

	if (argc != 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	ul = strtoul(argv[0], TC_NULL, 10);
	fprintf(stdout, "%lu:", ul);

	do {
		unsigned long fact;
		fact = factor(ul);
		if (fact < 2) {
			break;
		}
		fprintf(stdout, " %lu", fact);
		ul /= fact;
	} while (1);

	fprintf(stdout, "\n");

	tc_exit(TC_EXIT_SUCCESS);
}
