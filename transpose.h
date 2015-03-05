#ifndef TRANSPOSE_H_
#define TRANSPOSE_H_

#include <stdio.h>
#include <ctype.h>

struct Buf;
int transpose_ped(FILE* input, FILE* output, struct Buf* buf, char delim);

// for test
size_t count_column(const char* buf);

#endif
