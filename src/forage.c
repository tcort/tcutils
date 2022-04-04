 /*
    forage -- find files, directories and more using regular expressions
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
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <limits.h>
#include <regex.h>
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


static int matches(char *subject, char *pattern) {

	regex_t preg;
	regmatch_t pmatch[1];
	char errbuf[128];
	int cflags, eflags, errcode;
	size_t nmatch = 1;

	cflags = REG_EXTENDED;
	eflags = 0;

        errcode = regcomp(&preg, pattern, cflags);
        if (errcode != 0) {
		regerror(errcode, &preg, errbuf, sizeof(errbuf));
		fprintf(stdout, "Bad Pattern: %s", errbuf);
		regfree(&preg);
		exit(EXIT_FAILURE);
        }

	errcode = regexec(&preg, subject, nmatch, pmatch, eflags);
	regfree(&preg);
	return errcode == 0;
}

static void forage(char *pattern, char *pathname) {

	FILE *fp;
	int fd, i, rc;
	struct stat stats;
	struct dirent **dentries;
	int ndentries;

	fp = fopen(pathname, "r");
	if (fp == NULL) {
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

			subpathname = (char *) malloc(PATH_MAX+1);
			if (subpathname == NULL) {
				perror("malloc");
				free(dentry);
				continue;
			}
			memset(subpathname, '\0', PATH_MAX+1);
			snprintf(subpathname, PATH_MAX, "%s/%s", pathname, dentry->d_name);
			free(dentry);
			forage(pattern, subpathname);
			free(subpathname);
			subpathname = NULL;

		}

		free(dentries);
	}

	if (matches(basename(pathname), pattern)) {
		fprintf(stdout, "%s\n", pathname);
	}
}

int main(int argc, char *argv[]) {

	int ch;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "forage -- find files, directories and more using regular expressions\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: forage [OPTIONS] REGEX [BASEDIR]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # forage for files ending in .md\n");
				fprintf(stdout, "  forage '\\.md$'\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # forage for files ending in .c in /usr/src\n");
				fprintf(stdout, "  forage '\\.c$' /usr/src\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "forage (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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
		fprintf(stdout, "usage: forage [OPTIONS] REGEX [BASEDIR]\n");
		exit(EXIT_FAILURE);
	}

	forage(argv[0], argc > 1 ? argv[1] : ".");
	exit(EXIT_SUCCESS);
}
