/*
    check.c - unit testing framework
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

#include "tc/check.h"
#include "tc/const.h"
#include "tc/stdio.h"

/*
 * Execute a series of checks in 'checks'
 * Returns TC_CHECK_PASS if all return it, otherwise return on first fail/skip
 */
int tc_check(struct check *checks) {
	int i;

	for (i = 0; checks[i].message != TC_NULL; i++) {
		int rc;

		rc = checks[i].fn();
		if (rc == 0) {
			tc_puterr("check \"");
			tc_puterr(checks[i].message);
			tc_puterr("\" failed");
			tc_puterrln("");
			return rc;
		}
	}

	return TC_CHECK_PASS;
}

