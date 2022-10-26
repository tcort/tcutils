 /*
    hexdump -- prints the contents of a file in hexadecimal
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	FILE *f;
	char bytes[16];
	int i, j;
	size_t n, addr;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "hexdump foo.bin", .description = "show file contents as hex value" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "hexdump",
		.usage = "[OPTIONS] FILE",
		.description = "prints the contents of a file in hexadecimal",
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

	if (argc != 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	f = fopen(argv[0], "rb");
	if (f == TC_NULL) {
		perror("fopen");
		tc_exit(TC_EXIT_FAILURE);
	}

	addr = 0;
	do {
		memset(bytes, '\0', sizeof(char) * 16);
		n = fread(bytes, sizeof(char), 16, f);
		if (n == 0) {
			break;
		}
		printf("%.8lx  ", addr);
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 8; j++) {
				printf("%.2x", 0xff & bytes[(i*8)+j]);
				printf(" ");
			}
			printf(" ");
		}
		printf("|");
		for (i = 0; i < 16; i++) {
			printf("%c", tc_isprint(bytes[i]) ? bytes[i] : '.');
		}
		printf("|");
		printf("\n");
		addr+=n;
	} while (1);

	fclose(f);

	tc_exit(TC_EXIT_SUCCESS);
}
