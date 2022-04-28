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

#include "tc/args.h"
#include "tc/const.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <errno.h>
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
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "who", .description = "show who is logged into the system" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "who",
		.usage = "[OPTIONS]",
		.description = "list users currently logged in",
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
			time_t login_time = ut->ut_tv.tv_sec;
			struct tm *ts = localtime(&login_time);
			char login_ts[32];
			strftime(login_ts, 31, "%Y-%m-%d %H:%M:%S", ts);
			fprintf(stdout, "%s\t%s\t%s\t%s\n", ut->ut_user, ut->ut_line, login_ts, ut->ut_host);
		}

	} while (1);
	endutxent();

	tc_exit(TC_EXIT_SUCCESS);
}
