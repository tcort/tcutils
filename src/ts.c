 /*
    ts -- copy lines from standard input to standard output adding timestamps
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
#include <time.h>
#include <unistd.h>

#define FMT_HUMAN "%Y-%m-%d %H:%M:%S"
#define FMT_ISO8601 "%FT%T%z"
#define FMT_COMPACT "%Y%m%d%H%M%S"
#define FMT_UNIX "%s"

static void ts(char *fmt) {
	time_t now;
	struct tm *local;
	char tstring[32];

	now = time(TC_NULL);
	local = localtime(&now);
	memset(tstring, '\0', sizeof(tstring));
	strftime(tstring, sizeof(tstring)-1, fmt, local);
	fprintf(stdout, "[%s] ", tstring);
}

int main(int argc, char *argv[]) {

	int ch, pch;
	char *fmt;

	static struct option long_options[] = {
		{ "compact", no_argument, 0, 'c' },
		{ "help", no_argument, 0, 'h' },
		{ "iso8601", no_argument, 0, 'i' },
		{ "unix", no_argument, 0, 'u' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	fmt = FMT_HUMAN;

	while ((ch = getopt_long(argc, argv, "chiuV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'c':
				fmt = FMT_COMPACT;
				break;
			case 'h':
				fprintf(stdout, "ts -- copy lines from standard input to standard output adding timestamps\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: ts [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -c, --compact  filename safe format (YYYYmmddHHMMSS)\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -i, --iso8601  ISO8601 format\n");
				fprintf(stdout, "  -u, --unix     UNIX timestamp\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # add timestamps to lines from standard input\n");
				fprintf(stdout, "  food | ts > foo.log\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'i':
				fmt = FMT_ISO8601;
				break;
			case 'u':
				fmt = FMT_UNIX;
				break;
			case 'V':
				fprintf(stdout, "ts (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	ts(fmt);
	pch = 0;
	while ((ch = getc(stdin)) != EOF) {
		if (pch == '\n') {
			ts(fmt);
		}
		putc(ch, stdout);
		pch = ch;
	}

	tc_exit(TC_EXIT_SUCCESS);
}
