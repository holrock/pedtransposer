#ifndef OPT_H_
#define OPT_H_

#include <ctype.h>
#include <stdbool.h>

struct Opt
{
  const char* file_name;
  size_t buf_size;
};

bool parse_opt(int argc, char** argv, struct Opt* opt);

#endif
