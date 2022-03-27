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

#include "config.h"

#include <getopt.h>
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

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "cmp -- compare two files\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: cmp [OPTIONS] FILE1 FILE2\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # compare two files ./foo and ./bar\n");
				fprintf(stdout, "  cmp ./foo ./bar\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "cmp (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	if (argc != 2) {
		fprintf(stderr, "usage: cmp [OPTIONS] FILE1 FILE2\n");
		exit(EXIT_FAILURE);
	}

	f1 = fopen(argv[0], "r");
	if (f1 == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	f2 = fopen(argv[1], "r");
	if (f2 == NULL) {
		fclose(f1);
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	byte = line = 1;
	do {
		int c1;
		int c2;

		c1 = getc(f1);
		c2 = getc(f2);

		if (c1 != c2) {
			fprintf(stdout, "%s %s differ: byte %ld, line %ld\n", argv[0], argv[1], byte, line);
			exit(EXIT_FAILURE);
		}

		if (c1 == '\n') {
			line++;
		}
		byte++;

	} while (!feof(f1) && !feof(f2) && !ferror(f1) && !feof(f2));

	fclose(f1);
	fclose(f2);

	exit(EXIT_SUCCESS);
}
