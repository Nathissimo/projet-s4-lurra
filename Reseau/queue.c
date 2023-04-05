#include <err.h>
#include <stdlib.h>
#include "queue.h"

queue* queue_push(queue* start, int val)
{
    struct queue* q = malloc (sizeof(struct queue));
    if (q == NULL)
    {
        errx (EXIT_FAILURE, "error malloc");
    }
    q->val = val;
    
    if (start == NULL)
    {
        q->next = q;
    }
    else
    {
        q->next = start->next;
        start->next = q;
    }
    return q;
}

queue* queue_pop(queue* start, int *pval)
{
    if ( start == NULL )
    {
        return NULL;
    }
    else
    {
        struct queue* q = start->next;
        *pval = q->val;
	if ( q != start)
	{
        	start->next = q->next;
        	free (q);
       		return start;
	}
	else
	{
		free(start);
		return NULL;
	}
    }
}

void queue_empty(queue** pstart)
{
    int val;
    while ( *pstart != NULL)
    {
        *pstart = queue_pop ( *pstart, &val);
    }

}
