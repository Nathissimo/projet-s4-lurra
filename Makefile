# Makefile

CC = gcc 
CFLAGS = -Wall -Werror -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0` -g
CPPFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0`
LDFLAGS= 

OBJ = traitement/blur.o traitement/binarization.o traitement/dilatation_and_erosion.o traitement/grayscale.o traitement/sobel.o traitement/rescale.o traitement/main.o

all:traitement

traitement: $(OBJ) 
	$(CC) $(OBJ) $(LDLIBS) -o main_traitement
traitement/main.o: traitement/main.c
	$(CC) $(CFLAGS) -c -o traitement/main.o traitement/main.c

clean:
	$(RM) main_traitement
	$(RM) temp_files/*
	$(RM) $(OBJ)
	
#END
