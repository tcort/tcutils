 /*
    xkcdpass -- password generator based on the xkcd (diceware) method
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {


	unsigned seed;
	unsigned long c, i, j, n;
	FILE *dict = TC_NULL, *f;
	long *words = TC_NULL;
	size_t cap=0, dcap = 8192, dlen = 0, dinc = 8192, nel;
	ssize_t len = 0;
	int ch, d;
	char *a, *w, *ln = TC_NULL;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	a = TC_NULL; /* acrostic */
	c = 1;   /* count */
	d = ' '; /* delimiter */
	n = 4; /* nwords */
	w = strdup("/usr/share/dict/words"); /* word list */

	while ((ch = getopt_long(argc, argv, "hVa:c:d:n:w:", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'a': a = strdup(optarg); break;
			case 'c': c = strtoul(optarg, TC_NULL, 10); break;
			case 'd': d = optarg[0]; break;
			case 'n': n = strtoul(optarg, TC_NULL, 10); break;
			case 'w': free(w); w = strdup(optarg); break;
			case 'h':
				fprintf(stdout, "xkcdpass -- password generator based on the xkcd (diceware) method\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: xkcdpass [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -a ACROSTIC    attempt to make an acrostic passphrase\n");
				fprintf(stdout, "  -c COUNT       number of passphrases to generate\n");
				fprintf(stdout, "  -d DELIM       single character delimiter between words\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -n NWORDS      number of words per passphrase\n");
				fprintf(stdout, "  -w WORDLIST    path to custom wordlist\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # generate 10 passphrases of 5 words each\n");
				fprintf(stdout, "  xkcdpass -c 10 -n 5\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "xkcdpass (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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

	if (a != TC_NULL) { /* with acrostic, nwords must equal len of acrostic */
		n = tc_strlen(a);
		for (i = 0; i < tc_strlen(a); i++) {
			a[i] = (char) tc_tolower(a[i]);
		}
	}

	f = fopen("/dev/urandom", "r");
	if (f == TC_NULL) {
		perror("fopen");
		free(w);
		w = TC_NULL;
		tc_exit(TC_EXIT_FAILURE);
	}
	nel = fread(&seed, sizeof(unsigned), 1, f);
	if (nel != 1) {
		perror("fread");
		fclose(f);
		free(w);
		w = TC_NULL;
		tc_exit(TC_EXIT_FAILURE);
	}
	fclose(f);

	tc_srand(seed);

	dict = fopen(w, "r");
	if (dict == TC_NULL) {
		perror("fopen");
		fprintf(stderr, "Could not open /usr/share/dict/words\n");
		fprintf(stderr, "To use an alternative dictionary do:\n");
		fprintf(stderr, "xkcdpass -w /path/to/dict\n");
		free(w);
		w = TC_NULL;
		tc_exit(TC_EXIT_FAILURE);
	}

	words = (long *) malloc(sizeof(long) * dcap);
	words[dlen++] = 0;
	while ((len = getline(&ln, &cap, dict)) > 0) {
		if (dlen == dcap) {
			dcap += dinc;
			words = (long *) realloc(words, sizeof(long)*dcap);
			if (words == TC_NULL) {
				perror("realloc");
				free(ln);
				tc_exit(TC_EXIT_FAILURE);
			}
		}
		words[dlen++] = ftell(dict);
	}

	for (i = 0; i < c; i++) {
		printf("> ");
		for (j = 0; j < n; j++) {
			unsigned r = (unsigned) tc_rand() % (dlen-1);
			fseek(dict, words[r], SEEK_SET);
			len = getline(&ln, &cap, dict);
			if (len <= 0) {
				break;
			}

			if (a != TC_NULL && ln[0] != a[j]) {
				j--;
				continue;
			}

			if (ln[tc_strlen(ln)-1] == '\n') ln[tc_strlen(ln)-1]='\0';
			if (ln[tc_strlen(ln)-1] == '\r') ln[tc_strlen(ln)-1]='\0';
			if (strstr(ln, "'") != TC_NULL) *(strstr(ln, "'")) = '\0';
			printf("%s", ln);
			if (j + 1 < n) {
				printf("%c", d);
			}
		}
		printf("\n");
	}

	free(w);
	free(words);
	fclose(dict);
	if (a != TC_NULL) free(a);
	if (ln != TC_NULL) free(ln);
	tc_exit(TC_EXIT_SUCCESS);
}
