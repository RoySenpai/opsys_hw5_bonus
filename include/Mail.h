/*
 *  Operation Systems (OSs) Course Assignment 5 Bonus
 *  Mail - Header File
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

#ifndef _MAIL_H
#define _MAIL_H

/********************/
/* Includes Section */
/********************/

#include "Settings.h"


/*******************/
/* Structs Section */
/*******************/

/*
 * @brief A struct representing an attachment - an attachment is a file to be sent with the mail.
 * @param _attach_name The name of the attachment (the file name itself).
 * @param _attach_data The data of the attachment.
 * @param _attach_data_size The size of the attachment.
 * @param _attach_next The next attachment in the list.
 * @note The next attachment is used to create a linked list of attachments.
 * @note The name is used to identify the attachment.
 * @note The data is the actual file data, and we represent it as a stream of bytes.
 * @note The size is used for memory allocation purposes.
*/
typedef struct _Mail_Attachment_Node {
	/*
	 * @brief The name of the attachment (the file name itself).
	 * @note The name is used to identify the attachment.
	*/
	char _attach_name[ATTACH_FILENAME_MAX];

	/*
	 * @brief The size of the attachment.
	 * @note The size is used for memory allocation purposes.
	*/
	uint32_t _attach_data_size;

	/*
	 * @brief The data of the attachment.
	 * @note The data is the actual file data, and we represent it as a stream of bytes.
	*/
	char *_attach_data;

	/*
	 * @brief The next attachment in the list.
	 * @note The next attachment is used to create a linked list of attachments.
	*/
	struct _Mail_Attachment_Node *_attach_next;
} Attachment, *PAttachment;

/*
 * @brief A struct representing a stripped raw mail package, which is a mail package with the header fields only.
 * @param _mail_from The sender of the mail (Fixed size of USERNAME_MAX_LENGTH).
 * @param _mail_subject The subject of the mail (Fixed size of MAX_SUBJECT_LENGTH).
 * @param _mail_total_size The total size of the mail, including all the fields and the data.
 * @param _mail_data_body_size The size of the body of the mail.
 * @param _mail_data_attachments_num The number of attachments of the mail.
 * @warning Anything beyond the _mail_data_total_size field should be encoded, compressed and encrypted before sending it to the server.
 * @note The _mail_data_total_size field is calculated by this formula:
 * 			_mail_data_total_size = sizeof(struct _Mail_Raw_Packet_Stripped) + _mail_data_body_size + sum of the sizes of the attachments (if there are any).
 * @note The body is a string of characters, and it's size is controlled by the _mail_data_body_size field.
 * @note The attachments are a list of attachments, and the number of attachments is controlled by the _mail_data_attachments_num field.
 * @note Do not use this struct directly, only use it when you need to send the mail to the server or receive the mail from the server.
*/
typedef struct _Mail_Raw_Packet_Stripped {
	/*
	 * @brief The sender of the mail (Fixed size of USERNAME_MAX_LENGTH).
	 * @note The sender is used to identify the sender of the mail.
	*/
	char _mail_from[USERNAME_MAX_LENGTH];

	/*
	 * @brief The subject of the mail (Fixed size of MAX_SUBJECT_LENGTH).
	 * @note The subject is used to identify the subject of the mail.
	 * @note The subject can be empty.
	*/
	char _mail_subject[MAX_SUBJECT_LENGTH];

	/*
	 * @brief The total size of the mail, including all the fields and the data.
	 * @note The _mail_data_total_size field is calculated by this formula:
	 * 			_mail_data_total_size = sizeof(struct _Mail_Raw_Packet_Stripped) + _mail_data_body_size + sum of the sizes of the attachments (if there are any).
	*/
	uint32_t _mail_data_total_size;

	/*
	 * @brief The size of the body of the mail.
	 * @note The body is a string of characters, and it's size is controlled by the _mail_data_body_size field.
	 * @note The body can be empty.
	*/
	uint32_t _mail_data_body_size;

	/*
	 * @brief The number of attachments of the mail.
	 * @note The attachments are a list of attachments, and the number of attachments is controlled by the _mail_data_attachments_num field.
	 * @note The number of attachments can be zero.
	*/
	uint32_t _mail_data_attachments_num;
} MailRawPacketStripped, *PMailRawPacketStripped;


/*
 * @brief A struct representing a mail attachment, in raw form.
 * @param _mail_attachment_name The name of the attachment (Fixed size of ATTACH_FILENAME_MAX).
 * @param _mail_attachment_size The size of the attachment.
 * @note The attachment is a file, and it's size is controlled by the _mail_attachment_size field.
 * @note Do not use this struct directly, only use it when you need to send the mail to the server or receive the mail from the server.
*/
typedef struct _Mail_Attachment_Data {
	/*
	 * @brief The name of the attachment (Fixed size of ATTACH_FILENAME_MAX).
	 * @note The name is used to identify the attachment.
	*/
	char _mail_attachment_name[ATTACH_FILENAME_MAX];

	/*
	 * @brief The size of the attachment.
	 * @note The attachment is a file, and it's size is controlled by the _mail_attachment_size field.
	*/
	uint32_t _mail_attachment_size;
} MailAttachmentData, *PMailAttachmentData;


/********************************/
/* Function Declaratios Section */
/********************************/

/*
 * @brief Adds an attachment to the attachment list (a linked list of attachments).
 * @param head The head of the attachment list.
 * @param name The name of the attachment (the file name itself).
 * @param buffer The data of the attachment.
 * @param size The size of the attachment.
 * @return 0 on success, 1 on failure.
 * @note The list is a linked list of attachments, and it can be NULL if there are no attachments.
*/
int addAttachmentToList(PAttachment *head, const char *name, void *buffer, const uint32_t size);

/*
 * @brief Frees the memory allocated for the attachment list (a linked list of attachments).
 * @param head The head of the attachment list.
 * @note This function frees the memory allocated for the attachment list, including the memory allocated for the attachments themselves.
*/
void freeAttachmentList(PAttachment *head);

/*
 * @brief Creates a raw mail package from given mail data.
 * @param from The sender of the mail.
 * @param subject The subject of the mail.
 * @param body The body of the mail.
 * @param attachments A linked list of attachments of the mail.
 * @param output A pointer to the output buffer.
 * @return Size of the output buffer on success, -1 on failure.
*/
uint32_t createMailDataPacket(const char *from, const char *subject, const char *body, PAttachment attachments, char **output);

#if DEBUG_MESSAGES == 1
	/*
	 * @brief Prints a hex dump of a given buffer.
	 * @param desc The description of the buffer.
	 * @param addr The address of the buffer.
	 * @param len The length of the buffer.
	 * @param perLine The number of bytes to print per line.
	 * @return 0 on success, 1 on failure.
	*/
	int hexDump(const char *desc, const void *addr, const int len, int perLine);
#endif

#endif /* _MAIL_H */