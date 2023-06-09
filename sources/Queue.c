/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Active Object (AO) - Queue Implementation
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

#include "../include/Queue.h"
#include <stdio.h>
#include <stdlib.h>

PQueue queueCreate() {
	PQueue queue = (PQueue)malloc(sizeof(Queue));

	if (queue == NULL)
	{
		perror("malloc() failed");
		return NULL;
	}

	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;

	MUTEX_INIT(&queue->lock);
	COND_INIT(&queue->cond);

	return queue;
}

void queueDestroy(PQueue queue) {
	if (queue == NULL)
	{
		fprintf(stderr, "queueDestroy() failed: queue is NULL\n");
		return;
	}

	MUTEX_LOCK(&queue->lock);

	PQueueNode node = queue->head;

	// If the queue isn't empty, free all the nodes and free the data they hold.
	while (node != NULL)
	{
		PQueueNode next = node->next;
		free(node->data);
		free(node);
		node = next;
	}

	MUTEX_UNLOCK(&queue->lock);
	COND_DESTROY(&queue->cond);
	MUTEX_DESTROY(&queue->lock);

	free(queue);
}

void queueEnqueue(PQueue queue, void *data) {
	if (queue == NULL)
	{
		fprintf(stderr, "queueEnqueue() failed: queue is NULL\n");
		return;
	}

	PQueueNode node = (PQueueNode)malloc(sizeof(QueueNode));

	if (node == NULL)
	{
		perror("malloc() failed");
		return;
	}

	node->data = data;
	node->next = NULL;

	MUTEX_LOCK(&queue->lock);

	if (queue->head == NULL)
	{
		queue->head = node;
		queue->tail = node;

		COND_SIGNAL(&queue->cond);
	}

	else
	{
		queue->tail->next = node;
		queue->tail = node;
	}

	queue->size++;

	MUTEX_UNLOCK(&queue->lock);
}

void *queueDequeue(PQueue queue) {
	if (queue == NULL)
	{
		fprintf(stderr, "queueDequeue() failed: queue is NULL\n");
		return NULL;
	}

	MUTEX_LOCK(&queue->lock);

	// Wait for the queue to be non-empty.
	while (queue->head == NULL)
		COND_WAIT(&queue->cond, &queue->lock);

	PQueueNode node = queue->head;
	void *data = node->data;

	queue->head = node->next;

	if (queue->head == NULL)
		queue->tail = NULL;

	free(node);
	queue->size--;

	MUTEX_UNLOCK(&queue->lock);

	return data;
}

int queueIsEmpty(PQueue queue) {
	if (queue == NULL)
	{
		fprintf(stderr, "queueIsEmpty() failed: queue is NULL\n");
		return -1;
	}

	MUTEX_LOCK(&queue->lock);
	int isEmpty = (queue->size == 0);
	MUTEX_UNLOCK(&queue->lock);

	return isEmpty;
}

#if DEBUG_MESSAGES == 1
	int queueSize(PQueue queue) {
		if (queue == NULL)
		{
			fprintf(stderr, "queueSize() failed: queue is NULL\n");
			return -1;
		}

		MUTEX_LOCK(&queue->lock);
		int size = queue->size;
		MUTEX_UNLOCK(&queue->lock);

		return size;
	}

	void *queuePeek(PQueue queue) {
		if (queue == NULL)
		{
			fprintf(stderr, "queuePeek() failed: queue is NULL\n");
			return NULL;
		}

		MUTEX_LOCK(&queue->lock);

		if (queue->head == NULL)
		{
			MUTEX_UNLOCK(&queue->lock);
			return NULL;
		}

		void *data = queue->head->data;

		MUTEX_UNLOCK(&queue->lock);

		return data;
	}

	void *queuePeekTail(PQueue queue) {
		if (queue == NULL)
		{
			fprintf(stderr, "queuePeekTail() failed: queue is NULL\n");
			return NULL;
		}

		MUTEX_LOCK(&queue->lock);

		if (queue->tail == NULL)
		{
			MUTEX_UNLOCK(&queue->lock);
			return NULL;
		}

		void *data = queue->tail->data;

		MUTEX_UNLOCK(&queue->lock);

		return data;
	}

	void queuePrint(PQueue queue) {
		if (queue == NULL)
		{
			fprintf(stderr, "queuePrint() failed: queue is NULL\n");
			return;
		}

		MUTEX_LOCK(&queue->lock);

		fprintf(stdout, "Queue info:\n[\n");
		fprintf(stdout, "\tQueue: %p\n", (void *)queue);
		fprintf(stdout, "\tQueue size: %d\n", queue->size);
		fprintf(stdout, "\tQueue head: %p\n", (void *)queue->head);
		fprintf(stdout, "\tQueue tail: %p\n", (void *)queue->tail);
		fprintf(stdout, "\tQueue lock: %p\n", (void *)&queue->lock);
		fprintf(stdout, "\tQueue cond: %p\n", (void *)&queue->cond);
		fprintf(stdout, "\tQueue nodes:\n\t\t");

		if (queue->head == NULL)
		{
			fprintf(stdout, "NULL\n]\n");
			MUTEX_UNLOCK(&queue->lock);
			return;
		}

		PQueueNode node = queue->head;

		while (node != NULL)
		{
			fprintf(stdout, "%p -> ", node->data);
			node = node->next;
		}

		fprintf(stdout, "\n]\n");

		MUTEX_UNLOCK(&queue->lock);
	}
#endif // DEBUG_MESSAGES