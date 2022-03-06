 /*
    luhn -- luhn checker
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

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int doubles[] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};

static int ctoi(int ch) {
	switch (ch) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		default:  return 0;
	}
}

static int luhn(char *s) {

	int i;
	int dub;
	int check_digit;
	int computed_check_digit;

	/* must be at least 2 digits (1 digit + check digit) */
	if (strlen(s) < 2) {
		return -1;
	}

	/* all characters in string must be digits */
	for (i = 0; i < strlen(s); i++) {
		if (!isdigit(s[i])) {
			return -1;
		}
	}

	dub = 1;
	check_digit = ctoi(s[strlen(s) - 1]);
	computed_check_digit = 0;
	for (i = strlen(s) - 2; i >= 0; i--) {
		computed_check_digit += dub ? doubles[ctoi(s[i])] : ctoi(s[i]);
		dub = !dub;
	}

	computed_check_digit = (10 - (computed_check_digit % 10)) % 10;

	return computed_check_digit == check_digit ? 0 : -1;
}

int main(int argc, char *argv[]) {

	int ch;
	int i;
	int rc;
	int failure;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "luhn -- luhn checker\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: luhn [OPTIONS] [DIGITS...]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # check the luhn of a string of digits\n");
				fprintf(stdout, "  luhn 4030000010001234\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # check several at once\n");
				fprintf(stdout, "  luhn 4030000010001234 4003050500040005 5100000010001004\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "true (tcutils) v1.0.0\n");
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

	failure = 0;
	for (i = 0; i < argc; i++) {
		rc = luhn(argv[i]);
		if (rc == 0) {
			fprintf(stdout, "%s\tOK\n", argv[i]);
		} else {
			fprintf(stdout, "%s\tBAD\n", argv[i]);
			failure = 1;

		}
	}

	exit(failure == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
