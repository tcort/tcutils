 /*
    printenv -- print environment variables and their values
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
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char *argv[]) {

	int ch;
	size_t i;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "printenv -- print environment variables and their values\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: printenv [OPTIONS] [VARNAME]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show all environment variables\n");
				fprintf(stdout, "  printenv\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show just one environment variable's value\n");
				fprintf(stdout, "  printenv HOME\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "printenv (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	if (argc < 1) {
		for (i = 0; environ[i] != TC_NULL; i++) {
			fprintf(stdout, "%s\n", environ[i]);
		}
	} else {
		for (i = 0; environ[i] != TC_NULL; i++) {
			if (strncmp(environ[i], argv[0], tc_strlen(argv[0])) == 0) {
				char c;
				c = *(environ[i] + tc_strlen(argv[0]));
				if (c == '=') { 
					fprintf(stdout, "%s\n", environ[i] + tc_strlen(argv[0]) + 1);
				} else if (c == '\0') {
					fprintf(stdout, "%s\n", environ[i] + tc_strlen(argv[0]));
				}
			}
		}
	}	

	tc_exit(TC_EXIT_SUCCESS);
}
