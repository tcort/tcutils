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

#include "tc/const.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
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
	char ch;
	unsigned char flags = 0;
	struct utsname un;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "all", no_argument, 0, 'a' },
		{ "machine", no_argument, 0, 'm' },
		{ "nodename", no_argument, 0, 'n' },
		{ "kernel-release", no_argument, 0, 'r' },
		{ "kernel-version", no_argument, 0, 'v' },
		{ "kernel-name", no_argument, 0, 's' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "ahmnrsVv", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "uname -- print system information (name and hardware details)\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: uname [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -a, --all             print all fields\n");
				fprintf(stdout, "  -h, --help            print help text\n");
				fprintf(stdout, "  -m, --machine         print machine hardware name\n");
				fprintf(stdout, "  -n, --nodename        print node name\n");
				fprintf(stdout, "  -r, --kernel-release  print kernel release\n");
				fprintf(stdout, "  -s, --kernel-name     print kernel name\n");
				fprintf(stdout, "  -v, --kernel-version  print kernel version\n");
				fprintf(stdout, "  -V, --version         print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show all details\n");
				fprintf(stdout, "  uname -a\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "uname (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
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
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				tc_exit(TC_EXIT_FAILURE);
		}
	}

	if (flags == 0) { /* default when no options supplied */
		flags |= SFLAG;
	}

	argc -= optind;
	argv += optind;

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
