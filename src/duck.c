 /*
    duck -- virtual rubber duck debugging tool
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

#include <tc/tc.h>

int main(int argc, char *argv[]) {

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "duck", .description = "show me the duck" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "duck",
		.usage = "[OPTIONS]",
		.description = "virtual rubber duck debugging tool",
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

	tc_puts(TC_STDOUT, COLOUR_BRIGHT_YELLOW);
	tc_puts(TC_STDOUT, "\n");
	tc_puts(TC_STDOUT, "                                       dkcudkcud\n");
	tc_puts(TC_STDOUT, "                                    kcu         kcu\n");
	tc_puts(TC_STDOUT, "                                  ud               dk\n");
	tc_puts(TC_STDOUT, "                                 c          duck     c\n");
	tc_puts(TC_STDOUT, "                                k          d    d     u\n");
	tc_puts(TC_STDOUT, "                               d          u  kd  u    duckduckd\n");
	tc_puts(TC_STDOUT, "                               D           c    c     k       u\n");
	tc_puts(TC_STDOUT, "                               u            duck      c      c\n");
	tc_puts(TC_STDOUT, "                                c                     u   uck\n");
	tc_puts(TC_STDOUT, "                                 k                    d    cud\n");
	tc_puts(TC_STDOUT, "          d                       c                  kuduck\n");
	tc_puts(TC_STDOUT, "          ukc                     dk                cud\n");
	tc_puts(TC_STDOUT, "          c  udkcudkcudkcudkcudkcu                     k\n");
	tc_puts(TC_STDOUT, "          k                        ckduck               cu\n");
	tc_puts(TC_STDOUT, "          d      duckduckduckduckdu      d                d\n");
	tc_puts(TC_STDOUT, "          u      d                        u                k\n");
	tc_puts(TC_STDOUT, "          c       k                       c                c\n");
	tc_puts(TC_STDOUT, "           k       c                      k                u\n");
	tc_puts(TC_STDOUT, "            d       u                    d                 d\n");
	tc_puts(TC_STDOUT, "             u       dk                 u                 k\n");
	tc_puts(TC_STDOUT, "             c         cudkcudkcudkcudkc                 c\n");
	tc_puts(TC_STDOUT, "              kdu                                      du\n");
	tc_puts(TC_STDOUT, "                 ckd                                uck\n");
	tc_puts(TC_STDOUT, "                    uckduckduckduckduckduckduckduckd\n");
	tc_puts(TC_STDOUT, COLOUR_RESET);

	tc_exit(TC_EXIT_SUCCESS);
}
