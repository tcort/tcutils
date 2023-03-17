 /*
    md2 -- calculate a message-digest fingerprint for a file
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

int main(int argc, char *argv[]) {

	tc_uint64_t len, cap;
	int ch;
	int i;
	int fd;
	char *digest;
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

	fd = argc == 0 ? TC_STDIN : tc_open_reader(argv[0]);
	if (fd == TC_ERR) {
		tc_exit(TC_EXIT_FAILURE);
	}

	len = 0;
	cap = 512; /* initial capacity (tc_uint8_ts) */

	p = (tc_uint8_t *) tc_malloc(cap);
	if (p == TC_NULL) {
		tc_close(fd);
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((ch = tc_getc(fd)) != TC_EOF) {
		p[len] = (tc_uint8_t) ch;
		len++;
		if (len >= cap) {
			cap = 2 * len;
			q = (tc_uint8_t *) tc_malloc(cap);
			if (q == TC_NULL) {
				p = tc_free(p);
				tc_close(fd);
				tc_exit(TC_EXIT_FAILURE);
			}
			tc_memcpy(q, p, len);
			p = tc_free(p);
			p = q;
			q = TC_NULL;
		}
	}

	digest = tc_md2(p, len);

	tc_putln(TC_STDOUT, digest);

	digest = tc_free(digest);
	p = tc_free(p);
	tc_close(fd);

	tc_exit(TC_EXIT_SUCCESS);
}
