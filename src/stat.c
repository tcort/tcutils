 /*
    stat -- print file statistics
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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static void tc_strmode(mode_t mode, char *out) {

	if (S_ISREG(mode)) {
		out[0] = '-';
	} else if (S_ISBLK(mode)) {
		out[0] = 'b';
	} else if (S_ISCHR(mode)) {
		out[0] = 'c';
	} else if (S_ISDIR(mode)) {
		out[0] = 'd';
	} else if (S_ISLNK(mode)) {
		out[0] = 'l';
	} else if (S_ISFIFO(mode)) {
		out[0] = 'p';
#ifdef S_ISSOCK
	} else if (S_ISSOCK(mode)) {
		out[0] = 's';
#endif
	} else {
		out[0] = '?';
	}

	out[1] = (mode & S_IRUSR) ? 'r' : '-';
	out[2] = (mode & S_IWUSR) ? 'w' : '-';
	out[3] = (mode & S_IXUSR) ? 'x' : '-';

	out[4] = (mode & S_IRGRP) ? 'r' : '-';
	out[5] = (mode & S_IWGRP) ? 'w' : '-';
	out[6] = (mode & S_IXGRP) ? 'x' : '-';

	out[7] = (mode & S_IROTH) ? 'r' : '-';
	out[8] = (mode & S_IWOTH) ? 'w' : '-';
	out[9] = (mode & S_IXOTH) ? 'x' : '-';

	out[10] = '\0';
}

static void tc_time(time_t time, char *out) {
	struct tm *tm;
	tm = localtime(&time);
	strftime(out, 32, "%Y-%m-%d %H:%M:%S", tm);
}

static void tc_bytes(off_t size, char *out) {

	double amount = (double) size;
	char units[6] = { 'B', 'K', 'M', 'G', 'T', 'P' };
	size_t unit = 0;

	while (amount > 1024 && unit < sizeof(units)/sizeof(units[0])) {
		amount /= 1024.0;
		unit++;
	}

	sprintf(out, "%7.1f%c", amount, units[unit]);

}

static void show_stats(char *pathname) {
	int rc;
	struct stat statbuf;
	char human_atime[32];
	char human_mtime[32];
	char human_ctime[32];
	char human_size[16];
	char human_mode[32];

	memset(&statbuf, '\0', sizeof(struct stat));

	rc = stat(pathname, &statbuf);
	if (rc == -1) {
		perror("stat");
		return;
	}

	tc_time(statbuf.st_atime, human_atime);
	tc_time(statbuf.st_mtime, human_mtime);
	tc_time(statbuf.st_ctime, human_ctime);

	tc_bytes(statbuf.st_size, human_size);

	tc_strmode(statbuf.st_mode, human_mode);

	fprintf(stdout, "       File: %s\n", pathname);
	fprintf(stdout, "       Size: %s\n", human_size);
	fprintf(stdout, "Permissions: %s\n", human_mode);
	fprintf(stdout, "   Accessed: %s\n", human_atime);
	fprintf(stdout, "   Modified: %s\n", human_mtime);
	fprintf(stdout, "    Changed: %s\n", human_ctime);
}

int main(int argc, char *argv[]) {

	int ch;
	int i;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "stat -- print file statistics\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: stat [OPTIONS] FILE...\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # stat foo.txt and bar.txt\n");
				fprintf(stdout, "  stat foo.txt bar.txt\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "stat (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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
		fprintf(stderr, "usage: stat [OPTIONS] FILE...\n");
		exit(EXIT_FAILURE);
	}


	for (i = 0; i < argc; i++) {
		show_stats(argv[i]);
	}

	exit(EXIT_SUCCESS);
}
