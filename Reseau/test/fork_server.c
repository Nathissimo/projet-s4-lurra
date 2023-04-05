#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <err.h>
#include "echo.h"
#include  <signal.h>
#include <pthread.h>
#include "fork_server.h"
#define MAX 512

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

void * p_read ( void * arg)
{
	int* cfd = arg;
	char buff [MAX];
	while (1)
	{	
		ssize_t r =read ( *cfd, buff, MAX);
		if ( r== -1)
		{
			errx( 1, "error");
		}
		
		printf ("%s", buff);
		bzero(buff, MAX);
	}
	return NULL;
}

void discuss ( int cfd )
{
	char buff [MAX];
	int n;
	pthread_t thr;
	int e = pthread_create ( &thr, NULL , p_read, &cfd); 
	if ( e != 0)
		errx (EXIT_FAILURE, "pthread_created()");


	while (1)
	{
		n= 0;
		
		while ((buff[n++] = getchar()) != '\n');

		rewrite ( cfd, buff, n+1);
		if ( strncmp("exit", buff,4) == 0)
		{
			printf ("Server Exit...\n");
			bzero( buff, MAX);
			break;
		}
		bzero(buff, MAX);


	}
}	

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage:\n"
                "Arg 1 = Port number (e.g. 2048)");

    struct addrinfo hints;
    memset (&hints, 0, sizeof ( hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo* result;

    int err = getaddrinfo( NULL, argv[1], &hints, &result);

    //Utile
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


    signal( SIGCHLD, SIG_IGN);

    printf ( "Waiting for connections...\n");

    int* Key_Public = malloc(sizeof(int)*8);
    int* Key_Private = malloc(sizeof(int) * 8);
    int nb_co = 0;

    while (1)
    {


        int cfd = accept (sfd, NULL , NULL);
	

	nb_co++;
        if ( cfd == -1)
            errx (EXIT_FAILURE, "error for accept sfd");

        pid_t pid = fork ();

        if ( pid >  0)	// maybe ==
        {
            
            close (cfd);


            continue;
        }

        if ( pid == 0 ) // maybe >
        {
            close (sfd);

            printf ( "New connection (pid = %i)\n", getpid());

            discuss( cfd);

            close (cfd);
            printf ("Close connection (pid = %i)\n", getpid());



            exit (EXIT_FAILURE);
            // ou on met le le signal contre le zombie
        }
        else
        {
            close(cfd);
            close(sfd);
            errx ( EXIT_FAILURE, "error to creat a fork");
        }

    }
    for ( int i = 0 ; i<10 ; i++)
    {
	    printf ("personne 1 : %i, %i, %i, %i\n", Key_Private[i*2] ,Key_Private[i*2+1] , Key_Public[i*2], Key_Public[i*2+1]);
    }
    close (sfd);




}
