 /*
    cw2wav -- converts text into a morse code audio file
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

#define DEFAULT_FREQUENCY (600)
#define DEFAULT_WPM (18)

const int NUM_SAMPLES = TC_WAV_SAMPLE_RATE * (TC_WAV_BITS_PER_SAMPLE/8);
const int volume = 32000;
double frequency = DEFAULT_FREQUENCY;
int wpm = DEFAULT_WPM;

static int nsamples_dit(void) {
	return NUM_SAMPLES * (60.0 / (50.0 * wpm * 2)); 
}

static int nsamples_dah(void) {
	return 3 * NUM_SAMPLES * (60.0 / (50.0 * wpm * 2)); 
}

static void write_dit(int fd) {
	int i;
	const unsigned int nsamples = nsamples_dit();
	for (i = 0; i < nsamples; i++) {
		double t = (double) i / TC_WAV_SAMPLE_RATE;
		tc_int16_t samples[1];
		samples[0] = volume * tc_sin(frequency*t*2*TC_PI);

		tc_wav_write(fd, samples, 1);
	}

}

static void write_dah(int fd) {
	int i;
	const int nsamples = nsamples_dah();
	for (i = 0; i < nsamples; i++) {
		double t = (double) i / TC_WAV_SAMPLE_RATE;
		tc_int16_t samples[1];
		samples[0] = volume * tc_sin(frequency*t*2*TC_PI);

		tc_wav_write(fd, samples, 1);
	}
}

static void write_inter_character_space(int fd) {

	int i;
	const unsigned int nsamples = nsamples_dah();
	for (i = 0; i < nsamples; i++) {
		tc_int16_t samples[1];
		samples[0] = 0;
		tc_wav_write(fd, samples, 1);
	}

}

static void write_intra_character_space(int fd) {

	int i;
	const unsigned int nsamples = nsamples_dit();
	for (i = 0; i < nsamples; i++) {
		tc_int16_t samples[1];
		samples[0] = 0;
		tc_wav_write(fd, samples, 1);
	}

}

static void write_inter_word_space(int fd) {

	int i;
	const int nsamples = 7 * nsamples_dit();
	for (i = 0; i < nsamples; i++) {
		tc_int16_t samples[1];
		samples[0] = 0;
		tc_wav_write(fd, samples, 1);
	}

}

const char *alphabet[] = {
	"", /* 0 => '' */
	"", /* 1 => '' */
	"", /* 2 => '' */
	"", /* 3 => '' */
	"", /* 4 => '' */
	"", /* 5 => '' */
	"", /* 6 => '' */
	" ", /* 7 => '' */
	"", /* 8 => '' */
	"", /* 9 => '' */
	" ", /* 10 => '' */
	"", /* 11 => '' */
	"", /* 12 => '' */
	"", /* 13 => '' */
	"", /* 14 => '' */
	"", /* 15 => '' */
	"", /* 16 => '' */
	"", /* 17 => '' */
	"", /* 18 => '' */
	"", /* 19 => '' */
	"", /* 20 => '' */
	"", /* 21 => '' */
	"", /* 22 => '' */
	"", /* 23 => '' */
	"", /* 24 => '' */
	"", /* 25 => '' */
	"", /* 26 => '' */
	"", /* 27 => ' */
	"", /* 28 => '' */
	"", /* 29 => '' */
	"", /* 30 => '' */
	"", /* 31 => '' */
	" ", /* 32 => ' ' */
	"", /* 33 => '!' */
	"", /* 34 => '"' */
	"", /* 35 => '#' */
	"", /* 36 => '$' */
	"", /* 37 => '%' */
	"", /* 38 => '&' */
	"", /* 39 => ''' */
	"", /* 40 => '(' */
	"", /* 41 => ')' */
	"", /* 42 => '*' */
	"", /* 43 => '+' */
	"--..--", /* 44 => ',' */
	"", /* 45 => '-' */
	".-.-.-", /* 46 => '.' */
	"", /* 47 => '/' */
	"-----", /* 48 => '0' */
	".----", /* 49 => '1' */
	"..---", /* 50 => '2' */
	"...--", /* 51 => '3' */
	"....-", /* 52 => '4' */
	".....", /* 53 => '5' */
	"-....", /* 54 => '6' */
	"--...", /* 55 => '7' */
	"---..", /* 56 => '8' */
	"----.", /* 57 => '9' */
	"", /* 58 => ':' */
	"", /* 59 => ';' */
	"", /* 60 => '<' */
	"-...-", /* 61 => '=' */
	"", /* 62 => '>' */
	"..--..", /* 63 => '?' */
	"", /* 64 => '@' */
	".-", /* 65 => 'A' */
	"-...", /* 66 => 'B' */
	"-.-.", /* 67 => 'C' */
	"-..", /* 68 => 'D' */
	".", /* 69 => 'E' */
	"..-.", /* 70 => 'F' */
	"--.", /* 71 => 'G' */
	"....", /* 72 => 'H' */
	"..", /* 73 => 'I' */
	".---", /* 74 => 'J' */
	"-.-", /* 75 => 'K' */
	".-..", /* 76 => 'L' */
	"--", /* 77 => 'M' */
	"-.", /* 78 => 'N' */
	"---", /* 79 => 'O' */
	".--.", /* 80 => 'P' */
	"--.-", /* 81 => 'Q' */
	".-.", /* 82 => 'R' */
	"...", /* 83 => 'S' */
	"-", /* 84 => 'T' */
	"..-", /* 85 => 'U' */
	"...-", /* 86 => 'V' */
	".--", /* 87 => 'W' */
	"-..-", /* 88 => 'X' */
	"-.--", /* 89 => 'Y' */
	"==..", /* 90 => 'Z' */
	"", /* 91 => '[' */
	"", /* 92 => '\' */
	"", /* 93 => ']' */
	"", /* 94 => '^' */
	"", /* 95 => '_' */
	"", /* 96 => '`' */
	".-", /* 97 => 'a' */
	"-...", /* 98 => 'b' */
	"-.-.", /* 99 => 'c' */
	"-..", /* 100 => 'd' */
	".", /* 101 => 'e' */
	"..-.", /* 102 => 'f' */
	"--.", /* 103 => 'g' */
	"....", /* 104 => 'h' */
	"..", /* 105 => 'i' */
	".---", /* 106 => 'j' */
	"-.-", /* 107 => 'k' */
	".-..", /* 108 => 'l' */
	"--", /* 109 => 'm' */
	"-.", /* 110 => 'n' */
	"---", /* 111 => 'o' */
	".--.", /* 112 => 'p' */
	"--.-", /* 113 => 'q' */
	".-.", /* 114 => 'r' */
	"...", /* 115 => 's' */
	"-", /* 116 => 't' */
	"..-", /* 117 => 'u' */
	"...-", /* 118 => 'v' */
	".--", /* 119 => 'w' */
	"-..-", /* 120 => 'x' */
	"-.--", /* 121 => 'y' */
	"--..", /* 122 => 'z' */
	"", /* 123 => '{' */
	"", /* 124 => '|' */
	"", /* 125 => '}' */
	"", /* 126 => '~' */
	"", /* 127 => '' */
	"", /* 128 => '' */
	"", /* 129 => '' */
	"", /* 130 => '' */
	"", /* 131 => '' */
	"", /* 132 => '' */
	"", /* 133 => '' */
	"", /* 134 => '' */
	"", /* 135 => '' */
	"", /* 136 => '' */
	"", /* 137 => '' */
	"", /* 138 => '' */
	"", /* 139 => '' */
	"", /* 140 => '' */
	"", /* 141 => '' */
	"", /* 142 => '' */
	"", /* 143 => '' */
	"", /* 144 => '' */
	"", /* 145 => '' */
	"", /* 146 => '' */
	"", /* 147 => '' */
	"", /* 148 => '' */
	"", /* 149 => '' */
	"", /* 150 => '' */
	"", /* 151 => '' */
	"", /* 152 => '' */
	"", /* 153 => '' */
	"", /* 154 => '' */
	"", /* 155 => '' */
	"", /* 156 => '' */
	"", /* 157 => '' */
	"", /* 158 => '' */
	"", /* 159 => '' */
	"", /* 160 => ' ' */
	"", /* 161 => '¡' */
	"", /* 162 => '¢' */
	"", /* 163 => '£' */
	"", /* 164 => '¤' */
	"", /* 165 => '¥' */
	"", /* 166 => '¦' */
	"", /* 167 => '§' */
	"", /* 168 => '¨' */
	"", /* 169 => '©' */
	"", /* 170 => 'ª' */
	"", /* 171 => '«' */
	"", /* 172 => '¬' */
	"", /* 173 => '' */
	"", /* 174 => '®' */
	"", /* 175 => '¯' */
	"", /* 176 => '°' */
	"", /* 177 => '±' */
	"", /* 178 => '²' */
	"", /* 179 => '³' */
	"", /* 180 => '´' */
	"", /* 181 => 'µ' */
	"", /* 182 => '¶' */
	"", /* 183 => '·' */
	"", /* 184 => '¸' */
	"", /* 185 => '¹' */
	"", /* 186 => 'º' */
	"", /* 187 => '»' */
	"", /* 188 => '¼' */
	"", /* 189 => '½' */
	"", /* 190 => '¾' */
	"", /* 191 => '¿' */
	"", /* 192 => 'À' */
	"", /* 193 => 'Á' */
	"", /* 194 => 'Â' */
	"", /* 195 => 'Ã' */
	"", /* 196 => 'Ä' */
	"", /* 197 => 'Å' */
	"", /* 198 => 'Æ' */
	"", /* 199 => 'Ç' */
	"", /* 200 => 'È' */
	"", /* 201 => 'É' */
	"", /* 202 => 'Ê' */
	"", /* 203 => 'Ë' */
	"", /* 204 => 'Ì' */
	"", /* 205 => 'Í' */
	"", /* 206 => 'Î' */
	"", /* 207 => 'Ï' */
	"", /* 208 => 'Ð' */
	"", /* 209 => 'Ñ' */
	"", /* 210 => 'Ò' */
	"", /* 211 => 'Ó' */
	"", /* 212 => 'Ô' */
	"", /* 213 => 'Õ' */
	"", /* 214 => 'Ö' */
	"", /* 215 => '×' */
	"", /* 216 => 'Ø' */
	"", /* 217 => 'Ù' */
	"", /* 218 => 'Ú' */
	"", /* 219 => 'Û' */
	"", /* 220 => 'Ü' */
	"", /* 221 => 'Ý' */
	"", /* 222 => 'Þ' */
	"", /* 223 => 'ß' */
	"", /* 224 => 'à' */
	"", /* 225 => 'á' */
	"", /* 226 => 'â' */
	"", /* 227 => 'ã' */
	"", /* 228 => 'ä' */
	"", /* 229 => 'å' */
	"", /* 230 => 'æ' */
	"", /* 231 => 'ç' */
	"", /* 232 => 'è' */
	"", /* 233 => 'é' */
	"", /* 234 => 'ê' */
	"", /* 235 => 'ë' */
	"", /* 236 => 'ì' */
	"", /* 237 => 'í' */
	"", /* 238 => 'î' */
	"", /* 239 => 'ï' */
	"", /* 240 => 'ð' */
	"", /* 241 => 'ñ' */
	"", /* 242 => 'ò' */
	"", /* 243 => 'ó' */
	"", /* 244 => 'ô' */
	"", /* 245 => 'õ' */
	"", /* 246 => 'ö' */
	"", /* 247 => '÷' */
	"", /* 248 => 'ø' */
	"", /* 249 => 'ù' */
	"", /* 250 => 'ú' */
	"", /* 251 => 'û' */
	"", /* 252 => 'ü' */
	"", /* 253 => 'ý' */
	"", /* 254 => 'þ' */
	"", /* 255 => 'ÿ' */
};

static void write_character(int fd, unsigned char c) {
	int i;
	const char *s = alphabet[c];

	if (s == TC_NULL || s[0] == '\0') return;
	for (i = 0; s[i] != '\0'; i++) {
		if (i != 0) {
			write_intra_character_space(fd);
		}
		switch (s[i]) {
			case ' ':
				write_inter_word_space(fd);
				break;
			case '.':
				write_dit(fd);
				break;
			case '-':
				write_dah(fd);
				break;
		}
	}
}

int main(int argc, char *argv[]) {

	char ch;
	int i;
	int fd_in;
	int fd_out;
	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		{
			.arg = 'f',
			.longarg = "frequency",
			.description = "the frequency of the generated tone in Hertz. Default 600.",
			.has_value = 1
		},
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		{
			.arg = 'w',
			.longarg = "wpm",
			.description = "words per minute. Default 18.",
			.has_value = 1
		},
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "cw2wav hello.txt hello.wav", .description = "convert the text of hello.txt into a sound file hello.wav" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "cw2wav",
		.usage = "[OPTIONS] INPUT_TEXT_FILENAME OUTPUT_AUDIO_FILENAME",
		.description = "converts text into a morse code audio file",
		.package = TC_VERSION_NAME,
		.version = TC_VERSION_STRING,
		.copyright = TC_VERSION_COPYRIGHT,
		.license = TC_VERSION_LICENSE,
		.author =  TC_VERSION_AUTHOR,
		.args = args,
		.examples = examples
	};

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'f':
				frequency = tc_atoi(argval);
				frequency = frequency < 60 || frequency > 3000 ? DEFAULT_FREQUENCY : frequency;
				break;
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
			case 'w':
				wpm = tc_atoi(argval);
				wpm = wpm < 1 || wpm > 100 ? DEFAULT_WPM : wpm;
				break;
		}

	}

	argc -= argi;
	argv += argi;


	if (argc != 2) {
		tc_args_show_usage(&prog);
		tc_exit(TC_EXIT_FAILURE);
	}

	fd_in = tc_open_reader(argv[0]);
	if (fd_in == TC_ERR) {
		tc_puterrln("Could not open input file");
		tc_exit(TC_EXIT_FAILURE);
	}
	fd_out = tc_wav_open_writer(argv[1]);
	if (fd_out == TC_ERR) {
		tc_puterrln("Could not open output file");
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((ch = tc_getc(fd_in)) != TC_EOF) {
		if (i != 0) {
			write_inter_character_space(fd_out);
		}
		write_character(fd_out, ch);
		i++;
	}

	tc_wav_close(fd_out);
	tc_close(fd_in);

	tc_exit(TC_EXIT_SUCCESS);
}
