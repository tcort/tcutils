/*
    html.c - HTML functions
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

#include "tc/html.h"
#include "tc/string.h"
#include "tc/stdlib.h"
#include "tc/sys.h"

struct html_entity {
	char ch;
	char *entity;
};

static struct html_entity html_entities[] = {
	{ .ch = '"', 	.entity = "&quot;" },
	{ .ch = '\'', 	.entity = "&apos;" },
	{ .ch = '&', 	.entity = "&amp;" },
	{ .ch = '<', 	.entity = "&lt;" },
	{ .ch = '>', 	.entity = "&gt;" },
	{ .ch = '\0', 	.entity = TC_NULL }
};

char *tc_html_entity(char ch) {

	int i;

	for (i = 0; html_entities[i].entity != TC_NULL; i++) {
		if (ch == html_entities[i].ch) {
			return html_entities[i].entity;
		}
	}

	return TC_NULL;
}

char *tc_html_encode_entities(const char *input) {

	int i;
	char *output;
	char *tmp;

	output = TC_NULL;
	for (i = 0; i < tc_strlen(input); i++) {
		char ch[2] = { input[i], '\0' };
		char *encoded = tc_html_entity(ch[0]);

		tmp = output;
		output = tc_strconcat(output, encoded == TC_NULL ? ch : encoded);
		if (tmp != TC_NULL) {
			tmp = tc_free(tmp);
		}
		if (output == TC_NULL) {
			return TC_NULL;
		}

	}

	return output;
}

struct html_color {
	char *name;
	char *rgb;
};

static struct html_color html_colors[] = {
	{ .name = "aqua",	.rgb = "#00FFFF" },
	{ .name = "black",	.rgb = "#000000" },
	{ .name = "blue",	.rgb = "#0000FF" },
	{ .name = "fuchsia",	.rgb = "#FF00FF" },
	{ .name = "gray",	.rgb = "#808080" },
	{ .name = "green",	.rgb = "#008000" },
	{ .name = "lime",	.rgb = "#00FF00" },
	{ .name = "maroon",	.rgb = "#800000" },
	{ .name = "navy",	.rgb = "#000080" },
	{ .name = "olive",	.rgb = "#808000" },
	{ .name = "purple",	.rgb = "#800080" },
	{ .name = "red",	.rgb = "#FF0000" },
	{ .name = "silver",	.rgb = "#C0C0C0" },
	{ .name = "teal",	.rgb = "#008080" },
	{ .name = "white",	.rgb = "#FFFFFF" },
	{ .name = "yellow",	.rgb = "#FFFF00" },
	{ .name = TC_NULL,	.rgb = TC_NULL }
};

char *tc_html_color_rgb(char *name) {

	int i;

	for (i = 0; html_colors[i].name != TC_NULL; i++) {
		if (tc_streql(name, html_colors[i].name) == 1) {
			return html_colors[i].rgb;
		}
	}

	return TC_NULL;
}
