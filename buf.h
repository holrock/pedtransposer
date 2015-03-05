#ifndef BUF_H_
#define BUF_H_

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

struct Buf
{
  char* data;
  size_t size;
  char* rest_point;
};

struct Buf init_buf(const char* file_name, size_t buf_size);
bool read_next(struct Buf* buf, FILE* fp);
void free_buf(struct Buf* buf);

#endif
