/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Compression - Implementation File
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

#include "../include/Compression.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int Bzip2_Compress(void *data, uint32_t size, void **compressed_data, uint32_t *compressed_size) {
	// Initialize the Bzip2 stream
	bz_stream bzs;
	bzs.bzalloc = NULL;
	bzs.bzfree = NULL;
	bzs.opaque = NULL;

	if (DEBUG_MESSAGES)
		fprintf(stdout, "Bzip2_Compress(): Compressing %d bytes\n", size);

	int ret = BZ2_bzCompressInit(&bzs, BZIP2_BLOCK_SIZE, BZIP2_VERBOSITY, BZIP2_WORK_FACTOR);

	if (ret != BZ_OK)
	{
		Bzip2_print_error("BZ2_bzCompressInit()", ret);
		return 1;
	}

	// Calculate the size of the compressed data buffer
	uint32_t destSize = size + (size * 0.01) + 600; // BZ2_bzBuffToBuffCompress() recommends adding 1% + 600 bytes
	*compressed_data = malloc(destSize);
	if (*compressed_data == NULL)
	{
		perror("Error: Bzip2_Compress() failed: malloc() failed");
		return 1;
	}

	// Perform the compression
	bzs.next_in = (char *)data;
	bzs.avail_in = size;
	bzs.next_out = *compressed_data;
	bzs.avail_out = destSize;

	ret = BZ2_bzCompress(&bzs, BZ_FINISH);
	if (ret != BZ_STREAM_END)
	{
		Bzip2_print_error("BZ2_bzCompress()", ret);
		BZ2_bzCompressEnd(&bzs);
		free(*compressed_data);
		return 1;
	}

	// Set the compressed size
	*compressed_size = destSize - bzs.avail_out;

	// Clean up the Bzip2 stream
	ret = BZ2_bzCompressEnd(&bzs);
	if (ret != BZ_OK)
	{
		Bzip2_print_error("BZ2_bzCompressEnd()", ret);
		free(*compressed_data);
		return 1;
	}

	if (DEBUG_MESSAGES)
		fprintf(stdout, "Bzip2_Compress(): Compressed %d bytes to %d bytes\n", size, *compressed_size);

	return 0;
}

int Bzip2_Decompress(void *compressed_data, uint32_t compressed_size, void **data, uint32_t *size) {
	// Initialize the Bzip2 stream
	bz_stream bzs;
	bzs.bzalloc = NULL;
	bzs.bzfree = NULL;
	bzs.opaque = NULL;

	if (DEBUG_MESSAGES)
		fprintf(stdout, "Bzip2_Decompress(): Decompressing %d bytes\n", compressed_size);

	int ret = BZ2_bzDecompressInit(&bzs, BZIP2_VERBOSITY, BZIP2_DECOMPRESS_SMALL);

	if (ret != BZ_OK)
	{
		Bzip2_print_error("BZ2_bzDecompressInit()", ret);
		return 1;
	}

	// Allocate memory for the decompressed data
	*data = malloc(compressed_size * 10); // A rough estimate for the decompressed size

	if (*data == NULL)
	{
		perror("Error: Bzip2_Decompress() failed: malloc() failed");
		return 1;
	}

	// Perform the decompression
	bzs.next_in = compressed_data;
	bzs.avail_in = compressed_size;
	bzs.next_out = *data;
	bzs.avail_out = compressed_size * 10;

	ret = BZ2_bzDecompress(&bzs);

	if (ret != BZ_OK && ret != BZ_STREAM_END)
	{
		Bzip2_print_error("BZ2_bzDecompress()", ret);
		BZ2_bzDecompressEnd(&bzs);
		free(*data);
		return 1;
	}

	// Set the decompressed size
	*size = compressed_size * 10 - bzs.avail_out;

	// Clean up the Bzip2 stream
	ret = BZ2_bzDecompressEnd(&bzs);

	if (ret != BZ_OK)
	{
		Bzip2_print_error("BZ2_bzDecompressEnd()", ret);
		free(*data);
		return 1;
	}

	if (DEBUG_MESSAGES)
		fprintf(stdout, "Bzip2_Decompress(): Decompressed %d bytes to %d bytes\n", compressed_size, *size);

	return 0;
}

void Bzip2_print_error(const char *func_name, int bzerror) {
	switch (bzerror)
	{
		case BZ_OK:
		case BZ_RUN_OK:
		case BZ_FLUSH_OK:
		case BZ_FINISH_OK:
		case BZ_STREAM_END:
		{
			fprintf(stdout, "%s: Operation completed successfully.\n", func_name);
			break;
		}

		case BZ_CONFIG_ERROR:
		{
			fprintf(stderr, "%s: Configuration error. Please check the library was compiled correctly.\n", func_name);
			break;
		}

		case BZ_SEQUENCE_ERROR:
		{
			fprintf(stderr, "%s: Sequence error. Please check the function calls are in the correct order.\n", func_name);
			break;
		}

		case BZ_PARAM_ERROR:
		{
			fprintf(stderr, "%s: Parameter error. Please check the parameters passed to the function.\n", func_name);
			break;
		}

		case BZ_MEM_ERROR:
		{
			fprintf(stderr, "%s: Memory error. Please check there is sufficient memory available.\n", func_name);
			break;
		}

		case BZ_DATA_ERROR:
		{
			fprintf(stderr, "%s: Data error. Please check the compressed data stream is valid.\n", func_name);
			break;
		}

		case BZ_DATA_ERROR_MAGIC:
		{
			fprintf(stderr, "%s: Data error magic. Please check the compressed data stream is valid.\n", func_name);
			break;
		}

		case BZ_IO_ERROR:
		{
			fprintf(stderr, "%s: IO error. Please check the input and output files are valid.\n", func_name);
			break;
		}

		case BZ_UNEXPECTED_EOF:
		{
			fprintf(stderr, "%s: Unexpected EOF. Please check the compressed data stream is complete.\n", func_name);
			break;
		}

		case BZ_OUTBUFF_FULL:
		{
			fprintf(stderr, "%s: Output buffer full. Please check there is sufficient memory available.\n", func_name);
			break;
		}

		default:
		{
			fprintf(stderr, "%s: Unknown error. Please check the error code against the Bzip2 documentation. Error code: %d\n", func_name, bzerror);
			break;
		}
	}
}