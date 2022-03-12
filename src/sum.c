 /*
    sum -- compute a BSD checksum of the input
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sum(FILE *fp, char *filename) {

	unsigned int r;
	int ch;

	r = 0;

	while ((ch = getc(fp)) != EOF) {
		r = ((r >> 1) + ((r & 1) << 15) + ch) & 0xffff;
	}

	fprintf(stdout, "%u\t%s\n", r, filename);
}

int main(int argc, char *argv[]) {

	int ch;
	int i;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "sum -- compute a BSD checksum of the input\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: sum [OPTIONS] [FILE...]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # compute the checksum of foo.txt\n");
				fprintf(stdout, "  sum foo.txt\n");
				fprintf(stdout, "\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "sum (tcutils) v1.0.0\n");
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
		sum(stdin, "<stdin>");
		exit(EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		if (f == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}

		sum(f, argv[i]);

		fclose(f);
	}

	exit(EXIT_SUCCESS);
}
