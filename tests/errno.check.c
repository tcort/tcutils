/*
    errno.c - error number functions
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

int check_generic(void) { return tc_streql(tc_strerror(TC_EGENERIC), "Generic Error") ? TC_CHECK_PASS : TC_CHECK_FAIL; }
int check_io(void) { return tc_streql(tc_strerror(TC_EIO), "I/O error") ? TC_CHECK_PASS : TC_CHECK_FAIL; }
int check_fbig(void) { return tc_streql(tc_strerror(TC_EFBIG), "File too large") ? TC_CHECK_PASS : TC_CHECK_FAIL; }

int check_errn_low(void) { return tc_streql(tc_strerror(-1), "Generic Error") ? TC_CHECK_PASS : TC_CHECK_FAIL; }
int check_errn_high(void) { return tc_streql(tc_strerror(9999999), "Generic Error") ? TC_CHECK_PASS : TC_CHECK_FAIL; }

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_generic, "TC_EGENERIC defined and has error message" },
		{ check_io, "TC_EIO defined and has error message" },
		{ check_fbig, "TC_EFBIG defined and has error message" },
		{ check_errn_low, "-1 maps to generic error" },
		{ check_errn_high, "9999999 maps to generic error" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
