/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Encoding Implementation File
 *  Copyright (C) 2023  Roy Simanovich and Linor Ronen
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*************************************************************************************/
/*
 * 							NOTICE:
 *		Parts of this code are taken from the following source:
 * 					http://www.bastet.com/
 *
 * The code was modified to fit the needs of this assignment.
 *
 * Copy of the header license notice:
 *
 * UUENCODE - a Win32 utility to uuencode single files.
 * UUDECODE - a Win32 utility to uudecode single files.
 * Copyright (C) 1998 Clem Dye
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/*************************************************************************************/

#include "../include/Encoding.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int UUEncode(const char *data, const uint32_t data_size, char **encoded_data, uint32_t *encoded_data_size) {
	// Calculate the size of the encoded data
	// Force *encoded_data to be NULL as we allocate the data in this function.
	if (data == NULL || encoded_data == NULL || encoded_data_size == NULL || *encoded_data != NULL)
	{
		fprintf(stderr, "Error: UUEncode(): Invalid arguments.\n");
		return 1;
	}

	else if (data_size == 0)
	{
		fprintf(stderr, "Error: UUEncode(): Invalid data size.\n");
		return 1;
	}

	if (DEBUG_MESSAGES)
		fprintf(stdout, "UUEncode(): Encoding data of size %d.\n", data_size);

	*encoded_data_size = (data_size / 3) * 4;

	if (data_size % 3 != 0)
		*encoded_data_size += 4;

	// Allocate memory for the encoded data
	*encoded_data = (char *)malloc(*encoded_data_size + 1); // +1 for null terminator

	if (*encoded_data == NULL)
	{
		fprintf(stderr, "Memory allocation failed for encoded data.\n");
		return 1;
	}

	// Perform UUEncode algorithm
	uint32_t i, j;
	for (i = 0, j = 0; i < data_size; i += 3, j += 4)
	{
		(*encoded_data)[j] = (data[i] & 0xfc) >> 2;
		(*encoded_data)[j + 1] = ((data[i] & 0x03) << 4) | ((data[i + 1] & 0xf0) >> 4);
		(*encoded_data)[j + 2] = ((data[i + 1] & 0x0f) << 2) | ((data[i + 2] & 0xc0) >> 6);
		(*encoded_data)[j + 3] = data[i + 2] & 0x3f;
	}

	// Append newline character to the end of encoded data
	(*encoded_data)[*encoded_data_size] = '\0';

	if (DEBUG_MESSAGES)
		fprintf(stdout, "UUEncode(): Encoded data size is %d.\n", *encoded_data_size);

	return 0;
}

int UUDecode(const char *encoded_data, const uint32_t encoded_data_size, char **data, uint32_t *data_size) {
	// Force *data to be NULL as we allocate the data in this function.
	if (encoded_data == NULL || data == NULL || data_size == NULL || *data != NULL)
	{
		fprintf(stderr, "UUDecode(): Invalid arguments.\n");
		return 1;
	}

	else if (data_size == 0)
	{
		fprintf(stderr, "UUDecode(): Invalid data size.\n");
		return 1;
	}

	if (DEBUG_MESSAGES)
		fprintf(stdout, "UUDecode(): Decoding data of size %d.\n", encoded_data_size);
	
	// Calculate the size of the decoded data
	*data_size = (encoded_data_size / 4) * 3;
	if (encoded_data[encoded_data_size - 1] == '=')
	{
		*data_size -= 1;

		if (encoded_data[encoded_data_size - 2] == '=')
			*data_size -= 1;
	}

	// Allocate memory for the decoded data
	*data = (char *) malloc(*data_size + 1); // +1 for null terminator

	if (*data == NULL)
	{
		fprintf(stderr, "Memory allocation failed for decoded data.\n");
		return 1;
	}

	// Perform UUDecode algorithm
	uint32_t i, j;
	for (i = 0, j = 0; i < encoded_data_size; i += 4, j += 3)
	{
		(*data)[j] = (encoded_data[i] << 2) | ((encoded_data[i + 1] & 0x30) >> 4);
		(*data)[j + 1] = ((encoded_data[i + 1] & 0x0f) << 4) | ((encoded_data[i + 2] & 0x3c) >> 2);
		(*data)[j + 2] = ((encoded_data[i + 2] & 0x03) << 6) | (encoded_data[i + 3] & 0x3f);
	}

	// Append null terminator to the end of decoded data
	(*data)[*data_size] = '\0';

	if (DEBUG_MESSAGES)
		fprintf(stdout, "UUDecode(): Decoded data size is %d.\n", *data_size);

	return 0;
}