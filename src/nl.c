 /*
    nl -- number lines of text
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	size_t lineno;
	FILE *f;
	char *line = TC_NULL;
	size_t cap = 0;
	ssize_t len = 0;
	int bflag;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{
			.arg = 'b',
			.longarg = "body-numbering",
			.description = "control line numbering. opt is one of: 'a' (all lines numbered) 't' (non-empty lines numbered) or 'n' (no lines numbered)",
			.has_value = 1
		},
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "nl foo.c", .description = "number the lines in file foo.c" },
		{ .command = "nl -b t foo.c", .description = "number the non-empty lines in file foo.c" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "nl",
		.usage = "[OPTIONS] FILE",
		.description = "number lines of text",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	bflag = 'a';

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'b':
				bflag = argval[0];
				if (bflag != 'a' && bflag != 't' && bflag != 'n') {
					tc_args_show_usage(&prog);
					tc_exit(TC_EXIT_FAILURE);
				}
				break;
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

	f = fopen(argv[0], "r");
	if (f == TC_NULL) {
		perror("fopen");
		tc_exit(TC_EXIT_FAILURE);
	}

	for (lineno = 1; !feof(f) && !ferror(f); lineno++) {
		len = getline(&line, &cap, f);
		if (len <= 0) {
			break;
		}

		switch (bflag) {
			case 'a':
				fprintf(stdout, "%6ld\t%s", lineno, line);
				break;
			case 't':
				if (line[0] == '\n') {
					fprintf(stdout, "%s", line);
					lineno--;
				} else {
					fprintf(stdout, "%6ld\t%s", lineno, line);
				}
				break;
			default: /* 'n' */
				fprintf(stdout, "%s", line);
				break;
		}
	}

	free(line);
	fclose(f);

	tc_exit(TC_EXIT_SUCCESS);
}
