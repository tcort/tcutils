 /*
    dogfacts -- writes a random dog fact to standard output
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

static char *dogfacts[] = {
    "A dog’s nose print is unique, much like a person’s fingerprint.",
    "Forty-five percent of U.S. dogs sleep in their owner’s bed.",
    "Rin Tin Tin, the famous German Shepherd, was nominated for an Academy Award.",
    "Yawning is contagious—even for dogs. Research shows that the sound of a human yawn can trigger one from your dog. And it’s four times as likely to happen when it’s the yawn of a person he knows.",
    "Dogs curl up in a ball when sleeping to protect their organs—a hold over from their days in the wild, when they were vulnerable to predator attacks.",
    "The Dandie Dinmont Terrier is the only breed named for a fictional person—a character in the novel Guy Mannering, by Sir Walter Scott.",
    "The Australian Shepherd is not actually from Australia—they are an American breed.",
    "Human blood pressure goes down when petting a dog. And so does the dog’s.",
    "There are over 75 million pet dogs in the U.S.—more than in any other country.",
    "Dogs are not colorblind. They can see blue and yellow.",
    "All puppies are born deaf.",
    "Dalmatians are born completely white, and develop their spots as they get older.",
    "Dogs have 18 muscles controlling their ears.",
    "Dogs are the most popular pets on the planet Earth.",
    "A dog's sense of smell is at least 40 times more sensitive than a human's sense of smell.",
    "Dogs only sweat from their paws.",
    "Dogs use body language to express their feelings.",
    "Dachshunds were originally bred to fight badgers.",
    "Dogs have wet noses because it helps to absorb scent chemicals.",
    "Three dogs survived the sinking of the Titanic",
    "More than half of all U.S presidents have owned dogs.",
    "Dogs that have been spayed or neutered live longer than dogs who are intact."
};

static int ndogfacts = sizeof(dogfacts) / sizeof(dogfacts[0]);

int main(int argc, char *argv[]) {

	int i, flag_a;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{ .arg = 'a', .longarg = "all", .description = "print all dog facts", .has_value = 0 },
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "dogfacts", .description = "show a random fact about dogs" },
		{ .command = "dogfacts --all", .description = "show all facts about dogs in the knowledge base" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "dogfacts",
		.usage = "[OPTIONS]",
		.description = "writes a random dog fact to standard output",
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
		for (i = 0; i < ndogfacts; i++) {
			tc_puts(TC_STDOUT, dogfacts[i]);
			tc_puts(TC_STDOUT, "\n");
		}
	} else {
		tc_puts(TC_STDOUT, dogfacts[tc_abs(tc_rand()) % ndogfacts]);
		tc_puts(TC_STDOUT, "\n");
	}


	tc_exit(TC_EXIT_SUCCESS);
}
