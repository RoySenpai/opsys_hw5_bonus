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

    return 0;
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

    return 0;
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

    uint8_t *encrypted_data = NULL;

    int len = AES_func_encrypt_data(((uint8_t *)task->_data), ((int)task->_data_size), key, iv, encrypted_data);

    task->_data = encrypted_data;
    task->_data_size = len;

    return 0;
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

    uint8_t *decrypted_data = NULL;

    int len = AES_func_decrypt_data(((uint8_t *)task->_data), ((int)task->_data_size), key, iv, decrypted_data);

    task->_data = decrypted_data;
    task->_data_size = len;

    return 0;
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

    return 0;
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

    return 0;
}