/*
    string.c - string functions
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

#include "tc/const.h"
#include "tc/ctype.h"
#include "tc/math.h"
#include "tc/string.h"
#include "tc/sys.h"

/*
 * Sets len bytes of string s to character ch
 */
void tc_memset(char *s, char ch, int len) {
	int i;

	for (i = 0; i < len; i++) {
		s[i] = ch;
	}
}

/*
 * Copies a string. Destination must be equal to or longer than source.
 */
void tc_strcpy(char *dst, char *src) {
	int i;

	for (i = 0; i < tc_strlen(src); i++) {
		dst[i] = src[i];
	}

	dst[i] = '\0';
}

/*
 * Create a new string which is larger than the input string s.
 * Copy the input string to the new string and free the input.
 * Returns the expanded string.
 */
char *tc_strgrow(char *s, int newsize) {

	char *d;

	if (tc_strlen(s) + 1 > newsize) {
		s = tc_free(s);
		return TC_NULL;
	}

	d = tc_malloc(newsize);
	if (d == TC_NULL) {
		s = tc_free(s);
		return TC_NULL;
	}

	tc_strcpy(d, s);
	s = tc_free(s);

	return d;
}

/*
 * Computes the length of a string.
 * Return the length of string s.
 */
int tc_strlen(char *s) {
	int n;

	if (s == TC_NULL) {
		return 0;
	}

	n = 0;
	while (s[n] != TC_ENDSTR) {
		n++;
	}

	return n;
}

/*
 * Compare upto n characters of string x with string y
 * This is not a constant time equality check.
 * Returns 1 if equal, 0 if not equal
 */
int tc_strneql(char *x, char *y, int n) {

	int i;

	if (x == y) { /* both the same */
		return 1;
	} else if (x == TC_NULL || y == TC_NULL) { /* one null */
		return 0;
	} else if (tc_strlen(x) < n || tc_strlen(y) < n) { /* at least n */
		return 0;
	}

	for (i = 0; i < n; i++) {
		if (x[i] != y[i]) {
			return 0;
		}
	}

	return 1;
}

/*
 * Compare string x with string y
 * This is not a constant time equality check.
 * Returns 1 if equal, 0 if not equal
 */
int tc_streql(char *x, char *y) {

	int i;

	if (x == y) { /* both the same */
		return 1;
	} else if (x == TC_NULL || y == TC_NULL) { /* one null */
		return 0;
	} else if (tc_strlen(x) != tc_strlen(y)) { /* different lengths */
		return 0;
	}

	for (i = 0; i < tc_strlen(x); i++) {
		if (x[i] != y[i]) {
			return 0;
		}
	}

	return 1;
}

/*
 * Duplicate string s
 * Return a new string which is a clone of the input
 */
char *tc_strdup(char *s) {

	int len;
	char *d;

	len = tc_strlen(s) + 1;

	d = tc_malloc(len);
	if (d == TC_NULL) {
		return TC_NULL;
	}

	tc_strcpy(d, s);
	return d;
}

static int tc_utoa_rec(unsigned int n, char *s, int len, int i);

/*
 * Internal recursive portion of tc_utoa()
 * Adds one character at a time to s.
 */
static int tc_utoa_rec(unsigned int n, char *s, int len, int i) {

	if (i >= len - 2) { /* don't overflow s */
		return i;
	}

	if (n >= 10) {
		i = tc_utoa_rec(n / 10, s, len, i);
	}

	s[i] = (n % 10) + '0';
	s[i+1] = TC_ENDSTR;
	return i + 1;
}

/*
 * Builds a string representation of the unsigned integer n
 * Returns a string on the heap representing the input integer. Caller frees.
 */
char *tc_utoa(unsigned int n) {

	char *s;
	int len;

	len = 64; /* big enough to hold 128-bit unsigned int */

	s = tc_malloc(len);
	if (s == TC_NULL) {
		return TC_NULL;
	}

	tc_utoa_rec(n, s, len, 0);

	return s;
}

static int tc_itoa_rec(int n, char *s, int len, int i);

/*
 * Internal recursive portion of tc_itoa()
 * Adds one character at a time to s.
 */
static int tc_itoa_rec(int n, char *s, int len, int i) {

	if (i >= len - 2) { /* don't overflow s */
		return i;
	}

	if (n < 0) {
		s[i] = '-';
		return tc_itoa_rec(-n, s, len, i+1);
	}

	if (n >= 10) {
		i = tc_itoa_rec(n / 10, s, len, i);
	}

	s[i] = (n % 10) + '0';
	s[i+1] = TC_ENDSTR;
	return i + 1;
}

/*
 * Builds a string representation of the integer n
 * Returns a string on the heap representing the input integer. Caller frees.
 */
char *tc_itoa(int n) {

	char *s;
	int len;

	len = 32; /* big enough to hold 128-bit signed int */

	s = tc_malloc(len);
	if (s == TC_NULL) {
		return TC_NULL;
	}

	tc_itoa_rec(n, s, len, 0);

	return s;
}

/*
 * Convert a string s containing digits and optional '-' sign to an int
 * Returns the integer representation of the string of digits in s
 */
int tc_atoi(char *s) {
	int i;
	int neg;
	int r;

	if (s == TC_NULL || s[0] == TC_ENDSTR) {
		return 0;
	}

	neg = (s[0] == '-');

	r = 0;
	for (i = neg; i < tc_strlen(s) && tc_isdigit(s[i]); i++) {
		r = (r * 10) + (s[i] - '0');
	}

	return (neg == 0 ? 1 : -1) * r;
}

int tc_strchr(char *s, char c) {
	int i;

	for (i = 0; i < tc_strlen(s); i++) {
		if (s[i] == c) {
			return i;
		}
	}

	return -1;
}

int tc_strrchr(char *s, char c) {
	int i;

	for (i = tc_strlen(s) - 1; i >= 0; i--) {
		if (s[i] == c) {
			return i;
		}
	}

	return -1;
}

void tc_chompd(char *s, int delimiter) {
	int i;

	i = tc_strchr(s, delimiter);
	if (i != -1) {
		s[i] = TC_ENDSTR;
	}
}

void tc_chomp(char *s) {
	return tc_chompd(s, TC_NEWLINE);
}

int tc_strlist_includes(char *haystack[], char *needle) {
	int i;
	for (i = 0; haystack[i] != TC_NULL; i++) {
		if (tc_streql(haystack[i], needle) == 1) {
			return 1;
		}
	}
	return 0;
}

