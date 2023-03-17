/*
    check.c - unit testing framework
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

#include "tc/check.h"
#include "tc/stdio.h"
#include "tc/stdlib.h"

/*
 * Execute a series of checks in 'checks'
 *
 * Parameters:
 *  checks - a set of checks to execute
 *
 * Returns TC_CHECK_PASS if all pass, otherwise TC_CHECK_FAIL on first fail
 */
int tc_check(struct check *checks) {
	int i;

	if (checks == TC_NULL) {
		tc_puterr("list of checks is NULL\n");
		return TC_CHECK_FAIL;
	}

	for (i = 0; checks[i].message != TC_NULL; i++) {
		int rc;

		rc = checks[i].fn();
		if (rc == TC_CHECK_FAIL) {
			tc_puterr("check \"");
			tc_puterr(checks[i].message);
			tc_puterr("\" failed\n");
			return TC_CHECK_FAIL;
		}
	}

	return TC_CHECK_PASS;
}

