/*
    stdint.h - standard integer types
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

#ifndef TC_STDINT_H
#define TC_STDINT_H

typedef char tc_int8_t;
typedef unsigned char tc_uint8_t;

#if __SIZEOF_SHORT__ == 2
typedef short tc_int16_t;
typedef unsigned short tc_uint16_t;
#else
#error "strange integer width -- please update tc/stdint.h"
#endif

#if __SIZEOF_INT__ == 4
typedef int tc_int32_t;
typedef unsigned int tc_uint32_t;
#else
#error "strange integer width -- please update tc/stdint.h"
#endif

#if __SIZEOF_LONG__ == 8
typedef long int tc_int64_t;
typedef unsigned long int tc_uint64_t;
#else
#error "strange integer width -- please update tc/stdint.h"
#endif

#endif
