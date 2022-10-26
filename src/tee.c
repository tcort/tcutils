 /*
    tee -- copy characters from standard input to standard output and a file
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
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch, i, rc;
	FILE **fp;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "ps | tee foo.txt", .description = "write the results of ps to foo.txt and standard output" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "tee",
		.usage = "[OPTIONS] FILE...",
		.description = "copy characters from standard input to standard output and a file",
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

	if (argc == 0) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	fp = (FILE **) malloc(sizeof(FILE*) * argc);
	if (fp == TC_NULL) {
		perror("malloc");
		tc_exit(TC_EXIT_FAILURE);
	}

	for (i = 0; i < argc; i++) {
		fp[i] = fopen(argv[i], "w");
		if (fp[i] == TC_NULL) {
			perror("fopen");
			for (i = 0; i < argc; i++) {
				if (fp[i] != TC_NULL) {
					fclose(fp[i]);
				}
			}
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	while ((ch = getc(stdin)) != EOF) {
		for (i = 0; i < argc; i++) {
			rc = putc(ch, fp[i]);
			if (rc == EOF) {
				perror("putc");
				for (i = 0; i < argc; i++) {
					fclose(fp[i]);
				}
				tc_exit(TC_EXIT_FAILURE);
			}
		}
		putc(ch, stdout);
	}

	for (i = 0; i < argc; i++) {
		fclose(fp[i]);
	}
	free(fp);
	tc_exit(TC_EXIT_SUCCESS);
}
