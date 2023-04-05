#pragma once 

#include <semaphore.h>

typedef struct Key
{
    struct Key* next;
    size_t nb1;
    size_t nb2;
    int empty; // 0 empty /////////// 1 full
}Key;

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


// struct for all name
//
typedef struct name_connect
{
    struct name_connect* next;
    char* name;

}name_connect;

typedef struct data_name
{
    name_connect* list_name;
    sem_t lock;
}data_name;

typedef struct data_message
{
    size_t size;
    char* sender;
    char* receiver;
    char** message;
    struct data_message* next;

}data_message;

typedef struct struct_message
{
        data_message* list_message;
        sem_t lock;

}struct_message;


typedef struct data_cfd
{
    int* all_cfd;
    sem_t lock;

}data_cfd;


typedef struct struct_key
{
    struct Key* list_key;
    sem_t lock;
}struct_key;

typedef struct data_reseau
{
    //queue all connect
    shared_queue* sh_queue;

    //list of name people
    data_name* name;

    //list of cdf  (accept) 
    data_cfd* all_cfd;

    //list of all key_public to people connect	
    struct_key* all_key_public ;

    //elemnt to  echange information (here a image ) ( ex a string)

    struct_message* all_message;

    // nb connection
    size_t nb_thread;

}data_reseau;

