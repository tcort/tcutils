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

#include "tc/args.h"
#include "tc/const.h"
#include "tc/ctype.h"
#include "tc/sys.h"
#include "tc/version.h"

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
		if (!tc_isdigit(c) || i >= 63) {
			ungetc(c, stdin);
			push(strtol(n, TC_NULL, n[0] == '0' ? 8 : 10));
			return;
		}
		n[i++] = (char) c;
	} while (1);
}

int main(int argc, char *argv[]) {

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "dc", .description = "invoke the calculator" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "dc",
		.usage = "[OPTIONS]",
		.description = "desk calculator",
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
				tc_exit(TC_EXIT_SUCCESS);

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

