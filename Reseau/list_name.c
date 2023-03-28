#include <err.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "list_name.h"



data_name* init_data_name ( void )
{
    struct data_name* data_name = malloc (sizeof (struct data_name));
    if ( data_name == NULL)
        errx (EXIT_FAILURE, "malloc()");

    sem_init (&data_name->lock, 0 , 1);

    struct name_connect* name_connect = malloc (sizeof (struct name_connect));
    name_connect->next = NULL;


    data_name->list_name = name_connect;
    return data_name;

}


void push_name (data_name* data_name, char* name )
{
    sem_wait (&data_name->lock);

    //initialization of new name
    struct name_connect* new_name = malloc (sizeof (struct name_connect));
    new_name->next = NULL;
    new_name->name = name;

    struct name_connect* name_connect = data_name->list_name->next;

    // if list is empty
    if ( name_connect == NULL)
    {
        data_name->list_name->next = new_name;
        sem_post (&data_name->lock);
        printf ("good\n");
        return ;
    }

    if (name_connect->name == NULL)
    {
        name_connect->name = name;
        free ( new_name);
        sem_post (&data_name->lock);
        return;
    }

    for ( ;name_connect->next != NULL; name_connect = name_connect->next)
    {
        if ( name_connect->name == NULL)
        {
            name_connect->name = name;
            free ( new_name);
            sem_post (&data_name->lock);
            return;
        }
    }
    if ( name_connect->name == NULL)
    {
        name_connect->name = name;
        free ( new_name);
        sem_post (&data_name->lock);
        return;
    }
    name_connect->next = new_name;	

    sem_post (&data_name->lock);

}

// it is assumed that the name is exists
// must created a fct with check this
void pop_name ( data_name* data_name , size_t index )
{	

    sem_wait (&data_name->lock);
    name_connect* name_connect = data_name->list_name->next;
    for (size_t i =0 ; i < index ; i++ )
    {
        name_connect = name_connect->next;
        if ( name_connect == NULL)
            errx (EXIT_FAILURE, "error index is to big in struct data_name");
    }

    name_connect->name = NULL;

    sem_post (&data_name->lock);
}

void rewrite_name (int fd, const void *buf, size_t count)
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


void ask_name (  int cfd , char* name)
{
    char res[16];
    char quest[]= "What your name?\n";
    rewrite_name( cfd, quest , strlen( quest));

    ssize_t  r =read (cfd, res , 16 ); // max xname 16 caractere

    for (ssize_t i = 0 ; i<r ; i++)
    {
        name[i] = res[i];
    }
    name[r] = 0;

}

//ne prend pas en compte si il y a un commentaire
int same_name ( char* name1 , char* name2)
{
    if (name2 == NULL)
        return 0;
    size_t len1 = strlen ( name1);
    size_t len2 = strlen ( name2);

    if ( len1 != len2 )
    {
        return 0;
    }

    for ( size_t i = 0; i<len1 ; i++)
    {
        if ( name1[i] != name2[i])
            return 0;

    }
    return 1;


}

// return index if name exit in list
// return -1 if name don t existe
//
// don't use a sem_t (because don't edit a struct just seach a name)
int name_exist ( data_name* data_name, char* name )
{
    struct name_connect* name_connect = data_name->list_name->next;
    if ( name_connect == NULL)
        err(EXIT_FAILURE, "error no people in struct list");

    int index = 0;
    for ( ;name_connect->next != NULL; name_connect = name_connect->next)
    {
        if ( same_name ( name, name_connect->name) == 1)
        {
            return index;
        }
        index ++;


    }


    if ( same_name ( name, name_connect->name) == 1)
    {
        return index;
    }


    printf ("This name not connect in this server\n");

    return -1;	

}
