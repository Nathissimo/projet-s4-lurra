#pragma once

#include "struct.h"

data_name* init_data_name ( void);
void push_name (data_name* data_name, char* name );
void pop_name ( data_name* data_name , size_t index );

char* return_name (data_name* data_name, size_t index);

void rewrite_name (int fd, const void *buf, size_t count);
void ask_name (  int cfd , char* name);


int same_name ( char* name1 , char* name2);
int name_exist ( data_name* data_name, char* name );
