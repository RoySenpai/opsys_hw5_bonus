/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Active Object (AO) - Tasks Header File
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

#ifndef _TASK_H
#define _TASK_H

/********************/
/* Includes Section */
/********************/

#include "Settings.h"
#include <sys/types.h>


/*******************/
/* Structs Section */
/*******************/

/*
 * @brief A struct representing a task - a task is a unit of work to be done by an active object.
 * @param _data A generic purpose pointer to data to pass to the active object.
 * @param _data_size The size of the data to pass to the active object.
 * @note The struct is used to pass data to the active objects.
*/
typedef struct _Task {

	/*
	 * @brief The data to pass to the active object.
	 * @note The data is generic purpose, and can be used for any purpose by the active object.
	*/
	void *_data;

	/*
	 * @brief The size of the data to pass to the active object.
	 * @note This indicates the size of the data to pass to the active object, and is used for memory allocation purposes.
	*/
	u_int32_t _data_size;
} Task, *PTask;


/********************************/
/* Function Declaratios Section */
/********************************/


/*
 * @brief The function creates a task.
 * @param data A generic purpose pointer to data to pass to the active object.
 * @param size The size of the data to pass to the active object.
 * @return A pointer to the created task, NULL otherwise.
 * @note The function is not thread safe by default.
 */
PTask createTask(void *data, u_int32_t size);

/*
 * @brief The function destroys a task.
 * @param task A pointer to the task to destroy.
 * @note The function is not thread safe by default.
 */
void destroyTask(PTask task);

#if DEBUG_MESSAGES == 1
	/*
	* @brief The function prints a task.
	* @param task A pointer to the task to print.
	* @note The function is not thread safe by default.
	* @note The function prints the task in the following format: "Task: <address>, <size>".
	* @note Used for debugging purposes only.
	*/
	void printTask(PTask task);
#endif /* DEBUG_MESSAGES == 1 */

#endif // _TASK_H
