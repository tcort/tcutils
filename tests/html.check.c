/*
    string.c - string functions
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

#include <tc/tc.h>

static int check_html_entity_not_found(void) {
	return tc_html_entity('X') == TC_NULL ? TC_CHECK_PASS: TC_CHECK_FAIL;
}

static int check_html_entity_found(void) {
	return tc_streql(tc_html_entity('"'), "&quot;") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_html_color_not_found(void) {
	return tc_html_color_rgb("lemonchiffon") == TC_NULL ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_html_color_found(void) {
	return tc_streql(tc_html_color_rgb("purple"), "#800080") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_html_entity_not_found,	"returns TC_NULL when no entity" },
		{ check_html_entity_found,	"\" returns \"&quot;\"" },
		{ check_html_color_not_found,	"returns TC_NULL when no color" },
		{ check_html_color_found,	"\"purple\" returns \"#800080\"" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
