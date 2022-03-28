 /*
    host -- DNS lookup utility
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

#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int ch, i;
	struct hostent *host;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "host -- DNS lookup utility\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: host [OPTIONS] HOSTNAME\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # print the IP address associated with example.com\n");
				fprintf(stdout, "  host example.com\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "host (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	if (argc != 1) {
		fprintf(stderr, "usage: host [OPTIONS] HOSTNAME\n");
		exit(EXIT_FAILURE);
	}

	host = gethostbyname(argv[0]);
	if (host == NULL) {
		herror(argv[0]);
		exit(EXIT_FAILURE);
	}

	for (i = 0; host->h_aliases[i] != NULL; i++) {
		fprintf(stdout, "%s is an alias for %s\n", host->h_aliases[i], host->h_name);
	}
	for (i = 0; host->h_addr_list[i] != NULL; i++) {
		struct in_addr *addr = (struct in_addr *) host->h_addr_list[i];
		fprintf(stdout, "%s has address %s\n", host->h_name, inet_ntoa(*addr));
	}

	exit(EXIT_SUCCESS);
}
