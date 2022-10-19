 /*
    echo -- prints it's command line arguments
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

	int i;
	int flag_n;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'n', .longarg = "no-newline", .description = "omit trailing newline", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "echo \"Hello, ${USER}!\"", .description = "greet the user" },
		{ .command = "echo -n \"What is your name\"", .description = "print a message without a newline" },
		{ .command = "echo", .description = "print a newline" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "echo",
		.usage = "[OPTIONS] [ARGS...]",
		.description = "prints it's command line arguments",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_n = 0;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'n':
				flag_n = 1;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	for (i = 0; i < argc; i++) {
		tc_puts(TC_STDOUT, argv[i]);
		if (i + 1 < argc) {
			tc_puts(TC_STDOUT, " ");
		}
	}

	if (flag_n == 0) {
		tc_puts(TC_STDOUT, "\n");
	}

	tc_exit(TC_EXIT_SUCCESS);
}
