SRCS = silo.c tx.c
SRCS := $(SRCS) include/silo/helper/error_functions.c include/silo/helper/get_num.c include/silo/helper/read_line.c
OBJS = ${SRCS:.c=.o}
TEST := $(SRCS) test.c xoroshiro_128plus.h
PERFORMANCE := $(SRCS) performance.c xoroshiro_128plus.h
UNIT := $(SRCS) unit.c

PROG = test performance unit

all: $(PROG)

libsilo.a: $(OBJS)
	ar rv $@ $?
	ranlib $@

silo.o: silo.c
	gcc -g -c -o silo.o silo.c

tx.o: tx.c
	gcc -g -c -o tx.o tx.c

test: $(TEST)
	gcc -pthread -o test $^

performance: $(PERFORMANCE)
	gcc -pthread -Wno-pointer-to-int-cast -g -o performance $^

unit: $(UNIT)
	gcc -g -o unit $^

clean:
	-rm -f *.o *.a test performance unit