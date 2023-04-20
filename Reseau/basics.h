#pragma once

#include "struct.h"


//NAME
void rewrite_name (int fd, const void *buf, size_t count);
void ask_name (  int cfd , char* name);
int same_name ( char* name1 , char* name2);

//KEY
UserKey* receive_key_public ();
