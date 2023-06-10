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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int AES_func_encrypt_data(uint8_t *plaintext, int plaintext_len, const uint8_t *key, const uint8_t *iv, uint8_t *ciphertext) {
	// Initialize the OpenSSL library
	EVP_CIPHER_CTX *ctx;

	if (DEBUG_MESSAGES)
		fprintf(stdout, "AES_func_encrypt_data(): encrypting data of length %d with key %s and iv %s\n", plaintext_len, key, iv);

	if (!(ctx = EVP_CIPHER_CTX_new()))
	{
		fprintf(stderr, "Error: AES_func_encrypt_data(): OpenSSL initialization failed.\n");
		ERR_print_errors_fp(stderr);
		return 0;
	}

	// Set up the encryption parameters
	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
	{
		fprintf(stderr, "Error: AES_func_encrypt_data(): EVP_EncryptInit_ex() failed.\n");
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	// Perform the encryption
	int len;

	if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
	{
		fprintf(stderr, "Error: AES_func_encrypt_data(): EVP_EncryptUpdate() failed.\n");
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	int ciphertext_len = len;

	// Finalize the encryption
	if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
	{
		fprintf(stderr, "Error: AES_func_encrypt_data(): EVP_EncryptFinal_ex() failed.\n");
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	ciphertext_len += len;

	// Clean up the OpenSSL context
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int AES_func_decrypt_data(uint8_t *ciphertext, int ciphertext_len, const uint8_t *key, const uint8_t *iv, uint8_t *plaintext) {
	// Initialize the OpenSSL library
	EVP_CIPHER_CTX *ctx;

	if (DEBUG_MESSAGES)
		fprintf(stdout, "AES_func_decrypt_data(): decrypting data of length %d with key %s and iv %s\n", ciphertext_len, key, iv);
	
	if (!(ctx = EVP_CIPHER_CTX_new()))
	{
		fprintf(stderr, "Error: AES_func_decrypt_data() failed: EVP_CIPHER_CTX_new() failed.\n");
		ERR_print_errors_fp(stderr);
		return 0;
	}

	// Set up the decryption parameters
	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
	{
		fprintf(stderr, "Error: AES_func_decrypt_data() failed: EVP_DecryptInit_ex() failed.\n");
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	// Perform the decryption
	int len;

	if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
	{
		fprintf(stderr, "Error: AES_func_decrypt_data() failed: EVP_DecryptUpdate() failed.\n");
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	int plaintext_len = len;

	// Finalize the decryption
	if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
	{
		fprintf(stderr, "Error: AES_func_decrypt_data() failed: EVP_DecryptFinal_ex() failed.\n");
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	plaintext_len += len;

	// Clean up the OpenSSL context
	EVP_CIPHER_CTX_free(ctx);

	if (DEBUG_MESSAGES)
		fprintf(stdout, "AES_func_decrypt_data(): decrypted data to length %d\n", plaintext_len);

	return plaintext_len;
}