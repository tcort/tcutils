 /*
    d6 -- simulates a 6-sided dice roll and prints the reults
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
#include "tc/math.h"
#include "tc/stdlib.h"
#include "tc/stdio.h"
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

int main(int argc, char *argv[]) {

	int roll;
	char *result;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "d6", .description = "roll the die" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "d6",
		.usage = "[OPTIONS]",
		.description = "simulates a 6-sided dice roll and prints the reults",
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

	tc_srand((unsigned int) tc_getpid());

	do {	/* attempt to get a uniform roll */
		roll = tc_abs(tc_rand());	/* get a random number */
		roll &= 0x7;			/* capture lower 3 bits (value 0-7) */
	} while (roll >= 6);			/* repeat until value between 0-5 */

	result = tc_itoa(roll + 1); /* +1 to get value in range 1-6 */
	if (result == TC_NULL) {
		tc_puterrln("Out of Memory");
	} else {
		tc_putln(TC_STDOUT, result);
		result = tc_free(result);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
