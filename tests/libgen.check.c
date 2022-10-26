/*
    libgen.c - general library functions
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

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <tc/tc.h>

static int check_basename_path(void) {
	return tc_streql(tc_basename("/usr/lib"), "lib") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_dirname_path(void) {
	return tc_streql(tc_dirname("/usr/lib"), "/usr") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_basename_trail(void) {
	return tc_streql(tc_basename("/usr/"), "usr") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_dirname_trail(void) {
	return tc_streql(tc_dirname("/usr/"), "/") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_basename_single(void) {
	return tc_streql(tc_basename("usr"), "usr") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_dirname_single(void) {
	return tc_streql(tc_dirname("usr"), ".") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_basename_slash(void) {
	return tc_streql(tc_basename("/"), "/") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_dirname_slash(void) {
	return tc_streql(tc_dirname("/"), "/") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_basename_dot(void) {
	return tc_streql(tc_basename("."), ".") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_dirname_dot(void) {
	return tc_streql(tc_dirname("."), ".") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_basename_dotdot(void) {
	return tc_streql(tc_basename(".."), "..") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

static int check_dirname_dotdot(void) {
	return tc_streql(tc_dirname(".."), ".") == 1 ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_basename_path, "basename /usr/lib is lib" },
		{ check_dirname_path, "dirname /usr/lib is /usr" },
		{ check_basename_trail, "basename /usr/ is usr" },
		{ check_dirname_trail, "dirname /usr/ is /" },
		{ check_basename_single, "basename usr is usr" },
		{ check_dirname_single, "dirname usr is ." },
		{ check_basename_slash, "basename / is /" },
		{ check_dirname_slash, "dirname / is /" },
		{ check_basename_dot, "basename . is ." },
		{ check_dirname_dot, "dirname . is ." },
		{ check_basename_dotdot, "basename .. is .." },
		{ check_dirname_dotdot, "dirname .. is ." },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
