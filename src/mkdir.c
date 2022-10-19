 /*
    mkdir -- make directories
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

#include <errno.h>

static void make_directory(char *path, int ignore_exists) {
	int rc;

	rc = tc_mkdir(path, 0777);
	if (rc == TC_ERR && !(ignore_exists && errno == EEXIST)) {
		tc_puterr("Could not create directory: ");
		tc_puterrln(path);
		tc_exit(TC_EXIT_FAILURE);
	}
}

static void make_parents(char *path) {

	char *parent;

	path = tc_strdup(path);
	parent = tc_strdup(tc_dirname(path));
	if (tc_streql(".", parent) == 0 && tc_streql("/", parent) == 0 && tc_streql("", parent) == 0) {
		make_parents(parent);
		make_directory(parent, 1);
	}
	parent = tc_free(parent);
	path = tc_free(path);
}

int main(int argc, char *argv[]) {

	int rc, i, flag_p;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'p', .longarg = "parents", .description = "print help text", .has_value = 0 },
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "mkdir foo", .description = "create empty directory foo" },
		{ .command = "mkdir -p foo/bar", .description = "create empty directory bar and any other path components as needed" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "mkdir",
		.usage = "[OPTIONS] DIRNAME...",
		.description = "make directories",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	/* defaults */
	flag_p = 0;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'p':
				flag_p = 1;
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

	for (i = 0; i < argc; i++) {
		if (flag_p == 1) {
			make_parents(argv[i]);
		}
		make_directory(argv[i], 0);
	}


	tc_exit(TC_EXIT_SUCCESS);
}
