 /*
    coinflip -- simulates a coinflip and prints the result
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

	char *heads = "heads";
	char *tails = "tails";
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'H', .longarg = "heads", .description = "alternative label for heads face", .has_value = 1 },
		TC_PROG_ARG_HELP,
		{ .arg = 'T', .longarg = "tails", .description = "alternative label for tails face", .has_value = 1 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "coinflip", .description = "flip a coin" },
		{ .command = "coinflip -H true -T false", .description = "random boolean" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "coinflip",
		.usage = "[OPTIONS]",
		.description = "simulates a coinflip and prints the result",
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
			case 'H':
				heads = argval;
				break;
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'T':
				tails = argval;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	tc_srand((unsigned int) tc_getpid());

	if (tc_abs(tc_rand()) % 2 == 0) {
		tc_putln(TC_STDOUT, heads);
	} else {
		tc_putln(TC_STDOUT, tails);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
