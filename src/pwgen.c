 /*
    pwgen -- password generator
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

static const int alphabet_size = 0x40;
static const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-";

int main(int argc, char *argv[]) {

	unsigned seed;
	int fd;
	int i;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "pwgen", .description = "generate passwords" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "pwgen",
		.usage = "[OPTIONS]",
		.description = "password generator",
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

	fd = tc_open_reader("/dev/urandom");
	if (fd == TC_ERR) {
		tc_puterrln("Could not open /dev/urandom for reading");
		tc_exit(TC_EXIT_FAILURE);
	}
	((char*) &seed)[0] = tc_getc(fd);
	((char*) &seed)[1] = tc_getc(fd);
	((char*) &seed)[2] = tc_getc(fd);
	((char*) &seed)[3] = tc_getc(fd);
	tc_close(fd);
	tc_srand(seed);


	for (i = 0; i < 8; i++) {
		int n = tc_abs(tc_rand()) & (alphabet_size - 1);
		tc_putc(TC_STDOUT, alphabet[n]);
	}
	tc_putc(TC_STDOUT, TC_NEWLINE);

	tc_exit(TC_EXIT_SUCCESS);
}
