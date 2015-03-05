#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../rowtop.h"
#include "../buf.h"
#include "../util.h"
#include "../opt.h"
#include "../transpose.h"

void test_buf()
{
  { // set buf size
    struct Buf* buf = init_buf(10, 0);
    assert(buf->size == 10);
    assert(buf->file_size == 0);
    assert(buf->data);
    assert(!buf->rest_point);
    free_buf(buf);
  }

  { // size form file
    // file size + 1 byte
    struct Buf* buf = init_buf(0, 5);
    assert(buf->size == 6);
    assert(buf->file_size == 5);
    assert(buf->data);
    free_buf(buf);
  }

  { // set both param, use size param
    struct Buf* buf = init_buf(100, 10);
    assert(buf->size == 100);
    assert(buf->file_size == 10);
    assert(buf->data);
    free_buf(buf);
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

void test_opt()
{
  { // parse opt
    struct Opt opt = {};
    char* argv[7] = {
      "",
      "--file",
      "hoge",
      "--buf",
      "10"
    };
    assert(parse_opt(5, argv, &opt));
    assert(strcmp(opt.file_name, "hoge") == 0);
    assert(opt.buf_size == 10);
  }

  { // empty opt
    struct Opt opt = {};
    char* argv[1] = {
      "",
    };
    assert(!parse_opt(1, argv, &opt));
    assert(opt.file_name == NULL);
    assert(opt.buf_size == 0);
  }

  { // invalid opt
    struct Opt opt = {};
    char* argv[7] = {
      "",
      "--file",
    };
    assert(!parse_opt(2, argv, &opt));
    assert(opt.file_name == NULL);
    assert(opt.buf_size == 0);
  }
}

void test_transpose()
{
  { // space separete
    char* str = "0 1 2 3 4 5 6 A A G T";
    assert(count_column(str) == 9);
  }

  { // tab separete
    char* str = "0\t1\t2\t3\t4\t5\t6\tA A\tG T";
    assert(count_column(str) == 9);
  }

  { // multiple space separete
    char* str = "0  1  2  3  4 5 6  A A G T";
    assert(count_column(str) == 9);
  }

  { // multiple tab separete
    char* str = "0\t\t1\t\t2\t\t3\t\t4\t\t5\t\t6\t\tA A\tG T";
    assert(count_column(str) == 9);
  }
}

int main(void)
{
  fprintf(stderr, "start test\n");
  test_buf();
  test_rowtop();
  test_util();
  test_opt();
  test_transpose();
  fprintf(stderr, "end test\n");
  return 0;
}
