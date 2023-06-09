/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Encryption - Header File
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

#ifndef _ENCRYPTION_MAIL_H
#define _ENCRYPTION_MAIL_H

/********************/
/* Includes Section */
/********************/

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <string.h>
#include <bits/stdint-uintn.h>


/********************/
/* Settings Section */
/********************/

/*
 * @brief A 256-bit key for OpenSSL's AES algorithm.
 * @note The key is a string of 32 characters.
 * @note The key is used for both encryption and decryption.
 */
#define EVP_AES_KEY "01234567890123456789012345678901"

/*
 * @brief A 128-bit initialization vector for OpenSSL's AES algorithm.
 * @note The initialization vector is a string of 16 characters.
 * @note The initialization vector is used for both encryption and decryption.
 */
#define EVP_AES_IV "0123456789012345"


/********************************/
/* Function Declaratios Section */
/********************************/

/*
 * @brief A function that encrypts a given plaintext using OpenSSL's AES algorithm.
 * @param plaintext The data to be encrypted, in the form of a stream of bytes.
 * @param plaintext_len The length of the plaintext.
 * @param key The key to be used for encryption.
 * @param iv The initialization vector to be used for encryption.
 * @param ciphertext The encrypted data output, in the form of a stream of bytes.
 * @return The length of the ciphertext.
 * @note The ciphertext is allocated inside the function, and should be freed by the caller.
 * @note The key and the initialization vector are strings of characters.
*/
int AES_func_encrypt_data(uint8_t *plaintext, int plaintext_len, const uint8_t *key, const uint8_t *iv, uint8_t *ciphertext);

/*
 * @brief A function that decrypts a given ciphertext using OpenSSL's AES algorithm.
 * @param ciphertext The data to be decrypted, in the form of a stream of bytes.
 * @param ciphertext_len The length of the ciphertext.
 * @param key The key to be used for decryption.
 * @param iv The initialization vector to be used for decryption.
 * @param data The decrypted data output, in the form of a stream of bytes.
 * @return The length of the decrypted data.
 * @note The decrypted data is allocated inside the function, and should be freed by the caller.
 * @note The key and the initialization vector are strings of characters.
*/
int AES_func_decrypt_data(uint8_t *ciphertext, int ciphertext_len, const uint8_t *key, const uint8_t *iv, uint8_t *data);

/*
 * @brief A function that handles errors in OpenSSL's AES algorithm.
 * @note The function prints the error to stderr and aborts the program (core dump).
*/
void AES_func_handle_errors(void);

#endif /* _ENCRYPTION_MAIL_H */