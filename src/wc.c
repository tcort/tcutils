 /*
    wc -- counts lines, words, and characters and prints the results
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
#include "tc/sys.h"
#include "tc/version.h"

#include <stdio.h>

int main(int argc, char *argv[]) {

	int ch;
	int c;
	int inword;
	int chars;
	int words;
	int lines;
	int first;
	int flag_c;
	int flag_l;
	int flag_w;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'c', .longarg = "bytes", .description = "include character count in output", .has_value = 0 },
		{ .arg = 'h', .longarg = "help", .description = "print help text", .has_value = 0 },
		{ .arg = 'l', .longarg = "lines", .description = "include line count in output", .has_value = 0 },
		{ .arg = 'V', .longarg = "version", .description = "print version and copyright info", .has_value = 0 },
		{ .arg = 'w', .longarg = "words", .description = "include word count in output", .has_value = 0 },
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "wc < foo.txt", .description = "count characters, words, and lines in foo.txt" },
		{ .command = "wc -l < foo.c", .description = "count lines in foo.c" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "wc",
		.usage = "[OPTIONS]",
		.description = "counts lines, words, and characters and prints the results",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_c = 0;
	flag_l = 0;
	flag_w = 0;
	first = 0;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'c':
				flag_c = 1;
				break;
			case 'l':
				flag_l = 1;
				break;
			case 'w':
				flag_w = 1;
				break;
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

	inword = chars = words = lines = 0;
	while ((c = tc_getc(TC_STDIN)) != TC_EOF) {
		if (c == '\n') {
			lines++;
		}
		if (c == '\n' || c == '\t' || c == ' ') {
			inword = 0;
		} else if (inword == 0) {
			inword = 1;
			words++;
		}
		chars++;

	}

	if (flag_c == 0 && flag_l == 0 && flag_w == 0) {
		flag_c = 1;
		flag_l = 1;
		flag_w = 1;
	}

	if (flag_l == 1) {
		fprintf(stdout, "%7d", lines);
		first = 1;
	}

	if (first == 1) {
		fprintf(stdout, " ");
	}

	if (flag_w == 1) {
		fprintf(stdout, "%7d", words);
		first = 1;
	}

	if (first == 1) {
		fprintf(stdout, " ");
	}

	if (flag_c == 1) {
		fprintf(stdout, "%7d", chars);
		first = 1;
	}

	fprintf(stdout, "\n");

	tc_exit(TC_EXIT_SUCCESS);
}
