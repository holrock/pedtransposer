#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "buf.h"
#include "opt.h"
#include "transpose.h"

int main(int argc, char** argv)
{
  struct Opt opt = {};
  if (!parse_opt(argc, argv, &opt)) {
    exit(-1);
  }
  struct Buf buf = init_buf(opt.file_name, opt.buf_size);
  int status = transpose_ped(opt.file_name, &buf);
  free_buf(&buf);
  return status;
}
