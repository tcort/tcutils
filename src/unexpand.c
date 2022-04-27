 /*
    unexpand -- converts spaces to tabs
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
	int i;
	int col;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'h', .longarg = "help", .description = "print help text", .has_value = 0 },
		{ .arg = 'V', .longarg = "version", .description = "print version and copyright info", .has_value = 0 },
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "unexpand", .description = "expand < spaces.txt > tabs.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "unexpand",
		.usage = "[OPTIONS]",
		.description = "converts spaces to tabs",
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
		if (col == 0 && ch == ' ') {
			/* collect spaces */
			while ((ch = tc_getc(TC_STDIN)) == ' ') {
				col++;
			}
			/* replace with tabs */
			for (i = 3; i <= col; i+= 4) {
				tc_putc(TC_STDOUT, '\t');
			}
			tc_putc(TC_STDOUT, ch);
		} else if (ch == '\n') {
			tc_putc(TC_STDOUT, ch);
			col = 0;
		} else {
			tc_putc(TC_STDOUT, ch);
			col++;
		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}
