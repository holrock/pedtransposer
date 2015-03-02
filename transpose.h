#ifndef TRANSPOSE_H_
#define TRANSPOSE_H_

#include <ctype.h>

struct Buf;
int transpose_ped(const char* file_name, struct Buf* buf);

// for test
size_t count_column(const char* buf);

#endif
