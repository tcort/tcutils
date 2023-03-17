 /*
    hr -- horizontal rule generator
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

#include <sys/ioctl.h>
#include <stdlib.h>

#define DEFAULT_BAR ("#")

static void hr(char *bar, size_t cols) {

	int i;
	int len;

	len = tc_strlen(bar);
	for (i = 0; i < cols - 1; i++) {
		tc_putc(TC_STDOUT, bar[i % len]);
	}
	tc_putc(TC_STDOUT, '\n');
}

int main(int argc, char *argv[]) {

	int i;
	size_t cols;
	struct winsize ws;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "hr", .description = "horizontal rule with '#'" },
		{ .command = "hr '*'", .description = "horizontal rule with '*'" },
		{ .command = "hr '+=+' '[::]'" , .description = "two horizontal rules with various patterns" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "hr",
		.usage = "[OPTIONS] [SEPARATOR...]",
		.description = "horizontal rule generator",
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

	ioctl(TC_STDOUT, TIOCGWINSZ, &ws);
	cols = ws.ws_col;

	if (argc == 0) {
		hr(DEFAULT_BAR, cols);
	} else {
		for (i = 0; i < argc; i++) {
			hr(argv[i], cols);
		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}
