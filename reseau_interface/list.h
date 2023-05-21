#pragma once 

#include "struct.h"

list_connect* init_list_connect ();
void push_connect (char * name , list_connect* list_connect);
int pop_connect (char * name, list_connect* list_connect);
list_message* seach_connect (char* name, list_connect* list_connect);
void move_up_message ( list_message* list_message);
void add_message_receive (char* message, list_message* list_message);
void add_message_send (char* message, list_message* list_message);
