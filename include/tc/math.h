/*
    math.h - math functions
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

#ifndef TC_MATH_H
#define TC_MATH_H

#define TC_PI (3.14159265358979323)
#define TC_PI_2 (TC_PI / 2.0f)
#define TC_PI_TC_2 (TC_PI * 2.0f)
#define TC_EPSILON (0.00000000000000000001)

int tc_abs(int x);
int tc_max(int x, int y);
int tc_min(int x, int y);
int tc_dcompar(double x, double y);
double tc_fabs(double x);
double tc_sin(double x);

#endif
