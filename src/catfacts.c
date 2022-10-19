 /*
    catfacts -- writes a random cat fact to standard output
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

static char *catfacts[] = {
	"Kittens have 26 teeth while adult cats have 30 teeth",
	"Cats have about 230 bones",
	"A group of cats is called a clowder",
	"Cats only meow when communicating with humans",
	"The first cat video was recorded in 1894 and is titled 'Boxing Cats'",
	"A cat's jaw can't move sideways",
	"Cats can rotate their ears 180 degrees",
	"Most cats have no eyelashes",
	"The world cat population is larger than half a billion",
	"A male cat is called a tom",
	"A female cat is called a molly or queen",
	"Cats have whiskers on the backs of their front legs",
	"Cats want like camels and giraffes, both right feet and then both left feet",
	"The majority of male cats are left handed",
	"The majority of female cats are right handed",
	"Cats can dream",
	"A person who loves cats is called an ailurophile",
	"Ailurophobia is an irrational or extreme fear of cats",
	"Male cats are more likely to fight than female cats",
	"Cats cannot take in liquids via suction due to the structure of their cheeks",
	"Cats reject novel flavors",
	"Cats often play with their toys more when they are hungry",
	"Honorable Kodiak Kat was the 73rd Bombardment Wing mascot in WWII",
	"The most popular cat name in Quebec (Canada) is Minou"
};

static int ncatfacts = sizeof(catfacts) / sizeof(catfacts[0]);

int main(int argc, char *argv[]) {

	int i, flag_a;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'a', .longarg = "all", .description = "print all cat facts", .has_value = 0 },
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "catfacts", .description = "show a random fact about cats" },
		{ .command = "catfacts --all", .description = "show all facts about cats in the knowledge base" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "catfacts",
		.usage = "[OPTIONS]",
		.description = "writes a random cat fact to standard output",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	flag_a = 0;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'a':
				flag_a = 1;
				break;
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

	tc_srand((unsigned int) tc_getpid());

	if (flag_a == 1) {
		for (i = 0; i < ncatfacts; i++) {
			tc_puts(TC_STDOUT, catfacts[i]);
			tc_puts(TC_STDOUT, "\n");
		}
	} else {
		tc_puts(TC_STDOUT, catfacts[tc_abs(tc_rand()) % ncatfacts]);
		tc_puts(TC_STDOUT, "\n");
	}


	tc_exit(TC_EXIT_SUCCESS);
}
