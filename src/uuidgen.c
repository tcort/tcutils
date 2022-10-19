 /*
    uuidgen - COMB UUIDs (UUID v4 Variant B) generator
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define UUID_VERSION ('4')
#define UUID_VARIANT ('b')

int main(int argc, char *argv[]) {

	int i;
	int rc;

	struct timeval tv;
	uint64_t ts;
	FILE *fp;
	uint8_t random_bytes[18];
	size_t len;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "uuidgen", .description = "show a new UUID" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "uuidgen",
		.usage = "[OPTIONS]",
		.description = "COMB UUIDs (UUID v4 Variant B) generator",
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


	rc = gettimeofday(&tv, TC_NULL);
	if (rc == -1) {
		tc_exit(TC_EXIT_FAILURE);
	}

	tv.tv_usec /= 10;			/* round to 10us resolution */
	ts = (tv.tv_sec * 100000) + tv.tv_usec; /* combine s + us */


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

	/* print timestamp portion '01234567-89ab' */
	for (i = 3; i <= 8; i++) {
		uint8_t byte = ((ts>>(64 - (i*8))) & 0xFF);
		fprintf(stdout, "%01x", (0xF0 & byte) >> 4);
		fprintf(stdout, "%01x", (0x0F & byte));
		if (i == 6) {
			fprintf(stdout, "-");
		}
	}

	/* rather than make this complicated, we get 18 random bytes and use
	 * the lower 4 bytes instead of getting 9 random bytes and using both
	 * low and high bits
	 */

	/* print '-' and version followed by 3 random hex digits */
	fprintf(stdout, "-%c", UUID_VERSION);
	for (i = 0; i < 3; i++) {
		fprintf(stdout, "%01x", (0x0F & random_bytes[i]));
	}

	/* print '-' and variant followed by 3 random hex digits */
	fprintf(stdout, "-%c", UUID_VARIANT);
	for (i = 3; i < 6; i++) {
		fprintf(stdout, "%01x", (0x0F & random_bytes[i]));
	}

	/* print '-' followed by 12 random hex digits */
	fprintf(stdout, "-");
	for (i = 6; i < 18; i++) {
		fprintf(stdout, "%01x", (0x0F & random_bytes[i]));
	}

	/* done */
	fprintf(stdout, "\n");

	tc_exit(TC_EXIT_SUCCESS);
}
