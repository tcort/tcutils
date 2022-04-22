 /*
    grep -- searches for and prints lines that match a given pattern
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
#include "tc/colours.h"
#include "tc/ctype.h"
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
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

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "extended-regexp", no_argument, 0, 'E' },
		{ "fixed-strings", no_argument, 0, 'F' },
		{ "basic-regexp", no_argument, 0, 'G' },
		{ "count", no_argument, 0, 'c' },
		{ "ignore-case", no_argument, 0, 'i' },
		{ "line-number", no_argument, 0, 'n' },
		{ "color", required_argument, 0, 'C' },
		{ "colour", required_argument, 0, 'C' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "CEFGVchin", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'C':
				if (strcmp(optarg, "auto") == 0) {
					colors = COLOUR_MODE_AUTO;
				} else if (strcmp(optarg, "never") == 0) {
					colors = COLOUR_MODE_NEVER;
				} else if (strcmp(optarg, "always") == 0) {
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
				fprintf(stdout, "grep -- searches for and prints lines that match a given pattern\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: grep [OPTIONS] PATTERN [FILE...]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -C, --color=WHEN, --colour=WHEN  set color output to never, always, or auto\n");
				fprintf(stdout, "  -E, --extended-regexp            use POSIX extended regular expression syntax\n");
				fprintf(stdout, "  -F, --fixed-strings              performed fixed string search (similar to fgrep)\n");
				fprintf(stdout, "  -G, --basic-regexp               use POSIX basic regular expression syntax\n");
				fprintf(stdout, "  -c, --count                      just count matchiing lines\n");
				fprintf(stdout, "  -h, --help                       print help text\n");
				fprintf(stdout, "  -i, --ignore-case                case insensitive search\n");
				fprintf(stdout, "  -n, --line-number                prepend line numbers to output\n");
				fprintf(stdout, "  -V, --version                    print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print all lines in foo.txt containing 'bar' with line numbers\n");
				fprintf(stdout, "  grep -n bar foo.txt\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the count of lines in foo.txt that contain bar\n");
				fprintf(stdout, "  grep -c bar foo.txt\n");
				fprintf(stdout, "\n");
				tc_exit(TC_EXIT_SUCCESS);
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
				fprintf(stdout, "grep (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	if (argc != 1 && argc != 2) {
		fprintf(stderr, "usage: grep [OPTIONS] PATTERN [FILE]\n");
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

