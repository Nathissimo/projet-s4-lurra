#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <err.h>

#include "crypte.h"



int main ( int argc , char* argv[] )
{
	if ( argc != 2)
	{
		err (EXIT_FAILURE, "usage : message\n");
	}
	char* path = argv[1];
	fct_main ( path);

	return 1;
}
