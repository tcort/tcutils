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

#define DEFAULT_FREQUENCY (750)
#define DEFAULT_WPM (18)

const int volume = 16384 * (0.50);
double frequency = DEFAULT_FREQUENCY;

/* useful timing details: https://morsecode.world/international/timing.html */

static int nsamples_unit(int wpm) {
	return TC_WAV_SAMPLE_RATE * (60.0 / (50.0 * wpm));
}

static int nsamples_dit(int wpm) { return 1 * nsamples_unit(wpm); }
static int nsamples_dah(int wpm) { return 3 * nsamples_unit(wpm); }
static int nsamples_intra_character_space(int wpm) { return 1 * nsamples_unit(wpm); }
static int nsamples_inter_character_space(int wpm) { return 3 * nsamples_unit(wpm); }
static int nsamples_inter_word_space(int wpm)      { return 5 * nsamples_unit(wpm); }
/* inter word space is 5 because there are nsamples_intra_character_space
   before and after the space to bring it up to 7 */

/* shape output waveform so sound isn't as harsh, rise and fall is 10% of dit */
static int nsamples_rise_time(int wpm) { return nsamples_dit(wpm) / 10; }
static int nsamples_fall_time(int wpm) { return nsamples_dit(wpm) / 10; }


static tc_int16_t *dit_tone = TC_NULL;
static unsigned int dit_tone_len = 0;
static tc_int16_t *dah_tone = TC_NULL;
static unsigned int dah_tone_len = 0;

static void make_tone(tc_int16_t *samples, unsigned int nsamples, int rise_time, int fall_time) {
	int i;
	for (i = 0; i < nsamples; i++) {
		double t = (double) i / TC_WAV_SAMPLE_RATE;
		samples[i] = volume * tc_sin(frequency*t*2*TC_PI);

		if (i < rise_time) {
			samples[i] = samples[i] * (i*1.0/rise_time*1.0);
		} else if (i > nsamples - fall_time) {
			samples[i] = samples[i] * ((nsamples-i)*1.0/fall_time*1.0);
		}
	}
}

static int init_tone(int wpm) {

	int rise_time;
	int fall_time;

	rise_time = nsamples_rise_time(wpm);
	fall_time = nsamples_fall_time(wpm);

	dit_tone_len = nsamples_dit(wpm);
	dit_tone = (tc_int16_t *) tc_malloc(dit_tone_len * sizeof(tc_int16_t));
	if (dit_tone == TC_NULL) {
		return TC_ERR;
	}
	make_tone(dit_tone, dit_tone_len, rise_time, fall_time);

	dah_tone_len = nsamples_dah(wpm);
	dah_tone = (tc_int16_t *) tc_malloc(dah_tone_len * sizeof(tc_int16_t));
	if (dah_tone == TC_NULL) {
		return TC_ERR;
	}
	make_tone(dah_tone, dah_tone_len, rise_time, fall_time);

	return TC_OK;
}

static void exit_tone(void) {
	dit_tone = tc_free(dit_tone);
	dit_tone_len = 0;
	dah_tone = tc_free(dah_tone);
	dah_tone_len = 0;
}

static void write_dit(int fd) {
	tc_wav_write(fd, dit_tone, dit_tone_len);
}

static void write_dah(int fd) {
	tc_wav_write(fd, dah_tone, dah_tone_len);
}

static void make_space(tc_int16_t *samples, unsigned int nsamples) {
	int i;
	for (i = 0; i < nsamples; i++) {
		samples[i] = 0;
	}
}

static tc_int16_t *inter_character_space = TC_NULL;
static unsigned int inter_character_space_len = 0;
static tc_int16_t *intra_character_space = TC_NULL;
static unsigned int intra_character_space_len = 0;
static tc_int16_t *inter_word_space = TC_NULL;
static unsigned int inter_word_space_len = 0;

static int init_space(int wpm) {
	inter_character_space_len = nsamples_inter_character_space(wpm);
	inter_character_space = (tc_int16_t *) tc_malloc(inter_character_space_len * sizeof(tc_int16_t));
	if (inter_character_space == TC_NULL) {
		return TC_ERR;
	}
	make_space(inter_character_space, inter_character_space_len);

	intra_character_space_len = nsamples_intra_character_space(wpm);
	intra_character_space = (tc_int16_t *) tc_malloc(intra_character_space_len * sizeof(tc_int16_t));
	if (intra_character_space == TC_NULL) {
		return TC_ERR;
	}
	make_space(intra_character_space, intra_character_space_len);

	inter_word_space_len = nsamples_inter_word_space(wpm);
	inter_word_space = (tc_int16_t *) tc_malloc(inter_word_space_len * sizeof(tc_int16_t));
	if (inter_word_space == TC_NULL) {
		return TC_ERR;
	}
	make_space(inter_word_space, inter_word_space_len);

	return TC_OK;
}

static void exit_space(void) {
	inter_character_space = tc_free(inter_character_space);
	inter_character_space_len = 0;

	intra_character_space = tc_free(intra_character_space);
	intra_character_space_len = 0;

	inter_word_space = tc_free(inter_word_space);
	inter_word_space_len = 0;
}

static void write_inter_character_space(int fd) {
	tc_wav_write(fd, inter_character_space, inter_character_space_len);
}

static void write_intra_character_space(int fd) {
	tc_wav_write(fd, intra_character_space, intra_character_space_len);
}

static void write_inter_word_space(int fd) {
	tc_wav_write(fd, inter_word_space, inter_word_space_len);
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
	int rc;
	int fd_in;
	int fd_out;
	struct tc_prog_arg *arg;

	int wpm = DEFAULT_WPM;

	static struct tc_prog_arg args[] = {
		{
			.arg = 'f',
			.longarg = "frequency",
			.description = "the frequency of the generated tone in Hertz. Default 750.",
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

	rc = init_space(wpm);
	if (rc == TC_ERR) {
		tc_puterrln("Init Failed");
		tc_exit(TC_EXIT_FAILURE);
	}
	rc = init_tone(wpm);
	if (rc == TC_ERR) {
		tc_puterrln("Init Failed");
		tc_exit(TC_EXIT_FAILURE);
	}

	while ((ch = tc_getc(fd_in)) != TC_EOF) {
		if (i != 0) {
			write_inter_character_space(fd_out);
		}
		write_character(fd_out, ch);
		i++;
	}

	exit_tone();
	exit_space();

	tc_wav_close(fd_out);
	tc_close(fd_in);

	tc_exit(TC_EXIT_SUCCESS);
}
