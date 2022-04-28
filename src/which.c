 /*
    which -- finds an executable in $PATH
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

#include "tc/args.h"
#include "tc/const.h"
#include "tc/colours.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	char *path;
	char *dir;
	char *progname;
	char progpath[PATH_MAX+1];

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "which cc", .description = "find the C compiler" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "which",
		.usage = "[OPTIONS] PROGNAME",
		.description = "finds an executable in $PATH",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc != 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	progname = argv[0];

	path = getenv("PATH");
	if (path == TC_NULL) {
		fprintf(stderr, "$PATH not set in environment\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	dir = strtok(path, TC_PATHSEP);
	while (dir != TC_NULL) {
		int rc;

		snprintf(progpath, PATH_MAX, "%s/%s", dir, progname);

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
