 /*
    date -- print the current date and time
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
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FMT_HUMAN "%Y-%m-%d %H:%M:%S"
#define FMT_ISO8601 "%FT%T%z"
#define FMT_FILE "%Y%m%d%H%M%S"
#define FMT_UNIX "%s"

int main(int argc, char *argv[]) {

	int ch;
	time_t now;
	struct tm *local;
	char tstring[32];
	char *fmt;

	static struct option long_options[] = {
		{ "filename-safe", no_argument, 0, 'f' },
		{ "iso8601", no_argument, 0, 'i' },
		{ "help", no_argument, 0, 'h' },
		{ "unix", no_argument, 0, 'u' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	fmt = FMT_HUMAN;

	while ((ch = getopt_long(argc, argv, "fhiuV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'f':
				fmt = FMT_FILE;
				break;
			case 'h':
				fprintf(stdout, "date -- print the current date and time\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: date [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -f, --filename-safe  filename safe format (YYYYmmddHHMMSS)\n");
				fprintf(stdout, "  -h, --help           print help text\n");
				fprintf(stdout, "  -i, --iso8601        ISO8601 format\n");
				fprintf(stdout, "  -u, --unix           UNIX timestamp\n");
				fprintf(stdout, "  -V, --version        print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the date and time in human readable format\n");
				fprintf(stdout, "  date\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the date and time in ISO8601\n");
				fprintf(stdout, "  date -i\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the date and time in filename safe format\n");
				fprintf(stdout, "  tar -cf backup-$(date -f).tar foo.c bar.c\n");
				exit(EXIT_SUCCESS);
				break;
			case 'i':
				fmt = FMT_ISO8601;
				break;
			case 'u':
				fmt = FMT_UNIX;
				break;
			case 'V':
				fprintf(stdout, "date (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	now = time(NULL);
	local = localtime(&now);
	memset(tstring, '\0', sizeof(tstring));
	strftime(tstring, sizeof(tstring)-1, fmt, local);
	fprintf(stdout, "%s\n", tstring);

	exit(EXIT_SUCCESS);
}
