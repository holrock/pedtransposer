#include "transpose.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "buf.h"
#include "rowtop.h"
#include "util.h"

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
int transpose(struct RowTop* rt, FILE* out, char delim)
{
  const size_t row_size = rt->size;
  const size_t ncol = count_column(rt->top[0]);

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
  return 0;
}

static
struct RowTop* prepare_buf(struct Buf* buf)
{
  char* p = buf->data;
  struct RowTop* rt = init_rowtop(1000);
  push_rowtop(rt, p);

  while (*p != '\0') {
    if (*p == '\n') {
      *p = '\0';
      if (*(p - 1) == '\r') {
        *(p - 1) = '\0';
      }
      if (*(p + 1) != '\0') {
        push_rowtop(rt, p + 1);
      }
    }
    ++p;
  }
  return rt;
}

static
int transpose_one(FILE* input, FILE* output, struct Buf* buf, char delim)
{
  if (!read_next(buf, input)) {
    return -1;
  }

  struct RowTop* rt = prepare_buf(buf);
  transpose(rt, output, delim);
  free_rowtop(rt);
  return 0;
}

static
int merge_multi_files(FILE* output, FILE** tmpfiles, size_t ntmpfiles)
{
  for (size_t i = 0; i < ntmpfiles; ++i) {
    if (fseek(tmpfiles[i], 0L, SEEK_SET) != 0) {
      perror("fseek");
      return -1;
    }
  }

  char* buf = NULL;
  size_t nbuf = 0;
  int ret = 0;
  for (;;) {
    for (size_t i = 0; i < ntmpfiles; ++i) {
      ssize_t n = getline(&buf, &nbuf, tmpfiles[i]);
      if (n < 0) {
        if (errno != 0) {
          ret = -1;
          perror("getline");
        }
        goto EXIT;
      }
      buf[n-1] = '\0';
      fputs(buf, output);
      if (i == ntmpfiles - 1) {
        fputc('\n', output);
      } else {
        fputc('\t', output);
      }
    }
  }

  for (size_t i = 0; i < ntmpfiles; ++i) {
    if (!feof(tmpfiles[i])) {
      fprintf(stderr, "move_forward_rest_data: different temp file size\n");
    }
  }
EXIT:
  free(buf);
  return ret;
}

static
int transpose_multi(FILE* input, FILE* output, struct Buf* buf, char delim)
{
  size_t alloc_size = 10;
  FILE** tmpfiles = xmalloc(sizeof(FILE*) * alloc_size);
  size_t current_tmp = 0;
  int ret = 0;

  while (!feof(input)) {
    if (!read_next(buf, input)) {
      ret = -1;
      goto EXIT;
    }

    tmpfiles[current_tmp] = tmpfile();
    if (!tmpfiles[current_tmp]) {
      perror("tmpfile");
      ret = -1;
      goto EXIT;
    }
    struct RowTop* rt = prepare_buf(buf);
    int r = transpose(rt, tmpfiles[current_tmp], delim);
    free_rowtop(rt);
    ++current_tmp;
    if (r != 0) {
      ret = r;
      goto EXIT;
    }
    if (current_tmp > alloc_size) {
      alloc_size += 10;
      FILE** fs = realloc(tmpfiles, alloc_size);
      if (!fs) {
        ret = -1;
        goto EXIT;
      }
      tmpfiles = fs;
    }
  }
  
  ret =  merge_multi_files(output, tmpfiles, current_tmp);

EXIT:
  for (size_t i = 0; i < current_tmp; ++i) {
    fclose(tmpfiles[i]);
  }
  free(tmpfiles);
  return ret;
}

int transpose_ped(FILE* input, FILE* output, struct Buf* buf, char delim)
{
  if (!input || !output) {
    fprintf(stderr, "transpose_ped: invalid FILE*\n");
    return -1;
  }
  if (!buf) {
    fprintf(stderr, "transpose_ped: invalid buffer\n");
    return -1;
  }

  if (buf->file_size < buf->cap) {
    return transpose_one(input, output, buf, delim);
  }
  return transpose_multi(input, output, buf, delim);
}
