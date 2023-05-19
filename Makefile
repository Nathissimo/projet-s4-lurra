# Makefile

CC = gcc 
CFLAGS = -Wall -Wextra -O3 -g # `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0`
CPPFLAGS =  # `pkg-config --cflags sdl2 SDL2_image SDL2_t    tf gtk+-3.0`
LDLIBS = # -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0`
LDFLAGS = -fsanitize=address

interfaceobj =  traitement/k_moyen.o interfacePres/interface.o reseau_interface/client.o

traitementobj = k-moyenne/k_moyen.o

crypteobj = chiffrement/basics.o chiffrement/crypte.o chiffrement/main.o

OBJ = $(traitementobj) $(crypteobj) $(interfaceobj)


EXE = k-moyen crypte main

all: traitement interface crypte

traitement: CPPFLAGS += `pkg-config --cflags sdl2 SDL2_image` -MMD
traitement: LDLIBS += `pkg-config --libs sdl2 SDL2_image` -lSDL_image -lm
traitement: $(traitementobj)
	$(CC) $(LDFLAGS) $(traitementobj) $(LDLIBS) -o k-moyen
k-moyenne/k_moyen.o : k-moyenne/k_moyen.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o k-moyenne/k_moyen.o k-moyenne/k_moyen.c



crypte: LDFLAGS += -Wall -Wextra -lm
crypte: LDLIBS += -lm
crypte:  $(crypteobj)
	$(CC) $(crypteobj) $(LDLIBS) -o crypte


interface: CPPFLAGS += `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -MMD
interface: LDLIBS += `pkg-config --libs sdl2 SDL2_image gtk+-3.0` -lSDL_image -lm -pthread
interface: $(interfaceobj)
	$(CC) $(interfaceobj) $(LDLIBS) -o main
interfacePres/interface.o: interfacePres/interface.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o interfacePres/interface.o interfacePres/interface.c

clean:
	$(RM) $(FOLDER)
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) *.bmp
	
	
#END
