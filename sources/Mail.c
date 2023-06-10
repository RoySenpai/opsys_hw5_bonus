/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Mail - Implementation File
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

#include "../include/Mail.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int addAttachmentToList(PAttachment *head, const char *name, void *buffer, const uint32_t size) {
	if (*head == NULL)
	{
		*head = (PAttachment)malloc(sizeof(Attachment));

		if (*head == NULL)
		{
			perror("Error: malloc failed");
			return 1;
		}

		memcpy((*head)->_attach_name, name, strlen(name) + 1);
		(*head)->_attach_data = (char *)calloc(size, sizeof(char));

		if ((*head)->_attach_data == NULL)
		{
			perror("Error: malloc failed");
			free(*head);
			return 1;
		}

		memcpy((*head)->_attach_data, buffer, size);
		(*head)->_attach_data_size = size;
		(*head)->_attach_next = NULL;
	}

	else
	{
		PAttachment current = *head;

		while (current->_attach_next != NULL)
		{
			current = current->_attach_next;
		}

		current->_attach_next = (PAttachment)malloc(sizeof(Attachment));

		if (current->_attach_next == NULL)
		{
			perror("Error: malloc failed");
			return 1;
		}

		memcpy(current->_attach_next->_attach_name, name, strlen(name) + 1);
		current->_attach_next->_attach_data = (char *)calloc(size, sizeof(char));

		if (current->_attach_next->_attach_data == NULL)
		{
			perror("Error: malloc failed");
			free(current->_attach_next);
			return 1;
		}

		memcpy(current->_attach_next->_attach_data, buffer, size);
		current->_attach_next->_attach_data_size = size;
		current->_attach_next->_attach_next = NULL;
	}

	return 0;
}

void freeAttachmentList(PAttachment *head) {
	if (*head == NULL)
		return;

	PAttachment current = *head;
	PAttachment prev = NULL;

	while (current != NULL)
	{
		prev = current;
		current = current->_attach_next;

		free(prev->_attach_data);
		free(prev);
	}

	*head = NULL;
}

uint32_t createMailDataPacket(const char *from, const char *subject, const char *body, PAttachment attachments, char **output) {
	// Initialize size with the size of the username, subject, total size, body size and attachments number
	// Formula: USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + total_size field (4 bytes) + body_size field (4 bytes) + attachments_num field (4 bytes)
	uint32_t total_size = USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + (3 * sizeof(uint32_t));

	// Calculate body size
	uint32_t body_size = strlen(body) + 1;

	// Calculate attachments sizes
	uint32_t attachments_size = 0, attachments_count = 0;

	PAttachment current = attachments;

	total_size += body_size;

	while (current != NULL)
	{
		attachments_size += (ATTACH_FILENAME_MAX + sizeof(uint32_t) + current->_attach_data_size);
		current = current->_attach_next;
		attachments_count++;
	}

	total_size += attachments_size;

	// Free memory if already allocated, to avoid memory leaks
	if (*output != NULL)
	{
		free(*output);
		*output = NULL;
	}

	// Allocate memory for the output
	*output = (char *)calloc(total_size, sizeof(char));

	if (*output == NULL)
	{
		perror("Error: malloc failed");
		return -1;
	}

	// Copy the username and subject to the output
	memcpy(*output, from, strlen(from) + 1);
	memcpy(*output + USERNAME_MAX_LENGTH, subject, strlen(subject) + 1);

	// Copy the total size, body size and attachments number to the output
	memcpy(*output + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH, &total_size, sizeof(uint32_t));
	memcpy(*output + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + sizeof(uint32_t), &body_size, sizeof(uint32_t));
	memcpy(*output + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + (2 * sizeof(uint32_t)), &attachments_count, sizeof(uint32_t));

	// Copy the body to the output
	memcpy(*output + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + (3 * sizeof(uint32_t)), body, body_size);

	// Copy the attachments to the output
	current = attachments;

	uint32_t offset = USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + (3 * sizeof(uint32_t)) + body_size;

	while (current != NULL)
	{
		memcpy(*output + offset, current->_attach_name, strlen(current->_attach_name) + 1);
		memcpy(*output + offset + ATTACH_FILENAME_MAX, &(current->_attach_data_size), sizeof(uint32_t));
		memcpy(*output + offset + ATTACH_FILENAME_MAX + sizeof(uint32_t), current->_attach_data, current->_attach_data_size);

		// Recalculate the offset
		offset += (ATTACH_FILENAME_MAX + sizeof(uint32_t) + current->_attach_data_size);
		current = current->_attach_next;
	}

	return total_size;
}

#if DEBUG_MESSAGES == 1
	int hexDump(const char *desc, const void *addr, const int len, int perLine) {
		if (perLine < 4 || perLine > 64)
			perLine = 16;

		int i = 0;
		unsigned char buff[perLine+1];
		const unsigned char * pc = (const unsigned char *)addr;

		if (desc != NULL)
			printf ("%s:\n", desc);

		if (len == 0)
		{
			printf("  ZERO LENGTH\n");
			return 1;
		}

		if (len < 0)
		{
			printf("  NEGATIVE LENGTH: %d\n", len);
			return 1;
		}

		for (i = 0; i < len; i++)
		{
			if ((i % perLine) == 0)
			{
				if (i != 0)
					fprintf(stdout, "  %s\n", buff);

				printf ("  %04x ", i);
			}

			printf (" %02x", pc[i]);

			if ((pc[i] < 0x20) || (pc[i] > 0x7e))
				buff[i % perLine] = '.';

			else
				buff[i % perLine] = pc[i];

			buff[(i % perLine) + 1] = '\0';
		}

		while ((i % perLine) != 0)
		{
			printf ("   ");
			i++;
		}

		fprintf(stdout, "  %s\n", buff);

		return 1;
	}
#endif