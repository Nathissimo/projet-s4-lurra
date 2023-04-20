#pragma once



#include "struct.h"


int same_name2 ( char* name1 , char* name2);
struct_message* init_data_message ();
void push_message (struct_message* struct_message, unsigned long * message, size_t size, char* sender, char* receiver);
unsigned long* check_and_return_message ( struct_message* struct_message, char* receiver, size_t* size );
