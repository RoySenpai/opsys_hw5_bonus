/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Active Object (AO) - Task Implementation
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

#include "../include/Task.h"
#include <stdio.h>
#include <stdlib.h>

PTask createTask(void *data, u_int32_t size) {
	PTask task = (PTask)malloc(sizeof(Task));

	if (task == NULL)
	{
		perror("malloc() failed");
		return NULL;
	}

	task->_data = data;
	task->_data_size = size;

	return task;
}

void destroyTask(PTask task) {
	if (task == NULL)
	{
		fprintf(stderr, "destroyTask() failed: task is NULL\n");
		return;
	}

	free(task);
}

#if DEBUG_MESSAGES == 1
	void printTask(PTask task) {
		if (task == NULL)
		{
			fprintf(stderr, "printTask() failed: task is NULL\n");
			return;
		}

		printf("Task: %p, %u\n", task->_data, task->_data_size);
	}
#endif