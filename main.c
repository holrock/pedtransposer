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

  FILE* fp = fopen(opt.file_name, "rb");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  size_t file_size = get_file_size(opt.file_name);
  struct Buf* buf = init_buf(opt.buf_size, file_size);

  int status = transpose_ped(fp, buf);

  fclose(fp);
  free_buf(buf);
  buf = NULL;
  return status;
}
