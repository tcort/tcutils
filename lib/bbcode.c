/*
    bbcode.c - BBCode to HTML converter
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

#include "tc/array.h"
#include "tc/bbcode.h"
#include "tc/html.h"
#include "tc/stdlib.h"
#include "tc/string.h"
#include "tc/sys.h"

char *tc_bbcode_to_html(char *bbcode) {

	int i;
	int in_tag;
	char *input;
	char *output;
	char *token;

	in_tag = 0;
	output = TC_NULL;
	token = TC_NULL;

	if (bbcode == TC_NULL) {
		return TC_NULL;
	}

	input = tc_html_encode_entities(bbcode);
	if (input == TC_NULL) {
		return TC_NULL;
	}

	for (i = 0; i < tc_strlen(input); i++) {

		int ch = input[i];

		if (in_tag && ch == ']') {
			char s[2] = { ch, '\0' };
			token = tc_strconcat(token, s);
			if (token == TC_NULL) {
				break;
			}

			in_tag = 0;

			if (token[1] == '/') {
				/* end tag */;
			} else if (tc_array_index_of(tc_bbcode_standalone_tags, token, tc_strcmp) != -1) {
				/* stand along tag */;
			} else {
				/* start tag */;
			}

		} else if (!in_tag && ch == '[') {

			i = i - 1;
			in_tag = 1;

			if (tc_strlen(token) > 0) {
				/* this.text(token) */
			}

			token = tc_free(token);

		} else {
			char s[2] = { ch, '\0' };
			token = tc_strconcat(token, s);
			if (token == TC_NULL) {
				break;
			}
		}

	}
	output = tc_strconcat(output, &input[i]);


	input = tc_free(input);
	token = tc_free(token);

	return output;
}
