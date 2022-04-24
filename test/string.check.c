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

#include "tc/const.h"
#include "tc/check.h"
#include "tc/string.h"
#include "tc/sys.h"

static int check_strlen_null(void) { return tc_strlen(TC_NULL) == 0; }
static int check_strlen_empty(void) { return tc_strlen("") == 0; }
static int check_strlen_one(void) { return tc_strlen("X") == 1; }
static int check_strlen_many(void) { return tc_strlen("foo") == 3; }

static int check_itoa_neg(void) {
	int rc;
	char *s;
	s = tc_itoa(-42);
	rc = tc_streql(s, "-42");
	s = tc_free(s);
	return rc;
}
static int check_itoa_pos(void) {
	int rc;
	char *s;
	s = tc_itoa(1337);
	rc = tc_streql(s, "1337");
	s = tc_free(s);
	return rc;
}
static int check_itoa_zero(void) {
	int rc;
	char *s;
	s = tc_itoa(0);
	rc = tc_streql(s, "0");
	s = tc_free(s);
	return rc;
}

static int check_utoa_small(void) {
	int rc;
	char *s;
	s = tc_utoa(1337);
	rc = tc_streql(s, "1337");
	s = tc_free(s);
	return rc;
}
static int check_utoa_big(void) {
	int rc;
	char *s;
	s = tc_utoa(4294967295);
	rc = tc_streql(s, "4294967295");
	s = tc_free(s);
	return rc;
}
static int check_atoi_pos(void) { return tc_atoi("1337") == 1337; }
static int check_atoi_neg(void) { return tc_atoi("-42") == -42; }
static int check_atoi_zero(void) { return tc_atoi("0") == 0; }
static int check_atoi_short(void) { return tc_atoi("1") == 1; }
static int check_atoi_long(void) { return tc_atoi("12345678") == 12345678; }
static int check_atoi_mix(void) { return tc_atoi("123XYZ") == 123; }
static int check_atoi_empty(void) { return tc_atoi("") == 0; }
static int check_atoi_char(void) { return tc_atoi("X") == 0; }

static int check_strneql_yes_a(void) { return tc_strneql("--version", "--", 2) == 1; }
static int check_strneql_yes_b(void) { return tc_strneql("--", "--version", 2) == 1; }
static int check_strneql_yes_c(void) { return tc_strneql("--", "--", 2) == 1; }

static int check_strcaseeql_yes(void) { return tc_strcaseeql("FoO", "fOo") == 1; }
static int check_strcaseeql_no(void) { return tc_strcaseeql("Fo0", "fOo") == 0; }
static int check_strncaseeql_yes(void) { return tc_strncaseeql("FoOd", "fOop", 3) == 1; }
static int check_strncaseeql_no(void) { return tc_strncaseeql("Fo0d", "fOop", 3) == 0; }

static int check_strchr_none(void) { return tc_strchr("foo", '-') == -1; }
static int check_strchr_first(void) { return tc_strchr("foo", 'f') == 0; }
static int check_strchr_mid(void) { return tc_strchr("foo", 'o') == 1; }
static int check_strchr_last(void) { return tc_strchr("food", 'd') == 3; }

static int check_strrchr_none(void) { return tc_strchr("foo", '-') == -1; }
static int check_strrchr_first(void) { return tc_strchr("foo", 'f') == 0; }
static int check_strrchr_mid(void) { return tc_strchr("food", 'o') == 2; }
static int check_strrchr_last(void) { return tc_strchr("foo", 'o') == 2; }

static int check_chomp(void) {
	char *s = "foo\n";
	tc_chomp(s);
	return tc_streql("foo", s) == 1;
}
static int check_chompd(void) {
	char *s = "foobar";
	tc_chompd(s, 'r');
	return tc_streql("fooba", s) == 1;
}

static int check_strlist_includes_first(void) {
	char *list[] = { "foo", "bar", "baz", TC_NULL };
	return tc_strlist_includes(list, "foo") == 1;
}
static int check_strlist_includes_mid(void) {
	char *list[] = { "foo", "bar", "baz", TC_NULL };
	return tc_strlist_includes(list, "bar") == 1;
}
static int check_strlist_includes_last(void) {
	char *list[] = { "foo", "bar", "baz", TC_NULL };
	return tc_strlist_includes(list, "baz") == 1;
}
static int check_strlist_includes_not_found(void) {
	char *list[] = { "foo", "bar", "baz", TC_NULL };
	return tc_strlist_includes(list, "quux") == 0;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_strlen_null,	"length of TC_NULL is 0" },
		{ check_strlen_empty,	"length of empty string is 0" },
		{ check_strlen_one,	"length of \"X\" string is 1" },
		{ check_strlen_many,	"length of \"foo\" string is 3" },
		{ check_itoa_neg,	"int(-42) becomes \"-42\"" },
		{ check_itoa_pos,	"int(1337) becomes \"1337\"" },
		{ check_itoa_zero,	"int(0) becomes \"0\"" },
		{ check_utoa_big,       "uint(4294967295) becomes \"4294967295\"" },
		{ check_utoa_small,	"uint(1337) becomes \"1337\"" },
		{ check_atoi_pos,	"\"1337\" becomes int(1337)" },
		{ check_atoi_neg,	"\"-42\" becomes int(-42)" },
		{ check_atoi_zero,	"\"0\" becomes int(0)" },
		{ check_atoi_short,	"\"1\" becomes int(1)" },
		{ check_atoi_long,	"\"12345678\" becomes int(12345678)" },
		{ check_atoi_mix,	"\"123XYZ\" becomes int(123)" },
		{ check_atoi_empty,	"\"\" becomes int(0)" },
		{ check_atoi_char,	"\"X\" becomes int(0)" },
		{ check_strneql_yes_a,	"\"--version\" strneql(2) \"--\"" },
		{ check_strneql_yes_b,	"\"--\" strneql(2) \"--version\"" },
		{ check_strneql_yes_c,	"\"--\" strneql(2) \"--\"" },
		{ check_strcaseeql_yes, "\"fOo\" strcaseeql() \"FoO\" is 1" },
		{ check_strcaseeql_no,	"\"fOo\" strcaseeql() \"FoO\" is 0" },
		{ check_strncaseeql_yes,"\"fOod\" strncaseeql(3) \"FoOp\" is 1" },
		{ check_strncaseeql_no, "\"f0od\" strncaseeql(3) \"FoOp\" is 0" },
		{ check_strchr_none,	"\"foo\" strchr('-') is -1" },
		{ check_strchr_first,	"\"foo\" strchr('f') is 0" },
		{ check_strchr_mid,	"\"foo\" strchr('o') is 1" },
		{ check_strchr_last,	"\"food\" strchr('d') is 3" },
		{ check_strrchr_none,	"\"foo\" strrchr('-') is -1" },
		{ check_strrchr_first,	"\"foo\" strrchr('f') is 0" },
		{ check_strrchr_mid,	"\"foo\" strrchr('o') is 2" },
		{ check_strrchr_last,	"\"food\" strrchr('d') is 3" },
		{ check_chomp,		"\"foo\\n\" chomp is \"foo\"" },
		{ check_chompd,		"\"foobar\\n\" chompd('r') is \"fooba\"" },
		{ check_strlist_includes_first, "finds \"foo\" in list" },
		{ check_strlist_includes_mid, "finds \"bar\" in list" },
		{ check_strlist_includes_last, "finds \"baz\" in list" },
		{ check_strlist_includes_not_found, "does not find \"quux\" in list" },
		{ TC_NULL, TC_NULL }
	};


	return tc_check(checks);
}
