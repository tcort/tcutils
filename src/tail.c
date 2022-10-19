 /*
    tail -- prints lines at the end of an input text
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct line {
	char *text;
	size_t cap;
	ssize_t len;
};

static void tail(FILE *fin, FILE *fout, int n) {

	int i, j;
	struct line **buf;

	buf = (struct line **) malloc(sizeof(struct line*) * n);
	if (buf == TC_NULL) {
		perror("malloc");
		tc_exit(TC_EXIT_FAILURE);
	}
	memset(buf, '\0', sizeof(struct line*) * n);

	for (i = 0; i < n; i++) {
		buf[i] = (struct line *) malloc(sizeof(struct line));
		if (buf[i] == TC_NULL) {
			perror("malloc");
			for (j = 0; j < i; j++) {
				free(buf[j]->text);
				buf[j]->text = TC_NULL;
				free(buf[j]);
				buf[j] = TC_NULL;
			}
			free(buf);
			tc_exit(TC_EXIT_FAILURE);
		}
		memset(buf[i], '\0', sizeof(struct line));
	}

	i = 0;
	while ( (buf[i]->len = getline(&buf[i]->text, &buf[i]->cap, fin)) >= 0) {
		i = (i + 1) % n;
	}

	for (j = 0; j < n; j++) {
		fprintf(fout, "%s", buf[(i + j) % n]->text);
	}

	for (i = 0; i < n; i++) {
		free(buf[i]->text);
		buf[i]->text = TC_NULL;
		free(buf[i]);
		buf[i] = TC_NULL;
	}
	free(buf);
	buf = TC_NULL;

}

int main(int argc, char *argv[]) {

	int ch;
	int i;
	int flag_n;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'n', .longarg = "lines", .description = "number of lines to show (default 10)", .has_value = 1 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "tail foo.txt", .description = "print the last 10 lines of foo.txt" },
		{ .command = "tail -n 2 bar.txt", .description = "print the last 2 lines of bar.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "tail",
		.usage = "[OPTIONS] [FILE]",
		.description = "prints lines at the end of an input text",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_n = 10;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'n':
				flag_n = atoi(argval);
				flag_n = flag_n < 0 ? 10 : flag_n;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc == 0) {
		tail(stdin, stdout, flag_n);
		tc_exit(TC_EXIT_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		if (f == TC_NULL) {
			perror("fopen");
			tc_exit(TC_EXIT_FAILURE);
		}

		tail(f, stdout, flag_n);

		fclose(f);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
