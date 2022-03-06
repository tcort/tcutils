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

#include <getopt.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {


	unsigned seed;
	unsigned long c, i, j, n;
	FILE *dict = NULL, *f;
	long *words = NULL;
	size_t cap=0, dcap = 8192, dlen = 0, dinc = 8192, nel;
	ssize_t len = 0;
	int ch, d;
	char *a, *w, *ln = NULL;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	/* defaults */
	a = NULL; /* acrostic */
	c = 1;   /* count */
	d = ' '; /* delimiter */
	n = 4; /* nwords */
	w = strdup("/usr/share/dict/words"); /* word list */

	while ((ch = getopt_long(argc, argv, "hVa:c:d:n:w:", long_options, NULL)) != -1) {
		switch (ch) {
			case 'a': a = strdup(optarg); break;
			case 'c': c = strtoul(optarg, NULL, 10); break;
			case 'd': d = optarg[0]; break;
			case 'n': n = strtoul(optarg, NULL, 10); break;
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
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "xkcdpass (tcutils) v1.0.0\n");
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

	if (a != NULL) { /* with acrostic, nwords must equal len of acrostic */
		n = strlen(a);
		for (i = 0; i < strlen(a); i++) {
			a[i] = (char) tolower(a[i]);
		}
	}

	f = fopen("/dev/urandom", "r");
	if (f == NULL) {
		perror("fopen");
		free(w);
		w = NULL;
		exit(EXIT_FAILURE);
	}
	nel = fread(&seed, sizeof(unsigned), 1, f);
	if (nel != 1) {
		perror("fread");
		fclose(f);
		free(w);
		w = NULL;
		exit(EXIT_FAILURE);
	}
	fclose(f);

	srand(seed);

	dict = fopen(w, "r");
	if (dict == NULL) {
		perror("fopen");
		fprintf(stderr, "Could not open /usr/share/dict/words\n");
		fprintf(stderr, "To use an alternative dictionary do:\n");
		fprintf(stderr, "xkcdpass -w /path/to/dict\n", argv[0]);
		free(w);
		w = NULL;
		exit(EXIT_FAILURE);
	}

	words = (long *) malloc(sizeof(long) * dcap);
	words[dlen++] = 0;
	while ((len = getline(&ln, &cap, dict)) > 0) {
		if (dlen == dcap) {
			dcap += dinc;
			words = (long *) realloc(words, sizeof(long)*dcap);
			if (words == NULL) {
				perror("realloc");
				free(ln);
				exit(EXIT_FAILURE);
			}
		}
		words[dlen++] = ftell(dict);
	}

	for (i = 0; i < c; i++) {
		printf("> ");
		for (j = 0; j < n; j++) {
			unsigned r = (unsigned) rand() % (dlen-1);
			fseek(dict, words[r], SEEK_SET);
			len = getline(&ln, &cap, dict);
			if (len <= 0) {
				break;
			}

			if (a != NULL && ln[0] != a[j]) {
				j--;
				continue;
			}

			if (ln[strlen(ln)-1] == '\n') ln[strlen(ln)-1]='\0';
			if (ln[strlen(ln)-1] == '\r') ln[strlen(ln)-1]='\0';
			if (strstr(ln, "'") != NULL) *(strstr(ln, "'")) = '\0';
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
	if (a != NULL) free(a);
	if (ln != NULL) free(ln);
	exit(EXIT_SUCCESS);
}
