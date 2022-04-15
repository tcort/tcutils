 /*
    tac -- concatenate and print files in reverse
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

#include "tc/const.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
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

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "tac -- concatenate and print files in reverse\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: tac [OPTIONS] [FILES...]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # concatinate and reverse two files\n");
				fprintf(stdout, "  tac foo.txt bar.txt > foobar.txt\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "cat (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				tc_exit(TC_EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

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
			cur = (struct list *) malloc(sizeof(struct list));
			if (cur == TC_NULL) {
				perror("malloc");
				while (head != TC_NULL) {
					free(head->line);
					cur = head;
					head = head->next;
					free(cur);
				}
				tc_exit(TC_EXIT_FAILURE);
			}
			memset(cur, '\0', sizeof(struct list));

			cap = 0;
			len = getline(&cur->line, &cap, f);
			if (len < 0) {
				if (cur->line != TC_NULL) {
					free(cur->line);
				}
				free(cur);
				break;
			}

			cur->next = head;
			head = cur;
		}

		while (head != TC_NULL) {
			fprintf(stdout, "%s", head->line);
			free(head->line);
			cur = head;
			head = head->next;
			free(cur);
		}

		fclose(f);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
