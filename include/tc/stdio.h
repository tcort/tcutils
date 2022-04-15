/*
    stdio.h - standard input/output
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

#ifndef TC_STDIO_H
#define TC_STDIO_H

char *tc_getln(int fd, int *status);
int tc_puterr(char *msg);
int tc_puterrln(char *msg);
int tc_puts(int fd, char *s);
int tc_putln(int fd, char *s);

#endif
