 /*
    dc -- desk calculator
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

#include "config.h"

#include <getopt.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#define PROMPT "calc>"
#define STSZ 10000
static int64_t stack[STSZ];
static size_t sp = 0;
static int64_t reg[256];

static void push(int64_t v) {
	if (sp == STSZ) return;
	stack[sp] = v;
	sp++;
}

static int64_t peek(void) {
	if (sp == 0) return 0;
	return stack[sp-1];
}

static int64_t pop(void) {
	if (sp == 0) return 0;
	sp--;
	return stack[sp];
}

static void number(void) {
	int c, i;
	char n[64];

	i = 0;
	memset(n, '\0', 64);
	do {

		c = getc(stdin);
		if (!isdigit(c) || i >= 63) {
			ungetc(c, stdin);
			push(strtol(n, NULL, n[0] == '0' ? 8 : 10));
			return;
		}
		n[i++] = (char) c;
	} while (1);
}

int main(int argc, char *argv[]) {

	int ch;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "dc -- desk calculator\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: dc [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # invoke the calculator\n");
				fprintf(stdout, "  dc\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "dc (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	fprintf(stdout, "%s", PROMPT);
	do {
		int c;

		switch ((c = getc(stdin))) {

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				ungetc(c, stdin);
				number();
				break;

			case '+':
				push(pop() + pop());
				break;

			case '-':
				push(pop() - pop());
				break;

			case '*':
				push(pop() * pop());
				break;

			case '/':
				push(pop() / pop());
				break;

			case '%':
				push(pop() % pop());
				break;

			case 'd':
				push(peek());
				break;

			case 'f':
				while (sp > 0) {
					printf("%lld\n", pop());
				}
				break;

			case 'l':
				c = getc(stdin);
				push(reg[(unsigned char)c]);
				break;

			case 'p':
				printf("%lld\n", peek());
				break;

			case 'q':
				exit(EXIT_SUCCESS);

			case 's':
				c = getc(stdin);
				reg[(unsigned char)c] = pop();
				break;

			case '\n':
				fprintf(stdout, "%s", PROMPT);
				break;

			case ' ':
				break;

			default:
				puts("?");
				break;
		}
	} while (!feof(stdin) && !ferror(stdin));

	return 0;
}

