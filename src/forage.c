 /*
    forage -- find files, directories and more using regular expressions
    Copyright (C) 2022, 2023  Thomas Cort

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
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static int select_all_but_self_parent(const struct dirent *dentry) {
	return tc_streql(dentry->d_name, "..") != 1 &&
		tc_streql(dentry->d_name, ".") != 1;
}


static void forage(char *pattern, char *pathname) {

	FILE *fp;
	int fd, i, rc;
	struct stat stats;
	struct dirent **dentries;
	int ndentries;

	fp = fopen(pathname, "r");
	if (fp == TC_NULL) {
		perror("fopen");
		return;
	}

	fd = fileno(fp);
	if (fd == -1) {
		perror("fileno");
		fclose(fp);
		return;
	}

	rc = fstat(fd, &stats);
	if (rc == -1) {
		perror("fstat");
		fclose(fp);
		return;
	}

	fclose(fp);

	if (S_ISDIR(stats.st_mode)) { /* directory? */

		rc = scandir(pathname, &dentries, select_all_but_self_parent, alphasort);
		if (rc == -1) {
			perror("scandir");
			return;
		}
		ndentries = rc;

		for (i = 0; i < ndentries; i++) {
			char *subpathname;
			struct dirent *dentry;
			dentry = dentries[i];

			subpathname = (char *) tc_malloc(PATH_MAX+1);
			if (subpathname == TC_NULL) {
				perror("malloc");
				dentry = tc_free(dentry);
				continue;
			}
			tc_memset(subpathname, '\0', PATH_MAX+1);
			snprintf(subpathname, PATH_MAX, "%s/%s", pathname, dentry->d_name);
			dentry = tc_free(dentry);
			forage(pattern, subpathname);
			subpathname = tc_free(subpathname);

		}

		dentries = tc_free(dentries);
	}

	if (tc_match(basename(pathname), pattern)) {
		tc_putln(TC_STDOUT, pathname);
	}
}

int main(int argc, char *argv[]) {

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "forage '\\.md$'", .description = "forage for files ending in .md" },
		{ .command = "forage '\\.c$' /usr/src", .description = "forage for files ending in .c in /usr/src" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "forage",
		.usage = "[OPTIONS] REGEX [BASEDIR]",
		.description = "find files, directories and more using regular expressions",
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

	forage(argv[0], argc > 1 ? argv[1] : ".");
	tc_exit(TC_EXIT_SUCCESS);
}
