 /*
    touch -- change file timestamps
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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch, fd, rc, i;
	mode_t mode;
	struct timespec times[2];
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "touch foo.txt", .description = "create empty file foo.txt" },
		{ .command = "touch foo.txt", .description = "update timestamps on existing file foo.txt" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "touch",
		.usage = "[OPTIONS] FILE...",
		.description = "change file timestamps",
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

	mode = 0666 & ~umask(0);

	clock_gettime(CLOCK_REALTIME, &times[0]);
	memcpy(&times[1], &times[0], sizeof(struct timespec));

	for (i = 0; i < argc; i++) {

		fd = open(argv[i], O_WRONLY | O_CREAT, mode);
		if (fd == -1) {
			perror("open");
			tc_exit(TC_EXIT_FAILURE);
		}

		rc = futimens(fd, times);
		if (rc == -1) {
			perror("futimens");
			close(fd);
			tc_exit(TC_EXIT_FAILURE);
		}

		close(fd);

	}


	tc_exit(TC_EXIT_SUCCESS);
}
