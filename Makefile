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

# Phony targets - targets that are not files but commands to be executed by make.
.PHONY: all default clean

# Default target - compile everything and create the executables and libraries.
all: mail_client mail_server

# Alias for the default target.
default: all


############
# Programs #
############
mail_client: $(OBJECT_PATH)/Client.o $(OBJECT_PATH)/Tasks.o $(OBJECT_PATH)/Encryption.o $(OBJECT_PATH)/Mail.o $(OBJECT_PATH)/ActiveObject.o $(OBJECT_PATH)/Queue.o $(OBJECT_PATH)/Task.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(PFLAGS)

mail_server: $(OBJECT_PATH)/Server.o
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