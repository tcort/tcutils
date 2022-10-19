 /*
    units -- convert between units of measure
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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <tc/tc.h>

double C_to_F(double value) { return (((value / 5.0) * 9.0) + 32.0); }
double F_to_C(double value) { return (((value - 32.0) * 5.0) / 9.0); }
double cm_to_in(double value) { return (value / 2.54); }
double cm_to_m(double value) { return (value / 100.0); }
double ft_to_in(double value) { return (value * 12.0); }
double kg_to_lb(double value) { return (value / 0.453592); }
double in_to_cm(double value) { return (value * 2.54); }
double in_to_ft(double value) { return (value / 12.0); }
double lb_to_kg(double value) { return (value * 0.453592); }

struct conversion {
	char *unit_in;
	char *unit_out;
	double (*converter)(double value);
};

struct conversion conversions[] = {
	{ "C", "F", C_to_F },
	{ "F", "C", F_to_C },
	{ "cm", "in", cm_to_in },
	{ "cm", "m", cm_to_m },
	{ "ft", "in", ft_to_in },
	{ "kg", "lb", kg_to_lb },
	{ "in", "ft", in_to_ft },
	{ "in", "cm", in_to_cm },
	{ "lb", "kg", lb_to_kg }
};
#define NCONVERSIONS (sizeof(conversions)/sizeof(conversions[0]))

int convert(char *unit_in, char *unit_out, double value_in, double *value_out) {

	int i;

	for (i = 0; i < NCONVERSIONS; i++) {
		if (tc_streql(unit_in, conversions[i].unit_in) == 1 &&
			tc_streql(unit_out, conversions[i].unit_out) == 1) {

			*value_out = conversions[i].converter(value_in);
			return TC_OK;
		}
	}

	return TC_ERR;
}

int main(int argc, char *argv[]) {

	int rc;
	double value_in;
	double value_out;
	char *unit_in;
	char *unit_out;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "units 32C F", .description = "convert 32 Celsius to Ferinheight" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "units",
		.usage = "[OPTIONS] INPUT_VALUE_UNIT OUTPUT_UNIT",
		.description = "convert between units of measure",
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

	if (argc != 2) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	value_in = strtod(argv[0], &unit_in);
	unit_out = argv[1];

	rc = convert(unit_in, unit_out, value_in, &value_out);
	if (rc == TC_ERR) {
		tc_puterrln("Invalid Conversion");
		tc_exit(TC_EXIT_FAILURE);
	}

	fprintf(stdout, "%.2lf%s\n", value_out, unit_out);

	tc_exit(TC_EXIT_SUCCESS);
}
