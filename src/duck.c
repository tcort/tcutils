 /*
    duck -- virtual rubber duck debugging tool
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_BRIGHT_YELLOW  "\x1b[33;1m"

int main(int argc, char *argv[]) {

	int ch;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "duck -- virtual rubber duck debugging tool\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: duck [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # some me the duck\n");
				fprintf(stdout, "  duck\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "duck (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	fprintf(stdout, "%s", COLOR_BRIGHT_YELLOW);
	fprintf(stdout, "\n");
	fprintf(stdout, "                                       dkcudkcud\n");
	fprintf(stdout, "                                    kcu         kcu\n");
	fprintf(stdout, "                                  ud               dk\n");
	fprintf(stdout, "                                 c          duck     c\n");
	fprintf(stdout, "                                k          d    d     u\n");
	fprintf(stdout, "                               d          u  kd  u    duckduckd\n");
	fprintf(stdout, "                               D           c    c     k       u\n");
	fprintf(stdout, "                               u            duck      c      c\n");
	fprintf(stdout, "                                c                     u   uck\n");
	fprintf(stdout, "                                 k                    d    cud\n");
	fprintf(stdout, "          d                       c                  kuduck\n");
	fprintf(stdout, "          ukc                     dk                cud\n");
	fprintf(stdout, "          c  udkcudkcudkcudkcudkcu                     k\n");
	fprintf(stdout, "          k                        ckduck               cu\n");
	fprintf(stdout, "          d      duckduckduckduckdu      d                d\n");
	fprintf(stdout, "          u      d                        u                k\n");
	fprintf(stdout, "          c       k                       c                c\n");
	fprintf(stdout, "           k       c                      k                u\n");
	fprintf(stdout, "            d       u                    d                 d\n");
	fprintf(stdout, "             u       dk                 u                 k\n");
	fprintf(stdout, "             c         cudkcudkcudkcudkc                 c\n");
	fprintf(stdout, "              kdu                                      du\n");
	fprintf(stdout, "                 ckd                                uck\n");
	fprintf(stdout, "                    uckduckduckduckduckduckduckduckd\n");
	fprintf(stdout, "%s", COLOR_RESET);

	(void) fgetc(stdin);

	exit(EXIT_SUCCESS);
}
