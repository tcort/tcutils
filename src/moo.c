 /*
    moo -- guess the bulls
    Copyright (C) 2022, 2023, 2024  Thomas Cort

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

int main(int argc, char *argv[]) {

	int b,c,i,j;
	char line[TC_LINEMAX], x, farm[10];
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "moo", .description = "play the game" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "moo",
		.usage = "[OPTIONS]",
		.description = "guess the bulls",
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

	tc_srand((unsigned) tc_getpid());

	for (i = 0; i < 10; i++) {
		farm[i] = '0'+(char)i;
	}

	for (i = 9; i >= 1; i--) {
		b = tc_abs(tc_rand()) % (i+1);
		if (b == 0 && farm[i] == '0') {
			i++;
			continue;
		}
		x = farm[b];
		farm[b] = farm[i];
		farm[i] = x;
	}

	do {
		fputs("your guess?\n", stdout);
		fgets(line, TC_LINEMAX - 1, stdin);
		for (b=c=i=0; i < 4; i++)
			if (line[i] == farm[i]) b++;
			else 
				for (j = 0; j < 4; j++)
					if (line[i] == farm[j]) c++;
		printf("bulls = %d  cows = %d\n", b, c);
		if (b== 4) break;
	} while (!feof(stdin) && !ferror(stdin));

	tc_exit(TC_EXIT_SUCCESS);
}
