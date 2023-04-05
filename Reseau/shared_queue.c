#include <err.h>
#include <stdlib.h>
#include "shared_queue.h"

shared_queue* shared_queue_new()
{
    struct shared_queue* shared_q = malloc (sizeof (struct shared_queue));
    if (shared_q == NULL)
        errx ( EXIT_FAILURE, "error malloc()" );
    //struct queue* queue = malloc (sizeof(struct queue));
/*	
    if (queue == NULL)
        errx ( EXIT_FAILURE, "error malloc()" );
  */
    shared_q->queue = NULL;
    sem_init (&shared_q->lock,0,1);
    sem_init (&shared_q->size,0,0);
    return shared_q;
}

void shared_queue_push(shared_queue* sq, int val)
{
    sem_wait (&sq->lock);
    sq->queue = queue_push( sq->queue, val);
    sem_post(&sq->lock);
    sem_post(&sq->size);
}

int shared_queue_pop(shared_queue* sq)
{
    sem_wait(&sq->size);
    sem_wait(&sq->lock);
    int val;
    sq->queue = queue_pop ( sq->queue, &val);
    sem_post(&sq->lock);
    return val;
}

void shared_queue_destroy(shared_queue* sq)
{
    queue_empty (&sq->queue);
    sem_destroy (&sq->lock);
    sem_destroy (&sq->size);
    free (sq);

}
