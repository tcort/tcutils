/*
    sys.c - system calls
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
#include "tc/string.h"
#include "tc/sys.h"

#include <fcntl.h> /* open(2)/O_RDONLY/O_WRONLY */
#include <stdlib.h> /* malloc(2)/free(2)/exit(2) */
#include <sys/stat.h> /* mkdir(2) */
#include <unistd.h> /* getpid(2)/read(2)/write(2)/close(2)/ttyname(2)/sync(2)/sleep(2)/rmdir(2) */

/*
 * Read a character from file descriptor fd
 * returns the character or TC_EOF or TC_ERR
 */
char tc_getc(int fd) {

	int rc;
	char s[1];

	rc = read(fd, s, 1);
	if (rc == 0) {
		return TC_EOF;
	} else if (rc == -1) {
		return TC_ERR;
	}

	return s[0];
}

/*
 * Write a character ch to file descriptor fd
 * returns TC_ERR or TC_OK
 */
int tc_putc(int fd, char ch) {

	int rc;
	char s[1];

	s[0] = ch;

	rc = write(fd, s, 1);
	if (rc == -1) {
		return TC_ERR;
	}

	return TC_OK;
}

/*
 * Open the file at filepath for reading
 * returns file descriptor or TC_ERR
 */
int tc_open_reader(char *filepath) {
	int fd;

	fd = open(filepath, O_RDONLY);
	if (fd == -1) {
		return TC_ERR;
	}

	return fd;
}

/*
 * Open the file at filepath for writing
 * returns file descriptor or TC_ERR
 */
int tc_open_writer(char *filepath) {
	int fd;

	fd = open(filepath, O_WRONLY);
	if (fd == -1) {
		return TC_ERR;
	}

	return fd;
}

/*
 * Close a file descriptor
 * returns TC_OK or TC_ERR
 */
int tc_close(int fd) {
	int rc;

	rc = close(fd);
	if (rc == -1) {
		return TC_ERR;
	}

	return TC_OK;
}

/*
 * Allocates size bytes of memory on the heap
 * returns pointer to the memory or TC_NULL
 */
void *tc_malloc(int size) {

	void *m;

	if (size <= 0) {
		return TC_NULL;
	}

	m = malloc(size);
	if (m == TC_NULL) {
		return TC_NULL;
	}
	tc_memset(m, TC_ENDSTR, size);

	return m;
}

/*
 * Frees heap memory pointed to by pointer m
 * returns TC_NULL
 */
void *tc_free(void *m) {
	if (m != TC_NULL) {
		free(m);
		m = TC_NULL;
	}
	return TC_NULL;
}

/*
 * Kills the process and returns the given exit code
 */
void tc_exit(int exit_code) {
	exit(exit_code);
}

/*
 * Get the process ID of the current process
 */
int tc_getpid(void) {
	return (int) getpid();
}

/*
 * Get the name of a tty from a file descriptor
 * returns a null terminated string or TC_NULL
 */
char *tc_ttyname(int fd) {
	char *s;
	s = ttyname(fd);

	if (s) {
		return s;
	} else {
		return TC_NULL;
	}
}

/*
 * determine if fd refers to a valid tty devices
 * returns 1 or 0
 */
int tc_isatty(int fd) {
	return (tc_ttyname(fd) != TC_NULL);
}

/*
 * flush file system buffers to disk
 */
void tc_sync(void) {
	sync();
}

/*
 * remove a directory from the file system
 */
int tc_rmdir(char *dir) {
	int rc;

	rc =  rmdir(dir);
	if (rc == -1) {
		return TC_ERR;
	}
	return TC_OK;
}

/*
 * create a directory on the file system
 */
int tc_mkdir(char *dir, tc_mode_t mode) {
	int rc;

	rc = mkdir(dir, mode);
	if (rc == -1) {
		return TC_ERR;
	}
	return TC_OK;
}

/*
 * sleep for a given number of seconds
 * return 0 or the remaining time if interrupted
 */
unsigned int tc_sleep(unsigned int seconds) {
	return sleep(seconds);
}
