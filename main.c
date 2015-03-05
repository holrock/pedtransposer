#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "buf.h"
#include "opt.h"
#include "transpose.h"
#include "util.h"

int main(int argc, char** argv)
{
  struct Opt opt = {};
  if (!parse_opt(argc, argv, &opt)) {
    exit(-1);
  }

  size_t file_size = get_file_size(opt.file_name);
  struct Buf* buf = init_buf(opt.buf_size, file_size);

  if (file_size / buf->cap > 10) {
    fprintf(stderr, "warn: buffer size is small. uses a lot of temporary files, the program will be very slow.\n");
  }

  FILE* fp = fopen(opt.file_name, "rb");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  int status = transpose_ped(fp, stdout, buf, '\t');

  fclose(fp);
  free_buf(buf);
  return status;
}
