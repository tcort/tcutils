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
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "ahmnrsVv", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "uname -- print system information (name and hardware details)\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: uname [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -a             print all fields\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -m             print machine hardware name\n");
				fprintf(stdout, "  -n             print node name\n");
				fprintf(stdout, "  -r             print kernel release\n");
				fprintf(stdout, "  -s             print kernel name\n");
				fprintf(stdout, "  -v             print kernel version\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show all details\n");
				fprintf(stdout, "  uname -a\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "uname (tcutils) v1.0.0\n");
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
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
				exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
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

	exit(EXIT_SUCCESS);
}
