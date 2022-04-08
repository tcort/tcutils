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

#include "config.h"

#include <getopt.h>
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

enum color_mode {
	COLOR_MODE_NEVER,
	COLOR_MODE_AUTO,
	COLOR_MODE_ALWAYS
};

#define COLOR_RESET   "\x1b[0m"

#define COLOR_BLACK   "\x1b[30m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

#define COLOR_BRIGHT_BLACK   "\x1b[30;1m"
#define COLOR_BRIGHT_RED     "\x1b[31;1m"
#define COLOR_BRIGHT_GREEN   "\x1b[32;1m"
#define COLOR_BRIGHT_YELLOW  "\x1b[33;1m"
#define COLOR_BRIGHT_BLUE    "\x1b[34;1m"
#define COLOR_BRIGHT_MAGENTA "\x1b[35;1m"
#define COLOR_BRIGHT_CYAN    "\x1b[36;1m"
#define COLOR_BRIGHT_WHITE   "\x1b[37;1m"

static int tccmp(char *x, char *y, int icase) {
	size_t i;
	for (i = 0; i < strlen(x) && i < strlen(y); i++) {
		if (x[i] != y[i] && !(icase && tolower(x[i]) == tolower(y[i]))) {
			return -1;
		}
	}
	return 0;
}

static int tcstrstr(char *hay, char *need, int icase) {
	size_t i;
	int rc;
	for (i = 0; i < strlen(hay); i++) {
		if (hay[i] == need[0] || (icase && tolower(hay[i]) == tolower(need[0]))) {
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
	char *pattern = NULL, errbuf[128], line[2048], *retp = NULL;
	int cflags = 0, eflags = 0, errcode = 0, ch = 0, show_lineno = 0, line_len = sizeof(line), fixed_mode = 0, icase = 0, just_count = 0;
	size_t nmatch = 1, lineno = 0, count = 0;
	enum color_mode colors = COLOR_MODE_AUTO;

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

	while ((ch = getopt_long(argc, argv, "EFGVchin", long_options, NULL)) != -1) {
		switch (ch) {
			case 'C':
				if (strcmp(optarg, "auto") == 0) {
					colors = COLOR_MODE_AUTO;
				} else if (strcmp(optarg, "never") == 0) {
					colors = COLOR_MODE_NEVER;
				} else if (strcmp(optarg, "always") == 0) {
					colors = COLOR_MODE_ALWAYS;
				} else {
					fprintf(stderr, "grep: invalid colour options. must be one of auto, never, always\n");
					exit(EXIT_FAILURE);
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
				exit(EXIT_SUCCESS);
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
				fprintf(stdout, "grep (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	if (argc != 1 && argc != 2) {
		fprintf(stderr, "usage: grep [OPTIONS] PATTERN [FILE]\n");
		exit(EXIT_FAILURE);
	}

	if (colors == COLOR_MODE_AUTO) {
		colors = (isatty(fileno(stdout))) ? COLOR_MODE_ALWAYS : COLOR_MODE_NEVER;
	}

	pattern = argv[0];
	if (argc == 2) {
		in = fopen(argv[1], "r");
		if (in == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	}

	errcode = regcomp(&preg, pattern, cflags);
	if (errcode != 0) {
		regerror(errcode, &preg, errbuf, sizeof(errbuf));
		fprintf(stdout, "Bad Pattern: %s", errbuf);
		regfree(&preg);
		exit(EXIT_FAILURE);
	}

	do {
		retp = fgets(line, line_len, in);
		if (retp == NULL) {
			break;
		} else if (line[0] != '\0') {
			line[strlen(line)-1] = '\0';
		}

		lineno++;

		errcode = (fixed_mode) ? tcstrstr(line, pattern, icase) : regexec(&preg, line, nmatch, pmatch, eflags); 
		if (errcode == 0) {
			if (just_count == 0) {
				if (show_lineno) {
					if (colors == COLOR_MODE_ALWAYS) {
						fprintf(stdout, "%s", COLOR_BRIGHT_CYAN);
					}
					fprintf(stdout, "%lu:", lineno);
					if (colors == COLOR_MODE_ALWAYS) {
						fprintf(stdout, "%s", COLOR_RESET);
					}
				}
				if (colors == COLOR_MODE_ALWAYS) {
					fprintf(stdout, "%s", COLOR_BRIGHT_WHITE);
				}
				fprintf(stdout, "%s\n", line);
				if (colors == COLOR_MODE_ALWAYS) {
					fprintf(stdout, "%s", COLOR_RESET);
				}
			}
			count++;
		}
	} while (!feof(in) && !ferror(in));

	if (just_count == 1) {
		if (colors == COLOR_MODE_ALWAYS) {
			fprintf(stdout, "%s", COLOR_BRIGHT_GREEN);
		}
		fprintf(stdout, "%lu\n", count);
		if (colors == COLOR_MODE_ALWAYS) {
			fprintf(stdout, "%s", COLOR_RESET);
		}
	}

	fclose(in);
	regfree(&preg);

	exit(EXIT_SUCCESS);
}

