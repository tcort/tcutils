 /*
    leave -- kindly reminds the user to leave
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

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFSZ 64

#define MINUTE (60)
#define HOUR (3600)

static void exit_if_user_left(void) {
	char *tty;
	tty = ttyname(STDIN_FILENO);
	if (tty == NULL) {
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[]) {

	int ch;
	char buf[BUFSZ];
	char *when;
	int relative, i;
	int hhmm;
	int hh, mm, ss;
	pid_t pid;
	time_t now, leave;
	struct tm *t;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
    				fprintf(stdout, "leave -- kindly reminds the user to leave\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: leave [OPTIONS] [[+]hhmm]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # set leave reminders to leave at 5pm\n");
				fprintf(stdout, "  leave 1700\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # set leave reminders to leave in 30 minutes\n");
				fprintf(stdout, "  leave +0030\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "leave (tcutils) v1.0.0\n");
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

	if (argc > 1) {
		fprintf(stderr, "usage: leave [OPTIONS] [[+]hhmm]\n");
		exit(EXIT_FAILURE);
	}

	if (argc == 0) {
		fprintf(stdout, "When do you have to leave? ");
		fflush(stdout);
		when = fgets(buf, BUFSZ, stdin);
		if (when == NULL || when[0] == '\n') {
			exit(EXIT_SUCCESS);
		}
	} else {
		when = argv[0];
	}

	if ((relative = (when[0] == '+')) == 1) {
		when++;
	}

	hhmm = atoi(when);

	mm = hhmm % 100;
	hh = (hhmm - mm) / 100;

	now = time(NULL);
	if (relative == 1) {
		ss = (hh * HOUR) + (mm * MINUTE);
		leave = now + ss;
		t = localtime(&leave);
	} else {
		t = localtime(&now);
		t->tm_hour = hh;
		t->tm_min = mm;
		t->tm_sec = 0;
		leave = mktime(t);
		if (leave <= now) {
			fprintf(stdout, "You're already late!\n");
			exit(EXIT_FAILURE);
		}
		ss = leave - now;
	}


	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		memset(buf, '\0', BUFSZ);
		strftime(buf, BUFSZ-1, "%Y-%m-%d %H:%M:%S", t);
		fprintf(stdout, "Alarm set for %s (pid %d)\n", buf, pid);
		exit(EXIT_SUCCESS);
	}

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	if (ss >= (5*MINUTE)) {
		sleep(ss - (5*MINUTE));
		exit_if_user_left();
		fprintf(stdout, "\aYou have to leave in 5 minutes\n");
		ss = (5*MINUTE);
	}


	if (ss >= (1*MINUTE)) {
		sleep(ss - (1*MINUTE));
		exit_if_user_left();
		fprintf(stdout, "\aYou have 1 minute left before you have to leave\n");
		ss = (1*MINUTE);
	}

	if (ss >= 0) {
		sleep(ss);
		exit_if_user_left();
		fprintf(stdout, "\aTime to leave\n");
		ss = 0;
	}

	for (i = 1; i <= 10; i++) {
		sleep(1*MINUTE);
		exit_if_user_left();
		fprintf(stdout, "\aYou are %d minute%s late!\n", i, i == 1 ? "" : "s"); 
	}

	fprintf(stdout, "\aI have given up trying to get you out the door on time\n");

	exit(EXIT_SUCCESS);
}
