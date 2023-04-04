CC = gcc
CFLAGS = -Wall -Wextra -lm
all: main

OBJ = basics.o crypte.o main.o

main: $(OBJ)
	$(CC) $(OBJ) -lm -o main_crypte


.PHONY: clean

clean:
	${RM} main_crypte crypte.o basics.o main.o
