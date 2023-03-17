/*
    luhn.c - luhn check
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

static char *success[] = {
	"18",
	"125",
	"1230",
	"0604243228042426",
	"122000000000003",
	"1234567812345670",
	"135412345678911",
	"180056677601522",
	"201427580143164",
	"201452196307227",
	"210078631906866",
	"214914496133277",
	"214987395202831",
	"30007794564624",
	"30569309025904",
	"3088000000000009",
	"3337373835435230",
	"342400001000180",
	"3530111333300000",
	"3566002020360505",
	"370000000000002",
	"371095063560404",
	"371100001000131",
	"371449635398431",
	"378282246310005",
	"378734493671000",
	"38520000023237",
	"4003050500040005",
	"4030000010001234",
	"4012111111111111",
	"4012888888881881",
	"4111111111111111",
	"4123450131003312",
	"4149685344157815",
	"4222222222222",
	"4242424242424242",
	"4484000000002",
	"4504481742333",
	"4917300800000000",
	"4929110918560456",
	"4929562658340",
	"4984421209470251",
	"49927398716",
	"5038031154140958",
	"5019717010103742",
	"5100000010001004",
	"5100000020002000",
	"5105105105105100",
	"5280934283171080",
	"5506174487999345",
	"5555555555554444",
	"5610591081018250",
	"6007220000000004",
	"6011000900901111",
	"6011000990139424",
	"6011111111111117",
	"6011500080009080",
	"630495060000000000",
	"630490017740292441",
	"6331101999990016",
	"6777000000007",
	"67595425364017391",
	"6762613092357065",
	"6763946698976220",
	"869907502895655",
	"869913861969503"
};

static char *failure[] = {
	"",
	"0",
	"012345",
	"1.0",
	"1111111111111111",
	"123-34",
	"12345",
	"1234567812345678",
	"3560000000000000",
	"356000",
	"3700000",
	"4000000000000000",
	"4000000000000001",
	"4000000000000003",
	"4000000000000004",
	"4000000000000005",
	"4000000000000006",
	"4000000000000007",
	"4000000000000008",
	"4000000000000009",
	"41111111111111X1",
	"411111111111111X",
	"4111 1111 1111 1111",
	"4111-1111-1111-1111",
	"411111",
	"4111111111111112",
	"4111111111111121",
	"4111111111111211",
	"4111111111112111",
	"4111111111121111",
	"4111111111211111",
	"4111111112111111",
	"4111111121111111",
	"4111111211111111",
	"4111112111111111",
	"4111121111111111",
	"4111211111111111",
	"4112111111111111",
	"4121111111111111",
	"4211111111111111",
	"4242424242424241",
	"4929110918560457",
	"4929562658341",
	"49927398717",
	"5019000000000000",
	"510500",
	"5105105105105109",
	"5111111111111111",
	"6011000",
	"6360000000000000",
	"A",
	"AB",
	"ABC",
	"X411111111111111"
};

static int check_success(void) {

	int i;

	for (i = 0; i < sizeof(success)/sizeof(success[0]); i++) {
		if (!tc_luhn_check(success[i])) {
			return TC_CHECK_FAIL;
		}
	}

	return TC_CHECK_PASS;
}

static int check_failure(void) {

	int i;

	for (i = 0; i < sizeof(failure)/sizeof(failure[0]); i++) {
		if (tc_luhn_check(failure[i])) {
tc_putln(TC_STDOUT, failure[i]);
			return TC_CHECK_FAIL;
		}
	}

	return TC_CHECK_PASS;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_success,	"values that pass luhn check" },
		{ check_failure,	"values that fail luhn check" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
