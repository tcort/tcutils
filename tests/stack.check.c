/*
    stack.c - simple linked list based stack data structure
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

#include <tc/tc.h>

static int check_push_and_pop(void) {

	struct tc_stack * stack;
	int rc;
	void *p;
	char *x = "test";

	stack = tc_stack_malloc();
	if (stack == TC_NULL) {
		return TC_CHECK_FAIL;
	}

	if (!tc_stack_is_empty(stack)) {
		return TC_CHECK_FAIL;
	}

	p = tc_stack_pop(stack);
	if (p != TC_NULL) {
		return TC_CHECK_FAIL;
	}

	if (!tc_stack_is_empty(stack)) {
		return TC_CHECK_FAIL;
	}

	rc = tc_stack_push(stack, x);
	if (rc != 0) {
		return TC_CHECK_FAIL;
	}

	if (tc_stack_is_empty(stack)) {
		return TC_CHECK_FAIL;
	}

	p = tc_stack_pop(stack);
	if (p != x) {
		return TC_CHECK_FAIL;
	}

	stack = tc_stack_free(stack);
	if (stack != TC_NULL) {
		return TC_CHECK_FAIL;
	}

	return TC_CHECK_PASS;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_push_and_pop,	"Push and Pop" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
