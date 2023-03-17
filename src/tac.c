 /*
    tac -- concatenate and print files in reverse
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
#include <string.h>
#include <unistd.h>

struct list {
	char *line;
	struct list *next;
};

int main(int argc, char *argv[]) {

	int ch;
	int i;
	size_t cap = 0;
	ssize_t len = 0;
	struct list *head = TC_NULL;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "tac foo.txt bar.txt > foobar.txt", .description = "concatinate and reverse two files" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "tac",
		.usage = "[OPTIONS] [FILES...]",
		.description = "concatenate and print files in reverse",
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
		while ((ch = getc(stdin)) != EOF) {
			putc(ch, stdout);
		}
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		struct list *cur;
		FILE *f = fopen(argv[i], "r");
		if (f == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}

		while (1) {
			cur = (struct list *) tc_malloc(sizeof(struct list));
			if (cur == TC_NULL) {
				perror("malloc");
				while (head != TC_NULL) {
					head->line = tc_free(head->line);
					cur = head;
					head = head->next;
					cur = tc_free(cur);
				}
				tc_exit(TC_EXIT_FAILURE);
			}
			tc_memset(cur, '\0', sizeof(struct list));

			cap = 0;
			len = getline(&cur->line, &cap, f);
			if (len < 0) {
				if (cur->line != TC_NULL) {
					cur->line = tc_free(cur->line);
				}
				cur = tc_free(cur);
				break;
			}

			cur->next = head;
			head = cur;
		}

		while (head != TC_NULL) {
			fprintf(stdout, "%s", head->line);
			head->line = tc_free(head->line);
			cur = head;
			head = head->next;
			cur = tc_free(cur);
		}

		fclose(f);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
