 /*
    tmpl -- template processor
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

#define C '}'
#define O '{'
#define L 2048
#define D '$'
#define Y '\\'
#define S '%'

int main(int argc, char *argv[]) {

	int ch;
	char s[L];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "tmpl -- template processor\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: tmpl [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # render a template into html\n");
				fprintf(stdout, "  tmpl < foo.tmpl > foo.html\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "tmpl (tcutils) v1.0.0\n");
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

	while ((ch = getc(stdin)) != EOF) {
		int e;
		if (ch == D || ch == S) {
			int d;
			d = getc(stdin);
			if (d == O) {
				FILE *p;
				int h,i;

				for (i = 0; i < L-1; i++) {
					s[i] = (char) getc(stdin);
					if (s[i] == EOF || s[i] == C) {
						break;
					}
				}
				s[i] = '\0';

				if ((p = popen(s, "r")) == NULL) {
					perror("popen");
					break;
				}
				while ((h = getc(p)) != EOF) {
					if (h == '\n') {
						int n;
						n = getc(p);
						if (n == EOF) {
							break;
						} else {
							ungetc(n, p);
						}
					}
					if (ch == S) {
						printf("&#%d;", h);
					} else {
						putc(h, stdout);
					}
				}
				pclose(p);
				continue;
			} else if (d == EOF) {
				break;
			}


			putc(ch, stdout);
			ch = d;
		}
		if (ch == Y) {
			e = ch;
			ch = getc(stdin);
			if (!(ch == D || ch == S)) {
				ungetc(ch, stdin);
				ch = e;
			}
		}
		putc(ch, stdout);
	}

	return EXIT_SUCCESS;
}
