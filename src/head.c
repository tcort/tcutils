 /*
    head -- prints lines at the beginning of an input text
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void copy_line(FILE *src, FILE *dst) {
	int ch;
	while ((ch = getc(src)) != EOF) {
		putc(ch, dst);
		if (ch == '\n') {
			break;
		}
	}
}

static void copy_lines(FILE *src, FILE *dst, int n) {
	int i;
	for (i = 0; i < n; i++) {
		copy_line(src, dst);
	}
}

int main(int argc, char *argv[]) {

	int ch;
	int i;
	int flag_n;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "lines", required_argument, 0, 'n' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	flag_n = 10;

	while ((ch = getopt_long(argc, argv, "hn:V", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "head -- prints lines at the beginning of an input text\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: head [OPTIONS] [FILE]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help                   print help text\n");
				fprintf(stdout, "  -n INTEGER, --lines=INTEGER  number of lines to show (default 10)\n");
				fprintf(stdout, "  -V, --version                print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the first 10 lines of foo.txt\n");
				fprintf(stdout, "  head foo.txt\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the first 2 lines of bar.txt\n");
				fprintf(stdout, "  head -n 2 bar.txt\n");
				fprintf(stdout, "\n");
				exit(EXIT_SUCCESS);
				break;
			case 'n':
				flag_n = atoi(optarg);
				flag_n = flag_n < 0 ? 10 : flag_n;
				break;
			case 'V':
				fprintf(stdout, "head (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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
		copy_lines(stdin, stdout, flag_n);
		exit(EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		if (f == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}

		copy_lines(f, stdout, flag_n);

		fclose(f);
	}

	exit(EXIT_SUCCESS);
}
