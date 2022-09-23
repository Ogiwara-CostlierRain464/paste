#include <assert.h>
#include <memory.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include "include/silo/tx.h"
#include "include/silo/helper/error_functions.h"

atomic_bool wait = true;

struct silo s;

void *worker(void *thread_id){
	while (wait){;}
	struct value buf;
	char c_buf;
	buf.body = &c_buf;
	buf.len = 1;

	ssize_t num_commited = 0;
	for(size_t i = 0; i < 20; i++){
		struct tx t;
		tx_init(&s, &t);
		struct value v = tx_read(&t, 1); // do not edit this value
		*buf.body = v.body[0];
		(*buf.body)++;
		tx_write(&t, 1, buf);
		enum result r = tx_commit(&t);
		if(r == commited)
			num_commited++;
	}

	printf("Num commited: %zd\n", num_commited);

	pthread_exit(NULL);
}

int main(){
	int thread_num = get_nprocs();
	printf("Thread num: %d", thread_num);

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