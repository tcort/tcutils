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

#include "tc/const.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define UUID_VERSION ('4')
#define UUID_VARIANT ('b')

int main(int argc, char *argv[]) {

	int ch;
	int i;
	int rc;

	struct timeval tv;
	uint64_t ts;
	FILE *fp;
	uint8_t random_bytes[18];
	size_t len;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, TC_NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "uuidgen - COMB UUIDs (UUID v4 Variant B) generator\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: uuidgen [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # return a UUID\n");
				fprintf(stdout, "  uuidgen\n");
				tc_exit(TC_EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "uuidgen (%s) v%s\n", TC_VERSION_NAME, TC_VERSION_STRING);
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
