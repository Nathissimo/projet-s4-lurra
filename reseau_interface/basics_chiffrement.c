#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/random.h>
#include <time.h>

#include "basics_chiffrement.h"

size_t const TRUE = 1;
size_t const FALSE =0;



size_t get_Random ()
{
	int prime;
	if (getrandom (&prime, sizeof (int), GRND_NONBLOCK) == -1)
                    perror ("getramdom");
	
	if ( prime <0 )
		prime = prime * -1;
	size_t res = prime % 500;
	
	srand (time ( NULL));
	while ( is_prime_number(res) == FALSE )
	{
		if (getrandom (&prime, sizeof (int), GRND_NONBLOCK) == -1)
			perror ("getramdom");
		if (prime <0)
			prime = prime * -1;
		res = prime % 500;
	}
	return res;
}





size_t is_prime_number ( size_t nb)
{
	if ( nb <2 )
		return FALSE;

	double square = sqrt((double)nb);
	if ( nb % 2 == 0)
		return FALSE;

	for ( int i = 3; (double) i<=square ; i+=2)
	{
		if ( nb % i == 0)
			return FALSE;
	}
	return TRUE;
}


size_t pgcd ( size_t a , size_t b)
{
	if ( a == b )
		return a;
	else
	{
		if ( a< b)
			return pgcd ( a, b-a);
		else
			return pgcd (a-b, b);
	}
}


unsigned long Pow_and_Mod ( unsigned long long number,size_t exp , size_t m )
{
	unsigned long res = 1;
	
	while ( exp > 0)
	{
		if ( (exp & 1) > 0) 
			res = (res * number) % m;
		exp >>=1;
		number = (number * number) % m;
	}
	return res;

}






