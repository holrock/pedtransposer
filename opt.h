#ifndef OPT_H_
#define OPT_H_

#include <stddef.h>
#include <stdbool.h>

struct Opt
{
  const char* file_name;
  const char* out_name;
  size_t buf_size;
  char delim;
};

bool parse_opt(int argc, char** argv, struct Opt* opt);

#endif
