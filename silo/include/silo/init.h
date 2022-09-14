#ifndef PASTE_INIT_H
#define PASTE_INIT_H

#include "tuple.h"
#include <string.h>

void silo_init(){
	epoch = 1;
	thread_local_epochs = aligned_alloc(CACHE_LINE_SIZE , THREAD_NUM * sizeof(uint64_t));
	table = aligned_alloc(CACHE_LINE_SIZE, TUPLE_NUM * sizeof(struct tuple));

	for(size_t i = 0; i < TUPLE_NUM ; i++){
		struct tuple *t = &table[i];
		t->tid_word.epoch = 1;
		t->tid_word.latest = true;
		t->tid_word.lock = false;
		memset(t->body, 0, sizeof(t->body));
		t->body_len = 0;
	}
}

#endif // PASTE_INIT_H
