 /*
    sum -- compute a BSD checksum of the input
    Copyright (C) 2022, 2023  Thomas Cort

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

static void sum(int fd, char *filename) {

	unsigned int r;
	int ch;
	char *s;

	r = 0;

	while ((ch = tc_getc(fd)) != TC_EOF) {
		r = ((r >> 1) + ((r & 1) << 15) + ch) & 0xffff;
	}

	s = tc_utoa(r);
	if (s == TC_NULL) {
		tc_puterrln("Out of Memory");
		return;
	}

	tc_puts(TC_STDOUT, s);
	s = tc_free(s);

	tc_puts(TC_STDOUT, "\t");
	tc_puts(TC_STDOUT, filename);
	tc_puts(TC_STDOUT, "\n");
}

int main(int argc, char *argv[]) {

	int i;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "sum foo.txt", .description = "compute the checksum of foo.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "sum",
		.usage = "[OPTIONS] [FILE...]",
		.description = "compute a BSD checksum of the input",
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
		sum(TC_STDIN, "<stdin>");
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		int fd;
		fd = tc_open_reader(argv[i]);
		if (fd == TC_ERR) {
			tc_puterr("Failed to open file: ");
			tc_puterr(argv[i]);
			tc_puterr("\n");
			tc_exit(TC_EXIT_FAILURE);
		}

		sum(fd, argv[i]);

		tc_close(fd);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
