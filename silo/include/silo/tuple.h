#ifndef PASTE_TUPLE_H
#define PASTE_TUPLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include <stdatomic.h>

typedef int key;
typedef int value;

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
#define TUPLE_NUM 10000
#define THREAD_NUM 4

struct tuple{
	alignas(CACHE_LINE_SIZE) struct tid_word tid_word;
	value value;
};

typedef uint32_t epoch_t;
_Atomic extern epoch_t epoch;
alignas(CACHE_LINE_SIZE) extern uint64_t *thread_local_epochs;
alignas(CACHE_LINE_SIZE) extern struct tuple *table;


#endif // PASTE_TUPLE_H
