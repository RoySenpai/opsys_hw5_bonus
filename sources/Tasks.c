/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Active Object (AO) - Tasks Implementation
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

#include "../include/Tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const uint8_t key[] = EVP_AES_KEY;
const uint8_t iv[] = EVP_AES_IV;

extern PActiveObject AO_Send[AO_NUM_SEND];
extern PActiveObject AO_Receive[AO_NUM_RECEIVE];
extern PQueue sendOutputQueue;
extern PQueue receiveOutputQueue;

int AO_Task_Encode(void *data) {
	PTask task = (PTask)data;

	if (task == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Encrypt() failed: task is NULL.\n");
		return 1;
	}

	else if (task->_data == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Encrypt() failed: task data is NULL.\n");
		return 1;
	}

	else if (task->_data_size == 0)
	{
		fprintf(stderr, "Error: AO_Task_Encrypt() failed: task data size is 0 bytes.\n");
		return 1;
	}

	char *encoded_data = NULL;
	uint32_t len = 0;
	int ret = UUEncode(((char *)task->_data), ((int)task->_data_size), ((char **)&encoded_data), &len);

	if (ret != 0)
	{
		fprintf(stderr, "Error: AO_Task_Encode() failed: UUEncode() failed.\n");
		return 1;
	}

	// Free the old data, as it's descarted, and set the new encoded data.
	free(task->_data);

	task->_data = (void *)encoded_data;
	task->_data_size = len;

	PQueue q = getQueue(AO_Send[1]);
	queueEnqueue(q, task);

	return 1;
}

int AO_Task_Compress(void *data) {
	PTask task = (PTask)data;

	if (task == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Compress() failed: task is NULL.\n");
		return 1;
	}

	else if (task->_data == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Compress() failed: task data is NULL.\n");
		return 1;
	}

	else if (task->_data_size == 0)
	{
		fprintf(stderr, "Error: AO_Task_Compress() failed: task data size is 0 bytes.\n");
		return 1;
	}

	void *compressed_data = NULL;
	uint32_t compressed_data_size = 0;

	int ret = Bzip2_Compress(((char *)task->_data), task->_data_size, &compressed_data, &compressed_data_size);

	if (ret != 0)
	{
		fprintf(stderr, "Error: AO_Task_Compress() failed: compress_data() failed.\n");
		return 1;
	}

	// Free the old data, as it's descarted, and set the new compressed data.
	free(task->_data);

	task->_data = compressed_data;
	task->_data_size = compressed_data_size;

	PQueue q = getQueue(AO_Send[2]);
	queueEnqueue(q, task);

	return 1;
}

int AO_Task_Encrypt(void *data) {
	PTask task = (PTask)data;

	if (task == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Encrypt() failed: task is NULL.\n");
		return 1;
	}

	else if (task->_data == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Encrypt() failed: task data is NULL.\n");
		return 1;
	}

	else if (task->_data_size == 0)
	{
		fprintf(stderr, "Error: AO_Task_Encrypt() failed: task data size is 0 bytes.\n");
		return 1;
	}

	// Since the encrypted data can be larger than the original data, we allocate twice the size of the original data, just to be sure.
	uint8_t *encrypted_data = (uint8_t *)malloc(task->_data_size * sizeof(uint8_t) * 2);

	if (encrypted_data == NULL)
	{
		perror("Error: AO_Task_Encrypt() failed: malloc() failed");
		return 1;
	}

	int len = AES_func_encrypt_data(((uint8_t *)task->_data), ((int)task->_data_size), key, iv, encrypted_data);

	// Free the old data, as it's descarted, and set the new encrypted data.
	free(task->_data);

	task->_data = (void *)encrypted_data;
	task->_data_size = (uint32_t)len;

	queueEnqueue(sendOutputQueue, task);

	return 1;
}

int AO_Task_Decrypt(void *data) {
	PTask task = (PTask)data;

	if (task == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Decrypt() failed: task is NULL.\n");
		return 1;
	}

	else if (task->_data == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Decrypt() failed: task data is NULL.\n");
		return 1;
	}

	else if (task->_data_size == 0)
	{
		fprintf(stderr, "Error: AO_Task_Decrypt() failed: task data size is 0 bytes.\n");
		return 1;
	}

	// Since the decrypted data can be larger than the original data, we allocate twice the size of the original data, just to be sure.
	uint8_t *decrypted_data = (uint8_t *)malloc(task->_data_size * sizeof(uint8_t) * 2);

	if (decrypted_data == NULL)
	{
		perror("Error: AO_Task_Decrypt() failed: malloc() failed");
		return 1;
	}

	int len = AES_func_decrypt_data(((uint8_t *)task->_data), ((int)task->_data_size), key, iv, decrypted_data);

	// Free the old data, as it's descarted, and set the new decrypted data.
	free(task->_data);

	task->_data = decrypted_data;
	task->_data_size = (uint32_t)len;

	PQueue q = getQueue(AO_Receive[1]);
	queueEnqueue(q, task);

	return 1;
}

int AO_Task_Decompress(void *data) {
	PTask task = (PTask)data;

	if (task == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Decompress() failed: task is NULL.\n");
		return 1;
	}

	else if (task->_data == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Decompress() failed: task data is NULL.\n");
		return 1;
	}

	else if (task->_data_size == 0)
	{
		fprintf(stderr, "Error: AO_Task_Decompress() failed: task data size is 0 bytes.\n");
		return 1;
	}

	void *decompressed_data = NULL;
	uint32_t decompressed_data_size = 0;

	int ret = Bzip2_Decompress(((char *)task->_data), task->_data_size, &decompressed_data, &decompressed_data_size);

	if (ret != 0)
	{
		fprintf(stderr, "Error: AO_Task_Decompress() failed: decompress_data() failed.\n");
		//return 1;
	}

	// Free the old data, as it's descarted, and set the new decompressed data.
	free(task->_data);

	task->_data = decompressed_data;
	task->_data_size = decompressed_data_size;

	PQueue q = getQueue(AO_Receive[2]);
	queueEnqueue(q, task);

	return 1;
}

int AO_Task_Decode(void *data) {
	PTask task = (PTask)data;

	if (task == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Decode() failed: task is NULL.\n");
		return 1;
	}

	else if (task->_data == NULL)
	{
		fprintf(stderr, "Error: AO_Task_Decode() failed: task data is NULL.\n");
		return 1;
	}

	else if (task->_data_size == 0)
	{
		fprintf(stderr, "Error: AO_Task_Decode() failed: task data size is 0 bytes.\n");
		return 1;
	}

	char *decoded_data = NULL;
	uint32_t len = 0;

	int ret = UUDecode(((char *)task->_data), ((int)task->_data_size), ((char **)&decoded_data), &len);

	if (ret != 0)
	{
		fprintf(stderr, "Error: AO_Task_Decode() failed: UUDecode() failed.\n");
		return 1;
	}

	// Free the old data, as it's descarted, and set the new decoded data.
	free(task->_data);

	task->_data = (void *)decoded_data;
	task->_data_size = len;

	queueEnqueue(receiveOutputQueue, task);

	return 1;
}