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

#include "tc/adif.h"
#include "tc/ctype.h"
#include "tc/stdlib.h"
#include "tc/string.h"
#include "tc/sys.h"

int tc_adif_is_valid_data_type_specifier(char ch) {
	switch (tc_toupper(ch)) {
		case 'B': return 1; /* Boolean */
		case 'N': return 1; /* Number */
		case 'D': return 1; /* Date */
		case 'T': return 1; /* Time */
		case 'S': return 1; /* String */
		case 'M': return 1; /* Multiline String */
		case 'E': return 1; /* Enumeration */
		case 'L': return 1; /* Location */
		default : return 0;
	}

}

static int tc_adif_find_char(char *adif, int *i, char ch) {
	return tc_strchr(&(adif[*i]), ch);
}

static int tc_adif_find_data_specifier_begin(char *adif, int *i) {
	return tc_adif_find_char(adif, i, TC_ADIF_DATA_SPECIFIER_BEGIN);
}

static int tc_adif_find_data_specifier_sep(char *adif, int *i) {
	return tc_adif_find_char(adif, i, TC_ADIF_DATA_SPECIFIER_SEP);
}

static int tc_adif_find_data_specifier_end(char *adif, int *i) {
	return tc_adif_find_char(adif, i, TC_ADIF_DATA_SPECIFIER_END);
}

struct tc_adif_data_specifier *tc_adif_parse_data_specifier(char *adif, int *i) {

	int pos;
	struct tc_adif_data_specifier *ds;

	pos = tc_adif_find_data_specifier_begin(adif, i);
	if (pos == -1) {
		return TC_NULL;
	}

	*i = pos + 1; /* advance to start of key */



	return ds;
}

struct tc_adif *tc_adif_parse(char *adif) {

	int i;

	i = 0;

	if (adif == TC_NULL || tc_strlen(adif) == 0) {
		return TC_NULL;
	}

	return TC_NULL;
}
