#################################################################################
# 	Operation Systems (OSs) Course Assignment 5 Bonus Makefile		#
# 	Authors: Roy Simanovich and Linor Ronen (c) 2023			#
# 	Description: This Makefile compiles the programs and libraries 		#
# 				Date: 2023-06					#
# 			Course: Operating Systems				#
# 				Assignment: 5 (Bonus)				#
# 				Compiler: gcc					#
# 				OS: Linux					#
# 			IDE: Visual Studio Code					#
#################################################################################

# Use the gcc compiler.
CC = gcc

# Flags for the compiler.
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -pedantic -I$(SOURCE_PATH)

# Libraries to link.
LIBS = -lbz2 -lz -lssl -lcrypto

# Enable multithreading.
PFLAGS = -pthread

# Command to remove files.
RM = rm -f

# Constants for the source, object and include paths.
SOURCE_PATH = sources
OBJECT_PATH = objects
INCLUDE_PATH = include

# Variables for the source, object and header files.
SOURCES = $(wildcard $(SOURCE_PATH)/*.c)
HEADERS = $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS = $(subst sources/,objects/,$(subst .c,.o,$(SOURCES)))

# Variables for the client and server object files.
CLIENT_OBJECTS = Client.o Tasks.o Encoding.o Encryption.o Compression.o Mail.o ActiveObject.o Queue.o Task.o
SERVER_OBJECTS = Server.o

# Variables for the object files.
OBJ_C = $(addprefix $(OBJECT_PATH)/, $(CLIENT_OBJECTS))
OBJ_S = $(addprefix $(OBJECT_PATH)/, $(SERVER_OBJECTS))

# Phony targets - targets that are not files but commands to be executed by make.
.PHONY: all default clean

# Default target - compile everything and create the executables and libraries.
all: mail_client mail_server

# Alias for the default target.
default: all


############
# Programs #
############

# Compile the client program.
mail_client: $(OBJ_C)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(PFLAGS)

# Compile the server program.
mail_server: $(OBJ_S)
	$(CC) $(CFLAGS) -o $@ $^


################
# Object files #
################

# Compile all the C files that are in the source directory into object files that are in the object directory.
$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


#################
# Cleanup files #
#################

# Remove all the object files, shared libraries and executables.
clean:
	$(RM) $(OBJECTS) *.so mail_client mail_server