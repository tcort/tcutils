 /*
    uname -- print system information (name and hardware details)
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>

#define SFLAG (1<<0)
#define NFLAG (1<<1)
#define RFLAG (1<<2)
#define VFLAG (1<<3)
#define MFLAG (1<<4)

#define AFLAG (SFLAG|NFLAG|RFLAG|VFLAG|MFLAG)

int main(int argc, char *argv[]) {

	int rc;
	unsigned char flags = 0;
	struct utsname un;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'a', .longarg = "all", .description = "print all fields", .has_value = 0 },
		TC_PROG_ARG_HELP,
		{ .arg = 'm', .longarg = "machine", .description = "print machine hardware name", .has_value = 0 },
		{ .arg = 'n', .longarg = "nodename", .description = "print node name", .has_value = 0 },
		{ .arg = 'r', .longarg = "kernel-release", .description = "print kernel release", .has_value = 0 },
		{ .arg = 's', .longarg = "kernel-name", .description = "print kernel name", .has_value = 0 },
		{ .arg = 'v', .longarg = "kernel-version", .description = "print kernel version", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "uname -a", .description = "show all details" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "uname",
		.usage = "[OPTIONS]",
		.description = "print system information (name and hardware details)",
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
			case 'a':
				flags |= AFLAG;
				break;
			case 'm':
				flags |= MFLAG;
				break;
			case 'n':
				flags |= NFLAG;
				break;
			case 'r':
				flags |= RFLAG;
				break;
			case 's':
				flags |= SFLAG;
				break;
			case 'v':
				flags |= VFLAG;
				break;
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	if (flags == 0) { /* default when no options supplied */
		flags |= SFLAG;
	}


	argc -= argi;
	argv += argi;

	rc = uname(&un);
	if (rc == -1) {
		perror("uname");
		tc_exit(TC_EXIT_FAILURE);
	}

	if ((flags & SFLAG) == SFLAG) {
		fprintf(stdout, "%s ", un.sysname);
	}

	if ((flags & NFLAG) == NFLAG) {
		fprintf(stdout, "%s ", un.nodename);
	}

	if ((flags & RFLAG) == RFLAG) {
		fprintf(stdout, "%s ", un.release);
	}

	if ((flags & VFLAG) == VFLAG) {
		fprintf(stdout, "%s ", un.version);
	}

	if ((flags & MFLAG) == MFLAG) {
		fprintf(stdout, "%s ", un.machine);
	}

	fprintf(stdout, "\n");

	tc_exit(TC_EXIT_SUCCESS);
}
