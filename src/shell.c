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

    SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <tc/tc.h>

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

	rc = tc_isatty(TC_STDOUT);
	if (rc != 1) {
		return;
	}

	uid = getuid();

	passwd = getpwuid(uid);
	if (passwd == TC_NULL) {
		return;
	}

	memset(path, '\0', sizeof(char) * (MAXPATHLEN+1));
	p = getcwd(path, MAXPATHLEN);
	if (p == TC_NULL) {
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

	n++; /* last entry in argv must be TC_NULL */

	argv = (char **) malloc(sizeof(char*) * n);
	if (argv == TC_NULL) {
		return TC_NULL;
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
		tc_exit(TC_EXIT_FAILURE);
	}

	waitpid(pid, &wstatus, 0);
	return WEXITSTATUS(wstatus);
}

int main(int argc, char *argv[]) {

	int ch;
	char **cmdargv;
	char *cmdline = TC_NULL;
	size_t cap = 0;
	ssize_t len = 0;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "shell", .description = "launch command shell" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "shell",
		.usage = "[OPTIONS]",
		.description = "command shell",
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

	do {
		prompt();

		len = getline(&cmdline, &cap, stdin);
		if (len < 0) {
			break;
		}

		cmdargv = argvify(cmdline);
		if (cmdargv == TC_NULL) {
			perror("malloc");
			break;
		}

		if (!tryinternal(cmdargv)) {
			tryexternal(cmdargv);
		}

		free(cmdargv);

	} while (!done);

	free(cmdline);
	cmdline = TC_NULL;

	tc_exit(TC_EXIT_SUCCESS);
}
