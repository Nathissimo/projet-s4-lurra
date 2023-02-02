#pragma once 

/*
struct Key
{
	size_t nb1;
	size_t nb2;
};
*/

void init_key (struct Key* Key_Public, struct Key* Key_Private);
unsigned long* encryption (char* message , struct UserKey* userkey);
char* decrypte ( unsigned long* message, struct UserKey* userkey, char* old );
