#include <assert.h>
#include <memory.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "include/silo/tx.h"
#include "include/silo/helper/error_functions.h"
#include "include/silo/xoroshiro_128plus.h"

double make_ave_double(double arr[], ssize_t len){
	double tmp = 0;
	for(ssize_t i = 0; i < len; i++) {
		tmp += arr[i] / (double) len;
	}
	return tmp;
}

double make_sum_double(double arr[], ssize_t len){
	double tmp = 0;
	for(ssize_t i = 0; i < len; i++) {
		tmp += arr[i];
	}
	return tmp;
}

double throughput(double sec, size_t iter){
	return ((double) iter) * (1. / sec);
}

atomic_bool wait = true;
struct silo s;
double throughput_per_thread[300];
ssize_t thread_num = 0;
#define TUPLE_NUM 1000000
#define LOOP_SEC 10

void *worker(void *_thread_id){
	size_t t_id = (size_t) _thread_id;
	struct xoroshiro_128plus r = init_xoroshiro_128plus(t_id);

	while (wait){;}
	ssize_t iter_count = 0;
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	for(;;){
		struct tx t;
		tx_init(&s, &t);
		struct value v;
		char a[] = {'0', '\0'};
		v.body = a;
		v.len = 2;
		char acc = 0;

		for(size_t i = 1; i <= 2; i++){ // read
			key k = next(&r) % TUPLE_NUM;
			acc += tx_read(&t, k, NULL).body[0];
		}

		for(size_t i = 1; i <= 3; i++){ // write
			key k = next(&r) % TUPLE_NUM;
			v.body[0] = i;
			tx_write(&t, k, v);
		}

		tx_commit(&t, NULL);

		clock_gettime(CLOCK_MONOTONIC, &end);

		iter_count++;

		double seconds = (double)(end.tv_sec - start.tv_sec);
		if(seconds > LOOP_SEC){
			break;
		}
	}
	throughput_per_thread[t_id] = throughput((double)(end.tv_sec - start.tv_sec), iter_count);

	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	thread_num = get_nprocs();
	int c;
	while ((c = getopt(argc, argv, "t:")) != -1){
		switch (c) {
		case 't':
			thread_num = atoi(optarg);
			break;
		}
	}

	printf("Thread num: %zd\n", thread_num);
	printf("Tuple num: %d\n", TUPLE_NUM);
	printf("Loop sec: %d\n", LOOP_SEC);

	init_silo(&s, thread_num, TUPLE_NUM);

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
			fprintf(stderr, "Thread %zu has been canceled.\n", i);
		}
	}

	double throughput_total = make_sum_double(throughput_per_thread, thread_num);
	printf("Total throughput: %f\n", throughput_total);
	pthread_exit(NULL);
}