 /*
    ls -- list the contents of a directory
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
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
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

static void tc_time(time_t time, char *out) {
	struct tm *tm;
	tm = localtime(&time);
	strftime(out, 32, "%Y-%m-%d %H:%M:%S", tm);
}

static int select_all(const struct dirent *dentry) {
	(void) dentry;
	return 1;
}

static int select_non_hidden(const struct dirent *dentry) {
	return (dentry->d_name[0] != '.');
}

int main(int argc, char *argv[]) {

	int i;
	int j;
	int ch;
	int rc;
	int ndentries;
	int maxdentrylen;
	int dentries_per_row;
	int flag_a;
	int flag_1;
	int flag_l;
	struct dirent **dentries;
	struct passwd *passwd;
	struct group *group;
	struct stat st;
	char *dir;
	char modestring[16];
	char sizestring[16];
	char mtimestring[32];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	flag_1 = 0;
	flag_a = 0;
	flag_l = 0;

	while ((ch = getopt_long(argc, argv, "1ahlV", long_options, NULL)) != -1) {
		switch (ch) {
			case '1':
				flag_1 = 1;
				break;
			case 'a':
				flag_a = 1;
				break;
			case 'l':
				flag_l = 1;
				break;
			case 'h':
				fprintf(stdout, "ls -- list the contents of a directory\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: ls [OPTIONS] [DIR]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -1             print 1 filename per line\n");
				fprintf(stdout, "  -a             print all files (including hidden files)\n");
				fprintf(stdout, "  -l             list files attributes (mode, owner, group, etc)\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # list all files in the user's home directory\n");
				fprintf(stdout, "  ls -a ${HOME}\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # list detailed file attributes for all files in /tmp\n");
				fprintf(stdout, "  ls -al /tmp\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # list the contents of /etc, one filename per line\n");
				fprintf(stdout, "  ls -1 /etc\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # list the contents of the current directory\n");
				fprintf(stdout, "  ls\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "ls (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	dir = (argc > 0) ? argv[0] : ".";

	rc = scandir(dir, &dentries, flag_a > 0 ? select_all : select_non_hidden, alphasort);
	if (rc == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}
	ndentries = rc;

	maxdentrylen = 1; /* compute max filename length */
	for (i = 0; i < ndentries; i++) {
		int dentrylen;
		struct dirent *dentry;
		dentry = dentries[i];

		dentrylen = strlen(dentry->d_name);
		maxdentrylen = dentrylen > maxdentrylen ? dentrylen : maxdentrylen;
	}
	dentries_per_row = 80 / (maxdentrylen + 1);

	for (i = 0; i < ndentries; i++) {
		char path[PATH_MAX+1];
		struct dirent *dentry;
		dentry = dentries[i];

		memset(path, '\0', sizeof(char) * (PATH_MAX+1));

		snprintf(path, PATH_MAX, "%s/%s", dir, dentry->d_name);

		rc = stat(path, &st);
		if (rc == -1) {
			perror("stat");
			exit(EXIT_FAILURE);
		}

		passwd = getpwuid(st.st_uid);
		if (passwd == NULL) {
			perror("getpwuid");
			exit(EXIT_FAILURE);
		}

		group = getgrgid(st.st_gid);
		if (group == NULL) {
			perror("getgrgid");
			exit(EXIT_FAILURE);
		}

		tc_strmode(st.st_mode, modestring);
		tc_bytes(st.st_size, sizestring);
		tc_time(st.st_mtime, mtimestring);

		if (flag_1) {
			fprintf(stdout, "%s\n", dentry->d_name);
		} else if (flag_l) {
			fprintf(stdout, "%s  %s\t%s\t%s  %s  %s\n", modestring, passwd->pw_name, group->gr_name, sizestring, mtimestring, dentry->d_name);
		} else {
			fprintf(stdout, "%s", dentry->d_name);
			for (j = strlen(dentry->d_name); j < maxdentrylen + 1; j++) {
				fprintf(stdout, " ");
			}
			if (i % dentries_per_row == dentries_per_row - 1 || i + 1 == ndentries) {
				fprintf(stdout, "\n");
			}
		}

		free(dentry);
	}

	free(dentries);


	exit(EXIT_SUCCESS);
}
