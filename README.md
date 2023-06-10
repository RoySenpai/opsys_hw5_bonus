# Operation Systems (OSs) Course Assignment 5 (Bonus)

### For Computer Science B.S.c Ariel University

**By Roy Simanovich and Linor Ronen**

## Description
This bonus assignment is a mail server and client that uses OpenSSL crypto library to encrypt and decrypt the messages,
Bzip2 Compression library to compress and decompress the messages, and UUEncode to encode and decode the messages. The mail
server and client are using the vSMTP (Very Simple Mail Transfer Protocol) to exchange messages. The protocol supports adding
attachments to the messages. Each message is encrypted, compressed, and encoded before it is sent to the server. The server only
saves the messages in the encrypted format. The client can download the messages from the server and decrypt, decompress, and decode,
and all the attachments will be saved in the client's machine in a unique folder.

The mail server and client are using TCP/IP protocol to communicate with each other.

This bonus assignment is composed from the following components:
* **Mail Server** – A mail server that is using the vSMTP protocol to communicate with the client.
* **Mail Client** – A mail client that is using the vSMTP protocol to communicate with the server. The client can send messages
to the server, and download messages from the server. The client uses the active object to encode/decode, compress/decompress,
and encrypt/decrypt the messages. The client saves the messages in a unique folder in the client's machine.

### Deep Dive
This assignment is composed from the following APIs:
* **Task API** – A struct that represents a task, to be executed by an active object.
* **Linked List API** – A linked list that is used to store the mail messages.
* **Blocking Queue API** – A blocking queue that is used to store tasks (or any other data). The queue is thread-safe and is blocking
when it is empty.
* **Active Object API** – Using the Active Object design pattern, the active object is a thread that is running in the background and
executes tasks from the blocking queue.
* **Encoding API** – A class that is used to encode and decode messages. Based on the UUEncode algorithm and Clem Dye's implementation.
* **Compression API** – A class that is used to compress and decompress messages. Based on the Bzip2 Compression algorithm and Julian Seward's implementation.
* **Encryption API** – A class that is used to encrypt and decrypt messages. Based on the OpenSSL crypto library, and uses the AES-256-CBC algorithm.
* **Message API** – A class that represents a message. The message is composed from a header and a body. The header contains the sender's
address, the subject, the size of the mail, and the number of attachments. The body contains the message's content. The mail can also contain
attachments. The message is encrypted, compressed, and encoded before it is sent to the server, and decrypted, decompressed, and decoded
when it is downloaded from the server.

#### Task API
The Task API supports the following operations:
* `PTask createTask(void *data, u_int32_t size)` – Creates a new task with the given data (data and size), allocates memory for it and returns a pointer to the task.
* `void taskDestroy(PTask task)` – Destroys a task - frees the memory allocated for the task.

The Task struct has the following fields:
* `void *_data` – The data to pass to the active object. The data is generic purpose, and can be used for any purpose by the active object.
* `u_int32_t _data_size` – The size of the data to pass to the active object. This indicates the size of the data to pass to the active object,
and is used for memory allocation purposes.

#### Linked List API
The Linked List API supports the following operations:
* `PLinkedList createLinkedList()` – Creates a new linked list and returns a pointer to the list.
* `int addNode(PLinkedList list, void *data)` – Adds a new node to the end of the list.
* `int removeNode(PLinkedList list, void *data)` – Removes a node from the list.
* `void *getHead(PLinkedList list)` – Returns the data stored in the head of the list.
* `void *getTail(PLinkedList list)` – Returns the data stored in the tail of the list.
* `int destroyLinkedList(PLinkedList list)` – Destroys a linked list.

The LinkedList struct has the following fields:
* `PNode head` – A pointer to the head of the list. The head pointer is NULL if the list is empty. The head pointer should not be changed directly.
* `PNode tail` – A pointer to the tail of the list. The tail pointer is NULL if the list is empty. The tail pointer should not be changed directly.

The Node struct has the following fields:
* `void *data` – A pointer to the data stored in the node. The data pointer is void, so it can be used to store any type of data. The data pointer cannot be NULL.
* `PNode next` – A pointer to the next node in the list. The next pointer is NULL if the node is the last node in the list. Users should not change the next pointer directly.


#### Queue API
The Queue API, which is thread safe, supports the following operations:
* `PQueue queueCreate()` – Creates a new queue and returns a pointer to the queue.
* `void queueDestroy(PQueue queue)` – Destroys a queue - destroys all the nodes in the queue and frees the memory allocated for the queue.
* `void queueEnqueue(PQueue queue, void *data)` – Adds an item to the queue (the item is a generic pointer that can point to any type of data).
* `void *queueDequeue(PQueue queue)` – Removes an item from the queue and returns it (the item is a generic pointer that can point to any type of data).
* `int queueIsEmpty(PQueue queue)` – Checks if the queue is empty (returns 1 if the queue is empty, 0 otherwise).
* `int queueSize(PQueue queue)` – Returns the size of the queue (number of items in the queue). Defined only if ```DEBUG_MESSAGES``` is set to 1.
* `void *queuePeek(PQueue queue)` – A debug function that returns the head of the queue, without removing it from the queue and without changing the queue. Defined only if ```DEBUG_MESSAGES``` is set to 1.
* `void *queuePeekTail(PQueue queue)` – A debug function that returns the tail of the queue, without removing it from the queue and without changing the queue. Defined only if ```DEBUG_MESSAGES``` is set to 1.
* `void queuePrint(PQueue queue)` – A debug function that prints the queue. Defined only if ```DEBUG_MESSAGES``` is set to 1.

The Queue struct is defined as follows:
* `PQueueNode head` – A pointer to the head of the queue. The head is the first item that was added to the queue, and this field is used to remove items from the queue efficiently in O(1) time.
* `PQueueNode tail` – A pointer to the tail of the queue. The tail is the last item that was added to the queue, and this field is used to add items to the queue efficiently in O(1) time.
* `unsigned int size` – The size of the queue (number of items in the queue). This field is used to check if the queue is empty, and to get the size of the queue in O(1) time.
* `pthread_mutex_t lock` – A mutex lock that is used to lock the queue when it's being used by a thread. This field is used to make the queue thread safe.
* `pthread_cond_t cond` – A condition variable that is used to signal threads that are waiting for the queue to be unlocked. This field is used to make the queue a blocking queue.

Each queue node has the following fields:
* `void *data` – The data of the node. This is a generic pointer that can point to any type of data.
* `PQueueNode next` – A pointer to the next node in the queue. This field defines the node structure, as a singly linked list.

**_NOTE_:** The queue API is generic, and can be used to create a queue of any type of data. In this assignment, the queue is used to create a queue of tasks. Please note
that the queue API is not type safe, and the user must make sure that the data that is added to the queue is of the correct type.

**_NOTE_:** It is recommended to use the macros defined in the ```Queue.h``` file to enqueue and dequeue items from the queue. The macros are defined as follows:
* `ENQUEUE(queue, data)` – Enqueues an item to the queue. The item is a generic pointer that can point to any type of data.
* `DEQUEUE(queue, type)` – Dequeues an item from the queue and returns it with the correct type. The type is the type of the data that was added to the queue.

**_NOTE_:** For debugging purposes, the user can set the ```DEBUG_MESSAGES``` macro to 1 in the ```Task.h``` file. This will enable the user to use the `queuePeek` and `queuePeekTail` functions to
peek at the head and tail of the queue, without removing them from the queue. This is useful for debugging purposes, as it allows the user to see the queue contents without
changing the queue, while allowing the queue to be thread safe. This also enables the macros for the functions. The macros are defined as follows:
* `PEEK(queue, type)` – Peeks at the head of the queue and returns it with the correct type. The type is the type of the data that was added to the queue.
* `PEEK_TAIL(queue, type)` – Peeks at the tail of the queue and returns it with the correct type. The type is the type of the data that was added to the queue.

**_NOTE_:** Please note that direct access to the queue struct fields is **not allowed**, and the user must use the Queue API to do any operation on the queue,
as the functions that enqueue and dequeue items from the queue are what make the queue thread safe in the first place.

#### Active Object API
The Active Object API supports the following operations:
* `PActiveObject CreateActiveObject(PQueueFunc func)` – Creates a new active object, a new queue and starts the active object thread. Returns a pointer to the active object if
the operation was successful, or NULL if an error occurred.
* `PQueue getQueue(PActiveObject activeObject)` – Returns the queue of the active object, or NULL if an error occurred.
* `void stopActiveObject(PActiveObject activeObject)` – Stops the active object thread, destroys the queue and frees the memory allocated for the active object.
* `void *activeObjectRunFunction(void *activeObject)` – The function that is executed by the active object thread. This function is responsible for dequeuing tasks from the queue and executing them,
not to be called by the user directly.

The Active Object struct has the following fields:
* `unsigned int id` – The id of the active object, for debugging purposes.
* `pthread_t thread` – The thread of the active object. Users can use this field to join the thread, but not to cancel it. To cancel the thread, use the stopActiveObject function.
* `PQueue queue` – The queue of the active object, which holds the tasks that the active object will execute.
* `PQueueFunc func` – A function pointer to the handler function of the active object, which will be executed by the active object thread anytime there is a task in the queue.

Each active object receives a function pointer to a function that will be executed by the active object thread. The signature of the handler
function is: ```int handler_function(void *task)```, where task is the data that was dequeued from the queue. The handler function must
return 1 if the active object should continue running, or 0 if the active object should stop running. The handler function is responsible for
freeing the memory allocated for the task, and for handling any errors that may occur during the execution of the task.


#### Encoding API
The Encoding API supports the following operations:
* `int UUEncode(const char *data, const uint32_t data_size, char **encoded_data, uint32_t *encoded_data_size)` – Encodes the data using the UUEncode algorithm.
Returns 1 if the operation was successful, or 0 if an error occurred.
* `int UUDecode(const char *encoded_data, const uint32_t encoded_data_size, char **data, uint32_t *data_size)` – Decodes the data using the UUEncode algorithm.
Returns 1 if the operation was successful, or 0 if an error occurred.

The Encoding API uses the UUEncode algorithm, which is an open-source algorithm for encoding and decoding data.
The algorithm is available at: http://www.fourmilab.ch/webtools/base64/


#### Compression API
The Compression API supports the following operations:
* `int Bzip2_Compress(void* data, uint32_t size, void** compressed_data, uint32_t* compressed_size)` – Compresses the data using the Bzip2 algorithm.
Returns 1 if the operation was successful, or 0 if an error occurred.
* `int Bzip2_Decompress(void* compressed_data, uint32_t compressed_size, void** data, uint32_t* size)` – Decompresses the data using the Bzip2 algorithm.
Returns 1 if the operation was successful, or 0 if an error occurred.

The Compression API uses the Bzip2 library, which is an open-source library for compressing and decompressing data.
The library is available at: http://www.bzip.org/

The compression and decompression functions are based on the example code provided by the library. The example code is available at: http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz
The API also have some settings that can be changed in the ```Compression.h``` file:
* `BZIP2_BLOCK_SIZE` – Specifies the block size to be used for compression. It should be a value between 1 and 9 inclusive, and the actual block size used is 100000 x this figure.
9 gives the best compression but takes most memory. 1 gives the worst compression but uses least memory. The default value is 3.
* `BZIP2_VERBOSITY` – Specifies the verbosity level to be used for compression. It should be a value between 0 and 4 inclusive. 0 gives no output, 4 gives maximum output.
The default value is 0.
* `BZIP2_WORK_FACTOR` – Specifies the work factor to be used for compression. It should be a value between 0 and 250 inclusive. The default value is 30.
* `BZIP2_DECOMPRESS_SMALL` – Specifies whether to use the small memory decompression mode or not. It should be 1 to use the small memory decompression mode, or 0 to use the normal mode.
The default value is 0.


#### Encryption API
The Encryption API supports the following operations:
* `int AES_func_encrypt_data(uint8_t *plaintext, int plaintext_len, const uint8_t *key, const uint8_t *iv, uint8_t *ciphertext)` – Encrypts the data using the AES-256-CBC algorithm.
Returns 1 if the operation was successful, or 0 if an error occurred.
* `int AES_func_decrypt_data(uint8_t *ciphertext, int ciphertext_len, const uint8_t *key, const uint8_t *iv, uint8_t *data)` – Decrypts the data using the AES-256-CBC algorithm.
Returns 1 if the operation was successful, or 0 if an error occurred.

The Encryption API uses the OpenSSL crypto library, which is an open-source library for encryption and decryption. It is available at: https://www.openssl.org/

The key and the initialization vector (IV) that are used for encryption and decryption can be changed in the ```Encryption.h``` file:
* `EVP_AES_KEY` – The key that is used for encryption and decryption. The key is a 256-bit key, and it is represented as a 32-byte array.
The default value is `01234567890123456789012345678901`.
* `EVP_AES_IV` – The initialization vector (IV) that is used for encryption and decryption. The IV is a 128-bit IV, and it is represented as a 16-byte array.
The default value is `0123456789012345`.


#### Message API
The Message API supports the following operations:
* `int addAttachmentToList(PAttachment *head, const char *name, void *buffer, const uint32_t size)` – Adds an attachment to the attachments list.
Returns 1 if the operation was successful, or 0 if an error occurred.
* `void freeAttachmentList(PAttachment *head)` – Frees the attachments list by removing all the attachments from the list and freeing the memory that was allocated for them.
* `uint32_t createMailDataPacket(const char *from, const char *subject, const char *body, PAttachment attachments, char **output)` – Creates a mail data packet from the given parameters.
The whole mail data package is builded directly in the output buffer, and the function returns the size of the mail data package.

The message header is built from the following fields:
* `char _mail_from[USERNAME_MAX_LENGTH]` (USERNAME_MAX_LENGTH bytes) – The sender of the mail (Fixed size of USERNAME_MAX_LENGTH). The sender is used to identify the sender of the mail.
* `char _mail_subject[MAX_SUBJECT_LENGTH]` (MAX_SUBJECT_LENGTH bytes) – The subject of the mail (Fixed size of MAX_SUBJECT_LENGTH). The subject is used to identify the subject of the mail. It can be empty.
* `uint32_t _mail_data_total_size` (4 bytes) – The total size of the mail, including all the fields and the data. The _mail_data_total_size field is calculated by this formula:
_mail_data_total_size = sizeof(struct _Mail_Raw_Packet_Stripped) + _mail_data_body_size + sum of the sizes of the attachments (if there are any).
* `uint32_t _mail_data_body_size` (4 bytes) – The size of the mail body (excluding the attachments). The _mail_data_body_size field is calculated by this formula:
_mail_data_body_size = strlen(_mail_body) + 1.
* `uint32_t _mail_attachments_count` (4 bytes) – The number of attachments that are attached to the mail.
* `<body data>` (0-? bytes) – The body of the mail. The body is a string, and it's size is controlled by the _mail_data_body_size field. Can be empty.
* `<attachments>` (0-? bytes) – The attachments of the mail. The attachments are files, and their sizes are controlled by the _mail_attachment_size field. See the attachment header for more details.

The attachment header is built from the following fields:
* `char _mail_attachment_name[ATTACH_FILENAME_MAX]` (ATTACH_FILENAME_MAX bytes) – The name of the attachment (Fixed size of ATTACH_FILENAME_MAX). The name is used to identify the attachment.
* `uint32_t _mail_attachment_size` (4 bytes) – The size of the attachment. The attachment is a file, and it's size is controlled by the _mail_attachment_size field.
* `<attachment data>` (0-? bytes) – The data of the attachment. The data is a file, and it's size is controlled by the _mail_attachment_size field. Can be empty,
but it's ilogical to have an attachment with no data.

_**NOTICE**_: The mail server and client are using the vSMTP protocol to communicate with each other. The protocol is based on the SMTP protocol,
but it is a very simple version of it. The protocol is not a standard protocol, and it is not recommended to use it in real life.

_**NOTICE**_: The implementation of the mail server and client isn't complete, because of time limitations. The purpose of this submission
is to show the ability to understand and use OpenSSL crypto library, Bzip2 Compression library, and UUEncode algorithm.

_**TODO**_: Complete the implementation of the mail server and client, such that the chain of encryption, compression, and encoding will function
properly, and vice versa.


## Requirements
* Linux machine (Ubuntu 22.04 LTS preferable)
* GNU C Compiler
* OpenSSL crypto library
* Bzip2 Compression library
* Make

## Building
```
# Cloning the repo to local machine.
git clone https://github.com/RoySenpai/opsys_hw5_bonus.git

# Install OpenSSL crypto library
sudo apt install libssl-dev

# Bzip2 Compression library
sudo apt install libzip-dev

# Building all the necessary files & the main programs.
make all
```

## Running
```
# Run the mail server
./mail_server

# Run the mail client
./mail_client
```