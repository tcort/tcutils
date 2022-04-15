 /*
    touch -- change file timestamps
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

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch, fd, rc, i;
	mode_t mode;
	struct timespec times[2];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "touch -- change file timestamps\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: touch [OPTIONS] FILE...\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # create empty file foo.txt\n");
				fprintf(stdout, "  touch foo.txt\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "touch (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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


	if (argc == 0) {
		fprintf(stderr, "usage: touch [OPTIONS] FILE...\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	mode = 0666 & ~umask(0);

	clock_gettime(CLOCK_REALTIME, &times[0]);
	memcpy(&times[1], &times[0], sizeof(struct timespec));

	for (i = 0; i < argc; i++) {

		fd = open(argv[i], O_WRONLY | O_CREAT, mode);
		if (fd == -1) {
			perror("open");
			tc_exit(TC_EXIT_FAILURE);
		}

		rc = futimens(fd, times);
		if (rc == -1) {
			perror("futimens");
			close(fd);
			tc_exit(TC_EXIT_FAILURE);
		}

		close(fd);

	}


	tc_exit(TC_EXIT_SUCCESS);
}
