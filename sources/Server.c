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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

PMail mail_list = NULL;

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
		perror("Error: socket failed");
		return 1;
	}

	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		perror("Error: setsockopt failed");
		close(server_socket);
		return 1;
	}

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Error: bind failed");
		close(server_socket);
		return 1;
	}

	if (listen(server_socket, MAIL_MAX_CLIENTS) < 0)
	{
		perror("Error: listen failed");
		close(server_socket);
		return 1;
	}

	fprintf(stdout, "Mail Server is up and running on port %d\n", MAIL_SERVER_PORT);

	PMail mail_list = NULL;

	while (true)
	{
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

		if (client_socket < 0)
		{
			perror("Error: accept failed");
			return 1;
		}

		fprintf(stdout, "Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		char buffer[MAIL_MAX_BUFFER_SIZE];
		int bytes_read = recv(client_socket, buffer, MAIL_MAX_BUFFER_SIZE, 0);

		if (bytes_read < 0)
		{
			perror("Error: read failed");
			return 1;
		}

		else if (bytes_read == 0)
		{
			fprintf(stderr, "Warning: Client disconnected mid-connection\n");
			close(client_socket);
			continue;
		}

		else if (bytes_read < MAIL_MAX_BUFFER_SIZE)
			fprintf(stderr, "Error: Read only %d bytes out of %d\n", bytes_read, MAIL_MAX_BUFFER_SIZE);

		else
			fprintf(stdout, "Read %d bytes\n", bytes_read);

		buffer[bytes_read] = '\0';

		if (strcmp(buffer, "LIST") == 0)
		{
			char list_buffer[MAIL_MAX_BUFFER_SIZE];
			PMail mail = mail_list;
			int list_buffer_size = 0;
			int id = 0;

			while (mail != NULL)
			{
				//list_buffer_size += sprintf(list_buffer + list_buffer_size, "ID: %d From: %s; Subject: %s\n", ++id, mail->_from, mail->_mail_data->subject);
				mail = mail->_mail_next;
			}

			if (list_buffer_size == 0)
				list_buffer_size = sprintf(list_buffer, "No mails\n");

			int b_sent = send(client_socket, list_buffer, list_buffer_size, 0);

			if (b_sent < 0)
			{
				perror("Error: recv failed");
				close(client_socket);
				close(server_socket);
				return 1;
			}

			else if (b_sent == 0)
			{
				fprintf(stderr, "Warning: Client disconnected\n");
				close(client_socket);
				continue;
			}

			else if (b_sent < list_buffer_size)
				fprintf(stderr, "Warning: Sent only %d bytes out of %d\n", b_sent, list_buffer_size);

			else
				fprintf(stdout, "Sent %d bytes\n", b_sent);

			fprintf(stdout, "Sent list of %d mails to client %s:%d\n", id, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		}

		/*else if (strcmp(buffer, "SEND") == 0)
		{
			char subject_buffer[MAIL_MAX_BUFFER_SIZE];
			char sender_buffer[MAIL_MAX_BUFFER_SIZE];
			char recipient_buffer[MAIL_MAX_BUFFER_SIZE];
			char message_buffer[MAIL_MAX_BUFFER_SIZE];
			int subject_buffer_size = 0;
			int sender_buffer_size = 0;
			int recipient_buffer_size = 0;
			int message_buffer_size = 0;

			if (read(client_socket, subject_buffer, MAIL_MAX_BUFFER_SIZE) < 0)
			{
				perror("Error: read failed");
				return 1;
			}

			subject_buffer[MAIL_MAX_BUFFER_SIZE - 1] = '\0';
			subject_buffer_size = strlen(subject_buffer);

			if (read(client_socket, sender_buffer, MAIL_MAX_BUFFER_SIZE) < 0)
			{
				perror("Error: read failed");
				return 1;
			}

			sender_buffer[MAIL_MAX_BUFFER_SIZE - 1] = '\0';
			sender_buffer_size = strlen(sender_buffer);

		}*/

		close(client_socket);
	}

	close(server_socket);

	return 0;
}