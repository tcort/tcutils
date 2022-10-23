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

#include <tc/tc.h>

#include <curl/curl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SUPPORTED_PROTOCOLS	(	\
	CURLPROTO_SCP		|	\
	CURLPROTO_FTP		|	\
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

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		{ .arg = 'p', .longarg = "password", .description = "set password", .has_value = 1 },
		{ .arg = 'u', .longarg = "username", .description = "set username", .has_value = 1 },
		TC_PROG_ARG_VERSION,
		{ .arg = 'v', .longarg = "verbose", .description = "print verbose debugging details", .has_value = 0 },
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "up www.tcort.dev", .description = "get the status of https://www.tcort.dev/" },
		{ .command = "up smtp://mail.tcort.dev", .description = "get the status of a mail server" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "up",
		.usage = "[OPTIONS]",
		.description = "universal ping, checks the status of a URL",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	flag_v = 0;
	username = TC_NULL;
	password = TC_NULL;

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
			case 'v':
				flag_v = 1;
				break;
			case 'u':
				username = argval;
				break;
			case 'p':
				password = argval;
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc != 1) {
		fprintf(stderr, "usage: up [OPTIONS] URL\n");
		tc_exit(TC_EXIT_FAILURE);
	}

	url = argv[0];
	headers = TC_NULL;
	errbuf[0] = '\0';

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == TC_NULL) {
		perror("curl_easy_init");
		tc_exit(TC_EXIT_FAILURE);
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

	if (username != TC_NULL) curl_easy_setopt(curl, CURLOPT_USERNAME, username);
	if (password != TC_NULL) curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

	if (strncmp(url, "smtp", tc_strlen("smtp")) == 0 || strncmp(url, "smtps", tc_strlen("smtps")) == 0) {
		/* SMTP/SMTPS Options */
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "NOOP");
	} else if (strncmp(url, "imap", tc_strlen("imap")) == 0 || strncmp(url, "imaps", tc_strlen("imaps")) == 0) {
		/* IMAP/IMAPS options */
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXAMINE INBOX");
	} else if (strncmp(url, "scp", tc_strlen("scp")) == 0) {
		/* SCP options */
		/* none */;
	} else if (strncmp(url, "ftp", tc_strlen("ftp")) == 0) {
		/* FTP options */
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

	tc_exit(TC_EXIT_SUCCESS);
}
