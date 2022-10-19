 /*
    cmp -- compare two files
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	long byte;
	long line;
	FILE *f1;
	FILE *f2;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "cmp ./foo ./bar", .description = "compare two files ./foo and ./bar" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "cmp",
		.usage = "[OPTIONS] FILE1 FILE2",
		.description = "compare two files",
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

	if (argc != 2) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	f1 = fopen(argv[0], "r");
	if (f1 == TC_NULL) {
		perror("fopen");
		tc_exit(TC_EXIT_FAILURE);
	}
	f2 = fopen(argv[1], "r");
	if (f2 == TC_NULL) {
		fclose(f1);
		perror("fopen");
		tc_exit(TC_EXIT_FAILURE);
	}

	byte = line = 1;
	do {
		int c1;
		int c2;

		c1 = getc(f1);
		c2 = getc(f2);

		if (c1 != c2) {
			fprintf(stdout, "%s %s differ: byte %ld, line %ld\n", argv[0], argv[1], byte, line);
			tc_exit(TC_EXIT_FAILURE);
		}

		if (c1 == '\n') {
			line++;
		}
		byte++;

	} while (!feof(f1) && !feof(f2) && !ferror(f1) && !feof(f2));

	fclose(f1);
	fclose(f2);

	tc_exit(TC_EXIT_SUCCESS);
}
