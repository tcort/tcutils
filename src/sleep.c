 /*
    sleep -- sleep for a given number of seconds
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
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

int main(int argc, char *argv[]) {

	int seconds;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "sleep 1", .description = "sleep for one second" },
		{ .command = "sleep 4", .description = "sleep for four seconds" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "sleep",
		.usage = "[OPTIONS] SECONDS",
		.description = "sleep for a given number of seconds",
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

	seconds = tc_atoi(argv[0]);
	if (seconds < 0) {
		seconds = 0;
	}

	/* fall through to exit if interrupted by a signal */
	(void) tc_sleep(seconds);

	tc_exit(TC_EXIT_SUCCESS);
}
