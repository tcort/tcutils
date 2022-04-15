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
 */

#include "tc/const.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void fgrep(FILE *in, char *pattern, char *filename, int flag_H) {

	char *p;
	char *line = TC_NULL;
	size_t cap = 0;
	ssize_t len = 0;

	while ((len = getline(&line, &cap, in)) != EOF) {
		p = strstr(line, pattern);
		if (p != TC_NULL) {
			if (flag_H == 1) {
				fprintf(stdout, "%s:\t", filename);
			}
			fprintf(stdout, "%s", line);
		}
	}

	free(line);
}

int main(int argc, char *argv[]) {

	int ch;
	int i;
	FILE *in;
	char *pattern;
	int flag_H;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "filenames", no_argument, 0, 'H' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */

	flag_H = 0;

	while ((ch = getopt_long(argc, argv, "hHV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "fgrep -- searches for and prints lines that exactly match a given string\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: fgrep [OPTIONS] PATTERN [FILE...]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help       print help text\n");
				fprintf(stdout, "  -H, --filenames  print filenames with output lines\n");
				fprintf(stdout, "  -V, --version    print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # search the files foo.txt and bar.txt for the fixed string hello\n");
				fprintf(stdout, "  fgrep hello foo.txt bar.txt\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "fgrep (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'H':
				flag_H = 1;
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				tc_exit(TC_EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	if (argc < 1) {
		fprintf(stderr, "usage: fgrep [OPTIONS] PATTERN [FILE...]\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	pattern = argv[0];

	if (argc == 1) {
		fgrep(stdin, pattern, "<stdin>", flag_H);
	} else {
		for (i = 1; i < argc; i++) {
			in = fopen(argv[i], "r");
			if (in == TC_NULL) {
				perror("fopen");
				tc_exit(TC_EXIT_FAILURE);
			}

			fgrep(in, pattern, argv[i], flag_H);

			fclose(in);

		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}

