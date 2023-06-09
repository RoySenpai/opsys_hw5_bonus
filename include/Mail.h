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
	 * @brief The data of the attachment.
	 * @note The data is the actual file data, and we represent it as a stream of bytes.
	*/
	char *_attach_data;

	/*
	 * @brief The size of the attachment.
	 * @note The size is used for memory allocation purposes.
	*/
	uint32_t _attach_data_size;

	/*
	 * @brief The next attachment in the list.
	 * @note The next attachment is used to create a linked list of attachments.
	*/
	struct _Mail_Attachment_Node *_attach_next;
} Attachment, *PAttachment;

/*
 * @brief A struct representing a mail package - a mail package is a mail to be sent to the server or received from the server.

 * @param body The body of the mail.
 * @param body_size The size of the body of the mail.
 * @param attachments The list of attachments of the mail.
 * @note The list is a linked list of attachments, and it can be NULL if there are no attachments.
 * @note Both the subject and the body are strings of characters, and they can be empty.
 * @note This struct is used to store the entire mail data itself,
 * 			and this is the data that is sent to the server and vice versa.
 * @note When the mail is sent to the server it first encodes, compresses and encrypts the whole struct and it's data,
 * 				and then it sends the encoded, compressed and encrypted data to the server.
 * @note When the mail is received from the server it first decrypts, decompresses and decodes the data,
 * 				and then it stores the decoded, decompressed and decrypted data in this struct.
*/
typedef struct _Mail_Data_Package_Data_Node {
	/*
	 * @brief The body of the mail.
	 * @note The body is a string of characters, and it's size is controlled by the body_size field.
	*/
	char *_mail_data_body;

	/*
	 * @brief The size of the body of the mail.
	 * @note The size is used for memory allocation purposes.
	*/
	uint32_t _mail_data_body_size;

	/*
	 * @brief The list of attachments of the mail.
	 * @note The list is a linked list of attachments.
	 * @note The head is the first attachment in the list,
	 * 			and it can be NULL if there are no attachments.
	*/
	PAttachment _mail_data_attachments;
} MailData, *PMailData;

/*
 * @brief A struct representing a mail package node - a mail package node is a mail package with a sender.
 * @param _from The sender of the mail.
 * @param subject The subject of the mail.
 * @param _mail_data The mail itself.
 * @param _mail_next The next mail in the list.
 * @note The next mail is used to create a linked list of mails, and is used by the server to store
 * 			the mails and by the client to store retrieved mails.
 * @note In the server, the _mail_data field is encrypted, compressed and encoded,
 * 			and can be decrypted, decompressed and decoded only by the client.
*/
typedef struct _Mail_Data_Package_Node {

	/*
	 * @brief The sender of the mail.
	 * @note The sender is a string of characters, and it's size is controlled by the USERNAME_MAX_LENGTH macro.
	*/
	char _mail_from[USERNAME_MAX_LENGTH];

	/*
	 * @brief The subject of the mail.
	 * @note The subject is a string of characters, and can be empty.
	 * @note The size of the subject is controlled by the MAX_SUBJECT_LENGTH macro.
	*/
	char _mail_subject[MAX_SUBJECT_LENGTH];

	/*
	 * @brief The mail itself.
	 * @note The mail is a mail package, and it is represented by the MailData struct.
	*/
	PMailData _mail_data;

	/*
	 * @brief The next mail in the list.
	 * @note The next mail is used to create a linked list of mails.
	*/
	struct _Mail_Data_Package_Node *_mail_next;
} Mail, *PMail;


/********************************/
/* Function Declaratios Section */
/********************************/

/*
 * @brief Creates a new mail package.
 * @param body The body of the mail.
 * @param body_size The size of the body of the mail.
 * @param attachments The list of attachments of the mail.
 * @return A pointer to the new mail package, or NULL on failure.
 * @note The list is a linked list of attachments, and it can be NULL if there are no attachments.
*/
PMailData createMail(const char *body, const uint32_t body_size, PAttachment attachments);

/*
 * @brief Adds a mail to the mail list (a linked list of mails).
 * @param head The head of the mail list.
 * @param from The sender of the mail.
 * @param subject The subject of the mail.
 * @param body The body of the mail.
 * @param body_size The size of the body of the mail.
 * @param attachments The list of attachments of the mail.
 * @return 0 on success, 1 on failure.
 * @note The list is a linked list of mails, and it can be NULL if there are no mails.
*/
int addMailToList(PMail *head, const char *from, const char *subject, const char *body, const uint32_t body_size, PAttachment attachments);

/*
 * @brief Adds an attachment to the attachment list (a linked list of attachments).
 * @param head The head of the attachment list.
 * @param name The name of the attachment (the file name itself).
 * @param buffer The data of the attachment.
 * @param size The size of the attachment.
 * @return 0 on success, 1 on failure.
 * @note The list is a linked list of attachments, and it can be NULL if there are no attachments.
*/
int addAttachmentToList(PAttachment *head, const char *name, const char *buffer, const uint32_t size);

/*
 * @brief Frees the memory allocated for the mail list (a linked list of mails).
 * @param head The head of the mail list.
 * @note This function frees the memory allocated for the mail list, including the memory allocated for the mails themselves.
*/
void freeMailList(PMail *head);

/*
 * @brief Frees the memory allocated for the attachment list (a linked list of attachments).
 * @param head The head of the attachment list.
 * @note This function frees the memory allocated for the attachment list, including the memory allocated for the attachments themselves.
*/
void freeAttachmentList(PAttachment *head);


#endif /* _MAIL_H */