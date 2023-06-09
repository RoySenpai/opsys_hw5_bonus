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
#include <stdlib.h>

int Bzip2_Compress(void* data, uint32_t size, void** compressed_data, uint32_t* compressed_size) {
	bz_stream* strm = (bz_stream*)malloc(sizeof(bz_stream));

    if (strm == NULL)
    {
        perror("Error: Bzip2_Compress() failed: malloc() failed");
        return 1;
    }

	// Use the standard C library memory allocation routines, as we don't actually need any special allocation routines.
	strm->bzalloc = NULL;
	strm->bzfree = NULL;
	strm->opaque = NULL;

	// Initialize the compression library.
	int ret = BZ2_bzCompressInit(strm, 9, 0, 0);

	// Check the return value of the initialization function.
	if (ret != BZ_OK)
	{
		Bzip2_print_error("Bzip2_Compress()", ret);
		free(strm);
		return 1;
	}

	// Compress the data.
	strm->next_in = (char*)data;
	strm->avail_in = size;

	// Allocate memory for the compressed data.
	*compressed_data = malloc(size);

	// Check the return value of the memory allocation.
	if (*compressed_data == NULL)
	{
		perror("Error: Bzip2_Compress() failed: malloc() failed");
		BZ2_bzCompressEnd(strm);
		free(strm);
		return 1;
	}

	// Set the output buffer.
	strm->next_out = (char*)*compressed_data;
	strm->avail_out = size;

	// Compress the data.
	ret = BZ2_bzCompress(strm, BZ_FINISH);

	// Check the return value of the compression function.
	if (ret != BZ_STREAM_END)
	{
		Bzip2_print_error("Bzip2_Compress()", ret);
		BZ2_bzCompressEnd(strm);
		free(strm);
		free(*compressed_data);
		return 1;
	}

	// Finish the compression process by releasing the memory used by the compression library.
	ret = BZ2_bzCompressEnd(strm);

	// Check the return value of the finish function.
	if (ret != BZ_OK)
	{
		Bzip2_print_error("Bzip2_Compress()", ret);
		free(strm);
		free(*compressed_data);
		return 1;
	}

	// Set the size of the compressed data, free the memory of the stream and return.
	*compressed_size = strm->total_out_lo32;
	free(strm);

	return 0;
}

int Bzip2_Decompress(void* compressed_data, uint32_t compressed_size, void** data, uint32_t* size) {
	bz_stream* strm = (bz_stream*)malloc(sizeof(bz_stream));

    if (strm == NULL)
    {
        perror("Error: Bzip2_Decompress() failed: malloc() failed");
        return 1;
    }

	// Use the standard C library memory allocation routines, as we don't actually need any special allocation routines.
	strm->bzalloc = NULL;
	strm->bzfree = NULL;
	strm->opaque = NULL;

	// Initialize the decompression library.
	int ret = BZ2_bzDecompressInit(strm, BZIP2_VERBOSITY, BZIP2_DECOMPRESS_SMALL);

	// Check the return value of the initialization function.
	if (ret != BZ_OK)
	{
		Bzip2_print_error("Bzip2_Decompress()", ret);
		free(strm);
		return 1;
	}

	// Decompress the data.
	strm->next_in = (char*)compressed_data;
	strm->avail_in = compressed_size;

	// Allocate memory for the decompressed data.
	*data = malloc(compressed_size);

	// Check the return value of the memory allocation.
	if (*data == NULL)
	{
		perror("Error: Bzip2_Decompress() failed: malloc() failed");
		BZ2_bzDecompressEnd(strm);
		free(strm);
		return 1;
	}

	// Set the output buffer.
	strm->next_out = (char*)*data;
	strm->avail_out = compressed_size;

	ret = BZ2_bzDecompress(strm);

	// Check the return value of the decompression function.
	if (ret != BZ_STREAM_END)
	{
		Bzip2_print_error("Bzip2_Decompress()", ret);
		BZ2_bzDecompressEnd(strm);
		free(strm);
		free(*data);
		return 1;
	}

	ret = BZ2_bzDecompressEnd(strm);

	// Check the return value of the finish function.
	if (ret != BZ_STREAM_END)
	{
		Bzip2_print_error("Bzip2_Decompress()", ret);
		free(strm);
		free(*data);
		return 1;
	}

	// Set the size of the decompressed data, free the memory of the stream and return.
	*size = strm->total_out_lo32;
	free(strm);

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
			fprintf(stderr, "%s: Unknown error. Please check the error code against the Bzip2 documentation.\n", func_name);
			break;
		}
	}
}