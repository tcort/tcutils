/*
    args.c - functions for processing command line arguments
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
#include "tc/string.h"
#include "tc/sys.h"

void tc_args_show_help(struct tc_prog *prog) {
	int i;

	/* true -- exits with a success return code */
	tc_puts(TC_STDOUT, prog->program);
	tc_puts(TC_STDOUT, " -- ");
	tc_puts(TC_STDOUT, prog->description);
	tc_puts(TC_STDOUT, "\n");

	/* <br> */
	tc_puts(TC_STDOUT, "\n");

	/* usage: true [OPTIONS] */
	tc_puts(TC_STDOUT, "usage: ");
	tc_puts(TC_STDOUT, prog->program);
	tc_puts(TC_STDOUT, " ");
	tc_puts(TC_STDOUT, prog->usage);
	tc_puts(TC_STDOUT, "\n");

	/* <br> */
	tc_puts(TC_STDOUT, "\n");

	/* <describe each argument> */
	for (i = 0; prog->args[i].arg != '\0' && prog->args[i].longarg != TC_NULL; i++) {

		/*  -h, --help     print help text */
		tc_puts(TC_STDOUT, "  ");

		if (prog->args[i].arg != '\0') { /* have a short arg? */
			tc_puts(TC_STDOUT, "-");
			tc_putc(TC_STDOUT, prog->args[i].arg);

			/* followed by a long arg? */
			if (prog->args[i].longarg != TC_NULL) {
				tc_puts(TC_STDOUT, ", ");
			}
		}

		if (prog->args[i].longarg != TC_NULL) { /* have a long arg? */
			tc_puts(TC_STDOUT, "--");
			tc_puts(TC_STDOUT, prog->args[i].longarg);
		}

		tc_puts(TC_STDOUT, "\t");
		tc_puts(TC_STDOUT, prog->args[i].description);
		tc_puts(TC_STDOUT, "\n");
	}

	/* <br> */
	tc_puts(TC_STDOUT, "\n");

	tc_puts(TC_STDOUT, "examples:\n");
	for (i = 0; prog->examples[i].command != TC_NULL && prog->examples[i].description != TC_NULL; i++) {

		/* <br> */
		tc_puts(TC_STDOUT, "\n");

		if (prog->examples[i].description != TC_NULL) {
			tc_puts(TC_STDOUT, "  # ");
			tc_puts(TC_STDOUT, prog->examples[i].description);
			tc_puts(TC_STDOUT, "\n");
		}

		if (prog->examples[i].command != TC_NULL) {
			tc_puts(TC_STDOUT, "  ");
			tc_puts(TC_STDOUT, prog->examples[i].command);
			tc_puts(TC_STDOUT, "\n");
		}

	}

	/* <br> */
	tc_puts(TC_STDOUT, "\n");

	tc_exit(TC_EXIT_SUCCESS);
}

void tc_args_show_version(struct tc_prog *prog) {

	/* true (tcutils) v2022 */
	tc_puts(TC_STDOUT, prog->program);
	tc_puts(TC_STDOUT, " (");
	tc_puts(TC_STDOUT, prog->package);
	tc_puts(TC_STDOUT, ") v");
	tc_puts(TC_STDOUT, prog->version);
	tc_puts(TC_STDOUT, "\n");

	tc_puts(TC_STDOUT, prog->copyright);
	tc_puts(TC_STDOUT, "\n");

	tc_puts(TC_STDOUT, prog->license);
	tc_puts(TC_STDOUT, "\n");

	/* <br> */
	tc_puts(TC_STDOUT, "\n");

	tc_puts(TC_STDOUT, "Written by ");
	tc_puts(TC_STDOUT, prog->author);
	tc_puts(TC_STDOUT, "\n");

	tc_exit(TC_EXIT_SUCCESS);
}

int argi = 1;
static int argii = 1;

struct tc_prog_arg *tc_args_process(struct tc_prog *prog, int argc, char *argv[]) {

	int i;
	struct tc_prog_arg *arg;
	static struct tc_prog_arg arg_invalid = TC_PROG_ARG_END;

	if (argi >= argc) { /* no more arguments to process */
		return TC_NULL;
	}

	if (argv[argi][0] != '-') { /* argument isn't a arg/longarg, done */
		return TC_NULL;
	}

 	if (tc_streql(argv[argi], "--") == 1) { /* explicit end of args */
		return TC_NULL;
	}

	if (tc_strneql(argv[argi], "--", 2) == 1) { /* longarg */

		for (i = 0; prog->args[i].arg != '\0' && prog->args[i].longarg != TC_NULL; i++) {
			if (tc_streql(prog->args[i].longarg, &(argv[argi][tc_strlen("--")])) == 1) {
				struct tc_prog_arg *res;
				res = &(prog->args[i]);

				argi += 1;
				return res;
			}
		}

	} else { /* short arg */

		for (i = 0; prog->args[i].arg != '\0' && prog->args[i].longarg != TC_NULL; i++) {

			if (prog->args[i].arg == argv[argi][argii]) {
				struct tc_prog_arg *res;

				res = &(prog->args[i]);

				/* more to go? e.g. ls -al */
				if (argv[argi][argii + 1] == TC_ENDSTR) {
					argi += 1;
					argii = 1;
				} else {
					argii += 1;
				}
				return res;
			}
		}

	}

	tc_puts(TC_STDERR, "Invalid argument: ");
	tc_puts(TC_STDERR, argv[argi]);
	tc_puts(TC_STDERR, "\n");
	tc_puts(TC_STDERR, "Try '");
	tc_puts(TC_STDERR, prog->program);
	tc_puts(TC_STDERR, " --help' for more information\n");

	tc_exit(TC_EXIT_FAILURE);
	return TC_NULL; /* should not reach this line unless exit fails */
}
