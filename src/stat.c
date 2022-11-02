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

    SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <tc/tc.h>

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

	tc_memset(&statbuf, '\0', sizeof(struct stat));

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

	int i;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "stat foo.txt bar.txt", .description = "show file system statistics for foo.txt and bar.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "stat",
		.usage = "[OPTIONS] FILE...",
		.description = "print file statistics",
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
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc == 0) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}


	for (i = 0; i < argc; i++) {
		show_stats(argv[i]);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
