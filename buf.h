#ifndef BUF_H_
#define BUF_H_

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

struct Buf
{
  size_t cap;
  size_t file_size;
  char* rest_point;
  char data[];
};

struct Buf* init_buf(size_t buf_size, size_t file_size);
bool read_next(struct Buf* buf, FILE* fp);
void free_buf(struct Buf* buf);

#endif
