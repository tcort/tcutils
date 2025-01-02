/*
    adif.h -- Amateur Data Interchange Format (ADIF)
    Copyright (C) 2022, 2023, 2024  Thomas Cort

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

#ifndef TC_ADIF_H
#define TC_ADIF_H

#include "stdint.h"

#define TC_ADIF_DATA_SPECIFIER_BEGIN ('<')
#define TC_ADIF_DATA_SPECIFIER_SEP (':')
#define TC_ADIF_DATA_SPECIFIER_END ('>')

struct tc_adif_data_specifier {
	char *key;
	tc_uint32_t length;
	char type;
};

struct tc_adif_field {
	struct tc_adif_data_specifier *data_specifier;
	char *data;
};

struct tc_adif_record {
	struct tc_adif_field **fields;
};

typedef struct tc_adif_record tc_adif_header;
typedef struct tc_adif_record tc_adif_qso;

struct tc_adif {
	struct tc_adif_header *header;
	struct tc_adif_qso **qsos;
};

int tc_adif_is_valid_data_type_specifier(char ch);
struct tc_adif *tc_adif_parse(char *adif);

#endif
