#include "buf.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

struct Buf* init_buf(const char* file_name, size_t buf_size)
{
  struct Buf* buf = xmalloc(sizeof(struct Buf));
  buf->data = NULL;
  buf->size = 0;
  buf->rest_point = NULL;

  if (buf_size != 0) {
    buf->data = xmalloc(buf_size);
    buf->size = buf_size;
  } else {
    size_t size = get_file_size(file_name) + 1;
    buf->data = xmalloc(size);
    buf->size = size;
  }

  return buf;
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
  if (i == 0) {
    fprintf(stderr, "buffer size too small\n");
    return false;
  }
  return true;
}

bool read_next(struct Buf* buf, FILE* fp)
{
  if (!buf || !buf->data || buf->size == 0) {
    fprintf(stderr, "uninitialize buffer\n");
    return false;
  }

  size_t n = fread(buf->data, sizeof(char), buf->size, fp);
  if (ferror(fp)) {
    perror("fread");
    fclose(fp);
    return false;
  }
  buf->data[n + 1] = '\0';
  return mark_rest_point(buf, n + 1);
}

void free_buf(struct Buf* buf)
{
  free(buf->data);
  buf->data = NULL;
  buf->size = 0;
  buf->rest_point = NULL;
  free(buf);
}
