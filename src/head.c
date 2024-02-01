 /*
    head -- prints lines at the beginning of an input text
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

int main(int argc, char *argv[]) {

	int i;
	int flag_n;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'n', .longarg = "lines", .description = "number of lines to show (default 10)", .has_value = 1 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "head foo.txt", .description = "print the first 10 lines of foo.txt" },
		{ .command = "head -n 2 foo.txt", .description = "print the first 2 lines of foo.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "head",
		.usage = "[OPTIONS] [FILE...]",
		.description = "prints lines at the beginning of an input text",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_n = 10;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'n':
				flag_n = tc_atoi(argval);
				flag_n = flag_n < 0 ? 10 : flag_n;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc == 0) {
		tc_copylns(TC_STDIN, TC_STDOUT, flag_n);
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		int fd;
		fd = tc_open_reader(argv[i]);
		if (fd == TC_ERR) {
			tc_puterr("Failed to open file: ");
			tc_puterr(argv[i]);
			tc_puterr("\n");
			tc_exit(TC_EXIT_FAILURE);
		}

		tc_copylns(fd, TC_STDOUT, flag_n);

		tc_close(fd);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
