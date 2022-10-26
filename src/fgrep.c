 /*
    fgrep -- searches for and prints lines that exactly match a given string
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

static void fgrep(int in, char *pattern, char *filename, int flag_H) {

	char *p;

	do {
		char *line;
		int status;

		line = tc_getln(in, &status);
		if (line == TC_NULL || status == TC_ERR) {
			if (line != TC_NULL) {
				line = tc_free(line);
			}
			return;
		}

		p = tc_strstr(line, pattern);
		if (p != TC_NULL) {
			if (flag_H == 1) {
				tc_puts(TC_STDOUT, filename);
				tc_puts(TC_STDOUT, ":\t");
			}
			tc_putln(TC_STDOUT, line);
		}

		line = tc_free(line);

		if (status == TC_EOF) {
			return;
		}
	} while (1);

}

int main(int argc, char *argv[]) {

	int i;
	int in;
	char *pattern;
	int flag_H;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'H', .longarg = "filenames", .description = "print filenames with output lines", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "fgrep hello foo.txt bar.txt", .description = "search the files foo.txt and bar.txt for the fixed string hello" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "fgrep",
		.usage = "[OPTIONS] PATTERN [FILE...]",
		.description = "searches for and prints lines that exactly match a given string",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_H = 0;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'H':
				flag_H = 1;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc < 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	pattern = argv[0];

	if (argc == 1) {
		fgrep(TC_STDIN, pattern, "<stdin>", flag_H);
	} else {
		for (i = 1; i < argc; i++) {
			in = tc_open_reader(argv[i]);
			if (in == TC_ERR) {
				tc_puterr("Could not open file: ");
				tc_puterrln(argv[i]);
				tc_exit(TC_EXIT_FAILURE);
			}
			fgrep(in, pattern, argv[i], flag_H);
			tc_close(in);
		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}

