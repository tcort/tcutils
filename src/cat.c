 /*
    cat -- concatenate and print files
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

	int ch;
	int i;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "cat foo.txt bar.txt > foobar.txt", .description = "concatinate two files" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "cat",
		.usage = "[OPTIONS] [FILE...]",
		.description = "concatenate and print files",
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

	if (argc == 0) {
		while ((ch = tc_getc(TC_STDIN)) != TC_EOF) {
			tc_putc(TC_STDOUT, ch);
		}
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		int fd;
		fd = tc_open_reader(argv[i]);
		if (fd == TC_ERR) {
			tc_puterr("Could not open file: ");
			tc_puterrln(argv[i]);
			tc_exit(TC_EXIT_FAILURE);
		}

		while ((ch = tc_getc(fd)) != TC_EOF) {
			tc_putc(TC_STDOUT, ch);
		}

		tc_close(fd);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
