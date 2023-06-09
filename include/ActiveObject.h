/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Active Object (AO) - Active Object Header File
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

#ifndef _ACTIVE_OBJECT_H
#define _ACTIVE_OBJECT_H

#include "Queue.h"


/****************************/
/* Type Definitions Section */
/****************************/

/*
 * @brief A function pointer that is used to store the function that the active object will execute.
 * @param param The parameter that the active object will pass to the function that is stored in the active object itself.
 * @return An int, representing the status of the function that the active object will execute:
 * 			1 means to continue executing the function that the active object will execute.
 * 			0 means to stop executing the function that the active object will execute.
 * @note The function should receive a void pointer.
*/
typedef int (*PQueueFunc)(void *);


/******************************/
/* Struct Definitions Section */
/******************************/

/*
 * @brief An active object struct.
 * @param id The id of the active object.
 * @param thread The thread that executes the function that is stored in the queue.
 * @param queue The queue that stores the parameters for the function that the
 					active object will pass to the function that is stored in the active object itself.
 * @param func The function that the active object will execute.
 * @note This struct is used to represent the active object itself.
 * @note The active object is a thread that executes a function that is stored in the queue.
*/
typedef struct _ActiveObject {
	/*
	 * @brief The id of the active object.
	 * @note This is an unsigned int.
	 * @note The id is used to identify the active object, and is an incrementing number.
	 * @note This field has no other purpose other than to identify the active object for debugging purposes.
	*/
	unsigned int id;

	/*
	 * @brief The thread that executes the function that is stored in the queue.
	 * @note This is a pthread_t struct.
	 * @note The thread is created when the active object is created.
	 * @note The thread is destroyed when the active object is destroyed.
	 * @warning Only use this field to join the thread, or to check if the thread is running,
	 * 			never change this field directly or cancel the thread.
	*/
	pthread_t thread;

	/*
	 * @brief The queue that stores the parameters for the function that the
	 			active object will pass to the function that is stored in the active object itself.
	 * @note This is a pointer to a Queue struct.
	 * @warning Do not use this field directly, use the getQueue function instead.
	*/
	PQueue queue;

	/*
	 * @brief The function that the active object will execute.
	 * @note This is a function pointer. The function should receive a void pointer and return an int.
	 * @note The return value of the function should be 1 to continue executing the function that the active object will execute,
	 			or 0 to stop executing the function that the active object will execute and stop the active object.
	 * @warning Don't change this field after the active object is created, unless you know what you are doing.
	*/
	PQueueFunc func;
} ActiveObject, *PActiveObject;


/********************************/
/* Function Declaratios Section */
/********************************/

/*
 * @brief Creates an active object and starts its thread.
 * @param func The function that the active object will execute whenever it dequeues a parameter.
 * @return A pointer to the created active object.
 * @note The active object is created with a function pointer that is used to store
 * 			the function that the active object will execute.
 * @note The active object is a thread that executes a function that is stored in the queue.
*/
PActiveObject CreateActiveObject(PQueueFunc func);

/*
 * @brief Returns the queue of the active object.
 * @param activeObject The active object.
 * @return A pointer to the queue that stores the parameters for the function that the
 			active object will pass to the function that is stored in the active object itself.
*/
PQueue getQueue(PActiveObject activeObject);

/*
 * @brief Stops and releases the active object.
 * @param activeObject The active object.
 * @return void.
 * @note This function stops the thread of the active object and releases the memory that was allocated for the active object.
*/
void stopActiveObject(PActiveObject activeObject);

/*
 * @brief The thread function that the active object executes when it is created.
 * @param activeObject The active object itself.
 * @return The active object itself on success, NULL on failure.
 * @note This function is used to execute the function that is stored in the queue.
 * @warning Never call this function directly, use the CreateActiveObject function instead, which creates the active object and starts its thread.
*/
void *activeObjectRunFunction(void *activeObject);

#endif // _ACTIVE_OBJECT_H