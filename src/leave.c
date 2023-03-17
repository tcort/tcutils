 /*
    leave -- kindly reminds the user to leave
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
	tty = tc_ttyname(TC_STDOUT);
	if (tty == TC_NULL) {
		tc_exit(TC_EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[]) {

	char buf[BUFSZ];
	char *when;
	int relative, i;
	int hhmm;
	int hh, mm, ss;
	pid_t pid;
	time_t now, leave;
	struct tm *t;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "leave +1700", .description = "set leave reminders to leave at 5pm" },
		{ .command = "leave +0030", .description = "set leave reminders to leave in 30 minutes" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "leave",
		.usage = "[OPTIONS] [[+]hhmm]",
		.description = "kindly reminds the user to leave",
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

	if (argc > 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	if (argc == 0) {
		fprintf(stdout, "When do you have to leave? ");
		fflush(stdout);
		when = fgets(buf, BUFSZ, stdin);
		if (when == TC_NULL || when[0] == '\n') {
			tc_exit(TC_EXIT_SUCCESS);
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

	now = time(TC_NULL);
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
			tc_exit(TC_EXIT_FAILURE);
		}
		ss = leave - now;
	}


	pid = fork();
	if (pid < 0) {
		perror("fork");
		tc_exit(TC_EXIT_FAILURE);
	} else if (pid > 0) {
		tc_memset(buf, '\0', BUFSZ);
		strftime(buf, BUFSZ-1, "%Y-%m-%d %H:%M:%S", t);
		fprintf(stdout, "Alarm set for %s (pid %d)\n", buf, pid);
		tc_exit(TC_EXIT_SUCCESS);
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

	tc_exit(TC_EXIT_SUCCESS);
}
