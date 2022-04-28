 /*
    expand -- converts tabs to spaces
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
#include "tc/sys.h"
#include "tc/version.h"

int main(int argc, char *argv[]) {

	int ch;
	int col;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "expand", .description = "expand < tabs.txt > spaces.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "expand",
		.usage = "[OPTIONS]",
		.description = "converts tabs to spaces",
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

	col = 0;
	while ((ch = tc_getc(TC_STDIN)) != TC_EOF) {
		switch (ch) {
			case '\t':
				do {
					tc_putc(TC_STDOUT, ' ');
					col++;
				} while (col % 4 != 0);
				break;
			case '\n':
				tc_putc(TC_STDOUT, ch);
				col = 0;
				break;
			default:
				tc_putc(TC_STDOUT, ch);
				col++;
				break;
		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}
