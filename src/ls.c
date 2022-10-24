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

#include <tc/tc.h>

#include <errno.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
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

static int select_almost_all(const struct dirent *dentry) {
	return (
		!tc_streql(dentry->d_name, ".")
	&& 
		!tc_streql(dentry->d_name, "..")
	);
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
	int flag_1;
	int flag_l;
	int flag_G;
	struct dirent **dentries;
	struct passwd *passwd;
	struct group *group;
	struct stat st;
	struct winsize ws;
	char *dir;
	char modestring[16];
	char sizestring[16];
	char mtimestring[32];
	int (*selector)(const struct dirent *dentry);

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = '1', .longarg = "one", .description = "print 1 filename per line", .has_value = 0 },
		{ .arg = 'a', .longarg = "all", .description = "print all files (including hidden files)", .has_value = 0 },
		{ .arg = 'A', .longarg = "almost-all", .description = "print all files (including hidden files) except '.' and '..'", .has_value = 0 },
		{ .arg = 'G', .longarg = "colourize", .description = "colourize output", .has_value = 0 },
		{ .arg = 'l', .longarg = "list", .description = "list files attributes (mode, owner, group, etc)", .has_value = 0 },
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "ls -a ${HOME}", .description = "list all files in the user's home directory" },
		{ .command = "ls -al /tmp", .description = "ist detailed file attributes for all files in /tmp" },
		{ .command = "ls -1 /etc", .description = "list the contents of /etc, one filename per line" },
		{ .command = "ls", .description = "list the contents of the current directory" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "ls",
		.usage = "[OPTIONS]",
		.description = "list the contents of a directory",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	selector = select_non_hidden;
	flag_1 = 0;
	flag_l = 0;
	flag_G = 0;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case '1':
				flag_1 = 1;
				break;
			case 'a':
				selector = select_all;
				break;
			case 'A':
				selector = select_almost_all;
				break;
			case 'l':
				flag_l = 1;
				break;
			case 'G':
				flag_G = 1;
				break;
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

	dir = (argc > 0) ? argv[0] : ".";

	rc = scandir(dir, &dentries, selector, alphasort);
	if (rc == -1) {
		perror("scandir");
		tc_exit(TC_EXIT_FAILURE);
	}
	ndentries = rc;

	maxdentrylen = 1; /* compute max filename length */
	for (i = 0; i < ndentries; i++) {
		int dentrylen;
		struct dirent *dentry;
		dentry = dentries[i];

		dentrylen = tc_strlen(dentry->d_name);
		maxdentrylen = dentrylen > maxdentrylen ? dentrylen : maxdentrylen;
	}
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); /* look up width of terminal */
	dentries_per_row = ws.ws_col / (maxdentrylen + 1);

	for (i = 0; i < ndentries; i++) {
		char path[PATH_MAX+1];
		struct dirent *dentry;
		dentry = dentries[i];

		memset(path, '\0', sizeof(char) * (PATH_MAX+1));

		snprintf(path, PATH_MAX, "%s/%s", dir, dentry->d_name);

		rc = stat(path, &st);
		if (rc == -1) {
			perror("stat");
			tc_exit(TC_EXIT_FAILURE);
		}

		passwd = getpwuid(st.st_uid);
		if (passwd == TC_NULL) {
			perror("getpwuid");
			tc_exit(TC_EXIT_FAILURE);
		}

		group = getgrgid(st.st_gid);
		if (group == TC_NULL) {
			perror("getgrgid");
			tc_exit(TC_EXIT_FAILURE);
		}

		tc_strmode(st.st_mode, modestring);
		tc_bytes(st.st_size, sizestring);
		tc_time(st.st_mtime, mtimestring);

		if (flag_G) {
			switch (modestring[0]) {
				case 'd': /* directory */
					fprintf(stdout, "%s", COLOUR_BRIGHT_BLUE);
					break;
				case 's': /* socket */
					fprintf(stdout, "%s", COLOUR_MAGENTA);
					break;
				case 'b': /* block device */
					fprintf(stdout, "%s", COLOUR_BRIGHT_YELLOW);
					break;
				case 'c': /* char device */
					fprintf(stdout, "%s", COLOUR_BRIGHT_YELLOW);
					break;
				case 'l': /* link */
					fprintf(stdout, "%s", COLOUR_CYAN);
					break;
				case 'p': /* fifo */
					fprintf(stdout, "%s", COLOUR_YELLOW);
					break;
				case '?': /* unknown */
					fprintf(stdout, "%s", COLOUR_BRIGHT_RED);
					break;
				case '-': /* regular file */
					if (modestring[3] == 'x' || modestring[6] == 'x' || modestring[9] == 'x') {
						fprintf(stdout, "%s", COLOUR_BRIGHT_GREEN);
					} else {
						fprintf(stdout, "%s", COLOUR_BRIGHT_WHITE);
					}
					break;
			}
		}

		if (flag_1) {
			fprintf(stdout, "%s\n", dentry->d_name);
		} else if (flag_l) {
			fprintf(stdout, "%s  %s\t%s\t%s  %s  %s\n", modestring, passwd->pw_name, group->gr_name, sizestring, mtimestring, dentry->d_name);
		} else {
			fprintf(stdout, "%s", dentry->d_name);
			for (j = tc_strlen(dentry->d_name); j < maxdentrylen + 1; j++) {
				fprintf(stdout, " ");
			}
			if (i % dentries_per_row == dentries_per_row - 1 || i + 1 == ndentries) {
				fprintf(stdout, "\n");
			}
		}

		if (flag_G) {
			fprintf(stdout, "%s", COLOUR_RESET);
		}

		free(dentry);
	}

	free(dentries);


	tc_exit(TC_EXIT_SUCCESS);
}
