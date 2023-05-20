#pragma once

#include "struct.h"


all_data* init_data_client (void);

void delete_message_data ( data_message* data_message, all_data* all_data);

void push_client ( all_data* all_data, char * name, int cfd, Public_Private* key );
void pop_client ( all_data* all_data, int cfd );

data_client* search_client (all_data* all_data, char* name);

data_message* check_my_message ( all_data* all_data, char* name, int * end);
void push_message ( all_data* all_data, char* name, unsigned long* message, size_t size, char* sender);
