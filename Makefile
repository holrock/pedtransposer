.PHONY: clean


TARGET = pt
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)
CFLAGS = -std=c99 -g -Wall -Wextra -MMD -MP \
	-Wformat=2 -Wconversion \
	-Wstrict-aliasing=2 -Wdisabled-optimization \
	-Wfloat-equal \
	-Wpointer-arith \
	-Wdeclaration-after-statement \
	-Wbad-function-cast \
	-Wcast-align \
	-Wredundant-decls \
	-Winline \
	-Wno-unused-parameter

LDFLAGS =
INCLUDE =
OPT = -O2

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $(OPT) -o $@ -c $<

clean:
		$(RM) -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
