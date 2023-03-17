/*
    ctype.c - character type functions
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

#include "tc/ctype.h"

/*
 * Check if character ch is an ASCII character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isascii(int ch) {
	return !(ch & (1<<8));
}

/*
 * Check if character ch is a digit character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isdigit(int ch) {
	return ('0' <= ch && ch <= '9');
}

/*
 * Check if character ch is a hexadecimal digit character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isxdigit(int ch) {
	return (
		tc_isdigit(ch) ||
		('a' <= ch && ch <= 'f') ||
		('A' <= ch && ch <= 'F')
	);
}

/*
 * Check if character ch is an upper case character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isupper(int ch) {
	return ('A' <= ch && ch <= 'Z');
}

/*
 * Check if character ch is a lower case character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_islower(int ch) {
	return ('a' <= ch && ch <= 'z');
}

/*
 * Check if character ch is a lower case or upercase character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isalpha(int ch) {
	return (tc_isupper(ch) || tc_islower(ch));
}

/*
 * Check if character ch is a digit, a lower case or an upercase character or
 * not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isalnum(int ch) {
	return tc_isalpha(ch) || tc_isdigit(ch);
}

/*
 * Check if character ch is a whitespace character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isspace(int ch) {
	return (
		ch == '\t' || ch == '\n' || ch == '\v' ||
		ch == '\f' || ch == '\r' || ch == ' '
	);
}

/*
 * Check if character ch is a blank character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isblank(int ch) {
	return (ch == '\t' || ch == ' ');
}

/*
 * Check if character ch is a punctuation character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_ispunct(int ch) {
	return (
		ch == '!' || ch == '"' || ch == '#' || ch == '$' || ch == '%' ||
		ch == '&' || ch == '\'' || ch == '(' || ch == ')' || ch == '*' ||
		ch == '+' || ch == ',' || ch == '-' || ch == '.' || ch == '/' ||
		ch == ':' || ch == ';' || ch == '<' || ch == '=' || ch == '>' ||
		ch == '?' || ch == '@' || ch == '[' || ch == '\\' || ch == ']' ||
		ch == '^' || ch == '_' || ch == '`' || ch == '{' || ch == '|' ||
		ch == '}' || ch == '~'
	);
}

/*
 * Check if character ch is a control character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_iscntrl(int ch) {
	return (
		ch == 000 /* NUL */ || ch == 001 /* SOH */ ||
		ch == 002 /* STX */ || ch == 003 /* ETX */ ||
		ch == 004 /* EOT */ || ch == 005 /* ENQ */ ||
		ch == 006 /* ACK */ || ch == 007 /* BEL */ ||
		ch == 010 /* BS  */ || ch == 011 /* HT  */ ||
		ch == 012 /* NL  */ || ch == 013 /* VT  */ ||
		ch == 014 /* NP  */ || ch == 015 /* CR  */ ||
		ch == 016 /* SO  */ || ch == 017 /* SI  */ ||
		ch == 020 /* DLE */ || ch == 021 /* DC1 */ ||
		ch == 022 /* DC2 */ || ch == 023 /* DC3 */ ||
		ch == 024 /* DC4 */ || ch == 025 /* NAK */ ||
		ch == 026 /* SYN */ || ch == 027 /* ETB */ ||
		ch == 030 /* CAN */ || ch == 031 /* EM  */ ||
		ch == 032 /* SUB */ || ch == 033 /* ESC */ ||
		ch == 034 /* FS  */ || ch == 035 /* GS  */ ||
		ch == 036 /* RS  */ || ch == 037 /* US  */ ||
		ch == 0177/* DEL */
	);

}

/*
 * Check if character ch is a graphic character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isgraph(int ch) {
	return tc_isalnum(ch) || tc_ispunct(ch);
}

/*
 * Check if character ch is a printable character or not
 *
 * Parameters:
 *   ch - character to check
 *
 * Returns: true (non-zero) or false (zero)
 */
int tc_isprint(int ch) {
	return tc_isgraph(ch) || ch == ' ';
}

/*
 * Convert character ch from upper case to lower case.
 *
 * Parameters:
 *   ch - character to convert
 *
 * Returns: lower case character if ch was upper case, otherwise ch unchanged
 */
int tc_tolower(int ch) {
	switch (ch) {
		case 'A': return 'a';
		case 'B': return 'b';
		case 'C': return 'c';
		case 'D': return 'd';
		case 'E': return 'e';
		case 'F': return 'f';
		case 'G': return 'g';
		case 'H': return 'h';
		case 'I': return 'i';
		case 'J': return 'j';
		case 'K': return 'k';
		case 'L': return 'l';
		case 'M': return 'm';
		case 'N': return 'n';
		case 'O': return 'o';
		case 'P': return 'p';
		case 'Q': return 'q';
		case 'R': return 'r';
		case 'S': return 's';
		case 'T': return 't';
		case 'U': return 'u';
		case 'V': return 'v';
		case 'W': return 'w';
		case 'X': return 'x';
		case 'Y': return 'y';
		case 'Z': return 'z';
	}

	return ch;
}

/*
 * Convert character ch from lower case to upper case.
 *
 * Parameters:
 *   ch - character to convert
 *
 * Returns: upper case character if ch was lower case, otherwise ch unchanged
 */
int tc_toupper(int ch) {
	switch (ch) {
		case 'a': return 'A';
		case 'b': return 'B';
		case 'c': return 'C';
		case 'd': return 'D';
		case 'e': return 'E';
		case 'f': return 'F';
		case 'g': return 'G';
		case 'h': return 'H';
		case 'i': return 'I';
		case 'j': return 'J';
		case 'k': return 'K';
		case 'l': return 'L';
		case 'm': return 'M';
		case 'n': return 'N';
		case 'o': return 'O';
		case 'p': return 'P';
		case 'q': return 'Q';
		case 'r': return 'R';
		case 's': return 'S';
		case 't': return 'T';
		case 'u': return 'U';
		case 'v': return 'V';
		case 'w': return 'W';
		case 'x': return 'X';
		case 'y': return 'Y';
		case 'z': return 'Z';
	}

	return ch;
}
