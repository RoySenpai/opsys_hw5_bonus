/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Compression - Header File
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

#ifndef _COMPRESSION_MAIL_H
#define _COMPRESSION_MAIL_H

/********************/
/* Includes Section */
/********************/

#include "Settings.h"
#include <bzlib.h>
#include <bits/stdint-uintn.h>


/********************/
/* Settings Section */
/********************/

/*
 * @brief Specifies the block size to be used for compression.
 * @note It should be a value between 1 and 9 inclusive, and the actual block size used is 100000 x this figure.
 * @note 9 gives the best compression but takes most memory. 1 gives the worst compression but uses least memory.
 * @note The default value is 3.
 * @note Please refer to the Bzip2 documentation for more information.
*/
#define BZIP2_BLOCK_SIZE		3

/*
 * @brief Specifies the verbosity level to be used for compression.
 * @note It should be a value between 0 and 4 inclusive.
 * @note 0 gives no output, 4 gives maximum output.
 * @note The default value is 0.
 * @note Please refer to the Bzip2 documentation for more information.
*/
#define BZIP2_VERBOSITY			4

/*
 * @brief Specifies the work factor to be used for compression.
 * @note It should be a value between 0 and 250 inclusive.
 * @note The default value is 30.
 * @note Please refer to the Bzip2 documentation for more information.
*/
#define BZIP2_WORK_FACTOR		30

/*
 * @brief Specifies the small decompression memory usage.
 * @note It should be a value between 0 and 1 inclusive.
 * @note 0 gives the best compression but takes most memory. 1 gives the worst compression but uses least memory.
 * @note The default value is 0.
 * @note Please refer to the Bzip2 documentation for more information.
*/
#define BZIP2_DECOMPRESS_SMALL	0


/********************************/
/* Function Declaratios Section */
/********************************/

/*
 * @brief A function that compresses a given data using Bzip2 algorithm.
 * @param data The data to be compressed, in the form of a stream of bytes.
 * @param size The length of the data.
 * @param compressed_data A pointer to a pointer to the compressed data.
 * @param compressed_size A pointer to the length of the compressed data.
 * @return 0 on success, 1 on failure.
*/
int Bzip2_Compress(void* data, uint32_t size, void** compressed_data, uint32_t* compressed_size);

/*
 * @brief A function that decompresses a given compressed data using Bzip2 algorithm.
 * @param compressed_data The compressed data to be decompressed, in the form of a stream of bytes.
 * @param compressed_size The length of the compressed data.
 * @param data A pointer to a pointer to the decompressed data.
 * @param size A pointer to the length of the decompressed data.
 * @return 0 on success, 1 on failure.
*/
int Bzip2_Decompress(void* compressed_data, uint32_t compressed_size, void** data, uint32_t* size);

/*
 * @brief A function that prints an error message according to a given Bzip2 error code.
 * @param func_name The name of the function that returned the error code.
 * @param bzerror The Bzip2 error code.
 * @return void.
 * @note The function prints the error message to stderr.
*/
void Bzip2_print_error(const char *func_name, int bzerror);

#endif // _COMPRESSION_MAIL_H