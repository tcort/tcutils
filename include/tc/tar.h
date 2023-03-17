/*
    tar.h -- tape archive definitions
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

#ifndef TC_TAR_H
#define TC_TAR_H

struct tar_hdr {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char chksum[8];
	char typeflag;
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
};

/* magic string */
#define TMAGIC ("ustar")
/* length of magic string with a null byte */
#define TMAGLEN (6)
/* version */
#define TVERSION ("00")
/* length of version without a null byte */
#define TVERSLEN (2)

#define AREGTYPE ('\0') /* regular file */
#define REGTYPE ('0') /* regular file */
#define LNKTYPE ('1') /* link */
#define SYMTYPE ('2') /* sym link */
#define CHRTYPE ('3') /* character device */
#define BLKTYPE ('4') /* block device */
#define DIRTYPE ('5') /* directory */
#define FIFOTYPE ('6') /* fifo */
#define CONTTYPE ('7') /* reserved */

#define TSUID (04000) /* set uid */
#define TSGID (02000) /* set gid */
#define TSVTX (01000) /* on directory, restricted delete */
#define TUREAD (00400) /* read by owner */
#define TUWRITE (00200) /* write by owner */
#define TUEXEC (00100) /* exec by owner */
#define TGREAD (00040) /* read by group */
#define TGWRITE (00020) /* write by group */
#define TGEXEC (00010) /* exec by group */
#define TOREAD (00004) /* read by other */
#define TOWRITE (00002) /* write by other */
#define TOEXEC (00001) /* exec by other */

#endif
