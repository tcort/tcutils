 /*
    fold -- wraps long lines for fixed width viewing mediums
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

#define DEFAULT_WIDTH (80)
#define TAB_WIDTH (8)

#define TAB_CHAR ('\t')
#define BACKSPACE_CHAR ('\b')
#define NEWLINE_CHAR ('\n')
#define CARRAIGE_RETURN_CHAR ('\r')

int main(int argc, char *argv[]) {

	int ch = '\0';
	int width = DEFAULT_WIDTH;
	int bflag = 0;
	FILE *fp = TC_NULL;
	size_t cap = 0;
	ssize_t len = 0, i = 0, col = 0;
	char *line = TC_NULL;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'b', .longarg = "bytes", .description = "count bytes rather than columns", .has_value = 0 },
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		{ .arg = 'w', .longarg = "width", .description = "width of the output (default 80)", .has_value = 1 },
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "fold -w 72 foo.txt", .description = "wrap contents of foo.txt at 72 characters" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "fold",
		.usage = "[OPTIONS] [FILE]",
		.description = "wraps long lines for fixed width viewing mediums",
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
			case 'b':
				bflag = 1;
				break;
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
			case 'w':
				width = atoi(argval);
				if (width <= 0) {
					errno = EINVAL;
					perror("width");
					tc_exit(TC_EXIT_FAILURE);
				}
				break;
		}

	}

	argc -= argi;
	argv += argi;

	fp = argc == 0 ? stdin : fopen(argv[0], "r");
	if (fp == TC_NULL) {
		perror("fopen");
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((len = getline(&line, &cap, fp)) >= 0) {

		col = 0;
		for (i = 0; i < len; i++) {
			int ch = line[i];

			switch (ch) {
				case TAB_CHAR:
					col = col + (TAB_WIDTH - (col % TAB_WIDTH));
					break;
				case BACKSPACE_CHAR:
					col = col == 0 ? 0 : col - 1;
					break;
				case NEWLINE_CHAR:
					col = 0;
					break;
				case CARRAIGE_RETURN_CHAR:
					col = 0;
					break;
				default:
					col += 1;
					break;
			}

			if (col > width) {
				fputc('\n', stdout);
				col = 1;
				if (line[i] == TAB_CHAR) {
					col = TAB_WIDTH;
				}
			}

			fputc(line[i], stdout);
		}
	}

	free(line);
	fclose(fp);
	tc_exit(TC_EXIT_SUCCESS);
}
