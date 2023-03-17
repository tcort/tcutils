 /*
    wc -- counts lines, words, and characters and prints the results
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

struct counts {
	int bytes;
	int lines;
	int words;
};

static void count(int fd, struct counts *count, struct counts *total) {

	int c;
	int inword;

	inword = count->bytes = count->words = count->lines = 0;
	while ((c = tc_getc(fd)) != TC_EOF) {
		if (c == '\n') {
			count->lines++;
		}
		if (c == '\n' || c == '\t' || c == ' ') {
			inword = 0;
		} else if (inword == 0) {
			inword = 1;
			count->words++;
		}
		count->bytes++;
	}

	total->bytes += count->bytes;
	total->lines += count->lines;
	total->words += count->words;
}

static void show(int fd, char *filename, struct counts *count, int flag_c, int flag_l, int flag_w) {
	int first;

	if (flag_c == 0 && flag_l == 0 && flag_w == 0) {
		flag_c = 1;
		flag_l = 1;
		flag_w = 1;
	}

	if (flag_l == 1) {
		tc_putdec(fd, count->lines, 8);
		first = 1;
	}

	if (first == 1) {
		tc_putc(fd, TC_BLANK);
	}

	if (flag_w == 1) {
		tc_putdec(fd, count->words, 8);
		first = 1;
	}

	if (first == 1) {
		tc_putc(fd, TC_BLANK);
	}

	if (flag_c == 1) {
		tc_putdec(fd, count->bytes, 8);
		first = 1;
	}

	if (filename != TC_NULL) {
		tc_putc(fd, TC_BLANK);
		tc_puts(fd, filename);
		first = 1;
	}

	tc_putc(fd, TC_NEWLINE);
}

int main(int argc, char *argv[]) {

	int ch;
	int flag_c;
	int flag_l;
	int flag_w;
	int i;

	struct counts current;
	struct counts total;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'c', .longarg = "bytes", .description = "include character count in output", .has_value = 0 },
		TC_PROG_ARG_HELP,
		{ .arg = 'l', .longarg = "lines", .description = "include line count in output", .has_value = 0 },
		TC_PROG_ARG_VERSION,
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
	tc_memset((char *) &current, '\0', sizeof(struct counts));
	tc_memset((char *) &total, '\0', sizeof(struct counts));

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

	if (argc == 0) {
		count(TC_STDIN, &current, &total);
		show(TC_STDOUT, TC_NULL, &current, flag_c, flag_l, flag_w);
	} else {
		for (i = 0; i < argc; i++) {
			int fd;

			fd = tc_open_reader(argv[i]);
			if (fd == -1) {
				tc_puterr("Could not open file: ");
				tc_puterrln(argv[i]);
				tc_exit(TC_EXIT_FAILURE);
			}
			/* only count regular files */
			if (tc_is_file(fd)) {
				count(fd, &current, &total);
				show(TC_STDOUT, argv[i], &current, flag_c, flag_l, flag_w);
			}
			tc_close(fd);

		}
		if (argc > 1) {
			show(TC_STDOUT, "total", &total, flag_c, flag_l, flag_w);
		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}
