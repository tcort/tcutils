 /*
    extract -- extract values from the input and print them to the output
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
#include "tc/ctype.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	FILE *in = stdin;
	regex_t preg;
	regmatch_t pmatch[32];
	size_t nmatch = sizeof(pmatch)/sizeof(pmatch[0]), nread = 0, len = 0;
	char *pattern = TC_NULL, *format = TC_NULL, errbuf[128], *line = TC_NULL, *endnum = TC_NULL;
	int cflags = REG_EXTENDED, eflags = 0, ch, errcode;
	ssize_t i;
	long placeholder = 0;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "ignore-case", no_argument, 0, 'i' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "Vhi", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "extract -- extract values from the input and print them to the output\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: extract [OPTIONS] PATTERN FORMAT [FILE]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help         print help text\n");
				fprintf(stdout, "  -i, --ignore-case  case insensitive search\n");
				fprintf(stdout, "  -V, --version      print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # extract failed login attmpts (uid + ip) from auth.log\n");
				fprintf(stdout, "  extract 'Invalid user (.*) from (.*) port' 'uid=$1 client_ip=$2' /var/log/auth.log\n");
				fprintf(stdout, "\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'i':
				cflags |= REG_ICASE;
				break;
			case 'V':
				fprintf(stdout, "extract (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	if (argc != 2 && argc != 3) {
		fprintf(stdout, "usage: extract [OPTIONS] PATTERN FORMAT [FILE]\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	pattern = argv[0];
	format = argv[1];
	if (argc == 3) {
		in = fopen(argv[2], "r");
		if (in == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	errcode = regcomp(&preg, pattern, cflags);
	if (errcode != 0) {
		regerror(errcode, &preg, errbuf, sizeof(errbuf));
		fprintf(stdout, "Bad Pattern: %s", errbuf);
		regfree(&preg);
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((nread = getline(&line, &len, in)) != -1) {
		line[nread-1] = '\0';

		errcode = regexec(&preg, line, nmatch, pmatch, eflags); 
		if (errcode == 0) {
			for (i = 0; i < strlen(format); i++) {
				if (format[i] == '$' && format[i+1] == '$') {
					fputc(format[i], stdout);
					i++;
				} else if (format[i] == '$' && tc_isdigit(format[i+1])) {
					placeholder = strtol(format + i + 1, &endnum, 10);
					i = endnum - 1 - format;
					if (placeholder >= 0 && placeholder < nmatch && pmatch[placeholder].rm_so != -1) {
						fwrite(line + pmatch[placeholder].rm_so, pmatch[placeholder].rm_eo - pmatch[placeholder].rm_so, 1, stdout);
					}
				} else {
					fputc(format[i], stdout);
				}
			}
			fputc('\n', stdout);
		}
	}

	fclose(in);
	regfree(&preg);
	if (line != TC_NULL) {
		free(line);
	}

	tc_exit(TC_EXIT_SUCCESS);
}

