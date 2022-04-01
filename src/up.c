 /*
    up -- universal ping, checks the status of a URL
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

#include <curl/curl.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SUPPORTED_PROTOCOLS	(	\
	CURLPROTO_SCP		|	\
	CURLPROTO_HTTP		|	\
	CURLPROTO_HTTPS		|	\
	CURLPROTO_IMAP		|	\
	CURLPROTO_IMAPS		|	\
	CURLPROTO_SMTP		|	\
	CURLPROTO_SMTPS		)

/* curl write function that does nothing */
static size_t blackhole(void *data, size_t size, size_t nmemb, void *userp) {
	return size * nmemb;
}

int main(int argc, char *argv[]) {

	CURL *curl;
	CURLcode res;
	char errbuf[CURL_ERROR_SIZE];
	int ch;
	long proto;
	char *url;
	struct curl_slist *headers;
	int flag_v;
	char *username;
	char *password;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "password", required_argument, 0, 'p' },
		{ "username", required_argument, 0, 'u' },
		{ "version", no_argument, 0, 'V' },
		{ "verbose", no_argument, 0, 'v' },
		{ 0, 0, 0, 0 }
	};

	flag_v = 0;
	username = NULL;
	password = NULL;

	while ((ch = getopt_long(argc, argv, "hu:p:Vv", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "up -- universal ping, checks the status of a URL\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Supported protocols: http, https, imap, imaps, scp, smtp, smtps\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: up [OPTIONS] URL\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help            print help text\n");
				fprintf(stdout, "  -p, --password=abc23  set password\n");
				fprintf(stdout, "  -u, --username=alice  set username\n");
				fprintf(stdout, "  -V, --version         print version and copyright info\n");
				fprintf(stdout, "  -v, --verbose         print verbose debugging details\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # get the status of https://www.tcort.dev/\n");
				fprintf(stdout, "  up www.tcort.dev\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # get the status of a mail server\n");
				fprintf(stdout, "  up smtp://mail.tcort.dev\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "up (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
				fprintf(stdout, "Copyright (C) 2022  Thomas Cort\n");
				fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
				fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
				fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "Written by Thomas Cort.\n");
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				flag_v = 1;
				break;
			case 'u':
				username = optarg;
				break;
			case 'p':
				password = optarg;
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
		fprintf(stderr, "usage: up [OPTIONS] URL\n");
		exit(EXIT_FAILURE);
	}

	url = argv[0];
	headers = NULL;
	errbuf[0] = '\0';

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == NULL) {
		perror("curl_easy_init");
		exit(EXIT_FAILURE);
	}

	/* general options */
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl, CURLOPT_PROTOCOLS, SUPPORTED_PROTOCOLS);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, flag_v == 1 ? 1L : 0L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, blackhole);

	if (username != NULL) curl_easy_setopt(curl, CURLOPT_USERNAME, username);
	if (password != NULL) curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

	if (strncmp(url, "smtp", strlen("smtp")) == 0 || strncmp(url, "smtps", strlen("smtps")) == 0) {
		/* SMTP/SMTPS Options */
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "NOOP");
	} else if (strncmp(url, "imap", strlen("imap")) == 0 || strncmp(url, "imaps", strlen("imaps")) == 0) {
		/* IMAP/IMAPS options */
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXAMINE INBOX");
	} else if (strncmp(url, "scp", strlen("scp")) == 0) {
		/* SCP options */
		/* none */;
	} else {
		/* HTTP/HTTPS options (default) */
		headers = curl_slist_append(headers, "User-Agent: up/1.0.0");
		headers = curl_slist_append(headers, "Pragma: no-cache");
		headers = curl_slist_append(headers, "Cache-Control: no-cache");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "HEAD");
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
	}

	res = curl_easy_perform(curl);
	if (res == CURLE_OK || res == CURLE_PARTIAL_FILE) {
		fprintf(stdout, "OK %s\n", url);
	} else {
		fprintf(stderr, "FAIL %s\n", errbuf[0] == '\0' ? "request failed" : errbuf);
	}

	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	exit(EXIT_SUCCESS);
}
