#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>


#include "list.h"


list_connect* init_list_connect (void)
{
	struct list_connect* list_connect = malloc ( sizeof ( struct list_connect));

	if (list_connect == NULL)
		err ( EXIT_FAILURE, "error malloc list_connect");

	list_connect->next = NULL;
	list_connect->name = NULL;
	list_connect->list_message = NULL;

	return list_connect;
}


void push_connect (const char * name , list_connect* list_connect)
{

	struct list_connect* list_connect_new = malloc ( sizeof ( struct list_connect));

	list_connect_new-> next = NULL;
	list_connect_new->name = name;

	struct list_message* list_message = malloc ( sizeof ( struct list_message));
	
	list_message->next = NULL;
	list_connect_new->list_message = list_message;

	while (list_connect->next != NULL)
	{
		list_connect = list_connect->next;
	}
	list_connect->next = list_connect_new;
}


int pop_connect (const char * name, list_connect* list_connect)
{
	struct list_connect* list_connect_temp = list_connect;
	list_connect = list_connect->next;
	int index = 0;
	while ( strcmp ( name , list_connect->name) != 0)
	{
		index++;
		list_connect = list_connect->next;
		list_connect_temp = list_connect_temp->next;
	}
	list_connect_temp->next = list_connect->next;


	return index;

}



