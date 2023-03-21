CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -pthread -fsanitize=address -lm

all: server

#test_shared_queue: test_shared_queue.o shared_queue.o queue.o
server: server.o shared_queue.o queue.o ../chiffrement/crypte.o ../chiffrement/basics.o list_name.o list_cfd.o list_key.o
shared_queue.o: shared_queue.c shared_queue.h queue.o
queue.o: queue.c queue.h

.PHONY: clean

clean:
	${RM} server server.o
	${RM} shared_queue.o queue.o ../chiffrement/crypte.o ../chiffrement/basics.o list_name.o list_cfd.o list_key.o
