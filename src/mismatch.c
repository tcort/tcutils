 /*
    mismatch -- finds lines that are in input but not in FILE
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
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int g=0;
	char *x=TC_NULL, *y = TC_NULL;
	size_t xc = 0, yc = 0;
	ssize_t xn, yn;
	FILE *f;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{
			.description = "find words in input that are not in dictionary",
			.command = "makewords foo.txt | lowercase | sort | uniq | mismatch /usr/share/dict/words"
		},
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "mismatch",
		.usage = "[OPTIONS] FILE",
		.description = "finds lines that are in input but not in FILE",
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

	if (argc < 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	f = fopen(argv[0], "r");
	if (f == TC_NULL) {
		perror("fopen");
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((yn = getline(&y, &yc, stdin)) != -1) {
		while (g == 1 ||(xn = getline(&x, &xc, f)) != -1) {
			if (strcmp(y,x) <= 0) {
				break;
			}
			g = 0;
		}
		if (strcmp(y,x) != 0) {
			printf("%s", y);
			g = 1;
		}
	}

	if (x != TC_NULL) {
		free(x);
	}
	if (y != TC_NULL) {
		free(y);
	}
	fclose(f);

	tc_exit(TC_EXIT_SUCCESS);
}
