/*
    string.h - string functions
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

#ifndef TC_STRING_H
#define TC_STRING_H

/* CHARACTER CONSTANTS */
#define TC_ENDSTR ('\0')
#define TC_NEWLINE ('\n')

void tc_memset(char *s, char ch, int len);
void tc_strcpy(char *dst, const char *src);
char *tc_strgrow(char *s, int newsize);
int tc_strlen(const char *s);
int tc_strneql(const char *x, const char *y, int n);
int tc_streql(const char *x, const char *y);
int tc_strncaseeql(const char *x, const char *y, int n);
int tc_strcaseeql(const char *x, const char *y);
char *tc_strdup(const char *s);
char *tc_itoa(int n);
char *tc_utoa(unsigned int n);
int tc_ctoi(int ch);
char tc_itox(int i);
int tc_atoi(char *s);
int tc_strchr(char *s, char c);
int tc_strrchr(char *s, char c);
int tc_chomp(char *s);
int tc_chompd(char *s, int delimiter);
int tc_strlist_includes(char *haystack[], char *needle);
char *tc_strstr(char *haystack, char *needle);

#endif
