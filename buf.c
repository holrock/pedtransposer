#include "buf.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

struct Buf init_buf(const char* file_name, size_t buf_size)
{
  struct Buf buf = {};
  if (buf_size != 0) {
    buf.data = malloc(buf_size);
    buf.size = buf_size;
  } else {
    size_t size = get_file_size(file_name) + 1;
    buf.data = malloc(size);
    buf.size = size;
  }

  if (!buf.data) {
    fprintf(stderr, "malloc error\n");
    exit(-1);
  }
  return buf;
}

void free_buf(struct Buf* buf)
{
  free(buf->data);
  buf->size = 0;
  buf->data = NULL;
}
