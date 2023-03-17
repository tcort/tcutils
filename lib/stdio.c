/*
    stdio.c - standard input/output
    Copyright (C) 2022, 2023  Thomas Cort

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

#include "tc/ctype.h"
#include "tc/math.h"
#include "tc/stdio.h"
#include "tc/stdlib.h"
#include "tc/string.h"
#include "tc/sys.h"

/*
 * Read a line of input from file descriptor fd
 * Does not include newline in returned string
 * Returns line as read from file descriptor
 * status is set to one of TC_OK, TC_EOF, TC_ERR
 */
char *tc_getln(int fd, int *status) {

	int cap = 4;
	int len = 0;
	char *s;

	s = tc_malloc(cap);
	if (s == TC_NULL) {
		return TC_NULL;
	}

	do {
		char ch;

		ch = tc_getc(fd);
		if (ch == TC_NEWLINE) {
			*status = TC_OK;
			return s;
		} else if (ch == TC_ERR) {
			*status = TC_ERR;
			s = tc_free(s);
			return TC_NULL;
		} else if (ch == TC_EOF) {
			*status = TC_EOF;
			if (tc_strlen(s) > 0) {
				return s;
			}
			s = tc_free(s);
			return TC_NULL;
		}

		if (len + 1 == cap) {
			cap = cap + cap;
			s = tc_strgrow(s, cap);
			if (s == TC_NULL) {
				return TC_NULL;
			}
		}

		s[len] = ch;
		len++;
	} while (1);

}

/*
 * Write string message to standard error.
 * Returns TC_OK upon success, TC_ERR upon error.
 */
int tc_puterr(char *msg) {
	return tc_puts(TC_STDERR, msg);
}

/*
 * Write string message to standard error followed by a newline.
 * Returns TC_OK upon success, TC_ERR upon error.
 */
int tc_puterrln(char *msg) {
	return tc_putln(TC_STDERR, msg);
}

/*
 * Write string s to file descriptor fd
 * Returns TC_OK upon success, TC_ERR upon error.
 */
int tc_puts(int fd, char *s) {

	int rc;
	int i;

	for (i = 0; i < tc_strlen(s); i++) {
		rc = tc_putc(fd, s[i]);
		if (rc == TC_ERR) {
			return TC_ERR;
		}
	}

	return TC_OK;
}

/*
 * Write string s followed by a newline to file descriptor fd
 * Returns TC_OK upon success, TC_ERR upon error.
 */
int tc_putln(int fd, char *s) {

	int rc;

	rc = tc_puts(fd, s);
	if (rc == TC_ERR) {
		return TC_ERR;
	}

	rc = tc_putc(fd, TC_NEWLINE);
	if (rc == TC_ERR) {
		return TC_ERR;
	}

	return TC_OK;
}


/*
 * copies a line of characters from src to dst (including the final newline)
 * returns TC_OK, TC_EOF, or TC_ERR
 */
int tc_copyln(int src, int dst) {
	int ch;
	int rc;

	rc = TC_OK;
	while ((ch = tc_getc(src)) != TC_EOF) {
		rc = tc_putc(dst, ch);
		if (ch == TC_NEWLINE || rc != TC_OK) {
			break;
		}
	}

	if (ch == TC_EOF) {
		rc = TC_EOF;
	}

	return rc;
}

/*
 * copy n lines from src to dst
 * read until EOF if n < 0
 */
int tc_copylns(int src, int dst, int n) {
	int i;
	int rc;

	rc = TC_OK;
	for (i = 0; i < n || n < 0; i++) {
		rc = tc_copyln(src, dst);
		if (rc != TC_OK) { /* rc is TC_EOF or TC_ERR */
			break;
		}
	}

	return rc;

}

static void tc_putrep(int fd, int n, char ch) {

	const char WARNING = TC_TILDE;
	const int MAXREP = 26;
	const int THRESH = 4;

	while (n >= THRESH || (ch == WARNING && n > 0)) {
		tc_putc(fd, WARNING);
		tc_putc(fd, tc_min(n, MAXREP) - 1 + 'A');
		tc_putc(fd, ch);
		n = n - MAXREP;
	}
	for (n = n; n > 0; n--) {
		tc_putc(fd, ch);
	}
}

void tc_compress(int fd_in, int fd_out) {

	const char WARNING = TC_TILDE;
	char ch;
	char lastch;
	int n;

	n = 1;
	lastch = tc_getc(fd_in);
	while (lastch != TC_EOF) {
		if ((ch = tc_getc(fd_in)) == TC_EOF) {
			if (n > 1 || lastch == WARNING) {
				tc_putrep(fd_out, n, lastch);
			} else {
				tc_putc(fd_out, lastch);
			}
		} else if (ch == lastch) {
			n++;
		} else if (n > 1 || lastch == WARNING) {
			tc_putrep(fd_out, n, lastch);
			n = 1;
		} else {
			tc_putc(fd_out, lastch);
		}
		lastch = ch;
	}
}

void tc_decompress(int fd_in, int fd_out) {

	const char WARNING = TC_TILDE;
	char ch;
	int n;


	while ((ch = tc_getc(fd_in)) != TC_EOF) {

		if (ch != WARNING) {
			tc_putc(fd_out, ch);
		} else if (tc_isupper((ch = tc_getc(fd_in)))) {
			n = ch - 'A' + 1;
			if ((ch = tc_getc(fd_in)) != TC_EOF) {
				for (n = n; n > 0; n--) {
					tc_putc(fd_out, ch);
				}
			} else {
				tc_putc(fd_out, WARNING);
				tc_putc(fd_out, n - 1 + 'A');
			}
		} else {
			tc_putc(fd_out, WARNING);
			if (ch != TC_EOF) {
				tc_putc(fd_out, ch);
			}
		}

	}
}

void tc_putdec(int fd, int n, int w) {
	int i;
	int nd;
	char s[16];

	tc_memset(s, '\0', sizeof(16));

	nd = tc_itoc(n, s, 1);
	for (i = nd; i < w; i++) {
		tc_putc(fd, TC_BLANK);
	}
	for (i = 0; i < nd; i++) {
		tc_putc(fd, s[i]);
	}
}
