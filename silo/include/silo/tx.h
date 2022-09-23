#ifndef PASTE_TX_H
#define PASTE_TX_H

#include "silo.h"

struct read_operation{
	key key;
	struct tid_word tid_word;
};

struct write_operation{
	key key;
	struct value value;
	struct tuple *ptr;
};

struct tx{
	struct silo* silo;
	struct read_operation reads[10];
	ssize_t num_read;
	struct write_operation writes[10];
	ssize_t num_write;

	struct tid_word
	    max_read_tid,
	    max_write_tid,
	    most_recently_chosen_tid;
};

enum result{
	commited, aborted
};

void tx_init(struct silo*, struct tx*);
const struct value tx_read(struct tx*, key);
void tx_write(struct tx*, key, struct value);
enum result tx_commit(struct tx*);

#endif // PASTE_TX_H
