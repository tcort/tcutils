 /*
    uptime -- show how long the system has been up
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

#if defined(_PATH_WTMPX)
#define TCWTMPX_FILE _PATH_WTMPX
#elif defined(WTMPX_FILE)
#define TCWTMPX_FILE WTMPX_FILE
#elif defined(WTMPX_FILENAME)
#define TCWTMPX_FILE WTMPX_FILE
#else
#define TCWTMPX_FILE "/var/log/wtmpx"
#endif


int main(int argc, char *argv[]) {

	int ch;
	int rc;
	long nusers;
	double load[3];
	time_t now;
	struct tm *tm;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "uptime -- show how long the system has been up\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: uptime [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show uptime of the system\n");
				fprintf(stdout, "  uptime\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "uptime (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	now = time(TC_NULL);
	tm = localtime(&now);
	fprintf(stdout, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);

#if defined(utmpxname)
	utmpxname(TCUTMPX_FILE);
#endif

	do {
		struct utmpx *ut;
		ut = getutxent();
		if (ut == TC_NULL) {
			break;
		}
		if (ut->ut_type == BOOT_TIME) {
			long day, hour, min, sec;
			double seconds;
			time_t boot_time;

			boot_time = ut->ut_tv.tv_sec;
			seconds = difftime(now, boot_time);

			sec = (long) seconds;

			min = sec / 60;
			sec = sec % 60;

			hour = min / 60;
			min = min % 60;

			day = hour / 24;
			hour = hour % 24;

			fprintf(stdout, "up");

			if (day > 0) {
				fprintf(stdout, " %ldd", day);
			}

			if (hour > 0) {
				fprintf(stdout, " %ldh", hour);
			}

			if (min > 0) {
				fprintf(stdout, " %ldm", min);
			}

			if (sec > 0) {
				fprintf(stdout, " %lds", sec);
			}

			fprintf(stdout, ", ");
			break;
		}
	} while (1);
	endutxent();

	nusers = 0;
#if defined(utmpxname)
	utmpxname(TCUTMPX_FILE);
#endif
	do {
		struct utmpx *ut;
		ut = getutxent();
		if (ut == TC_NULL) {
			break;
		}
		if (ut->ut_type == USER_PROCESS) {
			nusers++;
		}

	} while (1);
	endutxent();
	fprintf(stdout, "%ld user%s, ", nusers, nusers == 1 ? "" : "s");

	rc = getloadavg(load, sizeof(load)/sizeof(load[0]));
	if (rc == -1) {
		perror("getloadavg");
		tc_exit(TC_EXIT_FAILURE);
	}
	fprintf(stdout, "load average: %.2f, %.2f, %.2f\n", load[0], load[1], load[2]);

	tc_exit(TC_EXIT_SUCCESS);
}
