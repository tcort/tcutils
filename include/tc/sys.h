/*
    sys.h - system calls
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

#ifndef TC_SYS_H
#define TC_SYS_H

char tc_getc(int fd);
int tc_putc(int fd, char ch);
int tc_open_reader(char *filepath);
int tc_open_writer(char *filepath);
int tc_close(int fd);
void *tc_malloc(int size);
void *tc_free(void *m);
void tc_exit(int exit_code);
int tc_getpid(void);
char *tc_ttyname(int fd);
int tc_isatty(int fd);
void tc_sync(void);
unsigned int tc_sleep(unsigned int seconds);

#endif
