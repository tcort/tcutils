 /*
    md2 -- calculate a message-digest fingerprint for a file
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	FILE *fp;
	tc_uint64_t len, cap;
	int ch;
	int i;
	tc_uint8_t *p, *q;
	tc_uint8_t b;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "md2 foo.txt", .description = "print the MD2 message digest of a file" },
		{ .command = "... | md2", .description = "print the MD2 message digest of standard input" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "md2",
		.usage = "[OPTIONS] [FILE]",
		.description = "calculate a message-digest fingerprint for a file",
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

	fp = argc == 0 ? stdin : fopen(argv[0], "r");
	if (fp == TC_NULL) {
		tc_exit(TC_EXIT_FAILURE);
	}

	len = 0;
	cap = 512; /* initial capacity (tc_uint8_ts) */

	p = (tc_uint8_t *) malloc(cap);
	if (p == TC_NULL) {
		fclose(fp);
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((ch = fgetc(fp)) != EOF) {
		p[len] = (tc_uint8_t) ch;
		len++;
		if (len >= cap) {
			cap = 2 * len;
			q = (tc_uint8_t *) malloc(cap);
			if (q == TC_NULL) {
				free(p);
				fclose(fp);
				tc_exit(TC_EXIT_FAILURE);
			}
			memcpy(q, p, len);
			free(p);
			p = q;
			q = TC_NULL;
		}
	}

	tc_md2(p, len);
	free(p);

	fclose(fp);

	tc_exit(TC_EXIT_SUCCESS);
}
