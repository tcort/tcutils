/*
    stdlib.h - standard library
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

#ifndef TC_STDLIB_H
#define TC_STDLIB_H

#include "stdint.h"

/* RETURN CODES */
#define TC_OK (0)
#define TC_ERR (-1)
#define TC_EOF (-2)

/* SPECIAL CHARS */
#define TC_TILDE ('~')

/* EXIT CODES */
#define TC_EXIT_SUCCESS (0)
#define TC_EXIT_FAILURE (1)

/* MISC */
#define TC_NULL ((void *) 0)

void tc_srand(unsigned seed);
int tc_rand(void);
tc_uint32_t tc_seed(void);

#endif
