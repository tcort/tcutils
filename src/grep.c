 /*
    grep -- searches for and prints lines that match a given pattern
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

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

enum color_mode {
	COLOUR_MODE_NEVER,
	COLOUR_MODE_AUTO,
	COLOUR_MODE_ALWAYS
};


static int tccmp(char *x, char *y, int icase) {
	size_t i;
	for (i = 0; i < tc_strlen(x) && i < tc_strlen(y); i++) {
		if (x[i] != y[i] && !(icase && tc_tolower(x[i]) == tc_tolower(y[i]))) {
			return -1;
		}
	}
	return 0;
}

static int tcstrstr(char *hay, char *need, int icase) {
	size_t i;
	int rc;
	for (i = 0; i < tc_strlen(hay); i++) {
		if (hay[i] == need[0] || (icase && tc_tolower(hay[i]) == tc_tolower(need[0]))) {
			rc = tccmp(hay + i, need, icase);
			if (rc == 0) {
				return 0;
			}
		}
	}

	return -1;
}

int main(int argc, char *argv[]) {

	FILE *in = stdin;
	regex_t preg;
	regmatch_t pmatch[1];
	char *pattern = TC_NULL, errbuf[128], line[2048], *retp = TC_NULL;
	int cflags = 0, eflags = 0, errcode = 0, ch = 0, show_lineno = 0, line_len = sizeof(line), fixed_mode = 0, icase = 0, just_count = 0;
	size_t nmatch = 1, lineno = 0, count = 0;
	enum color_mode colors = COLOUR_MODE_AUTO;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'C', .longarg = "color", .description = "set color output to never, always, or auto", .has_value = 1 },
		{ .arg = 'E', .longarg = "extended-regexp", .description = "use POSIX extended regular expression syntax", .has_value = 0 },
		{ .arg = 'F', .longarg = "fixed-strings", .description = "performed fixed string search (similar to fgrep)", .has_value = 0 },
		{ .arg = 'G', .longarg = "basic-regexp", .description = "use POSIX basic regular expression syntax", .has_value = 0 },
		{ .arg = 'c', .longarg = "count", .description = "just count matching lines", .has_value = 0 },
		TC_PROG_ARG_HELP,
		{ .arg = 'i', .longarg = "ignore-case", .description = "case insensitive search", .has_value = 0 },
		{ .arg = 'n', .longarg = "line-number", .description = "prepend line numbers to output", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "grep -n bar foo.txt", .description = "print all lines in foo.txt containing 'bar' with line numbers" },
		{ .command = "grep -c bar foo.txt", .description = "print the count of lines in foo.txt that contain bar" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "grep",
		.usage = "[OPTIONS] PATTERN [FILE...]",
		.description = "searches for and prints lines that match a given pattern",
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
			case 'C':
				if (strcmp(argval, "auto") == 0) {
					colors = COLOUR_MODE_AUTO;
				} else if (strcmp(argval, "never") == 0) {
					colors = COLOUR_MODE_NEVER;
				} else if (strcmp(argval, "always") == 0) {
					colors = COLOUR_MODE_ALWAYS;
				} else {
					fprintf(stderr, "grep: invalid colour options. must be one of auto, never, always\n");
					tc_exit(TC_EXIT_FAILURE);
				}

				break;
			case 'c':
				just_count = 1;
				break;
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'i':
				cflags |= REG_ICASE;
				icase = 1;
				break;
			case 'n':
				show_lineno = 1;
				break;
			case 'E':
				cflags |= REG_EXTENDED;
				break;
			case 'F':
				fixed_mode = 1;
				break;
			case 'G':
				cflags &= ~(REG_EXTENDED);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc != 1 && argc != 2) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	if (colors == COLOUR_MODE_AUTO) {
		colors = tc_isatty(TC_STDOUT) ? COLOUR_MODE_ALWAYS : COLOUR_MODE_NEVER;
	}

	pattern = argv[0];
	if (argc == 2) {
		in = fopen(argv[1], "r");
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

	do {
		retp = fgets(line, line_len, in);
		if (retp == TC_NULL) {
			break;
		} else if (line[0] != '\0') {
			line[tc_strlen(line)-1] = '\0';
		}

		lineno++;

		errcode = (fixed_mode) ? tcstrstr(line, pattern, icase) : regexec(&preg, line, nmatch, pmatch, eflags); 
		if (errcode == 0) {
			if (just_count == 0) {
				if (show_lineno) {
					if (colors == COLOUR_MODE_ALWAYS) {
						fprintf(stdout, "%s", COLOUR_BRIGHT_CYAN);
					}
					fprintf(stdout, "%lu:", lineno);
					if (colors == COLOUR_MODE_ALWAYS) {
						fprintf(stdout, "%s", COLOUR_RESET);
					}
				}
				if (colors == COLOUR_MODE_ALWAYS) {
					fprintf(stdout, "%s", COLOUR_BRIGHT_WHITE);
				}
				fprintf(stdout, "%s\n", line);
				if (colors == COLOUR_MODE_ALWAYS) {
					fprintf(stdout, "%s", COLOUR_RESET);
				}
			}
			count++;
		}
	} while (!feof(in) && !ferror(in));

	if (just_count == 1) {
		if (colors == COLOUR_MODE_ALWAYS) {
			fprintf(stdout, "%s", COLOUR_BRIGHT_GREEN);
		}
		fprintf(stdout, "%lu\n", count);
		if (colors == COLOUR_MODE_ALWAYS) {
			fprintf(stdout, "%s", COLOUR_RESET);
		}
	}

	fclose(in);
	regfree(&preg);

	tc_exit(TC_EXIT_SUCCESS);
}

