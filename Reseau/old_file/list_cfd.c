#include <semaphore.h>
#include "list_cfd.h"
#include <stdlib.h>
#include <stddef.h>
#include <err.h>
#include <stdio.h>


data_cfd* init_list_cfd (size_t size)
{
	struct data_cfd* data_cfd= malloc (sizeof (struct data_cfd));

	sem_init (&data_cfd->lock, 0, 1);

	int* list_cfd = calloc (size,sizeof(int));


	data_cfd->all_cfd = list_cfd;
	return data_cfd;

}


void push_cfd (data_cfd* data_cfd, int val, size_t size)
{
	sem_wait (&data_cfd->lock);
	for (size_t i =0; i<size; i++)
	{
		if (data_cfd->all_cfd[i] == 0)
		{	
			data_cfd->all_cfd[i] = val;
			break;
		}

	}
	sem_post (&data_cfd->lock);
}

void pop_cfd (data_cfd* data_cfd, int index)
{
	sem_wait(&data_cfd->lock);
	data_cfd->all_cfd[index] = 0;
	sem_post(&data_cfd->lock);
}

void seek_cfd (data_cfd* data_cfd, int cfd, size_t size, int* res)
{
	sem_wait (&data_cfd->lock);
	for (size_t i = 0; i<size; i++)
	{
		if ( data_cfd->all_cfd[i] == cfd)
		{
			*res = i;
			break;
		}
	}
	if ( *res  == -1)
		err(EXIT_FAILURE, "error this cfd don't exit");
	sem_post (&data_cfd->lock);
}
