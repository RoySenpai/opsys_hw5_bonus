/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Encryption - Implementation File
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

#include "../include/Encryption.h"

int AES_func_encrypt_data(uint8_t *plaintext, int plaintext_len, const uint8_t *key, const uint8_t *iv, uint8_t *ciphertext) {
	EVP_CIPHER_CTX *ctx;
	int len, ciphertext_len;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
		AES_func_handle_errors();

	/*
	 * Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * The IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	*/
	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
		AES_func_handle_errors();

	/*
	 * Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	*/
	if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
		AES_func_handle_errors();

	ciphertext_len = len;

	/*
	 * Finalise the encryption. Further plaintext bytes may be written at
	 * this stage.
	*/
	if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
		AES_func_handle_errors();
	
	ciphertext += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int AES_func_decrypt_data(uint8_t *ciphertext, int ciphertext_len, const uint8_t *key, const uint8_t *iv, uint8_t *plaintext) {
	EVP_CIPHER_CTX *ctx;

	int len, plaintext_len;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
		AES_func_handle_errors();

	/*
	 * Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * The IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
		AES_func_handle_errors();

	/*
	 * Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary.
	 */
	if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
		AES_func_handle_errors();

	plaintext_len = len;

	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
		AES_func_handle_errors();

	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

void AES_func_handle_errors(void) {
	ERR_print_errors_fp(stderr);
	abort();
}