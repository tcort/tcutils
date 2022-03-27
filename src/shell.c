 /*
    shell -- command shell
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
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int done = 0;

static void prompt(void) {
	int fd;
	int rc;
	uid_t uid;
	struct passwd *passwd;
	char path[MAXPATHLEN+1];
	char *p;
	char hostname[MAXHOSTNAMELEN+1];

	fd = fileno(stdin);
	if (fd == -1) {
		return;
	}

	rc = isatty(fd);
	if (rc != 1) {
		return;
	}

	uid = getuid();

	passwd = getpwuid(uid);
	if (passwd == NULL) {
		return;
	}

	memset(path, '\0', sizeof(char) * (MAXPATHLEN+1));
	p = getcwd(path, MAXPATHLEN);
	if (p == NULL) {
		return;
	}

	if (strlen(passwd->pw_dir) > 1 && !strncmp(path, passwd->pw_dir, strlen(passwd->pw_dir))) {
		p = path + strlen(passwd->pw_dir) - 1;
		*p = '~';
	}

	memset(hostname, '\0', sizeof(char) * (MAXHOSTNAMELEN+1));
	rc = gethostname(hostname, MAXHOSTNAMELEN);
	if (rc == -1) {
		return;
	}

	fprintf(stdout, "%s@%s:%s%c ", passwd->pw_name, hostname, p, geteuid() == 0 ? '#' : '$');
	fflush(stdout);
}

static char **argvify(char *cmdline) {

	int i, j;
	int n, len;
	char **argv;

	len = strlen(cmdline);
	for (i = 0, n = 0; i < len; i++) {
		if (cmdline[i] == ' ') {
			n++;
		} else if (cmdline[i] == '\n' || cmdline[i] == '\r') {
			cmdline[i] = '\0';
		}
	}

	n++; /* last entry in argv must be NULL */

	argv = (char **) malloc(sizeof(char*) * n);
	if (argv == NULL) {
		return NULL;
	}
	memset(argv, '\0', sizeof(char*) * n);

	argv[0] = cmdline;
	for (i = 0, j = 1; i < len; i++) {
		if (cmdline[i] == ' ') {
			cmdline[i] = '\0';
			argv[j] = cmdline + i + 1;
			j++;
		}
	}

	return argv;
}

static int tryinternal(char *argv[]) {

	int rc;

	if (strcmp(argv[0], "exit") == 0) {
		done = 1;
		return 1;
	} else if (!strcmp(argv[0], "cd")) {
		rc = chdir(argv[1]);
		if (rc == -1) {
			perror("chdir");
		}
		return 1;
	}

	return 0;
}

static int tryexternal(char *argv[]) {

	int wstatus;
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		execvp(argv[0], argv);
		exit(EXIT_FAILURE);
	}

	waitpid(pid, &wstatus, 0);
	return WEXITSTATUS(wstatus);
}

int main(int argc, char *argv[]) {

	int ch;
	char **cmdargv;
	char *cmdline = NULL;
	size_t cap = 0;
	ssize_t len = 0;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ 0, 0, 0, 0 }
	};

	while ((ch = getopt_long(argc, argv, "hV", long_options, NULL)) != -1) {
		switch (ch) {
			case 'h':
				fprintf(stdout, "shell -- command shell\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "usage: shell [OPTIONS]\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  -h, --help     print help text\n");
				fprintf(stdout, "  -V, --version  print version and copyright info\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "examples:\n");
				fprintf(stdout, "\n");
				fprintf(stdout, "  # launch the command shell\n");
				fprintf(stdout, "  shell\n");
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				fprintf(stdout, "shell (%s) v%s\n", PROJECT_NAME, PROJECT_VERSION);
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

	do {
		prompt();

		len = getline(&cmdline, &cap, stdin);
		if (len < 0) {
			break;
		}

		cmdargv = argvify(cmdline);
		if (cmdargv == NULL) {
			perror("malloc");
			break;
		}

		if (!tryinternal(cmdargv)) {
			tryexternal(cmdargv);
		}

		free(cmdargv);

	} while (!done);

	free(cmdline);
	cmdline = NULL;

	exit(EXIT_SUCCESS);
}
