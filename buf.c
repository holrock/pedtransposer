#include "buf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

struct Buf* init_buf(size_t buf_size, size_t file_size)
{
  struct Buf* buf = NULL;
  const size_t nul_size = sizeof(char); // size of '\0'

  if (buf_size != 0) {
    buf = xmalloc(sizeof(struct Buf) + sizeof(char) * buf_size);
    buf->cap = buf_size;
  } else {
    buf = xmalloc(sizeof(struct Buf) + sizeof(char) * (file_size + nul_size));
    buf->cap = file_size + nul_size;
  }
  buf->file_size = file_size;
  buf->rest_point = NULL;

  return buf;
}

static
size_t move_forward_rest_data(struct Buf* buf)
{
  if (!buf->rest_point) {
    return 0;
  }
  size_t nrest = strlen(buf->rest_point);
  memcpy(buf->data, buf->rest_point, nrest);
  buf->rest_point = NULL;
  return nrest;
}

static
bool mark_rest_point(struct Buf* buf, size_t read_size)
{
  size_t i;
  for (i = read_size; i != 0; --i) {
    if (buf->data[i] == '\n') {
      buf->data[i] = '\0';

      buf->rest_point = &buf->data[i + 1];
      if (i != 0 && buf->data[i - 1] == '\r') {
        buf->data[i - 1] = '\0';
      }
      break;
    }
  }
  // filled all buffer and missing '\n'
  if (i == 0 && buf->cap == read_size + 1) {
    fprintf(stderr, "mark_rest_point: can't read one line, buffer size is too small\n");
    return false;
  }
  return true;
}

bool read_next(struct Buf* buf, FILE* fp)
{
  if (!buf || buf->cap == 0) {
    fprintf(stderr, "read_next: uninitialized buffer\n");
    return false;
  }

  size_t nmoved = move_forward_rest_data(buf);
  char* p = buf->data + nmoved;
  size_t read_size =  buf->cap - 1 - nmoved;

  size_t n = fread(p, sizeof(char), read_size, fp);
  if (ferror(fp)) {
    perror("fread");
    return false;
  }
  p[n] = '\0';
  return mark_rest_point(buf, n);
}

void free_buf(struct Buf* buf)
{
  free(buf);
}
