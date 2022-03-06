 /*
    spell -- checks the spelling of words
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
#include <errno.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct cell {
	char ch;
	char terminal;
	struct cell *children[128];
} cell_t;

cell_t *cell_new(char ch, char terminal);
void cell_define(cell_t *root, char *s);
int cell_defined(cell_t *root, char *s);
void cell_free(cell_t *root);


int check(char *file, cell_t *root, int nflag, int oflag);


#define DEFAULT_DICTIONARY "/usr/share/dict/words"

cell_t *dict_load(cell_t **root, char *dict);


int match(char *subject, char *pattern, char **result);



cell_t *cell_new(char ch, char terminal) {
	cell_t *cell = malloc(sizeof(cell_t));
	if (cell==NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cell->ch = ch;
	cell->terminal = terminal;
	memset(cell->children, '\0', sizeof(cell_t *) * 128);
	return cell;
}

void cell_define(cell_t *root, char *s) {
	size_t i;
	size_t len = strlen(s) - 1 /* skip newline */;
	cell_t *cell = root;

	for (i = 0; i < len; i++) {
		if (cell->children[(unsigned char)s[i]] == NULL) {
			cell->children[(unsigned char)s[i]] = cell_new(s[i], 0);
		}
		cell = cell->children[(unsigned char)s[i]];
	}
	cell->terminal = 1;
}

int cell_defined(cell_t *root, char *s) {
	size_t i;
	size_t len = strlen(s);
	cell_t * cell = root;

	for (i = 0; i < len; i++) {
		if (cell->children[(unsigned char)s[i]] == NULL) {
			return 0;
		}
		cell = cell->children[(unsigned char)s[i]];
	}
	return cell->terminal;
}

void cell_free(cell_t *root) {
	int i;
	for (i = 0; i < 128;  i++) {
		if (root->children[i] != NULL) {
			cell_free(root->children[i]);
		}
	}
	free(root);
}


int check(char *file, cell_t *root, int nflag, int oflag) {

	FILE *f;
	char *s = NULL;
	char *line = NULL;
	size_t len = 0;
	size_t lineno = 0;

	f = file == NULL ? stdin : fopen(file, "r");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	do {
		int rc = getline(&line, &len, f);
		if (rc == -1) {
			break;
		}
		s = line;
		lineno++;
		do {
			char *result;
			rc = match(s, "([A-Za-z0-9]([A-Za-z0-9'&.,;?:]*[A-Za-z0-9])?)", &result);
			if (rc <= 0) {
				break;
			}

			rc = cell_defined(root, result);
			if (rc == 0) {
				if (oflag) {
					fprintf(stdout, "%s:", file == NULL ? "<stdin>" : file);
				}
				if (nflag) {
					fprintf(stdout, "%lu:", lineno);
				}
				if (oflag || nflag) {
					fprintf(stdout, " ");
				}
				fprintf(stdout, "%s\n", result);
			}
			s += strlen(result) + 1;
			free(result);
			result = NULL;
		} while (*s != '\0');
	} while (!feof(f) && !ferror(f));
	free(line);
	fclose(f);

	return EXIT_SUCCESS;
}

cell_t *dict_load(cell_t **root, char *dict) {
	FILE *f;
	char *word = NULL;
	size_t len = 0;

	f = fopen(dict, "r");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	if (*root == NULL) {
		*root = cell_new('\0', 0);
		if (*root == NULL) {
			perror("cell_new");
			exit(EXIT_FAILURE);
		}
	}

	do {
		size_t x = 0;
		int notascii = 0;
		int rc = getline(&word, &len, f);
		if (rc == -1) {
			break;
		}
		for (x = 0; x < strlen(word); x++) {
			if (!isascii(word[x])) {
				notascii = 1;
				break;
			}
		}
		if (notascii) {
			continue;
		}
		cell_define(*root, word);

	} while (!feof(f) && !ferror(f));
	free(word);
	fclose(f);

	return *root;
}

int match(char *subject, char *pattern, char **result) {

	int		rc;
	size_t		len;
	regex_t		regex;
	regmatch_t	matches[1];

	if (subject == NULL || pattern == NULL) {
		return 0;
	}

	rc = regcomp(&regex, pattern, REG_EXTENDED);
	if (rc != 0) {
		return -1;
	}

	rc = regexec(&regex, subject, 1, matches, 0);
	if (rc != 0) {
		regfree(&regex);
		return 0;
	}

	if (matches[0].rm_so == -1) {
		return 0;
	}

	len = (size_t) matches[0].rm_eo - matches[0].rm_so;
	*result = (char *)malloc(len + 1);
	if (*result == NULL) {
		regfree(&regex);
		return -1;
	}
	bzero(*result, len + 1);
	strncpy(*result, subject + matches[0].rm_so, len);

	regfree(&regex);
	return 1;
}

int main(int argc, char *argv[]) {

	int ch;
	int dostdin = 1;
	char nflag = 0;
	char oflag = 0;
	char *dict = DEFAULT_DICTIONARY;
	int i = 0;
	cell_t *root = NULL;

	dict_load(&root, dict);

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "d:hnoV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'd':
				dict_load(&root, optarg);
				break;
			case 'h':
				fprintf(stdout, "usage: spell [OPTIONS] [FILENAME]\n");
				fprintf(stdout, "spell -- checks the spelling of words\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: spell [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -d /path/words specify the location of the dictionary\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -n             include line number in output\n");
				fprintf(stdout, "  -o             include filename in output\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # spellcheck the file foo.txt with %s\n", DEFAULT_DICTIONARY);
				fprintf(stdout, "  spell foo.txt\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # spellcheck the file foo.txt with a custom dictionary\n");
				fprintf(stdout, "  spell -d /home/jdoe/words foo.txt\n");
				exit(EXIT_SUCCESS);
				break;
			case 'n':
				nflag++;
				break;
			case 'o':
				oflag++;
				break;
			case 'V':
				fprintf(stdout, "spell (tcutils) v1.0.0\n");
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

	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '+') {
			dict_load(&root, argv[i]+1);
		} else {
			check(argv[i], root, nflag, oflag);
			dostdin = 0;
		}
	}

	if (dostdin) {
		check(NULL, root, nflag, oflag);
	}

	cell_free(root);

	exit(EXIT_SUCCESS);
}

