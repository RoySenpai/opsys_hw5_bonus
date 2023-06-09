/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Encoding - Header File
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

#ifndef _ENCODING_MAIL_H
#define _ENCODING_MAIL_H

/********************/
/* Includes Section */
/********************/

#include "Settings.h"

/********************/
/* Settings Section */
/********************/

/*********************************/
/* Functions Declaretion Section */
/*********************************/

/*
 * @brief A function that encodes a given data using Unix to Unix encoding.
 * @param data The data to encode.
 * @param data_size The size of the data to encode.
 * @param encoded_data The encoded data.
 * @param encoded_data_size The size of the encoded data.
 * @return 0 on success, 1 on failure.
*/
int UUEncode(const char *data, const uint32_t data_size, char **encoded_data, uint32_t *encoded_data_size);

/*
 * @brief A function that decodes a given data using Unix to Unix encoding.
 * @param encoded_data The encoded data.
 * @param encoded_data_size The size of the encoded data.
 * @param data The decoded data.
 * @param data_size The size of the decoded data.
 * @return 0 on success, 1 on failure.
*/
int UUDecode(const char *encoded_data, const uint32_t encoded_data_size, char **data, uint32_t *data_size);


#endif //_ENCODING_MAIL_H