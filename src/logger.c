 /*
    logger -- add an entry in the system logs
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#define MSGSZ 2048
#define USAGE "[-h] [-v] [-i] [-s] [-c] [-t tag] MESSAGE..."

int main(int argc, char *argv[]) {

	size_t msgsize;
	int i;
	int ch;
	char *tag;
	int logopt;
	int facility;
	int level;
	char message[MSGSZ];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	tag = tc_getlogin();
	logopt = 0;
	facility = LOG_USER;
	level = LOG_INFO;
	message[0] = '\0';

	while ((ch = getopt_long(argc, argv, "chist:V", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'c':
				logopt |= LOG_CONS;
				break;
			case 'i':
				logopt |= LOG_PID;
				break;
			case 's':
				logopt |= LOG_PERROR;
				break;
			case 't':
				tag = optarg;
				break;
			case 'h':
    				fprintf(stdout, "logger -- add an entry in the system logs\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: logger [OPTIONS] MESSAGE...\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -c             print the log message to the system console\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -i             include PID with the log message\n");
				fprintf(stdout, "  -s             print the log message to STDERR as well\n");
				fprintf(stdout, "  -t tag         identifier prepended to the log message\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # log an application error for foobard, including PID\n");
				fprintf(stdout, "  logger -i -t foobard 'failed to authenticate user Alice'\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "logger (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
				fprintf(stdout, "Copyright (C) 2022, 2023  Thomas Cort\n");
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

	if (argc == 0) {
		fprintf(stderr, "usage: logger [OPTIONS] MESSAGE...\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	tc_memset(message, '\0', MSGSZ);
	msgsize = 1 /* NUL byte */;
	for (i = 0; i < argc; i++) {
		msgsize += tc_strlen(argv[i]);
		if (msgsize >= MSGSZ) {
			fprintf(stderr, "Message too long\n");
			tc_exit(TC_EXIT_FAILURE);
		}
		strncat(message, argv[i], MSGSZ - msgsize);
		if (i + 1 < argc) {
			msgsize += 1;
			if (msgsize >= MSGSZ) {
				fprintf(stderr, "Message too long\n");
				tc_exit(TC_EXIT_FAILURE);
			}
			strncat(message, " ", MSGSZ - msgsize);
		}
	}

	openlog(tag, logopt, facility);
	syslog(level, "%s", message);
	closelog();

	tc_exit(TC_EXIT_SUCCESS);
}
