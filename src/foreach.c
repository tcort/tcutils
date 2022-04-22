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
 */

#include "tc/const.h"
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <regex.h>
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
	memset(cmd, '\0', len + 1);

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
	memset(result, '\0', len + 1);

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

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "foreach -- executes a command for every line of standard input\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: foreach [OPTIONS] CMD\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # find all source files and remove the .c at the end to get the program names\n");
				fprintf(stdout, "  # execute each program with the `--help` argument and grab the first line of output\n");
				fprintf(stdout, "  # {} is the placeholder for standard input\n");
				fprintf(stdout, "  ls -1 ../src | extract '(.*)\\.c$' '$1' | foreach -- '{} --help | ./head -n 1'\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # rename files *.dat to *.dat.done\n");
				fprintf(stdout, "  # {} is the placeholder for standard input\n");
				fprintf(stdout, "  ls -1 *.dat | foreach mv {} {}.done\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "foreach (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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
