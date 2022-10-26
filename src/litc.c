 /*
    litc -- extract literate programs from markdown documents
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

	char *line = TC_NULL;
	size_t linecap = 0;
	ssize_t linelen;

	int in_code_block = 0;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "litc < doc.md > prog.c", .description = "extract C source code from a markdown document doc.md" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "litc",
		.usage = "[OPTIONS]",
		.description = "extract literate programs from markdown documents",
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

	while ((linelen = getline(&line, &linecap, stdin)) > 0) {

		if (strncmp("```", line, 3) == 0) {
			in_code_block = !in_code_block;
		} else if (in_code_block) {
			fwrite(line, linelen, 1, stdout);
		} else if (strncmp("    ", line, 4) == 0) {
			fwrite(line + 4, linelen - 4, 1, stdout);
		} else if (line[0] == '\t') {
			fwrite(line + 1, linelen - 1, 1, stdout);
		}
	}

	if (line != TC_NULL) {
		free(line);
		line = TC_NULL;
	}

	return 0;
}
