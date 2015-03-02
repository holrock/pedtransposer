#include <stdio.h>
#include <assert.h>

#include "../rowtop.h"
#include "../buf.h"
#include "../util.h"

void test_buf()
{
  { // set buf size
    struct Buf buf = init_buf("", 10);
    assert(buf.size == 10);
    assert(buf.data);
    free_buf(&buf);
  }

  { // size form file
    // file size + 1 byte
    struct Buf buf = init_buf("data/buf.data", 0);
    assert(buf.size == 6);
    assert(buf.data);
    free_buf(&buf);
  }

  { // set both param
    struct Buf buf = init_buf("data/buf.data", 100);
    assert(buf.size == 100);
    assert(buf.data);
    free_buf(&buf);
  }
}

void test_rowtop()
{
  { // initialize
    struct RowTop* rt = init_rowtop(10);
    assert(rt->size == 0);
    assert(rt->cap == 10);
    assert(rt->top);
    free_rowtop(rt);
  }

  { // push
    struct RowTop* rt = init_rowtop(2);
    char hoge[] = "hoge";
    push_rowtop(rt, &hoge[0]);
    push_rowtop(rt, &hoge[1]);
    push_rowtop(rt, &hoge[2]);
    assert(rt->size == 3);
    assert(rt->cap == 4);
    assert(rt->top[0] == &hoge[0]);
    assert(rt->top[1] == &hoge[1]);
    assert(rt->top[2] == &hoge[2]);
    free_rowtop(rt);
  }
}

void test_util()
{
  { // xstrtul
    unsigned long u = xstrtoul("10");
    assert(u == 10UL);
  }
  { // get_file_size
    assert(get_file_size("data/buf.data") == 5);
  }
  {
    void* p = xmalloc(10);
    assert(p);
    free(p);
  }
}

int main(void)
{
  fprintf(stderr, "start test\n");
  test_buf();
  test_rowtop();
  test_util();
  fprintf(stderr, "end test\n");
  return 0;
}
