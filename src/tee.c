 /*
    tee -- copy characters from standard input to standard output and a file
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
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch, i, rc;
	FILE **fp;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "tee -- copy characters from standard input to standard output and a file\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: tee [OPTIONS] FILE...\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # write the results of ps to foo.txt and standard output\n");
				fprintf(stdout, "  ps | tee foo.txt\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "tee (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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
		fprintf(stdout, "usage: tee [OPTIONS] FILE...\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	fp = (FILE **) malloc(sizeof(FILE*) * argc);
	if (fp == TC_NULL) {
		perror("malloc");
		tc_exit(TC_EXIT_FAILURE);
	}

	for (i = 0; i < argc; i++) {
		fp[i] = fopen(argv[i], "w");
		if (fp[i] == TC_NULL) {
			perror("fopen");
			for (i = 0; i < argc; i++) {
				if (fp[i] != TC_NULL) {
					fclose(fp[i]);
				}
			}
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	while ((ch = getc(stdin)) != EOF) {
		for (i = 0; i < argc; i++) {
			rc = putc(ch, fp[i]);
			if (rc == EOF) {
				perror("putc");
				for (i = 0; i < argc; i++) {
					fclose(fp[i]);
				}
				tc_exit(TC_EXIT_FAILURE);
			}
		}
		putc(ch, stdout);
	}

	for (i = 0; i < argc; i++) {
		fclose(fp[i]);
	}
	free(fp);
	tc_exit(TC_EXIT_SUCCESS);
}
