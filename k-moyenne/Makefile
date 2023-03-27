# Makefile
CC = gcc
CPPFLAGS = `pkg-config --cflags sdl2 SDL2_image` -MMD
CFLAGS = -Wall -Wextra -g
LDFLAGS = -fsanitize=address
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lSDL_image -lm

SRC = $(wildcard *.c)  k_moyen.o
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: k_moyen

interface: $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

.PHONY: clean

clean:
	${RM} k_moyen
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} ${DEP}
	${RM} *.bmp


# END

