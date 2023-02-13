# Makefile

CC = gcc 
CFLAGS = -Wall -Wextra -O3 # `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0`
CPPFLAGS =
LDLIBS = # -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0`
LDFLAGS=

traitementobj = traitement/blur.o traitement/binarization.o traitement/dilatation_and_erosion.o traitement/grayscale.o traitement/sobel.o traitement/rescale.o traitement/fill.o traitement/main.o

OBJ = $(traitementobj)


EXE = main_traitement

FOLDER = rmdir -rf temp_files/

all:traitement

temp_files/info.txt:
	mkdir temp_files
	touch temp_files/info.txt
tempfiles: temp_files/info.txt
	touch temp_files/info.txt

traitement: CFLAGS += `pkg-config --cflags sdl2 SDL2_image`
traitement: LDLIBS += -lm `pkg-config --libs sdl2 SDL2_image`
traitement: $(traitementobj) tempfiles
	$(CC) $(traitementobj) $(LDLIBS) -o main_traitement
traitement/main.o: traitement/main.c
	$(CC) $(CFLAGS) -c -o traitement/main.o traitement/main.c

clean:
	$(RM) $(FOLDER)
	$(RM) $(OBJ)
	$(RM) $(EXE)
	
#END
