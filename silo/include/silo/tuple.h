#ifndef PASTE_TUPLE_H
#define PASTE_TUPLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include <stdatomic.h>

struct value{
	char *body;
	size_t len; // <= BODY_SIZE
};

typedef uint64_t key;

struct tid_word{
	union {
		_Atomic uint64_t body;
		struct {
			bool lock: 1;
			bool latest: 1;
			bool absent: 1;
			uint64_t tid: 29;
			uint64_t epoch: 32;
		};
	};
};

#define CACHE_LINE_SIZE 64
#define BODY_SIZE 2048
#define MAX_READ_SIZE 50
#define MAX_WRITE_SIZE 50

struct tuple{
	alignas(CACHE_LINE_SIZE) struct tid_word tid_word;
	char body[BODY_SIZE];
	size_t body_len; // <= BODY_SIZE
};

typedef uint32_t epoch_t;


#endif // PASTE_TUPLE_H
