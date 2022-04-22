 /*
    witch -- finds an executable in $PATH
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
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	char *path;
	char *dir;
	char *prog;
	char progpath[PATH_MAX+1];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "witch -- finds an executable in $PATH\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: witch [OPTIONS] PROGNAME\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # find the C compiler\n");
				fprintf(stdout, "  witch cc\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "witch (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	if (argc != 1) {
		fprintf(stderr, "usage: witch [OPTIONS] PROGNAME\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	prog = argv[0];

	path = getenv("PATH");
	if (path == TC_NULL) {
		fprintf(stderr, "$PATH not set in environment\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	dir = strtok(path, TC_PATHSEP);
	while (dir != TC_NULL) {
		int rc;

		snprintf(progpath, PATH_MAX, "%s/%s", dir, prog);

		rc = access(progpath, X_OK);
		if (rc == 0) {
			if (tc_isatty(TC_STDOUT)) {
				fprintf(stdout, "%s", COLOUR_BRIGHT_GREEN);
			}
			fprintf(stdout, "%s", progpath);
			if (tc_isatty(TC_STDOUT)) {
				fprintf(stdout, "%s", COLOUR_RESET);
			}
			fprintf(stdout, "\n");
			tc_exit(TC_EXIT_SUCCESS);
		}

		dir = strtok(TC_NULL, TC_PATHSEP);
	}

	/* not found :( */
	tc_exit(TC_EXIT_FAILURE);
}
