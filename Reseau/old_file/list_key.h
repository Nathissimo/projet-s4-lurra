#pragma once 

#include "struct.h"

struct_key* init_data_key (void);
void push_key (struct_key* struct_key, size_t nb1 , size_t nb2 );
void pop_key ( struct_key* struct_key , size_t index);

void return_key_public ( struct_key* struct_key, size_t index, UserKey* UserKey);

UserKey* receive_key_public ( );
