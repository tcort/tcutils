 /*
    clear -- clear the terminal screen
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

#include <curses.h>
#include <stdlib.h>
#include <term.h>

int main(int argc, char *argv[]) {

	int rc;
	char *cl;
	char *term;
	char entry[4096];
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "clear", .description = "clear the screen" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "clear",
		.usage = "[OPTIONS]",
		.description = "clear the terminal screen",
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

	term = getenv("TERM");
	if (term == TC_NULL) {
		perror("getenv");
		tc_exit(TC_EXIT_FAILURE);
	}

	rc = tgetent(entry, term);
	if (rc != 1) {
		perror("tgetent");
		tc_exit(TC_EXIT_FAILURE);
	}

	cl = tgetstr("cl", TC_NULL);
	if (cl == TC_NULL) {
		perror("tgetstr");
		tc_exit(TC_EXIT_FAILURE);
	}

	tputs(cl, 1, putchar);

	tc_exit(TC_EXIT_SUCCESS);
}
