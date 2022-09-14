SRCS = tuple.c tx.c
SUB_DIR = include/silo/helper
SRCS := $(SRCS) $(wildcard *.c $(foreach fd, $(SUB_DIR), $(fd)/*.c))
OBJS = ${SRCS:.c=.o}
TEST := $(SRCS) test.c

libsilo.a: $(OBJS)
	ar rv $@ $?
	ranlib $@

test: $(TEST)
	gcc -pthread -o test $^

clean:
	rm *.o *.a test