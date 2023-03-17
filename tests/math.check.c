/*
    math.c - math functions
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

#include <tc/tc.h>

static int check_min_pos_neg(void) { return tc_min( 4, -3) == -3 ? TC_CHECK_PASS : TC_CHECK_FAIL; }
static int check_min_neg_neg(void) { return tc_min(-4, -3) == -4 ? TC_CHECK_PASS : TC_CHECK_FAIL; }
static int check_min_pos_pos(void) { return tc_min( 4,  3) ==  3 ? TC_CHECK_PASS : TC_CHECK_FAIL; }
static int check_min_neg_pos(void) { return tc_min(-4,  3) == -4 ? TC_CHECK_PASS : TC_CHECK_FAIL; }

static int check_max_pos_neg(void) { return tc_max( 4, -3) ==  4 ? TC_CHECK_PASS : TC_CHECK_FAIL; }
static int check_max_neg_neg(void) { return tc_max(-4, -3) == -3 ? TC_CHECK_PASS : TC_CHECK_FAIL; }
static int check_max_pos_pos(void) { return tc_max( 4,  3) ==  4 ? TC_CHECK_PASS : TC_CHECK_FAIL; }
static int check_max_neg_pos(void) { return tc_max(-4,  3) ==  3 ? TC_CHECK_PASS : TC_CHECK_FAIL; }

static int check_abs_pos(void) { return tc_abs( 4) ==  4 ? TC_CHECK_PASS : TC_CHECK_FAIL; }
static int check_abs_neg(void) { return tc_abs(-4) ==  4 ? TC_CHECK_PASS : TC_CHECK_FAIL; }

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_min_pos_neg,	"min( 4,-3) is -3" },
		{ check_min_neg_neg,	"min(-4,-3) is -4" },
		{ check_min_pos_pos,	"min( 4, 3) is  3" },
		{ check_min_neg_pos,	"min(-4, 3) is -4" },
		{ check_max_pos_neg,	"max( 4,-3) is  4" },
		{ check_max_neg_neg,	"max(-4,-3) is -3" },
		{ check_max_pos_pos,	"max( 4, 3) is  4" },
		{ check_max_neg_pos,	"max(-4, 3) is  3" },
		{ check_abs_pos,	"abs( 4) is 4" },
		{ check_abs_neg,	"abs(-4) is 4" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
