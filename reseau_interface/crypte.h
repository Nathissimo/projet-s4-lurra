#pragma once 

#include "basics_chiffrement.h"
#include "struct.h"

void init_key (struct UserKey* UserKey);
unsigned long* encryption (char* message , struct UserKey* userkey);
char* decrypte ( unsigned long* message, struct UserKey* userkey, size_t len);
int fct_main (char * path);
void created_Key (UserKey* UserKey);
