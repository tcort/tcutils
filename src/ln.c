 /*
    ln -- creates a file link
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

int main(int argc, char *argv[]) {

	int rc, flag_s = 0;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 's', .longarg = "symlink", .description = "creates a symbolic link", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "ln foo.txt bar.txt", .description = "create a link name bar.txt which links to existing file foo.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "ln",
		.usage = "[OPTIONS] FILE1 FILE2",
		.description = "creates a file link",
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
			case 's':
				flag_s = 1;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc != 2) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	if (flag_s == 1) {
		rc = tc_symlink(argv[0], argv[1]);
		if (rc == -1) {
			tc_puterrln("Could not create symbolic link");
			tc_exit(TC_EXIT_FAILURE);
		}
	} else {
		rc = tc_link(argv[0], argv[1]);
		if (rc == -1) {
			tc_puterrln("Could not create link");
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}
