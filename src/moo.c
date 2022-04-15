 /*
    moo -- guess the bulls
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
#include "tc/stdlib.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	int b,c,i,j;
	char line[LINE_MAX], x, farm[10];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "moo -- guess the bulls\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: moo [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # play the game\n");
				fprintf(stdout, "  moo\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "moo (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	tc_srand((unsigned)getpid());

	for (i = 0; i < 10; i++) {
		farm[i] = '0'+(char)i;
	}

	for (i = 9; i >= 1; i--) {
		b = tc_rand() % (i+1);
		if (b == 0 && farm[i] == '0') {
			i++;
			continue;
		}
		x = farm[b];
		farm[b] = farm[i];
		farm[i] = x;
	}

	do {
		fputs("your guess?\n", stdout);
		fgets(line, LINE_MAX, stdin);
		for (b=c=i=0; i < 4; i++)
			if (line[i] == farm[i]) b++;
			else 
				for (j = 0; j < 4; j++)
					if (line[i] == farm[j]) c++;
		printf("bulls = %d  cows = %d\n", b, c);
		if (b== 4) break;
	} while (!feof(stdin) && !ferror(stdin));

	tc_exit(TC_EXIT_SUCCESS);
}
