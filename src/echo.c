 /*
    echo -- prints it's command line arguments
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	int i;
	int flag_n;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	flag_n = 0;

	while ((ch = getopt_long(argc, argv, "hnV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "echo -- prints it's command line arguments\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: echo [OPTIONS] [ARG...]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -n             omit trailing newline\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # greet the user\n");
				fprintf(stdout, "  echo \"Hello, ${USER}!\"\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print a message without a newline\n");
				fprintf(stdout, "  echo -n \"What is your name?\"\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print a newline\n");
				fprintf(stdout, "  echo\n");
				exit(EXIT_SUCCESS);
				break;
			case 'n':
				flag_n = 1;
				break;
			case 'V':
				fprintf(stdout, "echo (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	for (i = 0; i < argc; i++) {
		fprintf(stdout, "%s", argv[i]);
		if (i + 1 < argc) {
			fprintf(stdout, " ");
		}
	}

	if (flag_n == 0) {
		fprintf(stdout, "\n");
	}

	exit(EXIT_SUCCESS);
}
