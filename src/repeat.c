 /*
    repeat -- repeatedly execute a command
    Copyright (C) 2022, 2023  Thomas Cort

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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int do_cmd(char *argv[]) {

	int wstatus;
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		tc_perror();
		return -1;
	} else if (pid == 0) {
		execvp(argv[0], argv);
		tc_exit(TC_EXIT_FAILURE);
	}

	waitpid(pid, &wstatus, 0);
	return WEXITSTATUS(wstatus);
}


int main(int argc, char *argv[]) {

	int i;
	int flag_n;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'n', .longarg = "lines", .description = "number of lines to show (default 10)", .has_value = 1 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "repeat cat /foo/bar/baz/status", .description = "repeatedly execute `cat /foo/bar/baz/status` forever" },
		{ .command = "repeat -n 3 cat /foo/bar/baz/status", .description = "execute `cat /foo/bar/baz/status` three times" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "repeat",
		.usage = "[OPTIONS]",
		.description = "repeatedly execute a command",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_n = 0;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'n':
				flag_n = tc_atoi(argval);
				flag_n = flag_n < 0 ? 0 : flag_n;
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	for (i = 0; i < flag_n || flag_n == 0; i++) {
		int rc;

		rc = do_cmd(argv);
		if (rc != TC_EXIT_SUCCESS) {
			tc_exit(rc);
		}
	}

	tc_exit(TC_EXIT_SUCCESS);
}
