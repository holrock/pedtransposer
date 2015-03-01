#ifndef UTIL_H_
#define UTIL_H_

#include <ctype.h>

unsigned long xstrtoul(const char* s);
size_t get_file_size(const char* s);
void* xmalloc(size_t size);

#endif
