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

#include "config.h"

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COLOR_RESET   "\x1b[0m"

#define COLOR_BLACK   "\x1b[30m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

#define COLOR_BRIGHT_BLACK   "\x1b[30;1m"
#define COLOR_BRIGHT_RED     "\x1b[31;1m"
#define COLOR_BRIGHT_GREEN   "\x1b[32;1m"
#define COLOR_BRIGHT_YELLOW  "\x1b[33;1m"
#define COLOR_BRIGHT_BLUE    "\x1b[34;1m"
#define COLOR_BRIGHT_MAGENTA "\x1b[35;1m"
#define COLOR_BRIGHT_CYAN    "\x1b[36;1m"
#define COLOR_BRIGHT_WHITE   "\x1b[37;1m"

char *dull_pattern[] = {
	COLOR_MAGENTA, COLOR_RED,  COLOR_YELLOW,
	COLOR_GREEN,   COLOR_BLUE, COLOR_CYAN,
	COLOR_WHITE
};
const int ndull_pattern = sizeof(dull_pattern)/sizeof(dull_pattern[0]);

char *bright_pattern[] = {
	COLOR_BRIGHT_MAGENTA, COLOR_BRIGHT_RED,  COLOR_BRIGHT_YELLOW,
	COLOR_BRIGHT_GREEN,   COLOR_BRIGHT_BLUE, COLOR_BRIGHT_CYAN,
	COLOR_BRIGHT_WHITE
};
const int nbright_pattern = sizeof(bright_pattern)/sizeof(bright_pattern[0]);

char *usa_pattern[] = {
	COLOR_BRIGHT_RED, COLOR_BRIGHT_WHITE, COLOR_BRIGHT_BLUE
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

	colourize = isatty(STDOUT_FILENO);

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
			fprintf(output, "%s", COLOR_RESET);
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

	while ((ch = getopt_long(argc, argv, "bdhuV", long_options, NULL)) != -1) {
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
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "lolcat (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	if (argc == 0) {
		copy(stdin, stdout, pat);
		exit(EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		if (f == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}

		copy(f, stdout, pat);

		fclose(f);
	}

	exit(EXIT_SUCCESS);
}
