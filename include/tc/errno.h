/*
    errno.h - error number functions
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

#ifndef TC_ERRNO_H
#define TC_ERRNO_H

enum tc_err {
	TC_EOK = 0,
	TC_EACCES = 1,
	TC_EADDRINUSE = 2,
	TC_EADDRNOTAVAIL = 3,
	TC_EAFNOSUPPORT = 4,
	TC_EAGAIN = 5,
	TC_EALREADY = 6,
	TC_EBADF = 7,
	TC_EBADMSG = 8,
	TC_EBUSY = 9,
	TC_ECANCELED = 10,
	TC_ECHILD = 11,
	TC_ECONNABORTED = 12,
	TC_ECONNREFUSED = 13,
	TC_ECONNRESET = 14,
	TC_EDEADLK = 15,
	TC_EDESTADDRREQ = 16,
	TC_EDOM = 17,
	TC_EDQUOT = 18,
	TC_EEXIST = 19,
	TC_EFAULT = 20,
	TC_EFBIG = 21,
	TC_EHOSTUNREACH = 22,
	TC_EIDRM = 23,
	TC_EILSEQ = 24,
	TC_EINPROGRESS = 25,
	TC_EINTR = 26,
	TC_EINVAL = 27,
	TC_EIO = 28,
	TC_EISCONN = 29,
	TC_EISDIR = 30,
	TC_ELOOP = 31,
	TC_EMFILE = 32,
	TC_EMLINK = 33,
	TC_EMSGSIZE = 34,
	TC_EMULTIHOP = 35,
	TC_ENAMETOOLONG = 36,
	TC_ENETDOWN = 37,
	TC_ENETRESET = 38,
	TC_ENETUNREACH = 39,
	TC_ENFILE = 40,
	TC_ENOBUFS = 41,
	TC_ENODATA = 42,
	TC_ENODEV = 43,
	TC_ENOENT = 44,
	TC_ENOEXEC = 45,
	TC_ENOLCK = 46,
	TC_ENOLINK = 47,
	TC_ENOMEM = 48,
	TC_ENOMSG = 49,
	TC_ENOPROTOOPT = 50,
	TC_ENOSPC = 51,
	TC_ENOSR = 52,
	TC_ENOSTR = 53,
	TC_ENOSYS = 54,
	TC_ENOTCONN = 55,
	TC_ENOTDIR = 56,
	TC_ENOTEMPTY = 57,
	TC_ENOTRECOVERABLE = 58,
	TC_ENOTSOCK = 59,
	TC_ENOTSUP = 60,
	TC_ENOTTY = 61,
	TC_ENXIO = 62,
	TC_EOPNOTSUPP = 63,
	TC_EOVERFLOW = 64,
	TC_EOWNERDEAD = 65,
	TC_EPERM = 66,
	TC_EPIPE = 67,
	TC_EPROTO = 68,
	TC_EPROTONOSUPPORT = 69,
	TC_EPROTOTYPE = 70,
	TC_ERANGE = 71,
	TC_EROFS = 72,
	TC_ESPIPE = 73,
	TC_ESRCH = 74,
	TC_ESTALE = 75,
	TC_ETIME = 76,
	TC_ETIMEDOUT = 77,
	TC_ETXTBSY = 78,
	TC_EWOULDBLOCK = 79,
	TC_EGENERIC = 80
};

extern char *tc_errlist[];
extern int tc_nerror;
extern enum tc_err tc_errno;

char *tc_strerror(enum tc_err code);
void tc_perror(void);

#endif
