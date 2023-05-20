#pragma once 

#include "struct.h"

list_connect* init_list_connect ();
void push_connect (const char * name , list_connect* list_connect);
int pop_connect ( const char * name, list_connect* list_connect);
