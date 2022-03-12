 /*
    wc -- counts lines, words, and characters and prints the results
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch;
	int c;
	int inword;
	int chars;
	int words;
	int lines;
	int first;
	int flag_c;
	int flag_l;
	int flag_w;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "bytes", no_argument, 0, 'c' },
		{ "lines", no_argument, 0, 'l' },
		{ "words", no_argument, 0, 'w' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	flag_c = 0;
	flag_l = 0;
	flag_w = 0;
	first = 0;

	while ((ch = getopt_long(argc, argv, "chlwV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'c':
				flag_c = 1;
				break;
			case 'l':
				flag_l = 1;
				break;
			case 'w':
				flag_w = 1;
				break;
			case 'h':
				fprintf(stdout, "wc -- counts lines, words, and characters and prints the results\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: wc [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -c, --bytes    include character count in output\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -l, --lines    include line count in output\n");
				fprintf(stdout, "  -w, --words    include word count in output\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # count characters, words, and lines in foo.txt\n");
				fprintf(stdout, "  wc < foo.txt\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # count lines in foo.c\n");
				fprintf(stdout, "  wc -l < foo.c\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "wc (tcutils) v1.0.0\n");
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

	inword = chars = words = lines = 0;
	while ((c = getc(stdin)) != EOF) {
		if (c == '\n') {
			lines++;
		}
		if (c == '\n' || c == '\t' || c == ' ') {
			inword = 0;
		} else if (inword == 0) {
			inword = 1;
			words++;
		}
		chars++;

	}

	if (flag_c == 0 && flag_l == 0 && flag_w == 0) {
		flag_c = 1;
		flag_l = 1;
		flag_w = 1;
	}

	if (flag_l == 1) {
		fprintf(stdout, "%7d", lines);
		first = 1;
	}

	if (first == 1) {
		fprintf(stdout, " ");
	}

	if (flag_w == 1) {
		fprintf(stdout, "%7d", words);
		first = 1;
	}

	if (first == 1) {
		fprintf(stdout, " ");
	}

	if (flag_c == 1) {
		fprintf(stdout, "%7d", chars);
		first = 1;
	}

	fprintf(stdout, "\n");

	exit(EXIT_SUCCESS);
}
