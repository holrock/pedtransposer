#ifndef ROWTOP_H_
#define ROWTOP_H_

#include <stddef.h>

struct RowTop
{
  char** top;
  size_t size;
  size_t cap;
};

struct RowTop* init_rowtop(size_t init_size);
void grow_rowtop(struct RowTop* rt);
void push_rowtop(struct RowTop* rt, char* p);
void free_rowtop(struct RowTop* rt);

#endif
