 /*
    splay -- executes a command where each argument comes from a line of standard input
    Copyright (C) 2022, 2023, 2024  Thomas Cort

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

static char **splay_add_arg(char **cmd_args, char *cmd_arg) {

	char *arg;

	arg = tc_strdup(cmd_arg);
	if (arg == TC_NULL) {
		cmd_args = tc_array_free(cmd_args);
		return TC_NULL;
	}

	cmd_args = tc_array_append(cmd_args, arg);
	if (cmd_args == TC_NULL) {
		return TC_NULL;
	}

	return cmd_args;
}

int main(int argc, char *argv[]) {

	int i;
	int ch;
	int rc;
	int status;
	char *line;
	char *cmd;
	char **cmd_args;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "forage '\\.mp3$' | splay mpv", .description = "execute mpv with a list of mp3 files which may contain spaces" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "splay",
		.usage = "[OPTIONS]",
		.description = "executes a command where each argument comes from a line of standard input",
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
		tc_args_show_help(&prog);
	}

	cmd = argv[0];
	cmd_args = tc_array_malloc(1);
	if (cmd_args == TC_NULL) {
		tc_puterr("Failed to Allocate Memory\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	for (i = 0; i < argc; i++) {
		cmd_args = splay_add_arg(cmd_args, argv[i]);
		if (cmd_args == TC_NULL) {
			tc_puterr("Failed to Allocate Memory\n");
			cmd_args = tc_array_free(cmd_args);
			tc_exit(TC_EXIT_FAILURE);
		}
	}

	do {
                line = tc_getln(TC_STDIN, &status);
                if (line == TC_NULL || status == TC_ERR || tc_strlen(line) == 0) {
			line = tc_free(line);
                        break;
                }

		cmd_args = splay_add_arg(cmd_args, line);
		if (cmd_args == TC_NULL) {
			tc_puterr("Failed to Allocate Memory\n");
			cmd_args = tc_array_free(cmd_args);
			tc_exit(TC_EXIT_FAILURE);
		}

		line = tc_free(line);

		if (status == TC_EOF) {
			break;
		}
	} while (1);

	rc = tc_execvp(cmd, cmd_args);
	if (rc == TC_ERR) {
		tc_puterr("Failed to Execute Command\n");
		line = tc_free(line);
		tc_exit(TC_EXIT_FAILURE);
	}

	line = tc_free(line);
	cmd_args = tc_array_free(cmd_args);
	tc_exit(TC_EXIT_SUCCESS);
}
