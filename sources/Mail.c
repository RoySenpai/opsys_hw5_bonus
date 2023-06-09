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

PMailData createMail(const char *body, const uint32_t body_size, PAttachment attachments) {
	PMailData mail_data = (PMailData)malloc(sizeof(MailData));

	if (mail_data == NULL)
	{
		perror("Error: malloc failed");
		return NULL;
	}

	mail_data->_mail_data_body = (char *)calloc(body_size, sizeof(char));

	if (mail_data->_mail_data_body == NULL)
	{
		perror("Error: malloc failed");
		free(mail_data);
		return NULL;
	}

	memcpy(mail_data->_mail_data_body, body, body_size);

	mail_data->_mail_data_attachments = attachments;

	return mail_data;
}

int addMailToList(PMail *head, const char *from, const char *subject, const char *body, const uint32_t body_size, PAttachment attachments) {
	if (*head == NULL)
	{
		*head = (PMail)malloc(sizeof(Mail));

		if (*head == NULL)
		{
			perror("Error: malloc failed");
			return 1;
		}

		memcpy((*head)->_mail_from, from, strlen(from) + 1);
		memcpy((*head)->_mail_subject, subject, strlen(subject) + 1);
		(*head)->_mail_data = createMail(body, body_size, attachments);
		(*head)->_mail_next = NULL;
	}

	else
	{
		PMail current = *head;

		while (current->_mail_next != NULL)
		{
			current = current->_mail_next;
		}

		current->_mail_next = (PMail)malloc(sizeof(Mail));

		if (current->_mail_next == NULL)
		{
			perror("Error: malloc failed");
			return 1;
		}

		memcpy(current->_mail_next->_mail_from, from, strlen(from) + 1);
		memcpy(current->_mail_next->_mail_subject, subject, strlen(subject) + 1);
		current->_mail_next->_mail_data = createMail(body, body_size, attachments);
		current->_mail_next->_mail_next = NULL;
	}

	return 0;
}

int addAttachmentToList(PAttachment *head, const char *name, const char *buffer, const uint32_t size) {
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

void freeMailList(PMail *head) {
	if (*head == NULL)
		return;

	PMail current = *head;
	PMail prev = NULL;

	while (current != NULL)
	{
		prev = current;
		current = current->_mail_next;

		free(prev->_mail_data->_mail_data_body);
		freeAttachmentList(&(prev->_mail_data->_mail_data_attachments));
		free(prev->_mail_data);
		free(prev);
	}

	*head = NULL;
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