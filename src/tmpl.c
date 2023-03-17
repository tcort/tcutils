 /*
    tmpl -- template processor
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define C '}'
#define O '{'
#define L 2048
#define D '$'
#define Y '\\'
#define S '%'

int main(int argc, char *argv[]) {

	int ch;
	char s[L];

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "tmpl < foo.tmpl > foo.html", .description = "tmpl < foo.tmpl > foo.html" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "tmpl",
		.usage = "[OPTIONS]",
		.description = "template processor",
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

	while ((ch = getc(stdin)) != EOF) {
		int e;
		if (ch == D || ch == S) {
			int d;
			d = getc(stdin);
			if (d == O) {
				FILE *p;
				int h,i;

				for (i = 0; i < L-1; i++) {
					s[i] = (char) getc(stdin);
					if (s[i] == EOF || s[i] == C) {
						break;
					}
				}
				s[i] = '\0';

				if ((p = popen(s, "r")) == TC_NULL) {
					perror("popen");
					break;
				}
				while ((h = getc(p)) != EOF) {
					if (h == '\n') {
						int n;
						n = getc(p);
						if (n == EOF) {
							break;
						} else {
							ungetc(n, p);
						}
					}
					if (ch == S) {
						printf("&#%d;", h);
					} else {
						putc(h, stdout);
					}
				}
				pclose(p);
				continue;
			} else if (d == EOF) {
				break;
			}


			putc(ch, stdout);
			ch = d;
		}
		if (ch == Y) {
			e = ch;
			ch = getc(stdin);
			if (!(ch == D || ch == S)) {
				ungetc(ch, stdin);
				ch = e;
			}
		}
		putc(ch, stdout);
	}

	return TC_EXIT_SUCCESS;
}
