 /*
    uniq -- show or hide duplicate lines in a file
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
#include "tc/ctype.h"
#include "tc/math.h"
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct op {
	FILE *input;
	FILE *output;
	size_t nskip_fields;
	size_t nskip_chars;
	int unique_only;
	int duplicate;
	int show_count;
	int ignore_case;
	int delimiter;
	char pd[4];
} op_t;

static char *skip_fields(char *x, size_t nskip_fields) {
	size_t n, i = 0;

	/* eat whitespace until first field */
	while (x[i] != '\0' && tc_isspace(x[i])) {
		i++;
	}

	/* skip over the fields */
	for (n = 1; x[i] != '\0' && n <= nskip_fields; n++) {
		/* eat field contents */
		while (x[i] != '\0' && !tc_isspace(x[i])) {
			i++;
		}
		/* eat whitespace until next field */
		while (x[i] != '\0' && tc_isspace(x[i])) {
			i++;
		}
		n++;
	}

	return (x+i);
}

static int duplicate(char *x, char *y, int ignore_case, size_t nskip_fields, size_t nskip_chars) {
	size_t i;

	/* skip over fields in both lines if needed */
	if (nskip_fields > 0) {
		x = skip_fields(x, nskip_fields);
		y = skip_fields(y, nskip_fields);
	}

	/* not the same length? definitely unique */
	if (tc_strlen(x) != tc_strlen(y)) {
		return 0;
	}

	/* compare each character until a mismatch or done */
	for (i = nskip_chars; i < tc_strlen(x); i++) {
		if (ignore_case) {
			if (tc_tolower(x[i]) != tc_tolower(y[i])) {
				return 0;
			}
		} else {
			if (x[i] != y[i]) {
				return 0;
			}
		}
	}

	return 1;
}

static void chomp(char *s, int delimiter) {
	int i;
	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] == delimiter) {
			s[i] = '\0';
			break;
		}
	}
}

static void show(FILE *o, char *s, size_t c, int show_count, int delimiter) {
	if (show_count) {
		fprintf(o, "%4ld %s%c", c, s, delimiter);
	} else {
		fprintf(o, "%s%c", s, delimiter);
	}
}

static void unique(op_t *op) {
	char *first = TC_NULL, *line = TC_NULL, *last = TC_NULL;
	size_t size = 0, sizelast=0, c=1;
	ssize_t len, lenlast;
	
	lenlast = getdelim(&last, &sizelast, op->delimiter, op->input);
	if (lenlast == -1) return;
	chomp(last, op->delimiter);

	while ((len = getdelim(&line, &size, op->delimiter, op->input)) != -1) {
		chomp(line, op->delimiter);
		if (duplicate(line, last, op->ignore_case, op->nskip_fields, op->nskip_chars)) {
			c++;
			if (first == TC_NULL) {
				first = strdup(last);
			}
		} else {
			if (op->unique_only && first == TC_NULL) {
				show(op->output, last, c, op->show_count, op->delimiter);
			} else if (op->duplicate&& first != TC_NULL) {
				show(op->output, first, c, op->show_count, op->delimiter);
			} else if (!op->duplicate&& !op->unique_only) {
				show(op->output, first == TC_NULL ? last : first, c, op->show_count, op->delimiter);
			}
			c = 1;
			if (first != TC_NULL) {
				free(first);
				first = TC_NULL;
			}
		}

		free(last);
		last = strdup(line);
	}

	if (op->unique_only && first == TC_NULL) {
		show(op->output, last, c, op->show_count, op->delimiter);
	} else if (op->duplicate && first != TC_NULL) {
		show(op->output, first, c, op->show_count, op->delimiter);
	} else if (!op->duplicate && !op->unique_only) {
		show(op->output, first == TC_NULL ? last : first, c, op->show_count, op->delimiter);
	}

	if (line != TC_NULL) free(line);
	if (last != TC_NULL) free(last);
}

int main(int argc, char *argv[]) {

	int ch;

	op_t op = {
		stdin,
		stdout,
		0,
		0,
		0,
		0,
		0,
		0,
		'\n',
		{
			'\0',
			'\0',
			'\0',
			'\0'
		}
	};

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "cdf:hiuVs:z", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'c':
				op.show_count = 1;
				break;
			case 'd':
				op.duplicate = 1;
				break;
			case 'i':
				op.ignore_case = 1;
				break;
			case 'u':
				op.unique_only = 1;
				break;
			case 's':
				op.nskip_chars = (size_t) tc_abs(atoi(optarg));
				break;
			case 'f':
				op.nskip_fields = (size_t) tc_abs(atoi(optarg));
				break;
			case 'z':
				op.delimiter = '\0';
				break;
			case 'h':
				fprintf(stdout, "uniq -- show or hide duplicate lines in a file\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: uniq [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # collapse duplicate lines from foo.txt\n");
				fprintf(stdout, "  uniq foo.txt\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "uniq (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	/* optional input file */
	if (argc > 0) {
		op.input = fopen(argv[0], "r");
		if (op.input == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}

		/* optional output file */
		if (argc > 1) {
			op.output = fopen(argv[1], "r");
			if (op.output == TC_NULL) {
				perror("fopen");
				fclose(op.input);
				tc_exit(TC_EXIT_FAILURE);
			}
		}
	}

	unique(&op);

	fflush(op.output);
	fclose(op.output);
	fclose(op.input);

	tc_exit(TC_EXIT_SUCCESS);
	perror("exit");
	return 1;
}
