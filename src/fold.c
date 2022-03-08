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

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_WIDTH (80)
#define TAB_WIDTH (8)

#define TAB_CHAR ('\t')
#define BACKSPACE_CHAR ('\b')
#define NEWLINE_CHAR ('\n')

int main(int argc, char *argv[]) {

	int ch = '\0';
	int width = DEFAULT_WIDTH;
	int bflag = 0;
	FILE *fp = NULL;
	size_t cap = 0;
	ssize_t len = 0, i = 0;
	char *line = NULL;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "bytes", no_argument, 0, 'b' },
		{ "width", required_argument, 0, 'w' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "bhVw:", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "fold -- wraps long lines for fixed width viewing mediums\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: fold [OPTIONS] [FILE]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -b, --bytes    count bytes rather than columns\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "  -w, --width    width of the output (default 80)\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # wrap contents of foo.txt at 72 characters\n");
				fprintf(stdout, "  fold -w 72 foo.txt\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "fold (tcutils) v1.0.0\n");
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
				break;
			case 'w':
				width = atoi(optarg);
				if (width <= 0) {
					errno = EINVAL;
					perror("width");
					exit(EXIT_FAILURE);
				}
				break;
			case 'b':
				bflag = 1;
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	fp = argc == 0 ? stdin : fopen(argv[0], "r");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	while ((len = getline(&line, &cap, fp)) > 0) {
		printf("%s", line);
	}

	free(line);
	fclose(fp);
	exit(EXIT_SUCCESS);
}
