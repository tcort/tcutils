 /*
    tail -- prints lines at the end of an input text
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

struct line {
	char *text;
	size_t cap;
	ssize_t len;
};

static void tail(FILE *fin, FILE *fout, int n) {

	int i, j;
	struct line **buf;

	buf = (struct line **) malloc(sizeof(struct line*) * n);
	if (buf == TC_NULL) {
		perror("malloc");
		tc_exit(TC_EXIT_FAILURE);
	}
	memset(buf, '\0', sizeof(struct line*) * n);

	for (i = 0; i < n; i++) {
		buf[i] = (struct line *) malloc(sizeof(struct line));
		if (buf[i] == TC_NULL) {
			perror("malloc");
			for (j = 0; j < i; j++) {
				free(buf[j]->text);
				buf[j]->text = TC_NULL;
				free(buf[j]);
				buf[j] = TC_NULL;
			}
			free(buf);
			tc_exit(TC_EXIT_FAILURE);
		}
		memset(buf[i], '\0', sizeof(struct line));
	}

	i = 0;
	while ( (buf[i]->len = getline(&buf[i]->text, &buf[i]->cap, fin)) >= 0) {
		i = (i + 1) % n;
	}

	for (j = 0; j < n; j++) {
		fprintf(fout, "%s", buf[(i + j) % n]->text);
	}

	for (i = 0; i < n; i++) {
		free(buf[i]->text);
		buf[i]->text = TC_NULL;
		free(buf[i]);
		buf[i] = TC_NULL;
	}
	free(buf);
	buf = TC_NULL;

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

	while ((ch = getopt_long(argc, argv, "hn:V", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "tail -- prints lines at the end of an input text\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: tail [OPTIONS] [FILE]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help                   print help text\n");
				fprintf(stdout, "  -n INTEGER, --lines=INTEGER  number of lines to show (default 10)\n");
				fprintf(stdout, "  -V, --version                print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the last 10 lines of foo.txt\n");
				fprintf(stdout, "  tail foo.txt\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the last 2 lines of bar.txt\n");
				fprintf(stdout, "  tail -n 2 bar.txt\n");
				fprintf(stdout, "\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'n':
				flag_n = atoi(optarg);
				flag_n = flag_n < 0 ? 10 : flag_n;
				break;
			case 'V':
				fprintf(stdout, "tail (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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
		tail(stdin, stdout, flag_n);
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		if (f == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}

		tail(f, stdout, flag_n);

		fclose(f);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
