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

	if (AO_Send[0] == NULL || AO_Send[1] == NULL || AO_Send[2] == NULL ||
		AO_Receive[0] == NULL || AO_Receive[1] == NULL || AO_Receive[2] == NULL)
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
		perror("Error: main() failed: connect() failed");

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

		fprintf(stdout, "Enter a command: ");
		fflush(stdout);
		fgets(input, sizeof(input), stdin);

		input[strlen(input) - 1] = '\0';

		if (strlen(input) == 0)
			continue;

		if (strcmp(input, "exit") == 0)
		{
			fprintf(stdout, "Exiting...\n");
			break;
		}

		else if (strcmp(input, "SEND"))
		{

		}

		else if (strcmp(input, "LIST"))
		{

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