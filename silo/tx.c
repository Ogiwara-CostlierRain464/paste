#include "include/silo/tx.h"
#include "include/silo/helper/tlpi_hdr.h"
#include <stdatomic.h>
#include <assert.h>

void tx_init(struct silo* silo, struct tx* t){
	memset(t, 0, sizeof(struct tx));
	t->silo = silo;
}

const struct value tx_read(struct tx* tx,key key){
	struct silo *s = tx->silo;
	struct tid_word before, after;
	struct value data;

	struct tuple *t = &s->table[key];

	for(;;){
		do{
			before.body = atomic_load(&t->tid_word.body);
		} while(before.lock);

		data.body = t->body;
		data.len = t->body_len;

		after.body = atomic_load(&t->tid_word.body);

		if(before.body ==  after.body){
			break;
		}else{
			continue;
		}
	}
	tx->reads[tx->num_read] = (struct read_operation){
	    .key = key,
	    .tid_word = after
	};
	tx->num_read++;
	return data;
}

void tx_write(struct tx* tx, key key, struct value val){
	// assert not to re-write.
	assert(val.len <= BODY_SIZE);
	struct silo *s = tx->silo;

	struct tuple *t = &s->table[key];
	tx->writes[tx->num_write] = (struct write_operation){
	    .key = key,
	    .value = val,
	    .ptr = t
	};
	tx->num_write++;
}

void tx_lock_write_set(struct tx* tx);
bool tx_exist_in_write_set(struct tx* tx, struct tuple* t);
void tx_unlock_write_set(struct tx* tx);

int compare_write(const void* a_, const void* b_){
	struct write_operation* a = (struct write_operation*) a_ ;
	struct write_operation* b = (struct write_operation*) b_ ;

	if(a->key == b->key) return 0;
	else if(a->key < b->key) return -1;
	else return 1;
}

enum result tx_commit(struct tx* tx){
	struct silo *s = tx->silo;
	qsort(tx->writes, tx->num_write, sizeof(struct write_operation), compare_write);
	tx_lock_write_set(tx);

	atomic_thread_fence(memory_order_acquire);
	_Atomic epoch_t e = atomic_load(&s->epoch);
	atomic_thread_fence(memory_order_release);

	for(size_t i = 0; i < tx->num_read; i++){
		struct read_operation *op = &tx->reads[i];
		struct tuple *t = &s->table[op->key];

		struct tid_word when_read = op->tid_word;
		struct tid_word now;
		now.body = atomic_load(&t->tid_word.body);

		if(now.tid != when_read.tid
		    || !now.latest
		    || (now.lock && !tx_exist_in_write_set(tx, t))
		    || now.epoch != when_read.epoch){
			tx_unlock_write_set(tx);
			return aborted;
		}

		tx->max_read_tid.body = max(tx->max_read_tid.body, now.body);
	}

	struct tid_word a, b, c;
	a.body = max(tx->max_read_tid.body, tx->max_write_tid.body);
	a.tid++;

	b.body = tx->most_recently_chosen_tid.body;
	b.tid++;

	c.epoch = e;

	struct tid_word max;
	max.body = max(max(a.body, b.body), c.body);
	max.lock = false;
	max.latest = true;
	tx->most_recently_chosen_tid = max;

	for(size_t i = 0; i < tx->num_write; i++){
		memcpy(tx->writes[i].ptr->body, tx->writes[i].value.body, tx->writes[i].value.len);
		tx->writes[i].ptr->body_len = tx->writes[i].value.len;
		atomic_store(&tx->writes[i].ptr->tid_word.body, max.body);
	}

	return commited;
}

void tx_lock_write_set(struct tx* tx){
	// assume write set has sorted.
	struct silo *s = tx->silo;
	struct tid_word expected, desired;
	for(size_t i = 0; i < tx->num_write; i++){
		key k = tx->writes[i].key;
		expected.body = atomic_load(&s->table[k].tid_word.body);

		for(;;){
			if(expected.lock){
				expected.body = atomic_load(&s->table[k].tid_word.body);
			}else{
				desired.body = expected.body;
				desired.lock = true;
				if(atomic_compare_exchange_weak(&s->table[k].tid_word.body, &expected.body, desired.body)){
					break;
				}
			}
		}

		tx->max_write_tid.body = max(tx->max_write_tid.body, expected.body);
	}
}

void tx_unlock_write_set(struct tx* tx){
	struct tid_word expected, desired;

	for(size_t i = 0; i < tx->num_write; i++){
		expected.body = atomic_load(&tx->writes[i].ptr->tid_word.body);
		desired.body = expected.body;
		desired.lock = false;
		atomic_store(&tx->writes[i].ptr->tid_word.body, desired.body);
	}
}

bool tx_exist_in_write_set(struct tx* tx, struct tuple* t){
	for(size_t i = 0; i < tx->num_write; i++){
		if(tx->writes[i].ptr == t)
			return true;
	}
	return false;
}