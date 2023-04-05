CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDLIBS = -pthread -lm  #-fsanitize=address 
LDFLAGS = -MMD -fsanitize=address

all: server client

#test_shared_queue: test_shared_queue.o shared_queue.o queue.o
server: server.o shared_queue.o queue.o ../chiffrement/crypte.o ../chiffrement/basics.o list_name.o list_cfd.o list_message.o list_key.o
shared_queue.o: shared_queue.c shared_queue.h queue.o
queue.o: queue.c queue.h

client: client.o

.PHONY: clean

clean:
	${RM} server server.o
	${RM} shared_queue.o queue.o ../chiffrement/crypte.o ../chiffrement/basics.o list_name.o list_cfd.o list_key.o list_message.o
	${RM} client client.o
