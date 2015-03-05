#include "opt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

bool parse_opt(int argc, char** argv, struct Opt* opt)
{
  char * fname = NULL;
  char* out = NULL;
  size_t bsize = 0;
  char delim = '\t';

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--file") == 0) {
      if (i < argc - 1) {
        fname = argv[i + 1];
        ++i;
      }
    } else if (strcmp(argv[i], "--out") == 0) {
      if (i < argc - 1) {
        out = argv[i + 1];
        ++i;
      }
    } else if (strcmp(argv[i], "--buf") == 0) {
      if (i < argc - 1) {
        bsize = xstrtoul(argv[i + 1]);
        ++i;
      }
    } else if (strcmp(argv[i], "--space") == 0) {
      delim = ' ';
    } else {
      fprintf(stderr, "%s: --file --out [--buf]  [--space]\n", argv[0]);
      fprintf(stderr, "\t--file\tinput ped file path\n");
      fprintf(stderr, "\t--out\toutput ped file path\n");
      fprintf(stderr, "\t--buf\tlimit buffer size(MiB). default: allocate input file size\n");
      fprintf(stderr, "\t--space\toutput delimite space. default: tab\n");
      exit(-1);
    }
  }
  if (!fname || !out) {
    fprintf(stderr, "empty file name\n");
    return false;
  }
  opt->file_name = fname;
  opt->out_name = out;
  opt->buf_size = bsize * 1024 * 1024;
  opt->delim = delim;
  return true;
}
