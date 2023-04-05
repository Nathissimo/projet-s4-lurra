#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "cmpr.h"

int main()
{
    
    int tab[] = {0,0,0,0,1,1,1,0,1,0,0,0};//12
    //int tab[] = {1,1,0,0,0,0};//6
    //int tab[] = {2,6,75,0,1,1,1,0,1,10,0,550,8,2};//14
    unsigned int lon = 4;
    unsigned int lag = 3;

    comprime rst = condense(tab, lon, lag);

    printf("code : %lu\n", rst.donnee);
    
    int result[rst.lag*rst.lon];
    decondense(rst, &result);
    for (int i=0; i<(rst.lon); i++)
    {
        printf("[");
        int j;
        for (j=0; j<(rst.lag); j++)
        {
            printf("%i, ", *(result+(i*rst.lag+j)));
        }
        printf("]\n");
    }
}
