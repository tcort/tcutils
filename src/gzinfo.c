 /*
    gzinfo -- provide information about a gzip file
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

    SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <tc/tc.h>

/* magic number */
#define ID1 (0x1f)
#define ID2 (0x8b)

int main(int argc, char *argv[]) {

	int i;
	int fd;
	int id1;
	int id2;
	int cm;
	int flg;
	int xfl;
	int os;
	tc_uint32_t mtime;
	char *mtimes;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "gzinfo foo.gz", .description = "show information about foo.gz" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "gzinfo",
		.usage = "[OPTIONS] [FILENAME]",
		.description = "provide information about a gzip file",
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

	/* gather info */

	fd = (argc == 0) ? TC_STDIN : tc_open_reader(argv[0]);
	if (fd == -1) {
		tc_puterr("Could not open file: ");
		tc_puterrln(argv[0]);
		tc_exit(TC_EXIT_FAILURE);
	}

	id1 = tc_getc(fd);
	if (id1 != ID1) {
		tc_puterrln("Not a gzip file");
		tc_exit(TC_EXIT_FAILURE);
	}

	id2 = tc_getc(fd);
	if (id2 != ID2) {
		tc_puterrln("Not a gzip file");
		tc_exit(TC_EXIT_FAILURE);
	}

	cm = tc_getc(fd);
	if (cm == TC_EOF || cm == TC_ERR) {
		tc_puterrln("unexpected error");
		tc_exit(TC_EXIT_FAILURE);
	}

	flg = tc_getc(fd);
	if (flg == TC_EOF || flg == TC_ERR) {
		tc_puterrln("unexpected error");
		tc_exit(TC_EXIT_FAILURE);
	}

	for (i = 0; i < sizeof(mtime); i++) {
		int tmp;
		tmp = tc_getc(fd);
		if (tmp == TC_EOF || tmp == TC_ERR) {
			tc_puterrln("unexpected error");
			tc_exit(TC_EXIT_FAILURE);
		}

		mtime = ((mtime >> 8) | (tmp << 24));
	}
	mtimes = tc_itoa(mtime);
	if (mtimes == TC_NULL) {
		tc_puterrln("Out of Memory");
		tc_exit(TC_EXIT_FAILURE);
	}

	xfl = tc_getc(fd);
	if (xfl == TC_EOF || xfl == TC_ERR) {
		tc_puterrln("unexpected error");
		tc_exit(TC_EXIT_FAILURE);
	}

	os = tc_getc(fd);
	if (os == TC_EOF || os == TC_ERR) {
		tc_puterrln("unexpected error");
		tc_exit(TC_EXIT_FAILURE);
	}


	tc_close(fd);

	/* show info */

	tc_putln(TC_STDOUT, "ID1 (IDentification 1): 0x1f");
	tc_putln(TC_STDOUT, "ID2 (IDentification 2): 0x8b");

	tc_puts(TC_STDOUT, "CM (Compression Method): ");
	switch (cm) {
		case 0x00: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x01: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x02: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x03: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x04: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x05: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x06: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x07: tc_putln(TC_STDOUT, "reserved"); break;
		case 0x08: tc_putln(TC_STDOUT, "deflate"); break;
		default  : tc_putln(TC_STDOUT, "unknown"); break;
	}

	tc_puts(TC_STDOUT, "FLG (FLaGs):");
	if (flg == 0) {
		tc_puts(TC_STDOUT, " NONE");
	} else {
		if ((flg & (1<<0))) {
			tc_puts(TC_STDOUT, " FTEXT");
		}
		if ((flg & (1<<1))) {
			tc_puts(TC_STDOUT, " FHCRC");
		}
		if ((flg & (1<<2))) {
			tc_puts(TC_STDOUT, " FEXTRA");
		}
		if ((flg & (1<<3))) {
			tc_puts(TC_STDOUT, " FNAME");
		}
		if ((flg & (1<<4))) {
			tc_puts(TC_STDOUT, " FCOMMENT");
		}
		if ((flg & (1<<5))) {
			tc_puts(TC_STDOUT, " reserved");
		}
		if ((flg & (1<<6))) {
			tc_puts(TC_STDOUT, " reserved");
		}
		if ((flg & (1<<7))) {
			tc_puts(TC_STDOUT, " reserved");
		}
	}
	tc_putln(TC_STDOUT, "");

	tc_puts(TC_STDOUT, "MTIME (Modification TIME): ");
	tc_putln(TC_STDOUT, mtimes);

	tc_puts(TC_STDOUT, "OS (Operating System): ");
	switch (os) {
		case 0: tc_putln(TC_STDOUT, "FAT filesystem (MS-DOS, OS/2, NT/Win32)"); break;
		case 1: tc_putln(TC_STDOUT, "Amiga"); break;
		case 2: tc_putln(TC_STDOUT, "VMS (or OpenVMS)"); break;
		case 3: tc_putln(TC_STDOUT, "Unix"); break;
		case 4: tc_putln(TC_STDOUT, "VM/CMS"); break;
		case 5: tc_putln(TC_STDOUT, "Atari TOS"); break;
		case 6: tc_putln(TC_STDOUT, "HPFS filesystem (OS/2, NT)"); break;
		case 7: tc_putln(TC_STDOUT, "Macintosh"); break;
		case 8: tc_putln(TC_STDOUT, "Z-System"); break;
		case 9: tc_putln(TC_STDOUT, "CP/M"); break;
		case 10: tc_putln(TC_STDOUT, "TOPS-20"); break;
		case 11: tc_putln(TC_STDOUT, "NTFS filesystem (NT)"); break;
		case 12: tc_putln(TC_STDOUT, "QDOS"); break;
		case 13: tc_putln(TC_STDOUT, "Acorn RISCOS"); break;
		case 255: tc_putln(TC_STDOUT, "unknown"); break;
		default: tc_putln(TC_STDOUT, "unknown"); break;
	}

	/* cleanup */
	mtimes = tc_free(mtimes);

	tc_exit(TC_EXIT_SUCCESS);
}
