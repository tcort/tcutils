 /*
    foreach -- executes a command for every line of standard input
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

void foreach(int argc, char *argv[], char *line) {

	int i;
	int j;
	int k;
	int n;
	char *cmd;
	char *result;
	size_t len = 0;

	/* combine argv into one string */
	for (i = 0; i < argc; i++) {
		len += tc_strlen(argv[i]) + 1;
	}

	cmd = (char *) malloc(len + 1);
	if (cmd == TC_NULL) {
		perror("malloc");
		tc_exit(TC_EXIT_FAILURE);
	}
	tc_memset(cmd, '\0', len + 1);

	for (i = 0; i < argc; i++) {
		strncat(cmd, argv[i], len);
		if (i + 1 != argc) {
			strncat(cmd, " ", len);
		}
	}

	tc_chomp(line);

	/* find instances of {} and replace with line */
	n = 0;
	for (i = 0; i < tc_strlen(cmd); i++) {
		if (cmd[i] == '{' && cmd[i+1] == '}') {
			n++;
		}
	}

	len = tc_strlen(cmd) + (n * tc_strlen(cmd)) + 1;
	result = (char *) malloc(len + 1);
	if (result == TC_NULL) {
		perror("malloc");
		tc_exit(TC_EXIT_FAILURE);
	}
	tc_memset(result, '\0', len + 1);

	/* i is index in cmd, j is index in result, k is index in line */
	for (i = j = 0; i < tc_strlen(cmd); i++) {
		if (cmd[i] == '{' && cmd[i+1] == '}') {
			for (k = 0; k < tc_strlen(line); k++) {
				result[j] = line[k];
				j += 1;
			}
			i += 1; /* skip over '}' */
		} else {
			result[j] = cmd[i];
			j += 1;
		}
	}

	free(cmd);
	cmd = TC_NULL;

	system(result);

	free(result);
	result = TC_NULL;

}

int main(int argc, char *argv[]) {

	int ch;
	char *line;
	size_t linecap;
	ssize_t linelen;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "ls -1 ../src | extract '(.*)\\.c$' '$1' | foreach -- '{} --help | ./head -n 1'", .description = "find all source files and remove the .c at the end to get the program names. execute each program with the `--help` argument and grab the first line of output. {} is the placeholder for standard input" },
		{ .command = "ls -1 *.dat | foreach mv {} {}.done", .description = "rename files *.dat to *.dat.done {} is the placeholder for standard input" },
		{ .command = "seq 1 10 | foreach d6", .description = "simulate 10 rolls of a 6 sided die" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "foreach",
		.usage = "[OPTIONS]",
		.description = "executes a command for every line of standard input",
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

	line = TC_NULL;
	linecap = 0;

	while ((linelen = getline(&line, &linecap, stdin)) > 0) {
		foreach(argc, argv, line);
	}

	if (line != TC_NULL) {
		free(line);
		line = TC_NULL;
	}

	tc_exit(TC_EXIT_SUCCESS);
}
