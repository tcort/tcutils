/*
    array.h - array utility functions
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

#ifndef TC_ARRAY_H
#define TC_ARRAY_H

char **tc_array_malloc(int nelements);
int tc_array_index_of(char *haystack[], char *needle, int (*compar)(const char *a, const char *b));
int tc_array_length(char *array[]);
void tc_array_copy(char **dst, char **src);
char **tc_array_append(char *old_array[], char *element);
char **tc_array_free(char *array[]);

#endif
