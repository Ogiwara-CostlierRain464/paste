#ifndef PASTE_SILO_H
#define PASTE_SILO_H

#include "tuple.h"

struct silo{
	_Atomic epoch_t epoch;
	alignas(CACHE_LINE_SIZE) uint64_t *thread_local_epochs;
	struct tuple *table;
};

void init_silo(struct silo*,ssize_t thread_num, ssize_t tuple_num);

#endif // PASTE_SILO_H
