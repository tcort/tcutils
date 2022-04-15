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

#include "tc/const.h"
#include "tc/colours.h"
#include "tc/stdio.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
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
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "duck (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	tc_puts(TC_STDOUT, COLOUR_BRIGHT_YELLOW);
	tc_puts(TC_STDOUT, "\n");
	tc_puts(TC_STDOUT, "                                       dkcudkcud\n");
	tc_puts(TC_STDOUT, "                                    kcu         kcu\n");
	tc_puts(TC_STDOUT, "                                  ud               dk\n");
	tc_puts(TC_STDOUT, "                                 c          duck     c\n");
	tc_puts(TC_STDOUT, "                                k          d    d     u\n");
	tc_puts(TC_STDOUT, "                               d          u  kd  u    duckduckd\n");
	tc_puts(TC_STDOUT, "                               D           c    c     k       u\n");
	tc_puts(TC_STDOUT, "                               u            duck      c      c\n");
	tc_puts(TC_STDOUT, "                                c                     u   uck\n");
	tc_puts(TC_STDOUT, "                                 k                    d    cud\n");
	tc_puts(TC_STDOUT, "          d                       c                  kuduck\n");
	tc_puts(TC_STDOUT, "          ukc                     dk                cud\n");
	tc_puts(TC_STDOUT, "          c  udkcudkcudkcudkcudkcu                     k\n");
	tc_puts(TC_STDOUT, "          k                        ckduck               cu\n");
	tc_puts(TC_STDOUT, "          d      duckduckduckduckdu      d                d\n");
	tc_puts(TC_STDOUT, "          u      d                        u                k\n");
	tc_puts(TC_STDOUT, "          c       k                       c                c\n");
	tc_puts(TC_STDOUT, "           k       c                      k                u\n");
	tc_puts(TC_STDOUT, "            d       u                    d                 d\n");
	tc_puts(TC_STDOUT, "             u       dk                 u                 k\n");
	tc_puts(TC_STDOUT, "             c         cudkcudkcudkcudkc                 c\n");
	tc_puts(TC_STDOUT, "              kdu                                      du\n");
	tc_puts(TC_STDOUT, "                 ckd                                uck\n");
	tc_puts(TC_STDOUT, "                    uckduckduckduckduckduckduckduckd\n");
	tc_puts(TC_STDOUT, COLOUR_RESET);

	(void) fgetc(stdin);

	tc_exit(TC_EXIT_SUCCESS);
}
