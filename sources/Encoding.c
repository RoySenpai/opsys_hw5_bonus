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

	int ch = 0, n = 0, idx = 0;
	int bytes_to_encode = data_size;
	int encoded_bytes = 0;
	int encoded_data_size_temp = 0;
	register char *p;
	char buf[80], outbuf[250];

	// Calculate the size of the encoded data
	encoded_data_size_temp = (bytes_to_encode / 3) * 4;

	// Padding with zeros if needed, and calculate the size of the encoded data
	if (bytes_to_encode % 3 != 0)
		encoded_data_size_temp += 4;

	// Allocate memory for the encoded data
	*encoded_data = (char *) malloc(encoded_data_size_temp * sizeof(char));

	// Check if the allocation succeeded
	if (*encoded_data == NULL)
	{
		fprintf(stderr, "Error: UUEncode(): Failed to allocate memory for the encoded data.\n");
		return 1;
	}

	// Encode the data
	while (bytes_to_encode > 0)
	{
		n = bytes_to_encode > 44 ? 44 : bytes_to_encode;
		if (n == 0)
			break;

		memcpy(buf, data + encoded_bytes, n);

		idx = 0;
		ch = ENC(n);
		outbuf[idx++] = ch;

		for (p = buf; n > 0; n -= 3, p += 3)
		{
			ch = *p >> 2;
			ch = ENC(ch);
			outbuf[idx++] = ch;
			ch = (((*p << 4) & 060) | ((p[1] >> 4) & 017));
			ch = ENC(ch);
			outbuf[idx++] = ch;
			ch = (((p[1] << 2) & 074) | ((p[2] >> 6) & 03));
			ch = ENC(ch);
			outbuf[idx++] = ch;
			ch = p[2] & 077;
			ch = ENC(ch);
			outbuf[idx++] = ch;
		}

		outbuf[idx++] = '\n';
		memcpy(*encoded_data + encoded_bytes, outbuf, idx);
		encoded_bytes += idx;
		bytes_to_encode -= 44;
	}

	*encoded_data_size = (uint32_t)encoded_bytes;

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

	int ch = 0, n = 0, idx = 0;
	int bytes_to_decode = encoded_data_size;
	int decoded_bytes = 0;
	int data_size_temp = 0;
	register char *p;
	char buf[80], outbuf[250];

	// Calculate the size of the decoded data
	data_size_temp = (bytes_to_decode / 4) * 3;

	// Padding with zeros if needed, and calculate the size of the decoded data
	if (bytes_to_decode % 4 != 0)
		data_size_temp += 3;

	// Allocate memory for the decoded data
	*data = (char *) malloc(data_size_temp * sizeof(char));

	// Check if the allocation succeeded
	if (*data == NULL)
	{
		fprintf(stderr, "Error: UUDecode(): Failed to allocate memory for the decoded data.\n");
		return 1;
	}

	// Decode the data
	while (bytes_to_decode > 0)
	{
		n = bytes_to_decode > 61 ? 61 : bytes_to_decode;
		if (n == 0)
			break;

		memcpy(buf, encoded_data + decoded_bytes, n);

		idx = 0;
		ch = DEC(buf[idx++]);
		n = ch;
		if (n <= 0)
			break;

		for (p = buf + 1; n > 0; n -= 4, p += 4)
		{
			ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
			outbuf[idx++] = ch;
			ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
			outbuf[idx++] = ch;
			ch = DEC(p[2]) << 6 | DEC(p[3]);
			outbuf[idx++] = ch;
		}

		memcpy(*data + decoded_bytes, outbuf, idx);
		decoded_bytes += idx;
		bytes_to_decode -= 61;
	}

	*data_size = (uint32_t)decoded_bytes;

	return 0;
}