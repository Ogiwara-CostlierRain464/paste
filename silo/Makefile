SRCS = silo.c
OBJS = ${SRCS:.c=.o}

libsilo.a: $(OBJS)
	ar rv $@ $?
	ranlib $@

clean:
	rm *.o *.a