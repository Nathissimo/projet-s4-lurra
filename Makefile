CC=gcc # explicitly set the c compiler
CFLAGS= -Wall -Werror -Wextra -g
CPPFLAGS= # c pre-processor flags (ex: -D_GNU_SOURCE)
LDFLAGS= # linker flags
LDLIBS= 

OBJ = traitment/main.o

all:traitement
main_traitement: OBJ =
main_traitement: traitement/main.o $(OBJ)
	 $(CC) $< $(OBJ) -o main_traitement

clean:
	$(RM) main_traitement
	$(RM) $(OBJ)
