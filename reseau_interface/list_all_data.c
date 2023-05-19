#include <err.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "list_all_data.h"

all_data* init_data_client (void)
{
	struct all_data* all_data = malloc ( sizeof (struct all_data));
	if ( all_data == NULL) 
		errx ( EXIT_FAILURE, "malloc () in init_data_client(1)");


	sem_init (&all_data->lock, 0 , 1);

	struct data_client* data_client = malloc (sizeof (struct data_client));
	if (data_client == NULL)
		errx ( EXIT_FAILURE, "malloc () in init_data_client (2)");

	data_client->next = NULL;

	all_data->list_data_client = data_client;

	return all_data;
}

void delete_message_data ( data_message* data_message, all_data* all_data)
{
	sem_wait ( &all_data->lock);
	if ( data_message ==NULL)
	{
		sem_post (&all_data->lock);
		return;
	}
	

	while ( data_message->next != NULL)
	{
		struct data_message* temp = data_message->next;

		free ( data_message->message);
		free ( data_message);

		data_message = temp;
		
	}
	free ( data_message->message);
        free ( data_message);
	sem_post (&all_data->lock);


}


void push_client ( all_data* all_data, char * name, int cfd, Public_Private* key )
{
	sem_wait ( &all_data->lock);

	//innitialisation
	struct data_client* new_data_client = malloc (sizeof (struct data_client));
	if (new_data_client == NULL )
		errx ( EXIT_FAILURE, "malloc () in push_client (1)");

	new_data_client->key = key;
	new_data_client->name = name;
	new_data_client->cfd = cfd;

	struct data_message* sent_message = malloc (sizeof (struct data_message));
	if (sent_message == NULL)
		errx ( EXIT_FAILURE, "malloc () in push_client (2)");

	sent_message->next = NULL;

	new_data_client->message = sent_message;

	struct data_client* data_client = all_data->list_data_client; // first element
								      //push 
	new_data_client->next = data_client->next;
	data_client->next = new_data_client;

	sem_post (&all_data->lock);

}

void pop_client ( all_data* all_data, int cfd )
{

	sem_wait ( &all_data->lock);

	struct data_client* data_client = all_data->list_data_client->next;
	struct data_client* temp_client = all_data->list_data_client;

	if ( data_client == NULL)
	{
		sem_post (&all_data->lock);
		return;
	}

	for ( ; data_client->next != NULL; data_client = data_client->next)
	{
		if ( data_client->cfd == cfd )
		{
			//delete data client

			temp_client->next = data_client->next;

			//free the struct 

			free ( data_client->key);
			free ( data_client->message);
			//delete_message_data (data_client->message, all_data);

			sem_post (&all_data->lock);
			return;
		}
		temp_client = temp_client->next;
	}

	if ( data_client->cfd == cfd )
	{
		//delete data client

		temp_client->next = data_client->next;

		//free the struct 

		free ( data_client->key);
		free ( data_client->message);
//		delete_message_data (data_client->message, all_data);

		sem_post (&all_data->lock);
		return;
	}


	sem_post (&all_data->lock);
	printf ("pb because this cfd don't exist");
}



data_client* search_client (all_data* all_data, char* name)
{

	sem_wait ( &all_data->lock);
	struct data_client* data_client = all_data->list_data_client->next;
	if (data_client == NULL)
		errx(EXIT_FAILURE, "error no people in struct list");

	for ( ; data_client->next != NULL ; data_client = data_client->next)
	{
		if ( strcmp ( name , data_client->name) == 0)
		{
			sem_post (&all_data->lock);
			return data_client;
		}
	}

	if ( strcmp ( name , data_client->name) == 0)
	{
		sem_post (&all_data->lock);
		return data_client;
	}
	sem_post (&all_data->lock);
	return NULL;

}


data_message* check_my_message ( all_data* all_data, char* name)
{
	// find the cliend 
	struct data_client* data_client = search_client ( all_data , name);


	sem_wait ( &all_data->lock);
	if (data_client == NULL)
		errx (EXIT_FAILURE, "Error list or name or push");

	// first message
	struct data_message* all_message = data_client->message;
	if (all_message->next == NULL)
	{
		sem_post (&all_data->lock);
		return NULL;	
	}
	else
	{

		struct data_message* stock = all_message->next;

		//join
		all_message->next = stock->next;

		// not free because you must read after the message
		sem_post (&all_data->lock);
		return stock;
	}

}

void push_message ( all_data* all_data, char* name, unsigned long* message, size_t size, char * sender)
{
	//init new strut for the new message
	struct data_message* new_data_message = malloc ( sizeof ( struct data_message));
	new_data_message->size = size;
	new_data_message->message = message;
	new_data_message->sender = sender;
	new_data_message->next = NULL;

	//find the client
	struct data_client* data_client = search_client ( all_data , name);

	sem_wait ( &all_data->lock);
	//get sentinel of data_message
	struct data_message* sent_data_message = data_client->message;

	//psuh message in list
	new_data_message->next = sent_data_message->next;
	sent_data_message->next = new_data_message;

	sem_post (&all_data->lock);
}


