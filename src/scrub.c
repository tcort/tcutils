 /*
    scrub -- overwrites files using the 35 pass Gutmann technique
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
#include "tc/stdlib.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct pass {
	enum method { RANDOM, PATTERN } method;
	uint8_t pattern[3];
};

/*
 * Pass schedule is from:
 *
 * 'Secure Deletion of Data from Magnetic and Solid-State Memory'
 *   by Peter Gutmann
 *
 * https://www.cs.auckland.ac.nz/~pgut001/pubs/secure_del.html
 */

static struct pass passes[35] = {
	{ RANDOM,  { 0x00, 0x00, 0x00 } },
	{ RANDOM,  { 0x00, 0x00, 0x00 } },
	{ RANDOM,  { 0x00, 0x00, 0x00 } },
	{ RANDOM,  { 0x00, 0x00, 0x00 } },
	{ PATTERN, { 0x55, 0x55, 0x55 } }, 
	{ PATTERN, { 0xAA, 0xAA, 0xAA } }, 
	{ PATTERN, { 0x92, 0x49, 0x24 } }, 
	{ PATTERN, { 0x49, 0x24, 0x92 } }, 
	{ PATTERN, { 0x24, 0x92, 0x49 } }, 
	{ PATTERN, { 0x00, 0x00, 0x00 } }, 
	{ PATTERN, { 0x11, 0x11, 0x11 } }, 
	{ PATTERN, { 0x22, 0x22, 0x22 } }, 
	{ PATTERN, { 0x33, 0x33, 0x33 } }, 
	{ PATTERN, { 0x44, 0x44, 0x44 } }, 
	{ PATTERN, { 0x55, 0x55, 0x55 } }, 
	{ PATTERN, { 0x66, 0x66, 0x66 } }, 
	{ PATTERN, { 0x77, 0x77, 0x77 } }, 
	{ PATTERN, { 0x88, 0x88, 0x88 } }, 
	{ PATTERN, { 0x99, 0x99, 0x99 } }, 
	{ PATTERN, { 0xAA, 0xAA, 0xAA } }, 
	{ PATTERN, { 0xBB, 0xBB, 0xBB } }, 
	{ PATTERN, { 0xCC, 0xCC, 0xCC } }, 
	{ PATTERN, { 0xDD, 0xDD, 0xDD } }, 
	{ PATTERN, { 0xEE, 0xEE, 0xEE } }, 
	{ PATTERN, { 0xFF, 0xFF, 0xFF } }, 
	{ PATTERN, { 0x92, 0x49, 0x24 } }, 
	{ PATTERN, { 0x49, 0x24, 0x92 } }, 
	{ PATTERN, { 0x24, 0x92, 0x49 } }, 
	{ PATTERN, { 0x6D, 0xB6, 0xDB } },
	{ PATTERN, { 0xB6, 0xDB, 0x6D } },
	{ PATTERN, { 0xDB, 0x6D, 0xB6 } },
	{ RANDOM,  { 0x00, 0x00, 0x00 } },
	{ RANDOM,  { 0x00, 0x00, 0x00 } },
	{ RANDOM,  { 0x00, 0x00, 0x00 } },
	{ RANDOM,  { 0x00, 0x00, 0x00 } }
};

#define NPASSES (sizeof(passes)/sizeof(passes[0]))

static void scrub(char *filename) {
	int ch, fd, rc, i, pati;
	FILE *fp;
	struct stat st;
	off_t pos;

	fp = fopen(filename, "r+");
	if (fp == TC_NULL) {
		perror("fopen");
		tc_exit(TC_EXIT_FAILURE);
	}

	fd = fileno(fp);
	if (fd == -1) {
		perror("fileno");
		fclose(fp);
		tc_exit(TC_EXIT_FAILURE);
	}

	rc = fstat(fd, &st);
	if (rc == -1) {
		perror("fstat");
		fclose(fp);
		tc_exit(TC_EXIT_FAILURE);
	}

	for (i = 0; i < NPASSES; i++) {
		rewind(fp);
		pati = 0;
		for (pos = 0; pos < st.st_size; pos++) {
			switch (passes[i].method) {
				case PATTERN:
					fputc(passes[i].pattern[pati], fp);
					pati = (pati + 1) % 3;
					break;

				case RANDOM:
				default:
					fputc(tc_rand(), fp);
					break;
			}
		}

		/* force a write to disk */
		fflush(fp);
		fsync(fd);
		sync();
	}

	fclose(fp);
}

int main(int argc, char *argv[]) {

	int i;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'h', .longarg = "help", .description = "print help text", .has_value = 0 },
		{ .arg = 'V', .longarg = "version", .description = "print version and copyright info", .has_value = 0 },
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "scrub foo.txt", .description = "overwrite foo.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "scrub",
		.usage = "[OPTIONS] FILE...",
		.description = "overwrites files using the 35 pass Gutmann technique",
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

	if (argc < 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	tc_srand((unsigned int) tc_getpid());
	for (i = 0; i < argc; i++) {
		scrub(argv[i]);
	}

	tc_exit(TC_EXIT_SUCCESS);
}
