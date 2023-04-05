#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
//#include <png.h>

#include "cmpr.h"

comprime rle(int *image, int lon, int lag)
{
    int max = 9;
    unsigned long int img = 0;
    unsigned int suite = 0;
    int prec = image[0];
    int act;
    for (int i=0; i<(lon*lag); i++)
    {
        //printf("%i\n", prec);
        act = image[i];
        if (prec == act)
        {
            suite +=1;
            if (suite==max)
            {
                img = (img*10)+suite;
                img = (img*10)+prec;
                suite = 1;
            }
        }

        else
        {
            //printf("suite:%i   prnb:%i\n", suite, prec);
            img = (img*10)+suite;
            img = (img*10)+prec;
            suite = 1;
        }

        prec = act;
    }

    img = (img*10)+suite;
    img = (img*10)+prec;

    comprime rslt;


    rslt.donnee = img;
    rslt.lon = lon;
    rslt.lag = lag;
    return (rslt);
}

void derle(comprime rslt, int*image)
{
    unsigned long int img = rslt.donnee;

    int nb;
    int suite;
    int i=rslt.lon;
    int j=rslt.lag;
    while (img>0)
    {
        //printf("%lu\n", img);
        nb = img%10;
        img = img/10;
        suite = img%10;
        img = img/10;
        printf("suite:%i   nb:%i\n", suite, nb);

        for (int d=0; d<suite; d++)
        {
            image[(i-1)*rslt.lag+j-1] = nb;
            //printf("enr ici: %i   %i \n", (i-1)*rslt.lag+j-1, image[(i-1)*rslt.lag+j-1]);
            j -=1;
            if (j == 0)
            {
                i -=1;
                j = rslt.lag;
            }
        }
    }
    //for (int i=0; i<(rslt.lon); i++)
    //{
    //   for (int j=0; j<(rslt.lag); j++)
    //    {
    //        printf("\n %i     ", i*(rslt.lag)+j);
    //        printf("%i", image[i*(rslt.lag)+j]);
    //    }
    //    printf("\n");
    //}
}


int main(int argc, char *argv[])
{
    //int tab[24];
    //unsigned int lon = 4;
    //unsigned int lag = 6;

    //if (argv[2] == "test.png")
    //{
    //    int tab[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0};
    unsigned int lon = 3;
    unsigned int lag = 4;
    //}
    //if (argv[2]=="test2.png")
    //{
    int tab[] = {0,0,0,0,1,1,1,0,1,0,0,0};//12
    //}
    //int tab[] = {1,1,0,0,0,0};//6
    //int tab[] = {2,6,75,0,1,1,1,0,1,10,0,550,8,2};//14
    for (int i=0; i<(lon); i++)
    {
        printf("[");
        int j;
        for (j=0; j<(lag); j++)
        {
            printf("%i, ", tab[i*lag+j]);
        }
        printf("]\n");
    }

    comprime rst = rle(tab, lon, lag);

    printf("code : %lu\n", rst.donnee);
    
    int result[rst.lag*rst.lon];
    derle(rst, &result);
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
