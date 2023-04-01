# Makefile

CC = gcc 
CFLAGS = -Wall -Wextra -O3 -g # `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0`
CPPFLAGS =  # `pkg-config --cflags sdl2 SDL2_image SDL2_t    tf gtk+-3.0`
LDLIBS = # -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0`
LDFLAGS = -fsanitize=address

traitementobj = traitement/k_moyen.o

crypteobj = chiffrement/basics.o chiffrement/crypte.o

OBJ = $(traitementobj) $(crypteobj)


EXE = k-moyen crypte

all: traitement crypte

traitement: CPPFLAGS += `pkg-config --cflags sdl2 SDL2_image` -MMD
traitement: LDLIBS += `pkg-config --libs sdl2 SDL2_image` -lSDL_image -lm
traitement: $(traitementobj)
	$(CC) $(LDFLAGS)  $(traitementobj) $(LDLIBS) -o k-moyen
traitement/k_moyen.o : traitement/k_moyen.c
	$(CC) $(CPPFLAGS) $(CFLAGS)  -c -o traitement/k_moyen.o traitement/k_moyen.c



crypte: LDFLAGS += -Wall -Wextra -lm
crypte: LDLIBS += -lm
crypte:  $(crypteobj)
	$(CC) $(crypteobj) $(LDLIBS) -o crypte


clean:
	$(RM) $(FOLDER)
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) *.bmp
	
	
#END
