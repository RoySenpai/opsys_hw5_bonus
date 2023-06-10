/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Mail Client implementation
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
#include "../include/Tasks.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

PActiveObject AO_Send[AO_NUM_SEND] = { NULL };
PActiveObject AO_Receive[AO_NUM_RECEIVE] = { NULL };
PQueue sendOutputQueue = NULL;
PQueue receiveOutputQueue = NULL;

int main(void) {
	fprintf(stdout, "\t\t\t\033[0;36mMail Client\n"
					"\tCopyright (C) 2023  Roy Simanovich and Linor Ronen\n"
					"\tThis program comes with ABSOLUTELY NO WARRANTY.\n"
					"\tThis is free software, and you are welcome to redistribute it\n"
					"\tunder certain conditions; see `LICENSE' for details.\033[0;37m\n\n");
	
	fprintf(stdout, "Mail Client startup...\n");

	// Initialize the queues
	sendOutputQueue = queueCreate();
	receiveOutputQueue = queueCreate();

	if (sendOutputQueue == NULL || receiveOutputQueue == NULL)
	{
		fprintf(stderr, "Error: main() failed: queueCreate() failed.\n");
		return 1;
	}

	// Initialize the active objects
	AO_Send[0] = CreateActiveObject(AO_Task_Encode);
	AO_Send[1] = CreateActiveObject(AO_Task_Compress);
	AO_Send[2] = CreateActiveObject(AO_Task_Encrypt);

	AO_Receive[0] = CreateActiveObject(AO_Task_Decrypt);
	AO_Receive[1] = CreateActiveObject(AO_Task_Decompress);
	AO_Receive[2] = CreateActiveObject(AO_Task_Decode);

	if (AO_Send[0] == NULL || AO_Send[1] == NULL || AO_Send[2] == NULL || AO_Receive[0] == NULL || AO_Receive[1] == NULL || AO_Receive[2] == NULL)
	{
		fprintf(stderr, "Error: main() failed: AO_Create() failed.\n");
		
		for (int i = 0; i < AO_NUM_SEND; i++)
			if (AO_Send[i] != NULL)
				stopActiveObject(AO_Send[i]);

		for (int i = 0; i < AO_NUM_RECEIVE; i++)
			if (AO_Receive[i] != NULL)
				stopActiveObject(AO_Receive[i]);

		
		return 1;
	}

	// Socket initialization
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror("Error: main() failed: socket() failed");

		for (int i = 0; i < AO_NUM_SEND; i++)
			if (AO_Send[i] != NULL)
				stopActiveObject(AO_Send[i]);

		for (int i = 0; i < AO_NUM_RECEIVE; i++)
			if (AO_Receive[i] != NULL)
				stopActiveObject(AO_Receive[i]);
		
		return 1;
	}

	struct sockaddr_in serv_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(MAIL_SERVER_PORT),
		.sin_addr.s_addr = inet_addr(MAIL_SERVER_IP)
	};

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Error: connect() failed");

		for (int i = 0; i < AO_NUM_SEND; i++)
			if (AO_Send[i] != NULL)
				stopActiveObject(AO_Send[i]);

		for (int i = 0; i < AO_NUM_RECEIVE; i++)
			if (AO_Receive[i] != NULL)
				stopActiveObject(AO_Receive[i]);
		
		return 1;
	}

	// Main loop
	while (true)
	{
		char input[64] = { 0 };

		fprintf(stdout, "Enter a command:\n"
						"\tSEND - Send a new mail\n"
						"\tRECEIVE - Receive a new mail\n"
						"\tGET - Get a mail from the server\n"
						"\texit - Exit the program\n");
		fgets(input, sizeof(input), stdin);

		input[strlen(input) - 1] = '\0';

		// Ignore empty input
		if (strlen(input) == 0)
			continue;

		if (strcmp(input, "exit") == 0)
		{
			fprintf(stdout, "Exiting...\n");
			break;
		}

		else if (strcmp(input, "SEND") == 0)
		{
			send(sockfd, input, (strlen(input) + 1), 0);
			PAttachment attachments = NULL;
			char subject[MAX_SUBJECT_LENGTH] = { 0 };
			char path[ATTACH_FILENAME_MAX] = { 0 };

			fprintf(stdout, "Enter subject: ");
			fflush(stdout);
			fgets(subject, sizeof(subject), stdin);

			subject[strlen(subject) - 1] = '\0';

			fprintf(stdout, "Enter number of attachments: ");
			fflush(stdout);

			int numAttachments = 0;

			if (scanf("%d", &numAttachments) != 1)
			{
				fprintf(stderr, "Error: main() failed: scanf() failed.\n");
				continue;
			}

			for (int i = 0; i < numAttachments; i++)
			{
				fprintf(stdout, "Enter attachment #%d path: ", i + 1);
				fflush(stdout);
				scanf("%s", path);

				fprintf(stdout, "Reading attachment #%d...\n", i + 1);

				fprintf(stdout, "Path: %s\n", path);

				FILE *fp = fopen(path, "rb");

				if (fp == NULL)
				{
					perror("fopen() failed");
					continue;
				}

				fseek(fp, 0, SEEK_END);
				uint32_t size = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				char *data = (char *)malloc(size);

				if (data == NULL)
				{
					fprintf(stderr, "Error: main() failed: malloc() failed.\n");
					continue;
				}

				if (fread(data, sizeof(char), size, fp) != size)
				{
					fprintf(stderr, "Error: main() failed: fread() failed.\n");
					continue;
				}

				fclose(fp);

				addAttachmentToList(&attachments, path, data, size);

				free(data);

				fprintf(stdout, "Attachment #%d added.\n", i + 1);
			}

			char message[8192] = { 0 };

			fprintf(stdout, "Enter message: ");
			fflush(stdout);

			scanf("%s", message);

			message[strlen(message) - 1] = '\0';

			struct sockaddr_in my_addr;
			socklen_t socklen = sizeof(my_addr);

			fprintf(stdout, "Preparing mail...\n");

			getsockname(sockfd, (struct sockaddr *)&my_addr, &socklen);

			char *mailToCompose = NULL;
			uint32_t len = createMailDataPacket(inet_ntoa(my_addr.sin_addr), subject, message, attachments, &mailToCompose);

			if (mailToCompose == NULL)
			{
				fprintf(stderr, "Error: main() failed: createMailDataPacket() failed.\n");
				continue;
			}

			// Create task to encapsulate mail data packet via encoding, compression, and encryption.
			// This will encapsulate only the fields after the total length field.
			PTask task = createTask(mailToCompose + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + sizeof(uint32_t), (len - USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + sizeof(uint32_t)));

			if (task == NULL)
			{
				fprintf(stderr, "Error: main() failed: createTask() failed.\n");
				continue;
			}

			// Start the chain of active objects
			ENQUEUE(getQueue(AO_Send[0]), task);

			// Wait for the task to be processed
			PTask task2 = DEQUEUE(sendOutputQueue, PTask);

			// Recompose mail data packet
			memcpy(mailToCompose + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + sizeof(uint32_t), task2->_data, task2->_data_size);

			// Update the length field
			((PMailRawPacketStripped)mailToCompose)->_mail_data_total_size = task2->_data_size;

			// Update the length variable
			len = task2->_data_size;

			// Free the task, since we don't need it anymore.
			destroyTask(task2);

			// Send length to the server, so the server knows how much data to expect
			send(sockfd, &len, sizeof(uint32_t), 0);

			// Send mail
			uint32_t totalSent = 0;

			while (totalSent < len)
			{
				int sent = send(sockfd, mailToCompose + totalSent, len- totalSent, 0);

				if (sent < 0)
				{
					perror("Error: main() failed: send() failed");
					return 1;
				}

				totalSent += sent;
			}

			free(mailToCompose);

			recv(sockfd, input, sizeof(input), 0);

			if (input[0] != 'O' || input[1] != 'K')
			{
				fprintf(stderr, "Error: main() failed: server failed to send OK.\n");
				continue;
			}

			memset(input, 0, sizeof(input));

			fprintf(stdout, "Mail sent successfully.\n");

			// Free the attachments
			freeAttachmentList(&attachments);
		}

		else if (strcmp(input, "LIST") == 0)
		{
			send(sockfd, input, (strlen(input) + 1), 0);

			fprintf(stdout, "Waiting for list...\n");

			char list_buffer[MAIL_MAX_BUFFER_SIZE] = { 0 };

			// Receive list
			recv(sockfd, list_buffer, sizeof(list_buffer), 0);

			fprintf(stdout, "List received:\n");
			fprintf(stdout, "%s\n", list_buffer);
		}

		else if (strcmp(input, "GET") == 0)
		{
			send(sockfd, input, (strlen(input) + 1), 0);

			fprintf(stdout, "Enter mail ID: ");
			fflush(stdout);

			uint32_t mail_id = 0;
			scanf("%u", &mail_id);

			send(sockfd, &mail_id, sizeof(uint32_t), 0);

			fprintf(stdout, "Waiting for mail...\n");

			uint32_t len = 0;

			// Receive length
			recv(sockfd, &len, sizeof(uint32_t), 0);

			char *mail = (char *)malloc(len);

			if (mail == NULL)
			{
				fprintf(stderr, "Error: main() failed: malloc() failed.\n");
				continue;
			}

			uint32_t totalReceived = 0;

			while (totalReceived < len)
			{
				int received = recv(sockfd, mail + totalReceived, len - totalReceived, 0);

				if (received < 0)
				{
					perror("Error: main() failed: recv() failed");
					return 1;
				}

				fprintf(stdout, "Received %d/%u bytes.\n", received, len);

				totalReceived += received;
			}

			fprintf(stdout, "Mail received:\n");

			// Create task to encapsulate mail data packet via decryption, decompression, and decoding.
			// This will encapsulate only the fields after the total length field.
			PTask task = createTask(mail + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + sizeof(uint32_t), (len - USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + sizeof(uint32_t)));

			if (task == NULL)
			{
				fprintf(stderr, "Error: main() failed: createTask() failed.\n");
				continue;
			}

			// Start the chain of active objects
			ENQUEUE(getQueue(AO_Receive[0]), task);

			// Wait for the task to be processed
			PTask task2 = DEQUEUE(receiveOutputQueue, PTask);

			// Recompose mail data packet
			memcpy(mail + USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + sizeof(uint32_t), task2->_data, task2->_data_size);

			// Update the length field
			((PMailRawPacketStripped)mail)->_mail_data_total_size = len;

			// Free the task, since we don't need it anymore.
			destroyTask(task2);

			// Save the mail and attachments to disk
			char maildir[ATTACH_FILENAME_MAX] = { 0 };
			sprintf(maildir, "mail/%d", mail_id);

			mkdir(maildir, 0777);

			sprintf(maildir, "mail/%d/mail.txt", mail_id);

			FILE *fp = fopen(maildir, "w");

			if (fp == NULL)
			{
				fprintf(stderr, "Error: main() failed: fopen() failed.\n");
				continue;
			}

			PMailRawPacketStripped mailPacket = (PMailRawPacketStripped)mail;
			uint32_t offset = (USERNAME_MAX_LENGTH + MAX_SUBJECT_LENGTH + (3 * sizeof(uint32_t)));
			char *mailBody = (char *)mailPacket + offset;

			fprintf(fp, "From: %s\n", mailPacket->_mail_from);
			fprintf(fp, "Subject: %s\n", mailPacket->_mail_subject);
			fprintf(fp, "Size: %u\n", mailPacket->_mail_data_total_size);
			fprintf(fp, "Attachments: %u\n", mailPacket->_mail_data_attachments_num);
			fprintf(fp, "Body: %s\n", mailBody);

			fprintf(stdout, "From: %s\n", mailPacket->_mail_from);
			fprintf(stdout, "Subject: %s\n", mailPacket->_mail_subject);
			fprintf(stdout, "Size: %u\n", mailPacket->_mail_data_total_size);
			fprintf(stdout, "Attachments: %u\n", mailPacket->_mail_data_attachments_num);
			fprintf(stdout, "Body: %s\n", mailBody);

			fclose(fp);

			offset += mailPacket->_mail_data_body_size;

			for (uint32_t i = 0; i < mailPacket->_mail_data_attachments_num; i++)
			{
				char filename[ATTACH_FILENAME_MAX] = { 0 };
				char *attachName = (char *)mailPacket + offset;
				sprintf(filename, "mail/%d/%s", mail_id, attachName);

				fp = fopen(filename, "w");

				if (fp == NULL)
				{
					fprintf(stderr, "Error: main() failed: fopen() failed.\n");
					continue;
				}

				offset += ATTACH_FILENAME_MAX;

				uint32_t size = *((uint32_t *)(mailPacket + offset));
				offset += sizeof(uint32_t);

				fwrite(mailPacket + offset, size, 1, fp);

				fclose(fp);

				offset += size;
			}

			free(mail);

			fprintf(stdout, "Mail saved successfully.\n");
		}

		else
		{
			fprintf(stderr, "Error: main() failed: invalid command.\n");
			continue;
		}
	}

	close(sockfd);

	fprintf(stdout, "Mail Client shutdown...\n");

	for (int i = 0; i < AO_NUM_SEND; i++)
		if (AO_Send[i] != NULL)
			stopActiveObject(AO_Send[i]);

	for (int i = 0; i < AO_NUM_RECEIVE; i++)
		if (AO_Receive[i] != NULL)
			stopActiveObject(AO_Receive[i]);

	return 0;
}