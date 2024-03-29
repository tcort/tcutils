 /*
    du -- show disk usage
    Copyright (C) 2022, 2023, 2024  Thomas Cort

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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static int select_all_but_self_parent(const struct dirent *dentry) {
	return strcmp(dentry->d_name, "..") != 0 &&
		strcmp(dentry->d_name, ".") != 0;
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


static off_t visit(char *pathname) {

	FILE *fp;
	int fd, i, rc;
	struct stat stats;
	struct dirent **dentries;
	int ndentries;
	char bytes[32];

	fp = fopen(pathname, "r");
	if (fp == TC_NULL) {
		perror("fopen");
		return 0;
	}

	fd = fileno(fp);
	if (fd == -1) {
		perror("fileno");
		fclose(fp);
		return 0;
	}

	rc = fstat(fd, &stats);
	if (rc == -1) {
		perror("fstat");
		fclose(fp);
		return 0;
	}

	fclose(fp);

	if (S_ISDIR(stats.st_mode)) { /* directory? */
		off_t total;

		total = stats.st_size;

		rc = scandir(pathname, &dentries, select_all_but_self_parent, alphasort);
		if (rc == -1) {
			perror("scandir");
			return 0;
		}
		ndentries = rc;

		for (i = 0; i < ndentries; i++) {
			char *subpathname;
			struct dirent *dentry;
			dentry = dentries[i];

			subpathname = (char *) malloc(PATH_MAX+1);
			if (subpathname == TC_NULL) {
				perror("malloc");
				free(dentry);
				continue;
			}
			tc_memset(subpathname, '\0', PATH_MAX+1);
			snprintf(subpathname, PATH_MAX, "%s/%s", pathname, dentry->d_name);
			free(dentry);

			total += visit(subpathname);
			free(subpathname);
			subpathname = TC_NULL;

		}

		free(dentries);

		tc_bytes(total, bytes);
		fprintf(stdout, "%s\t%s\n", bytes, pathname);
		return total;
	} else {
		tc_bytes(stats.st_size, bytes);
		fprintf(stdout, "%s\t%s\n", bytes, pathname);
		return stats.st_size;
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
		{ .command = "show disk usage in /usr", .description = "show disk usage in /usr" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "du",
		.usage = "[OPTIONS] [PATHNAME]",
		.description = "show disk usage",
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

	visit(argc == 0 ? "." : argv[0]);
	tc_exit(TC_EXIT_SUCCESS);
}
