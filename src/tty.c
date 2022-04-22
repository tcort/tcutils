 /*
    tty -- print the filename associated with the tty connected to stdin
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
#include "tc/stdio.h"
#include "tc/sys.h"
#include "tc/version.h"

int main(int argc, char *argv[]) {

	char *tty;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'h', .longarg = "help", .description = "print help text", .has_value = 0 },
		{ .arg = 'V', .longarg = "version", .description = "print version and copyright info", .has_value = 0 },
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "tty", .description = "print the current tty" },
		{ .command = "echo 'Hello' | tty", .description = "print 'not a tty' when stdin is not a tty" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "tty",
		.usage = "[OPTIONS]",
		.description = "print the filename associated with the tty connected to stdin",
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

	tty = tc_ttyname(TC_STDIN);
	if (tty == TC_NULL) {
		tc_putln(TC_STDOUT, "not a tty");
	} else {
		tc_putln(TC_STDOUT, tty);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
