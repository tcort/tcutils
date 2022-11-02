 /*
    lolcat -- concatenate and print files with funky colours
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

char *dull_pattern[] = {
	COLOUR_MAGENTA, COLOUR_RED,  COLOUR_YELLOW,
	COLOUR_GREEN,   COLOUR_BLUE, COLOUR_CYAN,
	COLOUR_WHITE
};
const int ndull_pattern = sizeof(dull_pattern)/sizeof(dull_pattern[0]);

char *bright_pattern[] = {
	COLOUR_BRIGHT_MAGENTA, COLOUR_BRIGHT_RED,  COLOUR_BRIGHT_YELLOW,
	COLOUR_BRIGHT_GREEN,   COLOUR_BRIGHT_BLUE, COLOUR_BRIGHT_CYAN,
	COLOUR_BRIGHT_WHITE
};
const int nbright_pattern = sizeof(bright_pattern)/sizeof(bright_pattern[0]);

char *usa_pattern[] = {
	COLOUR_BRIGHT_RED, COLOUR_BRIGHT_WHITE, COLOUR_BRIGHT_BLUE
};
const int nusa_pattern = sizeof(usa_pattern)/sizeof(usa_pattern[0]);

enum pattern {
	BRIGHT_PATTERN,
	DULL_PATTERN,
	USA_PATTERN
};

void copy(FILE *input, FILE *output, enum pattern pat) {

	int ch;
	int i;
	int colourize;

	colourize = tc_isatty(TC_STDOUT);

	for (i = 0; (ch = getc(input)) != EOF; i++) {
		if (colourize) {
			switch (pat) {
				case USA_PATTERN:
					fprintf(output, "%s", usa_pattern[i % nusa_pattern]);
					break;
				case DULL_PATTERN:
					fprintf(output, "%s", dull_pattern[i % ndull_pattern]);
					break;
				case BRIGHT_PATTERN:
				default:
					fprintf(output, "%s", bright_pattern[i % nbright_pattern]);
					break;
			}
		}
		putc(ch, output);
		if (colourize) {
			fprintf(output, "%s", COLOUR_RESET);
		}
	}
}

int main(int argc, char *argv[]) {

	int bright;
	int ch;
	int i;
	enum pattern pat;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'b', .longarg = "bright", .description = "bright colours (default)", .has_value = 0 },
		{ .arg = 'd', .longarg = "dull", .description = "dull colours", .has_value = 0 },
		TC_PROG_ARG_HELP,
		{ .arg = 'u', .longarg = "usa", .description = "USA colours", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "lolcat foo.txt bar.txt", .description = "concatinate two files" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "lolcat",
		.usage = "[OPTIONS] [FILES...]",
		.description = "concatenate and print files with funky colours",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	bright = 1;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'b':
				pat = BRIGHT_PATTERN;
				break;
			case 'd':
				pat = DULL_PATTERN;
				break;
			case 'u':
				pat = USA_PATTERN;
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

	if (argc == 0) {
		copy(stdin, stdout, pat);
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		if (f == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}

		copy(f, stdout, pat);

		fclose(f);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
