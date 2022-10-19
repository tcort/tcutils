/*
    errno.c - error number functions
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

#include "tc/errno.h"
#include "tc/stdio.h"
#include "tc/string.h"
#include "tc/sys.h"

char *tc_errlist[] = {
	"OK",
	"Permission denied",
	"Address in use",
	"Address not available",
	"Address family not supported",
	"Resource unavailable, try again (may be the same value as [EWOULDBLOCK])",
	"Connection already in progress",
	"Bad file descriptor",
	"Bad message",
	"Device or resource busy",
	"Operation canceled",
	"No child processes",
	"Connection aborted",
	"Connection refused",
	"Connection reset",
	"Resource deadlock would occur",
	"Destination address required",
	"Mathematics argument out of domain of function",
	"Reserved",
	"File exists",
	"Bad address",
	"File too large",
	"Host is unreachable",
	"Identifier removed",
	"Illegal byte sequence",
	"Operation in progress",
	"Interrupted function",
	"Invalid argument",
	"I/O error",
	"Socket is connected",
	"Is a directory",
	"Too many levels of symbolic links",
	"File descriptor value too large",
	"Too many links",
	"Message too large",
	"Reserved",
	"Filename too long",
	"Network is down",
	"Connection aborted by network",
	"Network unreachable",
	"Too many files open in system",
	"No buffer space available",
	"No message is available on the STREAM head read queue",
	"No such device",
	"No such file or directory",
	"Executable file format error",
	"No locks available",
	"Reserved",
	"Not enough space",
	"No message of the desired type",
	"Protocol not available",
	"No space left on device",
	"No STREAM resources",
	"Not a STREAM",
	"Functionality not supported",
	"The socket is not connected",
	"Not a directory or a symbolic link to a directory",
	"Directory not empty",
	"State not recoverable",
	"Not a socket",
	"Not supported",
	"Inappropriate I/O control operation",
	"No such device or address",
	"Operation not supported on socket",
	"Value too large to be stored in data type",
	"Previous owner died",
	"Operation not permitted",
	"Broken pipe",
	"Protocol error",
	"Protocol not supported",
	"Protocol wrong type for socket",
	"Result too large",
	"Read-only file system",
	"Invalid seek",
	"No such process",
	"Reserved",
	"Stream ioctl() timeout",
	"Connection timed out",
	"Text file busy",
	"Operation would block",
	"Generic Error"
};

int tc_nerror = sizeof(tc_errlist)/sizeof(char*);
enum tc_err tc_errno = TC_EGENERIC;

/*
 * Translate an error code into an English error message
 *
 * Parameters
 *  code - error code
 *
 * Returns pointer to a string containing the error message
 */
char *tc_strerror(enum tc_err code) {
	if (code >= tc_nerror || code < 0) {
		code = TC_EGENERIC;
	}
	return tc_errlist[code];
}

/*
 * Print the current error to standard error
 */
void tc_perror(void) {
	tc_puts(TC_STDERR, tc_strerror(tc_errno));
	tc_puts(TC_STDERR, "\n");
}
