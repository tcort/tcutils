 /*
    date -- print the current date and time
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
#include <time.h>
#include <unistd.h>

#define FMT_HUMAN "%Y-%m-%d %H:%M:%S"
#define FMT_ISO8601 "%FT%T%z"
#define FMT_FILE "%Y%m%d%H%M%S"
#define FMT_UNIX "%s"

int main(int argc, char *argv[]) {

	time_t now;
	struct tm *local;
	char tstring[32];
	char *fmt;

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'f', .longarg = "filename-safe", .description = "filename safe format (YYYYmmddHHMMSS)", .has_value = 0 },
		TC_PROG_ARG_HELP,
		{ .arg = 'i', .longarg = "iso8601", .description = "ISO8601 format", .has_value = 0 },
		{ .arg = 'u', .longarg = "unix", .description = "UNIX timestamp", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "date", .description = "print the date and time in human readable format" },
		{ .command = "date -i", .description = "print the date and time in ISO8601" },
		{ .command = "tar -cf backup-$(date -f).tar foo.c bar.c", .description = "print the date and time in filename safe format" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "date",
		.usage = "[OPTIONS]",
		.description = "print the current date and time",
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
			case 'f':
				fmt = FMT_FILE;
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

	now = time(TC_NULL);
	local = localtime(&now);
	memset(tstring, '\0', sizeof(tstring));
	strftime(tstring, sizeof(tstring)-1, fmt, local);
	fprintf(stdout, "%s\n", tstring);

	tc_exit(TC_EXIT_SUCCESS);
}
