 /*
    ts -- copy lines from standard input to standard output adding timestamps
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
#include "tc/stdio.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FMT_HUMAN "%Y-%m-%d %H:%M:%S"
#define FMT_ISO8601 "%FT%T%z"
#define FMT_COMPACT "%Y%m%d%H%M%S"
#define FMT_UNIX "%s"

static void ts(char *fmt) {
	time_t now;
	struct tm *local;
	char tstring[32];

	now = time(TC_NULL);
	local = localtime(&now);
	memset(tstring, '\0', sizeof(tstring));
	strftime(tstring, sizeof(tstring)-1, fmt, local);
	tc_puts(TC_STDOUT, "[");
	tc_puts(TC_STDOUT, tstring);
	tc_puts(TC_STDOUT, "] ");
}

int main(int argc, char *argv[]) {

	int ch, pch;
	char *fmt;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'c', .longarg = "compact", .description = "filename safe format (YYYYmmddHHMMSS)", .has_value = 0 },
		TC_PROG_ARG_HELP,
		{ .arg = 'i', .longarg = "iso8601", .description = "ISO8601 format", .has_value = 0 },
		{ .arg = 'u', .longarg = "unix", .description = "UNIX timestamp", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "food | ts > foo.log", .description = "add timestamps to lines from standard input" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "ts",
		.usage = "[OPTIONS]",
		.description = "copy lines from standard input to standard output adding timestamps",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	fmt = FMT_HUMAN;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'c':
				fmt = FMT_COMPACT;
				break;
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'i':
				fmt = FMT_ISO8601;
				break;
			case 'u':
				fmt = FMT_UNIX;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	ts(fmt);
	pch = 0;
	while ((ch = tc_getc(TC_STDIN)) != TC_EOF) {
		if (pch == '\n') {
			ts(fmt);
		}
		tc_putc(TC_STDOUT, ch);
		pch = ch;
	}

	tc_exit(TC_EXIT_SUCCESS);
}
