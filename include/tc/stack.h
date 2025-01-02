/*
    stack.h - simple linked list based stack data structure
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

#ifndef TC_STACK_H
#define TC_STACK_H

struct tc_stack_datum {
	void *data;
	struct tc_stack_datum *next;
};

struct tc_stack {
	struct tc_stack_datum *top;
};

int tc_stack_push(struct tc_stack *stack, void *data);
void *tc_stack_pop(struct tc_stack *stack);
int tc_stack_is_empty(struct tc_stack *stack);

struct tc_stack *tc_stack_malloc(void);
struct tc_stack *tc_stack_free(struct tc_stack *stack);

#endif
