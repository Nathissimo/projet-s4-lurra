#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

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

void *derle(comprime rslt, int*image)
{
    unsigned long int img = rslt.donnee;

    int nb;
    int suite;
    int i=rslt.lon;
    int j=rslt.lag;
    while (img>0)
    {
        printf("%lu\n", img);
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
    FILE *fichier = NULL;
    fichier = fopen("test.png", "r");

    int caractereActuel = 0;
    if (fichier != NULL)
    {
        do
        {
            caractereActuel = fgetc(fichier); 
            printf("%c", caractereActuel);
        } while (caractereActuel != EOF);
    }
    else
        printf("fichier non lu\n");





    int fclose(fichier);

    return 1;





    int tab[] = {0,0,0,0,1,1,1,0,1,0,0,0};//12
    //int tab[] = {1,1,0,0,0,0};//6
    //int tab[] = {2,6,75,0,1,1,1,0,1,10,0,550,8,2};//14
    unsigned int lon = 4;
    unsigned int lag = 3;

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
