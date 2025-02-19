#include <assert.h>
#include <memory.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include "include/silo/tx.h"
#include "include/silo/helper/error_functions.h"
#include "include/silo/xoroshiro_128plus.h"

atomic_bool wait = true;

struct silo s;

void *worker(void *thread_id){
	int t_id = (int) thread_id;
	struct xoroshiro_128plus rand = init_xoroshiro_128plus(t_id);

	while (wait){;}
	struct value buf;
	char c_buf;
	buf.body = &c_buf;
	buf.len = 1;

	ssize_t num_commited = 0;

	for(size_t i = 0; i < 1000000; i++){
		struct tx t;
		tx_init(&s, &t);
		struct value v = tx_read(&t, next(&rand) % 10000, NULL); // do not edit this value
		*buf.body = v.body[0];
		(*buf.body)++;
		tx_write(&t, next(&rand) % 10000, buf);
		enum result r = tx_commit(&t, NULL);
		if(r == commited)
			num_commited++;
	}

	printf("Num commited: %zd\n", num_commited);

	pthread_exit(NULL);
}

int main(){
	int thread_num = get_nprocs();
	printf("Thread num: %d", thread_num);

	// thread num
	// later: read_write count

	init_silo(&s, thread_num, 10000);

	pthread_t threads[thread_num];
	for(size_t i = 0; i < thread_num; i++){
		if(pthread_create(&threads[i], NULL, worker, (void*)i))
			errExit("pthread_create");
	}

	atomic_store(&wait, false);

	for(size_t i = 0; i < thread_num; i++){
		void *ret_val;
		pthread_join(threads[i], &ret_val);
		if(ret_val == PTHREAD_CANCELED){
			printf("Thread %zu has been canceled.\n", i);
		}else{
			printf("Thread %zu return code: %ld\n", i, ((intptr_t)ret_val));
		}
	}
	printf("Final value of key 1: %d\n", s.table[1].body[0]);
	pthread_exit(NULL);
}