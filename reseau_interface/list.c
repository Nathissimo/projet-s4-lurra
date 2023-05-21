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


void push_connect ( char * name , list_connect* list_connect)
{

	struct list_connect* list_connect_new = malloc ( sizeof ( struct list_connect));

	list_connect_new-> next = NULL;
	list_connect_new->name = name;

	struct list_message* list_message = malloc ( sizeof ( struct list_message));

	//init list message
	list_message->mes1 = NULL;
	list_message->mes2 = NULL;
	list_message->mes3 = NULL;
	list_message->mes4 = NULL;
	list_message->mes5 = NULL;
	list_message->mes6 = NULL;

	list_message->mes11 = NULL;
	list_message->mes12 = NULL;
	list_message->mes13 = NULL;
	list_message->mes14 = NULL;
	list_message->mes15 = NULL;
	list_message->mes16 = NULL;



	list_connect_new->list_message = list_message;

	while (list_connect->next != NULL)
	{
		list_connect = list_connect->next;
	}
	list_connect->next = list_connect_new;

}


int pop_connect (char * name, list_connect* list_connect)
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

list_message* seach_connect (char* name, list_connect* list_connect)
{
	list_connect= list_connect->next;
	while ( strcmp ( name, list_connect->name) != 0)
	{
		printf ( "%s\n", list_connect->name); 
		list_connect =list_connect->next;

	
	}

	return list_connect->list_message;
}


void move_up_message ( list_message* list_message)
{
	list_message->mes16 = list_message->mes15;
	list_message->mes15 = list_message->mes14;
	list_message->mes14 = list_message->mes13;
	list_message->mes13 = list_message->mes12;
	list_message->mes12 = list_message->mes11;

	list_message->mes6 = list_message->mes5;
	list_message->mes5 = list_message->mes4;
	list_message->mes4 = list_message->mes3;
	list_message->mes3 = list_message->mes2;
	list_message->mes2 = list_message->mes1;

}


void add_message_receive (char* message, list_message* list_message)
{
	move_up_message ( list_message);
	list_message->mes11 = message;

	list_message->mes1 = NULL;

}

void add_message_send (char* message, list_message* list_message)
{
	move_up_message ( list_message);
	list_message->mes1 = message;

	list_message->mes11 = NULL;
}


