 /*
    seq -- write a sequence of numbers to standard output
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	long long first = 1;
 	long long step = 1;
	long long last = 1;
	long long i;

	/* using getopt is kinda messy when negative numbers are allowed
	 * e.g. seq -10 2 10
	 * use manual argument processing
	 */
	if (argc == 1 || argc > 4) {
		fprintf(stderr, "usage: seq [OPTIONS] LAST\n");
		fprintf(stderr, "usage: seq [OPTIONS] FIRST LAST\n");
		fprintf(stderr, "usage: seq [OPTIONS] FIRST STEP LAST\n");
		tc_exit(TC_EXIT_FAILURE);
	} else if (argc == 2 && strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0) {
		fprintf(stdout, "seq (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
		fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
		fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
		fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
		fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "Written by Thomas Cort.\n");
		tc_exit(TC_EXIT_SUCCESS);
	} else if (argc == 2 && strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		fprintf(stdout, "seq -- write a sequence of numbers to standard output\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "usage: seq [OPTIONS] LAST\n");
		fprintf(stdout, "usage: seq [OPTIONS] FIRST LAST\n");
		fprintf(stdout, "usage: seq [OPTIONS] FIRST STEP LAST\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "  -h, --help     print help text\n");
		fprintf(stdout, "  -V, --version  print version and copyright info\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "examples:\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "  # print the numbers 1 to 10\n");
		fprintf(stdout, "  seq 10\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "  # print the numbers 5 to 55\n");
		fprintf(stdout, "  seq 5 55\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "  # print the numbers 100 to 1000 by 100s\n");
		fprintf(stdout, "  seq 100 100 1000\n");
		tc_exit(TC_EXIT_SUCCESS);
	} else if (argc == 2)  {
		last = strtoll(argv[1], TC_NULL, 10);
	} else if (argc == 3) {
		first = strtoll(argv[1], TC_NULL, 10);
		last = strtoll(argv[2], TC_NULL, 10);
	} else if (argc == 4) {
		first = strtoll(argv[1], TC_NULL, 10);
		step = strtoll(argv[2], TC_NULL, 10);
		last = strtoll(argv[3], TC_NULL, 10);
	}

	/* validate inputs */
	if (first < last && step < 0) {
		fprintf(stderr, "step must be positive when first < last\n");
		tc_exit(TC_EXIT_FAILURE);
	} else if (first > last && step > 0) {
		fprintf(stderr, "step must be negative when first > last\n");
		tc_exit(TC_EXIT_FAILURE);
	} else if (step == 0) {
		fprintf(stderr, "step must be non-zero\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	for (i = first; i <= last; i += step) {
		fprintf(stdout, "%lld\n", i);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
