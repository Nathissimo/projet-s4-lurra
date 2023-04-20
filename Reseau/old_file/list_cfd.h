#pragma once
#include "struct.h"

data_cfd* init_list_cfd (size_t size);
void push_cfd (data_cfd* data_cfd, int val, size_t size);
void pop_cfd (data_cfd* data_cfd, int index);
void seek_cfd (data_cfd* data_cfd, int cfd, size_t size, int *res);

