 /*
    loc -- counts lines of code
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
#include "tc/sys.h"
#include "tc/version.h"

#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct counts {
	int files;
	int lines;
};

static int count_lines(char *pathname) {

	FILE *fp;
	int lines = 0;
	char *line = TC_NULL;
	size_t linecap = 0;
	ssize_t linelen;

	fp = fopen(pathname, "r");
	if (fp == TC_NULL) {
		return 0;
	}

	while ((linelen = getline(&line, &linecap, fp)) > 0) {
		lines += 1;
	}

	free(line);
	line = TC_NULL;
	fclose(fp);

	return lines;
}

static int should_count(char *pathname) {

	regex_t preg;
	regmatch_t pmatch[1];
	size_t nmatch = 1;
	char *pattern;
	char errbuf[128];
	int errcode;
	int cflags = 0;
	int eflags = 0;

	cflags |= REG_ICASE;
	cflags |= REG_EXTENDED;

	pattern = "\\.(asm|bas|c|coffee|cpp|cxx|erl|go|h|hpp|hxx|java|js|lol|p|pas|pl|py|rb|ts)$";

	errcode = regcomp(&preg, pattern, cflags);
	if (errcode != 0) {
		regerror(errcode, &preg, errbuf, sizeof(errbuf));
		fprintf(stdout, "Bad Pattern: %s", errbuf);
		regfree(&preg);
		return 0;
	}

	errcode = regexec(&preg, pathname, nmatch, pmatch, eflags);

	regfree(&preg);

	return (errcode == 0);
}

static int select_non_hidden(const struct dirent *dentry) {
	return (dentry->d_name[0] != '.');
}

static void visit(char *pathname, struct counts *totals) {

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

	if (S_ISREG(stats.st_mode)) { /* file? */
		int lines;

		if (should_count(pathname)) {

			lines = count_lines(pathname);
			fprintf(stdout, "%s contains %d lines of code\n", pathname, lines);

			totals->lines += lines;
			totals->files += 1;
		}
	}

	fclose(fp);

	if (S_ISDIR(stats.st_mode)) { /* directory? */
		rc = scandir(pathname, &dentries, select_non_hidden, alphasort);
		if (rc == -1) {
			perror("scandir");
			return;
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
			memset(subpathname, '\0', PATH_MAX+1);
			snprintf(subpathname, PATH_MAX, "%s/%s", pathname, dentry->d_name);
			free(dentry);

			visit(subpathname, totals);
			free(subpathname);
			subpathname = TC_NULL;

		}

		free(dentries);
	}


}

int main(int argc, char *argv[]) {

	int ch;
	char *pathname;
	struct counts totals;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "loc -- counts lines of code\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: loc [OPTIONS] [PATHNAME]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # count lines of code in foo.c\n");
				fprintf(stdout, "  loc foo.c\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # lines of code in 'src' and subdirectories\n");
				fprintf(stdout, "  loc src\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "loc (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	pathname = argc == 0 ? "." : argv[0];

	memset(&totals, '\0', sizeof(struct counts));
	visit(pathname, &totals);

	fprintf(stdout, "%d line%s in %d file%s\n", totals.lines, totals.lines == 1 ? "" : "s", totals.files, totals.files == 1 ? "" : "s");

	tc_exit(TC_EXIT_SUCCESS);
}
