#include "include/silo/silo.h"
#include <assert.h>
#include <string.h>

void init_silo(
    struct silo* s,
    ssize_t thread_num,
    ssize_t tuple_num){
	assert(s != NULL);
	s->epoch = 1; // TODO make epoch advance thread
	s->thread_local_epochs =
	    aligned_alloc(CACHE_LINE_SIZE, thread_num * sizeof(uint64_t));
	s->table =
	    aligned_alloc(CACHE_LINE_SIZE, tuple_num * sizeof(struct tuple));

	for(size_t i = 0; i < tuple_num; i++){
		struct tuple *t = &s->table[i];
		t->tid_word.epoch = 1;
		t->tid_word.latest = true;
		t->tid_word.lock = false;
		memset(t->body, 0, sizeof(t->body));
		t->body_len = 0;
	}
}