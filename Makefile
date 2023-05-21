# Makefile

CC = gcc 
CFLAGS = -Wall -Wextra -g # `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0`
CPPFLAGS =  # `pkg-config --cflags sdl2 SDL2_image SDL2_t    tf gtk+-3.0`
LDLIBS = # -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0`
LDFLAGS = -fsanitize=address

interfaceobj =  traitement/k_moyen.o interfacePres/interface.o reseau_interface/client.o reseau_interface/list.o

traitementobj = k-moyenne/k_moyen.o

crypteobj = chiffrement/basics.o chiffrement/crypte.o chiffrement/main.o

serverobj = reseau_interface/server.o reseau_interface/shared_queue.o reseau_interface/queue.o reseau_interface/crypte.o reseau_interface/basics_chiffrement.o reseau_interface/list_all_data.o reseau_interface/basics.o

OBJ = $(traitementobj) $(crypteobj) $(interfaceobj) $(serverobj) 


EXE = k-moyen crypte main server

D = traitement/*.d reseau_interface/*.d interfacePres/*.d

all: interface server

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
interface: CFLAGS +=  -O3
interface: $(interfaceobj)
	$(CC) $(interfaceobj) $(LDLIBS) -o main
interfacePres/interface.o: interfacePres/interface.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o interfacePres/interface.o interfacePres/interface.c


server: LDFLAGS += -MMD
server: LDLIBS += -pthread -lm
server: CFLAGS += -std=c99
server: $(serverobj) 
	$(CC) $(serverobj) $(LDLIBS) -o server
reseau_interface/server.o: reseau_interface/server.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o reseau_interface/server.o reseau_interface/server.c
reseau_interface/shared_queue.o: reseau_interface/shared_queue.c reseau_interface/shared_queue.h reseau_interface/queue.o
reseau_interface/queue.o: reseau_interface/queue.c reseau_interface/queue.h

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) $(D)
	$(RM) temp_files/*.bmp
	$(RM) *.png

#END
