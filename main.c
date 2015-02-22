#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

const int N_COL_HEADER = 6;

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
    fprintf(stderr, "buffer empty");
    return;
  }

  const int ncol_header = 6; 
  int ncol = count_column(buf);
  int ngeno_col = ncol - ncol_header;

  for (int cur_col = 0; cur_col < N_COL_HEADER; ++cur_col) {
    put_col(buf, out, delim, cur_col, ngeno_col);
  }
}

int main(int argc, char** argv)
{
  if (argc < 2)
    return -1;

  size_t buf_size = 1024 * 1024 * 1024; // 1GiB
  char* buf = malloc(buf_size);
  if (!buf) {
    fprintf(stderr, "malloc error");
    return -1;
  }

  char* ped = argv[1];
  FILE* fp = fopen(ped, "rb");
  if (!fp) {
    perror(ped);
    return -1;
  }

  while (!feof(fp)) {
    size_t n = fread(buf, sizeof(char), buf_size-1, fp);
    if (ferror(fp)) {
      perror(ped);
      return -1;
    }

    buf[n + 1] = '\0';
    transpose(buf, stdout, '\t');
  }

  free(buf);
  fclose(fp);
  return 0;
}
