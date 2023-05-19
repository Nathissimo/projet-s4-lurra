#include <err.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "basics.h"
//NAME
//
#include <netdb.h>
//old file list_name.c

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
//    char quest[]= "What your name?\n";
//    rewrite_name( cfd, quest , strlen( quest));

    ssize_t  r = read ( cfd, res , 16); // max xname 16 caractere
	
    //test
    printf ("%s\n", res);

//    rewrite_name ( cfd, quest, sizeof (quest));    
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



//KEY (list_key.c)
UserKey* receive_key_public ( )
{
        UserKey* UserKey = malloc (sizeof (UserKey));

        struct Public_Private* Key_Private = malloc (sizeof(struct Public_Private));
        struct Public_Private* Key_Public = malloc ( sizeof(struct Public_Private));

        UserKey->Public = Key_Public;
        UserKey->Private = Key_Private;

        return UserKey;

}

