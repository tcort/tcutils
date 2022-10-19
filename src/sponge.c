 /*
    sponge -- soak up standard input and write to a file
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

int main(int argc, char *argv[]) {

	int ch, i, rc;
	int *fp;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "ps | sponge foo.txt bar.txt", .description = "write the results of ps to foo.txt and bar.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "sponge",
		.usage = "[OPTIONS] FILE...",
		.description = "soak up standard input and write to a file",
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

	if (argc == 0) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	fp = (int *) tc_malloc(sizeof(int) * argc);
	if (fp == TC_NULL) {
		tc_puterrln("Out of Memory");
		tc_exit(TC_EXIT_FAILURE);
	}
	for (i = 0; i < argc; i++) {
		fp[i] = TC_ERR;
	}

	for (i = 0; i < argc; i++) {
		fp[i] = tc_open_writer(argv[i]);
		if (fp[i] == TC_ERR) {
			tc_puterr("Could not open file: ");
			tc_puterrln(argv[i]);
			for (i = 0; i < argc; i++) {
				if (fp[i] != TC_ERR) {
					tc_close(fp[i]);
				}
			}
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	while ((ch = tc_getc(TC_STDIN)) != TC_EOF) {
		for (i = 0; i < argc; i++) {
			tc_putc(fp[i], ch);
		}
	}

	for (i = 0; i < argc; i++) {
		tc_close(fp[i]);
	}
	fp = tc_free(fp);
	tc_exit(TC_EXIT_SUCCESS);
}
