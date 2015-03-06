#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
    fprintf(stderr, "xstrtoul: strtoul error: %s\n", s);
    exit(-1);
  }
  return val;
}

size_t get_file_size(const char* s)
{
  struct stat st;
  if (stat(s, &st) == -1) {
    perror("stat");
    exit(-1);
  }
  if (st.st_size < 1) {
    fprintf(stderr, "get_file_size: invalid file size\n");
    exit(-1);
  }
  return (size_t)st.st_size;
}

void* xmalloc(size_t size)
{
  void* p = malloc(size);
  if (!p) {
    fprintf(stderr, "xmalloc: malloc error\n");
    exit(-1);
  }
  return p;
}
