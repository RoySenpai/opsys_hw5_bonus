/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Active Object (AO) - Tasks Header File
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

#ifndef _TASKS_H
#define _TASKS_H

#include "Compression.h"
#include "Encryption.h"
#include "Mail.h"
#include "ActiveObject.h"

/********************/
/* Settings Section */
/********************/

/*
 * @brief The number of active objects for handling emails to be sent - it handles the email's data before it is sent.
 * @note The first active object is the one that encodes the data to uuencode format.
 * @note The second active object is the one that compresses the uuencoded data via bzip2.
 * @note The third active object is the one that encrypts the compressed uuencoded data via
 *          OpenSSL's AES algorithm and returns it to the user.
 */
#define AO_NUM_SEND 3

/*
 * @brief The number of active objects for handling received emails - it handles the email data after it is received.
 * @note The first active object is the one that decrypts the data via OpenSSL's AES algorithm.
 * @note The second active object is the one that decompresses the decrypted data via bzip2.
 * @note The thirtd active object is the one that decodes the whole data from uuencode format and
 *          returns it to the user.
 */
#define AO_NUM_RECEIVE 3


/********************************/
/* Function Declaratios Section */
/********************************/

/*
 * @brief The function that encodes the email to uuencode format and sends it to the next active object.
 * @param data A pointer to a struct that contains the data to be encoded and the size of the data.
 * @return 0 on success, 1 on failure.
*/
int AO_Task_Encode(void *data);

/*
 * @brief The function that compresses the email via bzip2 and sends it to the next active object.
 * @param data A pointer to a struct that contains the data to be compressed and the size of the data.
 * @return 0 on success, 1 on failure.
*/
int AO_Task_Compress(void *data);

/*
 * @brief The function that encrypts the email via OpenSSL's AES algorithm and sends it to the next active object.
 * @param data A pointer to a struct that contains the data to be encrypted and the size of the data.
 * @return 0 on success, 1 on failure.
*/
int AO_Task_Encrypt(void *data);

/*
 * @brief The function that decrypts the email via OpenSSL's AES algorithm and sends it to the next active object.
 * @param data A pointer to a struct that contains the data to be decrypted and the size of the data.
 * @return 0 on success, 1 on failure.
*/
int AO_Task_Decrypt(void *data);

/*
 * @brief The function that decompresses the email via bzip2 and sends it to the next active object.
 * @param data A pointer to a struct that contains the data to be decompressed and the size of the data.
 * @return 0 on success, 1 on failure.
*/
int AO_Task_Decompress(void *data);

/*
 * @brief The function that decodes the email from uuencode format and prints it to the standard output.
 * @param data A pointer to a struct that contains the data to be decoded and the size of the data.
 * @return 0 on success, 1 on failure.
*/
int AO_Task_Decode(void *data);

#endif // _TASKS_H
