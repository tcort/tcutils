 /*
    mktemp -- create a temporary file or directory with a unique name
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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch, dflag, fd, qflag;
	char *tmpdir, *tp;
	char template[] = "tmp.XXXXXX";
	char path[PATH_MAX];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "directory", no_argument, 0, 'd' },
		{ "tmpdir", required_argument, 0, 'p' },
		{ "quiet", no_argument, 0, 'q' },
		{ 0, 0, 0, 0 }
	};

	tmpdir = getenv("TMPDIR");
	if (tmpdir == NULL) {
		tmpdir = "/tmp";
	}

	dflag = 0;
	qflag = 0;

	while ((ch = getopt_long(argc, argv, "dhVpq", long_options, NULL)) != -1) {
		switch (ch) {
			case 'd':
				dflag = 1;
				break;
			case 'h':
				fprintf(stdout, "mktemp -- create a temporary file or directory with a unique name\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: mktemp [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -d, --directory  create a directory instead of a file\n");
				fprintf(stdout, "  -h, --help       print help text\n");
				fprintf(stdout, "  -q, --quiet      surppress error diagnostics\n");
				fprintf(stdout, "  -p, --tmpdir DIR use a base directory other than $TMPDIR\n");
				fprintf(stdout, "  -V, --version    print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # create a temporary file\n");
				fprintf(stdout, "  mktemp\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # create a temporary directory\n");
				fprintf(stdout, "  mktemp -d\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "mktemp (tcutils) v1.0.0\n");
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
				break;
			case 'p':
				tmpdir = optarg;
				break;
			case 'q':
				qflag = 1;
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	memset(path, '\0', PATH_MAX);
	snprintf(path, PATH_MAX - 1, "%s/%s", tmpdir, template);

	if (dflag == 1) {
		tp = mkdtemp(path);
		if (tp == NULL) {
			if (qflag == 0) {
				perror("mkdtemp");
			}
			exit(EXIT_FAILURE);
		}
	} else {
		fd = mkstemp(path);
		if (fd == -1) {
			if (qflag == 0) {
				perror("mkstemp");
			}
			exit(EXIT_FAILURE);
		}
		close(fd);
	}

	fprintf(stdout, "%s\n", path);

	exit(EXIT_SUCCESS);
}