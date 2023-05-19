#pragma once 

#include <semaphore.h>


typedef struct Public_Private
{
    size_t nb1;
    size_t nb2;
}Public_Private;

typedef struct UserKey
{
    //key Public
    struct Public_Private* Public;
    //key private
    struct Public_Private* Private;
}UserKey;


typedef struct queue
{
    // Value of an element.
    int val;

    // Pointer to the next element.
    struct queue *next;
} queue;

typedef struct shared_queue
{
    // Used to lock the push and pop operations.
    // (Similar to a mutex.)
    sem_t lock;

    // Used to count the number of values.
    sem_t size;

    // Simple queue.
    queue* queue;
} shared_queue;


//data message
typedef struct data_message
{
    size_t size;
    unsigned long* message;
    char* sender;
    struct data_message* next;

}data_message;


//data all client connect with all message who have receive
typedef struct data_client
{
	struct data_client* next;
	char* name;
	int cfd;
	Public_Private* key;
	data_message *message;
	
}data_client;

typedef struct all_data
{
	data_client* list_data_client;
	sem_t lock;
}all_data;


typedef struct data_reseau
{
    //queue all connect
    shared_queue* sh_queue;
	
    // all data,: name, key and cfd (socket)
    all_data * all_data;

    size_t nb_thread;
}data_reseau;

//for thread message
typedef struct struct_thread
{
	data_reseau* data_reseau;
	int cfd;
	char* name;
	UserKey* key;
}struct_thread;

