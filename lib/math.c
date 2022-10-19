/*
    math.c - math functions
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

#include "tc/math.h"

/*
 * Absolute value
 *
 * Parameters:
 *  x - integer
 *
 * Returns the absolute value of input integer x
 */
int tc_abs(int x) {
	return (x < 0) ? -x : x;
}

/*
 * Max of two values
 *
 * Parameters:
 *  x - integer
 *  y - integer
 *
 * Returns the larger value of the two inputs integer x and integer y
 */
int tc_max(int x, int y) {
	return (x > y) ? x : y;
}

/*
 * Min of two values
 *
 * Parameters:
 *  x - integer
 *  y - integer
 *
 * Returns the smaller value of the two inputs integer x and integer y
 */
int tc_min(int x, int y) {
	return (x < y) ? x : y;
}
