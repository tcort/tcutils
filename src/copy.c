 /*
    copy -- copy characters from standard input to standard output
    Copyright (C) 2022, 2023  Thomas Cort

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

int main(int argc, char *argv[]) {

	int ch;
	int n;
	struct tc_prog_arg *arg;
	int flag_n;
	int flag_s;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'n', .longarg = "count", .description = "number of blocks to read", .has_value = 1 },
		{ .arg = 's', .longarg = "size", .description = "block size (in bytes)", .has_value = 1 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "copy < ./foo > ./bar", .description = "file copy - creates a copy of ./foo named ./bar" },
		{ .command = "copy -n 5 -s 1024 < /dev/random > ./foo", .description = "copy 5 KB from /dev/random into ./foo" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "copy",
		.usage = "[OPTIONS]",
		.description = "copy characters from standard input to standard output",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_n = -1; /* copy until EOF */
	flag_s = 1; /* 1 byte */

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'n':
				flag_n = tc_atoi(argval);
				flag_n = flag_n < 0 ? -1 : flag_n;
				break;
			case 's':
				flag_s = tc_atoi(argval);
				flag_s = flag_s < 0 ? 1 : flag_s;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (flag_n == -1) {
		tc_copylns(TC_STDIN, TC_STDOUT, -1);
		tc_exit(TC_EXIT_SUCCESS);
	} else {
		int rc;
		rc = TC_OK;
		for (n = 0; n < flag_n && rc == TC_OK; n++) {
			rc = tc_copynbytes(TC_STDIN, TC_STDOUT, flag_s);
		}
		tc_exit(rc == TC_OK ? TC_EXIT_SUCCESS : TC_EXIT_FAILURE);
	}
}
