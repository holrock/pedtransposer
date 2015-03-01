#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int N_COL_HEADER = 6;

struct Opt
{
  const char* file_name;
  size_t buf_size;
};

struct Buf
{
  char* data;
  size_t size;
};


static
unsigned long xstrtoul(const char* s)
{
  char* endp = NULL;
  errno = 0;
  unsigned long val = strtoul(s, &endp, 10);
  if ((errno == ERANGE && (val == ULONG_MAX || val == 0)) || (errno != 0 && val == 0)) {
    perror("strtoul");
    exit(-1);
  }

  if (endp == s) {
    fprintf(stderr, "strtoul error: %s\n", s);
    exit(-1);
  }
  return val;
}

static
size_t get_file_size(const char* s)
{
  struct stat st = {};
  if (stat(s, &st) == -1) {
    perror("stat");
    exit(-1);
  }
  if (st.st_size < 1) {
    fprintf(stderr, "invalid file size\n");
    exit(-1);
  }
  return (size_t)st.st_size;
}

static inline
bool is_eos(char c)
{
  return (c == '\n' || c == '\r' || c == '\0');
}

static
int count_column(const char* buf)
{
  int ncol = 0;
  while (!is_eos(*buf)) {
    if (isblank(*buf)) {
        ++ncol;
        if (ncol == N_COL_HEADER)
          break;
    }
    ++buf;
  }

  int nspace = 0;
  int ntab = 0;
  while (!is_eos(*buf)) {
    if (*buf == ' ')
      ++nspace;
    else if (*buf == '\t')
        ++ntab;
    ++buf;
  }
     
  if (nspace == 0)
    return ncol + 1;
  if (ntab != 0)
    return ncol + ntab + 1;
  return ncol + (nspace / 2 + 1) + 1;
}

static
void put_col_header(char* buf, FILE* out, char delim, int target_col, int ngeno_col)
{
  char* p = buf;
  int col = 0;

  while (*p != '\0') {
    while (!is_eos(*p)) {
      if (isblank(*p)) {
        if (col == target_col)
          fputc(delim, out);
        ++col;
      } else if (col == target_col) {
        fputc(*p, out);
      }

      ++p;
    }
    col = 0;
    ++p;
  }
  fputc('\n', out);
}

static
void put_col_geno(char* buf, FILE* out, char delim, int target_col, int ngeno_col)
{
}

static
void put_col(char* buf, FILE* out, char delim, int target_col, int ngeno_col)
{
  if (target_col < N_COL_HEADER)
    put_col_header(buf, out, delim, target_col, ngeno_col);
  else
    put_col_geno(buf, out, delim, target_col, ngeno_col);
}

static
void transpose(char* buf, FILE* out, char delim)
{
  if (!buf || buf[0] == '\0') {
    fprintf(stderr, "invalid buffer\n");
    return;
  }

  const int ncol_header = 6; 
  int ncol = count_column(buf);
  int ngeno_col = ncol - ncol_header;

  for (int cur_col = 0; cur_col < N_COL_HEADER; ++cur_col) {
    put_col(buf, out, delim, cur_col, ngeno_col);
  }
}

static
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
    fprintf(stderr, "buf isze %lu\n", buf.size);
  }

  if (!buf.data) {
    fprintf(stderr, "malloc error\n");
    exit(-1);
  }
  return buf;
}

static
int start_transpose(const char* file_name, struct Buf buf)
{
  FILE* fp = fopen(file_name, "rb");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  while (!feof(fp)) {
    size_t n = fread(buf.data, sizeof(char), buf.size, fp);
    if (ferror(fp)) {
      perror("fread");
      fclose(fp);
      return -1;
    }
    fprintf(stderr, "read isze %lu\n", n);

    buf.data[n + 1] = '\0';
    transpose(buf.data, stdout, '\t');
  }
  fclose(fp);
  return 0;
}

static
struct Opt parse_opt(int argc, char** argv)
{
  struct Opt opt = {};

  for (int i = 1; i < argc - 1; ++i) {
    if (strcmp(argv[i], "--file") == 0) {
      opt.file_name = argv[i + 1];
      ++i;
    } else if (strcmp(argv[i], "--buf") == 0) {
      opt.buf_size = xstrtoul(argv[i + 1]);
      ++i;
    }
  }
  if (!opt.file_name) {
    fprintf(stderr, "empty file name\n");
    exit(-1);
  }
  return opt;
}

int main(int argc, char** argv)
{
  struct Opt opt = parse_opt(argc, argv);
  struct Buf buf = init_buf(opt.file_name, opt.buf_size);
  int status = start_transpose(opt.file_name, buf);
  free(buf.data);
  return status;
}
