 /*
    bfi -- interpreter for the brainfuck programming language
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

#define MEMSZ (32768)

int main(int argc, char **argv) {

	int fd; /* file descriptor for reading source code */
	int i; /* index */
	int pc; /* program counter */
	int xc; /* working pointer */
	int prog_len; /* program length */
	int l; /* depth of loops */
	int x[MEMSZ]; /* working memory */
	int p[MEMSZ]; /* program memory */

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "bfi filename", .description = "interpret the contents of filename" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "bfi",
		.usage = "[OPTIONS] FILENAME...",
		.description = "interpreter for the brainfuck programming language",
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

	for (i = 0; i < argc; i++) {

		fd = tc_open_reader(argv[i]);
		if (fd == -1) {
			tc_puterr("Could not open file: ");
			tc_puterrln(argv[i]);
			tc_exit(TC_EXIT_FAILURE);
		}

		prog_len = 0;
		for (pc = 0; pc < MEMSZ && (p[pc] = tc_getc(fd)) != TC_EOF; pc++) {
			prog_len++;
		}

		tc_close(fd);

		xc = 0;
		for (xc = 0; xc < MEMSZ; xc++) {
			x[xc] = 0;
		}

		for(xc = l = pc = 0; pc < prog_len; pc++) {

			switch (p[pc]) {

				case '+':
					x[xc]++;
					break;

				case '-':
					x[xc]--;
					break;

				case '.':
					tc_putc(TC_STDOUT, x[xc]);
					break;

				case ',':
					x[xc] = tc_getc(TC_STDIN);
					break;

				case '>':
					xc++;
					break;

				case '<':
					xc--;
					break;

				case '[':
					if (x[xc] == 0) {
						pc++;
						while (l > 0 || p[pc] != ']') {
							if (p[pc] == '[') {
								l++;
							} else if (p[pc] == ']') {
								l--;
							}
							pc++;
						}
					}
					break;

				case ']':
					pc--;
					while (l > 0 || p[pc] != '[') {
						if (p[pc] == ']') {
							l++;
						} else if (p[pc] == '[') {
							l--;
						}
						pc--;
					}
					pc--;
					break;

				default:
					break;
			}
		}	
	}

	return 0;
}
