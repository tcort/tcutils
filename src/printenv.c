 /*
    printenv -- print environment variables and their values
    Copyright (C) 2022, 2023, 2024  Thomas Cort

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

    SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <tc/tc.h>

extern char **environ;

int main(int argc, char *argv[]) {

	int i;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "printenv", .description = "show all environment variables" },
		{ .command = "printenv HOME", .description = "show just one environment variable's value" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "printenv",
		.usage = "[OPTIONS] [VARNAME]",
		.description = "print environment variables and their values",
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

	if (argc < 1) {
		for (i = 0; environ[i] != TC_NULL; i++) {
			tc_putln(TC_STDOUT, environ[i]);
		}
	} else {
		for (i = 0; environ[i] != TC_NULL; i++) {
			if (tc_strneql(environ[i], argv[0], tc_strlen(argv[0])) == 1) {
				char c;
				c = *(environ[i] + tc_strlen(argv[0]));
				if (c == '=') { 
					tc_putln(TC_STDOUT, environ[i] + tc_strlen(argv[0]) + 1);
				} else if (c == '\0') {
					tc_putln(TC_STDOUT, environ[i] + tc_strlen(argv[0]));
				}
			}
		}
	}	

	tc_exit(TC_EXIT_SUCCESS);
}
