/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Email Server and Client settings file
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

#ifndef _SETTINGS_H
#define _SETTINGS_H

/********************/
/* Includes Section */
/********************/

#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE)
	#if __STDC_VERSION__ >= 199901L
		#define _XOPEN_SOURCE 600   /* SUS v3, POSIX 1003.1 2004 (POSIX 2001 + Corrigenda) */
	#else
		#define _XOPEN_SOURCE 500   /* SUS v2, POSIX 1003.1 1997 */
	#endif /* __STDC_VERSION__ */
#endif /* !_XOPEN_SOURCE && !_POSIX_C_SOURCE */

#include <bits/stdint-uintn.h>

/********************/
/* Settings Section */
/********************/

/*
 * @brief A macro for printing debug messages.
 * @note 1 for printing debug messages, 0 otherwise.
 * @note The default value is 0.
*/
#define DEBUG_MESSAGES			1

/*
 * @brief Mail server port, that the server will listen to.
 * @note The default value is port 9999 (TCP, IPv4, custom port for the vSMTP protocol).
*/
#define MAIL_SERVER_PORT		9999

/*
 * @brief The maximum number of clients that can connect to the server.
 * @note The default value is 10.
 * @note The value must be greater than 0.
*/
#define MAIL_MAX_CLIENTS		128

/*
 * @brief The maximum buffer size for storing commands.
 * @note The default value is 4096 bytes.
 * @note The value must be greater than 0.
*/
#define MAIL_MAX_BUFFER_SIZE 	4096

/*
 * @brief The maximum length of the file name, (the attachment name).
 * @note The default value is 256 characters.
 * @note The value must be greater than 0.
*/
#define ATTACH_FILENAME_MAX 	256

/*
 * @brief The maximum length of the subject.
 * @note The default value is 256 characters (including the null terminator character).
 * @note The value must be greater than 0.
*/
#define MAX_SUBJECT_LENGTH		256

/*
 * @brief The maximum length of the username.
 * @note The default value is 16 characters (including the null terminator character).
 * @note The value must be greater than 0.
*/
#define USERNAME_MAX_LENGTH		16

#endif /* _SETTINGS_H */