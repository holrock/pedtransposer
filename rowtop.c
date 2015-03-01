#include "rowtop.h"

#include <stdio.h>
#include <stdlib.h>

struct RowTop* init_rowtop(size_t init_size)
{
  struct RowTop* rt = malloc(sizeof(struct RowTop));
  rt->top = malloc(sizeof(char*) * init_size);
  if (!rt->top) {
    fprintf(stderr, "malloc error\n");
    exit(-1);
  }
  rt->size = 0;
  rt->cap = init_size;
  return rt;
}

void grow_rowtop(struct RowTop* rt)
{
  char** p = realloc(rt->top, sizeof(char*) * (rt->cap * 2));
  if (!p) {
    fprintf(stderr, "realloc error\n");
    exit(-1);
  }
  rt->top = p;
  rt->cap *= 2;
}

void push_rowtop(struct RowTop* rt, char* p)
{
    if (rt->size == rt->cap) {
      grow_rowtop(rt);
    }
    rt->top[rt->size] = p;
    ++rt->size;
}

void free_rowtop(struct RowTop* rt)
{
  free(rt->top);
  rt->top = NULL;
  free(rt);
}

