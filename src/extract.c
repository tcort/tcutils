 /*
    extract -- extract values from the input and print them to the output
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

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	FILE *in = stdin;
	regex_t preg;
	regmatch_t pmatch[32];
	size_t nmatch = sizeof(pmatch)/sizeof(pmatch[0]), nread = 0, len = 0;
	char *pattern = TC_NULL, *format = TC_NULL, errbuf[128], *line = TC_NULL, *endnum = TC_NULL;
	int cflags = REG_EXTENDED, eflags = 0, errcode;
	ssize_t i;
	long placeholder = 0;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'i', .longarg = "ignore-case", .description = "case insensitive search", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "extract 'Invalid user (.*) from (.*) port' 'uid=$1 client_ip=$2' /var/log/auth.log", .description = "extract failed login attmpts (uid + ip) from auth.log" },

		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "extract",
		.usage = "[OPTIONS] PATTERN FORMAT [FILE]",
		.description = "extract values from the input and print them to the output",
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

	if (argc != 2 && argc != 3) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	pattern = argv[0];
	format = argv[1];
	if (argc == 3) {
		in = fopen(argv[2], "r");
		if (in == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	errcode = regcomp(&preg, pattern, cflags);
	if (errcode != 0) {
		regerror(errcode, &preg, errbuf, sizeof(errbuf));
		fprintf(stdout, "Bad Pattern: %s", errbuf);
		regfree(&preg);
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((nread = getline(&line, &len, in)) != -1) {
		line[nread-1] = '\0';

		errcode = regexec(&preg, line, nmatch, pmatch, eflags); 
		if (errcode == 0) {
			for (i = 0; i < strlen(format); i++) {
				if (format[i] == '$' && format[i+1] == '$') {
					fputc(format[i], stdout);
					i++;
				} else if (format[i] == '$' && tc_isdigit(format[i+1])) {
					placeholder = strtol(format + i + 1, &endnum, 10);
					i = endnum - 1 - format;
					if (placeholder >= 0 && placeholder < nmatch && pmatch[placeholder].rm_so != -1) {
						fwrite(line + pmatch[placeholder].rm_so, pmatch[placeholder].rm_eo - pmatch[placeholder].rm_so, 1, stdout);
					}
				} else {
					fputc(format[i], stdout);
				}
			}
			fputc('\n', stdout);
		}
	}

	fclose(in);
	regfree(&preg);
	if (line != TC_NULL) {
		free(line);
	}

	tc_exit(TC_EXIT_SUCCESS);
}

