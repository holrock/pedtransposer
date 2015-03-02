#include "opt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

bool parse_opt(int argc, char** argv, struct Opt* opt)
{
  char * fname = NULL;
  size_t bsize = 0;
  for (int i = 1; i < argc - 1; ++i) {
    if (strcmp(argv[i], "--file") == 0) {
      fname = argv[i + 1];
      ++i;
    } else if (strcmp(argv[i], "--buf") == 0) {
      bsize = xstrtoul(argv[i + 1]);
      ++i;
    }
  }
  if (!fname) {
    fprintf(stderr, "empty file name\n");
    return false;
  }
  opt->file_name = fname;
  opt->buf_size = bsize;
  return true;
}
