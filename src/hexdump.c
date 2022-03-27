 /*
    hexdump -- prints the contents of a file in hexadecimal
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
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	FILE *f;
	char bytes[16];
	int i, j;
	size_t n, addr;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
    				fprintf(stdout, "hexdump -- prints the contents of a file in hexadecimal\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: hexdump [OPTIONS] FILE\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show file contents as hex values\n");
				fprintf(stdout, "  hexdump foo.txt\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "hexdump (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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
		fprintf(stderr, "usage: hexdump [OPTIONS] FILE\n");
		exit(EXIT_FAILURE);
	}

	f = fopen(argv[0], "rb");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	addr = 0;
	do {
		memset(bytes, '\0', sizeof(char) * 16);
		n = fread(bytes, sizeof(char), 16, f);
		if (n == 0) {
			break;
		}
		printf("%.8lx  ", addr);
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 8; j++) {
				printf("%.2x", 0xff & bytes[(i*8)+j]);
				printf(" ");
			}
			printf(" ");
		}
		printf("|");
		for (i = 0; i < 16; i++) {
			printf("%c", isprint(bytes[i]) ? bytes[i] : '.');
		}
		printf("|");
		printf("\n");
		addr+=n;
	} while (1);

	fclose(f);

	exit(EXIT_SUCCESS);
}
