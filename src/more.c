 /*
    more -- filter for paging through text, one screen at a time
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


static int fd = TC_STDIN;
static struct termios old_termios;

static void rawoff(void) {
	tcsetattr(fd, TCSAFLUSH, &old_termios);
	tc_close(fd);
}

static void rawon(void) {
	struct termios new_termios;

	tcgetattr(fd, &old_termios);
	atexit(rawoff);

	new_termios = old_termios;
	new_termios.c_lflag &= ~(ECHO|ICANON);
	tcsetattr(fd, TCSAFLUSH, &new_termios);
}


int main(int argc, char *argv[]) {

	int ch;
	char *tty;
	size_t lineno;
	FILE *f;
	char *line = TC_NULL;
	size_t cap = 0;
	ssize_t len = 0;
	struct winsize ws;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "ls | more", .description = "look a the listing of a large directory" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "more",
		.usage = "[OPTIONS] [FILE]",
		.description = "filter for paging through text, one screen at a time",
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

	if (argc == 1) {
		f = fopen(argv[0], "r");
		if (f == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}
	} else if (argc == 0) {
		f = stdin;
	} else {
		fprintf(stdout, "usage: more [OPTIONS] [FILE]\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	tty = tc_ttyname(TC_STDOUT);
	if (tty == NULL) {
		fd = TC_STDIN;
	} else {
		fd = tc_open_reader(tty);
		if (fd == TC_ERR) {
			fd = TC_STDIN;
		}
	}

	rawon();

	for (lineno = 0; !feof(f) && !ferror(f); lineno++) {
		len = getline(&line, &cap, f);
		if (len <= 0) {
			break;
		}

		fprintf(stdout, "%s", line);

		/* look up height of terminal */
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

		if (lineno % ws.ws_row == ws.ws_row - 2) {
			char ch;
			ch = tc_getc(fd);
			if (ch == 'q' || ch == 'Q') {
				break;
			}
			lineno = 0;
		}
	}
	free(line);
	fclose(f);

	tc_exit(TC_EXIT_SUCCESS);
}
