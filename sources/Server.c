/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Mail Server implementation
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
#include "../include/LinkedList.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

int main(void) {
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(MAIL_SERVER_PORT),
		.sin_addr.s_addr = INADDR_ANY
	};

	fprintf(stdout, "\t\t\t\033[0;36mMail Server\n"
					"\tCopyright (C) 2023  Roy Simanovich and Linor Ronen\n"
					"\tThis program comes with ABSOLUTELY NO WARRANTY.\n"
					"\tThis is free software, and you are welcome to redistribute it\n"
					"\tunder certain conditions; see `LICENSE' for details.\033[0;37m\n\n");
	
	fprintf(stdout, "Mail Server startup...\n");

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	int reuse = 1;

	if (server_socket < 0)
	{
		perror("Error: socket() failed");
		return 1;
	}

	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		perror("Error: setsockopt() failed");
		close(server_socket);
		return 1;
	}

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Error: bind() failed");
		close(server_socket);
		return 1;
	}

	if (listen(server_socket, MAIL_MAX_CLIENTS) < 0)
	{
		perror("Error: listen() failed");
		close(server_socket);
		return 1;
	}

	fprintf(stdout, "Mail Server is up and running on port %d\n", MAIL_SERVER_PORT);

	PLinkedList mail_list = createLinkedList();

	if (mail_list == NULL)
	{
		fprintf(stderr, "Error: createLinkedList() failed\n");
		close(server_socket);
		return 1;
	}

	while (true)
	{
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

		if (client_socket < 0)
		{
			perror("Error: accept() failed");
			return 1;
		}

		fprintf(stdout, "Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		while (true)
		{
			char buffer[MAIL_MAX_BUFFER_SIZE];
			int bytes_read = recv(client_socket, buffer, MAIL_MAX_BUFFER_SIZE, 0);

			if (bytes_read < 0)
			{
				perror("Error: recv() failed");
				return 1;
			}

			else if (bytes_read == 0)
			{
				fprintf(stderr, "Warning: Client disconnected mid-connection\n");
				break;
			}

			else
				fprintf(stdout, "Read %d bytes\n", bytes_read);

			buffer[bytes_read] = '\0';

			if (strcmp(buffer, "LIST") == 0)
			{
				char list_buffer[MAIL_MAX_BUFFER_SIZE] = {0};
				int list_buffer_size = 0;
				int id = 0;

				fprintf(stdout, "Client requested mail list\n");

				PNode current_node = mail_list->head;

				while (current_node != NULL)
				{
					PMailRawPacketStripped mail = (PMailRawPacketStripped)current_node->data;

					if (mail == NULL)
					{
						fprintf(stderr, "Error: current_node->data is NULL\n");
						break;
					}

					list_buffer_size += sprintf(list_buffer + list_buffer_size, "%d. From: %s; Subject: %s; Size: %u bytes\n", ++id, mail->_mail_from, mail->_mail_subject, mail->_mail_data_total_size);

					current_node = current_node->next;
				}

				if (list_buffer_size == 0)
					list_buffer_size = sprintf(list_buffer, "No mail in the server\n");

				if (send(client_socket, list_buffer, list_buffer_size, 0) < 0)
				{
					perror("Error: send() failed");
					return 1;
				}

				fprintf(stdout, "Sent %d bytes\n", list_buffer_size);
			}

			else if (strcmp(buffer, "SEND") == 0)
			{
				uint32_t mail_data_total_size = 0;

				if (recv(client_socket, &mail_data_total_size, sizeof(mail_data_total_size), 0) < 0)
				{
					perror("Error: recv() failed");
					return 1;
				}

				fprintf(stdout, "Expecting %u bytes\n", mail_data_total_size);

				char *mail_data = (char *)calloc(mail_data_total_size, sizeof(char));

				if (mail_data == NULL)
				{
					fprintf(stderr, "Error: calloc() failed\n");
					break;
				}

				int received_bytes = 0;

				while (received_bytes < (int)mail_data_total_size)
				{
					int bytes_read = recv(client_socket, mail_data + received_bytes, mail_data_total_size - received_bytes, 0);

					if (bytes_read < 0)
					{
						perror("Error: recv() failed");
						return 1;
					}

					else if (bytes_read == 0)
					{
						fprintf(stderr, "Error: Client disconnected mid-connection, Mail data may be corrupted, Aborting\n");
						free(mail_data);
						break;
					}

					fprintf(stdout, "Received %d/%u bytes\n", received_bytes + bytes_read, mail_data_total_size);

					received_bytes += bytes_read;
				}

				if (received_bytes < (int)mail_data_total_size)
					break;

				PMailRawPacketStripped mail = (PMailRawPacketStripped)mail_data;

				if (mail->_mail_data_total_size != mail_data_total_size)
				{
					fprintf(stderr, "Error: Mail data is corrupted, Aborting\n");
					free(mail_data);
					break;
				}

				if (addNode(mail_list, mail_data) != 0)
				{
					fprintf(stderr, "Error: addNode() failed\n");
					free(mail_data);
					break;
				}

				fprintf(stdout, "Mail received from %s, subject: %s, total size: %u\n", mail->_mail_from, mail->_mail_subject, mail->_mail_data_total_size);

				if (send(client_socket, "OK", 2, 0) < 0)
				{
					perror("Error: send() failed");
					return 1;
				}
			}

			else if (strcmp(buffer, "GET") == 0)
			{
				uint32_t mail_id = 0;

				if (recv(client_socket, &mail_id, sizeof(mail_id), 0) < 0)
				{
					perror("Error: recv() failed");
					return 1;
				}

				PNode current_node = mail_list->head;

				while (current_node != NULL)
				{
					PMailRawPacketStripped mail = (PMailRawPacketStripped)current_node->data;

					if (mail == NULL)
					{
						fprintf(stderr, "Error: current_node->data is NULL\n");
						break;
					}

					if (mail_id == 1)
						break;

					mail_id--;
					current_node = current_node->next;
				}

				if (current_node == NULL)
				{
					fprintf(stderr, "Error: Mail not found\n");
					break;
				}

				PMailRawPacketStripped mail = (PMailRawPacketStripped)current_node->data;

				if (mail == NULL)
				{
					fprintf(stderr, "Error: current_node->data is NULL\n");
					break;
				}

				if (send(client_socket, mail, mail->_mail_data_total_size, 0) < 0)
				{
					perror("Error: send() failed");
					return 1;
				}

				fprintf(stdout, "Sent %u bytes\n", mail->_mail_data_total_size);

				int bytes_send = 0;

				while (bytes_send < (int)mail->_mail_data_total_size)
				{
					int bytes_sent = send(client_socket, mail + bytes_send, mail->_mail_data_total_size - bytes_send, 0);

					if (bytes_sent < 0)
					{
						perror("Error: send() failed");
						return 1;
					}

					else if (bytes_sent == 0)
					{
						fprintf(stderr, "Error: Client disconnected mid-connection, stopping sending mail, Aborting\n");
						break;
					}

					bytes_send += bytes_sent;
				}

				if (bytes_send >= (int)mail->_mail_data_total_size)
					fprintf(stdout, "Mail sent to %s, subject: %s, total size: %u\n", inet_ntoa(client_addr.sin_addr), mail->_mail_subject, mail->_mail_data_total_size);
			}

			else if (strcmp(buffer, "DEL") == 0)
			{
				uint32_t mail_id = 0;

				if (recv(client_socket, &mail_id, sizeof(mail_id), 0) < 0)
				{
					perror("Error: recv() failed");
					return 1;
				}

				mail_id = ntohl(mail_id);

				PNode current_node = mail_list->head;

				while (current_node != NULL)
				{
					PMailRawPacketStripped mail = (PMailRawPacketStripped)current_node->data;

					if (mail == NULL)
					{
						fprintf(stderr, "Error: current_node->data is NULL\n");
						break;
					}

					if (mail_id == 0)
						break;

					mail_id--;
					current_node = current_node->next;
				}

				if (current_node == NULL)
				{
					fprintf(stderr, "Error: Mail not found\n");
					continue;
				}

				if (removeNode(mail_list, current_node) != 0)
				{
					fprintf(stderr, "Error: removeNode() failed\n");
					break;
				}

				fprintf(stdout, "Mail deleted\n");

				if (send(client_socket, "OK", 2, 0) < 0)
				{
					perror("Error: send() failed");
					return 1;
				}
			}
			
			else
				fprintf(stderr, "Error: Unknown command\n");
		}

		close(client_socket);
	}

	close(server_socket);

	return 0;
}