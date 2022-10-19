 /*
    snek -- hungry snake simulation game
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

#include <tc/tc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

struct sk {
	size_t y;
	size_t x;
};

typedef struct sk sk_t;

int main(int argc, char *argv[]) {

	size_t i; /* i */
	size_t l; /* length */
	size_t d; /* direction */
	sk_t sk[2048];
	sk_t a;
	size_t my, mx;
	struct winsize ws;
	struct tc_prog_arg *arg;
	int ch;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "snek", .description = "launch the game" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "snek",
		.usage = "[OPTIONS]",
		.description = "hungry snake simulation game",
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

	tc_srand((unsigned int) tc_getpid());

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	my = ws.ws_row;
	mx = ws.ws_col;

	d = 2; l = 1;
	for (i = 0; i < 2048; i++) {
		sk[i].y = sk[i].x = 0;
	}
	sk[0].y = my/2;
	sk[0].x = mx/2;
	a.y = (tc_abs(tc_rand()) % (my-2)) + 1;
	a.x = (tc_abs(tc_rand()) % (mx-2)) + 1;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	nodelay(stdscr, TRUE);
	for (i = 0; i < my; i++) {
		mvaddch(i, 0, '#');
		mvaddch(i, mx-1, '#');
	}
	for (i = 0; i < mx; i++) {
		mvaddch(0, i, '#');
		mvaddch(my-1, i, '#');
	}	

	do {
		for (i = 0; i < 1000; i++) {
			ch = getch();

			switch(ch) {
				case KEY_UP:       d=0; break;
				case KEY_RIGHT:    d=1; break;
				case KEY_DOWN:     d=2; break;
				case KEY_LEFT:     d=3; break;
				case 'q':          goto done;
			}

			usleep(100);
		}

		for (i = l; i > 0; i--) {
			sk[i].y = sk[i-1].y;
			sk[i].x = sk[i-1].x;
		}
		mvaddch(sk[l].y, sk[l].x, ' ');

		switch (d) {
			case 0: sk[0].y--; break;
			case 1: sk[0].x++; break;
			case 2: sk[0].y++; break;
			case 3: sk[0].x--; break;
		}

		if (sk[0].y >= my - 1 || sk[0].x >= mx - 1 ||
			sk[0].y == 0 || sk[0].x == 0) goto done;

		for (i = 1; i < l; i++)
			if (sk[0].y == sk[i].y &&
				sk[0].x == sk[i].x) {
				goto done;
			}

		mvaddch(a.y, a.x, '+');

		for (i = 0; i < l; i++) mvaddch(sk[i].y, sk[i].x,'@');

		if (sk[0].y == a.y && sk[0].x == a.x) {
			l++;
			a.y = (tc_abs(tc_rand()) % (my-2)) + 1;
			a.x = (tc_abs(tc_rand()) % (mx-2)) + 1;
		}

		refresh();
	} while (1);

done:
	clear();
	endwin();

	printf("Score: %ld\n", l-1);

	return 0;
}
