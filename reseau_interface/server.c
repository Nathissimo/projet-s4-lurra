#define _GNU_SOURCE

#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>

#include "shared_queue.h"
#include "crypte.h"
#include "struct.h"
//#include "list_cfd.h"
//#include "list_name.h"
//#include "list_key.h"
//#include "list_message.h"
#include "list_all_data.h"
#include "basics.h"


#define SIZE_MESSAGE 4096

// Number of threads.
const size_t THREAD_COUNT = 4;

// Buffer size in bytes.
const size_t BUFFER_SIZE = 256;
const size_t MAX_LETTER = 16;


void rewrite(int fd, const void *buf, size_t count)
{
	size_t print =0;
	while ( print != count)
	{
		ssize_t r =0;
		if ((r =write (fd, buf, count)) == -1)
			err (3, "error for write data");

		print += (size_t)r;
	}

}


// retun a int
// 0 the name exist
// -1 name don't exist
// -2 client quit the server
int read_name_init (all_data* all_data, char** receiver, int cfd)
{

	char buffer[MAX_LETTER+1];
	memset (buffer, 0, MAX_LETTER);
	ssize_t r = read (cfd, buffer , MAX_LETTER);
	char exit[] = "exit\n";

	char* name = malloc ( sizeof (char) * MAX_LETTER+1);
	for (ssize_t i = 0 ; i<r ; i++)
	{
		name[i] = buffer[i];
	}

	name[r] = 0;

	if ( same_name(exit, name))
	{
		printf ("EXIT\n");
		return -2;
	}
	data_client* client =  search_client ( all_data, name);
	int index;	
	//this client don't exist
	if (client == NULL)
		index = -1;
	else
		index = 0;

	*receiver = name;
	return index;
}


void* thread_message ( void * arg)
{
	struct_thread* struct_thread = arg;
	int cfd = struct_thread->cfd;
	char* name = struct_thread->name;
	data_reseau* data_reseau = struct_thread->data_reseau;
	UserKey* key_private_public = struct_thread->key;

	int end = 0;
	while ( 1)
	{
		sleep(1);

		// check if you have a message

		//char* rep = malloc (sizeof ( char)* SIZE_MESSAGE);
		data_message* rep = check_my_message (data_reseau->all_data, name , &end);
		if (end == 1)
		{
			break;
		}


		//first iteration
		if ( rep != NULL)
		{
			char* mess_decrypte = decrypte (rep->message, key_private_public, rep->size);
			/*
			//print the message
			char* info1 ="Message send from ";
			//print sentence
			rewrite ( cfd ,info1, strlen(info1));
			//print name of sender
			rewrite ( cfd , rep->sender, strlen ( rep->sender));
			*/
			send (cfd , "message", sizeof ("message"), 0);
			sleep ( 0.1);
			//print message
			send ( cfd , mess_decrypte , rep->size, 0);
			
			//DELETE?
			//sleep ( 0.1);
			send ( cfd , rep->sender, 16, 0);
			//free struct message

		}
		//while ( rep != NULL)
		while ( rep != NULL)
		{

			rep = check_my_message (data_reseau->all_data, name, &end);
			if ( rep != NULL)
			{
				char* mess_decrypte = decrypte (rep->message, key_private_public, rep->size);
				/*
				//print the message
				char* info1 ="Message send from ";
				//print sentence
				rewrite ( cfd ,info1, strlen(info1));
				//print name of sender
				rewrite ( cfd , rep->sender, strlen ( rep->sender));
				*/

				send (cfd , "message", sizeof ("message"), 0);
				sleep ( 0.1);
				//print message
				send ( cfd , mess_decrypte , rep->size, 0);
				//free struct message


				//DELETE?
				sleep ( 0.1);
				send ( cfd , rep->sender, 16, 0);

			}

		}
	}
	printf ("end thread message %s\n", name);

	return NULL;


}



void send_mess ( data_reseau* data_reseau, int cfd, char* name, UserKey* key_private_public)
{


	int r;
	struct struct_thread* struct_thread = malloc ( sizeof (struct struct_thread));
	struct_thread->name = name;
	struct_thread->cfd = cfd ;
	struct_thread->data_reseau = data_reseau;
	struct_thread->key = key_private_public;
	pthread_t thr; 

	r = pthread_create ( &thr, NULL, thread_message, struct_thread );
	if ( r !=0 )
		err ( EXIT_FAILURE, "error thread message");
	
	while (1)
	{
		/*
		   char info[] = "Write the name of the person you want to send a message to :\n";
		   rewrite (cfd, info, strlen (info));
		   */
		char* name_receiver;


		int index = read_name_init ( data_reseau->all_data, &name_receiver, cfd);
		//DELETE
		printf ( "%s\n", name_receiver);




		if ( index == -2)
			break;

		else if ( index == 0  )
		{



			printf ("Write message\n");
			char* reponse  = malloc ( sizeof (char)* SIZE_MESSAGE ) ;//initialization of size (maybe)
			memset (reponse, 0 , SIZE_MESSAGE);


			//send message
			r  =read ( cfd, reponse, SIZE_MESSAGE);
			if ( r == -1 )
				errx (EXIT_FAILURE, "read() error");

			printf ("%s\n", reponse);
			printf ("OK \n");
			// crypte message

			data_client* data_client = search_client (data_reseau->all_data, name_receiver);

			UserKey* UserKey_receiver = receive_key_public ( );
			UserKey_receiver->Public = data_client->key;
			unsigned long* message_crypte = encryption( reponse, UserKey_receiver); //message crypte
			

			push_message ( data_reseau->all_data,data_client->name ,message_crypte, strlen (reponse), name); //size is maybe not good	

			//simple write delete in future
			//rewrite ( data_client->cfd , reponse , strlen(reponse));
		}
		else if ( index == -1)
		{
			char name_dont_existe[] = "This people is not connected to this server\n";
			rewrite ( cfd , name_dont_existe, strlen( name_dont_existe));
		}



	}



}


// Function executed by the threads.
void* worker(void* arg)
{
	// Gets the shared queue.
	data_reseau* data_reseau = arg;
	shared_queue* queue = data_reseau->sh_queue;
	all_data* data = data_reseau->all_data;

	while (1)
	{
		if (queue->queue != NULL)
		{
			int cfd = shared_queue_pop ( queue);

			//send cfd



			//end send cfd


			char* name = malloc (17 * sizeof(char));
			ask_name (cfd, name);
			struct UserKey* UserKey = malloc (sizeof (struct UserKey));
			created_Key(UserKey);

			//ADD
			//send the new name connect in server
			data_client* temp_client = data->list_data_client->next; 
			for ( ; temp_client != NULL ; temp_client = temp_client->next)
			{
				if ( temp_client->c == 0)
					continue;

				printf ( "cfd = %i\n", temp_client->cfd);
				send ( temp_client->cfd, "name", sizeof ("name"),0);
				send ( temp_client->cfd, name , sizeof (name), 0 );

				//send to the new people all people who are connect
				send ( cfd, "name" , sizeof ( "name") , 0);
				printf ( "error : %s\n", temp_client->name);
				sleep( 1);
				send ( cfd, temp_client->name , 16 , 0);
			}


			printf ("name:%s cfd:%i key public: (%li,%li) \n", name, cfd, UserKey->Public->nb1, UserKey->Public->nb2 );

			//push all data to the client
			push_client ( data , name , cfd , UserKey->Public);


			// send a message
			send_mess ( data_reseau, cfd, name, UserKey);



			//delete all information in this cfd
			printf ("delete information\n");

			data_client* temp_delete_client = data->list_data_client->next;

			//delete name in interface
			for ( ; temp_delete_client != NULL; temp_delete_client = temp_delete_client->next)
			{
				if ( temp_delete_client->c == 0)
					continue;

				printf ( "cfd = %i\n", temp_delete_client->cfd);
				send ( temp_delete_client->cfd, "delete", sizeof ("delete"),0);
                                send ( temp_delete_client->cfd, name , sizeof (name), 0 );

			}
			pop_client (data, cfd );
			printf ("delete ok\n");
			close (cfd);
		}
	}
	return NULL;
}

int main()
{
	//Creates the data for thread

	struct data_reseau* data_reseau = malloc ( sizeof (struct data_reseau));

	//initialization
	data_reseau->sh_queue = shared_queue_new();
	data_reseau->all_data = init_data_client ();
	data_reseau->nb_thread = THREAD_COUNT;


	//Create threads
	for (size_t i = 0; i<THREAD_COUNT ; i++)
	{
		pthread_t thr;
		int test = pthread_create (&thr, NULL , worker, (void*)data_reseau);
		if (test != 0)
			errx (EXIT_FAILURE, "pthread_create()");
		pthread_detach (thr);
	}




	struct addrinfo hints;
	memset (&hints, 0, sizeof ( hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* result;

	int err = getaddrinfo( NULL, "2048" , &hints, &result);

	if ( err != 0)
		errx ( EXIT_FAILURE, "error port");

	int sfd;
	struct addrinfo* p;
	for ( p = result; p!= NULL; p = p->ai_next)
	{
		sfd = socket (p->ai_family,p->ai_socktype,p->ai_protocol);

		if (sfd == -1)
			continue;

		int optval = 1;
		err = setsockopt(sfd,SOL_SOCKET, SO_REUSEADDR,&optval, sizeof(int));
		if (err == -1)
			errx (EXIT_FAILURE, "error setsockopt");

		// Try to bind the socket
		err = bind(sfd, p->ai_addr, p->ai_addrlen);

		if (err == 0)
			break;
		else
			close(sfd);

	}
	freeaddrinfo (result);

	if (p == NULL)
		errx(EXIT_FAILURE, "Could not connect");

	err = listen ( sfd, 5);

	if ( err == -1)
	{
		close ( sfd);
		errx (EXIT_FAILURE, "error in function listen");
	}


	while (1)
	{

		printf ( "Waiting for connections...\n");

		int cfd = accept (sfd, NULL , NULL);

		if ( cfd == -1)
			errx (EXIT_FAILURE, "error for accept sfd");

		printf ( "Connection successful!\n");
		shared_queue_push (data_reseau->sh_queue, cfd);


		//close (cfd);

	}
	close ( sfd);



}
