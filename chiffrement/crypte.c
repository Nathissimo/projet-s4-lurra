#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <err.h>

#include "crypte.h"




void init_key (struct UserKey* UserKey)
{


    struct Public_Private* Key_Private = malloc (sizeof(struct Public_Private));
    struct Public_Private* Key_Public = malloc ( sizeof(struct Public_Private));

//    this variable are not random
//    size_t p = 11;// Ramdom first prime (ex : 3)
//    size_t q = 23;//Random first prime  (ex : 11)
	
    size_t p = get_Random();
    size_t q = get_Random();


    while ( p == q )
    {
	    q =get_Random();
    }


    //it's not necessairie
    if ( p > q)
    {
	    size_t tempe = p;
	    p =q ;
	    q =tempe;
    }

    //must delete
    printf ("(p,q) : (%lu,%lu)\n",p , q);
	
    size_t n = p *q;
    size_t phi = (p-1) * (q-1);
    size_t e= 2;	
    for(; e< phi; e++ )
    {
        if ( pgcd ( e , phi) == 1)
            break;
    }

    if ( e == phi)
        e = 1;

    size_t d= 1;
    while (((d*e)% phi) != 1)
    {
        d++;
    }

    Key_Public->nb1 = n;
    Key_Public->nb2 = e;

    Key_Private->nb1 =n;
    Key_Private->nb2 =d;

    UserKey->Public = Key_Public;
    UserKey->Private = Key_Private;


}


unsigned long* encryption (char* message , struct UserKey* userkey)
{
    size_t len = strlen (message);

    unsigned long* encrypte = malloc ( sizeof ( unsigned long) * (len +1));
    for ( size_t i = 0 ; i < len ; i++)
    {
        unsigned long long n1 = *(message +i) ;

        unsigned long value = Pow_and_Mod (n1 , userkey->Public->nb2 , userkey->Public->nb1);
        * (encrypte+i) = value;

    }
    *(encrypte + len) = 0;
    return encrypte;
}


char* decrypte ( unsigned long* message, struct UserKey* userkey, size_t len )
{

    char* encrypte = malloc ( sizeof ( char) * (len +1));
    for ( size_t i = 0 ; *(message +i) != 0 ; i++)
    {
        unsigned long long n1 = *(message +i) ;

        unsigned long value = Pow_and_Mod ( n1, userkey->Private->nb2 , userkey->Private->nb1); 
        * (encrypte+i) = value % 255 ;

    }
    *(encrypte + len) = 0;
    return encrypte;
}

void created_Key (UserKey* UserKey)
{

    init_key ( UserKey);
    //add created key int data_reseau
}


int fct_main (char * path)
{
    struct UserKey UserKey;

    init_key ( &UserKey);


    char* mes =path;
    unsigned long* res = encryption ( mes , &UserKey);
    char* res2 = decrypte ( res, &UserKey, strlen(mes));

    printf ( "Key Public : (%lu , %lu)\n", UserKey.Public->nb1, UserKey.Public->nb2);
    printf ( "Key Private : (%lu , %lu)\n", UserKey.Private->nb1, UserKey.Private->nb2);
    printf ( "Message to send : %s\n", mes);
    printf ( "Message after encryption : ");

    for (size_t i = 0; *(res+i) != 0 ; i++)
    {
        printf ( "%ld ", *(res+i));
    }
    printf ("\n");
    printf ( "Message after decryption : %s\n", res2);

    size_t i =0;
    for ( ; i< strlen(mes)+1; i++)
    {
        if ( *(mes) != *(res2))
            printf ("FALSE\n");
    }
    if ( i == strlen(mes)+1)
        printf ("TRUE\n");

    return 1;

}


/*
   int main (int argc , char * argv[])
   {

   if ( argc != 2 ) 
   err ( 1 , " usage : message\n");
   struct UserKey UserKey;

   init_key ( &UserKey);


   char* mes =argv[1];
   unsigned long* res = encryption ( mes , &UserKey);
   char* res2 = decrypte ( res, &UserKey, mes);

   printf ( "Key Public : (%lu , %lu)\n", UserKey.Public->nb1, UserKey.Public->nb2);
   printf ( "Key Private : (%lu , %lu)\n", UserKey.Private->nb1, UserKey.Private->nb2);
   printf ( "Message to send : %s\n", mes);
   printf ( "Message after encryption : ");

   for (size_t i = 0; *(res+i) != 0 ; i++)
   {
   printf ( "%ld ", *(res+i));
   }
   printf ("\n");
   printf ( "Message after decryption : %s\n", res2);

   size_t i =0;
   for ( ; i< strlen(mes)+1; i++)
   {
   if ( *(mes) != *(res2))
   printf ("FALSE\n");
   }
   if ( i == strlen(mes)+1)
   printf ("TRUE\n");

   return 1;
   }

*/
