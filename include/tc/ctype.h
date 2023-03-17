/*
    ctype.h - character type functions
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

#ifndef TC_CTYPE_H
#define TC_CTYPE_H

int tc_isascii(int ch);
int tc_isdigit(int ch);
int tc_isupper(int ch);
int tc_islower(int ch);
int tc_isalpha(int ch);
int tc_isalnum(int ch);
int tc_isspace(int ch);
int tc_isblank(int ch);
int tc_isxdigit(int ch);
int tc_ispunct(int ch);
int tc_iscntrl(int ch);
int tc_isgraph(int ch);
int tc_isprint(int ch);

int tc_tolower(int ch);
int tc_toupper(int ch);

#endif
