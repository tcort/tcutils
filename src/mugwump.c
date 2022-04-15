 /*
    mugwump -- find all the mugwumps
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
#include "tc/stdlib.h"
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 10
#define NWUMPS 4

static char *line = TC_NULL;
static size_t cap = 0;
static ssize_t len = 0;

typedef struct point {
	int x;
	int y;
	int found;
} point_t;

static point_t scan_point(char *s) {
	size_t i;
	int n = 0;
	point_t p;

	p.x = p.y = -1;
	for (i = 0; i < tc_strlen(s); i++) {
		if (s[i] >= '0' && s[i] <= '9') {
			if (n == 0) {
				p.x = atoi(s+i); n++;
			} else {
				p.y = atoi(s+i);
			}
		}
	}

	return p;
}

static double sqr(double x) {
	return x*x;
}

static double sqrt(double x) {
	double y, z;
	y = (double) 0.5 * x;
	do {
		z = y;
		y = y - ((sqr(y)-x)/(2*y));
	} while ((int) (10000 * z) != (int) (10000 * y));
	return z;
}

static double pythag(double a, double b) {
	return sqrt(sqr(a) + sqr(b));
}

static double dist(point_t a, point_t b) {
	return pythag(a.x - b.x, a.y - b.y);
}

static void play(void) {
	int i, j, nfound;
	point_t player, mugwumps[NWUMPS];

	player.x = player.y = 0;
	for (i = 0; i < NWUMPS; i++) {
		mugwumps[i].x = tc_rand() % N;
		mugwumps[i].y = tc_rand() % N;
		mugwumps[i].found = 0;
	}

	for (i = 0; i < N; i++) {

		for (j = 0; j < NWUMPS; j++) {
			double d = dist(player, mugwumps[j]);
			if (mugwumps[j].found == 1) {
				fprintf(stdout, "found mugwump %d at %d,%d\n", j, mugwumps[j].x, mugwumps[j].y);
			} else {
				fprintf(stdout, "%f from mugwump %d\n", d, j);
			}
		}

		do {
			fprintf(stdout, "Enter your guess (x,y): ");
			len = getline(&line, &cap, stdin);
			if (len <= 0) {
				return;
			}

			player = scan_point(line);
			if (player.x >= 0 && player.x <= 9 && player.y >= 0 && player.y <= 9) {
				break;
			}

			fprintf(stdout, "Bad Move\n");
		} while (!feof(stdin) && !ferror(stdin));

		for (j = nfound = 0; j < NWUMPS; j++) {
			if (player.x == mugwumps[j].x && player.y == mugwumps[j].y) {
				mugwumps[j].found = 1;
			}
			nfound += (mugwumps[j].found == 1);
		}

		if (nfound == NWUMPS) {
			fprintf(stdout, "YOU WIN!\n");
			return;
		}
	}
	fprintf(stdout, "YOU LOSE\n");

}

static int done(void) {
	int ch;
	if (feof(stdin) || ferror(stdin)) {
		return 1;
	}
	do {
		fprintf(stdout, "Play again? (y/n) ");
		fflush(stdout);
		ch = getc(stdin);
	} while (ch != 'n' && ch != 'y' && ch != EOF);
	return ch == 'n';
}

static void kthxbye(void) {
	if (line != TC_NULL) {
		free(line);
		line = TC_NULL;
	}
}

int main(int argc, char *argv[]) {

	int ch;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "mugwump -- find all the mugwumps\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: mugwump [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # play the game\n");
				fprintf(stdout, "  mugwump\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "mugwump (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	atexit(kthxbye);

	srand((unsigned) getpid());

	do {
		play();
	} while (!done());

	return 0;
}
