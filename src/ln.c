 /*
    ln -- creates a file link
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

#include <getopt.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch, rc, sflag = 0;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "symbolic", no_argument, 0, 's' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hsV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
    				fprintf(stdout, "ln -- creates a file link\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: ln [OPTIONS] FILE1 FILE2\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -s, --symbolic creates a symbolic link\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # create a link name bar.txt which links to existing file foo.txt\n");
				fprintf(stdout, "  ln foo.txt bar.txt\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "ln (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
				break;
			case 's':
				sflag = 1;
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	if (argc != 2) {
		fprintf(stderr, "usage: ln [OPTIONS] FILE1 FILE2\n");
		exit(EXIT_FAILURE);
	}

	if (sflag == 1) {
		rc = symlink(argv[0], argv[1]);
		if (rc == -1) {
			perror("symlink");
			exit(EXIT_FAILURE);
		}
	} else {
		rc = link(argv[0], argv[1]);
		if (rc == -1) {
			perror("link");
			exit(EXIT_FAILURE);
		}
	}

	exit(EXIT_SUCCESS);
}
