/*
    crc32 -- output the 32-bit cyclic redundancy check value for an input
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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void crc32(int fd, char *filename) {

	int ch;
	tc_uint32_t crc = tc_crc32_begin();
	tc_uint64_t len = 0;

	while ((ch = tc_getc(fd)) != TC_EOF) {
		tc_uint32_t i;
		tc_uint32_t value = ch;
		crc = tc_crc32_update(crc, value);
		len++;
	}

	crc = tc_crc32_end(crc);
	
	fprintf(stdout, "%"PRIu32" %"PRIu64" %s\n", crc, len, filename);

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
		{ .command = "crc32 foo.txt", .description = "compute the check for foo.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "crc32",
		.usage = "[OPTIONS] [FILE...]",
		.description = "output the 32-bit cyclic redundancy check value for an input",
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
		crc32(TC_STDIN, "<stdin>");
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		int fd = tc_open_reader(argv[i]);
		if (fd == TC_ERR) {
			tc_puterr("Could not open file: ");
			tc_puterrln(argv[i]);
			tc_exit(TC_EXIT_FAILURE);
		}

		crc32(fd, argv[i]);
		tc_close(fd);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
