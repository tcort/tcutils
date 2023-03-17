/*
    libgen.c - general library functions
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

#include "tc/libgen.h"
#include "tc/limits.h"
#include "tc/stdlib.h"
#include "tc/string.h"
#include "tc/sys.h"

char *tc_basename(char *path) {

	int i;
	int len;
	int all_slashes;
	static char result[TC_PATHMAX];

	if (path == TC_NULL) {
		tc_strcpy(result, ".");
		return result;
	}

	len = tc_strlen(path);

	if (len == 0) {
		tc_strcpy(result, ".");
		return result;
	}

	if (len >= TC_PATHMAX) {
		return TC_NULL;
	}

	all_slashes = 1;
	for (i = 0; i < len; i++) {
		if (path[i] != '/') {
			all_slashes = 0;
			break;
		}
	}

	if (all_slashes == 1) {
		tc_strcpy(result, "/");
		return result;
	}

	tc_strcpy(result, path);

	while (tc_chompd(result, '/') == 1) {
		/* eat trailing slash */;
	}

	i = tc_strrchr(result, '/');
	if (i < 0) {
		i = -1;
	}

	return &(result[i+1]);
}

char *tc_dirname(char *path) {

	int i;
	int len;
	int no_slashes;
	static char result[TC_PATHMAX];

	if (path == TC_NULL) {
		tc_strcpy(result, ".");
		return result;
	}

	len = tc_strlen(path);

	if (len == 0) {
		tc_strcpy(result, ".");
		return result;
	}

	if (len >= TC_PATHMAX) {
		return TC_NULL;
	}

	no_slashes = 1;
	for (i = 0; i < len; i++) {
		if (path[i] == '/') {
			no_slashes = 0;
			break;
		}
	}

	if (no_slashes == 1) {
		tc_strcpy(result, ".");
		return result;
	}

	tc_strcpy(result, path);

	while (tc_chompd(result, '/') == 1) {
		/* eat trailing slash */;
	}

	i = tc_strrchr(result, '/');
	if (i > 0) {
		result[i] = TC_ENDSTR;
	} else {
		result[0] = '/';
		result[1] = TC_ENDSTR;
	}

	return result;
}
