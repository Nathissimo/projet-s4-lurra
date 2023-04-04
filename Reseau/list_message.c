#include <err.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#include "list_message.h"

int same_name2 ( char* name1 , char* name2)
{
    if (name2 == NULL)
        return 0;
    size_t len1 = strlen ( name1);
    size_t len2 = strlen ( name2);

    if ( len1 != len2 )
    {
        return 0;
    }

    for ( size_t i = 0; i<len1 ; i++)
    {
        if ( name1[i] != name2[i])
            return 0;

    }
    return 1;


}


struct_message* init_data_message ()
{
    struct struct_message* struct_message = malloc (sizeof (struct struct_message));

    if (struct_message == NULL)
        errx (EXIT_FAILURE, "malloc()");

    memset ( struct_message, 0 , sizeof (struct struct_message));
    sem_init (&struct_message->lock,0,1);
	

    struct data_message* data_message = malloc (sizeof (struct data_message) );
	
	
    data_message->next = NULL;
    struct_message->list_message = data_message;	
	
    return struct_message; 
}

void push_message (struct_message* struct_message, char* message, size_t size, char* sender, char* receiver)
{
    sem_wait (&struct_message->lock);

	//initialisation of new data_message
    struct data_message* new_data_message = malloc ( sizeof (struct data_message));
    new_data_message->size = size;
    new_data_message->sender = sender;
    new_data_message->receiver = receiver;
    new_data_message->message = &message;
	
    //join a new element in chain list

    struct data_message* data_message = struct_message->list_message;

    new_data_message->next = data_message->next;
    data_message->next = new_data_message;

    sem_post (&struct_message->lock);
}






char* check_and_return_message ( struct_message* struct_message, char* receiver, size_t* size )
{
	sem_wait (&struct_message->lock);
	int message = 0;
	struct data_message* data_message = struct_message->list_message->next;
	
	struct data_message* temp_data_message = struct_message->list_message;
	

	if ( data_message == NULL)	//no message
	{	
		sem_post (&struct_message->lock);
		return NULL;
	}

	for ( ; data_message ->next != NULL; data_message = data_message->next )
	{
		if ( same_name2 ( data_message->receiver, receiver ))
		{	
			message = 1;
			break;
		}
		temp_data_message = temp_data_message->next;
	}

	if ( same_name2 ( data_message->receiver, receiver))
		message = 1;


	if ( message == 0) //no message
	{
		sem_post (&struct_message->lock);
		return NULL;
	}
	else // I have a message
	{
		char** mes = data_message->message;
		*size = data_message->size;
		//free
		free ( *(data_message->message));
		free ( data_message->sender);
		free ( data_message->receiver);

		// delete struct data_message
		temp_data_message->next	= data_message->next;
		free ( data_message);
		sem_post (&struct_message->lock);
		return *mes;
	}

}








