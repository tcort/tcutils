/*
    array.c - array utility functions
    Copyright (C) 2022, 2023, 2024  Thomas Cort

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

#include "tc/array.h"
#include "tc/stdlib.h"
#include "tc/sys.h"

char **tc_array_malloc(int nelements) {

	int i;
	char **array;

	array = (char **) tc_malloc(sizeof(char *) * (nelements + 1));
	if (array == TC_NULL) {
		return TC_NULL;
	}

	for (i = 0; i < nelements + 1; i++) {
		array[i] = TC_NULL;
	}

	return array;
}

int tc_array_index_of(char *haystack[], char *needle, int (*compar)(const char *a, const char *b)) {

	int i;

	if (haystack == TC_NULL) {
		return -1;
	}

	for (i = 0; haystack[i] != TC_NULL; i++) {
		if (compar(haystack[i], needle) == 0) {
			return i;
		}
	}

	return -1;
}

int tc_array_length(char *array[]) {
	int i;

	if (array == TC_NULL) {
		return -1;
	}

	for (i = 0; array[i] != TC_NULL; i++) {
		/* continue */;
	}

	return i;
}

void tc_array_copy(char **dst, char **src) {

	int i;
	int len;

	len = tc_array_length(src);
	if (len < 0) {
		return;
	}

	for (i = 0; i < len; i++) {
		dst[i] = src[i];
	}
}

char **tc_array_append(char *old_array[], char *element) {

	int len;
	char **new_array;

	len = tc_array_length(old_array);
	if (len < 0) {
		return TC_NULL;
	}

	new_array = tc_array_malloc(len + 1);
	if (new_array == TC_NULL) {
		old_array = tc_array_free(old_array);
		return TC_NULL;
	}

	tc_array_copy(new_array, old_array);
	new_array[len] = element;
	old_array = tc_free(old_array);

	return new_array;
}

char **tc_array_free(char *array[]) {

	int i;
	int len;

	len = tc_array_length(array);
	if (len < 0) {
		return TC_NULL;
	}

	for (i = 0; i < len; i++) {
		array[i] = tc_free(array[i]);
	}

	return tc_free(array);
}
