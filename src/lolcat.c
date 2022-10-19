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
 */

#include <tc/tc.h>

#include <getopt.h>
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

	static struct option long_options[] = {
		{ "bright", no_argument, 0, 'b' },
		{ "dull", no_argument, 0, 'd' },
		{ "usa", no_argument, 0, 'u' },
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	bright = 1;

	while ((ch = getopt_long(argc, argv, "bdhuV", long_options, TC_NULL)) != -1) {
		switch (ch) {
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
				fprintf(stdout, "lolcat -- concatenate and print files with funky colours\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: lolcat [OPTIONS] [FILES...]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -b, --bright   bright colours (default)\n");
				fprintf(stdout, "  -d, --dull     dull colours\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -u, --usa      USA colours\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # concatinate two files\n");
				fprintf(stdout, "  lolcat foo.txt bar.txt\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "lolcat (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				tc_exit(TC_EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

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
