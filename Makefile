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

# Flags for the compiler and linker.
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -pedantic -I$(SOURCE_PATH)
LIBS = -lbz2 -lz -lssl -lcrypto
PFLAGS = -pthread
RM = rm -f

SOURCE_PATH = sources
OBJECT_PATH = objects
INCLUDE_PATH = include
SOURCES = $(wildcard $(SOURCE_PATH)/*.c)
HEADERS = $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS = $(subst sources/,objects/,$(subst .c,.o,$(SOURCES)))
CLIENT_OBJECTS = Client.o Tasks.o Encoding.o Encryption.o Compression.o Mail.o ActiveObject.o Queue.o Task.o
SERVER_OBJECTS = Server.o

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
mail_client: $(OBJ_C)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(PFLAGS)

mail_server: $(OBJ_S)
	$(CC) $(CFLAGS) -o $@ $^

################
# Object files #
################
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
	
#################
# Cleanup files #
#################
clean:
	$(RM) $(OBJECTS) *.o *.so mail_client mail_server