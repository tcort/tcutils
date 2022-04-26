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

#include "tc/args.h"
#include "tc/const.h"
#include "tc/string.h"
#include "tc/stdio.h"
#include "tc/sys.h"
#include "tc/version.h"

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

		     if (match(word,i,"an")) { tc_puts(TC_STDOUT, "un"); i++; }
		else if (match(word,i,"An")) { tc_puts(TC_STDOUT, "Un"); i++; }
		else if (match(word,i,"au")) { tc_puts(TC_STDOUT, "oo"); i++; }
		else if (match(word,i,"Au")) { tc_puts(TC_STDOUT, "Oo"); i++; }
		else if (match(word,i,"a") && where(word,i)!=LAST) { tc_puts(TC_STDOUT, "e"); }
		else if (match(word,i,"A") && where(word,i)!=LAST) { tc_puts(TC_STDOUT, "E"); }
		else if (match(word,i,"en")&& where(word,i)==LAST) { tc_puts(TC_STDOUT, "ee"); i++; }
		else if (match(word,i,"ew")&& where(word,i)!=FIRST) { tc_puts(TC_STDOUT, "oo"); i++; }
		else if (match(word,i,"e") && where(word,i)==LAST) { tc_puts(TC_STDOUT, "e-a"); }
		else if (match(word,i,"e") && where(word,i)==FIRST) { tc_puts(TC_STDOUT, "i"); }
		else if (match(word,i,"E") && where(word,i)==FIRST) { tc_puts(TC_STDOUT, "I"); }
		else if (match(word,i,"f") && where(word,i)!=FIRST) { tc_puts(TC_STDOUT, "ff"); }
		else if (match(word,i,"ir") && where(word,i)!=FIRST) { tc_puts(TC_STDOUT, "ur"); i++; }
		else if (match(word,i,"the")) { tc_puts(TC_STDOUT, "zee"); i+=2; }
		else if (match(word,i,"The")) { tc_puts(TC_STDOUT, "Zee"); i+=2; }
		else if (match(word,i,"th")&& where(word,i)==LAST) { tc_puts(TC_STDOUT, "t"); i++; }
		else if (match(word,i,"tion") && where(word,i)!=FIRST) { tc_puts(TC_STDOUT, "shun"); i+=3; }
		else if (match(word,i,"u") && where(word,i)!=FIRST) { tc_puts(TC_STDOUT, "oo"); }
		else if (match(word,i,"U") && where(word,i)!=FIRST) { tc_puts(TC_STDOUT, "Oo"); }
		else if (match(word,i,"v")) { tc_puts(TC_STDOUT, "f"); }
		else if (match(word,i,"V")) { tc_puts(TC_STDOUT, "F"); }
		else if (match(word,i,"w")) { tc_puts(TC_STDOUT, "v"); }
		else if (match(word,i,"W")) { tc_puts(TC_STDOUT, "V"); }
		else tc_putc(TC_STDOUT, word[i]);
	}
}


int main(int argc, char *argv[]) {

	int ch, i;
	char buf[TC_LINEMAX];
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'h', .longarg = "help", .description = "print help text", .has_value = 0 },
		{ .arg = 'V', .longarg = "version", .description = "print version and copyright info", .has_value = 0 },
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "chef < foo.txt > bar.txt", .description = "translate foo.txt to chef-speak" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "chef",
		.usage = "[OPTIONS]",
		.description = "Swedish Chef translator",
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

	tc_exit(TC_EXIT_SUCCESS);
}
