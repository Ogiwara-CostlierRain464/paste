#ifndef PASTE_INIT_H
#define PASTE_INIT_H

#include "tuple.h"
#include <string.h>

void silo_init(ssize_t thread_num, size_t tuple_num){
	epoch = 1;
	thread_local_epochs = aligned_alloc(CACHE_LINE_SIZE , thread_num * sizeof(uint64_t));
	table = aligned_alloc(CACHE_LINE_SIZE, tuple_num * sizeof(struct tuple));

	for(size_t i = 0; i < tuple_num ; i++){
		struct tuple *t = &table[i];
		t->tid_word.epoch = 1;
		t->tid_word.latest = true;
		t->tid_word.lock = false;
		memset(t->body, 0, sizeof(t->body));
		t->body_len = 0;
	}
}

#endif // PASTE_INIT_H
