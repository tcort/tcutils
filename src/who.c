 /*
    who -- list users currently logged in
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

#include "config.h"

#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <paths.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utmpx.h>

#if defined(_PATH_UTMPX)
#define TCUTMPX_FILE _PATH_UTMPX
#elif defined(UTMPX_FILE)
#define TCUTMPX_FILE UTMPX_FILE
#elif defined(UTMPX_FILENAME)
#define TCUTMPX_FILE UTMPX_FILE
#else
#define TCUTMPX_FILE "/var/run/utmpx"
#endif

int main(int argc, char *argv[]) {

	int ch, rc;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "who -- list users currently logged in\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: who [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show who is logged into the system\n");
				fprintf(stdout, "  who\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "who (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

#if defined(utmpxname)
	utmpxname(TCUTMPX_FILE);
#endif
	do {
		struct utmpx *ut;
		ut = getutxent();
		if (ut == NULL) {
			break;
		}
		if (ut->ut_type == USER_PROCESS) {
			time_t login_time = ut->ut_tv.tv_sec;
			struct tm *ts = localtime(&login_time);
			char login_ts[32];
			strftime(login_ts, 31, "%Y-%m-%d %H:%M:%S", ts);
			fprintf(stdout, "%s\t%s\t%s\t%s\n", ut->ut_user, ut->ut_line, login_ts, ut->ut_host);
		}

	} while (1);
	endutxent();

	exit(EXIT_SUCCESS);
}
