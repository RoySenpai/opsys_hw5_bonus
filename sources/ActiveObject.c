/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Active Object (AO) - Active Object Implementation
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

#include "../include/ActiveObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

PActiveObject CreateActiveObject(PQueueFunc func) {
	static unsigned int id = 0;

	PActiveObject activeObject = (PActiveObject)malloc(sizeof(ActiveObject));

	if (activeObject == NULL)
	{
		perror("CreateActiveObject() failed: malloc() failed");
		return NULL;
	}

	activeObject->queue = queueCreate();

	if (activeObject->queue == NULL)
	{
		fprintf(stderr, "CreateActiveObject() failed: queueCreate() failed\n");
		free(activeObject);
		return NULL;
	}

	if (func == NULL)
	{
		fprintf(stderr, "CreateActiveObject() failed: func is NULL\n");
		queueDestroy(activeObject->queue);
		free(activeObject);
		return NULL;
	}

	activeObject->func = func;
	activeObject->id = id++;

	int ret = pthread_create(&activeObject->thread, NULL, activeObjectRunFunction, activeObject);

	if (ret != 0)
	{
		fprintf(stderr, "CreateActiveObject() failed: pthread_create() failed: %s\n", strerror(ret));
		queueDestroy(activeObject->queue);
		free(activeObject);
		return NULL;
	}

	if (DEBUG_MESSAGES)
		fprintf(stdout, "CreateActiveObject() succeeded: activeObject created and thread started, id %d\n", activeObject->id);

	return activeObject;
}

PQueue getQueue(PActiveObject activeObject) {
	if (activeObject == NULL)
	{
		if (DEBUG_MESSAGES)
			fprintf(stderr, "getQueue() failed: activeObject is NULL\n");
		return NULL;
	}

	return activeObject->queue;
}

void stopActiveObject(PActiveObject activeObject) {
	if (activeObject == NULL)
	{
		fprintf(stderr, "stopActiveObject() failed: activeObject is NULL\n");
		return;
	}

	unsigned int id = activeObject->id;

	// Gracefully stop the thread, if it is still running
	pthread_cancel(activeObject->thread);
	activeObject->func = NULL;

	// Wait for the thread to end
	//pthread_join(activeObject->thread, NULL);

	queueDestroy(activeObject->queue);
	free(activeObject);

	if (DEBUG_MESSAGES)
		fprintf(stdout, "stopActiveObject() succeeded: activeObject stopped and destroyed, id %d\n", id);
}

void *activeObjectRunFunction(void *activeObject) {
	if (activeObject == NULL)
	{
		fprintf(stderr, "activeObjectRunFunction() failed: activeObject is NULL\n");
		return NULL;
	}

	PActiveObject ao = (PActiveObject)activeObject;
	PQueue queue = ao->queue;
	void *task = NULL;

	if (queue == NULL)
	{
		fprintf(stderr, "activeObjectRunFunction() failed: queue is NULL\n");
		return NULL;
	}

	if (DEBUG_MESSAGES)
		fprintf(stdout, "ActiveObject thread started, id: %d\n", ao->id);

	while (ao->func && ((task = DEQUEUE(queue, void *))))
	{
		int ret = ao->func(task);

		if (ret == 0)
		{
			if (DEBUG_MESSAGES)
				fprintf(stdout, "activeObjectRunFunction() succeeded: task completed, id %d\n", ao->id);
	
			break;
		}
	}

	if (DEBUG_MESSAGES)
	{
		if (queueIsEmpty(queue))
			fprintf(stdout, "activeObjectRunFunction() succeeded: queue is empty, thread ended, id %d\n", ao->id);
		
		else
			fprintf(stderr, "activeObjectRunFunction() failed: queue is not empty, id %d\n", ao->id);
	}

	return activeObject;
}