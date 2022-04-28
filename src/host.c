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

#include "tc/args.h"
#include "tc/const.h"
#include "tc/sys.h"
#include "tc/version.h"

#include <arpa/inet.h>
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

	int i, rc;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	char *hostname;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "host example.com", .description = "show the IP address(es) associated with example.com" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "host",
		.usage = "[OPTIONS] HOSTNAME",
		.description = "DNS lookup utility",
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

	if (argc != 1) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_CANONNAME;
	hints.ai_protocol = 0;

	rc = getaddrinfo(argv[0], TC_NULL, &hints, &result);
	if (rc != 0) {
		fprintf(stderr, "getaddrinfo: %s", gai_strerror(rc));
		tc_exit(TC_EXIT_FAILURE);
	}

	hostname = result->ai_canonname;;
	for (rp = result; rp; rp = rp->ai_next) {
		fprintf(stdout, "%s has address %s\n", hostname, sa2s(rp->ai_addr));
	}

	freeaddrinfo(result);

	tc_exit(TC_EXIT_SUCCESS);
}
