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

#include "tc/stack.h"
#include "tc/stdlib.h"
#include "tc/sys.h"

int tc_stack_push(struct tc_stack *stack, void *data) {

	struct tc_stack_datum *new_top;

	new_top = tc_malloc(sizeof(struct tc_stack_datum));
	if (new_top == TC_NULL) {
		return -1;
	}

	new_top->data = data;
	new_top->next = stack->top;
	stack->top = new_top;

	return 0;
}

void *tc_stack_pop(struct tc_stack *stack) {

	void *data;
	struct tc_stack_datum *old_top;

	old_top = stack->top;

	if (old_top == TC_NULL) {
		return TC_NULL;
	}

	data = old_top->data;
	stack->top = old_top->next;

	old_top = tc_free(old_top);

	return data;
}

int tc_stack_is_empty(struct tc_stack *stack) {
	return (stack->top == TC_NULL);
}

struct tc_stack *tc_stack_malloc(void) {
	return tc_malloc(sizeof(struct tc_stack));;
}

struct tc_stack *tc_stack_free(struct tc_stack *stack) {
	while (!tc_stack_is_empty(stack)) {
		(void) tc_stack_pop(stack);
	}
	return tc_free(stack);
}

