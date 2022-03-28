 /*
    mkdir -- make directories
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
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static void make_directory(char *path, int ignore_exists) {
	int rc;

	rc = mkdir(path, 0777);
	if (rc == -1 && !(ignore_exists && errno == EEXIST)) {
		perror("mkdir");
		exit(EXIT_FAILURE);
	}
}

static void make_parents(char *path) {

	char *parent;

	path = strdup(path);
	parent = strdup(dirname(path));
	if (strcmp(".", parent) != 0 && strcmp("/", parent) != 0 && strcmp("", parent)) {
		make_parents(path);
		make_directory(parent, 1);
	}
	free(parent);
	free(path);

}

int main(int argc, char *argv[]) {

	int ch, rc, i, flag_p;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "parents", no_argument, 0, 'p' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	flag_p = 0;

	while ((ch = getopt_long(argc, argv, "hpV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "mkdir -- create directories\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: mkdir [OPTIONS] DIRNAME...\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -p, --parents  make all directories in path as needed\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # create empty directory foo\n");
				fprintf(stdout, "  mkdir foo\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # create empty directory bar and any other path components as needed\n");
				fprintf(stdout, "  mkdir -p foo/bar\n");
				exit(EXIT_SUCCESS);
				break;
			case 'p':
				flag_p = 1;
				break;
			case 'V':
				fprintf(stdout, "mkdir (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	if (argc == 0) {
		fprintf(stderr, "usage: mkdir [OPTIONS] DIRNAME...\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < argc; i++) {
		if (flag_p == 1) {
			make_parents(argv[i]);
		}
		make_directory(argv[i], 0);
	}


	exit(EXIT_SUCCESS);
}
