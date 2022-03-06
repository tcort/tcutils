 /*
    nl -- number lines of text
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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	size_t lineno;
	FILE *f;
	char *line = NULL;
	size_t cap = 0;
	ssize_t len = 0;
	int bflag;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	bflag = 'a';

	while ((ch = getopt_long(argc, argv, "b:hv", long_options, NULL)) != -1) {
		switch (ch) {
			case 'b':
				bflag = optarg[0];
				if (bflag != 'a' && bflag != 't' && bflag != 'n') {
					fprintf(stderr, "usage: nl [-h] [-V] [-b (a|t|n)] FILE\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 'h':
				fprintf(stdout, "nl -- number lines of text\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: nl [OPTIONS] FILE\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -b opt         control line numbering. opt is one of:\n");
				fprintf(stdout, "                   a - all lines numbered\n");
				fprintf(stdout, "                   t - non-empty lines numbered\n");
				fprintf(stdout, "                   n - no lines numbered\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # number the lines in file foo.c\n");
				fprintf(stdout, "  nl foo.c\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # number the non-empty lines in file foo.c\n");
				fprintf(stdout, "  nl -b t foo.c\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "nl (tcutils) v1.0.0\n");
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

	if (argc != 1) {
		fprintf(stderr, "usage: nl [OPTIONS] FILE\n");
		exit(EXIT_FAILURE);
	}

	f = fopen(argv[0], "r");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	for (lineno = 1; !feof(f) && !ferror(f); lineno++) {
		len = getline(&line, &cap, f);
		if (len <= 0) {
			break;
		}

		switch (bflag) {
			case 'a':
				fprintf(stdout, "%6ld\t%s", lineno, line);
				break;
			case 't':
				if (line[0] == '\n') {
					fprintf(stdout, "%s", line);
					lineno--;
				} else {
					fprintf(stdout, "%6ld\t%s", lineno, line);
				}
				break;
			default: /* 'n' */
				fprintf(stdout, "%s", line);
				break;
		}
	}

	free(line);
	fclose(f);

	exit(EXIT_SUCCESS);
}
