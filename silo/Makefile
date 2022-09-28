SRCS = silo.c tx.c
SUB_DIR = include/silo/helper
SRCS := $(SRCS) $(wildcard *.c $(foreach fd, $(SUB_DIR), $(fd)/*.c))
OBJS = ${SRCS:.c=.o}
TEST := $(SRCS) test.c

libsilo.a: $(OBJS)
	ar rv $@ $?
	ranlib $@

silo.o: silo.c
	gcc -g -c -o silo.o silo.c

tx.o: tx.c
	gcc -g -c -o tx.o tx.c

test: $(TEST)
	gcc -pthread -o test $^

clean:
	-rm -f *.o *.a test