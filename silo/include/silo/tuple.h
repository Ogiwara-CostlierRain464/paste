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
#define BODY_SIZE 1024

struct tuple{
	alignas(CACHE_LINE_SIZE) struct tid_word tid_word;
	char body[BODY_SIZE];
	size_t body_len; // <= BODY_SIZE
};

typedef uint32_t epoch_t;
_Atomic extern epoch_t epoch;
alignas(CACHE_LINE_SIZE) extern uint64_t *thread_local_epochs;
alignas(CACHE_LINE_SIZE) extern struct tuple *table;


#endif // PASTE_TUPLE_H
