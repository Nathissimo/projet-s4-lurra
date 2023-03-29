#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

#define BUFFER_SIZE 32

int main ( void)
{
	const char* command = "nc localhost 2048";
	int cr = system ( command);

	if ( cr != 0)
	{
		errx ( 1, "Commande is not valid");
	}
	    
}
