 /*
    nanoid - generate URL friendly unique ID strings
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
#include "tc/sys.h"
#include "tc/version.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

const int length = 21;
char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-";

int main(int argc, char *argv[]) {

	int i;
	int rc;

	FILE *fp;
	uint8_t random_bytes[length];
	size_t len;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "nanoid", .description = "show a new nanoid" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "nanoid",
		.usage = "[OPTIONS]",
		.description = "generate URL friendly unique ID strings",
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


	fp = fopen("/dev/urandom", "r");
	if (fp == TC_NULL) {
		tc_exit(TC_EXIT_FAILURE);
	}
	len = fread(random_bytes, sizeof(random_bytes[0]), sizeof(random_bytes)/sizeof(random_bytes[0]), fp);
	if (len != sizeof(random_bytes)/sizeof(random_bytes[0])) {
		tc_exit(TC_EXIT_FAILURE);
	}
	rc = fclose(fp);
	if (rc == -1) {
		tc_exit(TC_EXIT_FAILURE);
	}

	for (i = 0; i < length; i++) {
		fprintf(stdout, "%c", alphabet[random_bytes[i] % 64]);
	}

	/* done */
	fprintf(stdout, "\n");

	tc_exit(TC_EXIT_SUCCESS);
}
