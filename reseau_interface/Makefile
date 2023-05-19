CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDLIBS = -pthread -lm  #-fsanitize=address 
LDFLAGS = -MMD -fsanitize=address

all: server #client

#test_shared_queue: test_shared_queue.o shared_queue.o queue.o
server: server.o shared_queue.o queue.o crypte.o basics_chiffrement.o list_all_data.o basics.o
shared_queue.o: shared_queue.c shared_queue.h queue.o
queue.o: queue.c queue.h

#client: client.o

.PHONY: clean

clean:
	${RM} server server.o
	${RM} shared_queue.o queue.o crypte.o basics_chiffrement.o list_all_data.o basics.o
#	${RM} client client.o
