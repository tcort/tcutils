 /*
    rest -- tool for making rest requests with JSON
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

static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *stream) {
	return fread(ptr, size, nmemb, stream);
}

int main(int argc, char *argv[]) {

	CURL *curl;
	CURLcode res;
	char errbuf[CURL_ERROR_SIZE];
	int ch;
	char *method;
	char *url;
	FILE *input;
	struct curl_slist *headers;
	int flag_v;
	char *username;
	char *password;
	int rc;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "password", required_argument, 0, 'p' },
		{ "username", required_argument, 0, 'u' },
		{ "version", no_argument, 0, 'V' },
		{ "verbose", no_argument, 0, 'v' },
		{ 0, 0, 0, 0 }
	};

	rc = EXIT_SUCCESS;
	flag_v = 0;
	username = NULL;
	password = NULL;

	while ((ch = getopt_long(argc, argv, "hu:p:Vv", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "rest -- tool for making rest requests with JSON\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: rest [OPTIONS] [METHOD] URL\n");
				fprintf(stderr, "usage: rest [OPTIONS] METHOD URL (DATA|@FILENAME)\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help            print help text\n");
				fprintf(stdout, "  -p, --password=abc23  set password\n");
				fprintf(stdout, "  -u, --username=alice  set username\n");
				fprintf(stdout, "  -V, --version         print version and copyright info\n");
				fprintf(stdout, "  -v, --verbose         print verbose debugging details\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # fetch a JSON file from an https server\n");
				fprintf(stdout, "  rest get www.tcort.dev/foo.json\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # put a JSON file foo.json to an http server\n");
				fprintf(stdout, "  rest put http://www.tcort.dev/foo.json @foo.json\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # post command line argument input to an https server\n");
				fprintf(stdout, "  rest post https://www.tcort.dev/foo.json '{\"bar\":true}'\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # put input from standard input to an https server\n");
				fprintf(stdout, "  echo '{\"bar\":true}' | rest put https://www.tcort.dev/foo.json\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # delete a file on an https server\n");
				fprintf(stdout, "  rest delete https://www.tcort.dev/foo.json\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "rest (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	if (argc == 1) {
		method = "GET";
		url = argv[0];
		input = stdin;
	} else if (argc == 2) {
		method = argv[0];
		url = argv[1];
		input = stdin;
	} else if (argc == 3) {
		method = argv[0];
		url = argv[1];
		input = (argv[2][0] == '@' ? fopen((&argv[2][1]), "r") : fmemopen(argv[2], strlen(argv[2]), "r"));
		if (input == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "usage: rest [OPTIONS] [METHOD] URL\n");
		fprintf(stderr, "usage: rest [OPTIONS] METHOD URL (DATA|@FILENAME)\n");
		exit(EXIT_FAILURE);
	}

	headers = NULL;
	errbuf[0] = '\0';

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == NULL) {
		perror("curl_easy_init");
		fclose(input);
		exit(EXIT_FAILURE);
	}

	/* general options */
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, flag_v == 1 ? 1L : 0L);

	if (username != NULL) curl_easy_setopt(curl, CURLOPT_USERNAME, username);
	if (password != NULL) curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

	if (strcasecmp(method, "PUT") == 0) {
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READDATA, input);
		headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
		headers = curl_slist_append(headers, "Content-Type: application/json");
	} else if (strcasecmp(method, "POST") == 0) {
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_READDATA, input);
		headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
		headers = curl_slist_append(headers, "Content-Type: application/json");
	} else if (strcasecmp(method, "HEAD") == 0) {
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
	} else if (strcasecmp(method, "DELETE") == 0) {
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	}

	headers = curl_slist_append(headers, "User-Agent: rest/1.0.0");
	headers = curl_slist_append(headers, "Pragma: no-cache");
	headers = curl_slist_append(headers, "Cache-Control: no-cache");
	headers = curl_slist_append(headers, "Accept: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "FAIL %s\n", errbuf[0] == '\0' ? "request failed" : errbuf);
		rc = EXIT_FAILURE;
	}

	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	fclose(input);
	exit(rc);
}
