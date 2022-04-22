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
 */

#include "tc/const.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>



static struct termios old_termios;

static void rawoff(void) {
	 tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

static void rawon(void) {
	struct termios new_termios;

	tcgetattr(STDIN_FILENO, &old_termios);
	atexit(rawoff);

	new_termios = old_termios;
	new_termios.c_lflag &= ~(ECHO|ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
}


int main(int argc, char *argv[]) {

	int ch;
	size_t lineno;
	FILE *f;
	char *line = TC_NULL;
	size_t cap = 0;
	ssize_t len = 0;
	struct winsize ws;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "more -- filter for paging through text, one screen at a time\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: more [OPTIONS] [FILE]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # look at the listing of a large directory\n");
				fprintf(stdout, "  ls | more\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "more (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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
			ch = getc(stdin);
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
