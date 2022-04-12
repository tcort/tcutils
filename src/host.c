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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static char *sa2s(struct sockaddr *sa) {

	static char s[64+1];
	memset(s, '\0', 64+1);

	switch (sa->sa_family) {
		case AF_INET:
			inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, 64);
			break;
		case AF_INET6:
			inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, 64);
			break;
		default:
			snprintf(s, 64, "UNKNOW ADDRESS FAMILY\n");
			break;
	}

	return s;
}

int main(int argc, char *argv[]) {

	int ch, i, rc;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	char *hostname;

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

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_CANONNAME;
	hints.ai_protocol = 0;

	rc = getaddrinfo(argv[0], NULL, &hints, &result);
	if (rc != 0) {
		fprintf(stderr, "getaddrinfo: %s", gai_strerror(rc));
		exit(EXIT_FAILURE);
	}

	hostname = result->ai_canonname;;
	for (rp = result; rp; rp = rp->ai_next) {
		fprintf(stdout, "%s has address %s\n", hostname, sa2s(rp->ai_addr));
	}

	freeaddrinfo(result);

	exit(EXIT_SUCCESS);
}
