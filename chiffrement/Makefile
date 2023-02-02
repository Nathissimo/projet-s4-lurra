CC = gcc
CFLAGS = -Wall -Wextra -lm
all: main

OBJ = basics.o crypte.o

main: $(OBJ)
	$(CC) $(OBJ) -lm -o crypte


.PHONY: clean

clean:
	${RM} crypte crypte.o basics.o
