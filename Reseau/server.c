#define _GNU_SOURCE

#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "shared_queue.h"
#include "../chiffrement/crypte.h"
#include "list_cfd.h"
#include "struct.h"
#include "list_cfd.h"
#include "list_name.h"
#include "list_key.h"

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

void echo(int fd_in, int fd_out)
{
    char buffer[BUFFER_SIZE];
    while (1)
    {
        if ( fd_in == -1)
        {
            err (1 , "the connection do not accept");
        }

        ssize_t r = read (fd_in, buffer , BUFFER_SIZE);

        if ( r == -1)
        {
            err ( 1, "Error to read function");
        }
        rewrite ( fd_out , buffer , r);
        if (r == 0)
            break;

    }
    return;

}



void send_mess ( data_reseau* data_reseau, int cfd )
{

    char info[] = "if you want to send a message:\nwrite the name of the person then the message\n";
    rewrite(cfd , info, strlen (info));	
    while (1)
    {	
        char buffer[MAX_LETTER+1];
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
            break;
        }

        int index =  name_exist ( data_reseau->name, name);
        int cfd_receive  = data_reseau->all_cfd->all_cfd[index];
        
        printf ("connection : %i\n", index);

        if ( index == -1 )
            continue;

        
        char reponse [SIZE_MESSAGE] ;//initialization of size (maybe)


        r  =read ( cfd, reponse, SIZE_MESSAGE);
        printf ("OK \n");
        rewrite ( cfd_receive , reponse , strlen(reponse));
    }


}


// Function executed by the threads.
void* worker(void* arg)
{
    // Gets the shared queue.
    data_reseau* data_reseau = arg;
    shared_queue* queue = data_reseau->sh_queue;
    data_name* all_name = data_reseau->name;
    data_cfd* all_cfd = data_reseau->all_cfd;
    struct_key* all_key = data_reseau->all_key_public;

    while (1)
    {
        if (queue->queue != NULL)
        {
            int cfd = shared_queue_pop ( queue);

            char* name = malloc (17 * sizeof(char));
            ask_name (cfd, name);
            struct UserKey* UserKey = malloc (sizeof (struct UserKey));
            created_Key(UserKey);



            printf ("name:%s, cfd:%i key public: (%li,%li) \n", name, cfd, UserKey->Public->nb1, UserKey->Public->nb2 );

            //push all element in this cfd
            push_cfd ( all_cfd, cfd, data_reseau->nb_thread);
            push_name (all_name, name);
            push_key (all_key, UserKey->Public->nb1, UserKey->Public->nb2);// the key is not good


            // send a message
            send_mess ( data_reseau, cfd);


            // echange the data beetween the differents client

            //delete all information in this cfd
            printf ("delete information\n");

            int res = -1;
            seek_cfd ( all_cfd, cfd, data_reseau->nb_thread, &res);
            size_t index = res;
            pop_cfd (all_cfd, index);
            pop_name ( all_name, index);
            pop_key (  all_key, index);

            printf ("delete ok\n");
            close (cfd);
        }
    }
    return NULL;
}

int main()
{
    //Creates the data for thread

    data_reseau* data_reseau = malloc ( sizeof (data_reseau));

    //initialization
    data_reseau->sh_queue = shared_queue_new();
    data_reseau->name = init_data_name ();
    data_reseau->all_cfd = init_list_cfd ( THREAD_COUNT);
    data_reseau->all_key_public = init_data_key ();
    data_reseau->message = NULL;// I suppose delete in the future
    data_reseau->nb_thread= THREAD_COUNT;    

    //Create threads

    for (size_t i = 0; i<THREAD_COUNT ; i++)
    {
        pthread_t thr;
        int test = pthread_create (&thr, NULL , worker, (void*)data_reseau);
        if (test != 0)
            errx (EXIT_FAILURE, "pthread_create()");
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
