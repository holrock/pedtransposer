#include "transpose.h"

#include <stdio.h>
#include <string.h>

#include "buf.h"
#include "rowtop.h"

const int N_COL_HEADER = 6;

size_t count_column(const char* buf)
{
  size_t ncol = 0;
  while (*buf != '\0') {
    if (isblank(*buf)) {
        ++ncol;
        while (isblank(*buf)) {
          ++buf;
        }
        if (ncol == N_COL_HEADER) {
          break;
        }
    } else {
      ++buf;
    }
  }

  size_t nspace = 0;
  size_t ntab = 0;
  while (*buf != '\0') {
    if (*buf == ' ') {
      ++nspace;
      while (*buf == ' ') {
        ++buf;
      }
    } else if (*buf == '\t') {
        ++ntab;
        while (*buf == '\t') {
          ++buf;
        }
    } else {
      ++buf;
    }
  }
     
  if (nspace == 0)
    return ncol + 1;
  if (ntab != 0)
    return ncol + ntab + 1;
  return ncol + (nspace / 2 + 1);
}

static
void transpose(char* buf, struct RowTop* rt, FILE* out, char delim)
{
  if (!buf || buf[0] == '\0') {
    fprintf(stderr, "invalid buffer\n");
    return;
  }

  const size_t row_size = rt->size;
  const size_t ncol = count_column(buf);

  for (size_t cur_col = 0; cur_col < N_COL_HEADER; ++cur_col) {
    for (size_t i = 0; i < row_size; ++i) {
      char* p = rt->top[i];
      size_t n = 0;
      while (!isblank(*p)) {
        ++p;
        ++n;
      }
      fwrite(rt->top[i], sizeof(char), n, out);

      while (isspace(*p)) {
        ++p;
      }
      rt->top[i] = p;
      if (i + 1 < row_size)
        fputc(delim, out);
    }
    fputc('\n', out);
  }

  for (size_t cur_col = N_COL_HEADER; cur_col < ncol; ++cur_col) {
    for (size_t i = 0; i < row_size; ++i) {
      char* p = rt->top[i];
      fwrite(p, sizeof(char), 3, out);
      p += 4;
      while (isspace(*p)) {
        ++p;
      }
      rt->top[i] = p;

      if (i + 1 < row_size)
        fputc(delim, out);
    }
    fputc('\n', out);
  }
}

static
struct RowTop* prepare_buf(struct Buf* buf)
{
  char* p = buf->data;
  struct RowTop* rt = init_rowtop(1000);
  push_rowtop(rt, p);

  while (*p != '\0') {
    if (*p == '\n' || *p == '\r') {
      *p = '\0';
      if (*(p + 1) != '\0') {
        push_rowtop(rt, p + 1);
      }
    }
    ++p;
  }
  return rt;
}

int transpose_ped(const char* file_name, struct Buf* buf)
{
  FILE* fp = fopen(file_name, "rb");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  while (!feof(fp)) {
    size_t n = fread(buf->data, sizeof(char), buf->size, fp);
    if (ferror(fp)) {
      perror("fread");
      fclose(fp);
      return -1;
    }
    buf->data[n + 1] = '\0';

    struct RowTop* rt = prepare_buf(buf);
    transpose(buf->data, rt, stdout, '\t');
    free_rowtop(rt);
    rt = NULL;
  }
  fclose(fp);
  return 0;
}

