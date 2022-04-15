 /*
    chef -- Swedish Chef translator
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
#include "tc/string.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum location { FIRST, MIDDLE, LAST };

static enum location where(char *w, size_t i) {
	if (i == 0) return FIRST;
	else if (i == tc_strlen(w)-1) return LAST;
	else return MIDDLE;
}

static int match(char *word, size_t i, char *find) {
	return !strncmp(&word[i], find, tc_strlen(find));
}

static void tr(char *word) {
	size_t i;

	for (i = 0; i < tc_strlen(word); i++) {

		     if (match(word,i,"an")) { printf( "un"); i++; }		
		else if (match(word,i,"An")) { printf( "Un"); i++; }		
		else if (match(word,i,"au")) { printf( "oo"); i++; }		
		else if (match(word,i,"Au")) { printf( "Oo"); i++; }		
		else if (match(word,i,"a") && where(word,i)!=LAST) { printf("e"); }
		else if (match(word,i,"A") && where(word,i)!=LAST) { printf("E"); }
		else if (match(word,i,"en")&& where(word,i)==LAST) { printf("ee"); i++; }
		else if (match(word,i,"ew")&& where(word,i)!=FIRST) { printf("oo"); i++; }
		else if (match(word,i,"e") && where(word,i)==LAST) { printf("e-a"); }
		else if (match(word,i,"e") && where(word,i)==FIRST) { printf("i"); }
		else if (match(word,i,"E") && where(word,i)==FIRST) { printf("I"); }
		else if (match(word,i,"f") && where(word,i)!=FIRST) { printf("ff"); }
		else if (match(word,i,"ir") && where(word,i)!=FIRST) { printf("ur"); i++; }
		else if (match(word,i,"the")) { printf( "zee"); i+=2; }	
		else if (match(word,i,"The")) { printf( "Zee"); i+=2; }	
		else if (match(word,i,"th")&& where(word,i)==LAST) { printf("t"); i++; }
		else if (match(word,i,"tion") && where(word,i)!=FIRST) { printf("shun"); i+=3; }
		else if (match(word,i,"u") && where(word,i)!=FIRST) { printf("oo"); }
		else if (match(word,i,"U") && where(word,i)!=FIRST) { printf("Oo"); }
		else if (match(word,i,"v")) { printf( "f"); }		
		else if (match(word,i,"V")) { printf( "F"); }		
		else if (match(word,i,"w")) { printf( "v"); }		
		else if (match(word,i,"W")) { printf( "V"); }		
		else tc_putc(TC_STDOUT, word[i]);
	}
}


int main(int argc, char *argv[]) {

	int ch, i;
	char buf[2048];

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "chef -- Swedish Chef translator\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: chef [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # translate foo.txt to chef-speak\n");
				fprintf(stdout, "  chef < foo.txt > bar.txt\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "chef (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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


	for (i = 0; (ch = getc(stdin)) != EOF && i < sizeof(buf) - 1; i++) {
		if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')
							|| ch == '\'') {
			buf[i] = (char) ch;
		} else if (i == 0) {
			tc_putc(TC_STDOUT, ch);
			i--;
			continue;
		} else {
			ungetc(ch, stdin);
			buf[i] = '\0';
			tr(buf);
			i = -1;
		}
	}

	return 0;


	tc_exit(TC_EXIT_SUCCESS);
}
