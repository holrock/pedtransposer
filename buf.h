#ifndef BUF_H_
#define BUF_H_

#include <ctype.h>

struct Buf
{
  char* data;
  size_t size;
};

struct Buf init_buf(const char* file_name, size_t buf_size);
void free_buf(struct Buf* buf);

#endif
