#include <err.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#include "list_message.h"



data_message* init_data_message (void)
{
    struct data_message* data_message = malloc (sizeof (struct data_message));

    if (data_message == NULL)
        errx (EXIT_FAILURE, "malloc()");

    sem_init (&data_message->lock,0,1);

    data_message->next = NULL;
    return data_message;
}

void push_message (data_message* data_message, char* message)
{
    sem_wait (&data_message->lock);


}

