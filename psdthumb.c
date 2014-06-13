/*
 * Utility for extracting thumbnails from PSDs
 *
 * Copyright (C) 2014, Kevin Selwyn <kevinselwyn at gmail dot com>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if !defined(__APPLE__)
#include <malloc.h>
#endif

#define VERSION "1.0.1"

void fclose_if(FILE **resource) {
	if (*resource) {
		fclose(*resource);
	}
}

void free_if(char **resource) {
	if (*resource) {
		free(*resource);
	}
}

int main(int argc, char *argv[]) {
	int rc = 0, i = 0, l = 0, offset = 0, byte = 0, in_filesize = 0, out_filesize = 0;
	char *in_filename = NULL, *out_filename = NULL, *in_data = NULL, *out_data = NULL;
	FILE *in = NULL, *out = NULL;

	if (argc < 3) {
		printf("psdthumb (v%s)\n\nUsage: psdthumb <in.psd> <out.jpg>\n", VERSION);
		rc = 1;
		goto cleanup;
	}

	in_filename = argv[1];
	out_filename = argv[2];

	in = fopen(in_filename, "r");
	out = fopen(out_filename, "w+");

	if (!in) {
		printf("Could not open %s\n", in_filename);
		rc = 1;
		goto cleanup;
	}

	if (!out) {
		printf("Could not open %s\n", out_filename);
		rc = 1;
		goto cleanup;
	}

	fseek(in, 0, SEEK_END);
	in_filesize = ftell(in);
	fseek(in, 0, SEEK_SET);

	in_data = malloc(sizeof(char) * in_filesize);

	if (fread(in_data, 1, in_filesize, in) != in_filesize) {
		printf("Unable to read file: %s\n", in_filename);
		rc = 1;
		goto cleanup;
	}

	if (in_data[0] != '8' || in_data[1] != 'B' || in_data[2] != 'P' || in_data[3] != 'S') {
		printf("Not a PSD file\n");
		rc = 1;
		goto cleanup;
	}

	for (i = 0; i < in_filesize; i++) {
		if (in_data[i] == '8' && in_data[i + 1] == 'B' && in_data[i + 2] == 'I' && in_data[i + 3] == 'M') {
			if ((in_data[i + 4] == 4 && in_data[i + 5] == 9) || (in_data[i + 4] == 4 && in_data[i + 5] == 12)) {
				offset = i + 6 + 2;
				break;
			}
		}
	}

	for (i = 0, l = 3; i <= 4; i++) {
		byte = in_data[offset++];
		byte += byte < 0 ? 256 : 0;

		out_filesize += byte * pow(256, l - i);
	}

	offset += 27;
	out_filesize -= 28;

	out_data = malloc(sizeof(char) * out_filesize);

	for (i = 0; i < out_filesize; i++) {
		out_data[i] = in_data[offset++];
	}

	if (fwrite(out_data, 1, out_filesize, out) != out_filesize) {
		printf("Error writing %s\n", out_filename);
		rc = 1;
		goto cleanup;
	}

cleanup:
	fclose_if(&in);
	fclose_if(&out);
	free_if(&in_data);
	free_if(&out_data);

	return rc;
}
