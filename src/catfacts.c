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

#include "config.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

	int ch, i, flag_a;

	static struct option long_options[] = {
		{ "all", no_argument, 0, 'a' },
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	flag_a = 0;

	while ((ch = getopt_long(argc, argv, "ahV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'a':
				flag_a = 1;
				break;
			case 'h':
				fprintf(stdout, "catfacts -- writes a random cat fact to standard output\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: catfacts [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -a, --all      print all cat facts\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show a random fact about cats\n");
				fprintf(stdout, "  catfacts\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # show all facts about cats in the knowledge base\n");
				fprintf(stdout, "  catfacts --all\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "catfacts (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
				break;
			default:
				fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	/* if OS randomizes PIDs, then this is a random seed */
	srand((unsigned int) getpid());

	if (flag_a == 1) {
		for (i = 0; i < ncatfacts; i++) {
			fprintf(stdout, "%s\n", catfacts[i]);
		}
	} else {
		fprintf(stdout, "%s\n", catfacts[rand() % ncatfacts]);
	}


	exit(EXIT_SUCCESS);
}
