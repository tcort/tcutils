 /*
    makewords -- split the lines into words and print each on its own line
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

    SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <tc/tc.h>

int main(int argc, char *argv[]) {

	int ch;
	int i;
	int in = TC_STDIN;
	char b[TC_LINEMAX+1];
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "makewords foo.txt", .description = "split text in foo.txt up into words (one per line)" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "makewords",
		.usage = "[OPTIONS] [FILE]",
		.description = "split the lines into words and print each on its own line",
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

	if (argc > 0) {
		in = tc_open_reader(argv[0]);
		if (in == TC_ERR) {
			tc_puterr("Could no open file: ");
			tc_puterrln(argv[0]);
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	for (i = 0; (ch = tc_getc(in)) != TC_EOF && i < TC_LINEMAX-1; i++) {
		if (tc_isalpha(ch) || ch == '_' || ch == '\'') {
			b[i] = (char) ch;
		} else if (i == 0) {
			i--;
			continue;
		} else {
			b[i] = '\0';
			tc_putln(TC_STDOUT, b);
			i = -1;
		}
	}

	if (in != TC_STDIN) {
		tc_close(in);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
