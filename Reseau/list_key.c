#include <semaphore.h>
#include "list_key.h"
#include <stdlib.h>
#include <stddef.h>
#include <err.h>
#include <stdio.h>



struct_key* init_data_key ( void)
{
	struct struct_key* struct_key = malloc (sizeof (struct struct_key));
	if ( struct_key == NULL)
		errx (EXIT_FAILURE, "malloc()");
	
	sem_init(&struct_key->lock, 0 , 1);

	struct Key* key = malloc (sizeof (struct Key));
	key->next= NULL;

	struct_key->list_key = key;
	return struct_key;
}

void push_key (struct_key* struct_key, size_t nb1 , size_t nb2 )
{
	sem_wait(&struct_key->lock);
	
	struct Key* new_key = malloc (sizeof (struct Key));
	new_key->next = NULL;
	new_key->nb1 = nb1;
	new_key->nb2 = nb2;
	new_key->empty = 1;

	struct Key* key = struct_key->list_key->next;

	//if list is empty
	
	if ( key == NULL)
	{
		struct_key->list_key->next = new_key;
		sem_post( &struct_key->lock);
		return;
	}

	if ( key->empty == 0 )
	{
		key->nb1 = nb1;
		key->nb2 = nb2;
		key->empty = 1;
		free (new_key );
		sem_post( &struct_key->lock);
		return;
	}	

	for ( ; key->next != NULL ;key = key ->next)
	{
		if ( key->empty == 0 )
        	{
                	key->nb1 = nb1;
			key->nb2 = nb2;
                	key->empty = 1;
                	free (new_key );
			sem_post( &struct_key->lock);
                	return;
        	}

	}

	if ( key->empty == 0 )
        {
                key->nb1 = nb1;
                key->nb2 = nb2;
                key->empty = 1;
                free (new_key );
		sem_post( &struct_key->lock);
                return;
        }
	
	key->next = new_key;
	sem_post( &struct_key->lock);

}



void pop_key ( struct_key* struct_key , size_t index)
{
	sem_wait(&struct_key->lock);

	Key* key = struct_key->list_key->next;

	for ( size_t i = 0; i <index ; i++ )
	{
		key = key->next;
		if ( key == NULL )
			errx (EXIT_FAILURE, "error index is to big in struct Key");
	}
	key->empty = 0;

	sem_post (&struct_key->lock);

}



