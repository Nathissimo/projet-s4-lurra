#ifndef SHARED_QUEUE
#define SHARED_QUEUE

#include <semaphore.h>
#include "queue.h"
#include "struct.h"


// Allocates and initializes a new shared queue.
// Returns a pointer to the new shared queue.
shared_queue* shared_queue_new();

// Pushes a new value onto the shared queue.
// queue = Address of the shared queue.
// val = value to push.
void shared_queue_push(shared_queue* sq, int val);

// Pops a value off the shared queue.
// queue = Address of the shared queue.
// Returns the value.
int shared_queue_pop(shared_queue* sq);

// Frees the shared queue.
// queue = Address of the shared queue.
void shared_queue_destroy(shared_queue* sq);

#endif
