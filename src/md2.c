 /*
    md2 -- calculate a message-digest fingerprint for a file
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
#include <string.h>
#include <unistd.h>

typedef uint8_t byte;
typedef uint64_t length;

#define BLOCK_SIZE (16)
#define CHECKSUM_SIZE (16)
#define DIGEST_SIZE (48)
#define DIGEST_FINAL_SIZE (16)
#define NROUNDS (18)

const byte S[] = {
	0x29, 0x2E, 0x43, 0xC9, 0xA2, 0xD8, 0x7C, 0x01,
	0x3D, 0x36, 0x54, 0xA1, 0xEC, 0xF0, 0x06, 0x13,
	0x62, 0xA7, 0x05, 0xF3, 0xC0, 0xC7, 0x73, 0x8C,
	0x98, 0x93, 0x2B, 0xD9, 0xBC, 0x4C, 0x82, 0xCA, 
	0x1E, 0x9B, 0x57, 0x3C, 0xFD, 0xD4, 0xE0, 0x16,
	0x67, 0x42, 0x6F, 0x18, 0x8A, 0x17, 0xE5, 0x12, 
	0xBE, 0x4E, 0xC4, 0xD6, 0xDA, 0x9E, 0xDE, 0x49,
	0xA0, 0xFB, 0xF5, 0x8E, 0xBB, 0x2F, 0xEE, 0x7A, 
	0xA9, 0x68, 0x79, 0x91, 0x15, 0xB2, 0x07, 0x3F,
	0x94, 0xC2, 0x10, 0x89, 0x0B, 0x22, 0x5F, 0x21,
	0x80, 0x7F, 0x5D, 0x9A, 0x5A, 0x90, 0x32, 0x27,
	0x35, 0x3E, 0xCC, 0xE7, 0xBF, 0xF7, 0x97, 0x03, 
	0xFF, 0x19, 0x30, 0xB3, 0x48, 0xA5, 0xB5, 0xD1,
	0xD7, 0x5E, 0x92, 0x2A, 0xAC, 0x56, 0xAA, 0xC6, 
	0x4F, 0xB8, 0x38, 0xD2, 0x96, 0xA4, 0x7D, 0xB6,
	0x76, 0xFC, 0x6B, 0xE2, 0x9C, 0x74, 0x04, 0xF1, 
	0x45, 0x9D, 0x70, 0x59, 0x64, 0x71, 0x87, 0x20,
	0x86, 0x5B, 0xCF, 0x65, 0xE6, 0x2D, 0xA8, 0x02, 
	0x1B, 0x60, 0x25, 0xAD, 0xAE, 0xB0, 0xB9, 0xF6,
	0x1C, 0x46, 0x61, 0x69, 0x34, 0x40, 0x7E, 0x0F, 
	0x55, 0x47, 0xA3, 0x23, 0xDD, 0x51, 0xAF, 0x3A,
	0xC3, 0x5C, 0xF9, 0xCE, 0xBA, 0xC5, 0xEA, 0x26, 
	0x2C, 0x53, 0x0D, 0x6E, 0x85, 0x28, 0x84, 0x09,
	0xD3, 0xDF, 0xCD, 0xF4, 0x41, 0x81, 0x4D, 0x52, 
	0x6A, 0xDC, 0x37, 0xC8, 0x6C, 0xC1, 0xAB, 0xFA,
	0x24, 0xE1, 0x7B, 0x08, 0x0C, 0xBD, 0xB1, 0x4A, 
	0x78, 0x88, 0x95, 0x8B, 0xE3, 0x63, 0xE8, 0x6D,
	0xE9, 0xCB, 0xD5, 0xFE, 0x3B, 0x00, 0x1D, 0x39, 
	0xF2, 0xEF, 0xB7, 0x0E, 0x66, 0x58, 0xD0, 0xE4,
	0xA6, 0x77, 0x72, 0xF8, 0xEB, 0x75, 0x4B, 0x0A, 
	0x31, 0x44, 0x50, 0xB4, 0x8F, 0xED, 0x1F, 0x1A,
	0xDB, 0x99, 0x8D, 0x33, 0x9F, 0x11, 0x83, 0x14
};

void md2(byte *buf, length len) {

	byte digest[DIGEST_SIZE];
	byte checksum[CHECKSUM_SIZE];
	byte block[BLOCK_SIZE];
	byte l = 0;
	length i = 0, j = 0, t = 0, pos = 0;

	memset(digest,   '\0', DIGEST_SIZE);
	memset(checksum, '\0', CHECKSUM_SIZE);

	for (pos = 0; pos <= len; pos += BLOCK_SIZE) {

		const length pad = len - pos > BLOCK_SIZE ? 0 : BLOCK_SIZE - (len - pos);

		/* load block */

		for (i = 0; i < BLOCK_SIZE - pad; i++) {
			block[i] = buf[pos+i];
		}
		for (i = BLOCK_SIZE - pad; i < BLOCK_SIZE; i++) {
			block[i] = pad;
		}

		/* update checksum */

		for (i = 0; i < BLOCK_SIZE; i++) {
			l = S[block[i] ^ l] ^ checksum[i];
			checksum[i] = l;
		}

 		/* process block */

		for (i = 0; i < BLOCK_SIZE; i++) {
			digest[BLOCK_SIZE+i] = block[i];
			digest[(2*BLOCK_SIZE)+i] = digest[BLOCK_SIZE+i] ^ digest[i];
		}
		t = 0;
		for (i = 0; i < NROUNDS; i++) {
			for (j = 0; j < DIGEST_SIZE; j++) {
				t = digest[j] ^ S[t];
				digest[j] = t;
			}
			t = (t + i) % 256;
		}

	}

	/* append checksum */

	for (i = 0; i < BLOCK_SIZE; i++) {
		block[i] = checksum[i];
	}

	/* process block */
	for (i = 0; i < BLOCK_SIZE; i++) {
		digest[BLOCK_SIZE+i] = block[i];
		digest[(2*BLOCK_SIZE)+i] = digest[BLOCK_SIZE+i] ^ digest[i];
	}
	t = 0;
	for (i = 0; i < NROUNDS; i++) {
		for (j = 0; j < DIGEST_SIZE; j++) {
			t = digest[j] ^ S[t];
			digest[j] = t;
		}
		t = (t + i) % 256;
	}
		
	for (i = 0; i < DIGEST_FINAL_SIZE; i++) {
		fprintf(stdout, "%02x", digest[i]);
	}
	fprintf(stdout, "\n");

}

#ifdef MD2_TESTSUITE

int main(int argc, char *argv[]) {

	fprintf(stdout, "Test Suite (RFC1319)\n");

	fprintf(stdout, "MD2 (\"\") =\nEXP: 8350e5a3e24c153df2275c9f80692773\nACT: ");
	md2((byte *)"", 0);
	fprintf(stdout, "\n");

	fprintf(stdout, "MD2 (\"a\") =\nEXP: 32ec01ec4a6dac72c0ab96fb34c0b5d1\nACT: ");
	md2((byte *)"a", 1);
	fprintf(stdout, "\n");

	fprintf(stdout, "MD2 (\"abc\") =\nEXP: da853b0d3f88d99b30283a69e6ded6bb\nACT: ");
	md2((byte *) "abc", 3);
	fprintf(stdout, "\n");

	fprintf(stdout, "MD2 (\"message digest\") =\nEXP: ab4f496bfb2a530b219ff33031fe06b0\nACT: ");
	md2((byte *) "message digest", 14);
	fprintf(stdout, "\n");

	fprintf(stdout, "MD2 (\"abcdefghijklmnopqrstuvwxyz\") =\nEXP: 4e8ddff3650292ab5a4108c3aa47940b\nACT: ");
	md2((byte *) "abcdefghijklmnopqrstuvwxyz", 26);
	fprintf(stdout, "\n");

	fprintf(stdout, "MD2 (\"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\") =\nEXP: da33def2a42df13975352846c30338cd\nACT: ");
	md2((byte *) "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", 62);
	fprintf(stdout, "\n");

	fprintf(stdout, "MD2 (\"12345678901234567890123456789012345678901234567890123456789012345678901234567890\") =\nEXP: d5976f79d83d3a0dc9806c3c66f3efd8\nACT: ");
	md2((byte *) "12345678901234567890123456789012345678901234567890123456789012345678901234567890", 80);
	fprintf(stdout, "\n");

	return 0;
}

#else

int main(int argc, char *argv[]) {

	FILE *fp;
	length len, cap;
	int ch;
	int i;
	byte *p, *q;
	byte b;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "md2 foo.txt", .description = "print the MD2 message digest of a file" },
		{ .command = "... | md2", .description = "print the MD2 message digest of standard input" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "md2",
		.usage = "[OPTIONS] [FILE]",
		.description = "calculate a message-digest fingerprint for a file",
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

	fp = argc == 0 ? stdin : fopen(argv[0], "r");
	if (fp == TC_NULL) {
		tc_exit(TC_EXIT_FAILURE);
	}

	len = 0;
	cap = 512; /* initial capacity (bytes) */

	p = (byte *) malloc(cap);
	if (p == TC_NULL) {
		fclose(fp);
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((ch = fgetc(fp)) != EOF) {
		p[len] = (byte) ch;
		len++;
		if (len >= cap) {
			cap = 2 * len;
			q = (byte *) malloc(cap);
			if (q == TC_NULL) {
				free(p);
				fclose(fp);
				tc_exit(TC_EXIT_FAILURE);
			}
			memcpy(q, p, len);
			free(p);
			p = q;
			q = TC_NULL;
		}
	}

	md2(p, len);
	free(p);

	fclose(fp);

	tc_exit(TC_EXIT_SUCCESS);
}
#endif
