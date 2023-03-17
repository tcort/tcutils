/*
    args.h - command line argument parsing
    Copyright (C) 2022, 2023 Thomas Cort

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

#ifndef TC_ARGS_H
#define TC_ARGS_H

#include "sys.h"

struct tc_prog_arg {
	char arg;
	char *longarg;
	char *description;
	int has_value;
};

struct tc_prog_example {
	char *description;
	char *command;
};

struct tc_prog {
	char *program;
	char *version;
	char *package;
	char *description;
	char *copyright;
	char *license;
	char *author;
	char *usage;
	struct tc_prog_arg *args;
	struct tc_prog_example *examples;
};

#define TC_PROG_ARG_VERBOSE { .arg = 'v', .longarg = "verbose", .description = "print verbose debugging details", .has_value = 0 }
#define TC_PROG_ARG_SILENT { .arg = 's', .longarg = "silent", .description = "suppress output; yield exit status only", .has_value = 0 }
#define TC_PROG_ARG_HELP { .arg = 'h', .longarg = "help", .description = "print help text", .has_value = 0 }
#define TC_PROG_ARG_VERSION { .arg = 'V', .longarg = "version", .description = "print version and copyright info", .has_value = 0 }
#define TC_PROG_ARG_END { .arg = '\0', .longarg = TC_NULL, .description = TC_NULL, .has_value = 0 }
#define TC_PROG_EXAMPLE_END { .description = TC_NULL, .command = TC_NULL }

extern int argi;
extern char *argval;

struct tc_prog_arg *tc_args_process(struct tc_prog *prog, int argc, char *argv[]);
void tc_args_show_help(struct tc_prog *prog);
void tc_args_show_version(struct tc_prog *prog);
void tc_args_show_usage(struct tc_prog *prog);

#endif
